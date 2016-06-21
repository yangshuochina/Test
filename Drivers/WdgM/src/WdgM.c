/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgM.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:18:13MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: WdgM.c  $
 *  Revision 1.3 2011/02/02 09:18:13MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 09:52:22MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:26:35MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/ServiceLayer/WdgM/AA000/src/project.pj
 *
 * ***************************************************************************/
#include "WdgIf.h"
#include "WdgM.h"
#include "WdgM_Priv.h"

#include "Mcu.h"
#include "Gpt.h"
#include "Det.h"
#include "Dem.h"



#define WDGM_START_SEC_VAR_32BIT
#include "MemMap.h"

WdgM_Container wdgm; /*wdgm driver variable container */
const WdgM_ConfigType*  wdgm_cfg = NULL_PTR; /* WDGM  config pointer */

#define WDGM_STOP_SEC_VAR_32BIT
#include "MemMap.h"

/* Private functions prototypes */
static void wdgm_Initialize_container(WdgM_ModeType CurrentModeId, const WdgM_ConfigType* CfgPtr);
static void WdgM_TriggerAlgorithm (void);

#define WDGM_START_SEC_CODE
#include "MemMap.h"

/*******************************************************************
 * It is a local Interface
 * 1. Reset the wdgm container variables.
 * 2. Some are also rest in init and mode functions
 * 3. It is used in Mode and Init interface
 ******************************************************************/

static void wdgm_Initialize_container(WdgM_ModeType CurrentModeId,const WdgM_ConfigType* CfgPtr)
{
	WdgM_SupervisedEntityIdType Index;

	#if defined (WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)

	WdgM_SupervisedEntityIdType SEId;
	WdgMModeType CurrentMode =CfgPtr->WdgMModePtr[CurrentModeId]; 	/*get the passed mode configuration*/

	wdgm.GlobalStatus = WDGM_ALIVE_OK; /* Reset Global status */
	wdgm.CurExpCycTolerance=CurrentMode.WdgMExpiredSupervisionCycleTol; /*reset the expired cycle tolerance*/

	for(Index = 0x0U; Index < CurrentMode.WdgMNumberOfEntities; Index++)	   /* reset all other counters of SEs*/
	{
		SEId=CurrentMode.WdgMAliveSupervisionPtr[Index].WdgMSupervisedEntityId;

		wdgm.SEIndications[SEId] = 0x0U; 	/*Clear indications array*/
		wdgm.SERefCycles[SEId] = CurrentMode.WdgMAliveSupervisionPtr[Index].WdgMSupervisionReferenceCycle; /*Set number of reference cycles for the SE*/
		wdgm.SEExpCycles[SEId] = CurrentMode.WdgMAliveSupervisionPtr[Index].WdgMFailedSupervisionRefCycleTol; /*Set number of failed tolerance cycles for the SE*/
	}

	#endif

	for(Index = 0x0U; Index < WDGM_NUMBER_OF_WATCHDOGS_INSTANCES; Index++)
	{
		wdgm.wdg_TriggerCycles[Index] = 0x0U; /* Reset wdg trigger cycle counter*/
	}

}


/********************************************************************************
 * It is a local function.
 * 1. No error check needed
 * 2. if the global status is stopped, just return
 * 3. for all WDGs trigger it if WdgMTriggerReferenceCycle expires
 *********************************************************************************/

static void WdgM_TriggerAlgorithm (void)
{
	uint8 Index = 0x0U;
	WdgMModeType WdgMCurrentMode;

	WdgMCurrentMode=wdgm_cfg->WdgMModePtr[wdgm.CurrentMode];

	#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)

	/*Check the alive supervision global status	*/
	if(wdgm.GlobalStatus == WDGM_ALIVE_STOPPED)
	{
		/* If wdgm.GlobalStatus is WDGM_ALIVE_STOPPED no need to trigger the WDG
		 * So set the counter to max value to escape from looping.
		 */
		Index = WdgMCurrentMode.WdgMNumberOfWatchdogs;
	}

	#endif

	/*Go through all the configured WDG drivers for the current mode */
	/* Index is initialized earlier itself */
	for(; Index < WdgMCurrentMode.WdgMNumberOfWatchdogs; Index++)
	{
		wdgm.wdg_TriggerCycles[Index]++;
		/* trigger the Watchdog if the cycle period has passed */
		if(wdgm.wdg_TriggerCycles[Index] >= WdgMCurrentMode.WdgMTriggerPtr[Index].WdgMTriggerReferenceCycle)
		{
			WdgIf_Trigger(WdgMCurrentMode.WdgMTriggerPtr[Index].DeviceIndex);  /*Trigger WDG*/
			wdgm.wdg_TriggerCycles[Index] = 0x0U;          /* Reset cycle counter*/
		}/* else do nothing as the cycle period has not passed */
	}
}

