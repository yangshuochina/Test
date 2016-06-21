/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Mcu_Reset.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:18:09MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Mcu_Reset.c  $
 *  Revision 1.1 2010/05/19 14:18:09MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Mcu/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Os.h"
#include "Mcu.h"
#include "Mcu_Priv.h"

#define MCU_START_SEC_CODE
#include "MemMap.h"


void Mcu_PerformReset(void)
{
	__disable();

	ts_endinit_clear();

	/* the software reset shall cause a system reset */
	SCU_RSTCON.B.SW = 1;

	/* request the reset */
	SCU_SWRSTCON.B.SWRSTREQ = 1;

	/* wait until it occurs */
	while(1) {
	}
}


Mcu_ResetType Mcu_GetResetReason(void)
{
	Mcu_ResetType reset_type = MCU_RESET_UNDEFINED;

	if(SCU_RSTSTAT.B.WDT) {
		reset_type = MCU_WATCHDOG_RESET;
	}
	else if(SCU_RSTSTAT.B.SW) {
		reset_type = MCU_SW_RESET;
	}
	else if(SCU_RSTSTAT.B.PORST) {
		reset_type = MCU_POWER_ON_RESET;
	}

	return reset_type;
}


Mcu_RawResetType Mcu_GetResetRawValue(void)
{
	return SCU_RSTSTAT.U;
}
#define MCU_STOP_SEC_CODE
#include "MemMap.h"

