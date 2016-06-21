/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekalrm.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.47
|  Description: osek API-functions for handling alarms and counters
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/


/* next define is set in the makefile
 * switches off the include of (generated) tcb.h, includes instead
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

/* CovComment 0:  file: osekalrm.c */

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

#if (osdNumberOfAlarms > 0)

#ifndef osdGetAlarmBaseNotUsed
/*-+--------------------------------------------------------------------------
 / GetAlarmBase
 / ============
 /
 / Syntax:          StatusType GetAlarmBase ( AlarmType <AlarmID>,
 /                                            AlarmBaseRefType <Info> )
 / Parameter (In):  AlarmID Reference to alarm
 / Parameter (Out): Info Reference to structure with constants of the alarm 
 /                  base.
 / Description:     The system service GetAlarmBase reads the alarm base
 /                  characteristics. The return value <Info> is a structure 
 /                  in which the information of data type AlarmBaseType is 
 /                  stored.
 / Particularities: Allowed on task level, ISR, and in several hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook, PreTaskHook, PostTaskHook,
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/GetAlarmBase
 */
osqFunc1 StatusType osqFunc2 osGetAlarmBase(AlarmType AlarmId, AlarmBaseRefType Info)
{
   /* CovComment 4:  Function: osGetAlarmBase */
   OS_GB_ENTRY()
   COVER(0x01B4)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x01B5)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_GetAlarmBase & osCurrentContext) == 0)
   {
      COVER(0x01B6)
      #if osdErrorHook 
      COVER(0x01B7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarmBase_AlarmID(AlarmId)
      osSaveGetAlarmBase_Info(Info)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrGBCallContext);
      #if osdErrorHook 
      COVER(0x01B8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GB_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x01B9)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (AlarmId >= osrNumberOfAlarms)
   {
      COVER(0x01BA)
      #if osdErrorHook 
      COVER(0x01BB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarmBase_AlarmID(AlarmId)
      osSaveGetAlarmBase_Info(Info)
      #endif
      osAPIError(E_OS_ID, osdErrGBWrongAlarmID);
      #if osdErrorHook 
      COVER(0x01BC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GB_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x01BD)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplAlarmAccess[AlarmId] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x01BE)
      #if osdErrorHook 
      COVER(0x01BF)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarmBase_AlarmID(AlarmId)
      osSaveGetAlarmBase_Info(Info)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGBNoAccess);
      #if osdErrorHook 
      COVER(0x01C0)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GB_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x01C1)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)Info, (osuint32)sizeof(AlarmBaseType)) == osdFALSE)
      {  /* no writable area found -> no write acces rights for result */
          COVER(0x01C2)
          #if osdErrorHook 
          COVER(0x01C3)
          /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
          /* = API function SuspendOSInterrupts */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */
          osSaveGetAlarmBase_AlarmID(AlarmId)
          osSaveGetAlarmBase_Info(Info)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrGBIllegalAddr);
          #if osdErrorHook 
          COVER(0x01C4)
          /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
          /* = API function ResumeOSInterrupts */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          OS_GB_EXIT()
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x01C5)
   }
   ELSEWITHCOVER(0x01C6)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x01C7)
      #if osdErrorHook 
      COVER(0x01C8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarmBase_AlarmID(AlarmId)
      osSaveGetAlarmBase_Info(Info)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrGBIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x01C9)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GB_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x01CA)

#else
   /* KB begin dummyAssignAlarmId (default) */
   osdDummyRead(AlarmId)
   /* KB end dummyAssignAlarmId */
   COVER(0x01CB)
#endif

   /* copy structure */
#if osdUseCounterAPI
   COVER(0x01CC)
   Info->maxallowedvalue= oskCounterMaxAllowedValue[oskAlarmCounterRef[AlarmId]];
   Info->ticksperbase   = oskCounterTicksPerBase[oskAlarmCounterRef[AlarmId]];
   Info->mincycle       = oskCounterMinCycle[oskAlarmCounterRef[AlarmId]];
#else
   COVER(0x01CD)
#if osdLib
   COVER(0x01CE)
   Info->maxallowedvalue= oskMaxAllowedValue;
   Info->ticksperbase   = oskTicksPerBase;
   Info->mincycle       = oskMinCycle;
#else
   COVER(0x01CF)
   Info->maxallowedvalue= (TickType) OSMAXALLOWEDVALUE;
   Info->ticksperbase   = OSTICKSPERBASE;
   Info->mincycle       = OSMINCYCLE;
#endif
#endif

   OS_GB_EXIT()
   return E_OK;
} /* END OF osGetAlarmBase */
#endif


#ifndef osdGetAlarmNotUsed
/*-+--------------------------------------------------------------------------
 / GetAlarm
 / ========
 /
 / Syntax:          StatusType GetAlarm ( AlarmType <AlarmID>, 
 /                                        TickRefType <Tick>)
 / Parameter (In):  AlarmID Reference to an alarm
 / Parameter (Out): Tick Relative value in ticks before the alarm <AlarmID> 
 /                  expires.
 / Description:     The system service GetAlarm returns the relative value in 
 /                  ticks before the alarm <AlarmID> expires.
 / Particularities: It is up to the application to decide whether for example 
 /                  a CancelAlarm may still be useful.
 /                  If <AlarmID> is not in use, <Tick> is not defined.
 /                  Allowed on task level, ISR, and in several hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> is not used, E_OS_NOFUNC
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook, PreTaskHook, PostTaskHook,
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/GetAlarm
 */
