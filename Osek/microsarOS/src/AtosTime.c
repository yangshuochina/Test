/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: Autosar OS 2.1 / Autosar OS 3.0
|    File Name: atostime.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.51
|
|  Description: functions for schedule tables and synchronisation
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* next define is set in the makefile
 * switches of the include of (generated) tcb.h, includes instead
 * the library config file
 */
#ifdef osdGenerateLib
#define osdIncludeLibConfig 1
#endif

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

/* CovComment 0:  file: atostime.c */
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

#define osdSTINIT_AUTO       1
#define osdSTINIT_SYNCSTART  2
#define osdSTINIT_HRST       4      

#if (osdNumberOfScheduleTables > 0)

#if (osdScheduleTableSyncUsed != 0)

/* Flags
   Note: may be OR'ed, all values must therefore be powers of 2
*/
typedef osuint8 osHRSTFlagsType;
#define osdHRST_NoSpecialInit ((osHRSTFlagsType)0)
#define osdHRST_NeedMod       ((osHRSTFlagsType)1)
#define osdHRST_ForceAsync    ((osHRSTFlagsType)2)

static osqRAM1 osqRAM2 TickType             osqRAM3 osSTTime        [osdNumberOfScheduleTables]; /* Scheduletable time of normal STs */
static osqRAM1 osqRAM2 osHRSTFlagsType      osqRAM3 osSTFlags       [osdNumberOfScheduleTables]; /* Scheduletable time may still need a init */
static osqRAM1 osqRAM2 osTickTimeDiffType   osqRAM3 osSTDiffTime    [osdNumberOfScheduleTables]; 
#endif










#ifndef osdStartScheduleTableRelNotUsed
/*-+--------------------------------------------------------------------------
 / StartScheduleTableRel
 / =====================
 /
 / Syntax: 
 /    StatusType StartScheduleTableRel(
 /                                      ScheduleTableType ScheduleTableID, 
 /                                      TickType Offset
 /                                    )
 / Parameter (In):
 /   ScheduleTableType ScheduleTableID
 /   TickType Offset
 / Parameter (Out):
 /   none
 / Description:
 /   If its input parameters are valid, StartScheduleTableRel() starts the
 /   processing of a schedule table <ScheduleTableID> at its first expiry point
 /   after offset <Offset> ticks have elapsed.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /       - Schedule table <ScheduleTableID> is in any other state than 
 /         SCHEDULETABLE_STOPPED, E_OS_STATE
 /    EXTENDED: 
 /       - <ScheduleTableID> not valid, or schedule table is implicitely 
           synchronized E_OS_ID
 /       - <Offset> too big or zero, E_OS_VALUE
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC1, SC2, SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:191] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 StatusType osqFunc2 osStartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
{
   /* CovComment 4:  Function: osStartScheduleTableRel */
   OS_SR_ENTRY()
   COVER(0x0679)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x067A)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_StartScheduleTableRel & osCurrentContext) == 0)
   {
      COVER(0x067B)
      #if osdErrorHook 
         COVER(0x067C)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSRCallContext);
      #if osdErrorHook 
         COVER(0x067D)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SR_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x067E)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleTableID >= osrNumberOfScheduleTables)
   {
      /* fullfills [SR:187] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x067F)
      #if osdErrorHook
         COVER(0x0680)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_ID, osdErrSRWrongID);
      #if osdErrorHook
         COVER(0x0681)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SR_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0682)

   if (oskSTSyncKind[ScheduleTableID] == osdSTSyncImplicit)
   {
      /* fullfills [SR:192]: return E_OS_ID if the schedule table is implicitely synchronized */
      COVER(0x0683)
      #if osdErrorHook 
         COVER(0x0684)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_ID, osdErrSRImpliciteSync);
      #if osdErrorHook 
         COVER(0x0685)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SR_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0686)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[ScheduleTableID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0687)
      #if osdErrorHook 
         COVER(0x0688)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSRNoAccess);
      #if osdErrorHook 
         COVER(0x0689)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SR_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x068A)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (Offset == 0)
   {
      /* fullfills [SR:188] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x068B)
      #if osdErrorHook
         COVER(0x068C)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_VALUE, osdErrSRZeroOffset);
      #if osdErrorHook
         COVER(0x068D)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SR_EXIT()
      return E_OS_VALUE;
   }
   ELSEWITHCOVER(0x068E)
   /* check against MAXALLOWEDVALUE */
      /* fullfills [SR:189] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      if (Offset > (TickType)(oskCounterMaxAllowedValue[oskCounterOfST[ScheduleTableID]]-oskScheduleTableInitialOffset[ScheduleTableID])) 
      {
         COVER(0x068F)
         #if osdErrorHook 
            COVER(0x0690)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
            osSaveStartScheduleTableRel_Offset(Offset)
         #endif
         osAPIError(E_OS_VALUE, osdErrSROffsetTooBig);
         #if osdErrorHook 
            COVER(0x0691)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SR_EXIT()
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0692)
   
   if (osIntAPIStatus != 0)
   {
      COVER(0x0693)
      #if osdErrorHook 
         COVER(0x0694)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSRIntAPIDisabled);
      #if osdErrorHook 
         COVER(0x0695)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SR_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x0696)

#endif /*STATUS_LEVEL*/
   COVER(0x0697)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SR_START_CRITICAL()

   if (osSTState[ScheduleTableID] != SCHEDULETABLE_STOPPED)
   {
      /* fullfills [SR:190] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x0698)
      #if osdErrorHook
         COVER(0x0699)
         osSaveStartScheduleTableRel_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableRel_Offset(Offset)
      #endif
      osAPIError(E_OS_STATE, osdErrSRAlreadyRunningOrNext);
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SR_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x069A)
   

   
   COVER(0x069B)

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */


   osSTState[ScheduleTableID]= SCHEDULETABLE_RUNNING;


   #if (osdScheduleTableSyncUsed != 0)
      COVER(0x069C)
      osSTFlags[ScheduleTableID]=osdHRST_ForceAsync|osdHRST_NeedMod;
      osSTDiffTime[ScheduleTableID] = 0;     /* Synchronisation rest */
   #endif

   {
      COVER(0x069D)
      /* fullfills [SR:018] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      /* fullfills [SR:041] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      #if (osdScheduleTableSyncUsed != 0)
         /* Do the modulo only when the schedule table maximum value is below TickType maximum value */
         if((oskSTMaxAllowed[ScheduleTableID] + 1) == 0)
         {
            TickType Zero = 0;
            COVER(0x069E)
            /* MISRA RULE 21.1 not violated: Rule 21.1 is not violated, static code checking is applied.
               And the line below is fully intended: We want to get the negative of number in a finite ring,
               and some compilers complained about the standard method (writing "-Offset").
             */
            osSTTime[ScheduleTableID] = Zero-Offset;  /* PRQA S 3372 */            
         }
         else
         {
            COVER(0x069F)
            osSTTime[ScheduleTableID] = (TickType) ((oskSTMaxAllowed[ScheduleTableID] + 1) - 
                                                    (Offset % (oskSTMaxAllowed[ScheduleTableID] + 1)));
         }
      #endif
      osSTCurrentIndex[ScheduleTableID]= oskSTStartIndex[ScheduleTableID];
      /* initial offset needs no consideration as we have a fake expiry point at zero if there is no user expiry point */
      osSTCounter[ScheduleTableID]= Offset;
   }
   COVER(0x06A0)

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SR_END_CRITICAL()
   OS_SR_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
}
#endif /* #ifndef osdStartScheduleTableRelNotUsed */

#ifndef osdStartScheduleTableAbsNotUsed
/*-+--------------------------------------------------------------------------
 / StartScheduleTableAbs
 / =====================
 /
 / Syntax: 
 /    StatusType StartScheduleTableAbs(
 /                                      ScheduleTableType ScheduleTableID, 
 /                                      TickType Tickvalue
 /                                    )
 / Parameter (In):
 /   ScheduleTableType ScheduleTableID
 /   TickType Tickvalue
 / Parameter (Out):
 /   none
 / Description:
 /   If its input parameters are valid, StartScheduleTableAbs() starts the
 /   processing of a schedule table <ScheduleTableID> at its first expiry point
 /   when the underlaying counter reaches the value <Tickvalue>.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /       - Schedule table <ScheduleTableID> is in any other state than 
 /         SCHEDULETABLE_STOPPED, E_OS_STATE
 /    EXTENDED: 
 /       - <ScheduleTableID> not valid, E_OS_ID
 /       - <Tickvalue> too big, E_OS_VALUE
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC1, SC2, SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:196] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 StatusType osqFunc2 osStartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Tickvalue)
{
   /* CovComment 4:  Function: osStartScheduleTableAbs */
   OS_SS_ENTRY()
   COVER(0x06A1)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x06A2)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_StartScheduleTableAbs & osCurrentContext) == 0)
   {
      COVER(0x06A3)
      #if osdErrorHook 
         COVER(0x06A4)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableAbs_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableAbs_Tickvalue(Tickvalue)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSSCallContext);
      #if osdErrorHook 
         COVER(0x06A5)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SS_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x06A6)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleTableID >= osrNumberOfScheduleTables)
   {
      COVER(0x06A7)
      #if osdErrorHook
         COVER(0x06A8)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableAbs_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableAbs_Tickvalue(Tickvalue)
      #endif
      osAPIError(E_OS_ID, osdErrSSWrongID);
      #if osdErrorHook
         COVER(0x06A9)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SS_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x06AA)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[ScheduleTableID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x06AB)
      #if osdErrorHook 
         COVER(0x06AC)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableAbs_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableAbs_Tickvalue(Tickvalue)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSSNoAccess);
      #if osdErrorHook 
         COVER(0x06AD)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SS_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x06AE)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   /* check against MAXALLOWEDVALUE */
      if (Tickvalue > oskCounterMaxAllowedValue[oskCounterOfST[ScheduleTableID]]) 
      {
         /* fullfills [SR:194] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
         COVER(0x06AF)
         #if osdErrorHook 
            COVER(0x06B0)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveStartScheduleTableAbs_ScheduleTableID(ScheduleTableID)
            osSaveStartScheduleTableAbs_Tickvalue(Tickvalue)
         #endif
         osAPIError(E_OS_VALUE, osdErrSSTickvalueTooBig);
         #if osdErrorHook 
            COVER(0x06B1)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SS_EXIT()
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x06B2)

   if (osIntAPIStatus != 0)
   {
      COVER(0x06B3)
      #if osdErrorHook 
         COVER(0x06B4)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableAbs_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableAbs_Tickvalue(Tickvalue)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSSIntAPIDisabled);
      #if osdErrorHook 
         COVER(0x06B5)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SS_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x06B6)
   
#endif /*STATUS_LEVEL*/
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SS_START_CRITICAL()

   if (osSTState[ScheduleTableID] != SCHEDULETABLE_STOPPED)
   {
      /* fullfills [SR:195] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x06B7)
      #if osdErrorHook
         COVER(0x06B8)
         osSaveStartScheduleTableAbs_ScheduleTableID(ScheduleTableID)
         osSaveStartScheduleTableAbs_Tickvalue(Tickvalue)
      #endif
      osAPIError(E_OS_STATE, osdErrSSAlreadyRunningOrNext);
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SS_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x06B9)
  


   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   {
      /*No synchronization necessary, start immediately when Tickvalue is reached*/
      TickType now;
      
      #if osdUseCounterAPI
         COVER(0x06BA)
         now = osCounter[oskCounterOfST[ScheduleTableID]];
      #else
         COVER(0x06BB)
         now = osGetSystemCounter();
      #endif

      #if (osdUseHeapAlarm != 0)
         COVER(0x06BC)
         now &= (TickType)(~osdSystemCounterMask);
      #else
         COVER(0x06BD)
      #endif
      /*check, if Tickvalue is after the next overflow*/
      /* fullfills [SR:017] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      /* fullfills [SR:041] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      /* initial offset needs no consideration as we have a fake expiry point at zero if there is no user expiry point */
      if (Tickvalue > now)
      {
         COVER(0x06BE)
         /*Tickvalue is before next overflow*/
         osSTCounter[ScheduleTableID] = Tickvalue - now;
      }
      else
      {
         COVER(0x06BF)
         /*an overflow is necessary to reach Tickvalue time*/
         osSTCounter[ScheduleTableID] = (TickType) (Tickvalue + 1 +
            ( oskCounterMaxAllowedValue[oskCounterOfST[ScheduleTableID]] - now ));
      }
      #if (osdScheduleTableSyncUsed != 0)
      {
         TickType Offset;
         if( Tickvalue > now )
         {
            COVER(0x06C0)
            Offset = Tickvalue-now;
         }
         else
         {
            COVER(0x06C1)
            Offset = (TickType)(oskSTMaxAllowed[ScheduleTableID] + 1 - now + Tickvalue);
         }
         /* Do the modulo only when the schedule table maximum value is below TickType maximum value */
         if((oskSTMaxAllowed[ScheduleTableID] + 1) == 0)
         {
            TickType Zero = 0;
            COVER(0x06C2)
            osSTTime[ScheduleTableID] = Zero-Offset;             
         }
         else
         {
            COVER(0x06C3)
            osSTTime[ScheduleTableID] = (TickType)(oskSTMaxAllowed[ScheduleTableID] + 1 - 
                                                   (Offset % (oskSTMaxAllowed[ScheduleTableID] + 1)));
         }
      }
      #endif /* (osdScheduleTableSyncUsed != 0) */
   }

   osSTState[ScheduleTableID]= SCHEDULETABLE_RUNNING;

#if (osdScheduleTableSyncUsed != 0)
   COVER(0x06C4)
   osSTFlags[ScheduleTableID]=osdHRST_ForceAsync|osdHRST_NeedMod;
   osSTTime[ScheduleTableID]=0; 
   osSTDiffTime[ScheduleTableID] = 0;
#endif

   COVER(0x06C5)
   osSTCurrentIndex[ScheduleTableID]= oskSTStartIndex[ScheduleTableID];



   COVER(0x06C6)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */

   OS_SS_END_CRITICAL()
   OS_SS_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
}
#endif /* #ifndef osdStartScheduleTableAbsNotUsed */

