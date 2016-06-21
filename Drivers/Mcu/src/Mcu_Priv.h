/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Mcu_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:18:06MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Mcu_Priv.h  $
 *  Revision 1.1 2010/05/19 14:18:06MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Mcu/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef MCU_PRIV_H_
#define MCU_PRIV_H_

#include "team-s.h"
#include "Mcu_Cfg.h"


#if defined(MCU_DEV_ERROR_DETECT) && (MCU_DEV_ERROR_DETECT == STD_ON)
#  define MCU_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, MCU_MODULE_ID, MCU_INSTANCE_ID, api_id, error_id)
#  define MCU_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, MCU_MODULE_ID, MCU_INSTANCE_ID, api_id, error_id)
#else
#  define MCU_ASSERT_RETERR(cond, api_id, error_id)
#  define MCU_ASSERT_RETVOID(cond, api_id, error_id)
#endif


#define MCU_START_SEC_VAR_32BIT
#include "MemMap.h"

extern const Mcu_ConfigType *mcu_cfg;

#define MCU_STOP_SEC_VAR_32BIT
#include "MemMap.h"



#endif /* MCU_PRIV_H_ */