osqFunc1 StatusType osqFunc2 osGetAlarm(AlarmType alarmID, TickRefType diffTickTime)
{
   /* CovComment 4:  Function: osGetAlarm */

   OS_GA_ENTRY()
   COVER(0x01D0)

#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x01D1)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_GetAlarm & osCurrentContext) == 0)
   {
      COVER(0x01D2)
      #if osdErrorHook 
      COVER(0x01D3)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarm_AlarmID(alarmID)
      osSaveGetAlarm_Tick(diffTickTime)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrGACallContext);
      #if osdErrorHook 
      COVER(0x01D4)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GA_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x01D5)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      COVER(0x01D6)
      #if osdErrorHook 
      COVER(0x01D7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarm_AlarmID(alarmID)
      osSaveGetAlarm_Tick(diffTickTime)
      #endif
      osAPIError(E_OS_ID, osdErrGAWrongAlarmID);
      #if osdErrorHook 
      COVER(0x01D8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GA_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x01D9)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplAlarmAccess[alarmID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x01DA)
      #if osdErrorHook 
      COVER(0x01DB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarm_AlarmID(alarmID)
      osSaveGetAlarm_Tick(diffTickTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGANoAccess);
      #if osdErrorHook 
      COVER(0x01DC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GA_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x01DD)
   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)diffTickTime, (osuint32)sizeof(TickType)) == osdFALSE)
      {  /* no writable area found -> no write acces rights for result */
          COVER(0x01DE)
          #if osdErrorHook 
          COVER(0x01DF)
          /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
          /* = API function SuspendOSInterrupts */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */
          osSaveGetAlarm_AlarmID(alarmID)
          osSaveGetAlarm_Tick(diffTickTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrGAIllegalAddr);
          #if osdErrorHook 
          COVER(0x01E0)
          /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
          /* = API function ResumeOSInterrupts */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          OS_GA_EXIT()
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x01E1)
   }
   ELSEWITHCOVER(0x01E2)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x01E3)
      #if osdErrorHook 
      COVER(0x01E4)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetAlarm_AlarmID(alarmID)
      osSaveGetAlarm_Tick(diffTickTime)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrGAIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x01E5)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GA_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x01E6)

#endif

   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_GA_START_CRITICAL()
   /* if timer is inactive, return at once */
#if (osdUseHeapAlarm != 0)
   if (osAlarmActivated[alarmID] == 0)
   {
      COVER(0x01E7)
#else
   if (osAlarmTime[alarmID] == 0)
   {
      COVER(0x01E8)
#endif
      osSaveGetAlarm_AlarmID(alarmID)
      osSaveGetAlarm_Tick(diffTickTime)
      osAPIError(E_OS_NOFUNC, osdErrGANotActive);
      OS_GA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      OS_GA_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x01E9)

#if (osdUseHeapAlarm != 0) && (osdAlarmsOnSystemTimer)
   COVER(0x01EA)
#if (osdUseCounterAPI != 0)
   if ( oskAlarmCounterRef[alarmID] > 0 )
   {
      /*handled via CounterAPI => generated alarm*/
      *diffTickTime = osAlarmTime[alarmID];
      COVER(0x01EB)
   }
   else
   /*this alarm is handled via SystemCounter => heap-alarm*/
#else /*!osdUseCounterAPI*/
   /*all alarms are handled via SystemCounter => heap-alarm*/
#endif /*osdUseCounterAPI*/
   {
      *diffTickTime = osAlarmTime[alarmID] - osGetSystemCounter();
#if (osdUseHighResolutionTimer != 0)
      /*handle latency of timer interrupt of more than TickTime*/
      if ( (0==*diffTickTime) || (*diffTickTime > ((TickType)OSMAXALLOWEDVALUE)) ) 
      {
         /*alarm time already passed, alarm will be handled with next timer interrupt*/
         *diffTickTime = 1;
         COVER(0x01EC)
      }
      ELSEWITHCOVER(0x01ED)
#endif /*osdUseHighResolutionTimer*/
   }
#else /*!osdUseHeapAlarm*/
   /*all alarms are handled as generated alarm*/
   *diffTickTime = osAlarmTime[alarmID];
   COVER(0x01EE)
#endif /*osdUseHeapAlarm*/

   OS_GA_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   OS_GA_EXIT()
   return E_OK;
} /* END OF osGetAlarm */
#endif


#ifndef osdSetRelAlarmNotUsed
/*-+--------------------------------------------------------------------------
 / SetRelAlarm
 / ===========
 /
 / Syntax:          StatusType SetRelAlarm ( AlarmType <AlarmID>,
 /                                           TickType <increment>,
 /                                           TickType <cycle> )
 / Parameter (In):  -AlarmID Reference to the alarm element
 /                  -increment Relative value in ticks
 /                  -cycle Cycle value in case of cyclic alarm. In case of 
 /                   single alarms, cycle has to be zero.
 / Parameter (Out): none
 / Description:     The system service occupies the alarm <AlarmID> element.
 /                  After <increment> ticks have elapsed, the task assigned 
 /                  to the alarm <AlarmID> is activated or the assigned event
 /                  (only for extended tasks) is set or the alarm-callback 
 /                  routine is called.
 / Particularities: The value of <increment> must be unequal to 0.
 /                  If the relative value <increment> is very small, 
 /                  the alarm may expire, and the task may become ready or 
 /                  the alarm-callback may be called before the system 
 /                  service returns to the user.
 /                  If <cycle> is unequal zero, the alarm element is logged 
 /                  on again immediately after expiry with the relative 
 /                  value <cycle>.
 /                  The alarm <AlarmID> must not already be in use.
 /                  To change values of alarms already in use the alarm has 
 /                  to be cancelled first.
 /                  If the alarm is already in use, this call is
 /                  ignored and the error E_OS_STATE is returned.
 /                  Allowed on task level and in ISR, but not in hook 
 /                  routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> is already in use, E_OS_STATE
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Value of <increment> outside of the admissible 
 /                    limits (lower or equal zero or bigger than 
 /                    maxallowedvalue), E_OS_VALUE
 /                  - Value of <cycle> unequal to 0 and outside of the 
 /                    admissible counter limits (less than mincycle 
 /                    or bigger than maxallowedvalue), E_OS_VALUE
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:       BCC1, BCC2, ECC1, ECC2, Events only ECC1, ECC2
 /  -General:       Task, Cat2-ISR, interrupts enabled
 /  -SC1/SC2 only:  StartupHook (compatibility to older OSEK-OS)
 /--------------------------------------------------------------------------*/

