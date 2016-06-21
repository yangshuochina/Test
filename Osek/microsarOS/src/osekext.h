/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekext.h
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.63
|
|  Description: external declarations (only for system internal use)
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* MISRA RULE 19.15 VIOLATION: The QAC-Tool states a violation of rule 19.15 with 
 * message 0883 below. Anyhow, there is a double include prevention and therefore,
 * the message is considered to be wrong. */
/* double include preventer */
#ifndef _OSEKEXT_H      /* PRQA S 0883 EOF */ /* see MISRA comment above */
#define _OSEKEXT_H

/*lint -save Messages inhibited in this file, will be re-enabled at the end of file */
/*lint -e539 Did not expect positive indentation */

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

/* KB begin osekStartOfHModule (default) */
/* KB end osekStartOfHModule */

/* KB begin osekHwDefinitions (overwritten) */

/* KB end osekHwDefinitions */


/* prototypes for OSEK internal functions */
#if ((osdTimingProtectionUsed) || (osdSC== SC3) || (osdSC== SC4))
void osHandleProtectionErrors(ProtectionReturnType proRet, StatusType Fatalerror);
#endif

#if (osdNumberOfAlarms > 0)
osqFunc1 void osqFunc2 osInitAlarms(void);
#if ((osdSC== SC3) || (osdSC== SC4))
osqFunc1 void osqFunc2 osSysCancelAlarm(AlarmType alarmID);
#endif
#endif

#if (osdNumberOfScheduleTables > 0)
#if ((osdSC== SC3) || (osdSC== SC4))
osqFunc1 void osqFunc2 osSysStopScheduleTable(ScheduleTableType ScheduleTableID);
#endif
#endif

osqFunc1 void osqFunc2 osStartOSc(void);

osqFunc1 osbool osqFunc2 osSchedulePrio(void);
#if (osdNumberOfAllTasks > 0)
osqFunc1 StatusType osqFunc2 osSysActivateTask( TaskType taskIndex);
#endif

#if ((osdNumberOfAlarmSetEvent > 0) || (osdNumberOfSTSetEvent > 0))
osqFunc1 StatusType osqFunc2 osSysSetEvent(TaskType taskIndex, EventMaskType setMask);
#endif

#if (osdShutdownHook || (osdNumberOfApplications > 0))
osqFunc1 void osqFunc2 osSysShutdownOS(StatusType Error);
#else
osqFunc1 void osqFunc2 osSysShutdownOS(void);
#endif

#if ((osdSC== SC2) || (osdSC== SC3) || (osdSC== SC4))
ProtectionReturnType osProtectionHookWrapper(StatusType argErr);
#endif

#if osdStackCheck
/* KB begin osekHwosFillTaskStacksPrototype (default) */
/* KB end osekHwosFillTaskStacksPrototype */
#endif

osqFunc1 void osqFunc2 osWorkAlarms(void);

#ifndef osdUseGeneratedFastAlarm
osqFunc1 osuint8 osqFunc2 osRemoveItemMinHeap(osHeapType aItem);
osqFunc1 void osqFunc2 osInsertMinHeap(osHeapType newItem);
#if (osdUseHighResolutionTimer != 0)
osqFunc1 void osqFunc2 osInsertMinHeapWithCheck(osHeapType newItem);
#else
#define osInsertMinHeapWithCheck(x) osInsertMinHeap(x)
#endif
#endif

osqFunc1 void osqFunc2 osInitTimer(void);

#if ((osdSC == SC3) || (osdSC == SC4))
osqFunc1 osbool osqFunc2 osAddressParamValid(osuint32 refAddr, osuint32 dataSize);
osqFunc1 osbool osqFunc2 osApplErrStackAddressParamValid(osuint32 refAddr, osuint32 dataSize);
osqFunc1 void osqFunc2 osSysDisableISRSource(ISRType isrID);
#endif
#if (osdSC == SC3) || (osdSC == SC4)
osqFunc1 void osqFunc2 osSysKillTask(TaskType taskID, osbool issueError);
#endif
#if (((osdSC== SC3) || (osdSC== SC4)) && (osdProtReactKillApplAllowed || osdProtReactKillApplRestAllowed || !defined osdTerminateApplicationNotUsed))
osqFunc1 void osqFunc2 osSysKillApplikation(ApplicationType applToKill, RestartType RestartOption);
#endif


