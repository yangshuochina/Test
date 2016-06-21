/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: Autosar OS 2.1 / Autosar OS 3.0
|    File Name: atosappl.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.17
|
|  Description: API-functions for controlling applications
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2006-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/


#if defined USE_QUOTE_INCLUDES
 #include "osek.h"
 #include "osekext.h"
#else
 #include <osek.h>
 #include <osekext.h>
#endif

/* Vector release management */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif
/* KB begin vrmReleaseNumber (overwritten) */
/* Source release number */
#define osdVrmMajRelNum 5
#define osdVrmMinRelNum 5
/* KB end vrmReleaseNumber */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif

/* CovComment 0:  file: atosappl.c */

/* KB begin osekStartOfCModule (overwritten) */
/*
 * Some conditions in the source-code might be always true, because 
 * they depend on the value of generated macros. 
 * Switch of the warning about conditions, that are always true
 */
#pragma warning 549
/*
 * Switch of the warning about possible endless loops
 */
#pragma warning 557
/*
 * Switch of incorrect warning about possible uninitialized variable
 */
#pragma warning 507
/* KB end osekStartOfCModule */



#if ((osdSC== SC3) || (osdSC== SC4))

#ifndef osdGetApplicationIDNotUsed
/*-+--------------------------------------------------------------------------
 / GetApplicationID
 / ================
 /
 / Syntax: ApplicationType GetApplicationID (void)
 / Parameter (In): none
 / Parameter (Out): none
 / Return value: Identifier of running OS-Application or INVALID_OSAPPLICATION
 / Description: 
 / OS261: If an OS-Application is running, GetApplicationID shall return its identifier.
 / OS262: If no OS-Application is running, GetApplicationID shall return
 /        INVALID_OSAPPLICATION.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */

osqFunc1 ApplicationType osqFunc2 osGetApplicationID (void)
{
   /* CovComment 4:  Function: osGetApplicationID */
   OS_AI_ENTRY()
   COVER(0x088F)

   OS_AI_EXIT()
   return osActiveApplicationID;
} /* END OF osGetApplicationID */

#endif


#ifndef osdCheckTaskMemoryAccessNotUsed
/*-+--------------------------------------------------------------------------
 / osCheckTaskMemoryAccess
 / =======================
 /
 / Syntax: AccessType CheckTaskMemoryAccess (TaskType TaskID,
 /                       MemoryStartAddressType Address, MemorySizeType Size)
 / Parameter (In):    TaskID: Task reference
 /                    Address: Start of memory area
 /                    Size: Size of memory area
 / Parameter (Out): none
 / Return value: Value which contains the access rights to the memory area.
 / Description: 
 / OS269: If the Task reference <TaskID> is valid, CheckTaskMemoryAccess shall
 /        return the access rights of the task on the specified memory area.
 / OS314: If an access right is not valid for the whole specified memory area
 /        CheckTaskMemoryAccess shall yield no access right.
 / OS270: If the Task reference <TaskID> is not valid, CheckTaskMemoryAccess
 /        shall yield no access rights.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */

osqFunc1 AccessType osqFunc2 osCheckTaskMemoryAccess (TaskType TaskID,
                        MemoryStartAddressType Address, MemorySizeType Size)
{
  /* CovComment 4:  Function: osCheckTaskMemoryAccess  */
#if(osdNumberOfAllTasks > 0)
  osuint16 i;
  AccessType retval;

  OS_TM_ENTRY()

   if ((TaskID >= osrNumberOfAllTasks) || (Size == 0) || ((osdValidCtx_CheckTaskMemoryAccess & osCurrentContext) == 0))
   {
      COVER(0x0890)
      retval = oskAccess_None;
   }
   else
   { 
      if (oskTcbIsTrusted[TaskID] != 0)
      {
         COVER(0x0891)
         retval = oskAccess_Read | oskAccess_Write;
      }
      else
      {
         COVER(0x0892)
         retval = 0;
      }
      for (i=0; i<osdNumberOfProtectionAreas; i++)
      {
         if (((osuint32)Address >= (osuint32)(oskTaskMemoryAccessLow[TaskID][i])) 
            && (((((osuint32)Address)+Size)-1) <= (osuint32)(oskTaskMemoryAccessHigh[TaskID][i])))
            
         {
            COVER(0x0893)
            retval |= oskMemAreaAccessRights[i];
         }
         ELSEWITHCOVER(0x0894)
      }
   }
   OS_TM_EXIT()

   return retval;
#else /*(osdNumberOfAllTasks > 0)*/

   OS_TM_ENTRY()
   COVER(0x0895)
   osdDummyRead(TaskID)
   osdDummyRead(Address)
   osdDummyRead(Size)
   OS_TM_EXIT()
   return oskAccess_None;   /* no ISR -> no access */

#endif /*(osdNumberOfAllTasks > 0) else */

}  /* END OF CheckTaskMemoryAccess */
#endif