/* CovComment 4:  Function: osSetRelAlarm */
/* KB begin osekHWosSetRelAlarmHead (default) */
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/SetRelAlarm
 */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID, TickType deltaTicks, TickType cycle)
{
   COVER(0x01EF)
#else
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID)
{
   COVER(0x01F0)
#endif
/* KB end osekHWosSetRelAlarmHead */
 {/* extra curly brace to allow coverage code above and variable declarations inside OS_SA_ENTRY below */
   OS_SA_ENTRY()
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x01F1)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_SetRelAlarm & osCurrentContext) == 0)
   {
      COVER(0x01F2)
      #if osdErrorHook 
      COVER(0x01F3)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetRelAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x01F4)
      osSaveSetRelAlarm_increment(deltaTicks)
      osSaveSetRelAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSACallContext);
      #if osdErrorHook 
      COVER(0x01F5)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SA_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x01F6)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      COVER(0x01F7)
      #if osdErrorHook 
      COVER(0x01F8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetRelAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x01F9)
      osSaveSetRelAlarm_increment(deltaTicks)
      osSaveSetRelAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_ID, osdErrSAWrongAlarmID); 
      #if osdErrorHook 
      COVER(0x01FA)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SA_EXIT()
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x01FB)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplAlarmAccess[alarmID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x01FC)
      #if osdErrorHook 
      COVER(0x01FD)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetRelAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x01FE)
      osSaveSetRelAlarm_increment(deltaTicks)
      osSaveSetRelAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_ACCESS, osdErrSANoAccess);
      #if osdErrorHook 
      COVER(0x01FF)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SA_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0200)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/
   
#endif

#if (osdNumberOfVarAlarms > 0)
   COVER(0x0201)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x0202)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x0203)
#endif
      /* delta ticks must not be zero according to AutosarOS */
      if (deltaTicks == 0)
      {
         /* fullfills [SR:005] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
         COVER(0x0204)
         #if osdErrorHook
         COVER(0x0205)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSetRelAlarm_AlarmID(alarmID)
          #if (osdNumberOfVarAlarms > 0)
          COVER(0x0206)
          osSaveSetRelAlarm_increment(deltaTicks)
          osSaveSetRelAlarm_cycle(cycle)
          #endif
         #endif
         osAPIError(E_OS_VALUE, osdErrSAZeroIncrement);
         #if osdErrorHook
         COVER(0x0207)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SA_EXIT()
         return  E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0208)
#if (STATUS_LEVEL == EXTENDED_STATUS)
      COVER(0x0209)
      /* check timer limits */
      #if osdUseCounterAPI
      if (cycle != 0)
      {
         if ((cycle > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]])
             || (cycle < oskCounterMinCycle[oskAlarmCounterRef[alarmID]]))
         {
            COVER(0x020A)
            #if osdErrorHook 
            COVER(0x020B)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveSetRelAlarm_AlarmID(alarmID)
            #if (osdNumberOfVarAlarms > 0)
            COVER(0x020C)
            osSaveSetRelAlarm_increment(deltaTicks)
            osSaveSetRelAlarm_cycle(cycle)
            #endif
            #endif
            osAPIError(E_OS_VALUE, osdErrSAWrongCycle);  
            #if osdErrorHook 
            COVER(0x020D)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            #endif
            OS_SA_EXIT()
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x020E)
      }
      ELSEWITHCOVER(0x020F)
      if (deltaTicks > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]]) 
      {
         COVER(0x0210)
         #if osdErrorHook 
         COVER(0x0211)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSetRelAlarm_AlarmID(alarmID)
         #if (osdNumberOfVarAlarms > 0)
         COVER(0x0212)
         osSaveSetRelAlarm_increment(deltaTicks)
         osSaveSetRelAlarm_cycle(cycle)
         #endif
         #endif
         osAPIError(E_OS_VALUE, osdErrSAWrongDelta);
         #if osdErrorHook 
         COVER(0x0213)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SA_EXIT()
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0214)
      #else /* osdUseCounterAPI */
      COVER(0x0215)
      #if ((OSMAXALLOWEDVALUE < TICKTYPE_MAX) || (OSMINCYCLE > 1))
      if (cycle != 0)
      {
         if (
           #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) 
              cycle > oskAlarmDefinitions.maxallowedvalue
           #else
              0
           #endif
           #if ((OSMINCYCLE > 1) || (TICKTYPE_MIN !=0))
              || (cycle < oskAlarmDefinitions.mincycle)
           #endif
         )
         {
            COVER(0x0216)
            #if osdErrorHook 
            COVER(0x0217)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveSetRelAlarm_AlarmID(alarmID)
            #if (osdNumberOfVarAlarms > 0)
            COVER(0x0218)
            osSaveSetRelAlarm_increment(deltaTicks)
            osSaveSetRelAlarm_cycle(cycle)
            #endif
            #endif
            osAPIError(E_OS_VALUE, osdErrSAWrongCycle);  
            #if osdErrorHook 
            COVER(0x0219)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            #endif
            OS_SA_EXIT()
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x021A)
      }
      ELSEWITHCOVER(0x021B)
      #endif
      #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) 
      if (deltaTicks > oskAlarmDefinitions.maxallowedvalue)
      {
         COVER(0x021C)
         #if osdErrorHook 
         COVER(0x021D)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSetRelAlarm_AlarmID(alarmID)
         #if (osdNumberOfVarAlarms > 0)
         COVER(0x021E)
         osSaveSetRelAlarm_increment(deltaTicks)
         osSaveSetRelAlarm_cycle(cycle)
         #endif
         #endif
         osAPIError(E_OS_VALUE, osdErrSAWrongDelta);
         #if osdErrorHook 
         COVER(0x021F)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SA_EXIT()
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0220)
      #endif
      #endif /* osdUseCounterAPI */
#endif

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
      COVER(0x0221)
   }
   else /*(alarmID < osrNumberOfVarAlarms)*/
   {
      COVER(0x0222)
#endif
#endif /* (osdNumberOfVarAlarms > 0) */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)

      /* check for increment 0 for static alarm */
      /* cannot be checked in the generator as it must be allowed for SetAbsAlarm */
      if (oskStaticAlarmTime[alarmID] == 0)
      {
         /* fullfills [SR:005] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
         COVER(0x0223)
         #if osdErrorHook
         COVER(0x0224)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSetRelAlarm_AlarmID(alarmID)
         #if (osdNumberOfVarAlarms > 0)
         COVER(0x0225)
         osSaveSetRelAlarm_increment(deltaTicks)
         osSaveSetRelAlarm_cycle(cycle)
         #endif
         #endif
         osAPIError(E_OS_VALUE, osdErrSAZeroIncrement);
         #if osdErrorHook
         COVER(0x0226)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SA_EXIT()
         return  E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0227)
#if (osdNumberOfVarAlarms > 0)
      COVER(0x0228)
   }
#endif /* (osdNumberOfVarAlarms > 0) */
#endif /*(osdNumberOfVarAlarms < osdNumberOfAlarms)*/



#if (STATUS_LEVEL == EXTENDED_STATUS)
   if (osIntAPIStatus != 0)
   {
      COVER(0x0229)
      #if osdErrorHook 
      COVER(0x022A)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetRelAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x022B)
      osSaveSetRelAlarm_increment(deltaTicks)
      osSaveSetRelAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSAIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x022C)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SA_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x022D)
   