#if (osdScheduleTableSyncUsed!=0) && !defined(osdStartScheduleTableSynchronNotUsed)
/*-+--------------------------------------------------------------------------
 / StartScheduleTableSynchron
 / ==========================
 /
 / Syntax: 
 /    StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
 / Parameter (In):
 /   ScheduleTableType ScheduleTableID
 / Parameter (Out):
 /   none
 / Description:
 /   If its input parameters are valid, StartScheduleTableSynchron() starts
 /   the processing of schedule table <ScheduleTableID> at its first expiry 
 /   point after the global time of the schedule table is set via 
 /   SyncScheduleTable().
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /       - Schedule table <ScheduleTableID> is in any other state than 
 /         SCHEDULETABLE_STOPPED, E_OS_STATE
 /    EXTENDED: 
 /       - <ScheduleTableID> not valid, E_OS_ID
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /     SC2, SC4
 /--------------------------------------------------------------------------*/
osqFunc1 StatusType osqFunc2 osStartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
{
   /* CovComment 4:  Function: osStartScheduleTableSynchron */
   OS_SO_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS

   COVER(0x06C7)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_StartScheduleTableSynchron & osCurrentContext) == 0)
   {
      COVER(0x06C8)
      #if osdErrorHook 
         COVER(0x06C9)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableSynchron_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrTSCallContext);
      #if osdErrorHook 
         COVER(0x06CA)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SO_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x06CB)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleTableID >= osrNumberOfScheduleTables)
   {
      /* fullfills [SR:208]: return E_OS_ID if schedule table ID is not valid */
      COVER(0x06CC)
      #if osdErrorHook
         COVER(0x06CD)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableSynchron_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_ID, osdErrTSWrongID);
      #if osdErrorHook
         COVER(0x06CE)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SO_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x06CF)

   if (oskSTSyncKind[ScheduleTableID]!=osdSTSyncExplicit)
   {
      /* fullfills [SR:208]: return E_OS_ID if schedule table is not explicitely synchronized */
      COVER(0x06D0)
      #if osdErrorHook
         COVER(0x06D1)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableSynchron_ScheduleTableID(ScheduleTableID)
      #endif
      COVER(0x06D2)
      osAPIError(E_OS_ID, osdErrTSSyncKindNotExplicit);
      #if osdErrorHook
         COVER(0x06D3)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SO_EXIT()
      return E_OS_ID;   
   }
   ELSEWITHCOVER(0x06D4)


