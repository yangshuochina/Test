/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM_Process.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:38MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM_Process.c  $
 *  Revision 1.4 2011/02/02 09:17:38MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/12/14 09:40:18MEZ Cotor Vlad RGB (COTORV) 
 *  NvM Driver update
 *  Revision 1.2 2010/07/12 14:26:33MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "NvM.h"
#include "NvM_Priv.h"
#include "NvM_Cbk.h"

#include "Dem.h"
#include "MemIf.h"
#include "Crc.h"

#define NVM_START_SEC_CODE
#include "MemMap.h"


static void NvM_ReadStart(uint16 block)
{
	NVM_ASSERT_RETVOID(MemIf_Read(nvm_cfg.blocks[block].dev_id,
									nvm_cfg.blocks[block].block_number,
									0,
									nvm_cfg.blocks[block].status->ram_image,
									nvm_cfg.blocks[block].block_size) == E_OK,
							NVM_MAINFUNCTION_API_ID, NVM_E_ERROR_CODE);
}


static boolean NvM_ReadWait(uint16 block)
{
	MemIf_JobResultType result;
	boolean done = FALSE;

	result = MemIf_GetJobResult(nvm_cfg.blocks[block].dev_id);
	switch(result) {
		case MEMIF_JOB_PENDING:
			break;
		case MEMIF_JOB_OK:
			nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_WRITTEN;
			done = TRUE;
			break;
		case MEMIF_BLOCK_INCONSISTENT:
			Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
			nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_INVALID;
			done = TRUE;
			break;
		case MEMIF_BLOCK_INVALID:
			nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_INVALID;
			done = TRUE;
			break;
		case MEMIF_JOB_FAILED:
			Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
			done = TRUE;
			break;
		case MEMIF_JOB_CANCELLED:
		default:
			done = TRUE;
			break;
	}

	return done;
}



static void NvM_WriteStart(uint16 block)
{
	NVM_ASSERT_RETVOID(MemIf_Write(nvm_cfg.blocks[block].dev_id,
									nvm_cfg.blocks[block].block_number,
									nvm_cfg.blocks[block].status->ram_image) == E_OK,
							NVM_MAINFUNCTION_API_ID, NVM_E_ERROR_CODE);
}


static boolean NvM_WriteWait(uint16 block)
{
	MemIf_JobResultType result;
	boolean done = FALSE;

	result = MemIf_GetJobResult(nvm_cfg.blocks[block].dev_id);
	switch(result) {
		case MEMIF_JOB_PENDING:
			break;
		case MEMIF_JOB_OK:
			nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_WRITTEN;
			done = TRUE;
			break;
		case MEMIF_JOB_FAILED:
		case MEMIF_BLOCK_INCONSISTENT:
		case MEMIF_BLOCK_INVALID:
		case MEMIF_JOB_CANCELLED:
		default:
			nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_CHANGED;
			done = TRUE;
			break;
	}

	return done;
}


static void NvM_CrcStart(uint16 block)
{
	nvm_cfg.blocks[block].status->ram_crc_work = 0;
	nvm_cfg.blocks[block].status->ptr = nvm_cfg.blocks[block].status->ram_image;
	nvm_cfg.blocks[block].status->cnt = nvm_cfg.blocks[block].block_size;
}


static boolean NvM_CrcWait(uint16 block)
{
	uint32 length;
	boolean done = FALSE;

	length = __min(nvm_cfg.blocks[block].status->cnt, nvm_cfg.crc_calc_cnt);
	if(length == 0) {
		return TRUE;
	}

	switch(nvm_cfg.blocks[block].crc_type) {
		case NVM_CRC16:
			nvm_cfg.blocks[block].status->ram_crc_work =
				Crc_CalculateCRC16(nvm_cfg.blocks[block].status->ptr,
						length,
						(uint16)nvm_cfg.blocks[block].status->ram_crc_work);
			break;
		case NVM_CRC32:
			nvm_cfg.blocks[block].status->ram_crc_work =
				Crc_CalculateCRC32(nvm_cfg.blocks[block].status->ptr,
						length,
						nvm_cfg.blocks[block].status->ram_crc_work);
			break;
		case NVM_CRC_NONE:
		default:
			done = TRUE;
			break;
	}

	nvm_cfg.blocks[block].status->cnt = (uint16)(nvm_cfg.blocks[block].status->cnt - length);
	nvm_cfg.blocks[block].status->ptr += length;

	return done;
}