#endif

   COVER(0x022E)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SA_START_CRITICAL()

   /* timer running? */
#if (osdUseHeapAlarm != 0)
   if (osAlarmActivated[alarmID] != 0)
   {
      COVER(0x022F)
#else
   if (osAlarmTime[alarmID] != 0)
   {
      COVER(0x0230)
#endif
      osSaveSetRelAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x0231)
      osSaveSetRelAlarm_increment(deltaTicks)
      osSaveSetRelAlarm_cycle(cycle)
      #endif
      osAPIError(E_OS_STATE, osdErrSAAlreadyActive);
      OS_SA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SA_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x0232)

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   /*start alarm*/
#if (osdNumberOfVarAlarms > 0)
   COVER(0x0233)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x0234)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x0235)
#endif
      /*alarm time is variable*/
      osAlarmCycleTime[alarmID] = cycle;
#if (osdUseHeapAlarm != 0)
#if (osdUseCounterAPI != 0)
#if (osdNumberOfVarHeapAlarms > 0)
#if (osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)
      COVER(0x0236)
      if ( alarmID < osrNumberOfVarHeapAlarms )
#endif /*(osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)*/
      {
         /*alarm handled via SystemTimer as a heap-alarm*/
         osAlarmTime[alarmID] = deltaTicks + osGetSystemCounter();
         osAlarmActivated[alarmID] = 1;
         osInsertMinHeapWithCheck(alarmID);
         COVER(0x0237)
      }
#if (osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)
      else
#endif /*(osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)*/
#endif /*(osdNumberOfVarHeapAlarms > 0)*/
#if (osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)
      {
         /*alarm is handled via CounterAPI as generated alarm (no heap)*/
         osAlarmActivated[alarmID] = 1;
         osAlarmTime[alarmID] = deltaTicks;
         COVER(0x0238)
      }
      COVER(0x0239)
#endif /*(osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)*/
#else  /*!osdUseCounterAPI*/
      /*all alarms are handled via SystemTimer as heap-alarms*/
      osAlarmTime[alarmID] = deltaTicks + osGetSystemCounter();
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
      COVER(0x023A)
#endif /*!osdUseCounterAPI*/
#else /*!osdUseHeapAlarm*/
      /*all alarms are generated alarms (no heap)*/
      osAlarmTime[alarmID] = deltaTicks;
      COVER(0x023B)
#endif /*osdUseHeapAlarm*/
      COVER(0x023C)
#endif /* (osdNumberOfVarAlarms > 0) */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
#if (osdNumberOfVarAlarms > 0)
      COVER(0x023D)
   }
   else /*(alarmID < osrNumberOfVarAlarms)*/
#endif
   {
      /*alarm time is statically defined in the OIL-file*/
#if (osdUseHeapAlarm != 0)
#if (osdUseCounterAPI != 0)
#if (osdNumberOfStatHeapAlarms > 0)
#if (osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))
      COVER(0x023E)
      if ( alarmID < (osrNumberOfStatHeapAlarms+osrNumberOfVarAlarms) )
#endif /*(osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))*/
      {
         /*alarm is handled via SystemTimer as a heap-alarm*/
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] + osGetSystemCounter();
         osAlarmActivated[alarmID] = 1;
         osInsertMinHeapWithCheck(alarmID);
         COVER(0x023F)
      }
#if (osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))
      else
#endif /*(osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))*/
#endif /*(osdNumberOfStatHeapAlarms > 0)*/
#if (osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))
      {
         /*alarm is handled via CounterAPI as generated alarm (no heap)*/
         osAlarmActivated[alarmID] = 1;
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID];
         COVER(0x0240)
      }
#endif /*(osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))*/
      COVER(0x0241)
#else /*osdUseCounterAPI*/
      /*all alarms are handled via SystemTimer as heap-alarms*/
      osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] + osGetSystemCounter();
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
      COVER(0x0242)
#endif /*osdUseCounterAPI*/
#else /*osdUseHeapAlarm*/
      /*all alarms are generated alarms (no heap)*/
      osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID];
      COVER(0x0243)
#endif /*osdUseHeapAlarm*/
      COVER(0x0244)
   }
   COVER(0x0245)
#endif


   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SA_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
   OS_SA_EXIT()
   COVER(0x0246)
   return E_OK;
 }/* extra curly brace (see start of osSetRelAlarm) */
} /* END OF osSetRelAlarm */
#endif


#ifndef osdSetAbsAlarmNotUsed

#define osdSetAlarmTimeCorrected( dest, src )                          \
{                                                                      \
   TickType actualTime = osGetSystemCounter();                         \
   if ((src) > (actualTime & ~(osdSystemCounterMask)))                 \
   {                                                                   \
      (dest) = (src) | ( actualTime & (TickType)osdSystemCounterMask); \
      COVER(0x0247)                                                          \
   }                                                                   \
   else                                                                \
   {                                                                   \
      (dest) = (src) | ((actualTime-(TickType)osdSystemCounterMask) & (TickType)osdSystemCounterMask);\
      COVER(0x0248)                                                          \
   }                                                                   \
}