/**************************************************************
 * 1. check default errors
 * 2. Start the WDG
 * 			raise error any config mismatch in WDG.
 * 				If mode change is allowed for the WDG, set to the mode.
 * 3. If the init mode of WDGM is GPT, start the GPT channel. else no.
 * 4. Init the RAM structure counter,status
 * 5. Set SEId status to default(only for configured).
 * 6. Assumption :WDG driver is already initialized!
 **************************************************************/

void WdgM_Init(const WdgM_ConfigType* ConfigPtr )
{
	uint8 Index;
	WdgMModeType WdgMInitCfg;

	WDGM_ASSERT_RETVOID(!(ConfigPtr == NULL_PTR), WDGM_INIT_API_ID, WDGM_E_PARAM_CONFIG);

	wdgm_cfg = NULL_PTR; /* set it after completing the init successfully*/
	WdgMInitCfg=ConfigPtr->WdgMModePtr[ConfigPtr->WdgMInitialMode]; /*store the initial mode data */

	/*set the mode of the watchdogs as required by the initial WdgM mode*/
	for(Index = 0x0U; Index < WdgMInitCfg.WdgMNumberOfWatchdogs; Index++)	 /*for all watchdogs in the initial mode*/
	{
		#if defined(WDGM_OFF_MODE_ENABLED) && (WDGM_OFF_MODE_ENABLED == STD_OFF)
		/* if Disabling not allowed by definition but configured? report error */
		WDGM_ASSERT_RETVOID(!(WdgMInitCfg.WdgMTriggerPtr[Index].WdgMWatchdogMode==WDGIF_OFF_MODE), WDGM_INIT_API_ID, WDGM_E_DISABLE_NOT_ALLOWED);
		#endif

		if( WdgIf_SetMode(WdgMInitCfg.WdgMTriggerPtr[Index].DeviceIndex, WdgMInitCfg.WdgMTriggerPtr[Index].WdgMWatchdogMode) == E_NOT_OK)
		{
			/*if the watchdog mode change failed & live supervision is enabled? */
			#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION==STD_ON)
			/* assume a global alive-supervision failure and set the global supervision status to WDGM_ALIVE_STOPPED*/
			wdgm.GlobalStatus = WDGM_ALIVE_STOPPED;
			#endif
			Dem_ReportErrorStatus(WDGM_E_SET_MODE, DEM_EVENT_STATUS_FAILED);
			return;
		}
	}

	/*If initial mode is GPT activated*/
	if(WdgMInitCfg.WdgMActivationType==WDGM_ACTIVATION_TYPE_GPT) /*if new mode is GPT activated*/
	{
		/*enable GPT notification & Start the timer */
		Gpt_EnableNotification( WdgMInitCfg.WdgMActivationGptPtr->WdgMGptChannelRef );
		Gpt_StartTimer(WdgMInitCfg.WdgMActivationGptPtr->WdgMGptChannelRef, WdgMInitCfg.WdgMActivationGptPtr->WdgMGptCycle);
	}

	wdgm.CurrentMode=ConfigPtr->WdgMInitialMode; /* Store the current WdgMMode in wdgm Container for future use*/
	wdgm_Initialize_container(wdgm.CurrentMode,ConfigPtr); /*initialize all global counters in the structure wdgm*/

	/*set the initial status of the supervised entities configured for the initial WdgM mode*/
	#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION==STD_ON)

	for(Index=0x0U;Index<WDGM_NUMBER_OF_SUPERVISED_ENTITIES;Index++)
	{
		wdgm.SEStatus[Index]=WDGM_ALIVE_DEACTIVATED; /*Set all supervised entity statuses to de activated state */
	}
	for(Index=0x0U; Index < WdgMInitCfg.WdgMNumberOfEntities; Index++)
	{
		if(WdgMInitCfg.WdgMAliveSupervisionPtr[Index].WdgMActivationActivatedStatus ==WDGM_SUPERVISION_ENABLED)
		{
			wdgm.SEStatus[WdgMInitCfg.WdgMAliveSupervisionPtr[Index].WdgMSupervisedEntityId] = WDGM_ALIVE_OK; /*make them OK at init*/
		}
	}
	wdgm.GlobalStatus = WDGM_ALIVE_OK;  /*Set the global status to ok */

	#endif

	wdgm_cfg = ConfigPtr;	  /*initialize the global variable to store the address of the PB configuration*/
	return;
}


