/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fls.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:33MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Fls.c  $
 *  Revision 1.3 2011/02/02 09:17:33MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 09:46:19MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:16:37MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Fls/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Fls.h"
#include "Fls_Priv.h"

#include "Dem.h"


typedef void (*Fls_EraseFuncPtr)(uint32 *addr, uint32 page_size);
typedef void (*Fls_WriteFuncPtr)(uint32 *addr, const uint32 *data, uint32 page_size);


#define FLS_START_SEC_VAR_32BIT
#include "MemMap.h"


const Fls_ConfigType *fls_cfg;
uint32 fls_proc_time;	/* max. time for the processing of mainloop */
uint32 *fls_address;	/* current address in Flash */
uint32 *fls_data;		/* current address in RAM */
sint32 fls_length;		/* number of words to process */
uint32 fls_page_size;  	/* page size in words */
uint32 (*fls_proc_func)(void);	/* processing function -- returns 0 if finished */
enum {
	FLS_STATE_IDLE,			/* nothing is performed at the moment */
	FLS_STATE_TOSTART,		/* an operation is waiting for the start */
	FLS_STATE_INPROGRESS,	/* an operation is currently ongoing */
	FLS_STATE_FAILED,		/* the last operation failed -- got aborted due to an error */
	FLS_STATE_CANCELED,		/* the operation was canceled */
	FLS_STATE_INEQUAL,		/* the compare has found not equal data */
} fls_state;

#define FLS_STOP_SEC_VAR_32BIT
#include "MemMap.h"




#define FLS_START_SEC_CODE
#include "MemMap.h"


/*
 * get the page size of the flash located at "address"
 * the page size is returned in words
 */
static uint32 Fls_GetPageSize(uint32 *address)
{
	uint32 page_size;

	if((address >= FLS_START_DFLASH0) && (address <  FLS_END_DFLASH1)) {
		/* data flash */
		page_size = 128 / 4;
	}
	else if((address >= FLS_START_PFLASH) && (address < FLS_END_PFLASH)) {
		/* program flash */
		page_size = 256 / 4;
	}
	else {
		page_size = 0;
	}

	return page_size;
}


/*
 * get the sector size of the flash located at "address"
 * the sector size is returned in words
 */
static uint32 Fls_GetSectorSize(uint32 *address)
{
	uint32 sector_size;

	if((address >= FLS_START_DFLASH0) && (address <  FLS_END_DFLASH1)) {
		/* data flash has only a single sector */
		sector_size = 0x8000u / 4;
	}
	else {
		if(address < (uint32 *)0xA0020000u) {
			/* 16 KB sector */
			sector_size = 0x4000u / 4;
		}
		else if(address < (uint32 *)0xA0040000u) {
			/* 128 KB sector */
			sector_size = 0x20000u / 4;
		}
		else {
			/* 256 KB sector */
			sector_size = 0x40000u / 4;
		}
	}

	return sector_size;
}




void Fls_Init(const Fls_ConfigType *cfg)
{
	FLS_ASSERT_RETVOID(fls_proc_func == NULL_PTR, FLS_INIT_API_ID, FLS_E_BUSY);
	FLS_ASSERT_RETVOID(cfg != NULL_PTR, FLS_INIT_API_ID, FLS_E_PARAM_CONFIG);
	FLS_ASSERT_RETVOID((cfg->base_address == FLS_START_PFLASH) ||
			           (cfg->base_address == FLS_START_DFLASH0) ||
			           (cfg->base_address == FLS_START_DFLASH1) ||
			           (cfg->base_address == NULL_PTR), FLS_INIT_API_ID, FLS_E_PARAM_CONFIG);

	fls_proc_time = cfg->proc_time_slow;
	fls_proc_func = NULL_PTR;
	fls_state = FLS_STATE_IDLE;

	fls_cfg = cfg;
}


