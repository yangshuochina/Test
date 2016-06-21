/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekevnt.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.46
|
|  Description: osek API-functions for handling events
|               - SetEvent
|               - ClearEvent
|               - GetEvent
|               - WaitEvent
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

/* CovComment 0:  file: osekevnt.c */

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

#if ((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0)

/*-+--------------------------------------------------------------------------
 / SetEvent
 / ========          
 /
 / Syntax:          StatusType SetEvent ( TaskType <TaskID>
 /                                        EventMaskType <Mask> )
 / Parameter (In):  - TaskID Reference to the task for which one or 
 /                    several events are to be set.
 /                  - Mask Mask of the events to be set
 / Parameter (Out): none
 / Description:     The service might be called from an interrupt service 
 /                  routine and from the task level, but not from hook 
 /                  routines.
 /                  The events of task <TaskID> are set according to the 
 /                  event mask <Mask>. Calling SetEvent causes the task 
 /                  <TaskID> to be transferred to the ready state, if it
 /                  has been waiting for at least one of the events 
 /                  specified in <Mask>.
 / Particularities: Any events not set in the event mask remain unchanged.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Referenced task is no extended task, E_OS_ACCESS
 /                  - Events can not be set as the referenced task is in the
 /                    suspended state, E_OS_STATE
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     ECC1, ECC2
 / Context:         Task, Cat2-ISR, interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

#ifdef osdSysAPI
#undef osdSysAPI
#endif

#ifndef osdSetEventNotUsed
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/SetEvent
 */
osqFunc1 StatusType osqFunc2 osSetEvent(TaskType taskIndex, EventMaskType setMask)
{
   /* CovComment 4:  Function: osSetEvent */
/* KB begin setEventBody (default) */

   osPrioType priority;

   OS_SE_ENTRY()
   COVER(0x0397)

#ifndef osdSysAPI
   COVER(0x0398)
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0399)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_SetEvent & osCurrentContext) == 0)
   {
      COVER(0x039A)
      #if osdErrorHook 
      COVER(0x039B)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSECallContext);
      #if osdErrorHook 
      COVER(0x039C)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x039D)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x039E)
      #if osdErrorHook 
      COVER(0x039F)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ID, osdErrSEWrongTaskID);
      #if osdErrorHook 
      COVER(0x03A0)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x03A1)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplTaskAccess[taskIndex] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x03A2)
      #if osdErrorHook 
      COVER(0x03A3)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSENoAccess);
      #if osdErrorHook 
      COVER(0x03A4)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x03A5)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if osdNumberOfExtendedTasks != osdNumberOfAllTasks
   if (taskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x03A6)
      #if osdErrorHook 
      COVER(0x03A7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSENotExtendedTask);
      #if osdErrorHook 
      COVER(0x03A8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x03A9)
