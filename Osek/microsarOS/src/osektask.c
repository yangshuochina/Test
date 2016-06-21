/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osektask.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.62
|
|  Description: osek API-functions for controlling tasks
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

/* CovComment 0:  file: osektask.c */

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

#if (osdNumberOfAllTasks > 0)
/*-+--------------------------------------------------------------------------
 / ActivateTask
 / ============
 /
 / Syntax:          StatusType ActivateTask ( TaskType <TaskID> )
 / Parameter (In):  TaskID Task reference
 / Parameter (Out): none
 / Description:     The task <TaskID> is transferred from the suspended state
 /                  into the ready state. The operating system ensures that the 
 /                  task code is being executed from the first statement.
 / Particularities: The service may be called from interrupt level and from 
 /                  task level.
 /                  Rescheduling after the call to ActivateTask depends on the
 /                  place it is called from (ISR, non preemptable task, 
 /                  preemptable task).
 /                  If E_OS_LIMIT is returned the activation is ignored.
 /                  When an extended task is transferred from suspended state
 /                  into ready state all its events are cleared.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Too many task activations of <TaskID>, E_OS_LIMIT
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:
 /  -General:       Task, Cat2-ISR, interrupts not disabled by the 
 /                  application
 /  -SC1/SC2 only:  StartupHook (compatibility to older OSEK-OS)
 /     
 /--------------------------------------------------------------------------*/

#ifdef osdSysAPI
#undef osdSysAPI
#endif

#ifndef osdActivateTaskNotUsed

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/ActivateTask
 */
osqFunc1 StatusType osqFunc2 osActivateTask(TaskType taskIndex)
{
   /* CovComment 4:  Function: osActivateTask */
   /* KB begin actTaskBody (default) */
   
      osPrioType priority;
   
      OS_AT_ENTRY()
      COVER(0x0500)
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && !defined osdSysAPI)
   #if ((osdSC== SC3) || (osdSC== SC4)) 
      /* check call context */
      if ((osdValidCtx_ActivateTask & osCurrentContext) == 0)
      {
         COVER(0x0501)
         #if osdErrorHook 
         COVER(0x0502)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_CALLEVEL, osdErrATCallContext);
         #if osdErrorHook 
         COVER(0x0503)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_CALLEVEL;
      }
      ELSEWITHCOVER(0x0504)
   #endif /*((osdSC== SC3) || (osdSC== SC4))*/
   
      if (taskIndex >= osrNumberOfAllTasks)
      {
         COVER(0x0505)
         #if osdErrorHook 
         COVER(0x0506)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_ID, osdErrATWrongTaskID);
         #if osdErrorHook 
         COVER(0x0507)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_ID;
      }
      ELSEWITHCOVER(0x0508)
      if (osIntAPIStatus != 0)
      {
         COVER(0x0509)
         #if osdErrorHook 
         COVER(0x050A)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_DISABLEDINT, osdErrATIntAPIDisabled);
         #if osdErrorHook 
         COVER(0x050B)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_DISABLEDINT;
      }
      ELSEWITHCOVER(0x050C)
   #if ((osdSC== SC3) || (osdSC== SC4))
      /* check access rights for this object */
      if (((oskApplTaskAccess[taskIndex] & (1UL << osActiveApplicationID)) == 0))
      {
         COVER(0x050D)
         #if osdErrorHook 
         COVER(0x050E)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_ACCESS, osdErrATNoAccess);
         #if osdErrorHook 
         COVER(0x050F)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_ACCESS;
      }
      ELSEWITHCOVER(0x0510)
   #endif /*((osdSC== SC3) || (osdSC== SC4))*/
   #endif /* ((STATUS_LEVEL == EXTENDED_STATUS) && !defined osdSysAPI) */
   
      COVER(0x0511)
      priority = oskTcbHomePrio[taskIndex];
      osSysErrAssert((priority < osrNumberOfPriorities), osdErrATWrongTaskPrio)
   
   #ifndef osdSysAPI
      COVER(0x0512)
      /* KB begin osekHWosSaveDisableLevel (overwritten) */
      /* save level and disable up to system level, no nesting */
      osSaveDisableLevel();
      /* KB end osekHWosSaveDisableLevel */    /* use only in system version, not for user API */
      OS_AT_START_CRITICAL()
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
      if (osTcbActivationCount[taskIndex] >= oskTcbAllowedActivations[taskIndex])
      {
         COVER(0x0513)
   #else
      if (osTcbTaskState[taskIndex] != SUSPENDED)
      {
         COVER(0x0514)
   #endif
         osSaveActivateTask_TaskID(taskIndex)
   #ifndef osdSysAPI
         COVER(0x0515)
         osAPIError(E_OS_LIMIT, osdErrATMultipleActivation);
   #else
         COVER(0x0516)
         osAPIError(E_OS_LIMIT, osdErrATAlarmMultipleActivation);
   #endif
   #ifndef osdSysAPI
         COVER(0x0517)
         OS_AT_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevel (overwritten) */
         /* restore level saved by osSaveDisableLevel */
         osRestoreEnableLevel();
         /* KB end osekHWosRestoreEnableLevel */
   #endif
         OS_AT_EXIT()
         return E_OS_LIMIT;
      }
      ELSEWITHCOVER(0x0518)
   
   #if osdTimingProtectionUsed
      if (osTPArrivalAllowed(taskIndex)==osdTRUE)
      {
         COVER(0x0519)
   #endif
   
   #ifndef osdSysAPI
         COVER(0x051A)
         /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
         osSaveDisableGlobalNested();
         /* KB end osekHWosSaveDisableGlobalNested */
         #endif    
         /* KB end osekHWosDisableTimProtInterrupt */
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
         if (osTcbActivationCount[taskIndex] == 0)
         {
            COVER(0x051B)
            osTcbTaskState[taskIndex] = PRE_READY;
         }
         ELSEWITHCOVER(0x051C)
         osTcbActivationCount[taskIndex]++;
   #else
         COVER(0x051D)
         osTcbTaskState[taskIndex] = PRE_READY;
   #endif
   
   #if (osdNumberOfExtendedTasks > 0)
   #if ((osdNumberOfExtendedTasks < osdNumberOfAllTasks) || (osdLib))
         if(taskIndex<osrNumberOfExtendedTasks)
         {
            COVER(0x051E)
            osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
         }
         ELSEWITHCOVER(0x051F)
   #else
         /*all tasks are extended*/
         COVER(0x0520)
         osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
   #endif
   #endif /* (osdNumberOfExtendedTasks > 0) */
   
         /* KB begin osQInsertAtPrio (default) */
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0521)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; 
         #else
         COVER(0x0522)
         osQReadyTask[priority]= taskIndex;
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x0523)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x0524)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0525)
         osQReadyTaskTail[priority]++;
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])
         {
            COVER(0x0526)
            osQReadyTaskTail[priority]= 0;
         }
         ELSEWITHCOVER(0x0527)
         #endif
         
         
         /* KB end osQInsertAtPrio */
   
         /* start dispatcher only if new task has greater prio */
   
         if (priority < osHighReadyTaskPrio)
         {
   
            COVER(0x0528)
            osHighReadyTaskPrio   = priority;
            osHighReadyTaskIndex  = taskIndex;
            osStartDispatcher= osdTRUE;     /* for return from idle loop */
   
   #if osdFullPreempt || osdMixedPreempt
            if (osLockDispatcher == 0)
            {
               COVER(0x0529)
   #ifndef osdSysAPI
               COVER(0x052A)
               /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
               #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
               /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
               osRestoreEnableGlobalNested();
               /* KB end osekHWosRestoreEnableGlobalNested */
               #endif   
               /* KB end osekHWosEnableTimProtInterrupt */
   
               OS_AT_END_CRITICAL()
   
               /* KB begin osekHWosRestoreEnableLevel (overwritten) */
               /* restore level saved by osSaveDisableLevel */
               osRestoreEnableLevel();
               /* KB end osekHWosRestoreEnableLevel */
   #endif
               OS_AT_EXIT()
               OS_START_DISPATCH()
               /* KB begin osekHWcallosDispatcher (default) */
                  /* KB begin osekHWosDispatcher (overwritten) */
                  osDispatch()
                  /* KB end osekHWosDispatcher */
               /* KB end osekHWcallosDispatcher */
               return E_OK;
            }
            ELSEWITHCOVER(0x052B)
   #endif /* osdFullPreempt || osdMixedPreempt */
         }
         ELSEWITHCOVER(0x052C)
   #ifndef osdSysAPI
   
         /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
         osRestoreEnableGlobalNested();
         /* KB end osekHWosRestoreEnableGlobalNested */
         #endif   
         /* KB end osekHWosEnableTimProtInterrupt */
   
   #endif
   
   #if osdTimingProtectionUsed
   
      }/*if (osTPArrivalAllowed(taskIndex))*/
      ELSEWITHCOVER(0x052D)
   #endif
   
   #ifndef osdSysAPI
      OS_AT_END_CRITICAL()
   #endif
   
      OS_AT_EXIT()
   #ifndef osdSysAPI
      COVER(0x052E)
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
   #endif
      COVER(0x052F)
      return E_OK;
   
   /* KB end actTaskBody */
}

