/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fee.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:27MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fee.c  $
 *  Revision 1.3 2011/02/02 09:17:27MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 14:22:32MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "Fee.h"
#include "Fee_Priv.h"
#include "Fee_Cbk.h"



/* state variable for master state machine */
Fee_StateType fee_state;

/* temporary storage required due to the AutoSAR Fls driver specification */
static uint32 fee_page[FEE_PAGE_SIZE_IN_WORDS];

/* segment information; holds everything that is related to the current state */
/* of the segment. This information can be recovered anytime from the load pages. */
Fee_SegInformationType fee_seg_info[FEE_NUM_OF_SEGMENTS];

/* holds the pending command with its parameters */
Fee_CmdType fee_cmd;

/* result of the last job */
MemIf_JobResultType fee_job_result;


#define FEE_START_SEC_CODE
#include "MemMap.h"

static uint8 Fee_GetActiveSeg(void)
{
	if(fee_seg_info[0].serial_seg == 0) {
		return 1;
	}
	return 0;
}


static uint8 Fee_GetPagenumOfBlock(uint8 seg, uint16 block_number)
{
	uint8 num_of_mgmt = (uint8)fee_seg_info[seg].num_mgmt;

	if(num_of_mgmt != 0) {
		num_of_mgmt = FEE_NUM_OF_MGMT_BLOCKS;
	}
	return block_number + 1 + num_of_mgmt;
}


/* return the number of pages allocated by a block */
static uint8 Fee_GetPageSizeOfBlock(uint16 block_number)
{
	const Fee_BlockConfiguration *cfg = fee_block_configuration;

	while(cfg->block_size) {
		if(cfg->block_number == block_number) {
			return (cfg->block_size + (FEE_PAGE_SIZE - 1)) / FEE_PAGE_SIZE;
		}
		cfg++;
	}

	return 1;
}


boolean Fee_IsBlockImmediateWrite(uint16 block_number)
{
	const Fee_BlockConfiguration *cfg = fee_block_configuration;

	while(cfg->block_size) {
		if(cfg->block_number == block_number) {
			return cfg->immediate_data;
		}
		cfg++;
	}

	return FALSE;
}


Fls_AddressType Fee_GetPageAddr(uint8 seg, uint8 page)
{
	Fls_AddressType addr;

	addr = page * FEE_PAGE_SIZE;
	if(seg == 1) {
		/* pages are linearly addressed, but there is a gap between sector 0 and 1 */
		addr += 0x10000;
	}
	return addr;
}


static boolean Fee_PagesAreEmpty(uint8 seg, uint8 pagenum, uint8 page_cnt)
{
	uint32 i;

	for(i = 0; i < page_cnt; i++) {
		if(BITARRAY_GETBIT(fee_seg_info[seg].used, pagenum + i)) {
			return FALSE;
		}
	}

	return TRUE;
}


static void Fee_AllocatePages(uint8 seg, uint8 pagenum, uint8 page_cnt)
{
	uint32 i;

	for(i = 0; i < page_cnt; i++) {
		BITARRAY_SETBIT(fee_seg_info[seg].used, pagenum + i);
	}
}


static void Fee_SetMgmt(uint8 seg, uint8 block_number, uint8 length, uint8 generation)
{
	int i;

	for(i = 0; i < length; i++) {
		fee_seg_info[seg].mgmt.block[block_number + i] = generation;
	}
}


static uint8 Fee_GetMgmt(uint8 seg, uint8 block_number)
{
	return fee_seg_info[seg].mgmt.block[block_number];
}



void Fee_Init(void)
{
	memset(fee_seg_info, 0, sizeof(fee_seg_info));
	memset(&fee_cmd, 0, sizeof(fee_cmd));

	fee_job_result = MEMIF_JOB_OK;
	fee_state = FEE_STATE_LOAD_PAGES_START;
}


Std_ReturnType Fee_Read(uint16 block_number, uint16 block_offset, uint8 *buf, uint16 length)
{
	if(fee_state != FEE_STATE_READY) {
		return E_NOT_OK;
	}

	/* function is currently idle */
	fee_cmd.block_number = block_number;
	fee_cmd.buf = buf;
	fee_cmd.block_offset = block_offset;
	fee_cmd.length = length;
	fee_cmd.pending = FEE_CMD_READ;
	fee_job_result = MEMIF_JOB_PENDING;

	return E_OK;
}


