/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/17 15:31:31MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM_Priv.h  $
 *  Revision 1.3 2011/03/17 15:31:31MEZ Cotor Vlad RGB (COTORV) 
 *  Update NvM calling function due to power control
 *  Revision 1.2 2010/07/12 14:26:15MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef NVM_PRIV_H_
#define NVM_PRIV_H_

#include "team-s.h"
#include "NvM_Cfg.h"

/* status of variable block_status */
#define NVM_BLOCK_STATUS_UNKNOWN		0	/* the RAM image has not yet been verified. */
#define NVM_BLOCK_STATUS_IMAGE_CHANGED	1	/* the RAM image has been changed. */
#define NVM_BLOCK_STATUS_IMAGE_DEFAULT	2	/* the default have been loaded and image is unchanged. */
#define NVM_BLOCK_STATUS_IMAGE_WRITTEN	3	/* the RAM image and the EE image is identical. */
#define NVM_BLOCK_STATUS_IMAGE_INVALID	4	/* the RAM image is invalid since it cannot be restored. */


#if defined(NVM_DEV_ERROR_DETECT) && (NVM_DEV_ERROR_DETECT == STD_ON)
#  define NVM_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, NVM_MODULE_ID, NVM_INSTANCE_ID, api_id, error_id)
#  define NVM_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, NVM_MODULE_ID, NVM_INSTANCE_ID, api_id, error_id)
#  define NVM_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, NVM_MODULE_ID, NVM_INSTANCE_ID, api_id, error_id)
#else
#  define NVM_ASSERT_RETERR(cond, api_id, error_id)
#  define NVM_ASSERT_RETVOID(cond, api_id, error_id)
#  define NVM_ASSERT(cond, api_id, error_id)
#endif

typedef struct {
	uint32 error_cnt;
	uint16 current_block;
	uint8 cancel_req;
	uint8 initialized;
	boolean rw_all_in_progress;
} NvM_Var;


#define NVM_START_SEC_VAR_32BIT
#include "MemMap.h"

extern const NvM_ConfigType nvm_cfg;
extern NvM_Var nvm_var;
extern NvM_Block1 nvm_block1;


#define NVM_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define NVM_NO_OF_USED_BLOCKS             nvm_cfg.num_of_blocks      /* number of used NVMY blocks */


#define NVM_START_SEC_CODE
#include "MemMap.h"

extern void NvM_Schedule(void);

#define NVM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* NVM_PRIV_H_ */