#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[ScheduleTableID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x06D5)
      #if osdErrorHook 
         COVER(0x06D6)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableSynchron_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_ACCESS, osdErrTSNoAccess);
      #if osdErrorHook 
         COVER(0x06D7)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SO_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x06D8)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x06D9)
      #if osdErrorHook 
         COVER(0x06DA)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStartScheduleTableSynchron_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrTSIntAPIDisabled);
      #if osdErrorHook 
         COVER(0x06DB)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SO_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x06DC)


   
   
#endif /*STATUS_LEVEL*/

   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SO_START_CRITICAL()

   if (osSTState[ScheduleTableID] != SCHEDULETABLE_STOPPED)
   {
      COVER(0x06DD)
      #if osdErrorHook
         COVER(0x06DE)
         osSaveStartScheduleTableSynchron_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_STATE, osdErrTSSTAlreadyRunning);
      COVER(0x06DF)
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SO_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x06E0)
      
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */
   osSTFlags[ScheduleTableID] = osdHRST_NoSpecialInit;
   osSTTime[ScheduleTableID] = 0;       /* Start Offset Local Time */
   osSTDiffTime[ScheduleTableID] = 0;   /* Start synchronized  */
   osSTState[ScheduleTableID]= SCHEDULETABLE_WAITING;
   
   
      COVER(0x06E1)
      osSTCurrentIndex[ScheduleTableID]= oskSTStartIndex[ScheduleTableID];
   
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */

   OS_SO_END_CRITICAL()
   OS_SO_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
} /* StartScheduleTableSynchron() */

#endif /* if (osdScheduleTableSyncUsed!=0) && !defined(osdStartScheduleTableSynchronNotUsed) */


#ifndef osdStopScheduleTableNotUsed
/*-+--------------------------------------------------------------------------
 / StopScheduleTable
 / =================
 /
 / Syntax: 
 /    StatusType StopScheduleTable(ScheduleTableType ScheduleTableID)
 / Parameter (In):
 /   ScheduleTableType ScheduleTableID
 / Parameter (Out):
 /   none
 / Description:
 /   If its input parameter is valid, StopScheduleTable() stops the schedule 
 /   table <ScheduleTableID> from processing any further expiry points.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /       - Schedule table <ScheduleTableID> is in the state 
 /         SCHEDULETABLE_STOPPED or SCHEDULETABLE_NEXT, E_OS_NOFUNC
 /    EXTENDED: 
 /       - <ScheduleTableID> not valid, E_OS_ID
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC1, SC2, SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:019] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:199] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 StatusType osqFunc2 osStopScheduleTable(ScheduleTableType ScheduleTableID)
{
   /* CovComment 4:  Function: osStopScheduleTable */
   OS_SP_ENTRY()
   COVER(0x06E2)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x06E3)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_StopScheduleTable & osCurrentContext) == 0)
   {
      COVER(0x06E4)
      #if osdErrorHook 
         COVER(0x06E5)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStopScheduleTable_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSPCallContext);
      #if osdErrorHook 
         COVER(0x06E6)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SP_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x06E7)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleTableID >= osrNumberOfScheduleTables)
   {
      /* fullfills [SR:197] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x06E8)
      #if osdErrorHook
         COVER(0x06E9)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStopScheduleTable_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_ID, osdErrSPWrongID);
      #if osdErrorHook
         COVER(0x06EA)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SP_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x06EB)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[ScheduleTableID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x06EC)
      #if osdErrorHook 
         COVER(0x06ED)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStopScheduleTable_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSPNoAccess);
      #if osdErrorHook 
         COVER(0x06EE)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SP_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x06EF)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x06F0)
      #if osdErrorHook 
         COVER(0x06F1)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveStopScheduleTable_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSPIntAPIDisabled);
      #if osdErrorHook 
         COVER(0x06F2)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SP_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x06F3)
#endif /*STATUS_LEVEL*/
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SP_START_CRITICAL()

   if (
       (osSTState[ScheduleTableID] == SCHEDULETABLE_STOPPED) ||
       (osSTState[ScheduleTableID] == SCHEDULETABLE_NEXT)
      )
   {
      /* fullfills [SR:198] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x06F4)
      #if osdErrorHook
         COVER(0x06F5)
         osSaveStopScheduleTable_ScheduleTableID(ScheduleTableID)
      #endif
      osAPIError(E_OS_NOFUNC, osdErrSPNotRunning);
      COVER(0x06F6)
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SP_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x06F7)
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   /*Exists a next schedule table?*/
   /* fullfills [SR:207] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   if (osSTNextTable[ScheduleTableID] != INVALID_SCHEDULETABLE)
   {
      COVER(0x06F8)
      osSTState[osSTNextTable[ScheduleTableID]] = SCHEDULETABLE_STOPPED;
   }
   ELSEWITHCOVER(0x06F9)

   osSTNextTable[ScheduleTableID]= INVALID_SCHEDULETABLE;
   osSTState[ScheduleTableID]= SCHEDULETABLE_STOPPED;

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SP_END_CRITICAL()
   OS_SP_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
}
#endif /* osdStopScheduleTableNotUsed */

#if ((osdSC == SC3) || (osdSC == SC4))
/* internal stopping of scheduletables */
/* !!! must be called with interrupts globally disabled! */
osqFunc1 void osqFunc2 osSysStopScheduleTable(ScheduleTableType ScheduleTableID)
{
   /* CovComment 4:  Function: osSysStopScheduleTable */
   /*Exists a next schedule table?*/
   if (osSTNextTable[ScheduleTableID] != INVALID_SCHEDULETABLE)
   {
      COVER(0x06FA)
      /*Yes: reset it to not started*/
      osSTState[osSTNextTable[ScheduleTableID]] = SCHEDULETABLE_STOPPED;
   }
   ELSEWITHCOVER(0x06FB)
   
   osSTNextTable[ScheduleTableID]= INVALID_SCHEDULETABLE;

   /*reset schedule table to not started*/
   osSTState[ScheduleTableID] = SCHEDULETABLE_STOPPED;
}
#endif /* ((osdSC == SC3) || (osdSC == SC4)) */




#ifndef osdNextScheduleTableNotUsed
/*-+--------------------------------------------------------------------------
 / NextScheduleTable
 / ===================
 /
 / Syntax: 
 /   StatusType NextScheduleTable( 
 /                                 ScheduleTableType ScheduleTableID_current,
 /                                 ScheduleTableType ScheduleTableID_next 
 /                               )
 / Parameter (In):
 /   ScheduleTableType ScheduleTableID_current,
 /   ScheduleTableType ScheduleTableID_next 
 / Parameter (Out):
 /   none
 / Description:
 /   Start the schedule table <ScheduleTableID_next> after 
 /   <ScheduleTableID_current> reaches its period/length. In case, an other 
 /   schedule table is already configured to be started after 
 /   <ScheduleTableID_current> reaches its period/length, that other schedule
 /   table is replaced by <ScheduleTableID_next>.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /       - The schedule table <ScheduleTableID_current> is in the state
 /         SCHEDULETABLE_STOPPED, E_OS_NOFUNC
 /       - The schedule table <ScheduleTableID_current> is in the state
 /         SCHEDULETABLE_NEXT, E_OS_NOFUNC
 /    EXTENDED: 
 /       - <ScheduleTableID_current> not valid, E_OS_ID
 /       - <ScheduleTableID_nest> not valid, E_OS_ID
 /       - Schedule table <ScheduleTableID_next> is in any other state than
 /         SCHEDULETABLE_STOPPED, E_OS_STATE
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC1, SC2, SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:020] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:204] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 StatusType osqFunc2 osNextScheduleTable(ScheduleTableType ScheduleTableID_current, ScheduleTableType ScheduleTableID_next )
{
   /* CovComment 4:  Function: osNextScheduleTable */
   OS_SN_ENTRY()
   COVER(0x06FC)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x06FD)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_NextScheduleTable & osCurrentContext) == 0)
   {
      COVER(0x06FE)
      #if osdErrorHook 
         COVER(0x06FF)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSNCallContext);
      #if osdErrorHook 
         COVER(0x0700)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SN_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0701)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleTableID_current >= osrNumberOfScheduleTables)
   {
      /* fullfills [SR:200] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x0702)
      #if osdErrorHook
         COVER(0x0703)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_ID, osdErrSNWrongCurrentID);
      #if osdErrorHook
         COVER(0x0704)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SN_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0705)
   if (ScheduleTableID_next >= osrNumberOfScheduleTables)
   {
      /* fullfills [SR:200] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x0706)
      #if osdErrorHook
         COVER(0x0707)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_ID, osdErrSNWrongNextID);
      #if osdErrorHook
         COVER(0x0708)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SN_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0709)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for these objects */
   if (    ((oskApplScheduleTableAccess[ScheduleTableID_current] & (1UL << osActiveApplicationID)) == 0) 
        || ((oskApplScheduleTableAccess[ScheduleTableID_next] & (1UL << osActiveApplicationID)) == 0)
      )
   {
      COVER(0x070A)
      #if osdErrorHook 
         COVER(0x070B)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSNNoAccess);
      #if osdErrorHook 
         COVER(0x070C)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SN_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x070D)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