#endif


   if (osIntAPIStatus != 0)
   {
      COVER(0x03AA)
      #if osdErrorHook 
      COVER(0x03AB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x03AC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x03AD)

#endif   /* STATUS_LEVEL == EXTENDED_STATUS */

   COVER(0x03AE)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SE_START_CRITICAL()
#endif   /* osdSysAPI */


#if STATUS_LEVEL == EXTENDED_STATUS
   if (osTcbTaskState[taskIndex] == SUSPENDED)
   {
      COVER(0x03AF)
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      osAPIError(E_OS_STATE, osdErrSETaskSuspended);
#ifndef osdSysAPI
      COVER(0x03B0)
      OS_SE_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
#endif
      OS_SE_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x03B1)
#endif

#ifndef osdSysAPI
   COVER(0x03B2)
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */
#endif

   COVER(0x03B3)
   osTcbSetEventMask[taskIndex]|= setMask; /* set event flag(s)    */

   if (osTcbTaskState[taskIndex] == WAITING)
   {
      /* task is waiting */
#if osdTimingProtectionUsed
      osbool EventsRequireStateChange = osdFALSE;
      osbool StateChangeAllowed = osdFALSE;

      if ((osTcbSetEventMask[taskIndex] & osTcbWaitEventMask[taskIndex]) != 0)
      {
         COVER(0x03B4)
         EventsRequireStateChange = osdTRUE;
         StateChangeAllowed = osTPArrivalAllowed(taskIndex);
      }
      ELSEWITHCOVER(0x03B5)
      if ((EventsRequireStateChange == osdTRUE) && (StateChangeAllowed == osdTRUE))
      {
         COVER(0x03B6)
#else
      if ((osTcbSetEventMask[taskIndex] & osTcbWaitEventMask[taskIndex]) != 0)
      {
         COVER(0x03B7)
#endif
         /*  satisfied, change from waiting to ready */

         /* get priority of continuued task */
         priority = oskTcbHomePrio[taskIndex];
#if (osdNumberOfPriorities > 0)
         COVER(0x03B8)
         osSysErrAssert((priority < osrNumberOfPriorities), osdErrSEWrongTaskPrio)
#endif

         osTcbTaskState[taskIndex] = READY;
         
         /* KB begin osQInsertAtPrio (default) */
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x03B9)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; 
         #else
         COVER(0x03BA)
         osQReadyTask[priority]= taskIndex;
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x03BB)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x03BC)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x03BD)
         osQReadyTaskTail[priority]++;
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])
         {
            COVER(0x03BE)
            osQReadyTaskTail[priority]= 0;
         }
         ELSEWITHCOVER(0x03BF)
         #endif
         
         
         /* KB end osQInsertAtPrio */

         if (priority < osHighReadyTaskPrio)
         {
            COVER(0x03C0)
            osHighReadyTaskPrio   = priority;
            osHighReadyTaskIndex  = taskIndex;
            osStartDispatcher= osdTRUE;

#ifndef osdSysAPI

            COVER(0x03C1)
            /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
            #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
            osRestoreEnableGlobalNested();
            /* KB end osekHWosRestoreEnableGlobalNested */
            #endif   
            /* KB end osekHWosEnableTimProtInterrupt */

            OS_SE_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel (overwritten) */
            /* restore level saved by osSaveDisableLevel */
            osRestoreEnableLevel();
            /* KB end osekHWosRestoreEnableLevel */
#endif
#if osdFullPreempt || osdMixedPreempt
            if (osLockDispatcher == 0)
            {
               COVER(0x03C2)
               OS_START_DISPATCH()
               OS_SE_EXIT()
               /* KB begin osekHWcallosDispatcher (default) */
                  /* KB begin osekHWosDispatcher (overwritten) */
                  osDispatch()
                  /* KB end osekHWosDispatcher */
               /* KB end osekHWcallosDispatcher */
               return E_OK;
            }
            ELSEWITHCOVER(0x03C3)
#endif
            OS_SE_EXIT()
            return E_OK;
         }
         ELSEWITHCOVER(0x03C4)
      }
      ELSEWITHCOVER(0x03C5)
   }
   ELSEWITHCOVER(0x03C6)
#ifndef osdSysAPI
   COVER(0x03C7)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */

   OS_SE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
#endif
   OS_SE_EXIT()
   return E_OK;


/* KB end setEventBody */
} /* END OF osSetEvent */
#endif


#if ((osdNumberOfAlarmSetEvent > 0) || (osdNumberOfSTSetEvent > 0))
#define osdSysAPI
/*
 * MISRA RULE 14.1 VIOLATION: The function osSysSetEvent is unused in some configurations. In order to keep the code 
 * simple and error free, it is available in more configurations than necessary.
 */
osqFunc1 StatusType osqFunc2  osSysSetEvent(TaskType taskIndex, EventMaskType setMask) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osSysSetEvent */
/* KB begin setEventBody (default) */

   osPrioType priority;

   OS_SE_ENTRY()
   COVER(0x03C8)