static void NvM_ProcessRequest(uint16 block)
{
	switch(nvm_cfg.blocks[block].status->req) {
		case NVM_REQ_VERIFY_CRC:
			if(nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_UNKNOWN ||
				nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_IMAGE_INVALID) {
				/* the block status is set to unknown/invalid -- no need to calculate the */
				/* CRC since it must be recovered anyway */
				NvM_ReadStart(block);
				nvm_cfg.blocks[block].status->req = NVM_REQ_LOAD_EE;
			}
			else {
				NvM_CrcStart(block);
				nvm_cfg.blocks[block].status->req = NVM_REQ_VERIFY_CRC_WAIT;
			}
			break;

		case NVM_REQ_VERIFY_CRC_WAIT:
			if(NvM_CrcWait(block)) {
				/* CRC calculation completed */
				if(nvm_cfg.blocks[block].status->ram_crc_work == nvm_cfg.blocks[block].status->ram_crc) {
					/* CRC is identical -> block in RAM is validated */
					nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_CHANGED;
					nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
					nvm_cfg.blocks[block].status->result = NVM_REQ_BLOCK_SKIPPED;
				}
				else {
					/* CRC is bad -> load from EE */
					NvM_ReadStart(block);
					nvm_cfg.blocks[block].status->req = NVM_REQ_LOAD_EE;
					Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
				}
			}
			break;

		case NVM_REQ_LOAD:
			NvM_ReadStart(block);
			nvm_cfg.blocks[block].status->req = NVM_REQ_LOAD_EE;
			break;

		case NVM_REQ_LOAD_EE:
			if(NvM_ReadWait(block)) {
				/* load is finished */
				if(nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_IMAGE_WRITTEN) {
					/* load was successful if the status indicates that both are identical */
					if(nvm_cfg.blocks[block].crc_type != NVM_CRC_NONE) {
						/* a CRC is configured -- calculate the CRC for the loaded image */
						/* and compare it against the CRC in the block 1 */
						NvM_CrcStart(block);
						nvm_cfg.blocks[block].status->req = NVM_REQ_CALC_CRC_AND_COMPARE_WITH_EE;
					}
					else {
						nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
						nvm_cfg.blocks[block].status->result = NVM_REQ_OK;
					}
				}
				else {
					/* load from EE was not successful */
					nvm_var.error_cnt++;
					nvm_cfg.blocks[block].status->req = NVM_REQ_RECOVER_BLOCK;
				}
			}
			break;

		case NVM_REQ_RECOVER_BLOCK:
			if(nvm_cfg.blocks[block].rom_image != NULL_PTR) {
				memcpy(nvm_cfg.blocks[block].status->ram_image,
						nvm_cfg.blocks[block].rom_image, nvm_cfg.blocks[block].block_size);
				nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_DEFAULT;
			}

			if(nvm_cfg.blocks[block].init_block != NULL_PTR) {
				nvm_cfg.blocks[block].init_block();
				nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_DEFAULT;
			}

			if(nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_IMAGE_INVALID ||
				nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_UNKNOWN) {
				/* the block could not be restored */
				if(nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_IMAGE_INVALID) {
					nvm_cfg.blocks[block].status->result = NVM_REQ_NV_INVALIDATED;
				}
				else {
					nvm_cfg.blocks[block].status->result = NVM_REQ_INTEGRITY_FAILED;
				}
				nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
			}
			else {
				/* the block is restored -- calculate the CRC of it */
				NvM_CrcStart(block);
				nvm_cfg.blocks[block].status->req = NVM_REQ_CALC_CRC;
				nvm_cfg.blocks[block].status->result = NVM_REQ_INTEGRITY_FAILED;
			}
			break;

		case NVM_REQ_START_CALC_CRC:
			NvM_CrcStart(block);
			nvm_cfg.blocks[block].status->req = NVM_REQ_CALC_CRC;
			break;

		case NVM_REQ_CALC_CRC:
			if(NvM_CrcWait(block)) {
				/* CRC calculation completed */
				nvm_cfg.blocks[block].status->ram_crc = nvm_cfg.blocks[block].status->ram_crc_work;
				nvm_block1.crc_of_block[block] = nvm_cfg.blocks[block].status->ram_crc_work;
				nvm_cfg.blocks[1].status->block_status = NVM_BLOCK_STATUS_IMAGE_CHANGED;
				nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
				nvm_cfg.blocks[block].status->result = NVM_REQ_OK;
				if(nvm_cfg.blocks[block].single_block_callback != NULL_PTR) {
					nvm_cfg.blocks[block].single_block_callback();
				}
			}
			break;

		case NVM_REQ_CALC_CRC_AND_COMPARE_WITH_EE:
			if(NvM_CrcWait(block)) {
				/* CRC calculation completed */
				nvm_cfg.blocks[block].status->ram_crc = nvm_cfg.blocks[block].status->ram_crc_work;
				if(nvm_cfg.blocks[block].status->ram_crc != nvm_block1.crc_of_block[block]) {
					/* mismatch in CRC */
					nvm_cfg.blocks[block].status->req = NVM_REQ_RECOVER_BLOCK;
					nvm_var.error_cnt++;
				}
				else {
					/* loaded successfully */
					nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
					nvm_cfg.blocks[block].status->result = NVM_REQ_OK;
					if(nvm_cfg.blocks[block].single_block_callback != NULL_PTR) {
						nvm_cfg.blocks[block].single_block_callback();
					}
				}
			}
			break;

		case NVM_REQ_VERIFY_ID:
			NvM_ReadStart(block);
			nvm_cfg.blocks[block].status->req = NVM_REQ_VERIFY_ID_WAIT;
			break;

		case NVM_REQ_VERIFY_ID_WAIT:
			if(NvM_ReadWait(block)) {
				/* load is finished */
				if(nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_IMAGE_WRITTEN) {
					if(nvm_block1.id == nvm_cfg.config_id) {
						/* configuration ID matches */
						nvm_cfg.blocks[block].status->result = NVM_REQ_OK;
					}
					else {
						/* configuration ID mismatch */
						nvm_var.error_cnt++;
						nvm_block1.id = nvm_cfg.config_id;
						nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_DEFAULT;
						nvm_cfg.blocks[block].status->result = NVM_REQ_INTEGRITY_FAILED;
						Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
					}
				}
				else {
					/* the configuration ID could not be read */
					nvm_var.error_cnt++;
					nvm_block1.id = nvm_cfg.config_id;
					nvm_cfg.blocks[block].status->block_status = NVM_BLOCK_STATUS_IMAGE_DEFAULT;
					nvm_cfg.blocks[block].status->result = NVM_REQ_NV_INVALIDATED;
					Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
				}
				nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
			}
			break;

		case NVM_REQ_WRITE:
			NvM_WriteStart(block);
			nvm_cfg.blocks[block].status->req = NVM_REQ_WRITE_EE;
			break;

		case NVM_REQ_WRITE_EE:
			if(NvM_WriteWait(block)) {
				/* load is finished */
				if(nvm_cfg.blocks[block].status->block_status == NVM_BLOCK_STATUS_IMAGE_WRITTEN) {
					/* write was successful if the status indicates that both are identical */
					nvm_cfg.blocks[block].status->result = NVM_REQ_OK;
				}
				else {
					/* write to EE was not successful */
					nvm_cfg.blocks[block].status->result = NVM_REQ_NOT_OK;
				}

				nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
			}
			break;

		default:
			nvm_cfg.blocks[block].status->result = NVM_REQ_NOT_OK;
			nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
			break;
	}
}