#if (osdNumberOfCounters > 1)
   if (oskCounterOfST[ScheduleTableID_next] != 
       oskCounterOfST[ScheduleTableID_current] )
   {
      /* fullfills [SR:201] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x070E)
      #if osdErrorHook
         COVER(0x070F)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_ID, osdErrSNDifferentCounters);
      #if osdErrorHook
         COVER(0x0710)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SN_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0711)
#endif

   if (osIntAPIStatus != 0)
   {
      COVER(0x0712)
      #if osdErrorHook 
         COVER(0x0713)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSNIntAPIDisabled);
      #if osdErrorHook 
         COVER(0x0714)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SN_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x0715)
#endif /*STATUS_LEVEL*/
   COVER(0x0716)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SN_START_CRITICAL()

   if (osSTState[ScheduleTableID_current] < SCHEDULETABLE_WAITING)
   {
      /* fullfills [SR:202] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x0717)
      #if osdErrorHook
         COVER(0x0718)
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_NOFUNC, osdErrSNNotRunning);
      COVER(0x0719)
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SN_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x071A)
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x071B)
   if (osSTState[ScheduleTableID_next] != SCHEDULETABLE_STOPPED)
   {
      /* fullfills [SR:203] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x071C)
      #if osdErrorHook
         COVER(0x071D)
         osSaveNextScheduleTable_ScheduleTableID_current(ScheduleTableID_current)
         osSaveNextScheduleTable_ScheduleTableID_next(ScheduleTableID_next)
      #endif
      osAPIError(E_OS_STATE, osdErrSNAlreadyRunningOrNext);
      COVER(0x071E)
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SN_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x071F)
#endif /*STATUS_LEVEL*/

   COVER(0x0720)
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   /* Is there already a schedule table planned for next? */
   /* fullfills [SR:205] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   if (osSTNextTable[ScheduleTableID_current] != INVALID_SCHEDULETABLE)
   {
      COVER(0x0721)
      /* Yes: set its state back to not started */
      osSTState[osSTNextTable[ScheduleTableID_current]]= SCHEDULETABLE_STOPPED;
   }
   ELSEWITHCOVER(0x0722)

   osSTNextTable[ScheduleTableID_current]= ScheduleTableID_next;
   osSTState[ScheduleTableID_next]= SCHEDULETABLE_NEXT;

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SN_END_CRITICAL()
   OS_SN_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
}
#endif /* #ifndef osdNextScheduleTableNotUsed */


#ifndef osdGetScheduleTableStatusNotUsed
/*-+--------------------------------------------------------------------------
 / GetScheduleTableStatus
 / ======================
 /
 / Syntax: 
 /   StatusType osGetScheduleTableStatus(
 /                                  ScheduleTableType ScheduleTableID,
 /                                  ScheduleTableStatusRefType ScheduleStatus
 /                                      )
 / Parameter (In):
 /   ScheduleTableType ScheduleTableID,
 / Parameter (Out):
 /   ScheduleTableStatusRefType ScheduleStatus
 / Description:
 /   Returns the state of the schedule table <ScheduleTableID> via the 
 /   (out) parameter ScheduleTableStatusRefType.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /    EXTENDED: 
 /       - <ScheduleTableID> not valid, E_OS_ID
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC1, SC2, SC3, SC4
 /--------------------------------------------------------------------------*/
/* fullfills [SR:021] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 StatusType osqFunc2 osGetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus)
{
   /* CovComment 4:  Function: osGetScheduleTableStatus */
   OS_SG_ENTRY()
   COVER(0x0723)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0724)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_GetScheduleTableStatus & osCurrentContext) == 0)
   {
      COVER(0x0725)
      #if osdErrorHook 
        COVER(0x0726)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveGetScheduleTableStatus_ScheduleID(ScheduleTableID)
        osSaveGetScheduleTableStatus_ScheduleStatus(ScheduleStatus)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSGCallContext);
      #if osdErrorHook 
         COVER(0x0727)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SG_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0728)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleTableID >= osrNumberOfScheduleTables)
   {
      /* fullfills [SR:225] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
      COVER(0x0729)
      #if osdErrorHook
        COVER(0x072A)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveGetScheduleTableStatus_ScheduleID(ScheduleTableID)
        osSaveGetScheduleTableStatus_ScheduleStatus(ScheduleStatus)
      #endif
      osAPIError(E_OS_ID, osdErrSGWrongID);
      #if osdErrorHook
        COVER(0x072B)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SG_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x072C)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[ScheduleTableID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x072D)
      #if osdErrorHook 
        COVER(0x072E)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveGetScheduleTableStatus_ScheduleID(ScheduleTableID)
        osSaveGetScheduleTableStatus_ScheduleStatus(ScheduleStatus)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSGNoAccess);
      #if osdErrorHook 
        COVER(0x072F)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SG_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0730)
   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)ScheduleStatus, (osuint32)sizeof(ScheduleTableStatusType)) == osdFALSE)
      {  /* no writable area found -> no write acces rights for result */
         COVER(0x0731)
         #if osdErrorHook 
            COVER(0x0732)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveGetScheduleTableStatus_ScheduleID(ScheduleTableID)
            osSaveGetScheduleTableStatus_ScheduleStatus(ScheduleStatus)
         #endif
         osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrSGIllegalAddr);
         #if osdErrorHook 
            COVER(0x0733)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SG_EXIT()
         return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x0734)
   }  
   ELSEWITHCOVER(0x0735)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/
   if (osIntAPIStatus != 0)
   {
      COVER(0x0736)
      #if osdErrorHook 
        COVER(0x0737)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveGetScheduleTableStatus_ScheduleID(ScheduleTableID)
        osSaveGetScheduleTableStatus_ScheduleStatus(ScheduleStatus)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSGIntAPIDisabled);
      #if osdErrorHook 
        COVER(0x0738)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SG_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x0739)
#endif /*STATUS_LEVEL*/

   COVER(0x073A)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SG_START_CRITICAL()

   /* fullfills [SR:220] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   /* fullfills [SR:221] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   COVER(0x073B)
   *ScheduleStatus = osSTState[ScheduleTableID];

   OS_SG_END_CRITICAL()
   OS_SG_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
}
#endif /* #ifndef osdGetScheduleTableStatusNotUsed */