#ifndef osdSysAPI
   COVER(0x03C9)
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x03CA)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_SetEvent & osCurrentContext) == 0)
   {
      COVER(0x03CB)
      #if osdErrorHook 
      COVER(0x03CC)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSECallContext);
      #if osdErrorHook 
      COVER(0x03CD)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x03CE)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x03CF)
      #if osdErrorHook 
      COVER(0x03D0)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ID, osdErrSEWrongTaskID);
      #if osdErrorHook 
      COVER(0x03D1)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x03D2)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplTaskAccess[taskIndex] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x03D3)
      #if osdErrorHook 
      COVER(0x03D4)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSENoAccess);
      #if osdErrorHook 
      COVER(0x03D5)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x03D6)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if osdNumberOfExtendedTasks != osdNumberOfAllTasks
   if (taskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x03D7)
      #if osdErrorHook 
      COVER(0x03D8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSENotExtendedTask);
      #if osdErrorHook 
      COVER(0x03D9)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x03DA)
#endif


   if (osIntAPIStatus != 0)
   {
      COVER(0x03DB)
      #if osdErrorHook 
      COVER(0x03DC)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x03DD)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x03DE)

#endif   /* STATUS_LEVEL == EXTENDED_STATUS */

   COVER(0x03DF)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_SE_START_CRITICAL()
#endif   /* osdSysAPI */


#if STATUS_LEVEL == EXTENDED_STATUS
   if (osTcbTaskState[taskIndex] == SUSPENDED)
   {
      COVER(0x03E0)
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      osAPIError(E_OS_STATE, osdErrSETaskSuspended);
#ifndef osdSysAPI
      COVER(0x03E1)
      OS_SE_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
#endif
      OS_SE_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x03E2)
#endif

#ifndef osdSysAPI
   COVER(0x03E3)
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */
#endif

   COVER(0x03E4)
   osTcbSetEventMask[taskIndex]|= setMask; /* set event flag(s)    */

   if (osTcbTaskState[taskIndex] == WAITING)
   {
      /* task is waiting */
#if osdTimingProtectionUsed
      osbool EventsRequireStateChange = osdFALSE;
      osbool StateChangeAllowed = osdFALSE;

      if ((osTcbSetEventMask[taskIndex] & osTcbWaitEventMask[taskIndex]) != 0)
      {
         COVER(0x03E5)
         EventsRequireStateChange = osdTRUE;
         StateChangeAllowed = osTPArrivalAllowed(taskIndex);
      }
      ELSEWITHCOVER(0x03E6)
      if ((EventsRequireStateChange == osdTRUE) && (StateChangeAllowed == osdTRUE))
      {
         COVER(0x03E7)
#else
      if ((osTcbSetEventMask[taskIndex] & osTcbWaitEventMask[taskIndex]) != 0)
      {
         COVER(0x03E8)
#endif
         /*  satisfied, change from waiting to ready */

         /* get priority of continuued task */
         priority = oskTcbHomePrio[taskIndex];
#if (osdNumberOfPriorities > 0)
         COVER(0x03E9)
         osSysErrAssert((priority < osrNumberOfPriorities), osdErrSEWrongTaskPrio)
#endif

         osTcbTaskState[taskIndex] = READY;
         
         /* KB begin osQInsertAtPrio (default) */
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x03EA)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; 
         #else
         COVER(0x03EB)
         osQReadyTask[priority]= taskIndex;
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x03EC)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x03ED)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x03EE)
         osQReadyTaskTail[priority]++;
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])
         {
            COVER(0x03EF)
            osQReadyTaskTail[priority]= 0;
         }
         ELSEWITHCOVER(0x03F0)
         #endif
         
         
         /* KB end osQInsertAtPrio */

         if (priority < osHighReadyTaskPrio)
         {
            COVER(0x03F1)
            osHighReadyTaskPrio   = priority;
            osHighReadyTaskIndex  = taskIndex;
            osStartDispatcher= osdTRUE;

#ifndef osdSysAPI

            COVER(0x03F2)
            /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
            #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
            osRestoreEnableGlobalNested();
            /* KB end osekHWosRestoreEnableGlobalNested */
            #endif   
            /* KB end osekHWosEnableTimProtInterrupt */

            OS_SE_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel (overwritten) */
            /* restore level saved by osSaveDisableLevel */
            osRestoreEnableLevel();
            /* KB end osekHWosRestoreEnableLevel */
#endif
#if osdFullPreempt || osdMixedPreempt
            if (osLockDispatcher == 0)
            {
               COVER(0x03F3)
               OS_START_DISPATCH()
               OS_SE_EXIT()
               /* KB begin osekHWcallosDispatcher (default) */
                  /* KB begin osekHWosDispatcher (overwritten) */
                  osDispatch()
                  /* KB end osekHWosDispatcher */
               /* KB end osekHWcallosDispatcher */
               return E_OK;
            }
            ELSEWITHCOVER(0x03F4)
#endif
            OS_SE_EXIT()
            return E_OK;
         }
         ELSEWITHCOVER(0x03F5)
      }
      ELSEWITHCOVER(0x03F6)
   }
   ELSEWITHCOVER(0x03F7)