#ifndef osdCheckISRMemoryAccessNotUsed
/*-+--------------------------------------------------------------------------
 / osCheckISRMemoryAccess
 / =======================
 /
 / Syntax: AccessType CheckISRMemoryAccess (ISRType ISRID,
 /                       MemoryStartAddressType Address, MemorySizeType Size)
 / Parameter (In):    ISRID: ISR reference
 /                    Address: Start of memory area
 /                    Size: Size of memory area
 / Parameter (Out): none
 / Return value: Value which contains the access rights to the memory area.
 / Description: 
 / OS267: If the ISR reference <ISRID> is valid, CheckISRMemoryAccess shall return
 /        the access rights of the ISR on the specified memory area.
 / OS313: If an access right is not valid for the whole specified memory area
 /        CheckISRMemoryAccess shall yield no access right.
 / OS268: If the ISR reference <ISRID> is not valid, CheckISRMemoryAccess shall
 /        yield no access rights.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */

osqFunc1 AccessType osqFunc2 osCheckISRMemoryAccess (ISRType ISRID,
                        MemoryStartAddressType Address, MemorySizeType Size)
{
  /* CovComment 4:  Function: osCheckISRMemoryAccess */
#if (osdNumberOfCat2ISRs > 0)
  osuint16 i;
  AccessType retval;

  OS_IM_ENTRY()

   if ((ISRID >= osdNumberOfCat2ISRs) || (Size == 0) || ((osdValidCtx_CheckISRMemoryAccess & osCurrentContext) == 0))
   {
      COVER(0x0896)
      retval = oskAccess_None;
   }
   else
   { 
      if (oskIcbIsTrusted[ISRID] != 0)
      {
         COVER(0x0897)
         retval = oskAccess_Read | oskAccess_Write;
      }
      else
      {
         COVER(0x0898)
         retval = 0;
      }
      for (i=0; i<osdNumberOfProtectionAreas; i++)
      {
         if (i != osdStackAreaID)
         {
            if (((osuint32)Address >= (osuint32)(oskISRMemoryAccessLow[ISRID][i])) 
               && (((((osuint32)Address)+Size)-1) <= (osuint32)(oskISRMemoryAccessHigh[ISRID][i])))
            {
               COVER(0x0899)
               retval |= oskMemAreaAccessRights[i];
            }
            ELSEWITHCOVER(0x089A)
         }
         else
         {  /* special check for ISR stack area */
            COVER(0x089B)
            /* KB begin osekHWosCheckISRStackAccess (overwritten) */
            if ((osIntNestingDepth > 0) && (osIsrStoredInfo[osIntNestingDepth-1].currentISRID == ISRID) && 
               ((osuint32)Address >= (osuint32)(osIsrStoredInfo[osIntNestingDepth-1].stackstart-oskIcbStackSize[ISRID])) 
               && (((osuint32)Address+Size) <= (osuint32)(osIsrStoredInfo[osIntNestingDepth-1].stackstart)))
            {
               retval |= oskMemAreaAccessRights[osdStackAreaID];
            }
            /* KB end osekHWosCheckISRStackAccess */
         }
      }
   }
   OS_IM_EXIT()

   return retval;
#else
   OS_IM_ENTRY()
   COVER(0x089C)
   osdDummyRead(ISRID)
   osdDummyRead(Address)
   osdDummyRead(Size)
   OS_IM_EXIT()
   return oskAccess_None;   /* no ISR -> no access */
#endif
}  /* END OF CheckISRMemoryAccess */


#endif



#ifndef osdCheckObjectOwnershipNotUsed
/*-+--------------------------------------------------------------------------
 / CheckObjectOwnership
 / ====================
 /
 / Syntax: ApplicationType CheckObjectOwnership (ObjectTypeType ObjectType,
 /                                               ObjectIDType ObjectID)
 / Parameter (In): ObjectType: Type of the following parameter
 /                 ObjectID: The object to be examined
 / Parameter (Out): none
 / Return value: The service returns the OS-Application to which the object ObjectType 
 /               belongs. If the object does not exists the service returns 
 /               INVALID_OSAPPLICATION.
 / Description: 
 / OS273: If the specified object ObjectType exists, CheckObjectOwnership shall
 /        return the identifier of the OS-Application to which the object belongs.
 / OS274: If the specified object ObjectType does not exist, CheckObjectOwnership
 /        shall return INVALID_OSAPPLICATION.
 / OS319: If the object to be examined is the RES_SCHEDULER
 /        CheckObjectOwnership shall always return INVALID_OSAPPLICATION.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */

