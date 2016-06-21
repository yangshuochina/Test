/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgM_Alive.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:18:30MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgM_Alive.c  $
 *  Revision 1.3 2011/02/02 09:18:30MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 14:41:10MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/
 
#include "WdgM.h"
#include "WdgM_Priv.h"
#include "WdgIf.h"

#include "SchM_WdgM.h"
#include "Mcu.h"
#include "Gpt.h"

#include "Det.h"
#include "Dem.h"

#define WDGM_START_SEC_CODE
#include "MemMap.h"

#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)

/********************************************************************************
 * This local function is the implementation of alive supervision Algo
 * It is called by MainFunction.
 * 1. No error check is needed as it is done by the calling function.
 * 2. If Globals status is stopped, don't do anything
 * 3. If Globals status is expired, based on tollerence reset, raise DEM based on configuration
 * 4. if the ref cycle over,
 * 5. 	check the min,max range
 * 6.	if within/beyond range update SE status accordingly
 * 7. update global status accordingly based on SE status

 ********************************************************************************/

void WDGM_AliveSupervisionAlgorithm (void)
{
	WdgM_SupervisedEntityIdType Index;
	WdgM_AliveSupervisionStatusType SE_Status_Temp;
	WdgM_AliveSupervisionStatusType GlobalStatus_Temp;
	uint16 SEId;
	WdgMModeType WdgMCurrentMode;
	WdgMAliveSupervisionType Temp_AliveSupervision_struct;

	if(wdgm.GlobalStatus != WDGM_ALIVE_STOPPED) /*If global status is OK, perform the algo*/
	{
		if (wdgm.GlobalStatus != WDGM_ALIVE_EXPIRED) /* is global status is expired ? */
		{
			WdgMCurrentMode=wdgm_cfg->WdgMModePtr[wdgm.CurrentMode]; /*Store the current mode*/
			GlobalStatus_Temp = WDGM_ALIVE_OK; /* keep the temp global status as OK */
			for(Index = 0x0U; Index <WdgMCurrentMode.WdgMNumberOfEntities  ; Index++)
			{
				Temp_AliveSupervision_struct=WdgMCurrentMode.WdgMAliveSupervisionPtr[Index];
				SEId=Temp_AliveSupervision_struct.WdgMSupervisedEntityId;
				SE_Status_Temp = wdgm.SEStatus[SEId];
				 /*if the current SE is disabled, skip it. */
				if (SE_Status_Temp != WDGM_ALIVE_DEACTIVATED)
				{
					wdgm.SERefCycles[SEId]--; /* update the #of cycles passed*/
					if(wdgm.SERefCycles[SEId]==0x00U) /* #of cycles over?  */
					{ /* is it beyond allowed limit?*/
						if(	(wdgm.SEIndications[SEId] < Temp_AliveSupervision_struct.WdgMMinMargin) ||
							(wdgm.SEIndications[SEId] > Temp_AliveSupervision_struct.WdgMMaxMargin)	)
						{
							/* Too many or too few AI received, Check the Failed Cycle Tolerance*/
							if(wdgm.SEExpCycles[SEId] ==0x00U) /* if no tolerance, set the EXPIRED status*/
							{
								SE_Status_Temp = WDGM_ALIVE_EXPIRED;
							}
							else
							{
								/*SE failed but recovery is still possible*/
								wdgm.SEExpCycles[SEId]--; /*Decrement Expired Cycle Tolerance counter*/
								SE_Status_Temp = WDGM_ALIVE_FAILED;
							}
						}
						else
						{
							/*No error detected, recover the error if needed*/
							if(SE_Status_Temp == WDGM_ALIVE_FAILED)
							{
								SE_Status_Temp = WDGM_ALIVE_OK;
								/*Reload  default Expired Cycle Tollerance counter */
								wdgm.SEExpCycles[SEId] = Temp_AliveSupervision_struct.WdgMFailedSupervisionRefCycleTol;
							}
						}
						/*Reset Alive Incication counter*/
						wdgm.SEIndications[SEId] = 0x0U;
						wdgm.SERefCycles[SEId] = Temp_AliveSupervision_struct.WdgMSupervisionReferenceCycle;
					}
					/* update TEMP GLOBAL status based on SE status!!!!*/
					switch (SE_Status_Temp)
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
					/* update SE's GLOBAL status now*/
					/* if the SE is not deactivated just before this, don't change it */
					if (wdgm.SEStatus[SEId] !=WDGM_ALIVE_DEACTIVATED)
					{
						wdgm.SEStatus[SEId] = SE_Status_Temp;
					}

				} /*end if (Status check)*/
			} /*end for loop on all SEs*/

			/* update WDGM's GLOBAL status now*/
			/* if the WdgM_SetMode is called just before this and Failed, it may be in STOPPED state */
			if (wdgm.GlobalStatus !=WDGM_ALIVE_STOPPED)
			{
				wdgm.GlobalStatus = GlobalStatus_Temp;
			}

		}/*If global status not expired*/

		/*Check Watchdog Manager is not in WDGM_ALIVE_EXPIRED state*/
		if(wdgm.GlobalStatus == WDGM_ALIVE_EXPIRED)
		{
			/*Check if the expired tolerance is finished*/
			if(wdgm.CurExpCycTolerance == 0x00U)
			{
				wdgm.GlobalStatus = WDGM_ALIVE_STOPPED; /* no more AS since ESCC>=ESCT */
				Dem_ReportErrorStatus(WDGM_E_ALIVE_SUPERVISION , DEM_EVENT_STATUS_FAILED);

				#if defined(WDGM_IMMEDIATE_RESET) && (WDGM_IMMEDIATE_RESET == STD_ON)
				Mcu_PerformReset();
				#endif
			}
			else
			{
				wdgm.CurExpCycTolerance--; /*wait till 0 */
			}
		}
	}/*If stopped no action is required*/
}