static uint32 Fls_ProcessErase(void)
{
	uint32 busy = 0;
	uint32 sector_size;
	Fls_EraseFuncPtr erase_func;

	if(fls_state == FLS_STATE_INPROGRESS) {
		/* a erase operation is currently ongoing, check whether it has been finished */
		if(!FLASH0_FSR.B.D0BUSY && !FLASH0_FSR.B.D1BUSY && !FLASH0_FSR.B.PBUSY) {
			/* erase is finished */
			Fls_AccFinish(fls_address);

			if(FLASH_FSR.B.VER == 1) {
				/* verification error detected -- abort */
				fls_state = FLS_STATE_FAILED;
				Dem_ReportErrorStatus(FLS_E_ERASE_FAILED, DEM_EVENT_STATUS_FAILED);
				return 0;
			}

			/* get the next sector to erase */
			sector_size = Fls_GetSectorSize(fls_address);
			fls_address += sector_size;
			fls_length -= sector_size;

			fls_state = FLS_STATE_TOSTART;
		}
	}

	if(fls_state == FLS_STATE_TOSTART) {
		/* start a new erase */
		if(fls_length <= 0) {
			/* finished */
			fls_state = FLS_STATE_IDLE;
			return 0;
		}
		else {
			fls_state = FLS_STATE_INPROGRESS;
			if(fls_cfg->ram_image_addr != NULL_PTR) {
				Fls_AccCopyEraseToRam(fls_cfg->ram_image_addr);
				erase_func = (Fls_EraseFuncPtr)fls_cfg->ram_image_addr;
				erase_func(fls_address, fls_page_size);
				Fls_AccClearEraseFromRam(fls_cfg->ram_image_addr);
			}
			else {
				Fls_AccEraseSector(fls_address, fls_page_size);
			}
		}
	}

	return 1;
}


Std_ReturnType Fls_Erase(Fls_AddressType address, Fls_LengthType length)
{
	uint32 sector_size;  /* sector size of the last sector to be erased */

	FLS_ASSERT_RETERR(fls_cfg != NULL_PTR, FLS_ERASE_API_ID, FLS_E_UNINIT);
	FLS_ASSERT_RETERR(length > 0, FLS_ERASE_API_ID, FLS_E_PARAM_LENGTH);
	FLS_ASSERT_RETERR(fls_proc_func == NULL_PTR, FLS_ERASE_API_ID, FLS_E_BUSY);

	fls_address = fls_cfg->base_address + address / 4;
	fls_page_size = Fls_GetPageSize(fls_address);
	sector_size = Fls_GetSectorSize(fls_address + length - 1);

	FLS_ASSERT_RETERR(sector_size != 0, FLS_ERASE_API_ID, FLS_E_PARAM_LENGTH);
	FLS_ASSERT_RETERR(fls_page_size != 0, FLS_ERASE_API_ID, FLS_E_PARAM_ADDRESS);
	FLS_ASSERT_RETERR(((fls_address >= FLS_START_PFLASH)  && (fls_address+length/4 <= FLS_END_PFLASH)) ||
			          ((fls_address >= FLS_START_DFLASH0) && (fls_address+length/4 <= FLS_END_DFLASH0)) ||
			          ((fls_address >= FLS_START_DFLASH1) && (fls_address+length/4 <= FLS_END_DFLASH1)),
			        		  FLS_ERASE_API_ID, FLS_E_PARAM_ADDRESS);

	/* calculate the number of words to erase */
	fls_length = (length+3) / 4;

	fls_state = FLS_STATE_TOSTART;
	fls_proc_func = Fls_ProcessErase;

	return E_OK;
}