/*-+--------------------------------------------------------------------------
 / SetAbsAlarm
 / ===========
 /
 / Syntax:          StatusType SetAbsAlarm ( AlarmType <AlarmID>, 
 /                                           TickType <start>,
 /                                           TickType <cycle> )
 / Parameter (In):  -AlarmID Reference to the alarm element
 /                  -start Absolute value in ticks
 /                  -cycle Cycle value in case of cyclic alarm. In case 
 /                   of single alarms, cycle has to be zero.
 / Parameter (Out): none
 / Description:     The system service occupies the alarm <AlarmID> element.
 /                  When <start> ticks are reached, the task assigned to 
 /                  the alarm <AlarmID> is activated or the assigned event
 /                  (only for extended tasks) is set or the alarm-callback 
 /                  routine is called.
 / Particularities: If the absolute value <start> is very close to the 
 /                  current counter value, the alarm may expire, and the 
 /                  task may become ready or the alarm-callback may be 
 /                  called before the system service returns to the user.
 /                  If the absolute value <start> was already reached 
 /                  before the system call, the alarm will only expire 
 /                  when the absolute value <start> is reached again,
 /                  i.e. after the next overrun of the counter.
 /                  If <cycle> is unequal zero, the alarm element is 
 /                  logged on again immediately after expiry with the 
 /                  relative value <cycle>.
 /                  The alarm <AlarmID> must not be already in use.
 /                  To change values of alarms already in use the alarm 
 /                  has to be cancelled first.
 /                  If the alarm is already in use, this call is ignored 
 /                  and the error E_OS_STATE is returned.
 /                  Allowed on task level and in ISR, but not in hook 
 /                  routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> is already in use, E_OS_STATE
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Value of <start> outside of the admissible counter 
 /                    limit (less than zero or bigger than maxallowedvalue),
 /                    E_OS_VALUE
 /                  - Value of <cycle> unequal to 0 and outside of the 
 /                    admissible counter limits (less than mincycle or 
 /                    bigger than maxallowedvalue), E_OS_VALUE
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2, Events only ECC1, ECC2
 / Context:
 /  -General:       Task, Cat2-ISR, interrupts not disabled by the 
 /                  application
 /  -SC1/SC2 only:  StartupHook (compatibility to older OSEK-OS)
 /--------------------------------------------------------------------------*/

/* CovComment 4:  Function: osSetAbsAlarm */
/* KB begin osekHWosSetAbsAlarmHead (default) */
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/SetAbsAlarm
 */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID, TickType start, TickType cycle)
{
   COVER(0x0249)
#else
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID)
{
   COVER(0x024A)
#endif
/* KB end osekHWosSetAbsAlarmHead */
 {/* extra curly brace to allow coverage code above and variable declarations inside OS_SL_ENTRY below */
   OS_SL_ENTRY()
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x024B)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_SetAbsAlarm & osCurrentContext) == 0)
   {
      COVER(0x024C)
      #if osdErrorHook 
      COVER(0x024D)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetAbsAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x024E)
      osSaveSetAbsAlarm_start(start)
      osSaveSetAbsAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSLCallContext);
      #if osdErrorHook 
      COVER(0x024F)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SL_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0250)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      COVER(0x0251)
      #if osdErrorHook 
      COVER(0x0252)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetAbsAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x0253)
      osSaveSetAbsAlarm_start(start)
      osSaveSetAbsAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_ID, osdErrSLWrongAlarmID);
      #if osdErrorHook 
      COVER(0x0254)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SL_EXIT()
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x0255)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplAlarmAccess[alarmID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0256)
      #if osdErrorHook 
      COVER(0x0257)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetAbsAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x0258)
      osSaveSetAbsAlarm_start(start)
      osSaveSetAbsAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_ACCESS, osdErrSLNoAccess);
      #if osdErrorHook 
      COVER(0x0259)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SL_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x025A)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/
   
#endif

#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x025B)
#if (osdNumberOfVarAlarms > 0)
   COVER(0x025C)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x025D)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x025E)
#endif
      /* check timer limits */
      #if osdUseCounterAPI
      if (cycle != 0)
      {
         if ((cycle > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]])
             || (cycle < oskCounterMinCycle[oskAlarmCounterRef[alarmID]]))
         {
            COVER(0x025F)
            #if osdErrorHook 
            COVER(0x0260)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveSetAbsAlarm_AlarmID(alarmID)
            #if (osdNumberOfVarAlarms > 0)
            COVER(0x0261)
            osSaveSetAbsAlarm_start(start)
            osSaveSetAbsAlarm_cycle(cycle)
            #endif
            #endif
            osAPIError(E_OS_VALUE, osdErrSLWrongCycle);  
            #if osdErrorHook 
            COVER(0x0262)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            #endif
            OS_SA_EXIT()
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x0263)
      }
      ELSEWITHCOVER(0x0264)
      if (start > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]]) 
      {
         COVER(0x0265)
         #if osdErrorHook 
         COVER(0x0266)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSetAbsAlarm_AlarmID(alarmID)
         #if (osdNumberOfVarAlarms > 0)
         COVER(0x0267)
         osSaveSetAbsAlarm_start(start)
         osSaveSetAbsAlarm_cycle(cycle)
         #endif
         #endif
         osAPIError(E_OS_VALUE, osdErrSLWrongStart);
         #if osdErrorHook 
         COVER(0x0268)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SA_EXIT()
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0269)
      #else /* osdUseCounterAPI */
      COVER(0x026A)
      #if ((OSMAXALLOWEDVALUE < TICKTYPE_MAX) || (OSMINCYCLE > 1))
      COVER(0x026B)
      if (cycle != 0)
      {
         if (
           #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) 
              cycle > oskAlarmDefinitions.maxallowedvalue
           #else
              0
           #endif
           #if ((OSMINCYCLE > 1) || (TICKTYPE_MIN !=0))
              || (cycle < oskAlarmDefinitions.mincycle)
           #endif
            )
         {
            COVER(0x026C)
            #if osdErrorHook 
            COVER(0x026D)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveSetAbsAlarm_AlarmID(alarmID)
            #if (osdNumberOfVarAlarms > 0)
            COVER(0x026E)
            osSaveSetAbsAlarm_start(start)
            osSaveSetAbsAlarm_cycle(cycle)
            #endif
            #endif
            osAPIError(E_OS_VALUE, osdErrSLWrongCycle);  
            #if osdErrorHook 
            COVER(0x026F)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            #endif
            OS_SL_EXIT()
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x0270)
      }
      ELSEWITHCOVER(0x0271)
      #endif /* ((OSMAXALLOWEDVALUE < TICKTYPE_MAX) || (OSMINCYCLE > 1)) */
      #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX)  
      if (start > oskAlarmDefinitions.maxallowedvalue)
      {
         COVER(0x0272)
         #if osdErrorHook 
         COVER(0x0273)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveSetAbsAlarm_AlarmID(alarmID)
         #if (osdNumberOfVarAlarms > 0)
         COVER(0x0274)
         osSaveSetAbsAlarm_start(start)
         osSaveSetAbsAlarm_cycle(cycle)
         #endif
         #endif
         osAPIError(E_OS_VALUE, osdErrSLWrongStart);
         #if osdErrorHook 
         COVER(0x0275)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_SL_EXIT()
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0276)
      #endif
      #endif /* osdUseCounterAPI */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   }
