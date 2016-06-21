/*
 * Eep_Lcfg.c
 *
 *  Created on: 2015-2-2
 *      Author: Administrator
 */

#include "Eep.h"
#include "Ea_Cbk.h"
#include "Prj_MainCfg.h"


#define EEP_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
const Eep_ConfigType eep_cfg =
{
	{
		SequenceIdEepReadStatus,
		SequenceIdEepReadData,
		SequenceIdEepWriteData
	},
	MEMIF_MODE_SLOW,
	EEP_SIZE,
	EEP_PAGE_SIZE,
};
#define EEP_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"


#define EEP_START_SEC_CODE
#include "MemMap.h"

 void EepJobEndNotification(void)
 {

 }
 void EepJobErrorNotification(void)
 {

 }
#define EEP_STOP_SEC_CODE
#include "MemMap.h"