osqFunc1 ApplicationType osqFunc2 osCheckObjectOwnership(ObjectTypeType ObjectType,
                                                         ObjectIDType ObjectID)
{
   /* CovComment 4:  Function: osCheckObjectOwnership */
   ApplicationType retval;

   OS_OO_ENTRY()   

   if ((osdValidCtx_CheckObjectOwnership & osCurrentContext) == 0)
   {
      COVER(0x089D)
      OS_OO_EXIT()
      return INVALID_OSAPPLICATION;
   }
   ELSEWITHCOVER(0x089E)
   
   switch (ObjectType)
   {
      case OBJECT_TASK:
#if (osdNumberOfAllTasks > 0)
         if (ObjectID < osdNumberOfAllTasks)
         {
            COVER(0x089F)
            retval = oskTaskToAppl[ObjectID];
         }
         else
         {
            COVER(0x08A0)
#else
            COVER(0x08A1)
#endif         
            retval = INVALID_OSAPPLICATION;
#if (osdNumberOfAllTasks > 0)
         } 
#endif         
         break;
      case OBJECT_ISR:
#if (osdNumberOfCat2ISRs > 0)
         if (ObjectID < osdNumberOfCat2ISRs)
         {
            COVER(0x08A2)
            retval = oskISRToAppl[ObjectID];
         }
         else
         {
            COVER(0x08A3)
#else
            COVER(0x08A4)
#endif         
            retval = INVALID_OSAPPLICATION;
#if (osdNumberOfCat2ISRs > 0)
         }
#endif         
         break;
      case OBJECT_ALARM:
#if (osdNumberOfAlarms > 0)
         if (ObjectID < osdNumberOfAlarms)
         {
            COVER(0x08A5)
            retval = oskAlarmToAppl[ObjectID];
         }
         else
         {
            COVER(0x08A6)
#else
            COVER(0x08A7)
#endif         
            retval = INVALID_OSAPPLICATION;
#if (osdNumberOfAlarms > 0)
         }
#endif         
         break;
      case OBJECT_RESOURCE:
#if (osdNumberOfAllResources > 0)
         if (ObjectID < osdNumberOfAllResources)
         {
            COVER(0x08A8)
            retval = oskResourceToAppl[ObjectID];
         }
         else
         {
            COVER(0x08A9)
#else
            COVER(0x08AA)
#endif         
            retval = INVALID_OSAPPLICATION;
#if (osdNumberOfAllResources > 0)
         }
#endif         
         break;
      case OBJECT_COUNTER:
#if (osdNumberOfCounters > 0)
         if (ObjectID < osdNumberOfCounters)
         {
            COVER(0x08AB)
            retval = oskCounterToAppl[ObjectID];
         }
         else
         {
            COVER(0x08AC)
#else
            COVER(0x08AD)
#endif         
            retval = INVALID_OSAPPLICATION;
#if (osdNumberOfCounters > 0)
         }
#endif         
         break;
      case OBJECT_SCHEDULETABLE:
#if (osdNumberOfScheduleTables > 0)
         if (ObjectID < osdNumberOfScheduleTables)
         {
            COVER(0x08AE)
            retval = oskScheduleTableToAppl[ObjectID];
         }
         else
         {
            COVER(0x08AF)
#else
            COVER(0x08B0)
#endif         
            retval = INVALID_OSAPPLICATION;
#if (osdNumberOfScheduleTables > 0)
         }
#endif         
         break;
      case OBJECT_MESSAGE:
         {
#if (osdNumberOfRxMessages > 0)
            MessageIdentifier RxId = ObjectID & (~osdMsgIdIsTransmit);

            /* Is the ObjectID in the correct range? */
            if(RxId < osdNumberOfRxMessages)
            {
               /* Yes: Is this a transmit message? */
               if(RxId != ObjectID)
               {
                  COVER(0x08B1)
                  /* Yes: Take the result out of the array for tx-messages */
                  retval = oskTxMessageToAppl[RxId];
               }
               else
               {
                  COVER(0x08B2)
                  /* No: Take the result out of the array for rx-messages */
                  retval = oskRxMessageToAppl[RxId];
               }
            }
            else
            {
               COVER(0x08B3)
#else
               COVER(0x08B4)
#endif
               retval = INVALID_OSAPPLICATION;
#if (osdNumberOfRxMessages > 0)
            }
#endif
         }
         break;
      default:
         COVER(0x08B5)
         retval = INVALID_OSAPPLICATION;
         break;
   }

   COVER(0x08B6)
   OS_OO_EXIT()   

   return retval;
} /* END OF osCheckObjectOwnership */

#endif


