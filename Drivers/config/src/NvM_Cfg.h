/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/17 15:30:57MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM_Cfg.h  $
 *  Revision 1.5 2011/03/17 15:30:57MEZ Cotor Vlad RGB (COTORV) 
 *  Update NvM calling function due to power control
 *  Revision 1.4 2011/02/22 15:04:56MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.3 2010/11/22 09:00:31MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 13:57:56MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef NVM_CFG_H_
#define NVM_CFG_H_

#include "NvM_Types.h"


#define NVM_DEV_ERROR_DETECT        STD_ON

#define NVM_INSTANCE_ID    (0u)

#define NVM_MAX_NUM_OF_BLOCKS             (18u)      /**< The maximum possible number of NVM blocks managed by NvM */

#define NVM_SET_RAM_BLOCK_STATUS_API      (STD_ON)   /**< Enable or disable the NvM_SetRamBlockStatus() interface. */

#define NVM_DRV_MODE_SWITCH               (STD_ON)   /**< Switch to fast mode for NvM_ReadAll() and NvM_WriteAll(). */

#define NVM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"

extern boolean ecum_nvm_job_done;
extern boolean ecum_nvm_job_error;

#define NVM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"


#endif /* NVM_CFG_H_ */