#ifndef osdSysAPI
   COVER(0x03F8)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */

   OS_SE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
#endif
   OS_SE_EXIT()
   return E_OK;


/* KB end setEventBody */
} /* END OF osSysSetEvent */
#endif


#ifndef osdClearEventNotUsed
/*-+--------------------------------------------------------------------------
 / ClearEvent
 / =========
 /
 / Syntax:          StatusType ClearEvent ( EventMaskType <Mask> )
 / Parameter (In):  Mask Mask of the events to be cleared
 / Parameter (Out): none
 / Description:     The events of the extended task calling ClearEvent are 
 /                  cleared according to the event mask <Mask>.
 / Particularities: The system service ClearEvent is restricted to extended 
 /                  tasks which own the event.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Call not from extended task, E_OS_ACCESS
 /                  - Call at interrupt level, E_OS_CALLEVEL
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     ECC1, ECC2
 / Context:         Extended task, interrupts enabled
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/ClearEvent
 */
osqFunc1 StatusType osqFunc2 osClearEvent (EventMaskType Mask )
{
   /* CovComment 4:  Function: osClearEvent */
   OS_CE_ENTRY()
   COVER(0x03F9)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x03FA)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   COVER(0x03FB)
   /* check call context */
   if ((osdValidCtx_ClearEvent & osCurrentContext) == 0)
   {
      COVER(0x03FC)
      #if osdErrorHook 
      COVER(0x03FD)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrCECallContext);
      #if osdErrorHook 
      COVER(0x03FE)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x03FF)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntNestingDepth != 0)
   {
      COVER(0x0400)
      #if osdErrorHook 
      COVER(0x0401)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrCEOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x0402)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0403)

   if (osActiveTaskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x0404)
      #if osdErrorHook 
      COVER(0x0405)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrCENotExtendedTask);
      #if osdErrorHook 
      COVER(0x0406)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0407)

   if (osIntAPIStatus != 0)
   {
      COVER(0x0408)
      #if osdErrorHook 
      COVER(0x0409)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrCEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x040A)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x040B)
#endif

   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */ /* disable interrupts as next command is not atomic */
   OS_CE_START_CRITICAL()
   osTcbSetEventMask[osActiveTaskIndex]&=(EventMaskType)(~Mask);
   OS_CE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

   COVER(0x040C)
   OS_CE_EXIT()
   return E_OK;
} /* END OF osClearEvent */
#endif