void NvM_Schedule(void)
{
	uint16 block;
	int block_cnt = 0;

	while(block_cnt < nvm_cfg.num_of_blocks) {
		block = nvm_var.current_block;
		if((nvm_cfg.blocks[block].status == NULL_PTR) ||
				(nvm_cfg.blocks[block].status->req == NVM_REQ_IDLE)) {
			/* advance to next block */
			if(nvm_var.cancel_req) {
				/* perform the cancellation */
				nvm_var.cancel_req = 0;
				for(block=0; block<nvm_cfg.num_of_blocks; block++) {
					if(nvm_cfg.blocks[block].status->req == NVM_REQ_WRITE) {
						nvm_cfg.blocks[block].status->req = NVM_REQ_IDLE;
						nvm_cfg.blocks[block].status->result = NVM_REQ_CANCELLED;
					}
				}
			}
			else {
				/* continue with next block */
				nvm_var.current_block++;
				block_cnt++;
				if(nvm_var.current_block >= nvm_cfg.num_of_blocks) {
					nvm_var.current_block = 0;
				}
			}
		}
		else {
			/* continue with the processing of this block */
			if(MemIf_GetStatus(nvm_cfg.blocks[block].dev_id) == MEMIF_IDLE) {
				NvM_ProcessRequest(block);
			}
			break;
		}
	}

	if(nvm_var.rw_all_in_progress) {
		if(block_cnt >= nvm_cfg.num_of_blocks) {
			nvm_var.rw_all_in_progress = FALSE;
#if defined(NVM_DRV_MODE_SWITCH) && (NVM_DRV_MODE_SWITCH == STD_ON)
			MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
			if(nvm_var.error_cnt == 0) {
				NvM_JobEndNotification();
			}
			else {
				NvM_JobErrorNotification();
			}
		}
	}
}

#define NVM_STOP_SEC_CODE
#include "MemMap.h"