#if (STATUS_LEVEL == EXTENDED_STATUS)
osqFunc1 void osqFunc2 osMissingTerminateError(void);
#endif

/* KB begin osekHwosUnhandledExceptionPrototype (overwritten) */
osqFunc1 void osqFunc2 __indirect osUnhandledException(void);
/* KB end osekHwosUnhandledExceptionPrototype */

#if osdStackCheck
osqFunc1 void osqFunc2 osStackOverflow(void);
#endif

#if (osdNumberOfScheduleTables > 0)
osqFunc1 void osqFunc2 osInitScheduleTables(void);
osqFunc1 void osqFunc2 osWorkScheduleTables(CounterType CounterID);
#endif

#if osdTimingProtectionUsed
/* KB begin osTPTimerPrototypesExt (overwritten) */
osqFunc1 void          osqFunc2 osInitTimingProtection (void)                        ;
osqFunc1 osProcessType osqFunc2 osTPExeSwitchTo      (osProcessType NewProcess)          ;
osqFunc1 void          osqFunc2 osTPExeSwitchBack      (osProcessType OldProcess)          ;
osqFunc1 void          osqFunc2 osTPSuspendTimerBlock  (void)                        ;
osqFunc1 void          osqFunc2 osTPResumeTimerBlock   (osProcessType Proc)          ;
osqFunc1 osbool        osqFunc2 osTPArrivalAllowed     (osProcessType ArrivedProcess);
osqFunc1 void          osqFunc2 osTPCleanListBlock     (osProcessType proc)          ;
osqFunc1 void          osqFunc2 osTPReloadExeBudget    (osProcessType proc)          ;

/* KB end osTPTimerPrototypesExt */
#endif /* osdTimingProtectionUsed */

/* KB begin osekHwPrototypes (overwritten) */
/* implementation specific prototypes */

#if ((osdSC== SC3) || (osdSC== SC4))
void __trap(2) InstructionTrap(void); 
void __trap(1) osProtectionTrap(void);

/* highest index of writable protection areas */
#define osdLastWritableProtectionArea 2

__syscallfunc(0xff) void osRETI2(void);
__syscallfunc(0xfe) void osEnterTrustedCode(void);

#define osLeaveTrustedCode()  osd_mtcr(os_PSW_OFFSET,((osd_mfcr(os_PSW_OFFSET) & 0xffffc3ffUL) | (1 << 12)))

#endif /*(osdSC== ...)*/


/* set interrupt level */
#define osSetTaskLevel osSetLevel( osdTaskLevel )


#define osSetSystemLevel osSetLevel( osdSystemLevel )

#if (osdNumberOfInterruptResources > 0) || (osdTimingProtectionUsed)
/* MISRA RULE 14.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is a inline function. Misra tool cannot deal with this */
static inline osIntLevelType osGetLevel(void) /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */
{
   os_dsync();
   return osd_mfcr(os_ICR_OFFSET) & 0x000000FFUL;
}
#endif

/* disable interrupts up to system level */
#define osDisableLevel() osSetSystemLevel  

/* enable task level interrupt */
#define osEnableLevel() osSetTaskLevel

/* disable all interrupts except timing protection */
#define osDisableHighLevel() osSetLevel( osdHighLevel ) 

/* set alarm interrupt level */
#define osSetAlarmLevel() osSetLevel(osdAlarmLevel)


#if (osdTimingProtectionUsed)
void osRestoreEnableGlobalNested(void);
void osSaveDisableGlobalNested(void);
#endif /*(osdTimingProtectionUsed)*/