/********************************************************************************
 * This function Called by the SEs.
 * 1. Do the error check
 * 2. if SE is not de activated, increment the counter.
 * 3. else return error
 *******************************************************************************/

Std_ReturnType WdgM_UpdateAliveCounter(WdgM_SupervisedEntityIdType SEId	)
{
	Std_ReturnType ret_val = E_NOT_OK;
	/*Check if Watchdog Manager is initialized */
	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_UPDATEALIVECOUNTER_API_ID, WDGM_E_NO_INIT);
	/*is the SEid is within range?  */
	WDGM_ASSERT_RETERR(!(SEId >= WDGM_NUMBER_OF_SUPERVISED_ENTITIES), WDGM_UPDATEALIVECOUNTER_API_ID, WDGM_E_PARAM_SEID);

	/*If supervision entity is active and is not deactivated*/
		if(wdgm.SEStatus[SEId] != WDGM_ALIVE_DEACTIVATED)
		{
			SchM_Enter_WdgM(WDGM_INSTANCE_ID,0);
			wdgm.SEIndications[SEId]++; /*Increment alive counter */
			SchM_Exit_WdgM(WDGM_INSTANCE_ID,0);
			ret_val = E_OK;
		}
	return ret_val;
}

/********************************************************************************
 * 1. Do the error check.
 * 2. if de active, activate it.
 * 3. else return error
 *******************************************************************************/

Std_ReturnType WdgM_ActivateAliveSupervision(WdgM_SupervisedEntityIdType SEId )
{
	Std_ReturnType ret_val = E_OK;

	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_ACTALIVESUPERVISION_API_ID, WDGM_E_NO_INIT);
	WDGM_ASSERT_RETERR(!(SEId >= WDGM_NUMBER_OF_SUPERVISED_ENTITIES), WDGM_ACTALIVESUPERVISION_API_ID, WDGM_E_PARAM_SEID);

	/*Activate entity supervison */
	if(wdgm.SEStatus[SEId] == WDGM_ALIVE_DEACTIVATED)
	{
		wdgm.SEStatus[SEId] = WDGM_ALIVE_OK;
		ret_val = E_OK;
	}
	return ret_val;
}

/*********************************************************************************
 *  1. do the error check.
 *  2. if SE status is OK, & de activation allowed, de activate it.
 *  3. else raise error incl DET error.
 *********************************************************************************/