/**************************************************************
 * 1. check the basic errors
 * 2. if the mode is same as existing, just return
 * 3. check if new mode's WDG config is OK . else raise error
 * 4. Trigger the old config WDGs
 * 5. stop the GPT if running & re init if needed by new config
 * 6. Change the WDG mode to new mode, any failure, report
 * 7. Update SE status
 * 8. update global status based on individual SE status
 * 9. update the mode in RAM container
 **************************************************************/

Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode )
{
	uint8 Index;
	WdgMModeType WdgMNewMode;
	WdgMModeType WdgMOldMode;

	#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)
	WdgMAliveSupervisionType Temp_AliveSupervision_struct;
	WdgM_SupervisedEntityIdType SEId;
	WdgM_AliveSupervisionStatusType GlobalStatus_Temp;
	#endif

	/*Check if Watchdog Manager is initialized */
	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_SETMODE_API_ID, WDGM_E_NO_INIT);
	/*Check if given mode is OK */
	WDGM_ASSERT_RETERR(!(Mode > WDGM_NUMBER_OF_MODES), WDGM_SETMODE_API_ID, WDGM_E_PARAM_MODE);

	#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION==STD_ON)
	if(wdgm.GlobalStatus!=WDGM_ALIVE_OK)
	{
		Dem_ReportErrorStatus(WDGM_E_SET_MODE, DEM_EVENT_STATUS_FAILED);
		return((uint8)E_NOT_OK);
	}
	#endif

	if(Mode == wdgm.CurrentMode)	  /*if the requested mode is the same as the old mode */
	{
		return ((uint8)E_OK);
	}

	WdgMNewMode=wdgm_cfg->WdgMModePtr[Mode];
	WdgMOldMode=wdgm_cfg->WdgMModePtr[wdgm.CurrentMode];

	#if defined(WDGM_OFF_MODE_ENABLED) && (WDGM_OFF_MODE_ENABLED == STD_OFF)
	/*Check if the new mode disables the watchdog driver(s) */
	for(Index = 0x0U; Index < WdgMNewMode.WdgMNumberOfWatchdogs; Index++)
	{
		if (WdgMNewMode.WdgMTriggerPtr[Index].WdgMWatchdogMode==WDGIF_OFF_MODE)
		{
			/*Disabling not allowed, raise DET,DEM error*/
			WDGM_ASSERT_REPORTONLY(!(1), WDGM_SETMODE_API_ID, WDGM_E_DISABLE_NOT_ALLOWED);
			Dem_ReportErrorStatus(WDGM_E_SET_MODE, DEM_EVENT_STATUS_FAILED);
			return((uint8)E_NOT_OK);
		}
	}
	#endif /*WDGM_OFF_MODE_ENABLED*/

	/*Trigger old watchdogs*/
	for(Index=0x0U;Index< WdgMOldMode.WdgMNumberOfWatchdogs; Index++)
	{
		WdgIf_Trigger(WdgMOldMode.WdgMTriggerPtr[Index].DeviceIndex);
	}

	/*If the old mode is gpt activated, deactivate the GPT channel */
	if(WdgMOldMode.WdgMActivationType==WDGM_ACTIVATION_TYPE_GPT)
	{
		/*stop the gpt timer & disable the CBK notification*/
		Gpt_StopTimer(WdgMOldMode.WdgMActivationGptPtr->WdgMGptChannelRef );
		Gpt_DisableNotification(WdgMOldMode.WdgMActivationGptPtr->WdgMGptChannelRef);
	}

	/*if the new mode is gpt activated, activate the new gpt data */
	if(WdgMNewMode.WdgMActivationType==WDGM_ACTIVATION_TYPE_GPT) /*if new mode is GPT activated*/
	{
		/*enable notification & start the gpt timer*/
		Gpt_EnableNotification(WdgMNewMode.WdgMActivationGptPtr->WdgMGptChannelRef);
		Gpt_StartTimer( WdgMNewMode.WdgMActivationGptPtr->WdgMGptChannelRef, WdgMNewMode.WdgMActivationGptPtr->WdgMGptCycle);
	}

	/*Set the watchdog drivers to the new mode*/
	for(Index = 0x0U; Index < WdgMNewMode.WdgMNumberOfWatchdogs; Index++)	 /*for all watchdogs in the initial mode*/
	{
		if( WdgIf_SetMode(WdgMNewMode.WdgMTriggerPtr[Index].DeviceIndex, WdgMNewMode.WdgMTriggerPtr[Index].WdgMWatchdogMode) == E_NOT_OK)
		{
			/*if the watchdog mode change failed */
			#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION==STD_ON)   /*if alive supervision is enabled*/
			/* assume a global alive-supervision failure and set the global supervision status to WDGM_ALIVE_STOPPED*/
			wdgm.GlobalStatus = WDGM_ALIVE_STOPPED;
			#endif
			Dem_ReportErrorStatus(WDGM_E_SET_MODE, DEM_EVENT_STATUS_FAILED);
			return((uint8)E_NOT_OK);
		}
	}

	wdgm_Initialize_container(Mode, wdgm_cfg); /*reset all global counters*/
	/*change the Supervision states of the SE to the new mode*/
	#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION==STD_ON)

	GlobalStatus_Temp=WDGM_ALIVE_OK;
	/*set the status of the new SE according to the configuration and old SE*/
	for(Index=0x0U;Index<WdgMNewMode.WdgMNumberOfEntities;Index++)
	{
		Temp_AliveSupervision_struct = WdgMNewMode.WdgMAliveSupervisionPtr[Index];
		SEId=Temp_AliveSupervision_struct.WdgMSupervisedEntityId;
		if(Temp_AliveSupervision_struct.WdgMActivationActivatedStatus==WDGM_SUPERVISION_ENABLED)
		{
			if(wdgm.SEStatus[SEId]!=WDGM_ALIVE_DEACTIVATED)
			{
				/*recalculate the global supervision status after updating the Supervised Entities
				 * alive supervision status according to the new mode
				 */
				switch (wdgm.SEStatus[SEId])
				{
					case WDGM_ALIVE_FAILED:
						if (GlobalStatus_Temp == WDGM_ALIVE_OK)
						{
							GlobalStatus_Temp = WDGM_ALIVE_FAILED;
						}/*if FAILED/EXPIRED already don't do anything!!! */
						break;
					case WDGM_ALIVE_EXPIRED:
						/* it doesn't matter of it's previous condition, this is worst of all*/
						GlobalStatus_Temp = WDGM_ALIVE_EXPIRED;
						break;

					default:
						break;
				}
			}
			else
			{
				/* this SE was deactivated in before mode but, need to be enabled in this mode.*/
				wdgm.SEStatus[SEId]=WDGM_ALIVE_OK;
			}
		}
		else
		{
			wdgm.SEStatus[SEId]=WDGM_ALIVE_DEACTIVATED;
			/* new status is WDGM_ALIVE_DEACTIVATED */
		}
	} /* loop for all SEs*/

	wdgm.GlobalStatus=GlobalStatus_Temp;		 /* use the temp status*/

	#endif /*if alive supervision is enabled*/

	/*all changes have been made successful. set the global mode variable*/
	wdgm.CurrentMode=Mode;

	return E_OK;
}