/* save level and disable up to system level, no nesting */
extern osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntLevel;
/* MISRA RULE 14.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is a inline function. Misra tool cannot deal with this */
static inline void osSaveDisableLevel(void)  /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */
{
   osIntLevelType intLevel;                                                  
   osuint32 icr;  
   os_dsync();                                                                 
   icr = osd_mfcr(os_ICR_OFFSET);                                             
   intLevel = (osIntLevelType)(icr & 0x000000ffUL);                                           
   icr = (icr & 0xFFFFFF00UL) | osdSystemLevel;  /* PRQA S 3199 */ /* value of icr is used in assembler */
   osd_mtcr(os_ICR_OFFSET,icr); /* the required isync instruction is executed by osd_mtcr */
   osSavedIntLevel = intLevel;                                               
}

/* restore level saved by osSaveDisableLevel */
#define osRestoreEnableLevel() osSetLevel(osSavedIntLevel)

/* enable all interrupts, no nesting */
#define osEnableGlobal() __enable()

extern osqRAM1 volatile osqRAM2 __bit osqRAM3 osSavedGlobalInt;  
#if (osdTimingProtectionUsed)
extern osqRAM1 volatile osqRAM2 __bit osqRAM3 osSavedGlobalIntNested;
#endif

/* enable Dispatcher-Interrupt to trigger the Dispatcher */

#if (osdTC_CORE_VERSION == 0x120)
#define osCPU_SRC0 (*((volatile osuint32*) ((void*)0xFFFEFFFCUL))) /* CPUSRC0 */
#elif (osdTC_CORE_VERSION == 0x130)
#define osCPU_SRC0 (*((volatile osuint32*) ((void*)0xF7E0FFFCUL))) /* CPUSRC0 */
#endif

/*Starts the dispatcher of the OS by requesting an interrupt*/ 
#define osDispatch()                            \
{                                               \
   osCPU_SRC0 |= 0x00008000UL;                  \
   if(osIntNestingDepth==(osuint8)0)            \
   {                                            \
      while((osCPU_SRC0 & 0x00002000UL)!=0)     \
      {                                         \
         ;                                      \
      }                                         \
   }                                            \
}

/*Starts the dispatcher of the OS by requesting an interrupt*/ 
#define osSimpleDispatch()                      \
{                                               \
   osCPU_SRC0 |= 0x00008000UL;                  \
}

void osDispatcher(void);

osbool osCheckInterruptsEnabled(void);
osbool osCheckInterruptsDisabled(void);

#if osdStackCheck
void osSystemStackOverflow(void);   
#endif


void osEnterProtectedISR(osuint16 intID);


#if osdTimingProtectionUsed
#define osdMaxExeTimeValue   0x7FFFFFFFUL
#define osdMaxBlockTimeValue 0x7FFFFFFFUL
#endif

/* KB end osekHwPrototypes */


/* OSEK internal defines */

/* Task flags */
#define osdTaskFlagScheduleAllowed  ((osTaskFlagType)0U)
#define osdTaskFlagNotUsingSchedule ((osTaskFlagType)2U)


/* Preemption Type */
#define osdTaskTypeNonPreempt       ((osTaskPreemptionType) 0U)
#define osdTaskTypePreempt          ((osTaskPreemptionType) 1U)

/* idle definition for task */
#define osdNoActiveTask   INVALID_TASK

/* idle definition for prio */
#define osdNoActivePrio   ((osPrioType)(0xFFFFFFFFUL))

/* idle definition for resource list */
#define osdNoResource   ((ResourceType)(0xFFFFFFFFUL))



/* alarm action */
#define osdAlarmActionSetEvent     ((osuint8)0U)
#define osdAlarmActionActivateTask ((osuint8)1U)
#define osdAlarmActionCallback     ((osuint8)2U)
#define osdAlarmActionIncrementCounter ((osuint8)3U)
/* KB begin osekHWosDiffTickTypeComparison (default) */
typedef union
{
   TickType           unsignedNumber;
   osTickTimeDiffType signedNumber;
}TickTypeTmp;

/*
 * Macro osdIF_COUNTER( min, op, sub ):
 *
 * Do a comparison of counter values according to the operator "op".
 * A counter is considered to cyclically increase its n-bit value.
 * The counter overflows from the value (2^n)-1 to 0. Future values
 * are considered to be bigger than current values. Past values are 
 * smaller. A future value can be reached within 2^(n-1)-1 increments
 * from an current value, this is guaranteed externally.
 */