#endif


#define osdSysAPI
/* version for OS internal use, not using osDisable/osEnable */
/*
 * MISRA RULE 14.1 VIOLATION: The function osSysActivateTask is unused in some configurations. In order to keep the code 
 * simple and error free, it is available in more configurations than necessary.
 */
osqFunc1 StatusType osqFunc2 osSysActivateTask(TaskType taskIndex) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osSysActivateTask */
   /* KB begin actTaskBody (default) */
   
      osPrioType priority;
   
      OS_AT_ENTRY()
      COVER(0x0530)
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && !defined osdSysAPI)
   #if ((osdSC== SC3) || (osdSC== SC4)) 
      /* check call context */
      if ((osdValidCtx_ActivateTask & osCurrentContext) == 0)
      {
         COVER(0x0531)
         #if osdErrorHook 
         COVER(0x0532)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_CALLEVEL, osdErrATCallContext);
         #if osdErrorHook 
         COVER(0x0533)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_CALLEVEL;
      }
      ELSEWITHCOVER(0x0534)
   #endif /*((osdSC== SC3) || (osdSC== SC4))*/
   
      if (taskIndex >= osrNumberOfAllTasks)
      {
         COVER(0x0535)
         #if osdErrorHook 
         COVER(0x0536)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_ID, osdErrATWrongTaskID);
         #if osdErrorHook 
         COVER(0x0537)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_ID;
      }
      ELSEWITHCOVER(0x0538)
      if (osIntAPIStatus != 0)
      {
         COVER(0x0539)
         #if osdErrorHook 
         COVER(0x053A)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_DISABLEDINT, osdErrATIntAPIDisabled);
         #if osdErrorHook 
         COVER(0x053B)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_DISABLEDINT;
      }
      ELSEWITHCOVER(0x053C)
   #if ((osdSC== SC3) || (osdSC== SC4))
      /* check access rights for this object */
      if (((oskApplTaskAccess[taskIndex] & (1UL << osActiveApplicationID)) == 0))
      {
         COVER(0x053D)
         #if osdErrorHook 
         COVER(0x053E)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_ACCESS, osdErrATNoAccess);
         #if osdErrorHook 
         COVER(0x053F)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_ACCESS;
      }
      ELSEWITHCOVER(0x0540)
   #endif /*((osdSC== SC3) || (osdSC== SC4))*/
   #endif /* ((STATUS_LEVEL == EXTENDED_STATUS) && !defined osdSysAPI) */
   
      COVER(0x0541)
      priority = oskTcbHomePrio[taskIndex];
      osSysErrAssert((priority < osrNumberOfPriorities), osdErrATWrongTaskPrio)
   
   #ifndef osdSysAPI
      COVER(0x0542)
      /* KB begin osekHWosSaveDisableLevel (overwritten) */
      /* save level and disable up to system level, no nesting */
      osSaveDisableLevel();
      /* KB end osekHWosSaveDisableLevel */    /* use only in system version, not for user API */
      OS_AT_START_CRITICAL()
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
      if (osTcbActivationCount[taskIndex] >= oskTcbAllowedActivations[taskIndex])
      {
         COVER(0x0543)
   #else
      if (osTcbTaskState[taskIndex] != SUSPENDED)
      {
         COVER(0x0544)
   #endif
         osSaveActivateTask_TaskID(taskIndex)
   #ifndef osdSysAPI
         COVER(0x0545)
         osAPIError(E_OS_LIMIT, osdErrATMultipleActivation);
   #else
         COVER(0x0546)
         osAPIError(E_OS_LIMIT, osdErrATAlarmMultipleActivation);
   #endif
   #ifndef osdSysAPI
         COVER(0x0547)
         OS_AT_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevel (overwritten) */
         /* restore level saved by osSaveDisableLevel */
         osRestoreEnableLevel();
         /* KB end osekHWosRestoreEnableLevel */
   #endif
         OS_AT_EXIT()
         return E_OS_LIMIT;
      }
      ELSEWITHCOVER(0x0548)
   
   #if osdTimingProtectionUsed
      if (osTPArrivalAllowed(taskIndex)==osdTRUE)
      {
         COVER(0x0549)
   #endif
   
   #ifndef osdSysAPI
         COVER(0x054A)
         /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
         osSaveDisableGlobalNested();
         /* KB end osekHWosSaveDisableGlobalNested */
         #endif    
         /* KB end osekHWosDisableTimProtInterrupt */
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
         if (osTcbActivationCount[taskIndex] == 0)
         {
            COVER(0x054B)
            osTcbTaskState[taskIndex] = PRE_READY;
         }
         ELSEWITHCOVER(0x054C)
         osTcbActivationCount[taskIndex]++;
   #else
         COVER(0x054D)
         osTcbTaskState[taskIndex] = PRE_READY;
   #endif
   
   #if (osdNumberOfExtendedTasks > 0)
   #if ((osdNumberOfExtendedTasks < osdNumberOfAllTasks) || (osdLib))
         if(taskIndex<osrNumberOfExtendedTasks)
         {
            COVER(0x054E)
            osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
         }
         ELSEWITHCOVER(0x054F)
   #else
         /*all tasks are extended*/
         COVER(0x0550)
         osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
   #endif
   #endif /* (osdNumberOfExtendedTasks > 0) */
   
         /* KB begin osQInsertAtPrio (default) */
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0551)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; 
         #else
         COVER(0x0552)
         osQReadyTask[priority]= taskIndex;
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x0553)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x0554)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0555)
         osQReadyTaskTail[priority]++;
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])
         {
            COVER(0x0556)
            osQReadyTaskTail[priority]= 0;
         }
         ELSEWITHCOVER(0x0557)
         #endif
         
         
         /* KB end osQInsertAtPrio */
   
         /* start dispatcher only if new task has greater prio */
   
         if (priority < osHighReadyTaskPrio)
         {
   
            COVER(0x0558)
            osHighReadyTaskPrio   = priority;
            osHighReadyTaskIndex  = taskIndex;
            osStartDispatcher= osdTRUE;     /* for return from idle loop */
   
   #if osdFullPreempt || osdMixedPreempt
            if (osLockDispatcher == 0)
            {
               COVER(0x0559)
   #ifndef osdSysAPI
               COVER(0x055A)
               /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
               #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
               /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
               osRestoreEnableGlobalNested();
               /* KB end osekHWosRestoreEnableGlobalNested */
               #endif   
               /* KB end osekHWosEnableTimProtInterrupt */
   
               OS_AT_END_CRITICAL()
   
               /* KB begin osekHWosRestoreEnableLevel (overwritten) */
               /* restore level saved by osSaveDisableLevel */
               osRestoreEnableLevel();
               /* KB end osekHWosRestoreEnableLevel */
   #endif
               OS_AT_EXIT()
               OS_START_DISPATCH()
               /* KB begin osekHWcallosDispatcher (default) */
                  /* KB begin osekHWosDispatcher (overwritten) */
                  osDispatch()
                  /* KB end osekHWosDispatcher */
               /* KB end osekHWcallosDispatcher */
               return E_OK;
            }
            ELSEWITHCOVER(0x055B)
   #endif /* osdFullPreempt || osdMixedPreempt */
         }
         ELSEWITHCOVER(0x055C)
   #ifndef osdSysAPI
   
         /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
         osRestoreEnableGlobalNested();
         /* KB end osekHWosRestoreEnableGlobalNested */
         #endif   
         /* KB end osekHWosEnableTimProtInterrupt */
   
   #endif
   
   #if osdTimingProtectionUsed
   
      }/*if (osTPArrivalAllowed(taskIndex))*/
      ELSEWITHCOVER(0x055D)
   #endif
   
   #ifndef osdSysAPI
      OS_AT_END_CRITICAL()
   #endif
   
      OS_AT_EXIT()
   #ifndef osdSysAPI
      COVER(0x055E)
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
   #endif
      COVER(0x055F)
      return E_OK;
   
   /* KB end actTaskBody */
}