#endif
#endif /* (osdNumberOfVarAlarms > 0) */

   if (osIntAPIStatus != 0)
   {
      COVER(0x0277)
      #if osdErrorHook 
      COVER(0x0278)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetAbsAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x0279)
      osSaveSetAbsAlarm_start(start)
      osSaveSetAbsAlarm_cycle(cycle)
      #endif
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSLIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x027A)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SL_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x027B)
#endif

   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SL_START_CRITICAL()

   /* timer running? */
#if (osdUseHeapAlarm != 0)
   if (osAlarmActivated[alarmID] != 0)
   {
      COVER(0x027C)
#else
   if (osAlarmTime[alarmID] != 0)
   {
      COVER(0x027D)
#endif
      osSaveSetAbsAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
      COVER(0x027E)
      osSaveSetAbsAlarm_start(start)
      osSaveSetAbsAlarm_cycle(cycle)
      #endif
      osAPIError(E_OS_STATE, osdErrSLAlreadyActive);
      OS_SL_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_SL_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x027F)

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */
   /* start alarm */
#if (osdNumberOfVarAlarms > 0)
   COVER(0x0280)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x0281)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x0282)
#endif
      /*alarm time is variable*/
      osAlarmCycleTime[alarmID] = cycle;
#if (osdUseHeapAlarm != 0)
#if (osdUseCounterAPI != 0)
#if (osdNumberOfVarHeapAlarms > 0)
#if (osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)
      COVER(0x0283)
      if ( alarmID < osrNumberOfVarHeapAlarms )
#endif /*(osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)*/
      {
         /*alarm handled via SystemTimer as a heap-alarm*/
         osdSetAlarmTimeCorrected( osAlarmTime[alarmID], start );
         osAlarmActivated[alarmID] = 1;
         osInsertMinHeapWithCheck(alarmID);
         COVER(0x0284)
      }
#if (osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)
      else
#endif /*(osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)*/
#endif /*(osdNumberOfVarHeapAlarms > 0)*/
#if (osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)
      {
         /*alarm is handled via CounterAPI as generated alarm (no heap)*/
         /*check, if start is after the next overflow*/
         if ( start > osCounter[oskAlarmCounterRef[alarmID]] )
         {
            /*start is before next overflow*/
            osAlarmTime[alarmID] = start - osCounter[oskAlarmCounterRef[alarmID]];
            COVER(0x0285)
         }
         else
         {
            /*an overflow is necessary to reach start time*/
            osAlarmTime[alarmID] = (TickType)(start + 1 +
                                   ( oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]] - 
                                     osCounter[oskAlarmCounterRef[alarmID]]
                                   ));
            /* Has an overflow to zero occured? */
            if (osAlarmTime[alarmID] == 0)
            {
               /* YES: Take the maximum value */
               osAlarmTime[alarmID] = oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]];
               COVER(0x0286)
            }
            ELSEWITHCOVER(0x0287)
         }
         osAlarmActivated[alarmID] = 1;
         COVER(0x0288)
      }
      COVER(0x0289)
#endif /*(osdNumberOfVarHeapAlarms < osdNumberOfVarAlarms)*/
#else /*!osdUseCounterAPI*/
      /*all alarms are handled via SystemTimer as heap-alarms*/
      osdSetAlarmTimeCorrected( osAlarmTime[alarmID], start )
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
      COVER(0x028A)
#endif /*!osdUseCounterAPI*/
#else /*!osdUseHeapAlarm*/
      /*all alarms are generated alarms (no heap)*/
   {
      TickType now;
      COVER(0x028B)
      #if osdUseCounterAPI
      COVER(0x028C)
      now = osCounter[oskAlarmCounterRef[alarmID]];
      #else
      COVER(0x028D)
      now = osGetSystemCounter();
      #endif

      /*check, if start is after the next overflow*/
      if( start > now )
      {
         COVER(0x028E)
         /*start is before next overflow*/
         osAlarmTime[alarmID] = start - now;
      }
      else
      {
         COVER(0x028F)
         /*an overflow is necessary to reach start time*/
         #if osdUseCounterAPI
         COVER(0x0290)
         osAlarmTime[alarmID] = (TickType) (start + 1 +
                                ( oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]] -
                                  now
                                ));
         /* Has an overflow to zero occured? */
         if (osAlarmTime[alarmID] == 0)
         {
            /* YES: Take the maximum value */
            osAlarmTime[alarmID] = oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]];
            COVER(0x0291)
         }
         ELSEWITHCOVER(0x0292)
         #else
         COVER(0x0293)
         osAlarmTime[alarmID] = (TickType) (start + 1 + (((TickType)OSMAXALLOWEDVALUE) - now));
         /* Has an overflow to zero occured? */
         if (osAlarmTime[alarmID] == 0)
         {
            /* YES: Take the maximum value */
            osAlarmTime[alarmID] = (TickType)OSMAXALLOWEDVALUE;
            COVER(0x0294)
         }
         ELSEWITHCOVER(0x0295)
         #endif
      }
   }
   #endif
#endif /* (osdNumberOfVarAlarms > 0) */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x0296)
#if (osdNumberOfVarAlarms > 0)
   COVER(0x0297)
   }
   else