/* MISRA RULE 19.10 VIOLATION: (few lines below at PRQA-comment 3410) 
 * The second parameter (op) of this macro is meant to be a binary 
 * comparison operator, so it cannot be put into paranteses. This is 
 * no problem as putting in anything but a binary operator must lead 
 * to compile time errors which are easily found in the release tests 
 * of the OS. In case, the binary operator is not a comparison operator, 
 * the MISRA checker would have to find the problem with another message. 
 */
#define osdIF_COUNTER( min, op, sub ) \
{\
   TickTypeTmp osHeapDiffCaster;\
   osHeapDiffCaster.unsignedNumber = (min) - (sub);\
   if( osHeapDiffCaster.signedNumber op ((osTickTimeDiffType) 0))      /* PRQA S 3410 */ /* see MISRA comment above */

#define osdENDIF_COUNTER \
}

/* KB end osekHWosDiffTickTypeComparison */

#if (osdUseHeapAlarm != 0)
   #define osdSystemCounterMask    ((TickType)(0xC0000000UL))
#endif

/* KB begin osekAssignStack (default) */
/* stack grows from high to lower addresses; decrement before push */
#define osAssignStack(stack)    ((osStackDataType*)((&(stack)[0])+(sizeof(stack)/sizeof((stack)[0])))) 
/* KB end osekAssignStack */
/* KB begin osekBottomofStack (default) */
#define osBottomOfStack(stack)  ((osStackDataType*)(stack))
/* KB end osekBottomofStack */


/* KB begin osekHWVariableSectionStartHeader (overwritten) */
/* KB end osekHWVariableSectionStartHeader */

/****************************************************************************/
/* schedule table synchronization */
#define osdSTSyncNon           ((osuint8)0U)
#define osdSTSyncImplicit      ((osuint8)1U) /* "Implicit" and "Non" need to be distinguished in StartScheduleTableRel */
#define osdSTSyncExplicit      ((osuint8)2U)

#if ((osdSC== SC3) || (osdSC== SC4))
extern osqRAM1 osqRAM2 osuint8 osqRAM3  osInSystemFct;
#endif /*((osdSC== SC3) || (osdSC== SC4))*/


#if osdStackCheck
extern osqRAM1 osqRAM2 TaskType   osqRAM3 osLastActiveTaskIndex;
#endif

/* KB begin osekGlobalOccCounterDecl (default) */
#if (STATUS_LEVEL == EXTENDED_STATUS) && ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))
#if ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0)
extern osqRAM1 osqRAM2 osResCounterType osqRAM3 osResGlobalOccupationCounter;
#endif /* ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0) */
#endif /* (STATUS_LEVEL == EXTENDED_STATUS) && ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))) */
/* KB end osekGlobalOccCounterDecl */

/* KB begin osekHwExternDeclLockDispatcher (default) */
#if (osdFullPreempt || osdMixedPreempt)
extern osqRAM1 volatile osqRAM2 osDispatcherLockType osqRAM3 osLockDispatcher;
#endif
/* KB end osekHwExternDeclLockDispatcher */

/* KB begin osekHwExternSavedIntLevelVariables (overwritten) */


/* KB end osekHwExternSavedIntLevelVariables */

#if osdErrorHook
   extern osqBOOL1 osqBOOL2 osbool osqBOOL3 osErrorFlag;
#endif
#if osdComErrorHook
   extern osqBOOL1 osqBOOL2 osbool osqBOOL3 osCOMErrorFlag;
#endif


#if (osdNumberOfAlarms > 0)
   #ifndef osdUseGeneratedFastAlarm
   extern osqRAM1 osqRAM2 osAlarmIndexType osqRAM3 osAlarmHeapCount;    /* Number of heap elements */
   #endif
#endif

#if (osdUseHighResolutionTimer)
 #if (osdMinTimeBetweenTimerIrqs > 0) && (osdAlarmsOnSystemTimer)
   extern osqRAM1 volatile osqRAM2 TickType osqRAM3 osNextAllowedTimerInt;
 #endif
#endif