#ifndef osdTerminateTaskNotUsed
/*-+--------------------------------------------------------------------------
 / TerminateTask
 / =============
 /
 / Syntax:          StatusType TerminateTask ( void )
 / Parameter (In):  none
 / Parameter (Out): none
 / Description:     This service causes the termination of the calling task. 
 /                  The calling task is transferred from the running state 
 /                  into the suspended state.
 / Particularities: An internal resource assigned to the calling task is
 /                  automatically released. Other resources occupied by the 
 /                  task must have been released before the call of 
 /                  TerminateTask. If a resource is still occupied in standard 
 /                  status the behaviour is undefined.
 /                  If the call was successful, TerminateTask does not return 
 /                  to the call level and the status can not be evaluated.
 /                  If the version with extended status is used, the service 
 /                  returns in case of error, and provides a status which 
 /                  can be evaluated in the application.
 /                  If the service TerminateTask is called successfully, 
 /                  it enforces a rescheduling.
 /  -SC1/SC2 only:  Ending a task function without call of TerminateTask or
 /                  ChainTask is strictly forbidden and may leave the system 
 /                  in an undefined state. This error is detected and reported
 /                  by osCAN in extended status.
 /  -SC3/SC4 only:  Ending a task function without call of TerminateTask or
 /                  ChainTask is reported by a call of the ErrorHook, 
 /                  afterwards interrupts are enabled, resources released
 /                  and a task switch is performed like TerminateTask had
 /                  been called.
 / Status:
 /  -Standard:      No return to call level
 /  -Extended:      - Task still occupies resources, E_OS_RESOURCE
 /                  - Call at interrupt level, E_OS_CALLEVEL
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, interrupts enabled, no resources occupied
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/TerminateTask
 */