Std_ReturnType WdgM_DeactivateAliveSupervision(WdgM_SupervisedEntityIdType SEId	)
{
	Std_ReturnType ret_val = E_NOT_OK;

	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_DEACTALIVESUPVISON_API_ID, WDGM_E_NO_INIT);
	WDGM_ASSERT_RETERR(!(SEId >= WDGM_NUMBER_OF_SUPERVISED_ENTITIES), WDGM_DEACTALIVESUPVISON_API_ID, WDGM_E_PARAM_SEID);

	if(wdgm.SEStatus[SEId] == WDGM_ALIVE_OK)	  /*check if the SE is healthy? */
	{
		/*Clear activation flag if allowed*/
		if(wdgm_cfg->WdgMModePtr[wdgm.CurrentMode].WdgMAliveSupervisionPtr[SEId].WdgMDeactivationAccessEnabled ==WDGM_DEACTIVATION_ACCESS_ENABLED)
		{
			/*Deactivation allowed*/
			wdgm.SEStatus[SEId] = WDGM_ALIVE_DEACTIVATED;
			ret_val = E_OK;
		}
		else
		{
			/*De activation not allowed, return error */
			WDGM_ASSERT_RETERR(!(1), WDGM_DEACTALIVESUPVISON_API_ID, WDGM_E_DEACTIVATE_NOT_ALLOWED);
		}
	}
return ret_val;
}

/*********************************************************************************
 * 1. do the error check
 * 2. Call the Algo function to perform alive supervision.
 *********************************************************************************/

void WdgM_MainFunction_AliveSupervision(void)
{
	/* Check for non Init. in case of error return immediately*/
	WDGM_ASSERT_RETVOID(!(wdgm_cfg == NULL_PTR), WDGM_MAINFUNCALIVESUPER_API_ID, WDGM_E_NO_INIT);
	/*if the current activation source for WdgM is SchM*/
	if(wdgm_cfg->WdgMModePtr[wdgm.CurrentMode].WdgMActivationType==WDGM_ACTIVATION_TYPE_SCHEDULER)
	{
			WDGM_AliveSupervisionAlgorithm();   /*do the alive supervision */
	} /*activation source is GPT, do nothing, in this case */
}

/********************************************************************************
 * 1. Do the error check
 * 2. Return SE's Status.
 ********************************************************************************/

Std_ReturnType WdgM_GetAliveSupervisionStatus(WdgM_SupervisedEntityIdType SEId,	WdgM_AliveSupervisionStatusType* Status )
{
	/* Check for non Init. in case of error return E_NOT_OK */
	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_GETALIVESUPVISSTAT_API_ID, WDGM_E_NO_INIT);
	/* is SEid is within limit? check */
	WDGM_ASSERT_RETERR(!(SEId >= WDGM_NUMBER_OF_SUPERVISED_ENTITIES), WDGM_GETALIVESUPVISSTAT_API_ID, WDGM_E_PARAM_SEID);
	/* passed pointer is valid? check*/
	WDGM_ASSERT_RETERR(!(Status == NULL_PTR), WDGM_GETALIVESUPVISSTAT_API_ID, WDGM_E_NULL_POINTER);
	*Status=wdgm.SEStatus[SEId]; /* update the SEId's status*/
	return E_OK;
}

/********************************************************************************
 * 1. do the error check
 * 2. Return the global status :-)
 *******************************************************************************/

Std_ReturnType WdgM_GetGlobalStatus(WdgM_AliveSupervisionStatusType* Status )
{
	/* Check for non Init. in case of error return E_NOT_OK */
	WDGM_ASSERT_RETERR(!(wdgm_cfg == NULL_PTR), WDGM_GETGLOBALSTATUS_API_ID, WDGM_E_NO_INIT);
	/* passed pointer is valid? check*/
	WDGM_ASSERT_RETERR(!(Status == NULL_PTR), WDGM_GETGLOBALSTATUS_API_ID, WDGM_E_NULL_POINTER);
	*Status=wdgm.GlobalStatus; /* update the global status*/
	return E_OK;
}

#endif /* for #if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON) */


#define WDGM_STOP_SEC_CODE
#include "MemMap.h"