/*-+--------------------------------------------------------------------------
 / osInitScheduleTables
 /
 / Funktion:
 /   Initializes the variables, used for schedule tables.
 / Context:
 /   Interrupts have to be disabled up to the system level at least.
 / Parameter:
 /   none
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osInitScheduleTables(void)
{
   /* CovComment 4:  Function: osInitScheduleTables */
   ScheduleTableType stID;


   /* loop through all schedule tables */
   for (stID= 0; stID < osrNumberOfScheduleTables; stID++)
   {
      osuint8 condition=0;
      COVER(0x073C)
               
      if ((oskScheduleTableAutoStart[stID] & osActiveAppMode) != 0)
      {
         condition |= osdSTINIT_AUTO;
         #if (osdScheduleTableSyncUsed!=0)
         if (oskScheduleTableAutoStartType[stID]==0)
         {
            COVER(0x073D)
            condition |= osdSTINIT_SYNCSTART;
         }
         ELSEWITHCOVER(0x073E)
         #endif
      }
      ELSEWITHCOVER(0x073F)
      
      #if (osdNumberOfHiResSchedTabs>0)
      if (oskCounterOfST[stID]==HiResSchedTabCounter)
      {
         COVER(0x0740)
         condition |= osdSTINIT_HRST;
      }
      ELSEWITHCOVER(0x0741)
      #endif
      
      /* osSTNextTable */
      osSTNextTable[stID]     = INVALID_SCHEDULETABLE;
      
      /* osSTState */
      #if (osdScheduleTableSyncUsed!=0)
      COVER(0x0742)
      if ((condition & osdSTINIT_SYNCSTART) != 0)
      {
         COVER(0x0743)
         osSTState[stID]         = SCHEDULETABLE_WAITING;
      }
      else 
      #endif
      if ((condition & osdSTINIT_AUTO) != 0)
      {
         COVER(0x0744)
         osSTState[stID]         = SCHEDULETABLE_RUNNING;
      }
      else
      {
         COVER(0x0745)
         osSTState[stID]         = SCHEDULETABLE_STOPPED;
      }
      
      
      /* osSTCounter */      
      if ((condition & osdSTINIT_AUTO) != 0)
      {              
         COVER(0x0746)
         #if (osdNumberOfHiResSchedTabs>0)
         COVER(0x0747)
         if (!(condition&(osdSTINIT_HRST)))
         {
            COVER(0x0748)
         #else
            COVER(0x0749)
         #endif
            osSTCounter[stID] = oskAutostartScheduleTableOffset[stID];
         #if (osdNumberOfHiResSchedTabs>0)
         }
         ELSEWITHCOVER(0x074A)
         /* HRST Init is done with the rest of the HRST init (below) */
         #endif
         /* No need to initialize if not immediately started. */
      
         /* osSTTime osSTDiffTime */
         #if (osdScheduleTableSyncUsed != 0)
         COVER(0x074B)
         osSTTime[stID]     = 0;
         osSTDiffTime[stID] = 0;
         #endif
           
      
         /* osSTCurrentIndex */
         #if (osdScheduleTableSyncUsed != 0)
         COVER(0x074C)
         if ((condition & osdSTINIT_SYNCSTART) == 0)
         {
         #endif
            COVER(0x074D)
            #if (osdNumberOfHiResSchedTabs>0)
            if ((condition & osdSTINIT_HRST) != 0)
            {     
              COVER(0x074E)
               osSTCurrentIndex[stID]= oskSTStartIndex[stID];
            }
            else
            {
               COVER(0x074F)
            #else
               COVER(0x0750)
            #endif
               osSTCurrentIndex[stID]= oskSTStartIndex[stID];
            #if (osdNumberOfHiResSchedTabs>0)
            }
            #endif
         #if (osdScheduleTableSyncUsed != 0)
         }
         ELSEWITHCOVER(0x0751)
         #endif
      }
      ELSEWITHCOVER(0x0752)
      
      /* HRST init if we have an autostarted HRST */
      #if (osdNumberOfHiResSchedTabs>0)
      if ((condition & osdSTINIT_HRST) != 0)
      {
         if ((condition & osdSTINIT_AUTO) != 0)
         {      
             COVER(0x0753)
             osSysErrAssert((osHRSTActiveST==INVALID_SCHEDULETABLE), osdErrSTMultipleHRSTAutostart);
             osHRSTActiveST=stID;
             osHRSTCurrentEP= oskSTStartIndex[stID];
             osHRSTUpdateTime();
             osSTCounter[stID]=oskAutostartScheduleTableOffset[stID]+osHRSTLocalTime;
             osHRSTRelSTTime=0;
             osHRSTEnableIntCompareHW();    
             (void)osHRSTSetCompare(osSTCounter[stID]);
            /* No special handling of already expired time */
         }
         ELSEWITHCOVER(0x0754)
      }
      ELSEWITHCOVER(0x0755)
      #endif
      
   } /* for (all schedule tables) */
}

#if (osdNumberOfScheduleTables>osdNumberOfHiResSchedTabs)

#if (osdScheduleTableSyncUsed != 0)

static osqFunc1 void osqFunc2 osSTUpdateTime(ScheduleTableType currentST, osTickTimeDiffType DiffToSet);
static osqFunc1 void osqFunc2 osSTUpdateTime(ScheduleTableType currentST, osTickTimeDiffType DiffToSet)
{
   /* CovComment 4:  Function: osSTUpdateTime */
   /*Correct the schedule table time*/
   TickType NewTime;
   if (DiffToSet>=0)   
   {
      NewTime = osSTTime[currentST] + ((TickType)DiffToSet);
   }
   else
   {
      NewTime = osSTTime[currentST] - ((TickType)-DiffToSet);
   }
   /*Detect value over/underflow*/
   if (DiffToSet >= 0) /* Actually, will never be called with (DiffSet==0) */
   {
      if (NewTime > oskSTMaxAllowed[currentST])
      {
         COVER(0x0756)
         /*correct the overflow of maxallowedvalue*/
         NewTime = NewTime - (TickType)oskSTMaxAllowed[currentST] - 1;
      }
      else if (NewTime < osSTTime[currentST])
      {
         COVER(0x0757)
         /*correct overflow of the maximum value in TickType*/
         NewTime = NewTime - (TickType)oskSTMaxAllowed[currentST] - 1;
      }
      else 
      { 
         /* This else is only here for MISRA, Rule 60. Intentionally left blank */
         COVER(0x0758)
         ;
      } 
   }
   else 
   {
      /*DiffToSet < 0 here, as it is !=0 and not >0*/
      if (NewTime > osSTTime[currentST])
      {
         COVER(0x0759)
         /*correct underflow*/
         NewTime += oskSTMaxAllowed[currentST] + 1;
      }
      ELSEWITHCOVER(0x075A)
   }
   /*Set the corrected time*/
   osSTTime[currentST] = NewTime;
}

/*
 *  Function: osSTCalculateSyncState
 *    Checks for the given schedule table, whether or not it is synchronous and
 *    stores the result in the variable osSTState[].
 *  Parameters:
 *    currentST:  Index of the current schedule table
 *  Return value:
 *    none
 *  Call context:
 *    Called inside the functions osSTWorkActions and osSyncScheduleTable only. 
 *    Interrupts are disabled up to the system level at least. The function is 
 *    called only for schedule tables that are running or running and synchronous. 
 *    All schedule tables, this function is called for, are explicitely synchronized 
 *    schedule tables.
 */

 /* MISRA RULE 8.10 VIOLATION: In some configurations, this function might be used from the same file only.
    Catching all these configurations is complex and error prone */