osqFunc1 StatusType osqFunc2 osTerminateTask (void)
{
   /* CovComment 4:  Function: osTerminateTask */
   OS_TT_ENTRY()
   COVER(0x0560)

#if STATUS_LEVEL == EXTENDED_STATUS
   if (osIntNestingDepth != 0)
   {
      COVER(0x0561)
      #if osdErrorHook 
      COVER(0x0562)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrTTOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x0563)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0564)
 #if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_TerminateTask & osCurrentContext) == 0)
   {
      COVER(0x0565)
      #if osdErrorHook 
      COVER(0x0566)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrTTCallContext);
      #if osdErrorHook 
      COVER(0x0567)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0568)
 #endif /*((osdSC == SC3) || (osdSC == SC4))*/

 #if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
   {
      COVER(0x0569)
 #else
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
   {
      COVER(0x056A)
 #endif
      #if osdErrorHook 
      COVER(0x056B)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_RESOURCE, osdErrTTResourcesOccupied);
      #if osdErrorHook 
      COVER(0x056C)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x056D)

   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x056E)
      #if osdErrorHook 
      COVER(0x056F)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrTTDisabledInterrupts);
      #if osdErrorHook 
      COVER(0x0570)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x0571)
#endif

   COVER(0x0572)
   /* KB begin osekHWosDisableLevel (overwritten) */
   /* disable interrupts up to system level */
   osDisableLevel();
   /* KB end osekHWosDisableLevel */
   OS_TT_START_CRITICAL()

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

#if osdPostTaskHook
   COVER(0x0573)
   /* call PostTaskHook before clearing osRunTaskIndex! */
   osPostTaskHook();
#endif

#if (osdTimingProtectionUsed != 0)
   COVER(0x0574)
   /* Stop execution time measurement/monitoring */
   osTPExeSwitchBack(INVALID_PROCESS);
#endif

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */

#if osdNumberOfInternalResources > 0
   COVER(0x0575)
   /* KB begin osReleaseInternalResource (default) */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
   #if osdRTSize == 1
      COVER(0x0576)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x0577)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] &= /*;*/
         ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x0578)
      osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   #if osdORTIDebug
      COVER(0x0579)
      osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   }
   ELSEWITHCOVER(0x057A)
   
   
   /* KB end osReleaseInternalResource */
#endif

#if ((CC == ECC2) || (CC == BCC2))
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x057B)
   osSysErrAssert(osTcbActivationCount[osActiveTaskIndex], osdErrTTNotActivated)
#endif
   
   COVER(0x057C)
   osTcbActivationCount[osActiveTaskIndex]--;
   
   if (osTcbActivationCount[osActiveTaskIndex] != 0)
   {  
      COVER(0x057D)
      osTcbTaskState[osActiveTaskIndex] = PRE_READY;
   }
   else
   {
      COVER(0x057E)
      osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
   }
#else
   COVER(0x057F)
   osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
#endif

   /* KB begin osQDeleteFromPrio (default) */
   
   #if ((CC == ECC2) || (CC == BCC2))
   /* delete task out of the prio queue */
   osQReadyTaskHead[osActiveTaskPrio]++;
   if (osQReadyTaskHead[osActiveTaskPrio] > oskQMaxActivations[osActiveTaskPrio])
   {
      COVER(0x0580)
      osQReadyTaskHead[osActiveTaskPrio]= 0;
   }
   ELSEWITHCOVER(0x0581)
   
   if (osQReadyTaskHead[osActiveTaskPrio] == osQReadyTaskTail[osActiveTaskPrio])
   {  /* queue empty */
      COVER(0x0582)
   #endif
   
   #if (osdRTSize > 1)
      COVER(0x0583)
      osQReadyPrios[oskTcbReadyPrioOffset[osActiveTaskIndex]]&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #else
      COVER(0x0584)
      osQReadyPrios&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
   }
   ELSEWITHCOVER(0x0585)
   #endif
   
   /* KB end osQDeleteFromPrio */

#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)
   COVER(0x0586)
   /* KB begin osPrepareForStackCorrection (overwritten) */
   #if (osdStackModel == osdSingleStackOsek)
   COVER(0x0587)
   #if (((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))
   COVER(0x0588)
   if(osActiveTaskIndex >= osdNumberOfExtendedTasks)
   #endif
   {
      COVER(0x0589)
      osCurrentBasicSP = (osuint32) osTcbInitialSP[osActiveTaskIndex-osdNumberOfExtendedTasks];
   }
   #endif
   /* KB end osPrepareForStackCorrection */

 #if osdStackCheck
   COVER(0x058A)
   osLastActiveTaskIndex = osActiveTaskIndex;
 #endif
#endif /* (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS) */

   osActiveTaskIndex = osdNoActiveTask;
   osActiveTaskPrio = osdNoActivePrio;

   /* for TerminateTask, osSchedulePrio returns always osdTRUE! */
   /* (same task is never running after TerminateTask) */
   (void) osSchedulePrio(); 

   OS_TT_END_CRITICAL()
   OS_TT_EXIT()

#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)
   /* [SPMF0043:0003] [SPMF0043:0011] */

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

   /* KB begin osekHWosRollBackStack (default) */
   /* KB end osekHWosRollBackStack */

   /* KB begin osekHWAfterOsDispatchInTT (default) */
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
      osDisableGlobal();
      osAssertFailed_(osdErrTTNoImmediateTaskSwitch);
      /* MISRA RULE 14.1 VIOLATION: The return is unreachable because osAssertFailed is 
       * a function that does not return. Anyhow, the return E_OK below is necessary 
       * to prevent a compiler warning  */
      return E_OK; /* PRQA S 3201 *//*lint !e527 see MISRA comment above */
   #else /* if STATUS_LEVEL ... */
      for(;;)
      {
         ;
      }
   #endif /* STATUS_LEVEL ... */
   /* KB end osekHWAfterOsDispatchInTT */