#ifndef osdGetEventNotUsed
/*-+--------------------------------------------------------------------------
 / GetEvent
 / ========
 /
 / Syntax:          StatusType GetEvent ( TaskType <TaskID>, 
 /                                        EventMaskRefType <Event> )
 / Parameter (In):  TaskID Task whose event mask is to be returned.
 / Parameter (Out): Event Reference to the memory of the return data.
 / Description:     This service returns the current state of all event bits 
 /                  of the task <TaskID>, not the events that the task is 
 /                  waiting for.
 /                  The service might be called from interrupt service 
 /                  routines, task level and some hook routines.
 /                  The current status of the event mask of task <TaskID> is
 /                  copied to <Event>.
 / Particularities: The referenced task must be an extended task.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Referenced task <TaskID> is not an extended task, 
 /                    E_OS_ACCESS
 /                  - Referenced task <TaskID> is in the suspended state, 
 /                    E_OS_STATE
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook, PreTaskHook, PostTaskHook,
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/GetEvent
 */
osqFunc1 StatusType osqFunc2 osGetEvent(TaskType taskIndex, EventMaskRefType Event)
{
   /* CovComment 4:  Function: osGetEvent */

   OS_GE_ENTRY()
   COVER(0x040D)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x040E)
#if ((osdSC== SC3) || (osdSC== SC4))
   COVER(0x040F)
   /* check call context */
   if ((osdValidCtx_GetEvent & osCurrentContext) == 0)
   {
      COVER(0x0410)
      #if osdErrorHook 
      COVER(0x0411)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrGECallContext);
      #if osdErrorHook 
      COVER(0x0412)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0413)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      COVER(0x0414)
      if (osAddressParamValid((osuint32)Event, (osuint32)sizeof(EventMaskType)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
         COVER(0x0415)
         #if osdErrorHook 
         COVER(0x0416)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetEvent_TaskID(taskIndex)
         osSaveGetEvent_Event(Event)
         #endif
         osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrGEIllegalAddr);
         #if osdErrorHook 
         COVER(0x0417)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_GE_EXIT()
         return E_OS_ILLEGAL_ADDRESS;
       }
       ELSEWITHCOVER(0x0418)
   }
   ELSEWITHCOVER(0x0419)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x041A)
      #if osdErrorHook 
      COVER(0x041B)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_ID, osdErrGEWrongTaskID);
      #if osdErrorHook 
      COVER(0x041C)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x041D)
#if ((osdSC== SC3) || (osdSC== SC4))
   COVER(0x041E)
   /* check access rights for this object */
   if (((oskApplTaskAccess[taskIndex] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x041F)
      #if osdErrorHook 
      COVER(0x0420)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGENoAccess);
      #if osdErrorHook 
      COVER(0x0421)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0422)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if osdNumberOfExtendedTasks != osdNumberOfAllTasks
   COVER(0x0423)
   if (taskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x0424)
      #if osdErrorHook 
      COVER(0x0425)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGENotExtendedTask);
      #if osdErrorHook 
      COVER(0x0426)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0427)
#endif

   if (osTcbTaskState[taskIndex] == SUSPENDED)
   {
      COVER(0x0428)
      #if osdErrorHook 
      COVER(0x0429)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_STATE, osdErrGETaskSuspended);
      #if osdErrorHook 
      COVER(0x042A)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x042B)

   if (osIntAPIStatus != 0)
   {
      COVER(0x042C)
      #if osdErrorHook 
      COVER(0x042D)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrGEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x042E)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x042F)
#endif

#ifndef osdEventMaskTypeSize
#error "osdEventMaskTypeSize not defined"
#endif

#if( osdEventMaskTypeSize > 1 )
   /* CovComment 2:  Config: EventMaskType > MachineWord */
   COVER(0x0430)
   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_GE_START_CRITICAL()
   /* CovComment 2:  */
#endif

   COVER(0x0431)
   *Event= osTcbSetEventMask[taskIndex];