/********************************************************************************
 * it calls the local function WdgM_TriggerAlgorithm for this.
 * 1. Do the error check
 * 2. if activation source is SCH manage, call local trigger func.
 *******************************************************************************/

void WdgM_MainFunction_Trigger(void)
{
	/* Check for non Init. in case of error return E_NOT_OK */
	WDGM_ASSERT_RETVOID(!(wdgm_cfg == NULL_PTR), WDGM_MAINFUNCTRIGGER_API_ID, WDGM_E_NO_INIT);
	/*if the current activation source for WdgM is SchM*/
	if(wdgm_cfg->WdgMModePtr[wdgm.CurrentMode].WdgMActivationType==WDGM_ACTIVATION_TYPE_SCHEDULER)
	{
		WdgM_TriggerAlgorithm();
	}/* else activation source is GPT , do nothing WdgM_Cbk_GptNotification will take care */
}

/********************************************************************************
 * This is mainly a callback in GPT mode.
 * 1. Do the error check
 * 2. if source is GPT, do alive supervision and WDG Trigger.
 * 3.  	else only WDG_Trigger service.
 ********************************************************************************/

void WdgM_Cbk_GptNotification(void)
{
	/* Check for non Init. in case of error return E_NOT_OK */
	WDGM_ASSERT_RETVOID(!(wdgm_cfg == NULL_PTR), WDGM_CBKGPTNOTIFICATION_API_ID, WDGM_E_NO_INIT);
	/*is current activation source is GPT ? */
	if(wdgm_cfg->WdgMModePtr[wdgm.CurrentMode].WdgMActivationType==WDGM_ACTIVATION_TYPE_GPT)
	{
	/*if alive supervision is enabled perform WDGM functionality*/
	#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)
		WDGM_AliveSupervisionAlgorithm(); /* perform the supervision activity if it is switched on */
	#endif
		WdgM_TriggerAlgorithm(); /* service the WDG HW service now*/
	}/* else activation source is SchM, do nothing */

}

/********************************************************************************
 * 1. Do the error check
 * 2. Return the mode from ram container
 *******************************************************************************/

Std_ReturnType WdgM_GetMode(WdgM_ModeType* Mode)
{
	/* Check for non Init. in case of error return E_NOT_OK */
	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_GETMODE_API_ID, WDGM_E_NO_INIT);
	/* passed pointer is valid? check*/
	WDGM_ASSERT_RETERR(!(Mode == NULL_PTR), WDGM_GETMODE_API_ID, WDGM_E_NULL_POINTER);
	*Mode=wdgm.CurrentMode;	 /*return the current mode*/
	return E_OK;
}


#define WDGM_STOP_SEC_CODE
#include "MemMap.h"