#else /* (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS) */
   /* [SPMF0043:0003] [SPMF0043:0011] */
   return E_OK;
#endif /* (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS) */

} /* END OF osTerminateTask */

#endif


#ifndef osdChainTaskNotUsed
/*-+--------------------------------------------------------------------------
 / ChainTask
 / =========
 /
 / Syntax:          StatusType ChainTask ( TaskType <TaskID> )
 / Parameter (In):  TaskID Reference to the task to be activated after the 
 /                  current task has been terminated.
 / Parameter (Out): none
 / Description:     This service causes the termination of the calling task. 
 /                  After termination of the calling task a succeeding 
 /                  task <TaskID> is activated. Using this service 
 /                  ensures that the succeeding task starts to run 
 /                  earliest after the calling task has been terminated.
 / Particularities: If the succeeding task is identical with the current task, 
 /                  this does not result in multiple requests. The task is not 
 /                  transferred to the suspended state. An internal resource 
 /                  assigned to the calling task is automatically released, 
 /                  even if the succeeding task is identical with the current 
 /                  task. Other resources occupied by the calling task must 
 /                  have been released before ChainTask is called. If a 
 /                  resource is still occupied in standard status the 
 /                  behaviour is undefined.
 /                  If called successfully, ChainTask does not return to the 
 /                  call level and the status can not be evaluated.
 /                  In case of error the service returns to the calling task 
 /                  and provides a status which can then be evaluated in the
 /                  application.
 /                  If the service ChainTask is called successfully, this 
 /                  enforces a rescheduling.
 /                  If E_OS_LIMIT is returned the activation is ignored.
 /                  When an extended task is transferred from suspended state
 /                  into ready state all its events are cleared.
 /  -SC1/SC2 only:  Ending a task function without call of TerminateTask or
 /                  ChainTask is strictly forbidden and may leave the system 
 /                  in an undefined state. This error is detected and reported
 /                  by osCAN in extended status.
 /  -SC3/SC4 only:  Ending a task function without call of TerminateTask or
 /                  ChainTask is reported by a call of the ErrorHook, 
 /                  afterwards interrupts are enabled, resources released
 /                  and a task switch is performed like TerminateTask had
 /                  been called.
 / Status:
 /  -Standard:      - No return to call level
 /                  - Too many task activations of <TaskID>, E_OS_LIMIT
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Calling task still occupies resources, E_OS_RESOURCE
 /                  - Call at interrupt level, E_OS_CALLEVEL
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, interrupts enabled, no resources occupied
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/ChainTask
 */
osqFunc1 StatusType osqFunc2 osChainTask(TaskType taskIndex)
{
   /* CovComment 4:  Function: osChainTask */
   osPrioType priority;
#if (osdTimingProtectionUsed != 0)
   osbool ActivationAllowed;
#endif

   OS_HT_ENTRY()
   COVER(0x058B)

#if STATUS_LEVEL == EXTENDED_STATUS
   if (osIntNestingDepth != 0)
   {
      COVER(0x058C)
      #if osdErrorHook 
      COVER(0x058D)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrHTOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x058E)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x058F)

#if ((osdSC == SC3) || (osdSC == SC4)) 
   /* check call context */
   if ((osdValidCtx_ChainTask & osCurrentContext) == 0)
   {
      COVER(0x0590)
      #if osdErrorHook 
      COVER(0x0591)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrHTCallContext);
      #if osdErrorHook 
      COVER(0x0592)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0593)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/

#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
   {
      COVER(0x0594)
#else
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
   {
      COVER(0x0595)
#endif
      #if osdErrorHook 
      COVER(0x0596)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_RESOURCE, osdErrHTResourcesOccupied);
      #if osdErrorHook 
      COVER(0x0597)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x0598)

   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x0599)
      #if osdErrorHook 
      COVER(0x059A)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_ID, osdErrHTWrongTaskID);
      #if osdErrorHook 
      COVER(0x059B)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x059C)

   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x059D)
      #if osdErrorHook 
      COVER(0x059E)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrHTInterruptsDisabled);
      #if osdErrorHook 
      COVER(0x059F)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x05A0)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplTaskAccess[taskIndex] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x05A1)
      #if osdErrorHook 
      COVER(0x05A2)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_ACCESS, osdErrHTNoAccess);
      #if osdErrorHook 
      COVER(0x05A3)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x05A4)
#endif /*((osdSC == SC3) || (osdSC == SC4))*/
#endif

   COVER(0x05A5)
   /* KB begin osekHWosDisableLevel (overwritten) */
   /* disable interrupts up to system level */
   osDisableLevel();
   /* KB end osekHWosDisableLevel */
   OS_HT_START_CRITICAL()

   if (taskIndex != osActiveTaskIndex)
   {
#if ((CC == ECC2) || (CC == BCC2))
      if (osTcbActivationCount[taskIndex] >= oskTcbAllowedActivations[taskIndex])
      {
         COVER(0x05A6)
#else
      if (osTcbTaskState[taskIndex] != SUSPENDED)
      {
         COVER(0x05A7)
#endif
         osSaveChainTask_TaskID(taskIndex)
         osAPIError(E_OS_LIMIT, osdErrHTMultipleActivation);
         OS_HT_END_CRITICAL()
         OS_HT_EXIT()
         /* KB begin osekHWosEnableLevel (overwritten) */
         /* enable task level interrupt */
         osEnableLevel();   
         /* KB end osekHWosEnableLevel */
         return E_OS_LIMIT;
      }
      ELSEWITHCOVER(0x05A8)
   }
   ELSEWITHCOVER(0x05A9)

   /*************************************************************************
    * terminate old task
    *************************************************************************/

   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   /* call hook routine for the terminated task */
#if osdPostTaskHook
   COVER(0x05AA)
   /* call PostTaskHook before clearing osActiveTaskIndex! */
   osPostTaskHook();
#endif

#if osdNumberOfInternalResources > 0
   COVER(0x05AB)
   /* KB begin osReleaseInternalResource (default) */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
   #if osdRTSize == 1
      COVER(0x05AC)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x05AD)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] &= /*;*/
         ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x05AE)
      osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   #if osdORTIDebug
      COVER(0x05AF)
      osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   }
   ELSEWITHCOVER(0x05B0)
   
   
   /* KB end osReleaseInternalResource */