#if( osdEventMaskTypeSize > 1 )
   /* CovComment 2:  Config: EventMaskType > MachineWord */
   COVER(0x0432)
   OS_GE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
   /* CovComment 2:  */
#endif

   COVER(0x0433)
   OS_GE_EXIT()
   return E_OK;
} /* END OF osGetEvent */
#endif


#ifndef osdWaitEventNotUsed
/*-+--------------------------------------------------------------------------
 / WaitEvent
 / =========
 /
 / Syntax:          StatusType WaitEvent ( EventMaskType <Mask> )
 / Parameter (In):  Mask Mask of the events waited for.
 / Parameter (Out): none
 / Description:     The state of the calling task is set to waiting, unless 
 /                  at least one of the events specified in <Mask> has 
 /                  already been set.
 / Particularities: This call enforces rescheduling, if the wait condition 
 /                  occurs. If rescheduling takes place, the internal 
 /                  resource of the task is released while the task is in 
 /                  the waiting state.
 /                  This service must only be called from the extended task 
 /                  owning the event.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Calling task is not an extended task, E_OS_ACCESS
 /                  - Calling task occupies resources, E_OS_RESOURCE
 /                  - Call at interrupt level, E_OS_CALLEVEL
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     ECC1, ECC2
 / Context:         Extended task, interrupts enabled, no resources occupied
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/WaitEvent
 */
osqFunc1 StatusType osqFunc2 osWaitEvent(EventMaskType mask)
{
   /* CovComment 4:  Function: osWaitEvent */
#if osdTimingProtectionUsed
   osbool EventsRequireWait = osdFALSE;
   osbool ArrivalAllowed = osdTRUE;
#endif
   OS_WE_ENTRY()
   COVER(0x0434)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0435)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   COVER(0x0436)
   /* check call context */
   if ((osdValidCtx_WaitEvent & osCurrentContext) == 0)
   {
      COVER(0x0437)
      #if osdErrorHook 
      COVER(0x0438)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrWECallContext);
      #if osdErrorHook 
      COVER(0x0439)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x043A)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntNestingDepth != 0)
   {
      COVER(0x043B)
      #if osdErrorHook 
      COVER(0x043C)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrWEOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x043D)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x043E)

   if (osActiveTaskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x043F)
      #if osdErrorHook 
      COVER(0x0440)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrWENotExtendedTask);
      #if osdErrorHook 
      COVER(0x0441)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0442)

#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   COVER(0x0443)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
#else
   COVER(0x0444)
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
#endif
   {
      COVER(0x0445)
      #if osdErrorHook 
      COVER(0x0446)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_RESOURCE, osdErrWEResourcesOccupied);
      #if osdErrorHook 
      COVER(0x0447)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x0448)

   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x0449)
      #if osdErrorHook 
      COVER(0x044A)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrWEInterruptsDisabled);
      #if osdErrorHook 
      COVER(0x044B)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x044C)
#endif

   COVER(0x044D)
   /* KB begin osekHWosDisableLevel (overwritten) */
   /* disable interrupts up to system level */
   osDisableLevel();
   /* KB end osekHWosDisableLevel */ /* (WaitEvent is only called from task level) */
   OS_WE_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   osTcbWaitEventMask[osActiveTaskIndex]= mask;