#endif
   {
      /*alarm time is statically defined in the OIL-file*/
#if (osdUseHeapAlarm != 0)
#if (osdUseCounterAPI != 0)
#if (osdNumberOfStatHeapAlarms > 0)
#if (osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))
      COVER(0x0298)
      if ( alarmID < (osrNumberOfStatHeapAlarms+osrNumberOfVarAlarms) )
#endif /*(osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))*/
      {
         /*alarm is handled via SystemTimer as a heap-alarm*/
         osdSetAlarmTimeCorrected( osAlarmTime[alarmID], oskStaticAlarmTime[alarmID] )
         osAlarmActivated[alarmID] = 1;
         osInsertMinHeapWithCheck(alarmID);
         COVER(0x0299)
      }
#if (osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))
      else
#endif /*(osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))*/
#endif /*(osdNumberOfStatHeapAlarms > 0)*/
#if (osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))
      {
         /*alarm is handled via CounterAPI as generated alarm (no heap)*/
         /*check, if start is after the next overflow*/
         if ( oskStaticAlarmTime[alarmID] > osCounter[oskAlarmCounterRef[alarmID]] )
         {
            /*start is before next overflow*/
            osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] - 
                                   osCounter[oskAlarmCounterRef[alarmID]];
            COVER(0x029A)
         }
         else
         {
            /*an overflow is necessary to reach start time*/
            osAlarmTime[alarmID] = (TickType)(oskStaticAlarmTime[alarmID] + 1 +
                                   ( oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]] - 
                                     osCounter[oskAlarmCounterRef[alarmID]]
                                   ));
            /* Has an overflow to zero occured? */
            if (osAlarmTime[alarmID] == 0)
            {
               /* YES: Take the maximum value */
               osAlarmTime[alarmID] = oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]];
               COVER(0x029B)
            }
            ELSEWITHCOVER(0x029C)
         }
         osAlarmActivated[alarmID] = 1;
         COVER(0x029D)
      }
      COVER(0x029E)
#endif /*(osdNumberOfStatHeapAlarms < (osdNumberOfAlarms-osdNumberOfVarAlarms))*/
#else /*osdUseCounterAPI*/
      /*all alarms are handled via SystemTimer as heap-alarms*/
      osdSetAlarmTimeCorrected( osAlarmTime[alarmID], oskStaticAlarmTime[alarmID] )
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
      COVER(0x029F)
#endif /*osdUseCounterAPI*/
#else /*!osdUseHeapAlarm*/
      /*all alarms are generated alarms (no heap)*/
   {
      TickType now;
      COVER(0x02A0)

      #if osdUseCounterAPI
      COVER(0x02A1)
      now = osCounter[oskAlarmCounterRef[alarmID]];
      #else
      COVER(0x02A2)
      now = osGetSystemCounter();
      #endif

      /*check, if alarm time is after the next overflow*/
      if( oskStaticAlarmTime[alarmID] > now )
      {
         COVER(0x02A3)
         /*alarm time is before next overflow*/
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] - now;
      }
      else
      {
         COVER(0x02A4)
         /*an overflow is necessary to reach alarm time*/
         #if osdUseCounterAPI
         COVER(0x02A5)
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] + 1 + 
                                ( oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]] -
                                  now
                                );
         /* Has an overflow to zero occured? */
         if (osAlarmTime[alarmID] == 0)
         {
            /* YES: Take the maximum value */
            osAlarmTime[alarmID] = oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]];
            COVER(0x02A6)
         }
         ELSEWITHCOVER(0x02A7)
         #else
         COVER(0x02A8)
            osAlarmTime[alarmID] = (TickType) oskStaticAlarmTime[alarmID] + 1 + 
                                   (((TickType)OSMAXALLOWEDVALUE) - now );
         /* Has an overflow to zero occured? */
         if (osAlarmTime[alarmID] == 0)
         {
            /* YES: Take the maximum value */
            osAlarmTime[alarmID] = (TickType) OSMAXALLOWEDVALUE;
            COVER(0x02A9)
         }
         ELSEWITHCOVER(0x02AA)
         #endif
      }
   }
   #endif
   }
#endif

   COVER(0x02AB)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SL_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
   OS_SL_EXIT()

   return E_OK;
 }/* extra curly brace (see start of osSetAbsAlarm) */
} /* END OF SetAbsAlarm */
#endif /* osdSetAbsAlarmNotUsed */


#ifndef osdCancelAlarmNotUsed
/*-+--------------------------------------------------------------------------
 / CancelAlarm
 / ===========
 /
 / Syntax:          StatusType CancelAlarm ( AlarmType <AlarmID> )
 / Parameter (In):  AlarmID Reference to an alarm
 / Parameter (Out): none
 / Description:     The system service cancels the alarm <AlarmID>.
 / Particularities: Allowed on task level and in ISR, but not in hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> not in use, E_OS_NOFUNC
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/CancelAlarm
 */
osqFunc1 StatusType osqFunc2 osCancelAlarm(AlarmType alarmID)
{
   /* CovComment 4:  Function: osCancelAlarm */
   OS_CA_ENTRY()
   COVER(0x02AC)

#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x02AD)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_CancelAlarm & osCurrentContext) == 0)
   {
      COVER(0x02AE)
      #if osdErrorHook 
      COVER(0x02AF)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCancelAlarm_AlarmID(alarmID)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrCACallContext);
      #if osdErrorHook 
      COVER(0x02B0)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CA_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x02B1)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      COVER(0x02B2)
      #if osdErrorHook 
      COVER(0x02B3)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCancelAlarm_AlarmID(alarmID)
      #endif
      osAPIError(E_OS_ID, osdErrCAWrongAlarmID);
      #if osdErrorHook 
      COVER(0x02B4)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CA_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x02B5)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplAlarmAccess[alarmID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x02B6)
      #if osdErrorHook 
      COVER(0x02B7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCancelAlarm_AlarmID(alarmID)
      #endif
      osAPIError(E_OS_ACCESS, osdErrCANoAccess);
      #if osdErrorHook 
      COVER(0x02B8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CA_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x02B9)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x02BA)
      #if osdErrorHook 
      COVER(0x02BB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveCancelAlarm_AlarmID(alarmID)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrCAIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x02BC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CA_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x02BD)
#endif

   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_CA_START_CRITICAL()