#ifndef osdCheckObjectAccessNotUsed
/*-+--------------------------------------------------------------------------
 / CheckObjectAccess
 / =================
 /
 / Syntax: ObjectAccessType CheckObjectAccess (ApplicationType   ApplID,
 /                                             ObjectTypeType    ObjectType,
 /                                             ObjectIDType      ObjectId)
 / Parameter (In): ApplID      OS-Application identifier
 /                 ObjectType  Type of the following parameter
 /                 ObjectId    The object to be examined
 / Parameter (Out): none
 / Return value: ACCESS if the ApplID has access to the object, NO_ACCESS otherwise 
 / Description: 
 / OS271: If the OS-Application <ApplID> has access to the queried object, 
 /        CheckObjectAccess shall return ACCESS [SR:124].
 / OS272: If the OS-Application <ApplID> has no access to the queried object, 
 /        CheckObjectAccess shall return NO_ACCESS [SR:125].
 / OS318: If the object to be examined is the RES_SCHEDULER CheckObjectAccess 
 /        shall always return ACCESS [SR:126].
 /--------------------------------------------------------------------------*/
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */

osqFunc1 ObjectAccessType osqFunc2 osCheckObjectAccess(ApplicationType   ApplID,
                                                       ObjectTypeType    ObjectType,
                                                       ObjectIDType      ObjectId)
{
   /* CovComment 4:  Function: osCheckObjectAccess */
   OS_OA_ENTRY()

   if ((osdValidCtx_CheckObjectAccess & osCurrentContext) == 0)
   {
      COVER(0x08B7)
      OS_OA_EXIT()
      return NO_ACCESS;
   }
   ELSEWITHCOVER(0x08B8)
   
   /* Is the application identifier (ApplID) valid? */
   if(ApplID < osdNumberOfApplications)
   {
      COVER(0x08B9)
      /* Yes: Is the object type any other than OBJECT_MESSAGE? */
      if (ObjectType < OBJECT_MESSAGE)
      {
         /* Yes: Is the ObjectId valid and access granted for the specified application? */
         if((ObjectId < oskObjectCount[ObjectType])
            && ((oskApplObjectAccess[ObjectType][ObjectId] & (1UL << ApplID)) != 0))
         {
            COVER(0x08BA)
            OS_OA_EXIT() 
            return ACCESS;
         }
         ELSEWITHCOVER(0x08BB)
      }
#if (osdNumberOfRxMessages > 0)
      /* No: Is this object a message? */
      else if(ObjectType == OBJECT_MESSAGE)
      {
         /* Yes: Remove the transmit bit from the identifer */
         MessageIdentifier RxId = ObjectId & (~osdMsgIdIsTransmit);

         /* Is the ObjectID in the correct range? */
         if(RxId < osdNumberOfRxMessages)
         {
            /* Yes: Is this a transmit message? */
            if(RxId != ObjectId)
            {
               /* Yes: Take the result out of the array for tx-messages */
               if((oskApplObjectAccess[ObjectType][RxId] & (1UL << ApplID)) != 0)
               {
                  COVER(0x08BC)
                  OS_OA_EXIT() 
                  return ACCESS;
               }
               ELSEWITHCOVER(0x08BD)
            }
            else
            {
               /* No: Take the result out of the array for rx-messages */
               if((oskApplObjectAccess[ObjectType+1][RxId] & (1UL << ApplID)) != 0)
               {
                  COVER(0x08BE)
                  OS_OA_EXIT() 
                  return ACCESS;
               }
               ELSEWITHCOVER(0x08BF)
            }
         } /* if(RxId < osdNumberOfRxMessages) */
         ELSEWITHCOVER(0x08C0)
      } /* else if ... */
#else
      ELSEWITHCOVER(0x08C1)
#endif /* (osdNumberOfRxMessages > 0) */

   } /* if(ApplID < osdNumberOfApplications) */

   COVER(0x08C2)
   OS_OA_EXIT() 
   return NO_ACCESS;
} /* END OF CheckObjectAccess */

#endif




#ifndef osdCallTrustedFunctionNotUsed
/*-+--------------------------------------------------------------------------
 / CallTrustedFunction
 / ====================
 /
 / Syntax: StatusType CallTrustedFunction (TrustedFunctionIndexType FunctionIndex,
 /                                         TrustedFunctionParameterRefType FunctionParams)
 / Parameter (In): FunctionIndex: Index of the function to be called. 
 /                 FunctionParams or NULL:
 /                 Pointer to the parameters for the function – specified by
 /                 the FunctionIndex - to be called. If no parameters are
 /                 provided, a NULL pointer has to be passed.
 / Parameter (Out): none
 / Return value: E_OK: No Error 
 /               E_OS_SERVICEID: No function defined for this index.
 / Description: 
 / OS265: If <FunctionIndex> is a defined function index, CallTrustedFunction shall
 /        switch the processor into privileged mode AND shall call the function
 /        <FunctionIndex> out of a list of implementation specific trusted functions AND
 /        shall return E_OK after completion.
 / OS312: The called trusted function must conform to the following C prototype:
 /        void TRUSTED_<name_of_the_trusted_service>(
 /        TrustedFunctionIndexType,TrustedFunctionParameterRefType);
 /        (The argument is the same as the argument of CallTrustedFunction).
 / OS266: When the function <FunctionIndex> is called, it shall get the same
 /        permissions (access rights) than the associated trusted OS-Application.
 / OS292: If the function index <FunctionIndex> is undefined, CallTrustedFunction
 /        shall return E_OS_SERVICEID.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osCallTrustedFunction(TrustedFunctionIndexType FunctionIndex,
                                                   TrustedFunctionParameterRefType FunctionParams)
{
   /* CovComment 4:  Function: osCallTrustedFunction */
   OS_CT_ENTRY()

   /* check call context */
   if ((osdValidCtx_CallTrustedFunction & osCurrentContext) == 0)
   {
      COVER(0x08C3)
      #if osdErrorHook 
      COVER(0x08C4)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCallTrustedFunction_FunctionIndex(FunctionIndex)
      osSaveCallTrustedFunction_FunctionParams(FunctionParams)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrCTCallContext);
      #if osdErrorHook 
      COVER(0x08C5)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x08C6)