Std_ReturnType Fee_Write(uint16 block_number, const uint8 *buf)
{
	/*
	 * algorithm:
	 *
	 * find generation that is available for block
	 * if generation is not available then
	 *   initialize second sector
	 *   start copy procedure
	 * endif
	 *
	 * write block
	 * write mgmt
	 * if last mgmt was used OR last generation used then
	 *   initialize second sector
	 *   start copy procedure
	 * endif
	 */

	if(fee_state != FEE_STATE_READY) {
		return E_NOT_OK;
	}

	/* function is currently idle */
	fee_cmd.block_number = block_number;
	fee_cmd.buf = (uint8 *)buf;
	fee_cmd.pending = FEE_CMD_WRITE;
	fee_job_result = MEMIF_JOB_PENDING;

	return E_OK;
}


Std_ReturnType Fee_InvalidateBlock(uint16 block_number)
{
	if(fee_state != FEE_STATE_READY) {
		return E_NOT_OK;
	}

	/* function is currently idle */
	fee_cmd.block_number = block_number;
	fee_cmd.pending = FEE_CMD_INVALIDATE_BLOCK;
	fee_job_result = MEMIF_JOB_PENDING;

	return E_OK;
}


void Fee_Cancel(void)
{
	Fls_Cancel();
}


Std_ReturnType Fee_EraseImmediateBlock(uint16 block_number)
{
	if(fee_state != FEE_STATE_READY) {
		return E_NOT_OK;
	}

	if(!Fee_IsBlockImmediateWrite(block_number)) {
		return E_NOT_OK;
	}

	/* function is currently idle */
	fee_cmd.block_number = block_number;
	fee_cmd.pending = FEE_CMD_ERASE_IMMEDIATE_BLOCK;
	fee_job_result = MEMIF_JOB_PENDING;

	return E_OK;
}


MemIf_StatusType Fee_GetStatus(void)
{
	if((fee_state == FEE_STATE_READY) && (fee_job_result != MEMIF_JOB_PENDING)) {
		return Fls_GetStatus();
	}

	if(fee_state == FEE_STATE_UNINIT) {
		return MEMIF_UNINIT;
	}

	return MEMIF_BUSY_INTERNAL;
}


MemIf_JobResultType Fee_GetJobResult(void)
{
	if(fee_job_result == MEMIF_JOB_OK) {
		return Fls_GetJobResult();
	}
	else {
		return fee_job_result;
	}
}