#endif

#if ((CC == ECC2) || (CC == BCC2))
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x05B1)
   osSysErrAssert(osTcbActivationCount[osActiveTaskIndex], osdErrHTNotActivated)
#endif

   osTcbActivationCount[osActiveTaskIndex]--;

   if (osTcbActivationCount[osActiveTaskIndex] != 0)
   {  
      COVER(0x05B2)
      osTcbTaskState[osActiveTaskIndex] = PRE_READY;
   }
   else
   {
      COVER(0x05B3)
      osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
   }
#else /* ((CC == ECC2) || (CC == BCC2)) */
   COVER(0x05B4)
   osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
#endif /* ((CC == ECC2) || (CC == BCC2)) else */

   COVER(0x05B5)
   /* KB begin osQDeleteFromPrio (default) */
   
   #if ((CC == ECC2) || (CC == BCC2))
   /* delete task out of the prio queue */
   osQReadyTaskHead[osActiveTaskPrio]++;
   if (osQReadyTaskHead[osActiveTaskPrio] > oskQMaxActivations[osActiveTaskPrio])
   {
      COVER(0x05B6)
      osQReadyTaskHead[osActiveTaskPrio]= 0;
   }
   ELSEWITHCOVER(0x05B7)
   
   if (osQReadyTaskHead[osActiveTaskPrio] == osQReadyTaskTail[osActiveTaskPrio])
   {  /* queue empty */
      COVER(0x05B8)
   #endif
   
   #if (osdRTSize > 1)
      COVER(0x05B9)
      osQReadyPrios[oskTcbReadyPrioOffset[osActiveTaskIndex]]&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #else
      COVER(0x05BA)
      osQReadyPrios&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
   }
   ELSEWITHCOVER(0x05BB)
   #endif
   
   /* KB end osQDeleteFromPrio */

#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)

   /* KB begin osPrepareForStackCorrection (overwritten) */
   #if (osdStackModel == osdSingleStackOsek)
   COVER(0x05BC)
   #if (((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))
   COVER(0x05BD)
   if(osActiveTaskIndex >= osdNumberOfExtendedTasks)
   #endif
   {
      COVER(0x05BE)
      osCurrentBasicSP = (osuint32) osTcbInitialSP[osActiveTaskIndex-osdNumberOfExtendedTasks];
   }
   #endif
   /* KB end osPrepareForStackCorrection */

 #if osdStackCheck
   COVER(0x05BF)
   osLastActiveTaskIndex = osActiveTaskIndex;
 #endif
#endif /* (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS) */

#if (osdTimingProtectionUsed != 0)
   if (osActiveTaskIndex == taskIndex)
   {
      COVER(0x05C0)
      /* Stop execution time measurement/monitoring */
      (void)osTPExeSwitchTo(INVALID_PROCESS);
      /* self activation always possible, no new budget, not considered as an arrival */
      ActivationAllowed = osdTRUE;
   }
   else
   {
      COVER(0x05C1)
      /* Stop execution time measurement/monitoring, refresh execution time budget */
      osTPExeSwitchBack(INVALID_PROCESS);
      /* check whether the activation is allowed, considering the time since last activation */
      ActivationAllowed = osTPArrivalAllowed(taskIndex);
   }
#endif

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   osActiveTaskIndex = osdNoActiveTask;
   osActiveTaskPrio = osdNoActivePrio;

   
#if (osdTimingProtectionUsed != 0)
   if (ActivationAllowed == osdTRUE)
   {
      COVER(0x05C2)
#endif

      /*************************************************************************
       * now activate the new task
       *************************************************************************/

#if (STATUS_LEVEL == STANDARD_STATUS)
      #if ((CC == ECC2) || (CC == BCC2))
      if (osTcbActivationCount[taskIndex] < oskTcbAllowedActivations[taskIndex])
      {
         COVER(0x05C3)
      #else
      if (osTcbTaskState[taskIndex] == SUSPENDED)
      {
         COVER(0x05C4)
      #endif
#endif
         priority = oskTcbHomePrio[taskIndex];
         osSysErrAssert((priority < osrNumberOfPriorities), osdErrHTWrongTaskPrio)

#if (osdNumberOfExtendedTasks > 0)
 #if ((osdNumberOfExtendedTasks < osdNumberOfAllTasks) || (osdLib))
         /* with extended tasks clear event field */
         if(taskIndex < osrNumberOfExtendedTasks)
         {
            COVER(0x05C5)
            osTcbSetEventMask[taskIndex]= 0;
         }
         ELSEWITHCOVER(0x05C6)
 #else /* ((osdNumberOfExtendedTasks < osdNumberOfAllTasks) || (osdLib)) */
         COVER(0x05C7)
         /* no basic tasks, so for all tasks clear event field */
         osTcbSetEventMask[taskIndex] = 0;
 #endif /* ((osdNumberOfExtendedTasks < osdNumberOfAllTasks) || (osdLib)) */
#endif /* (osdNumberOfExtendedTasks > 0) */

#if ((CC == ECC2) || (CC == BCC2))
         if (osTcbActivationCount[taskIndex] == 0)
         {
            COVER(0x05C8)
            osTcbTaskState[taskIndex] = PRE_READY;
         }
         ELSEWITHCOVER(0x05C9)
         osTcbActivationCount[taskIndex]++;
#else
         COVER(0x05CA)
         osTcbTaskState[taskIndex] = PRE_READY;
#endif

         COVER(0x05CB)
         /* KB begin osQInsertAtPrio (default) */
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x05CC)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; 
         #else
         COVER(0x05CD)
         osQReadyTask[priority]= taskIndex;
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x05CE)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x05CF)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x05D0)
         osQReadyTaskTail[priority]++;
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])
         {
            COVER(0x05D1)
            osQReadyTaskTail[priority]= 0;
         }
         ELSEWITHCOVER(0x05D2)
         #endif
         
         
         /* KB end osQInsertAtPrio */
#if (STATUS_LEVEL == STANDARD_STATUS)
      }
      ELSEWITHCOVER(0x05D3)
