/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/28 10:19:37MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: SchM.c  $
 *  Revision 1.4 2011/02/28 10:19:37MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.3 2010/07/16 14:37:14MESZ Cotor Vlad RGB (COTORV) 
 *  Update SchM and EcuM due to ADC driver update
 *  Revision 1.2 2010/07/12 09:49:18MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:26:15MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/ServiceLayer/SchM/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "SchM.h"
#include "SchM_cfg.h"
#include "SchM_Priv.h"

#include "Adc.h"
#include "Adc_Cfg.h"
#include "NvM.h"
#include "Fee.h"
#include "Fls.h"
#include "Spi.h"
#include "Eep.h"
#include "Ea.h"


#include "os.h"

/**
 * @todo include OSEK task definitions
 */

#define SCHM_START_SEC_VAR_32BIT
#include "MemMap.h"

/**
 * Bitfield containing the man function activation of BSW modules.
 */
static BITFIELD32(schm_module_activation);

#define SCHM_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define SCHM_START_SEC_CODE
#include "MemMap.h"

void SchM_Init(void) {
	SchM_Os_SetRelAlarm(Alarm_SCHM_TASK_CYCLE_10MS, SCHM_TASK_START_TIME_10MS, SCHM_TASK_CYCLE_TIME_10MS);
}


void SchM_Deinit(void) {

}

SchM_ReturnType SchM_ActMainFunction(uint8 instance, uint8 activationPoint) {
	SCHM_ASSERT_RETVAL(activationPoint < (sizeof(schm_module_activation) * 8),
			instance,
			SCHM_ACT_MAIN_FUNCTION_API_ID,
			SCHM_E_LIMIT, SCHM_E_LIMIT);
	SCHM_ASSERT_RETVAL(BITFIELD32_GETBIT(schm_module_activation, activationPoint) == 0,
			SCHM_ACT_MAIN_FUNCTION_API_ID,
            instance,
			SCHM_E_STATE,
			SCHM_E_STATE);

	BITFIELD32_SETBIT(schm_module_activation, activationPoint);

	return SCHM_E_OK;
}

SchM_ReturnType SchM_CancelMainFunction(uint8 instance, uint8 activationPoint) {
	SCHM_ASSERT_RETVAL(activationPoint < (sizeof(schm_module_activation) * 8),
			instance,
			SCHM_CANCEL_MAIN_FUNCTION_API_ID,
			SCHM_E_NOFUNC, SCHM_E_NOFUNC);
	SCHM_ASSERT_RETVAL(BITFIELD32_GETBIT(schm_module_activation, activationPoint) == 0,
			instance,
			SCHM_CANCEL_MAIN_FUNCTION_API_ID,
			SCHM_E_NOFUNC, SCHM_E_NOFUNC);

	BITFIELD32_CLRBIT(schm_module_activation, activationPoint);

	return SCHM_E_OK;
}

SchM_Os_Task(SCHM_TASK_CYCLE_10MS) {


#if defined(ADC_POLLING_MODE) && (ADC_POLLING_MODE == STD_ON)
	if(BITFIELD32_GETBIT(schm_module_activation, SCHM_ADC_ACTIVATION_POINT_1) != 0) {
		Adc_MainFunction_Handling();
	}
#endif

/*   if(BITFIELD32_GETBIT(schm_module_activation, SCHM_NVM_ACTIVATION_POINT_1) != 0) { */
      NvM_MainFunction();
/*   } */

	if(BITFIELD32_GETBIT(schm_module_activation, SCHM_EA_ACTIVATION_POINT_1) != 0) {
		Ea_MainFunction();
	}

	if(BITFIELD32_GETBIT(schm_module_activation, SCHM_EEP_ACTIVATION_POINT_1) != 0) {
		Eep_MainFunction();
	}

	if(BITFIELD32_GETBIT(schm_module_activation, SCHM_FLS_ACTIVATION_POINT_1) != 0) {
		Fls_MainFunction();
	}

	if(BITFIELD32_GETBIT(schm_module_activation, SCHM_SPI_ACTIVATION_POINT_1) != 0) {
		Spi_MainFunction_Handling();
	}

	SchM_Os_TerminateTask();
}


#define SCHM_STOP_SEC_CODE
#include "MemMap.h"