#if osdTimingProtectionUsed
   if ((osTcbSetEventMask[osActiveTaskIndex] & mask) == 0)
   {
      EventsRequireWait = osdTRUE;
      COVER(0x044E)
   }
   else
   {
      ArrivalAllowed = osTPArrivalAllowed(osActiveTaskIndex);
      COVER(0x044F)
   }
   if ((EventsRequireWait == osdTRUE) || (ArrivalAllowed == osdFALSE))
   {
      COVER(0x0450)
#else
   if ((osTcbSetEventMask[osActiveTaskIndex] & mask) == 0)
   {
      COVER(0x0451)
#endif
      /* no expected event set, set task into waiting state */

      /* call hook routine for the waiting task */
#if osdPostTaskHook
      COVER(0x0452)
      osPostTaskHook();
#endif

#if osdTimingProtectionUsed
      COVER(0x0453)
      /* stop execution time monitoring/measurement for this task, reload execution time budget */
      osTPExeSwitchBack(INVALID_PROCESS);
#endif

#if osdNumberOfInternalResources > 0
      COVER(0x0454)
      /* KB begin osReleaseInternalResource (default) */
      if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
      {
      #if osdRTSize == 1
         COVER(0x0455)
         osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
      #else
         COVER(0x0456)
         osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] &= /*;*/
            ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
      #endif
      #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0457)
         osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
      #endif
      
      #if osdORTIDebug
         COVER(0x0458)
         osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
      #endif
      
      }
      ELSEWITHCOVER(0x0459)
      
      
      /* KB end osReleaseInternalResource */
#endif

      COVER(0x045A)
      /* change task state from ready to waiting */
      osTcbTaskState[osActiveTaskIndex] = WAITING;

      /* KB begin osQDeleteFromPrio (default) */
      
      #if ((CC == ECC2) || (CC == BCC2))
      /* delete task out of the prio queue */
      osQReadyTaskHead[osActiveTaskPrio]++;
      if (osQReadyTaskHead[osActiveTaskPrio] > oskQMaxActivations[osActiveTaskPrio])
      {
         COVER(0x045B)
         osQReadyTaskHead[osActiveTaskPrio]= 0;
      }
      ELSEWITHCOVER(0x045C)
      
      if (osQReadyTaskHead[osActiveTaskPrio] == osQReadyTaskTail[osActiveTaskPrio])
      {  /* queue empty */
         COVER(0x045D)
      #endif
      
      #if (osdRTSize > 1)
         COVER(0x045E)
         osQReadyPrios[oskTcbReadyPrioOffset[osActiveTaskIndex]]&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
      #else
         COVER(0x045F)
         osQReadyPrios&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
      #endif
      
      #if ((CC == ECC2) || (CC == BCC2))
      }
      ELSEWITHCOVER(0x0460)
      #endif
      
      /* KB end osQDeleteFromPrio */

      /* for WaitEvent, osSchedulePrio returns always osdTRUE! */
      /* (same task is never running when reaching this point of code) */
      (void) osSchedulePrio(); 

      /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      #endif   
      /* KB end osekHWosEnableTimProtInterrupt */
      OS_WE_END_CRITICAL()
      OS_WE_EXIT()
      /* KB begin osekHWosEnableLevel (overwritten) */
      /* enable task level interrupt */
      osEnableLevel();   
      /* KB end osekHWosEnableLevel */

      /* start dispatcher */
      OS_START_DISPATCH()
      /* KB begin osekHWcallosDispatcher (default) */
         /* KB begin osekHWosDispatcher (overwritten) */
         osDispatch()
         /* KB end osekHWosDispatcher */
      /* KB end osekHWcallosDispatcher */

      return E_OK;
   }
#if osdTimingProtectionUsed
   else
   {
      COVER(0x0461)
      /* stop execution time monitoring/measurement for this task , reload execution time budget */
      osTPExeSwitchBack(osActiveTaskIndex);
   }
#else
   ELSEWITHCOVER(0x0462)
#endif

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_WE_END_CRITICAL()
   OS_WE_EXIT()
   /* KB begin osekHWosEnableLevel (overwritten) */
   /* enable task level interrupt */
   osEnableLevel();   
   /* KB end osekHWosEnableLevel */

   return E_OK;
} /* END OF osWaitEvent */
#endif

#else  /* End of ((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0) */
/* KB begin osekHWosOsekEvntDummy (default) */
/* KB end osekHWosOsekEvntDummy */
#endif

/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE osekevnt.c */