#endif
#if (osdTimingProtectionUsed != 0)

   }/*if (ActivationAllowed == osdTRUE)*/
   ELSEWITHCOVER(0x05D4)
#endif
   COVER(0x05D5)
   (void) osSchedulePrio(); 

   OS_HT_END_CRITICAL()
   OS_HT_EXIT()

#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)
   /* [SPMF0043:0004] [SPMF0043:0011] */

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

   /* KB begin osekHWosRollBackStack (default) */
   /* KB end osekHWosRollBackStack */

   /* KB begin osekHWAfterOsDispatchInHT (default) */
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
      osDisableGlobal();
      osAssertFailed_(osdErrHTNoImmediateTaskSwitch);
      /* MISRA RULE 14.1 VIOLATION: The return is unreachable because osAssertFailed is 
       * a function that does not return. Anyhow, the return E_OK below is necessary 
       * to prevent a compiler warning  */
      return E_OK; /* PRQA S 3201 *//*lint !e527 see MISRA comment above */
   #else /* if STATUS_LEVEL ... */
      for(;;)
      {
         ;
      }
   #endif /* STATUS_LEVEL ... */
   /* KB end osekHWAfterOsDispatchInHT */

#else /* (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS) */
   /* [SPMF0043:0004] [SPMF0043:0011] */
   return E_OK;
#endif /* (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS) */

} /* END OF osChainTask */
#endif /* osdChainTaskNotUsed */

#ifndef osdScheduleNotUsed
/*-+--------------------------------------------------------------------------
 / Schedule
 / ========
 /
 / Syntax:          StatusType Schedule ( void )
 / Parameter (In):  none
 / Parameter (Out): none
 / Description:     If a higher-priority task is ready, the internal resource 
 /                  of the current task is released, the current task is put 
 /                  into the ready state, its context is saved and the 
 /                  higher-priority task is executed.
 /                  Otherwise the calling task is continued.
 / Particularities: Rescheduling can only take place if an internal resource is
 /                  assigned to the calling task during system generation.
 /                  (A non-preemptable task is seen as a task with a resource
 /                  of highest task priority assigned.) For these tasks, Schedule 
 /                  enables a processor assignment to other tasks with lower 
 /                  priority than the ceiling priority of the internal
 /                  resource and higher priority than the priority of the 
 /                  calling task in application-specific locations. 
 /                  When returning from Schedule, the internal resource has 
 /                  been taken again.
 /                  This service has no influence on tasks with no internal 
 /                  resource assigned (preemptable tasks).
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Call at interrupt level, E_OS_CALLEVEL
 /                  - Calling task occupies resources, E_OS_RESOURCE
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, interrupts enabled, no resources occupied
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/Schedule
 */
osqFunc1 StatusType osqFunc2 osSchedule(void)
{
   /* CovComment 4:  Function: osSchedule */

   OS_SH_ENTRY()
   COVER(0x05D6)
#if STATUS_LEVEL == EXTENDED_STATUS
   if (osIntNestingDepth != 0)
   {
      COVER(0x05D7)
      #if osdErrorHook 
      COVER(0x05D8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSHOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x05D9)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x05DA)

#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_Schedule & osCurrentContext) == 0)
   {
      COVER(0x05DB)
      #if osdErrorHook 
      COVER(0x05DC)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSHCallContext);
      #if osdErrorHook 
      COVER(0x05DD)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x05DE)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x05DF)
      #if osdErrorHook 
      COVER(0x05E0)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrSHInterruptsDisabled);
      #if osdErrorHook 
      COVER(0x05E1)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x05E2)
#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
   {
      COVER(0x05E3)
#else
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
   {
      COVER(0x05E4)
#endif
      #if osdErrorHook 
      COVER(0x05E5)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_RESOURCE, osdErrSHResourcesOccupied);
      #if osdErrorHook 
      COVER(0x05E6)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x05E7)

   osSysErrAssert(((oskTcbTaskFlags[osActiveTaskIndex] & osdTaskFlagNotUsingSchedule) == 0U),
       osdErrSHScheduleNotAllowed)   /* NOCOV */     

#endif

   COVER(0x05E8)
   /* KB begin osekHWosDisableLevel (overwritten) */
   /* disable interrupts up to system level */
   osDisableLevel();
   /* KB end osekHWosDisableLevel */
   OS_SH_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

#if osdNumberOfInternalResources > 0
   COVER(0x05E9)
   /* KB begin osReleaseInternalResourceSchedule (default) */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
   #if osdRTSize == 1
      COVER(0x05EA)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x05EB)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] &= /*;*/
         ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   
   #if osdORTIDebug
      COVER(0x05EC)
      osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   }
   ELSEWITHCOVER(0x05ED)
   
   /* KB end osReleaseInternalResourceSchedule */
#endif

   if (osSchedulePrio() != 0)
   {
      COVER(0x05EE)
      OS_SH_END_CRITICAL()
      OS_SH_EXIT()
      /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      #endif   
      /* KB end osekHWosEnableTimProtInterrupt */
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
   ELSEWITHCOVER(0x05EF)

#if osdNumberOfInternalResources > 0
   COVER(0x05F0)
   /* KB begin osGetInternalResourceSchedule (default) */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
   #if osdRTSize == 1
      COVER(0x05F1)
      osQReadyPrios |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x05F2)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] |= /*;*/
         oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   
      /* osHighReadyTaskPrio is modified in osSchedulePrio */
      osHighReadyTaskPrio = oskResInternalCeilingPrio[osActiveTaskIndex];
   
   #if osdORTIDebug
      COVER(0x05F3)
      osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
   #endif
   
   }
   ELSEWITHCOVER(0x05F4)
   /* KB end osGetInternalResourceSchedule */
#endif

   COVER(0x05F5)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SH_END_CRITICAL()
   OS_SH_EXIT()
   /* KB begin osekHWosEnableLevel (overwritten) */
   /* enable task level interrupt */
   osEnableLevel();   
   /* KB end osekHWosEnableLevel */
   return E_OK;
} /* END OF osSchedule */

#endif