osqFunc1 void osqFunc2 osSTCalculateSyncState(ScheduleTableType currentST) /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osSTCalculateSyncState */

   /* Has the schedule table been set to asynchronous? */
   if ((osSTFlags[currentST] & osdHRST_ForceAsync) == 0)
   {
      /* NO: calculate the new status */
      if (osSTDiffTime[currentST] >=  (osTickTimeDiffType)oskPrecision[currentST]) 
      {
         COVER(0x075B)
         osSTState[currentST] = SCHEDULETABLE_RUNNING;
      }
      else if (osSTDiffTime[currentST] <= (-(osTickTimeDiffType)oskPrecision[currentST]))
      {
         COVER(0x075C)
         osSTState[currentST] = SCHEDULETABLE_RUNNING;
      }
      else
      {
         COVER(0x075D)
         osSTState[currentST] = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
      }
   }
   ELSEWITHCOVER(0x075E)
}

/*
 *  Function: osSTDoSyncStep
 *    Performs a synchronization step
 *  Parameters:
 *    currentST:  Index of the current schedule table
 *    RelOffs:    The relative offset to the next expiry point
 *    MaxRetard:  The value of the attribute MAXRETARD for this expiry point
 *    MaxAdvance: The value of the attribure MAXADVANCE for this expiry point
 *  Return value:
 *    The corrected relative offset to the next expity point
 *  Call context:
 *    Called inside the function osSTWorkActions only. Interrupts are disabled 
 *    up to the system level at least. The function is called only for schedule
 *    tables that are running or running and synchronous. All schedule tables,
 *    this function is called for, are explicitely synchronized schedule tables.
 */
/* MISRA RULE 14.1 VIOLATION: In some configurations, this function might be unused. Catching 
   all these cases is too complex and error prone 
 */
osqFunc1 TickType osqFunc2 osSTDoSyncStep(ScheduleTableType currentST, TickType RelOffs, TickType MaxRetard, TickType MaxAdvance) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osSTDoSyncStep */
   osTickTimeDiffType DiffToSet = osSTDiffTime[currentST];/* current deviation to global time */
   if(DiffToSet == 0)
   {
      COVER(0x075F)
      /* nothing to do, really */
      return RelOffs;
   }
   ELSEWITHCOVER(0x0760)

   if (DiffToSet>0)
   {
      if ((TickType)DiffToSet > MaxRetard)
      {
         COVER(0x0761)
         DiffToSet = (osTickTimeDiffType)MaxRetard; 
      }
      ELSEWITHCOVER(0x0762)
      RelOffs=(RelOffs - (TickType)DiffToSet);
   }
   else
   {
      /* this is the case (DiffToSet<0), 
        the case (DiffToSet==0) is handled above 
      */
      /* -MaxAdvance is always possible, checked by the generator */
      if (DiffToSet < -((osTickTimeDiffType)MaxAdvance))
      {
         COVER(0x0763)
         DiffToSet= - ((osTickTimeDiffType)MaxAdvance); 
      }
      ELSEWITHCOVER(0x0764)
      RelOffs=(RelOffs + (TickType)-DiffToSet);
   }

   /* Update schedule table time */
   osSTUpdateTime(currentST, DiffToSet);   
   /*Correct the remaining difference to the global time*/
   osSTDiffTime[currentST] -= DiffToSet;
        
   return RelOffs;
}

#endif /* #if (osdScheduleTableSyncUsed != 0) */
 
/* If there is a next schedule table for the currentST, copy all state from 
   currentST to the next - except osSTCounter and osSTCurrentIndex.
   State is corrected. 
 */ 
static osqFunc1 ScheduleTableType osqFunc2 osSTSwitchToNext(ScheduleTableType currentST);

static osqFunc1 ScheduleTableType osqFunc2 osSTSwitchToNext(ScheduleTableType currentST)
{
   /* CovComment 4:  Function: osSTSwitchToNext */
   ScheduleTableType next= osSTNextTable[currentST];
   if (next != INVALID_SCHEDULETABLE)
   {
      /* activate the next table */
      osSTNextTable[currentST]= INVALID_SCHEDULETABLE;
      
      #if (osdScheduleTableSyncUsed != 0)
         /*the next schedule table derives synchrony from the current table*/
         osSTTime[next]        = osSTTime[currentST];
         if (next>currentST)
         {
            COVER(0x0765)
            osSTUpdateTime(next,-1);
         }
         ELSEWITHCOVER(0x0766)
         osSTDiffTime[next]    = osSTDiffTime[currentST];
         osSTState[next]       = osSTState[currentST];
      #else
         COVER(0x0767)
         osSTState[next]       = SCHEDULETABLE_RUNNING;   
      #endif
      
      /* stop the current schedule table*/
      osSTState[currentST]     = SCHEDULETABLE_STOPPED;
   }
   else
   {
      COVER(0x0768)
      next=currentST;
   }
   return next;
}



/*-+--------------------------------------------------------------------------
 / osWorkScheduleTables
 /
 / Funktion:
 /   Checks for schedule table actions and performs them if necessary. This
 /   function is called from the system timer ISR and from IncrementCounter().
 / Context:
 /   Interrupts are enabled. On level based systems, interrupts might be
 /   disabled up to the level of the caller.
 / Parameter:
 /   CounterID : The name of the counter, the schedule tables shall be 
 /               checked for.
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/
/* fullfills [SR:014] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:015] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:036] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:042] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:204] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 void osqFunc2 osWorkScheduleTables(CounterType CounterID)
{
   /* CovComment 4:  Function: osWorkScheduleTables */
   ScheduleTableType  StIdx;
   ScheduleTableType  currentST;
   ScheduleTableType  next;
   osbool             restart;
   osSTReactionType   react;
   TickType           diff;
#ifndef osdIncludeLibConfig
   osuint8 osHasBeenNexted[osrNumberOfScheduleTables];
#else
   /* declared in tcb.h */