/* KB begin osekHwExternalDeclarations (overwritten) */
extern osqRAM1 osqRAM2 osuint8 osqRAM3 osIntSaveDisableGlobalCounter; /* extern because of osLeaveISR */
#if ((osdSC== SC3) || (osdSC== SC4))
extern osqRAM1 osqRAM2 osuint32 osqRAM3 osSaved_DPR1_1L;
extern osqRAM1 osqRAM2 osuint32 osqRAM3 osSaved_DPR1_1U;
#endif


#if (osdStackModel == osdSingleStackOsek)
extern osqRAM1 osqRAM2 osuint32 osqRAM3 osCurrentBasicSP;
extern osqRAM1 osqRAM2 osuint8 osqRAM3 osIsOnSystemStack;
#endif

#if (osdUseHighResolutionTimer != 0)
extern osqRAM1 osqRAM2 TickType osqRAM3 osInitialHwCount;
#endif


#if (osdShutdownHook || (osdNumberOfApplications > 0))
extern osqFunc1 void osqFunc2 osSysShutdownOS(StatusType Error);
#else
extern osqFunc1 void osqFunc2 osSysShutdownOS(void);
#endif


#if osdStackCheck
   void osIntStackOverflow(void);
#endif

#if (osdNumberOfInterruptResources > 0)
#if STATUS_LEVEL == EXTENDED_STATUS
extern osIntLevelType osInitialIntLevel;
/* MISRA RULE 14.1 not violated: This is an inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is an inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is an inline function. Misra tool cannot deal with this */
static inline osIntLevelType osGetInitialLevel(void) /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */
{ 
   return (osInitialIntLevel);
}
#endif
#endif

/* MISRA RULE 14.1 not violated: This is an inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is an inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is an inline function. Misra tool cannot deal with this */
static inline void osEnterISR(ISRType argIsrId, osIntLevelType argLevel )  /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */
{ 
   #if (osdORTIDebug && (osdORTIVersion == 200))
      osIsrStoredInfo[osIntNestingDepth].ORTIServiceId = osORTICurrentServiceId;
      osORTICurrentServiceId = osdORTINoService;
   #endif
   #if STATUS_LEVEL == EXTENDED_STATUS
   #if (osdNumberOfInterruptResources > 0)
      osIsrStoredInfo[osIntNestingDepth].InitialIntLevel = osInitialIntLevel;
      osInitialIntLevel = argLevel;
   #endif
   #endif

   #if ((osdSC== SC3) || (osdSC== SC4))
      #if (osdNumberOfMsgCallbackNotifics > 0)
         osIsrStoredInfo[osIntNestingDepth].savedContext = osCurrentContext;
      #endif
      osCurrentContext = osdCtxISRCat2;
      
      /* Save and set protection registers */
      osEnterProtectedISR(argIsrId);
      
      osIsrStoredInfo[osIntNestingDepth].savedApplID = osActiveApplicationID;
      osActiveApplicationID = oskISRToAppl[argIsrId];
      osIsrStoredInfo[osIntNestingDepth].InSystemFct = osInSystemFct;
      osInSystemFct = 0;
   #endif

   #if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
      osIsrStoredInfo[osIntNestingDepth].storedPcxi = osd_mfcr(os_PCXI_OFFSET);
      #if (osdNumberOfCat2ISRs > 0)
         osIcbLockedResource[argIsrId] = osdNoResource;
      #endif
   #endif

   osIsrStoredInfo[osIntNestingDepth].savedISRID = osActiveISRID;

   #if ((osdSC== SC3) || (osdSC== SC4))
      osIsrStoredInfo[osIntNestingDepth].currentISRID = argIsrId;
      osIsrStoredInfo[osIntNestingDepth].isKilled = osdFALSE;
   #endif

   osIntNestingDepth++;

   #if (osdStackModel == osdSingleStackOsek)
      osIsOnSystemStack++;
   #endif

   osActiveISRID = argIsrId;   
}