static uint32 Fls_ProcessWrite(void)
{
	Fls_WriteFuncPtr write_func;
#if defined(FLS_DEV_ERROR_DETECT) && (FLS_DEV_ERROR_DETECT == STD_ON)
	int i;
	int ts_error = 0;
#endif

	if(fls_state == FLS_STATE_INPROGRESS) {
		/* a write operation is currently ongoing, check whether it has been finished */
		if(!FLASH0_FSR.B.D0BUSY && !FLASH0_FSR.B.D1BUSY && !FLASH0_FSR.B.PBUSY) {
			/* write is finished */
			Fls_AccFinish(fls_address);

			if(FLASH_FSR.B.VER == 1) {
				/* verification error detected -- abort */
				fls_state = FLS_STATE_FAILED;
				Dem_ReportErrorStatus(FLS_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
				return 0;
			}

			/* advance to the next page to write */
			fls_address += fls_page_size;
			fls_length -= fls_page_size;
			fls_data += fls_page_size;

			fls_state = FLS_STATE_TOSTART;
		}
	}

	if(fls_state == FLS_STATE_TOSTART) {
		/* start a new write cycle */
		if(fls_length <= 0) {
			/* finished */
			fls_state = FLS_STATE_IDLE;
			return 0;
		}
		else {
			fls_state = FLS_STATE_INPROGRESS;
#if defined(FLS_DEV_ERROR_DETECT) && (FLS_DEV_ERROR_DETECT == STD_ON)
			/* check whether the memory is erased */
			for(i=0; i<fls_page_size; i++) {
				FLS_ASSERT(*(fls_address + i) == 0, FLS_WRITE_API_ID, FLS_E_VERIFY_WRITE_FAILED);
				if(ts_error != 0) {
					fls_state = FLS_STATE_FAILED;
					return 0;
				}
			}
#endif
			if(fls_cfg->ram_image_addr != NULL_PTR) {
				Fls_AccCopyWriteToRam(fls_cfg->ram_image_addr);
				write_func = (Fls_WriteFuncPtr)fls_cfg->ram_image_addr;
				write_func(fls_address, fls_data, fls_page_size);
				Fls_AccClearWriteFromRam(fls_cfg->ram_image_addr);
			}
			else {
				Fls_AccWritePage(fls_address, fls_data, fls_page_size);
			}
		}
	}

	return 1;
}


Std_ReturnType Fls_Write(Fls_AddressType dst_addr, const uint8* src_addr, Fls_LengthType length)
{
	FLS_ASSERT_RETERR(fls_cfg != NULL_PTR, FLS_WRITE_API_ID, FLS_E_UNINIT);
	FLS_ASSERT_RETERR(length > 0, FLS_WRITE_API_ID, FLS_E_PARAM_LENGTH);
	FLS_ASSERT_RETERR(fls_proc_func == NULL_PTR, FLS_WRITE_API_ID, FLS_E_BUSY);

	fls_address = fls_cfg->base_address + dst_addr / 4;
	fls_page_size = Fls_GetPageSize(fls_address);

	FLS_ASSERT_RETERR(fls_page_size != 0, FLS_WRITE_API_ID, FLS_E_PARAM_ADDRESS);
	FLS_ASSERT_RETERR(((fls_address >= FLS_START_PFLASH)  && (fls_address+length/4 <= FLS_END_PFLASH)) ||
			          ((fls_address >= FLS_START_DFLASH0) && (fls_address+length/4 <= FLS_END_DFLASH0)) ||
			          ((fls_address >= FLS_START_DFLASH1) && (fls_address+length/4 <= FLS_END_DFLASH1)),
			          FLS_INIT_API_ID, FLS_E_PARAM_CONFIG);

	/* calculate the number of words to program - always round up to a page */
	fls_length = (((length / 4) + (fls_page_size - 1)) / fls_page_size) * fls_page_size;

	fls_data = (uint32 *)src_addr;

	fls_state = FLS_STATE_TOSTART;
	fls_proc_func = Fls_ProcessWrite;

	return E_OK;
}


static uint32 Fls_ProcessRead(void)
{
	int i;

	for(i=0; i<fls_page_size; i++) {
		*fls_data = *fls_address;
		fls_length--;
		fls_data++;
		fls_address++;

		if(fls_length == 0) {
			/* finished */
			fls_state = FLS_STATE_IDLE;
			return 0;
		}
	}

	return 1;
}



Std_ReturnType Fls_Read(Fls_AddressType src_addr, uint8* dst_addr, Fls_LengthType length)
{
	FLS_ASSERT_RETERR(fls_cfg != NULL_PTR, FLS_READ_API_ID, FLS_E_UNINIT);
	FLS_ASSERT_RETERR(length > 0, FLS_READ_API_ID, FLS_E_PARAM_LENGTH);
	FLS_ASSERT_RETERR(fls_proc_func == NULL_PTR, FLS_READ_API_ID, FLS_E_BUSY);
	FLS_ASSERT_RETERR(dst_addr != NULL_PTR, FLS_READ_API_ID, FLS_E_PARAM_DATA);

	fls_address = fls_cfg->base_address + (uint32)src_addr / 4;
	fls_page_size = Fls_GetPageSize(fls_address);

	FLS_ASSERT_RETERR(fls_page_size != 0, FLS_READ_API_ID, FLS_E_PARAM_ADDRESS);
	FLS_ASSERT_RETERR(((fls_address >= FLS_START_PFLASH)  && (fls_address+length/4 <= FLS_END_PFLASH)) ||
			          ((fls_address >= FLS_START_DFLASH0) && (fls_address+length/4 <= FLS_END_DFLASH0)) ||
			          ((fls_address >= FLS_START_DFLASH1) && (fls_address+length/4 <= FLS_END_DFLASH1)),
			          FLS_INIT_API_ID, FLS_E_PARAM_CONFIG);

	/* calculate the number of words to read - always round up to a word */
	fls_length = (length + 3) / 4;

	fls_data = (uint32 *)dst_addr;

	fls_state = FLS_STATE_TOSTART;
	fls_proc_func = Fls_ProcessRead;

	return E_OK;
}


#if defined(FLS_COMPARE_API) && (FLS_COMPARE_API == STD_ON)

static uint32 Fls_ProcessCompare(void)
{
	int i;

	for(i=0; i<fls_page_size; i++) {
		if(*fls_data != *fls_address) {
			/* not equal -- abort */
			fls_state = FLS_STATE_INEQUAL;
			return 0;
		}
		fls_length--;
		fls_data++;
		fls_address++;

		if(fls_length == 0) {
			/* finished */
			fls_state = FLS_STATE_IDLE;
			return 0;
		}
	}

	return 1;
}


Std_ReturnType Fls_Compare(Fls_AddressType src_addr, const uint8* dst_addr, Fls_LengthType length)
{
	FLS_ASSERT_RETERR(fls_cfg != NULL_PTR, FLS_COMPARE_API_ID, FLS_E_UNINIT);
	FLS_ASSERT_RETERR(length > 0, FLS_COMPARE_API_ID, FLS_E_PARAM_LENGTH);
	FLS_ASSERT_RETERR(fls_proc_func == NULL_PTR, FLS_COMPARE_API_ID, FLS_E_BUSY);
	FLS_ASSERT_RETERR(dst_addr != NULL_PTR, FLS_COMPARE_API_ID, FLS_E_PARAM_DATA);

	fls_address = fls_cfg->base_address + (uint32)src_addr / 4;
	fls_page_size = Fls_GetPageSize(fls_address);

	FLS_ASSERT_RETERR(fls_page_size != 0, FLS_COMPARE_API_ID, FLS_E_PARAM_ADDRESS);
	FLS_ASSERT_RETERR(((fls_address >= FLS_START_PFLASH)  && (fls_address+length/4 <= FLS_END_PFLASH)) ||
			          ((fls_address >= FLS_START_DFLASH0) && (fls_address+length/4 <= FLS_END_DFLASH0)) ||
			          ((fls_address >= FLS_START_DFLASH1) && (fls_address+length/4 <= FLS_END_DFLASH1)),
			          FLS_INIT_API_ID, FLS_E_PARAM_CONFIG);

	/* calculate the number of words to read - always round up to a word */
	fls_length = (length + 3) / 4;

	fls_data = (uint32 *)dst_addr;

	fls_state = FLS_STATE_TOSTART;
	fls_proc_func = Fls_ProcessCompare;

	return E_OK;
}

#endif


#if defined(FLS_CANCEL_API) && (FLS_CANCEL_API == STD_ON)

void Fls_Cancel(void)
{
	FLS_ASSERT_RETVOID(fls_cfg != NULL_PTR, FLS_CANCEL_API_ID, FLS_E_UNINIT);

	if(fls_state == FLS_STATE_TOSTART || fls_state == FLS_STATE_INPROGRESS) {
		fls_proc_func = NULL_PTR;
		fls_state = FLS_STATE_CANCELED;
		while(FLASH0_FSR.B.PBUSY || FLASH0_FSR.B.D0BUSY || FLASH0_FSR.B.D1BUSY) {
		}
		Fls_AccFinish(fls_address);
	}
}

#endif


#if defined(FLS_GETSTATUS_API) && (FLS_GETSTATUS_API == STD_ON)

MemIf_StatusType Fls_GetStatus(void)
{
	if(fls_cfg == NULL_PTR) {
		return MEMIF_UNINIT;
	}
	if(fls_proc_func == NULL_PTR) {
		return MEMIF_IDLE;
	}

	return MEMIF_BUSY;
}

#endif


#if defined(FLS_GETJOBRESULT_API) && (FLS_GETJOBRESULT_API == STD_ON)

MemIf_JobResultType Fls_GetJobResult(void)
{
	switch(fls_state) {
		case FLS_STATE_IDLE:
			return MEMIF_JOB_OK;
		case FLS_STATE_TOSTART:
		case FLS_STATE_INPROGRESS:
			return MEMIF_JOB_PENDING;
		case FLS_STATE_FAILED:
			return MEMIF_JOB_FAILED;
		case FLS_STATE_INEQUAL:
			return MEMIF_BLOCK_INCONSISTENT;
		case FLS_STATE_CANCELED:
			return MEMIF_JOB_CANCELLED;
		default:
			return MEMIF_JOB_OK;
	}
}

#endif


#if defined(FLS_SETMODE_API) && (FLS_SETMODE_API == STD_ON)

void Fls_SetMode(MemIf_ModeType mode)
{
	FLS_ASSERT_RETVOID(fls_cfg != NULL_PTR, FLS_SETMODE_API_ID, FLS_E_UNINIT);
	FLS_ASSERT_RETVOID(fls_proc_func == NULL, FLS_SETMODE_API_ID, FLS_E_BUSY);

	if(mode == MEMIF_MODE_SLOW) {
		fls_proc_time = fls_cfg->proc_time_slow;
	}
	else {
		fls_proc_time = fls_cfg->proc_time_fast;
	}
}

#endif


void Fls_MainFunction(void)
{
	uint32 pending;
	uint32 end_time;

	FLS_ASSERT_RETVOID(fls_cfg != NULL_PTR, FLS_MAINFUNCTION_API_ID, FLS_E_UNINIT);

	/* if nothing to do then just return */
	if(fls_proc_func == NULL_PTR) {
		return;
	}

	/* keep on processing until finished or the allowed time has been elapsed */
	end_time = STM_TIM0.U + fls_proc_time;
	do {
		pending = fls_proc_func();
		if(pending == 0) {
			/* finished the operation */
			fls_proc_func = NULL_PTR;

			/* call the notification */
			if(fls_state == FLS_STATE_IDLE) {
				if(fls_cfg->job_end_notification != NULL_PTR) {
					fls_cfg->job_end_notification();
				}
			}
			else {
				if(fls_cfg->job_error_notification != NULL_PTR) {
					fls_cfg->job_error_notification();
				}
			}
			return;
		}
	} while((sint32)(STM_TIM0.U - end_time) < 0);
}
#define FLS_STOP_SEC_CODE
#include "MemMap.h"