#if (0==osdUseHeapAlarm)

   /*all alarms are handled as generated alarm*/
   if (osAlarmTime[alarmID] == 0)
   {
      /* timer is inactive => return at once */
      osSaveCancelAlarm_AlarmID(alarmID)
      osAPIError(E_OS_NOFUNC, osdErrCANotActive);
      OS_CA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_CA_EXIT()
      COVER(0x02BE)
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x02BF)

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */
   osAlarmTime[alarmID] = 0;
   COVER(0x02C0)

#else /*osdUseHeapAlarm*/
#if (osdAlarmsOnSystemTimer)
   if (osAlarmActivated[alarmID] == 0)
   {
      /* timer is inactive => return at once */
      osSaveCancelAlarm_AlarmID(alarmID)
      osAPIError(E_OS_NOFUNC, osdErrCANotActive);
      OS_CA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      OS_CA_EXIT()
      COVER(0x02C1)
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x02C2)

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

#if (osdUseCounterAPI != 0)
   if ( oskAlarmCounterRef[alarmID] > 0 ) /*this alarm is handled via CounterAPI => generated alarm*/
   {      
      if (osAlarmTime[alarmID] == 0)
      {
         /* timer is inactive => return at once */
         osSaveCancelAlarm_AlarmID(alarmID)
         osAPIError(E_OS_NOFUNC, osdErrCANotActive);
         /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
         osRestoreEnableGlobalNested();
         /* KB end osekHWosRestoreEnableGlobalNested */
         #endif   
         /* KB end osekHWosEnableTimProtInterrupt */
         OS_CA_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevel (overwritten) */
         /* restore level saved by osSaveDisableLevel */
         osRestoreEnableLevel();
         /* KB end osekHWosRestoreEnableLevel */
         OS_CA_EXIT()
         COVER(0x02C3)
         return E_OS_NOFUNC;
      }
      ELSEWITHCOVER(0x02C4)
      osAlarmTime[alarmID] = 0;
      osAlarmActivated[alarmID] = 0;
      COVER(0x02C5)
   }
   else                                 /*this alarm is handled via SystemCounter => heap-alarm*/
#else  /*!osdUseCounterAPI*/
                                        /*all alarms are handled via SystemCounter => heap-alarm*/
#endif /*!osdUseCounterAPI*/
   {
      switch(osRemoveItemMinHeap(alarmID))
      {
#if (osdUseHighResolutionTimer != 0)
         case 3:
            osClearTimerInterruptPoint();
            COVER(0x02C6)
            break;
         case 2:
            (void)osSetTimerInterruptPoint( osAlarmTime[osAlarmHeap[0]] );
            COVER(0x02C7)
            break;
#endif /*osdUseHighResolutionTimer*/
         case 1:
            COVER(0x02C8)
            break;
         case 0:
         default:
            ; /*empty instruction, required for MISRA*/
#if osdEnableAssertions
            /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
            #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
            osRestoreEnableGlobalNested();
            /* KB end osekHWosRestoreEnableGlobalNested */
            #endif   
            /* KB end osekHWosEnableTimProtInterrupt */
            OS_CA_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel (overwritten) */
            /* restore level saved by osSaveDisableLevel */
            osRestoreEnableLevel();
            /* KB end osekHWosRestoreEnableLevel */
            OS_CA_EXIT()
            osAbortSystem(osdErrCAAlarmInternal);
#endif /*osdEnableAssertions*/
      }/*switch*/
      osAlarmActivated[alarmID] = 0;
      COVER(0x02C9)
   }
   COVER(0x02CA)
#endif /*osdAlarmsOnSystemTimer*/
#endif /*osdUseHeapAlarm*/

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_CA_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
   OS_CA_EXIT()
   COVER(0x02CB)
   return E_OK;

} /* END OF osCancelAlarm */
#endif


#if ((osdSC== SC3) || (osdSC== SC4))
/*-+--------------------------------------------------------------------------
 / osSysCancelAlarm
 /
 / Funktion:
 /   Same as CancelAlarm but without any checks.
 / Context:
 /   Interrupts have to be disabled up to the system level at least.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   alarmID The identifier of the alarm to cancel.
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osSysCancelAlarm(AlarmType alarmID)
{
   /* CovComment 4:  Function: osSysCancelAlarm */
#if (0==osdUseHeapAlarm)
   /*all alarms are handled as generated alarm*/
   osAlarmTime[alarmID] = 0;
   COVER(0x02CC)
#else /*osdUseHeapAlarm*/
#if (osdAlarmsOnSystemTimer)
#if (osdUseCounterAPI != 0)
   if ( oskAlarmCounterRef[alarmID] > 0 ) /*this alarm is handled via CounterAPI => generated alarm*/
   {      
      osAlarmTime[alarmID] = 0;
      osAlarmActivated[alarmID] = 0;
      COVER(0x02CD)
   }
   else                                 /*this alarm is handled via SystemCounter => heap-alarm*/
#else  /*!osdUseCounterAPI*/
                                        /*all alarms are handled via SystemCounter => heap-alarm*/
#endif /*!osdUseCounterAPI*/
   {
#if (osdUseHighResolutionTimer != 0)
      switch(osRemoveItemMinHeap(alarmID))
      {
         case 3:
            osClearTimerInterruptPoint();
            COVER(0x02CE)
            break;
         case 2:
            (void)osSetTimerInterruptPoint( osAlarmTime[osAlarmHeap[0]] );
            COVER(0x02CF)
            break;
         default:
            ; /*empty instruction, required for MISRA*/
      }/*switch*/
#else  /*!osdUseHighResolutionTimer*/
      (void) osRemoveItemMinHeap(alarmID);
      COVER(0x02D0)
#endif /*!osdUseHighResolutionTimer*/
      osAlarmActivated[alarmID] = 0;
      COVER(0x02D1)
   }
   COVER(0x02D2)
#endif /*osdAlarmsOnSystemTimer*/
#endif /*osdUseHeapAlarm*/
} /* END OF osSysCancelAlarm */
#endif /* ((osdSC== SC3) || (osdSC== SC4)) */

#else
/* KB begin osekHWosOsekAlrmDummy (default) */
/* KB end osekHWosOsekAlrmDummy */
#endif /* (osdNumberOfAlarms > 0) */

/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */
/* END OF MODULE osekalrm.c */