/* MISRA RULE 14.1 not violated: This is an inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is an inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is an inline function. Misra tool cannot deal with this */
static inline void osLeaveISR(void)  /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */
{ 
   #if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
      osuint32 *usedPtr;
      osuint32 currentPCX;
      osuint32 nextPCX;
      osuint32 currentFCX;
      osuint32 storedPCX;
   #endif

   __disable();
   
#if ((osdSC == SC3) || (osdSC == SC4))
#if (osdNumberOfAllResources > 0)
   if (osIcbLockedResource[osActiveISRID] != osdNoResource)
   {
      COVER(0x0000)
      osAPIError(E_OS_RESOURCE, osdErrIXResourcesOccupied);
      osSysKillResourceList(osIcbLockedResource[osActiveISRID]);
      osIcbLockedResource[osActiveISRID] = osdNoResource;
   }
   ELSEWITHCOVER(0x0001)

#endif /*(osdNumberOfAllResources > 0)*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x0002)
      osAPIError(E_OS_DISABLEDINT, osdErrIXIntAPIDisabled);
      osIntAPIStatus = 0;
      osIntSaveDisableCounter = 0;
      osIntSaveDisableGlobalCounter = 0;
#if (osdTimingProtectionUsed != 0) /*implies osdSC == SC4*/
      COVER(0x0003)
      osIntSaveDisableHighLevelCounter = 0;
#endif
   }
   ELSEWITHCOVER(0x0004)

#endif /*((osdSC == SC3) || (osdSC == SC4))*/

   #if (osdStackModel == osdSingleStackOsek)
      osIsOnSystemStack--;
   #endif

   osIntNestingDepth--;/* decrement once (always) to have correct index to osIsrStoredInfo[] */

   #if ((osdSC== SC3) || (osdSC== SC4))
      /* As long as a killed ISR exists on the next lower nesting level... */
      while ((osIntNestingDepth > 0)
              && (osIsrStoredInfo[osIntNestingDepth-1].isKilled == osdTRUE))
      {
         osIntNestingDepth--;     /* decrement further */

         #if (osdStackModel == osdSingleStackOsek)
            osIsOnSystemStack--;
         #endif
      }

      /*
       * Now either no lower nesting level exists or the ISR on the lower nesting level is
       * not killed so that we have to return to there
       */
   #endif

   #if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
      /* Free the CSAs of killed ISRs */
      /* Get the pointer part of the stored PCXI register value */
      storedPCX = osIsrStoredInfo[osIntNestingDepth].storedPcxi & 0xFFFFFUL;
      /* Get the pointer part of the current PCXI register value */
      currentPCX = osd_mfcr(os_PCXI_OFFSET) & 0xFFFFFUL;
      /* Get the (pointer part) of the current FCX register value */
      currentFCX = osd_mfcr(os_FCX_OFFSET);
      /* remove all contexts until the stored pointer is reached */
      while(currentPCX != storedPCX)
      {
         /* Make a correct pointer from the current PCX-value */
         usedPtr = (osuint32*)(((currentPCX & 0xf0000UL) << 12) | ((currentPCX & 0xffffUL) << 6));
         /* Get the next PCX-value from the link word in the current CSA */
         nextPCX = (*usedPtr) & 0xFFFFFUL;
         /* Link the free context list to the current CSA */
         (*usedPtr) = currentFCX;
         /* Let the free context pointer point to the current CSA */
         currentFCX = currentPCX;
         /* Update the PCX-value to point to the next CSA */
         currentPCX = nextPCX;
      }
      /* Put the new FCX-value into the FCX-register */
      osd_mtcr(os_FCX_OFFSET, currentFCX);

      #if (osdStackModel != osdSingleStackOsek)
         /* Set the call depth counter to zero to prevent from NEST-Trap */
         osd_mtcr(os_PSW_OFFSET, 0x00000A80UL ); 
      #else
         /* Set the call depth counter to one to prevent from NEST-Trap 
          * (consider the additional wrapper-function for this stack model) */
         osd_mtcr(os_PSW_OFFSET, 0x00000A81UL );
      #endif

      /* Get the new PCXI-Value from the osIsrStoredInfo-Array always */
      /* (necessary as the value in the array is modified if the active task is killed) */
      osd_mtcr(os_PCXI_OFFSET, osIsrStoredInfo[osIntNestingDepth].storedPcxi );
      /* the required isync instruction is executed by osd_mtcr */
   #endif

   /* restore OS-variables */
   osActiveISRID = osIsrStoredInfo[osIntNestingDepth].savedISRID;
   #if (osdNumberOfInterruptResources > 0)
   #if STATUS_LEVEL == EXTENDED_STATUS
      osInitialIntLevel = osIsrStoredInfo[osIntNestingDepth].InitialIntLevel;
   #endif
   #endif
   #if (osdORTIDebug && (osdORTIVersion == 200))
      osORTICurrentServiceId = osIsrStoredInfo[osIntNestingDepth].ORTIServiceId;
   #endif

   #if osdStackCheck
      if ( osSystemStack[0] != 0xAAAAAAAAUL )
      {
         osIntStackOverflow();
      }
   #endif
   
   #if ((osdSC== SC3) || (osdSC== SC4))
      osInSystemFct = osIsrStoredInfo[osIntNestingDepth].InSystemFct;
      osActiveApplicationID = osIsrStoredInfo[osIntNestingDepth].savedApplID;

      /* restore protection registers */
      /* private data lower address */
      osd_mtcr(os_DPR1_0L_OFFSET,osIsrStoredInfo[osIntNestingDepth].dpr1_0l);
      /* private data upper address */
      osd_mtcr(os_DPR1_0U_OFFSET,osIsrStoredInfo[osIntNestingDepth].dpr1_0u);
      /* stack lower address */
      osd_mtcr(os_DPR1_1L_OFFSET,osIsrStoredInfo[osIntNestingDepth].dpr1_1l);
      /* stack upper address */
      osd_mtcr(os_DPR1_1U_OFFSET,osIsrStoredInfo[osIntNestingDepth].dpr1_1u);
      /* application data lower address */
      osd_mtcr(os_DPR1_2L_OFFSET,osIsrStoredInfo[osIntNestingDepth].dpr1_2l);
      /* application data upper address */
      osd_mtcr(os_DPR1_2U_OFFSET,osIsrStoredInfo[osIntNestingDepth].dpr1_2u);

      #if (osdNumberOfMsgCallbackNotifics > 0)
         osCurrentContext = osIsrStoredInfo[osIntNestingDepth].savedContext;
      #else
         if (osIntNestingDepth == 0)
         {
            osCurrentContext = osdCtxTask;
         }
         else
         {
            osCurrentContext = osdCtxISRCat2;
         }
      #endif
   #endif
}