#endif
   
   for(StIdx=0; StIdx<osrNumberOfScheduleTables; StIdx++)
   {
      COVER(0x0769)
      osHasBeenNexted[StIdx] = 0;
   }
   
   COVER(0x076A)
   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */

   StIdx = 0;/* Start with first schedule table */
   while (StIdx < osrNumberOfScheduleTables)
   {
      if(osHasBeenNexted[StIdx] == 0)
      {
         /* this if checks if the current ST has already been nexted to the previous one */
         /* then its potential action has already been performed (if initial offset is zero) */
         /* this if prevents the incrementation of the counter for a just nexted ST */
         if (oskCounterOfST[StIdx] == CounterID)
         {
            /* is the schedule table running? */
            if (osSTState[StIdx] >= SCHEDULETABLE_RUNNING)
            {
               COVER(0x076B)
               #if (osdScheduleTableSyncUsed != 0)
               COVER(0x076C)
               osSTUpdateTime(StIdx,1);
               #endif           
            
               /*Yes: decrement the counter*/
               osSTCounter[StIdx]--;
                        
               /*expiry point reached?*/
               if (osSTCounter[StIdx] == 0)
               {
                  COVER(0x076D)
                  /* Yes: perform all actions of this expiry point */
                  currentST=StIdx;
                  do 
                  {
                     COVER(0x076E)
                     restart=0;
                     /*loop for all actions at this expiry point*/
                     react=osSTWorkActions(&diff, osSTCurrentIndex[currentST]);
    
                     /* Now what? */   
                     switch (react)
                     {
                        case osdSTReact_Continue   :  
                        case osdSTReact_InitialDelay :
                        case osdSTReact_FinalDelay :
                           /* Normal EP of a table 
                           Just add the time offset to next ep to current time and continue */
                           COVER(0x076F)
                           osSTCounter[currentST]=diff;
                           /* point to the next expiry point action */
                           osSTCurrentIndex[currentST]++;
                           break;
                        
                        case osdSTReact_Repeat     :  
                        /* fullfills [SR:023] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
                        /* this is the end of a repeating non-sync table 
                           reset values and restart
                        */
                           next=osSTSwitchToNext(currentST);
                           if (next!=currentST)
                           {
                              COVER(0x0770)
                              /* there is a next'ed schedule table */
                              /* mark the next schedule table as the current one */
                              currentST=next;
                              /* store the information that the current ST has been nexted */
                              osHasBeenNexted[currentST] = 1;
                           }
                           ELSEWITHCOVER(0x0771)

                           /* initialize the expiry point action pointer */
                           osSTCurrentIndex[currentST]=oskSTStartIndex[currentST];
                           /* set restart to execute osSTWorkActions again on the expiry point at offset zero */
                           restart=1;
                          break;
    
                        case osdSTReact_Stop       :  
                        /* fullfills [SR:022] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
                        /* this is the end of a non-repeating non-sync table 
                           if it has a next schedule, do the same as for a repeating schedule table
                           otherwise, stop the schedule table
                        */
                           /* get ID of a potentially next'ed scheduletable */
                           next=osSTSwitchToNext(currentST);
                           if (next==currentST)
                           {
                              COVER(0x0772)
                              /* a next scheduletable does not exist -> simple stop the current one */
                              osSTState[currentST]= SCHEDULETABLE_STOPPED;                         
                           }
                           else
                           {
                              COVER(0x0773)
                              /* there is a next'ed schedule table */
                              /* mark the next schedule table as the current one */
                              currentST=next;
                              /* initialize the expiry point action pointer */
                              osSTCurrentIndex[currentST]=oskSTStartIndex[currentST];
                              osHasBeenNexted[currentST] = 1;
                              /* set restart to execute osSTWorkActions again on the expiry point at offset zero */
                              restart=1;
                           }
                           break;

                        default:
                           COVER(0x0774)
                           /* MISRA RULE 14.1 VIOLATION: (2 times) Some statements in the assertion might be not 
                            * reachable but this is the only way for secure usage of internal assertion API (3201)
                            * This is also the reason for usage of the <always true> expression (3346, no MISRA error)
                            */
                           osSysErrAssert(osdFALSE, osdErrWSUnknownReaction)   /* PRQA S 3201, 3346 */
                           osSTState[currentST] = SCHEDULETABLE_STOPPED;   /* PRQA S 3201 */
                           break;
                        
                     } /* switch */
                    
                  } while (restart != 0);
               } /* if (osSTState[StIdx] >= SCHEDULETABLE_RUNNING) */
               ELSEWITHCOVER(0x0775)
            } /* if (osSTCounter[StIdx] == 0) */
            ELSEWITHCOVER(0x0776)
         } /* if (oskCounterOfST[StIdx] == CounterID) */
         ELSEWITHCOVER(0x0777)
      } /* (osHasBeenNexted[StIdx] == 0) */
      ELSEWITHCOVER(0x0778)
      StIdx++; /* increment schedule table index */

      /* possibility to interrupt after 4 schedule tables*/
      if((StIdx % 4) == 0)
      {
         COVER(0x0779)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
      } 
      ELSEWITHCOVER(0x077A)
      
   }/*while (StIdx < osrNumberOfScheduleTables) */
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
}

#endif /* (osdNumberOfScheduleTables>osdNumberOfHiResSchedTabs) */

#if (osdScheduleTableSyncUsed != 0)



#ifndef osdSyncScheduleTableNotUsed
/*-+--------------------------------------------------------------------------
 / SyncScheduleTable
 / =================
 /
 / Syntax: 
 /    StatusType SyncScheduleTable(
 /                                  ScheduleTableType SchedTableID, 
 /                                  GlobalTimeTickType GlobalTime
 /                                )
 / Parameter (In):
 /   ScheduleTableType ScheTableID,
 /   GlobalTimeTickType GlobalTime
 / Parameter (Out):
 /   none
 / Description:
 /   This service provides the operating system with the current global time.
 /   It is used to synchronize the processing of the schedule table to global
 /   time.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /    EXTENDED: 
 /       - <ScheduleTableID> is not valid or cannot be synchronized, E_OS_ID
 /       - <GlobalTime> is too big, E_OS_VALUE
 /       - The state of the schedule table is so that in cannot be synchronized,
 /         E_OS_STATE
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC2, SC4
 /--------------------------------------------------------------------------*/
osqFunc1 StatusType osqFunc2 osSyncScheduleTable(ScheduleTableType SchedTableID, GlobalTimeTickType GlobalTime)
{
   /* CovComment 4:  Function: osSyncScheduleTable */
   TickTypeTmp osHeapDiffCaster     ;
   TickType    Zero; /*This variable is needed to prevent from compiler warnings only*/
   OS_SY_ENTRY()
   COVER(0x077B)
   Zero=0;

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x077C)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_SyncScheduleTable & osCurrentContext) == 0)
   {
      COVER(0x077D)
      #if osdErrorHook 
        COVER(0x077E)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSyncScheduleTable_ScheduleID(SchedTableID)
        osSaveSyncScheduleTable_GlobalTime(GlobalTime)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSYCallContext);
      #if osdErrorHook 
         COVER(0x077F)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SY_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0780)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (SchedTableID >= osrNumberOfScheduleTables)
   {
      COVER(0x0781)
      #if osdErrorHook
        COVER(0x0782)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSyncScheduleTable_ScheduleID(SchedTableID)
        osSaveSyncScheduleTable_GlobalTime(GlobalTime)
      #endif
      osAPIError(E_OS_ID, osdErrSYWrongID);
      #if osdErrorHook
        COVER(0x0783)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SY_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0784)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[SchedTableID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0785)
      #if osdErrorHook 
        COVER(0x0786)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSyncScheduleTable_ScheduleID(SchedTableID)
        osSaveSyncScheduleTable_GlobalTime(GlobalTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSYNoAccess);
      #if osdErrorHook 
         COVER(0x0787)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SY_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0788)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

#ifndef osdSyncScheduleTableAllowsDisabledInterrupts
   if (osIntAPIStatus != 0)
   {
      COVER(0x0789)
      #if osdErrorHook 
        COVER(0x078A)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSyncScheduleTable_ScheduleID(SchedTableID)
        osSaveSyncScheduleTable_GlobalTime(GlobalTime)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSYIntAPIDisabled);
      #if osdErrorHook 
        COVER(0x078B)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SY_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x078C)
#endif /* osdSyncScheduleTableAllowsDisabledInterrupts */
   
   if (oskSTSyncKind[SchedTableID]!=osdSTSyncExplicit)
   {
      COVER(0x078D)
      #if osdErrorHook
         COVER(0x078E)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSyncScheduleTable_ScheduleID(SchedTableID)
         osSaveSyncScheduleTable_GlobalTime(GlobalTime)
     #endif
     osAPIError(E_OS_ID, osdErrSYSyncKindNotExplicit);
     #if osdErrorHook
        COVER(0x078F)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
     #endif
     OS_SY_EXIT()
     return E_OS_ID;   
   }
   ELSEWITHCOVER(0x0790)

   if (GlobalTime>oskSTMaxAllowed[SchedTableID])
   {
      COVER(0x0791)
      #if osdErrorHook 
         COVER(0x0792)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSyncScheduleTable_ScheduleID(SchedTableID)
         osSaveSyncScheduleTable_GlobalTime(GlobalTime)
      #endif
      osAPIError(E_OS_VALUE, osdErrSYGlobalTimeTooBig);
      #if osdErrorHook 
         COVER(0x0793)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SY_EXIT()
      return E_OS_VALUE;
   }
   ELSEWITHCOVER(0x0794)
   
#endif /*STATUS_LEVEL*/


#ifndef osdSyncScheduleTableAllowsDisabledInterrupts
   COVER(0x0795)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
#else
   COVER(0x0796)
   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
#endif
   OS_SY_START_CRITICAL()

#if STATUS_LEVEL == EXTENDED_STATUS
   if (osSTState[SchedTableID] < SCHEDULETABLE_WAITING)
   {
      COVER(0x0797)
      #if osdErrorHook 
         COVER(0x0798)
         osSaveSyncScheduleTable_ScheduleID(SchedTableID)
         osSaveSyncScheduleTable_GlobalTime(GlobalTime)
      #endif
      osAPIError(E_OS_STATE, osdErrSYSTNotRunning);
      COVER(0x0799)
      #ifndef osdSyncScheduleTableAllowsDisabledInterrupts
         COVER(0x079A)
         /* KB begin osekHWosRestoreEnableLevel (overwritten) */
         /* restore level saved by osSaveDisableLevel */
         osRestoreEnableLevel();
         /* KB end osekHWosRestoreEnableLevel */
      #else
         COVER(0x079B)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SY_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x079C)