#if STATUS_LEVEL == EXTENDED_STATUS
   if (osIntAPIStatus != 0)
   {
      COVER(0x08C7)
      #if osdErrorHook 
      COVER(0x08C8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCallTrustedFunction_FunctionIndex(FunctionIndex)
      osSaveCallTrustedFunction_FunctionParams(FunctionParams)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrCTIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x08C9)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      OS_CT_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x08CA)
#endif

#if (osdNumberOfTrustedFunctions > 0)
   if (FunctionIndex >= osdNumberOfTrustedFunctions)
   {
      COVER(0x08CB)
#else
      COVER(0x08CC)
#endif   
      #if osdErrorHook 
      COVER(0x08CD)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCallTrustedFunction_FunctionIndex(FunctionIndex)
      osSaveCallTrustedFunction_FunctionParams(FunctionParams)
      #endif
      osAPIError(E_OS_SERVICEID, osdErrCTWrongFctIdx);
      #if osdErrorHook 
      COVER(0x08CE)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CT_EXIT()
      return E_OS_SERVICEID;
#if (osdNumberOfTrustedFunctions > 0)
   }
   ELSEWITHCOVER(0x08CF)
   osInSystemFct++;
   (*oskTrustedFctTab[FunctionIndex])(FunctionIndex, FunctionParams); 
   osInSystemFct--;

   OS_CT_EXIT()

   return E_OK;
#endif   
} /* END OF osCallTrustedFunction */

#endif

/*-+--------------------------------------------------------------------------
 / osSysKillApplikation
 / ====================
 /
 / Syntax: void osSysKillApplikation (ApplicationType applToKill,
 /                                    RestartType RestartOption)
 / Parameter (In): applToKill: ID of the application to be killed. 
 /                 RestartOption: RESTART: activate restart task
 /                                NO_RESTART: don't activate restart task
 / Return value: none 
 / Description: 
 / This function terminates all running, ready and waiting Tasks/OsIsrs of the 
 / application. All interrupt sources assigned to the application will are disabled. 
 / All active alarms of the application are cancelled and all schedule tables of the 
 / application are stopped [SR:080], [SR:146].
 / If the ISRs of the application have locked resources, these resources are 
 / released [SR:145]. Resources locked by the tasks will be released during 
 / killing of the tasks by osSysKillTask.
 / If RestartOption=RESTART, the restart task of the application will be activated [SR:148].
 / This function can be called either from API function TerminateTask or from 
 / osHandleProtectionErrors (if the required protection violation reaction is 
 / to terminate an application.   
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/

#if (osdProtReactKillApplAllowed || osdProtReactKillApplRestAllowed || !defined osdTerminateApplicationNotUsed)
/* MISRA RULE 14.1 VIOLATION: This function might be unused in some configurations.
 * To reduce code complexity this function is not removed for complex configurations.
 */