static inline osqFunc1 void osqFunc2 osSysDisableInterruptSource (ISRType DisableISR)
{
   (*((volatile osuint32*)((void*)(osSRCtab[DisableISR])))) &= 0xFFFFEFFF;/* clear interrupt enable flag */
}

/* KB end osekHwExternalDeclarations */


#if (osdNumberOfScheduleTables>0)
typedef osuint8 osSTReactionType;
#define osdSTReact_Continue     ((osSTReactionType)0U)
#define osdSTReact_FinalDelay   ((osSTReactionType)1U)
#define osdSTReact_InitialDelay ((osSTReactionType)2U)
#define osdSTReact_Repeat       ((osSTReactionType)3U)
#define osdSTReact_Stop         ((osSTReactionType)4U)


osqFunc1 osSTReactionType osqFunc2 osSTWorkActions(GlobalTimeTickType* diff, 
                                                   osSTIndexType CurrentEP   );
#if (osdScheduleTableSyncUsed != 0)
osqFunc1 TickType osqFunc2 osSTDoSyncStep(ScheduleTableType currentST, 
                                          TickType RelOffs, 
                                          TickType MaxRetard, 
                                          TickType MaxAdvance          );
osqFunc1 void osqFunc2 osSTCalculateSyncState(ScheduleTableType currentST);
#endif

#endif /*if (osdNumberOfScheduleTables>0)*/



/* KB begin osekHWVariableSectionEndHeader (overwritten) */
/* KB end osekHWVariableSectionEndHeader */

/* KB begin osekEndOfHModule (default) */
/* KB end osekEndOfHModule */

/*lint -restore re-enable messages*/

#endif /* _OSEKEXT_H, double include preventer */

/* END OF HEADER osekext.h */