#endif /*STATUS_LEVEL*/

   /*Is the schedule table running?*/
   if ((osSTState[SchedTableID] == SCHEDULETABLE_RUNNING) || 
       (osSTState[SchedTableID] == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS))
   {
         COVER(0x079D)
          
         osSTFlags[SchedTableID]&=(osHRSTFlagsType)~osdHRST_ForceAsync;
          
         /*Yes: compute time difference between global and local time*/
         /*Difference between global time and local time is in the range (-maxallowedvalue ... +maxallowedvalue)*/
         /*This difference might not fit into TickType, so first check the sign.*/
         if(GlobalTime > osSTTime[SchedTableID])
         {
            /*Result will be positive*/
            osHeapDiffCaster.unsignedNumber = GlobalTime - osSTTime[SchedTableID];
            /*Result > maxallowedvalue/2?*/
            if( osHeapDiffCaster.unsignedNumber > (oskSTMaxAllowed[SchedTableID]/2))
            {
               COVER(0x079E)
                /*Yes: Correction into the opposite direction is faster*/
               osHeapDiffCaster.unsignedNumber -= oskSTMaxAllowed[SchedTableID]+1;
            }
            ELSEWITHCOVER(0x079F)
         }
         else
         {
            /*Result will be negative, consider the absolute value*/
            osHeapDiffCaster.unsignedNumber = osSTTime[SchedTableID] - GlobalTime;
            /*Abs(Result) > maxallowedvalue/2 + maxallowedvalue%2 ?*/
            if( (osHeapDiffCaster.unsignedNumber) > 
                ((oskSTMaxAllowed[SchedTableID]/2) + (oskSTMaxAllowed[SchedTableID]%2))
              )
            {
               COVER(0x07A0)
                /*Yes: Correction into the opposite direction is faster*/
                osHeapDiffCaster.unsignedNumber -= oskSTMaxAllowed[SchedTableID]+1;
            }
            ELSEWITHCOVER(0x07A1)
            /*now flip the sign*/
            osHeapDiffCaster.unsignedNumber = Zero - osHeapDiffCaster.unsignedNumber;
         }
    
         COVER(0x07A2)
         /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
         osSaveDisableGlobalNested();
         /* KB end osekHWosSaveDisableGlobalNested */
         #endif    
         /* KB end osekHWosDisableTimProtInterrupt */
    
         /* Interpret the difference as a signed number now */
         osSTDiffTime[SchedTableID] = osHeapDiffCaster.signedNumber;
          
         /* Now check, whether the schedule table is sychronous and update the state accordingly*/
         osSTCalculateSyncState(SchedTableID);

   }
   /*Is the schedule table waiting?*/
   else if (osSTState[SchedTableID] == SCHEDULETABLE_WAITING)
   {
      COVER(0x07A3)
      /*Yes: synchronize and start it */

      /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      #endif    
      /* KB end osekHWosDisableTimProtInterrupt */
         COVER(0x07A4)
         osSTFlags[SchedTableID]&=(osHRSTFlagsType)~osdHRST_ForceAsync;
         osSTTime[SchedTableID] = GlobalTime;
 
         /* maximum value for osSTCounter: oskSTMaxAllowed + 1, so overflow to 0 possible */
         /* osWorkScheduleTables correctly considers this overflow */
         osSTCounter[SchedTableID] = (1 + oskSTMaxAllowed[SchedTableID]) - GlobalTime;
 
         osSTState[SchedTableID]= SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
         osSTCurrentIndex[SchedTableID]= oskSTStartIndex[SchedTableID];
        
   } /*   else if (osSTState[SchedTableID] == SCHEDULETABLE_WAITING) ... else ...*/ 
   else
   { 
      /* This else is only here for MISRA, Rule 60. Intenionally left blank. */
      COVER(0x07A5)
      ;
   }
    /* else: in standard status, simply ignore the call. */

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SY_END_CRITICAL()
   OS_SY_EXIT()
 #ifndef osdSyncScheduleTableAllowsDisabledInterrupts
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
 #else
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
 #endif
   
   return E_OK;
}
#endif /* #ifndef osdSyncScheduleTableNotUsed */


#ifndef osdSetScheduleTableAsyncNotUsed
/*-+--------------------------------------------------------------------------
 / SetScheduleTableAsync
 / =====================
 /
 / Syntax: 
 /   StatusType SetScheduleTableAsync(ScheduleTableType ScheduleID)
 / Parameter (In):
 /   ScheduleTableType ScheduleID,
 / Parameter (Out):
 /   none
 / Description:
 /   The schedule table, defined by ScheduleID, is set asynchronous. If the
 /   schedule table is running, it keeps running.
 / Return value (depends on OIL-Attribute STATUS):
 /    STANDARD:
 /       - No error, E_OK
 /    EXTENDED: 
 /       - <ScheduleID> is not valid, E_OS_ID
 /       - The service has been called with interrupts disabled, 
 /         E_OS_DISABLEDINT
 / Scalability Classes:
 /    SC2, SC4
 /--------------------------------------------------------------------------*/
osqFunc1 StatusType osqFunc2 osSetScheduleTableAsync(ScheduleTableType ScheduleID)
{
   /* CovComment 4:  Function: osSetScheduleTableAsync */

   OS_AY_ENTRY()
   COVER(0x07A6)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x07A7)
#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_SetScheduleTableAsync & osCurrentContext) == 0)
   {
      COVER(0x07A8)
      #if osdErrorHook 
        COVER(0x07A9)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSetScheduleTableAsync_ScheduleID(ScheduleID)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrAYCallContext);
      #if osdErrorHook 
         COVER(0x07AA)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_AY_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x07AB)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (ScheduleID >= osrNumberOfScheduleTables)
   {
      COVER(0x07AC)
      #if osdErrorHook
        COVER(0x07AD)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSetScheduleTableAsync_ScheduleID(ScheduleID)
      #endif
      osAPIError(E_OS_ID, osdErrAYWrongID);
      #if osdErrorHook
        COVER(0x07AE)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_AY_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x07AF)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplScheduleTableAccess[ScheduleID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x07B0)
      #if osdErrorHook 
        COVER(0x07B1)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSetScheduleTableAsync_ScheduleID(ScheduleID)
      #endif
      osAPIError(E_OS_ACCESS, osdErrAYNoAccess);
      #if osdErrorHook 
         COVER(0x07B2)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_AY_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x07B3)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x07B4)
      #if osdErrorHook 
        COVER(0x07B5)
        /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
        /* = API function SuspendOSInterrupts */
        osSaveDisableLevelNested();
        /* KB end osekHWosSaveDisableLevelNested */
        osSaveSetScheduleTableAsync_ScheduleID(ScheduleID)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrAYIntAPIDisabled);
      #if osdErrorHook 
        COVER(0x07B6)
        /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
        /* = API function ResumeOSInterrupts */
        osRestoreEnableLevelNested();
        /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_AY_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x07B7)
#endif /*STATUS_LEVEL*/

   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_AY_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */


   if((osSTState[ScheduleID] == SCHEDULETABLE_RUNNING) || 
      (osSTState[ScheduleID] == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS))
   {
      /*set asynchronous*/
      osSTState[ScheduleID] = SCHEDULETABLE_RUNNING;
      
      COVER(0x07B8)
      osSTDiffTime[ScheduleID]=0;
      /* prevent from further synchronization*/
      osSTFlags[ScheduleID] |= osdHRST_ForceAsync;
   }
   ELSEWITHCOVER(0x07B9)
   /*else: simply ignore the call in all other states*/

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_AY_END_CRITICAL()
   OS_AY_EXIT()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   return E_OK;
}
#endif /* #ifndef osdSetScheduleTableAsyncNotUsed */

#endif /* #if (osdScheduleTableSyncUsed != 0)*/

#endif /*(osdNumberOfScheduleTables > 0)*/


/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE atostime.c */