#ifndef osdGetTaskIDNotUsed
/*-+--------------------------------------------------------------------------
 / GetTaskID
 / =========
 /
 / Syntax:          StatusType GetTaskID ( TaskRefType <TaskID> )
 / Parameter (In):  none
 / Parameter (Out): TaskID Reference to the task which is currently running
 / Description:     GetTaskID writes the information about the TaskID of the 
 /                  task which is currently running to the location, defined
 /                  by the reference TaskID.
 / Particularities: Allowed on task level, ISR level and in several hook 
 /                  routines.
 /                  This service is intended to be used by library functions 
 /                  and hook routines.
 /                  If <TaskID> can't be evaluated (no task currently 
 /                  running), the service returns INVALID_TASK as TaskID.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - No error, E_OK
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook, PreTaskHook, PostTaskHook,
 /                  ProtectionHook, interrupts not disabled by the 
 /                  application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/GetTaskID
 */
osqFunc1 StatusType osqFunc2 osGetTaskID (TaskRefType TaskId)
{
   /* CovComment 4:  Function: osGetTaskID */

   OS_GI_ENTRY()
   COVER(0x05F6)

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check call context */
   if ((osdValidCtx_GetTaskID & osCurrentContext) == 0)
   {
      COVER(0x05F7)
      #if osdErrorHook 
      COVER(0x05F8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskID_TaskID(TaskId)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrGICallContext);
      #if osdErrorHook 
      COVER(0x05F9)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GI_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x05FA)
   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)TaskId, (osuint32)sizeof(TaskRefType)) == osdFALSE)
      {  /* no writable area found -> no write acces rights for result */
         COVER(0x05FB)
         #if osdErrorHook 
         COVER(0x05FC)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskID_TaskID(TaskId)
         #endif
         osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrGIIllegalAddr);
         #if osdErrorHook 
         COVER(0x05FD)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_GI_EXIT()
         return E_OS_ILLEGAL_ADDRESS;
      }      
      ELSEWITHCOVER(0x05FE)
   }
   ELSEWITHCOVER(0x05FF)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if STATUS_LEVEL == EXTENDED_STATUS

   if (osIntAPIStatus != 0)
   {
      COVER(0x0600)
      #if osdErrorHook 
      COVER(0x0601)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskID_TaskID(TaskId)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrGIIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x0602)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GI_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x0603)

#endif
   COVER(0x0604)
   *TaskId= osActiveTaskIndex;

   OS_GI_EXIT()
   return E_OK;
} /* END OF osGetTaskID */

#endif


#ifndef osdGetTaskStateNotUsed
/*-+--------------------------------------------------------------------------
 / GetTaskState
 / ============
 /
 / Syntax:          StatusType GetTaskState ( TaskType <TaskID>,
 /                                            TaskStateRefType <State> )
 / Parameter (In):  TaskID Task reference
 / Parameter (Out): State Reference to the state of the task <TaskID>
 / Description:     Writes the current state of a task (RUNNING, READY, WAITING, 
 /                  SUSPENDED) to the location, defined by the parameter
 /                  State.
 / Particularities: The service may be called from interrupt service routines, 
 /                  task level, and some hook routines.
 /                  When a call is made from a task in a full preemptive system,
 /                  the result may already be incorrect at the time of 
 /                  evaluation.
 /                  When the service is called for a task, which is multiply
 /                  activated, the state is set to running if any instance 
 /                  of the task is running.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook, PreTaskHook, PostTaskHook,
 /                  interrupts not disabled by the application
/--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/GetTaskState
 */
osqFunc1 StatusType osqFunc2 osGetTaskState(TaskType TaskId, TaskStateRefType State)
{
   /* CovComment 4:  Function: osGetTaskState */

   OS_GS_ENTRY()
   COVER(0x0605)

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check call context */
   if ((osdValidCtx_GetTaskState & osCurrentContext) == 0)
   {
      COVER(0x0606)
      #if osdErrorHook 
      COVER(0x0607)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskState_TaskID(TaskId)
      osSaveGetTaskState_State(State)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrGSCallContext);
      #if osdErrorHook 
      COVER(0x0608)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GS_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0609)

   /* check access rights for this object */
   if (((oskApplTaskAccess[TaskId] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x060A)
      #if osdErrorHook 
      COVER(0x060B)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskState_TaskID(TaskId)
      osSaveGetTaskState_State(State)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGSNoAccess);
      #if osdErrorHook 
      COVER(0x060C)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GS_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x060D)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)State, (osuint32)sizeof(TaskStateType)) == osdFALSE)
      {  /* no writable area found -> no write acces rights for result */
             COVER(0x060E)
             #if osdErrorHook 
             COVER(0x060F)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveGetTaskState_TaskID(TaskId)
             osSaveGetTaskState_State(State)
             #endif
             osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrGSIllegalAddr);
             #if osdErrorHook 
             COVER(0x0610)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
             #endif
             OS_GS_EXIT()
             return E_OS_ILLEGAL_ADDRESS;
       }
       ELSEWITHCOVER(0x0611)
   }
   ELSEWITHCOVER(0x0612)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if STATUS_LEVEL == EXTENDED_STATUS
   if (TaskId >= osrNumberOfAllTasks)
   {
      COVER(0x0613)
      #if osdErrorHook 
      COVER(0x0614)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskState_TaskID(TaskId)
      osSaveGetTaskState_State(State)
      #endif
      osAPIError(E_OS_ID, osdErrGSWrongTaskID);
      #if osdErrorHook 
      COVER(0x0615)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GS_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0616)


   if (osIntAPIStatus != 0)
   {
      COVER(0x0617)
      #if osdErrorHook 
      COVER(0x0618)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskState_TaskID(TaskId)
      osSaveGetTaskState_State(State)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrGSIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x0619)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GS_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x061A)

#endif

   COVER(0x061B)
   *State = osTcbTaskState[TaskId];

   /* change PRE_READY to READY */
   if ((*State) == PRE_READY)
   {
      COVER(0x061C)
      *State = READY;
   }
   ELSEWITHCOVER(0x061D)

   OS_GS_EXIT()
   return E_OK;
} /* END OF osGetTaskState */

#endif

#else
/* KB begin osekHWosOsekTaskDummy (default) */
/* KB end osekHWosOsekTaskDummy */
#endif /* (osdNumberOfAllTasks > 0) */


/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE osektask.c */