osqFunc1 void osqFunc2 osSysKillApplikation(ApplicationType applToKill, RestartType RestartOption)   /* PRQA S 1503 */
{
#if (osdNumberOfAlarms > 0)
   osAlarmIndexType ai;
#endif   
#if (osdNumberOfScheduleTables > 0)
   ScheduleTableType si;
#endif   
#if (osdNumberOfAllTasks > 0)
   osTaskIndexType ti;
#endif   
   /* KB begin osekHWosKillApplISRsVars (overwritten) */
   #if (osdNumberOfCat2ISRs > 0)
   osuint16 i;
   #endif
   /* KB end osekHWosKillApplISRsVars */
   
#if (osdNumberOfAlarms > 0)
   COVER(0x08D0)
   /* cancel all alarms belonging to the application */
   for (ai=0; ai<osdNumberOfAlarms; ai++)
   {
      if (oskAlarmToAppl[ai] == applToKill)
      {
         COVER(0x08D1)
         osSysCancelAlarm((osAlarmIndexType)ai);
      }
      ELSEWITHCOVER(0x08D2)
   }
#endif

#if (osdNumberOfScheduleTables > 0)
   COVER(0x08D3)
   /* cancel all scheduletables belonging to the application */
   for (si=0; si<osdNumberOfScheduleTables; si++)
   {
      if (oskScheduleTableToAppl[si] == applToKill)
      {
         COVER(0x08D4)
         osSysStopScheduleTable((ScheduleTableType)si);
      }
      ELSEWITHCOVER(0x08D5)
   }
#endif

#if (osdNumberOfAllTasks > 0)
   COVER(0x08D6)
#if (osdFullPreempt || osdMixedPreempt)
   COVER(0x08D7)
   osLockDispatcher++;
#endif
   for (ti=0; ti<osdNumberOfAllTasks; ti++)
   {
      if ((osTcbTaskState[ti] != SUSPENDED) && (oskTaskToAppl[ti] == applToKill))
      {
         COVER(0x08D8)
         osSysKillTask((TaskType)ti, osdFALSE);
      }
      ELSEWITHCOVER(0x08D9)
   }
   if ((RestartOption == RESTART) && (oskRestartTask[applToKill] != osdNoActiveTask))
   {
      COVER(0x08DA)
      (void) osSysActivateTask(oskRestartTask[applToKill]);
   }
   ELSEWITHCOVER(0x08DB)
#if (osdFullPreempt || osdMixedPreempt)
   COVER(0x08DC)
   osLockDispatcher--;
#endif
#endif

#if osdErrorHook
   osErrorFlag = osdFALSE;
#endif
#if osdComErrorHook
   osCOMErrorFlag = osdFALSE;
#endif
   
#if (osdNumberOfCat2ISRs > 0)
   for (i=0; i<osdNumberOfCat2ISRs; i++)
   {
      if (oskISRToAppl[i] == applToKill)
      {
         COVER(0x08DD)
         osSysDisableInterruptSource(i);
      }
      ELSEWITHCOVER(0x08DE)
   }

   if (osIntNestingDepth > 0)
   {
      for (i=0; i<osIntNestingDepth; i++)
      {
         if (oskISRToAppl[osIsrStoredInfo[i].currentISRID] == applToKill)
         {
            osIsrStoredInfo[i].isKilled = osdTRUE;
            COVER(0x08DF)
            
#if (osdTimingProtectionUsed != 0)
            COVER(0x08E0)
            /* Release the list of measured resource locks */
            osTPCleanListBlock(osIsrStoredInfo[i].currentISRID + osdNumberOfAllTasks);
            /* Refresh remaining execution time */
            osTPReloadExeBudget(osIsrStoredInfo[i].currentISRID + osdNumberOfAllTasks);
#endif
#if (osdNumberOfAllResources > 0)
            COVER(0x08E1)
            osSysKillResourceList(osIcbLockedResource[osIsrStoredInfo[i].currentISRID]);
            osIcbLockedResource[osIsrStoredInfo[i].currentISRID] = osdNoResource;
#endif   
         }
         ELSEWITHCOVER(0x08E2)
      }
      
      osIntSaveDisableCounter = 0;
      /* KB begin osekHWInitGlobalDisableCounter (overwritten) */
      #if osdTimingProtectionUsed
      osIntSaveDisableHighLevelCounter = 0;
      #else
      osIntSaveDisableGlobalCounter = 0;
      #endif
      /* KB end osekHWInitGlobalDisableCounter */

      #if (osdNumberOfAllTasks > 0)
         osStartDispatcher = osSchedulePrio();
      #endif
   }
   ELSEWITHCOVER(0x08E3)
     
#endif

}
#endif /* (osdProtReactKillApplAllowed || osdProtReactKillApplRestAllowed || !defined osdTerminateApplicationNotUsed) */

