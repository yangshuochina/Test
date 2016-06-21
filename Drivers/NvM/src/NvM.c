/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/12/14 09:40:07MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM.c  $
 *  Revision 1.3 2010/12/14 09:40:07MEZ Cotor Vlad RGB (COTORV) 
 *  NvM Driver update
 *  Revision 1.2 2010/07/12 14:25:08MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "NvM.h"
#include "NvM_Priv.h"
#include "MemIf.h"

#define NVM_START_SEC_VAR_32BIT
#include "MemMap.h"

NvM_Var nvm_var;
NvM_Block1 nvm_block1;

#define NVM_STOP_SEC_VAR_32BIT
#include "MemMap.h"



#define NVM_START_SEC_CODE
#include "MemMap.h"




void NvM_Init(void)
{
	int i;

	for(i=1; i<nvm_cfg.num_of_blocks; i++) {
		if(nvm_cfg.blocks[i].status != NULL_PTR) {
			nvm_cfg.blocks[i].status->ram_image = nvm_cfg.blocks[i].ram_image;
			nvm_cfg.blocks[i].status->result = NVM_REQ_OK;
			nvm_cfg.blocks[i].status->req = NVM_REQ_IDLE;
		}
		else {
			NVM_ASSERT_RETVOID(nvm_cfg.blocks[i].status != NULL_PTR, NVM_INIT_API_ID, NVM_E_PARAM_CONFIG);
		}
	}

	nvm_cfg.blocks[1].status->ram_image = (uint8 *)&nvm_block1;

	nvm_var.error_cnt = 0;
	nvm_var.rw_all_in_progress = FALSE;
	nvm_var.cancel_req = 0;
	nvm_var.current_block = 0;
	nvm_var.initialized = 1;
}