void Fee_MainFunction(void)
{
	MemIf_JobResultType job_result;
	static uint8 seg;
	static uint16 page;
	static uint8 seg_old;
	static uint8 page_old;
	static uint8 seg_new;
	static uint8 page_new;
	uint8 length;
	uint8 generation;
	boolean found;
	int i;
	uint8 mgmt;
	uint8 mgmt_new;
	uint8 mgmt_old;

	if(Fls_GetStatus() == MEMIF_IDLE) {
		job_result = Fls_GetJobResult();

		switch(fee_state) {
			case FEE_STATE_UNINIT:
				fee_state = FEE_STATE_LOAD_PAGES_START;
				break;

			case FEE_STATE_LOAD_PAGES_START:
				/* init variables */
				memset(fee_seg_info, 0, sizeof(fee_seg_info));
				fee_state = FEE_STATE_LOAD_PAGES_READ;
				seg = 0;
				page = 0;
				break;

			case FEE_STATE_LOAD_PAGES_READ:
				/*
				 * start the reading of the next page
				 */
				if(Fls_Read(Fee_GetPageAddr(seg, (uint8)page), (uint8 *)fee_page, sizeof(fee_page)) == E_OK) {
					fee_state = FEE_STATE_LOAD_PAGES_EVAL;
				}
				break;

			case FEE_STATE_LOAD_PAGES_EVAL:
				if(job_result != MEMIF_JOB_OK) {
					fee_state = FEE_STATE_LOAD_PAGES_READ;
					break;
				}

				/* evaluate the read information */
				if(page == 0) {
					/* serial information of segment */
					fee_seg_info[seg].serial_seg = fee_page[0];
					fee_seg_info[seg].num_mgmt = fee_page[1];
					fee_seg_info[seg].num_of_pages_in_generation = fee_page[2];
				}
				else if((page + 1) < fee_seg_info[seg].num_mgmt) {
					/* evaluate the management block */
					Fee_MgmtType *mgmt;

					mgmt = (Fee_MgmtType *)fee_page;
					if(mgmt->serial != 0) {
						/* valid serial information -> copy the management data */
						memcpy(&fee_seg_info[seg].mgmt, mgmt, sizeof(fee_seg_info[seg].mgmt));
						fee_seg_info[seg].used_mgmt_page++;
					}
				}

				/*
				 * check whether this page is empty
				 */
				BITARRAY_CLRBIT(fee_seg_info[seg].used, page);
				for(i = 0; i < FEE_PAGE_SIZE_IN_WORDS; i++) {
					if(fee_page[i] != 0) {
						/* not empty */
						BITARRAY_SETBIT(fee_seg_info[seg].used, page);
						break;
					}
				}

				/*
				 * check what to do next
				 */
				fee_state = FEE_STATE_LOAD_PAGES_READ;
				page++;
				if(page > FEE_NUM_OF_PAGES_PER_SECTOR) {
					page = 0;
					seg++;
					if(seg >= FEE_NUM_OF_SEGMENTS) {
						/* load pages finished continue with copy pages */
						fee_state = FEE_STATE_COPYPAGES_START;
					}
				}
				break;

			case FEE_STATE_COPYPAGES_START:
				/*
				 * both segments are initialized - so a copy procedure is not
				 * yet complete. Find the remaining blocks to copy and finally
				 * clear the old segment
				 *
				 * for all blocks
				 *   if(block of segold is valid and
				 *      block of segnew is not valid)
				 *      if(block in generation 1 of segnew is empty)
				 *         copy block of generation x in segold to generation 1 in segnew
				 *      endif
				 *   endif
				 * done
				 *
				 * update management information by using the management information
				 * of segold but replace generation x by generation 1
				 *
				 * clear segment segold
				 */
				if(fee_seg_info[0].serial_seg != 0 && fee_seg_info[1].serial_seg != 0) {
					/* both segments are in use -- continue with the copying from the older to */
					/* the newer one */
					if((sint32)(fee_seg_info[0].serial_seg - fee_seg_info[1].serial_seg) > 0) {
						/* segment 0 is the newer one */
						seg_new = 0;
						seg_old = 1;
					}
					else {
						/* segment 1 is the newer one */
						seg_new = 1;
						seg_old = 0;
					}
					fee_state = FEE_STATE_COPYPAGES_CONT;
				}
				else {
					if(fee_seg_info[0].serial_seg == 0 && fee_seg_info[1].serial_seg == 0) {
						/* none is initialized -- initialize it */
						fee_state = FEE_STATE_INITSEG;
					}
					else {
						/* only one is initialized -- check the fill level */
						fee_state = FEE_STATE_CHECKFILL_LEVEL;
					}
				}
				break;

			case FEE_STATE_COPYPAGES_CONT:
				/* if not decided otherwise below then write the management information */
				fee_state = FEE_STATE_COPYPAGES_WRITEMGMT;

				/* serial is always starting with one, when a new segment is started */
				fee_seg_info[seg_new].mgmt.serial = 1;

				for(i = 0; i < sizeof(fee_seg_info[seg_old].mgmt.block); i++) {
					/* check all blocks */
					mgmt_old = Fee_GetMgmt(seg_old, (uint8)i);
					mgmt_new = Fee_GetMgmt(seg_new, (uint8)i);

					if(mgmt_old > 0) {
						/* there is something that needs to be transferred */
						if(mgmt_new == 0) {
							/* it is not yet done */
							if(mgmt_old == 255) {
								/* we need to set only the invalidated information */
								Fee_SetMgmt(seg_new, (uint8)i, 1, mgmt_old);
							}
							else {
								page_old = (uint8)(Fee_GetPagenumOfBlock(seg_old, (uint8)i)
										+ (mgmt_old - 1) * fee_seg_info[seg_old].num_of_pages_in_generation);
								page_new = Fee_GetPagenumOfBlock(seg_new, (uint8)i);
								if(Fee_PagesAreEmpty(seg_new, page_new, 1)) {
									/* no data is contained in this generation -- so it needs to be written first */
									uint16 page_old_next = (uint16)(page_old + fee_seg_info[seg_old].num_of_pages_in_generation);

									if(page_old_next < 256 && !Fee_PagesAreEmpty(seg_old, (uint8)page_old_next, 1)) {
										/* this block contains inconsistent data since the next generation */
										/* contains already data but the mgmt points still to the previous generation */
										/* therefore nothing needs to be copied for this block */
										Fee_SetMgmt(seg_new, (uint8)i, (uint8)Fee_GetPageSizeOfBlock((uint8)i), 0);
										/* advance to the end of the block */
										i += Fee_GetPageSizeOfBlock((uint8)i) - 1;
									}
									else {
										fee_state = FEE_STATE_COPYPAGES_LOADPAGE;
										break;
									}
								}
								else {
									/* the data is available only the management information needs to be written */
									Fee_SetMgmt(seg_new, (uint8)i, 1, 1);
								}
							}
						}
					}
				}
				break;

			case FEE_STATE_COPYPAGES_LOADPAGE:
				/* the read needs to be performed */
				if(Fls_Read(Fee_GetPageAddr(seg_old, page_old), (uint8 *)fee_page, sizeof(fee_page)) == E_OK) {
					/* advance to write */
					fee_state = FEE_STATE_COPYPAGES_WRITEPAGE;
				}
				break;

			case FEE_STATE_COPYPAGES_WRITEPAGE:
				if(job_result != MEMIF_JOB_OK) {
					fee_state = FEE_STATE_COPYPAGES_LOADPAGE;
					break;
				}

				if(Fls_Write(Fee_GetPageAddr(seg_new, page_new), (uint8 *)fee_page, sizeof(fee_page)) == E_OK) {
					/* advance to copy */
					fee_state = FEE_STATE_COPYPAGES_CONT;
					BITARRAY_SETBIT(fee_seg_info[seg_new].used, page_new);
				}
				break;

			case FEE_STATE_COPYPAGES_WRITEMGMT:
				/* all information is consistent now - update the management block */
				if(Fls_Write(Fee_GetPageAddr(seg_new, 1), (uint8 *)&fee_seg_info[seg_new].mgmt,
						sizeof(fee_seg_info[seg_new].mgmt)) == E_OK) {
					/* advance to erase seg */
					fee_state = FEE_STATE_COPYPAGES_ERASESEG;
					BITARRAY_SETBIT(fee_seg_info[seg_new].used, 1);
					fee_seg_info[seg_new].used_mgmt_page = 1;
				}
				break;

			case FEE_STATE_COPYPAGES_ERASESEG:
				/* all information is consistent now - update the management block */
				if(job_result != MEMIF_JOB_OK) {
					fee_state = FEE_STATE_COPYPAGES_START;
					break;
				}
				if(Fls_Erase(Fee_GetPageAddr(seg_old, 0), FEE_NUM_OF_PAGES_PER_SECTOR * FEE_PAGE_SIZE)
						== E_OK) {
					/* all data is transfered -- flash in now in an consistent state again */
					memset(&fee_seg_info[seg_old], 0, sizeof(fee_seg_info[seg_old]));
					fee_state = FEE_STATE_READY;
				}
				break;

			case FEE_STATE_CHECKFILL_LEVEL:
				if(fee_seg_info[seg].used_mgmt_page >= fee_seg_info[seg].num_mgmt - FEE_NUM_OF_IMMEDIATE_BLOCKS) {
					fee_state = FEE_STATE_INITSEG;
				}
				else {
					fee_state = FEE_STATE_READY;
				}
				break;

			case FEE_STATE_INITSEG:
				seg_old = Fee_GetActiveSeg();
				if(seg_old == 1) {
					seg_new = 0;
				}
				else {
					seg_new = 1;
				}

				if(fee_seg_info[seg_new].serial_seg != 0) {
					/* some data is contained in this segment */
					fee_state = FEE_STATE_LOAD_PAGES_START;
				}
				else {
					fee_seg_info[seg_new].serial_seg = fee_seg_info[seg_old].serial_seg + 1;
					fee_seg_info[seg_new].num_mgmt = FEE_NUM_OF_MGMT_BLOCKS; /* use configuration value */
					fee_seg_info[seg_new].num_of_pages_in_generation = FEE_NUM_OF_PAGES_IN_GENERATION;

					memset(fee_page, 0, sizeof(fee_page));
					fee_page[0] = fee_seg_info[seg_new].serial_seg;
					fee_page[1] = fee_seg_info[seg_new].num_mgmt;
					fee_page[2] = fee_seg_info[seg_new].num_of_pages_in_generation;

					if(Fls_Write(Fee_GetPageAddr(seg_new, 0), (uint8 *)fee_page, sizeof(fee_page)) != E_OK) {
						Det_ReportError(FEE_MODULE_ID, FEE_INSTANCE_ID, FEE_MAINFUNCTION_API_ID, FEE_E_FLSREAD);
						fee_state = FEE_STATE_ERROR;
					}
					else {
						/* copy all data */
						fee_state = FEE_STATE_COPYPAGES_START;
						BITARRAY_SETBIT(fee_seg_info[seg_new].used, 0);
					}
				}
				break;

			case FEE_STATE_CMD_READ:
				seg = Fee_GetActiveSeg();
				mgmt = Fee_GetMgmt(seg, (uint8)fee_cmd.block_number);
				page = Fee_GetPagenumOfBlock(seg, fee_cmd.block_number);
				page = (uint16)(page + (mgmt - 1) * fee_seg_info[seg].num_of_pages_in_generation);
				length = Fee_GetPageSizeOfBlock(fee_cmd.block_number);

				if(mgmt > 0 && mgmt < 255) {
					if((page + fee_seg_info[seg].num_of_pages_in_generation < FEE_NUM_OF_PAGES_PER_SECTOR) &&
					   (!Fee_PagesAreEmpty(seg, (uint8)(page + fee_seg_info[seg].num_of_pages_in_generation), length))) {
						/* check for next generation */
						/* there is data in the next generation -- this means that a write has been interrupted */
						/* the block contains no data */
						fee_job_result = MEMIF_BLOCK_INCONSISTENT;
						fee_state = FEE_STATE_ERROR;
					}
					else {
						/* data is in an consistent state */
						if(Fls_Read(Fee_GetPageAddr(seg, (uint8)page) + fee_cmd.block_offset, fee_cmd.buf, fee_cmd.length) == E_OK) {
							fee_state = FEE_STATE_SUCCESS;
						}
						else {
							Det_ReportError(FEE_MODULE_ID, FEE_INSTANCE_ID, FEE_MAINFUNCTION_API_ID, FEE_E_FLSREAD);
							fee_state = FEE_STATE_ERROR;
						}
					}
				}
				else {
					if(mgmt == 255) {
						/* set block was set to an invalid state */
						fee_job_result = MEMIF_BLOCK_INVALID;
					}
					else {
						/* the block contains no data */
						fee_job_result = MEMIF_BLOCK_INCONSISTENT;
					}
					fee_state = FEE_STATE_ERROR;
				}
				break;

			case FEE_STATE_CMD_WRITE:
				seg = Fee_GetActiveSeg();
				page = Fee_GetPagenumOfBlock(seg, fee_cmd.block_number);
				length = Fee_GetPageSizeOfBlock(fee_cmd.block_number);

				found = FALSE;
				generation = 1;
				for(i = 0; i < FEE_NUM_OF_GENERATIONS; i++) {
					if(Fee_PagesAreEmpty(seg, (uint8)page, length)) {
						found = TRUE;
						break;
					}
					page += (uint16)(fee_seg_info[seg].num_of_pages_in_generation);
					generation++;
				}

				if(found) {
					if(Fls_Write(Fee_GetPageAddr(seg, (uint8)page), fee_cmd.buf, Fee_GetPageSizeOfBlock(fee_cmd.block_number)
							* FEE_PAGE_SIZE) == E_OK) {
						/* advance to write mgmt */
						Fee_AllocatePages(seg, (uint8)page, length);
						Fee_SetMgmt(seg, (uint8)fee_cmd.block_number, length, generation);
						fee_seg_info[seg].mgmt.serial++;
						if(fee_seg_info[seg].mgmt.serial == 0) {
							fee_seg_info[seg].mgmt.serial++;
						}
						fee_state = FEE_STATE_CMD_WRITE_MGMT;
					}
				}
				else {
					/* advance to other segment, since this one used all generations */
					/* the command stays in the pending state, so nothing gets lost here */
					fee_state = FEE_STATE_INITSEG;
				}
				break;

			case FEE_STATE_CMD_INVALIDATE_BLOCK:
				seg = Fee_GetActiveSeg();
				page = Fee_GetPagenumOfBlock(seg, fee_cmd.block_number);
				length = Fee_GetPageSizeOfBlock(fee_cmd.block_number);

				Fee_SetMgmt(seg, (uint8)fee_cmd.block_number, length, 255);
				fee_state = FEE_STATE_CMD_WRITE_MGMT;
				break;

			case FEE_STATE_CMD_WRITE_MGMT:
				/* write mgmt */
				if(job_result != MEMIF_JOB_OK) {
					fee_state = FEE_STATE_ERROR;
					break;
				}
				seg = Fee_GetActiveSeg();
				fee_seg_info[seg].used_mgmt_page++;

				fee_state = FEE_STATE_SUCCESS;
				if(Fls_Write(Fee_GetPageAddr(seg, fee_seg_info[seg].used_mgmt_page), (uint8 *)&fee_seg_info[seg].mgmt,
						sizeof(fee_seg_info[seg].mgmt)) == E_OK) {
					Fee_AllocatePages(seg, fee_seg_info[seg].used_mgmt_page, 1);

					if(fee_seg_info[seg].used_mgmt_page >= fee_seg_info[seg].num_mgmt - FEE_NUM_OF_IMMEDIATE_BLOCKS) {
						fee_state = FEE_STATE_INITSEG;
					}
					else {
						/* if an immediate write shall be possible then it is required */
						/* to change to the new sector if the last generation was used */
						if(Fee_IsBlockImmediateWrite(fee_cmd.block_number)) {
							seg = Fee_GetActiveSeg();
							generation = Fee_GetMgmt(seg, (uint8)fee_cmd.block_number);

							if(generation + 1 >= FEE_NUM_OF_GENERATIONS) {
								fee_state = FEE_STATE_INITSEG;
							}
						}
					}
				}
				else {
					fee_state = FEE_STATE_ERROR;
				}
				break;

			case FEE_STATE_CMD_ERASE_IMMEDIATE_BLOCK:
				/* change to the new sector if the last generation was used */
				seg = Fee_GetActiveSeg();
				generation = Fee_GetMgmt(seg, (uint8)fee_cmd.block_number);
				if(generation + 1 >= FEE_NUM_OF_GENERATIONS) {
					fee_state = FEE_STATE_INITSEG;
				}
				else {
					fee_state = FEE_STATE_SUCCESS;
				}
				break;

			case FEE_STATE_ERROR:
				fee_cmd.pending = FEE_CMD_IDLE;
				fee_state = FEE_STATE_LOAD_PAGES_START;
				if(fee_job_result == MEMIF_JOB_PENDING) {
					fee_job_result = MEMIF_JOB_FAILED;
				}
				Fee_JobErrorNotification();
				break;

			case FEE_STATE_SUCCESS:
				fee_cmd.pending = FEE_CMD_IDLE;
				fee_state = FEE_STATE_READY;
				if(fee_job_result == MEMIF_JOB_PENDING) {
					fee_job_result = MEMIF_JOB_OK;
				}
				Fee_JobEndNotification();
				break;

			case FEE_STATE_READY:
				if(fee_cmd.pending != FEE_CMD_IDLE) {
					/* if a new command is started now then set it to pending */
					fee_job_result = MEMIF_JOB_PENDING;
				}
				switch(fee_cmd.pending) {
					case FEE_CMD_WRITE:
						fee_state = FEE_STATE_CMD_WRITE;
						break;
					case FEE_CMD_READ:
						fee_state = FEE_STATE_CMD_READ;
						break;
					case FEE_CMD_INVALIDATE_BLOCK:
						fee_state = FEE_STATE_CMD_INVALIDATE_BLOCK;
						break;
					case FEE_CMD_ERASE_IMMEDIATE_BLOCK:
						fee_state = FEE_STATE_CMD_ERASE_IMMEDIATE_BLOCK;
						break;
					case FEE_CMD_IDLE:
						break;
					default:
						fee_cmd.pending = FEE_CMD_IDLE;
						break;
				}
				break;
			default:
				fee_state = FEE_STATE_UNINIT;
				break;
		}
	}

	Fls_MainFunction();
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"