#ifndef osdTerminateApplicationNotUsed
/*-+--------------------------------------------------------------------------
 / TerminateApplication
 / ====================
 /
 / Syntax: StatusType TerminateApplication(RestartType RestartOption)
 / Parameter (In): RestartOption  Either RESTART for doing a restart of the OS-Application
 /                                or NO_RESTART if OS-Application shall not be restarted.
 / Parameter (Out): none
 / Return value: E_OS_CALLEVEL Called in the wrong context.
 /               E_OS_ID RestartOption is neither RESTART nor NO_RESTART.
 / Description: 
 / OS287: If called from allowed context, TerminateApplication() shall terminate 
 /        the calling OS-Application (i.e. to kill all tasks and free all other 
 /        OS resources associated with the application).
 / OS288: If called from wrong context, TerminateApplication() shall return
 /        E_OS_CALLEVEL.
 / OS346: If RestartOption equals RESTART, TerminateApplication() shall
 /        activate the configured RESTARTTASK of the terminated OS-Application.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/

#if (osdNumberOfApplications > 0)

osqFunc1 StatusType osqFunc2 osTerminateApplication(RestartType RestartOption)
{
   /* CovComment 4:  Function: osTerminateApplication */
   ApplicationType applToKill;
   
   OS_TA_ENTRY()

   if ((osdCtxErrorHookFromStartupHook & osCurrentContext) != 0)
   {  /* TerminateApplication was called from ErrorHook inside StartupHook */
      COVER(0x08E4)
      osSysShutdownOS(E_OS_SYS_ABORT);
   }
   ELSEWITHCOVER(0x08E5)
      
   /* check call context */
   if ((osdValidCtx_TerminateApplication & osCurrentContext) == 0)
   {
      COVER(0x08E6)
      #if osdErrorHook 
      COVER(0x08E7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveTerminateApplication_RestartOption(RestartOption)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrTACallContext);
      #if osdErrorHook 
      COVER(0x08E8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TA_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x08E9)
#if STATUS_LEVEL == EXTENDED_STATUS
   if (osIntAPIStatus != 0)
   {
      COVER(0x08EA)
      #if osdErrorHook 
      COVER(0x08EB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveTerminateApplication_RestartOption(RestartOption)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrTAIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x08EC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GI_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x08ED)
#endif

   if ((RestartOption != NO_RESTART) && (RestartOption != RESTART))
   {
      COVER(0x08EE)
      #if osdErrorHook 
      COVER(0x08EF)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveTerminateApplication_RestartOption(RestartOption)
      #endif
      osAPIError(E_OS_VALUE, osdErrTAWrongRestartOption);
      #if osdErrorHook 
      COVER(0x08F0)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TA_EXIT()
      return E_OS_VALUE;
   }
   ELSEWITHCOVER(0x08F1)
   applToKill = osActiveApplicationID;

   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_TA_START_CRITICAL()

   /* KB begin osekHWosTPStopTimerExeInTermAppl (default) */
   #if (osdTimingProtectionUsed != 0)
      COVER(0x08F2)
      {
         osProcessType Proc;
         Proc = osTPExeSwitchTo(INVALID_PROCESS);
         if (Proc != INVALID_PROCESS)
         {
            COVER(0x08F3)
            osTPSuspendTimerBlock();
            osTPReloadExeBudget(Proc);
            osTPCleanListBlock(Proc);
         }
         ELSEWITHCOVER(0x08F4)
      }
   #endif
   /* KB end osekHWosTPStopTimerExeInTermAppl */

   osSysKillApplikation(applToKill, RestartOption);
   
   osIntSaveDisableCounter = 0;
   /* KB begin osekHWInitGlobalDisableCounter (overwritten) */
   #if osdTimingProtectionUsed
   osIntSaveDisableHighLevelCounter = 0;
   #else
   osIntSaveDisableGlobalCounter = 0;
   #endif
   /* KB end osekHWInitGlobalDisableCounter */

#if (osdNumberOfCat2ISRs > 0)
   if (osIntNestingDepth > 0)
   {
      COVER(0x08F5)
      /* KB begin osekHWLeaveKilledISRFromAPI (overwritten) */
      osLeaveISR();
      #if osdTimingProtectionUsed
         osTPExeSwitchBack(osIsrStoredInfo[osIntNestingDepth].ProcTP);
         osTPResumeTimerBlock(osIsrStoredInfo[osIntNestingDepth].ProcTP);
      #endif
      OS_TA_END_CRITICAL()
      OS_TA_EXIT()  
      /* Return to ISR-wrapper of the killed ISR 
       * (dispatcher becomes active if necessary immediately on the return from isr to task level) 
       */
      #if (osdStackModel == osdSingleStackOsek)
         pcxiraw=osd_mfcr(os_PCXI_OFFSET);   /* Get PCXI value */
         /* Is the current context a lower context? */
         if ((pcxiraw & 0x00400000UL) == 0)
         {
            /* Yes: restore the lower context */
            __asm( "rslcx" );/* restore lower context */
         }
         __asm( "ret" );/* return to the stack switching wrapper */
         
      #else
         __asm( "rslcx" );/* restore lower context */
         __asm( "rfe" )  ;/* return from exception */
      #endif
      /* KB end osekHWLeaveKilledISRFromAPI */
   }     
   ELSEWITHCOVER(0x08F6)
#endif

#if (osdNumberOfAllTasks > 0)
   (void)osSchedulePrio();  /* no result check required because running task is terminated */
#endif
   OS_TA_END_CRITICAL()

   /* enable task level interrupt */
   /* KB begin osekHWosEnableLevel (overwritten) */
   /* enable task level interrupt */
   osEnableLevel();   
   /* KB end osekHWosEnableLevel */

   /* start dispatcher */
   OS_TA_EXIT()  
   OS_START_DISPATCH()

   /* KB begin osekHWcallosDispatcher (default) */
      /* KB begin osekHWosDispatcher (overwritten) */
      osDispatch()
      /* KB end osekHWosDispatcher */
   /* KB end osekHWcallosDispatcher */

   /* KB begin osekHWosEndOfTerminateApplication (overwritten) */
   /* KB end osekHWosEndOfTerminateApplication */   
   return E_OK;
} /* END OF TerminateApplication */

#endif  /* (osdNumberOfApplications > 0) */
#endif  /* osdTerminateApplicationNotUsed */


#if (osdNumberOfApplications > 0)
/*-+--------------------------------------------------------------------------
 / osAddressParamValid
 / ====================
 /
 / Syntax: osbool osAddressParamValid(osuint32 refAddr, osuint32 dataSize)
 / Parameter (In): refAddr: Start address of address range
/                 dataSize: Size of address range in byte
 / Parameter (Out): none
 / Return value: TRUE if active task or ISR has write access on full address range.
 /               FALSE if active task or ISR has not write access on full address range.
 / Description: 
 / Checks whether the currently active task or ISR has write access on a address range
 / in memory.
 / Scalability Classes: SC3, SC4
 /--------------------------------------------------------------------------*/
/* MISRA RULE 14.1 VIOLATION: This function might be unused in some configurations.
 * To reduce code complexity this function is not removed for complex configurations.
 */
osbool osAddressParamValid(osuint32 refAddr, osuint32 dataSize)   /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osAddressParamValid */
   osuint8 i;
   
   if (osIntNestingDepth == 0)
   {
      /* called from task level */
      if (oskTcbIsTrusted[osActiveTaskIndex] != 0)
      {
         COVER(0x08F7)
         return osdTRUE;   /* task of trusted application has always access */
      }
      else
      {
         COVER(0x08F8)
         /* Search all protection areas */
         for (i = 0; i <= osdLastWritableProtectionArea; i++)
         { 
            /* Is the address parameter contained in this protection area? */               
            if ((refAddr >= (osuint32)(oskTaskMemoryAccessLow[osActiveTaskIndex][i]))
                && ((refAddr+dataSize) <= (osuint32)(oskTaskMemoryAccessHigh[osActiveTaskIndex][i])))
            {
               COVER(0x08F9)
               /* Yes: Stop searching, the address parameter is valid */
               return osdTRUE;
            }
            ELSEWITHCOVER(0x08FA)
         }/* for */
      }/* else */
   }
   else
   {
      /* called from interrupt level */
      if (oskIcbIsTrusted[osActiveISRID] != 0)
      {
         COVER(0x08FB)
         return osdTRUE;   /* ISR of trusted application has always access */
      }
      else
      {
         COVER(0x08FC)
         /* Search all protection areas */
         for (i = 0; i <= osdLastWritableProtectionArea; i++)
         {
           /* Is this a stack area? */
            if (i == osdStackAreaID)
            {
               COVER(0x08FD)
               /* Yes: Is the address parameter contained in the stack area? */
               /* KB begin osekHWosCheckAddrParamInIsrStack (overwritten) */
               if ((refAddr >= (osuint32)(osIsrStoredInfo[osIntNestingDepth-1].stackstart-oskIcbStackSize[osActiveISRID]))
                   && ((refAddr+dataSize) <= (osuint32)(osIsrStoredInfo[osIntNestingDepth-1].stackstart)))
               {
                  COVER(0x08FE)
                  /* Yes: Stop searching, the address parameter is valid */
                  return osdTRUE;
               }
               /* KB end osekHWosCheckAddrParamInIsrStack */
            }
            else
            {
               COVER(0x08FF)
               /* No: Is the address parameter contained in this protection area? */               
               if ((refAddr >= (osuint32)(oskISRMemoryAccessLow[osActiveISRID][i]))
                   && ((refAddr+dataSize) <= (osuint32)(oskISRMemoryAccessHigh[osActiveISRID][i])))
               {
                  COVER(0x0900)
                  /* Yes: Stop searching, the address parameter is valid */
                  return osdTRUE;
               }
               ELSEWITHCOVER(0x0901)
            }/* else */
         }/* for */
      }/* else */
   }
   /* The address parameter is not contained in any of the considered protection areas */
   /* The parameter is invalid */
   if (osCurrentContext == osdCtxApplErrorHook) 
   {  /* for appl ErrorHook the address might be valid on system stack */
      COVER(0x0902)
      return osApplErrStackAddressParamValid(refAddr, dataSize);
   }
   ELSEWITHCOVER(0x0903)
   return osdFALSE;
}
#endif  /* (osdNumberOfApplications > 0) */



#endif /* ((osdSC== SC3) || (osdSC== SC4)) */

/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */


/* END OF MODULE atosappl.c */
