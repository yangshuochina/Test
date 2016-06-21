/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Mcu.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/08/19 19:25:57MESZ $
 *                      $Author: Opel Martin RGB (OPELM) $
 *
 * ***************************************************************************
 *
 *  $Log: Mcu.c  $
 *  Revision 1.2 2010/08/19 19:25:57MESZ Opel Martin RGB (OPELM) 
 *  - added missing memory section
 *  Revision 1.1 2010/05/19 14:18:03MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Mcu/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Mcu.h"
#include "Mcu_Priv.h"

#define MCU_START_SEC_VAR_32BIT
#include "MemMap.h"

const Mcu_ConfigType *mcu_cfg;

#define MCU_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define MCU_START_SEC_CODE
#include "MemMap.h"

void Mcu_Init(const Mcu_ConfigType *cfg)
{
	mcu_cfg = NULL_PTR;

	MCU_ASSERT_RETVOID(cfg != NULL_PTR, MCU_INIT_API_ID, MCU_E_PARAM_CONFIG);
	MCU_ASSERT_RETVOID(cfg->Mcu_RamConfig != NULL_PTR || cfg->Mcu_NumberOfRamSections==0, MCU_INIT_API_ID, MCU_E_PARAM_CONFIG);
	MCU_ASSERT_RETVOID(cfg->Mcu_ModeConfig != NULL_PTR || cfg->Mcu_NumberOfMcuModes==0, MCU_INIT_API_ID, MCU_E_PARAM_CONFIG);
	MCU_ASSERT_RETVOID(cfg->Mcu_ClockConfig != NULL_PTR || cfg->Mcu_NumberOfClockSettings==0, MCU_INIT_API_ID, MCU_E_PARAM_CONFIG);

	mcu_cfg = cfg;
}

#define MCU_STOP_SEC_CODE
#include "MemMap.h"