void NvM_ReadAll(void)
{
	int i;

	NVM_ASSERT_RETVOID(nvm_var.initialized != 0, NVM_READALL_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETVOID(!nvm_var.rw_all_in_progress, NVM_READALL_API_ID, NVM_E_BLOCK_PENDING);

	nvm_var.error_cnt = 0;

	nvm_var.current_block = 1;
	if(nvm_cfg.blocks[1].status != NULL_PTR) {
		nvm_cfg.blocks[1].status->req = NVM_REQ_VERIFY_ID;
		nvm_cfg.blocks[1].status->result = NVM_REQ_PENDING;
	}

	for(i=2; i<nvm_cfg.num_of_blocks; i++) {
		if(nvm_cfg.blocks[i].enable_readall) {
			if(nvm_cfg.blocks[i].status != NULL_PTR) {
				if(nvm_cfg.blocks[i].status->ram_image != NULL_PTR) {
					if(nvm_cfg.blocks[i].status->req == NVM_REQ_IDLE) {
						if(nvm_cfg.blocks[i].crc_type != NVM_CRC_NONE) {
							/* CRC available */
							nvm_cfg.blocks[i].status->req = NVM_REQ_VERIFY_CRC;
							nvm_cfg.blocks[i].status->result = NVM_REQ_PENDING;
						}
						else {
							/* no CRC */
							if(nvm_cfg.blocks[i].status->block_status == NVM_BLOCK_STATUS_UNKNOWN) {
								nvm_cfg.blocks[i].status->req = NVM_REQ_LOAD;
								nvm_cfg.blocks[i].status->result = NVM_REQ_PENDING;
							}
						}
					}
				}
			}
		}

		if(nvm_cfg.blocks[i].status->req == NVM_REQ_IDLE) {
			nvm_cfg.blocks[i].status->result = NVM_REQ_BLOCK_SKIPPED;
		}
	}
#if defined(NVM_DRV_MODE_SWITCH) && (NVM_DRV_MODE_SWITCH == STD_ON)
	MemIf_SetMode(MEMIF_MODE_FAST);
#endif

	nvm_var.rw_all_in_progress = TRUE;
}


void NvM_WriteAll(void)
{
	int i;

	NVM_ASSERT_RETVOID(nvm_var.initialized != 0, NVM_WRITEALL_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETVOID(!nvm_var.rw_all_in_progress, NVM_WRITEALL_API_ID, NVM_E_BLOCK_PENDING);

	nvm_var.error_cnt = 0;

	for(i=1; i<nvm_cfg.num_of_blocks; i++) {
		if(nvm_cfg.blocks[i].status != NULL_PTR) {
			if(nvm_cfg.blocks[i].status->ram_image != NULL_PTR) {
				if(nvm_cfg.blocks[i].status->block_status != NVM_BLOCK_STATUS_IMAGE_WRITTEN
						&& nvm_cfg.blocks[i].status->block_status != NVM_BLOCK_STATUS_IMAGE_INVALID
						&& nvm_cfg.blocks[i].status->block_status != NVM_BLOCK_STATUS_UNKNOWN
						) {
					nvm_cfg.blocks[i].status->req = NVM_REQ_WRITE;
					nvm_cfg.blocks[i].status->result = NVM_REQ_PENDING;
				}
				else {
					nvm_cfg.blocks[i].status->result = NVM_REQ_BLOCK_SKIPPED;
				}
			}
		}
	}

#if defined(NVM_DRV_MODE_SWITCH) && (NVM_DRV_MODE_SWITCH == STD_ON)
	MemIf_SetMode(MEMIF_MODE_FAST);
#endif

	nvm_var.rw_all_in_progress = TRUE;
}


void NvM_CancelWriteAll(void)
{
	NVM_ASSERT_RETVOID(nvm_var.initialized != 0, NVM_CANCELWRITEALL_API_ID, NVM_E_NOT_INITIALIZED);

	nvm_var.cancel_req = 1;
}


void NvM_SetRamBlockStatus(NvM_BlockIdType block_id, boolean block_changed)
{
	NVM_ASSERT_RETVOID(nvm_var.initialized != 0, NVM_SETRAMBLOCKSTATUS_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETVOID(block_id < nvm_cfg.num_of_blocks, NVM_SETRAMBLOCKSTATUS_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETVOID(nvm_cfg.blocks[block_id].status != NULL_PTR, NVM_SETRAMBLOCKSTATUS_API_ID, NVM_E_PARAM_CONFIG);
	NVM_ASSERT_RETVOID(nvm_cfg.blocks[block_id].status->req == NVM_REQ_IDLE, NVM_SETRAMBLOCKSTATUS_API_ID, NVM_E_BLOCK_PENDING);

	if(block_changed) {
		nvm_cfg.blocks[block_id].status->block_status = NVM_BLOCK_STATUS_IMAGE_CHANGED;
		if(nvm_cfg.blocks[block_id].crc_type != NVM_CRC_NONE) {
			nvm_cfg.blocks[block_id].status->req = NVM_REQ_START_CALC_CRC;
			nvm_cfg.blocks[block_id].status->result = NVM_REQ_PENDING;
		}
	}
	else {
		nvm_cfg.blocks[block_id].status->block_status = NVM_BLOCK_STATUS_IMAGE_WRITTEN;
	}
}


void NvM_GetErrorStatus(NvM_BlockIdType block_id, NvM_RequestResultType *req_result_ptr)
{
	NVM_ASSERT_RETVOID(nvm_var.initialized != 0, NVM_GETERRORSTATUS_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETVOID(block_id < nvm_cfg.num_of_blocks, NVM_GETERRORSTATUS_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETVOID(req_result_ptr != NULL_PTR, NVM_GETERRORSTATUS_API_ID, NVM_E_PARAM_DATA);
	NVM_ASSERT_RETVOID(nvm_cfg.blocks[block_id].status != NULL_PTR, NVM_GETERRORSTATUS_API_ID, NVM_E_PARAM_CONFIG);

	*req_result_ptr = nvm_cfg.blocks[block_id].status->result;
}



Std_ReturnType NvM_ReadBlock(NvM_BlockIdType block_id, uint8 *dst_ptr)
{
	NVM_ASSERT_RETERR(nvm_var.initialized != 0, NVM_READBLOCK_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status != NULL_PTR, NVM_READBLOCK_API_ID, NVM_E_PARAM_CONFIG);
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status->req == NVM_REQ_IDLE, NVM_READBLOCK_API_ID, NVM_E_BLOCK_PENDING);
	NVM_ASSERT_RETERR(block_id >= 2, NVM_READBLOCK_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETERR(block_id < nvm_cfg.num_of_blocks, NVM_READBLOCK_API_ID, NVM_E_PARAM_BLOCK_ID);

	if(dst_ptr != NULL_PTR) {
		nvm_cfg.blocks[block_id].status->ram_image = dst_ptr;
	}

	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status->ram_image != NULL_PTR, NVM_READBLOCK_API_ID, NVM_E_PARAM_ADDRESS);

	if(nvm_cfg.blocks[block_id].status->req == NVM_REQ_IDLE) {
		if(nvm_cfg.blocks[block_id].crc_type != NVM_CRC_NONE) {
			/* CRC available */
			nvm_cfg.blocks[block_id].status->req = NVM_REQ_VERIFY_CRC;
			nvm_cfg.blocks[block_id].status->result = NVM_REQ_PENDING;
		}
		else {
			/* no CRC */
			if(nvm_cfg.blocks[block_id].status->block_status == NVM_BLOCK_STATUS_UNKNOWN) {
				nvm_cfg.blocks[block_id].status->req = NVM_REQ_LOAD;
				nvm_cfg.blocks[block_id].status->result = NVM_REQ_PENDING;
			}
		}
	}

	return E_OK;
}


Std_ReturnType NvM_WriteBlock(NvM_BlockIdType block_id, uint8 *src_ptr)
{
	NVM_ASSERT_RETERR(nvm_var.initialized != 0, NVM_WRITEBLOCK_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETERR(block_id >= 2, NVM_WRITEBLOCK_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETERR(block_id < nvm_cfg.num_of_blocks, NVM_WRITEBLOCK_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status != NULL_PTR, NVM_WRITEBLOCK_API_ID, NVM_E_PARAM_CONFIG);
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status->req == NVM_REQ_IDLE, NVM_WRITEBLOCK_API_ID, NVM_E_BLOCK_PENDING);

	if(src_ptr != NULL_PTR) {
		nvm_cfg.blocks[block_id].status->ram_image = src_ptr;
	}
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status->ram_image != NULL_PTR, NVM_WRITEBLOCK_API_ID, NVM_E_PARAM_ADDRESS);

	if(nvm_cfg.blocks[block_id].status->block_status != NVM_BLOCK_STATUS_IMAGE_WRITTEN &&
			nvm_cfg.blocks[block_id].status->block_status != NVM_BLOCK_STATUS_UNKNOWN) {
		nvm_cfg.blocks[block_id].status->req = NVM_REQ_WRITE;
		nvm_cfg.blocks[block_id].status->result = NVM_REQ_PENDING;
	}

	return E_OK;
}


Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType block_id, uint8 *dst_ptr)
{
	NVM_ASSERT_RETERR(nvm_var.initialized != 0, NVM_RESTOREBLOCKDEFAULTS_API_ID, NVM_E_NOT_INITIALIZED);
	NVM_ASSERT_RETERR(block_id >= 2, NVM_RESTOREBLOCKDEFAULTS_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETERR(block_id < nvm_cfg.num_of_blocks, NVM_RESTOREBLOCKDEFAULTS_API_ID, NVM_E_PARAM_BLOCK_ID);
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status != NULL_PTR, NVM_RESTOREBLOCKDEFAULTS_API_ID, NVM_E_PARAM_CONFIG);
	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status->req == NVM_REQ_IDLE, NVM_RESTOREBLOCKDEFAULTS_API_ID, NVM_E_BLOCK_PENDING);

	if(dst_ptr != NULL_PTR) {
		nvm_cfg.blocks[block_id].status->ram_image = dst_ptr;
	}

	NVM_ASSERT_RETERR(nvm_cfg.blocks[block_id].status->ram_image != NULL_PTR, NVM_RESTOREBLOCKDEFAULTS_API_ID, NVM_E_PARAM_ADDRESS);

	nvm_cfg.blocks[block_id].status->req = NVM_REQ_RECOVER_BLOCK;
	nvm_cfg.blocks[block_id].status->result = NVM_REQ_PENDING;

	return E_OK;
}


void NvM_MainFunction(void)
{
	if(nvm_var.initialized != 0) {
		NvM_Schedule();
	}
}

#define NVM_STOP_SEC_CODE
#include "MemMap.h"

