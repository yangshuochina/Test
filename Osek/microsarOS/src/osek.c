/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osek.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=2.11
|
|  Description: osek startup, scheduler and dispatcher
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

/* CovComment 0:  file: osek.c */

/* KB begin osekHWStartupAdditional (default) */
/* KB end osekHWStartupAdditional */

/* KB begin osekHWOsekAdditionalInclude (default) */
/* KB end osekHWOsekAdditionalInclude */

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

/* KB begin osekHWVariableSectionStart (overwritten) */
/* KB end osekHWVariableSectionStart */

/* control variables */

/* MISRA RULE 8.10 not violated: (7 times) (comment 1504) See below at rule 8.7. */
/* MISRA RULE 8.7 not violated: (4 times) (comment 1514) The variables below are all accessed by 
 * several modules in C and/or assembly language. The MISRA checker sees only the accesses from C language, 
 * which leeds to some messages. For the variables osActiveTaskPrio, osHighReadyTaskPrio, 
 * osHighReadyTaskIndex and osStartDispatcher, the MISRA checker might see only accesses from within the 
 * function StartOSc, dependent on the implementation and configuration. So rules 8.10 and/or 8.7 might seem
 * violated for this variables. The variables osActiveTaskIndex, osIntNestingDepth and osIntSaveDisableCounter
 * are accessed by several functions in this file, so the MISRA checker might see a violation of rule 8.10 only.
 */
osqRAM1           osqRAM2  osPrioType osqRAM3 osActiveTaskPrio;     /* Priority of currently active task */            /* PRQA S 1504 */ /* PRQA S 1514 */
osqRAM1           osqRAM2  osPrioType osqRAM3 osHighReadyTaskPrio;  /* Priority of ready task with highest priority */ /* PRQA S 1504 */ /* PRQA S 1514 */
osqRAM1           osqRAM2  TaskType   osqRAM3 osActiveTaskIndex;    /* indicates the currently running task */         /* PRQA S 1504 */
osqRAM1           osqRAM2  TaskType   osqRAM3 osHighReadyTaskIndex; /* ready task with highest priority */             /* PRQA S 1504 */ /* PRQA S 1514 */

osqBOOL1 volatile osqBOOL2 osbool    osqBOOL3 osStartDispatcher;    /* variable to request a task switch */            /* PRQA S 1504 */ /* PRQA S 1514 */
osqRAM1           osqRAM2  osuint8   osqRAM3  osIntNestingDepth;    /* counter for the nesting depth of interrupts */  /* PRQA S 1504 */
osqRAM1           osqRAM2  osuint8   osqRAM3  osIntSaveDisableCounter;                                                 /* PRQA S 1504 */

#if (osdTimingProtectionUsed)
/* MISRA RULE 8.10 VIOLATION: On some platforms, this variable could also be static. Catching all cases would 
   make the code too complex and error prone */
osqRAM1 osqRAM2 osuint8 osqRAM3 osIntSaveDisableHighLevelCounter; /* PRQA S 1504 */
#endif


#if ((osdSC== SC3) || (osdSC== SC4))

osqRAM1 osqRAM2 osuint16 osqRAM3 osCurrentContext;    

#if (osdNumberOfApplications > 0)
osqRAM1 osqRAM2 ApplicationType osqRAM3 osActiveApplicationID;    
osqBOOL1 volatile osqBOOL2 osbool osqBOOL3 osInApplHook;
#endif
osqRAM1 osqRAM2 osuint8 osqRAM3  osInSystemFct;

#endif /*((osdSC== SC3) || (osdSC== SC4))*/

/* MISRA RULE 8.7 VIOLATION: (1514) See below at the 8.10 violation*/
/* MISRA RULE 8.10 VIOLATION: (1504) The variable osActiveISRID is accessed from within the function StartOSc 
 * (initialization) below (always), function osHandleProtectionErrors below (platform and configuration dependent), 
 * the cat2 ISR-wrappers (configuration dependent) and the API-macro GetISRID (application dependent). Therefore, 
 * a MISRA checker might see a violation of rules 8.7 or 8.10. Anyhow, the variable needs to be extern because the
 * macro GetISRID might be used anywhere in the application code. 
 */
/* Id of the currently running (cat2) ISR */
osqRAM1 osqRAM2 ISRType osqRAM3 osActiveISRID;     /* PRQA S 1514 */ /* PRQA S 1504 */

#if osdStackCheck
/* MISRA RULE 8.7 not violated: (1514) See below at the rule 8.10 comment. */
/* MISRA RULE 8.10 not violated: (1504) The variable osLastActiveTaskIndex is accessed by 
 * several modules. On some platforms the analysis tool sees accesses by only one 
 * function (rule 8.7) or one module (rule 8.10) in C language but misses accesses from 
 * assembly language.
 */
osqRAM1 osqRAM2 TaskType   osqRAM3 osLastActiveTaskIndex;    /* PRQA S 1514 */ /* PRQA S 1504*/
#endif

#ifdef osdEnableCoverage
osqRAM1 osqRAM2 osuint8 osqRAM3 osCoverage[osdCoverageArraySize];
#endif

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
#ifndef osdSuppressFilenames 
/* variables for error handling */
osqRAM1 osqRAM2 osLineNumberType osqRAM3 osActiveTaskLineNumber;

/* MISRA RULE 6.3 VIOLATION: The variable osActiveTaskModule is used to store the name of the 
 * file where the last OS-API-service has been called. This is performed by usage of the 
 * __FILE__ macro. Unfortunately some compilers warn if the type is not a character pointer. 
 * Usage of char* is save here as no computation on the value is intended. 
 */
osqPROM1 const osqPROM2 char osqPROM3 * osqPRAM3 osActiveTaskModule;   /* PRQA S 5013 */
#endif
osqRAM1 volatile osqRAM2 osuint8 osqRAM3 osIntAPIStatus;
#endif

#if (STATUS_LEVEL == EXTENDED_STATUS) && ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))
 #if ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0)
osqRAM1 osqRAM2 osResCounterType osqRAM3 osResGlobalOccupationCounter;
 #endif
#endif

#if osdSystemCounterNeeded
#if (osdUseCounterAPI==0)
osqRAM1 volatile osqRAM2 TickType osqRAM3 osSystemCounter;
#endif
#endif

/* KB begin osekHwDeclLockDispatcher (default) */
#if (osdFullPreempt || osdMixedPreempt)
osqRAM1 volatile osqRAM2 osDispatcherLockType osqRAM3 osLockDispatcher;
#endif
/* KB end osekHwDeclLockDispatcher */

/* KB begin osekHwSavedIntLevelVariables (overwritten) */
osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntLevel;

/* MISRA RULE 8.7 VIOLATION: in some very rare configuration, the following variable might be accessed by one function only.
   Catching all these cases is too complex and therefore error prone.
 */
osqRAM1 static osqRAM2 osIntLevelType osqRAM3 osSavedIntLevelNested; /* PRQA S 3218 */

#if (osdTimingProtectionUsed)
osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntHighLevel;

/* MISRA RULE 8.7 VIOLATION: In some very rare configurations, the following variable is accessed by 
   only one function. Catching all these cases would be complex to catch and therefore error prone */
osqRAM1 static osqRAM2 osIntLevelType osqRAM3 osSavedIntHighLevelNested; /* PRQA S 3218 */
#endif
/* KB end osekHwSavedIntLevelVariables */

#if osdErrorHook
osqBOOL1 osqBOOL2 osbool osqBOOL3 osErrorFlag;
#endif
#if osdComErrorHook
osqBOOL1 osqBOOL2 osbool osqBOOL3 osCOMErrorFlag;
#endif

#if (osdNumberOfAlarms > 0)
   #ifndef osdUseGeneratedFastAlarm
   osqRAM1 osqRAM2 osAlarmIndexType osqRAM3 osAlarmHeapCount;    /* Number of heap elements */
   #endif
#endif

#if osdORTIDebug
osqRAM1 volatile osqRAM2 osuint8 osqRAM3 osORTICurrentServiceId;
#endif

osqRAM1 osqRAM2 osuint8 osqRAM3 osActiveAppMode;

#if (osdParameterAccessMacros && osdErrorHook) || (osdComUseParameterAccess && osdComErrorHook)
osqRAM1 osqRAM2 osAPIParam1Type osqRAM3 osAPIParam1;
osqRAM1 osqRAM2 osAPIParam2Type osqRAM3 osAPIParam2;
osqRAM1 osqRAM2 osAPIParam3Type osqRAM3 osAPIParam3;
osqPRAM1 osuint32 osqPRAM2 * osqPRAM3 osAPIParam4;
#endif


#if ( (osdErrorHook!=0) || (osdComErrorHook!=0) )
/* KB begin osekGlobalErrorVariables (default) */
#if osdORTIDebug
osqRAM1 osqRAM2 StatusType osqRAM3 osLastErrorCode;
#endif
osqRAM1 osqRAM2 osuint16 osqRAM3 osLastError;
/* KB end osekGlobalErrorVariables */
#endif

/* KB begin dummyVariables (default) */
/* KB end dummyVariables */

/* KB begin osekHWVariableDefinition (overwritten) */
/* MISRA RULE 8.10 VIOLATION: This variable might not be used or used only once 
   in some rare configurations. Catching all these configutation would be too complex. */
osqRAM1 volatile osqRAM2 __bit osqRAM3 osSavedGlobalInt; /* PRQA S 3207 */  /* PRQA S 1504 */
/* MISRA RULE 8.7 VIOLATION: (2 times) in some rare configurations, this variable may be 
   referenced by only one function. Catching all these configutation would be too complex. */
#if (osdTimingProtectionUsed) 
osqRAM1        volatile osqRAM2 __bit osqRAM3 osSavedGlobalIntNested; /* PRQA S 3218 */
#else
osqRAM1 static volatile osqRAM2 __bit osqRAM3 osSavedGlobalIntNested; /* PRQA S 3218 */
#endif

#if (osdUseHighResolutionTimer != 0)
osqRAM1 osqRAM2 TickType osqRAM3 osInitialHwCount;
#endif

osqRAM1 osqRAM2 osuint8 osqRAM3 osIntSaveDisableGlobalCounter; /* extern because of osLeaveISR */

#if ( STATUS_LEVEL == EXTENDED_STATUS )

#if (osdNumberOfInterruptResources > 0)
osIntLevelType osInitialIntLevel;
#endif
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
/* Variables to save protection registers in the osErrorHook */
osqRAM1 osqRAM2 osuint32 osqRAM3 osSaved_DPR1_1L;
osqRAM1 osqRAM2 osuint32 osqRAM3 osSaved_DPR1_1U;
#endif

#if (osdStackModel == osdSingleStackOsek)
osqRAM1 osqRAM2 osuint32 osqRAM3 osCurrentBasicSP;
osqRAM1 osqRAM2 osuint8 osqRAM3 osIsOnSystemStack;
#endif


/*declare this variables extern, as they are not used in the c-code*/
__asm( "   .extern oskTcbTaskStartAddress" );
__asm( "   .extern osTcbTaskState" );

#if ((osdStackModel==osdMultiStack) ||\
     (osdStackModel==osdSingleStackOsek))

/* MISRA RULE 8.7 not violated: variable is accessed from assembler code, too */
static osqRAM1 volatile osqRAM2 osuint32 osqRAM3 osInitialPcxiVal; /* PRQA S 3218 */

__asm( "   .extern osTcbPCXI" );
__asm( "   .extern oskTcbStackTop" );

#if osdStackCheck

__asm( "   .extern oskTcbStackBottom" );

#endif /*osdStackCheck*/

#endif /*osdStackModel...*/


#if ( STATUS_LEVEL == EXTENDED_STATUS )
__asm( "   .extern osMissingTerminateError" );
#endif

#if osdMixedPreempt
__asm( "   .extern oskTcbTaskPreemption" );
#endif

static void osDispatcherActivator(void);

/* KB end osekHWVariableDefinition */

/* KB begin osekHWVariableSectionEnd (overwritten) */
/* KB end osekHWVariableSectionEnd */

/* KB begin osekConstSectionCodeStart (default) */
/* KB end osekConstSectionCodeStart */


/* MISRA RULE 8.7 violated: This constant is to be included into the binary to identify the configuration. 
 * It is not used by internal code, instead it is meant for usage by external tools. So it exists without a reference. 
 */
osqROM0 osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant=
{
   0xAF, /* magic number */
   0xFE,
   0xDE,
   0xAD,
   (osuint8)osdVrmMajRelNumRef, /* version of operating system, Major, defined in vrm.h */
   (osuint8)osdVrmMinRelNumRef, /* version of operating system, Minor */

   (osuint8)osdVrmGenMajRelNumRef, /* version of code generator */
   (osuint8)osdVrmGenMinRelNumRef,

   /* variant used (part 1) */
   /* Bit 0..1:   Conformance Class */
   /* Bit 2:      Status level */
   /* Bit 3..4:   Scheduling policy */
   /* Bit 5:      Stackcheck */
   /* Bit 6:      API errors with file names */
   /* Bit 7:      EnableInternalAssertions */

   (osuint8)(((osdVariantCC)                    ) |
             ((osdVariantStatus)            << 2) |
             ((osdVariantSchedule)          << 3) |
             ((osdVariantStackcheck)        << 5) |
             ((osdVariantModulnamesEnabled) << 6) |
             ((osdVariantAssertionsEnabled) << 7)),

   /* variant used (part 2) */
   /* Bit 0..1:   Scalability Class */
   /* Bit 2:      Schedule table(s) used (Not used in library version) */
   /* Bit 3:      High resolution schedule table(s) used (Not used in library version) */
   /* Bit 4:      Schedule table synchronization used*/
   /* Bit 5:      timing protection used */
   /* Bit 6:      Not yet used, always zero */
   /* Bit 7:      Not yet used, always zero */
   (osuint8)(((osdVariantSC)                    ) |
             ((osdVariantFeaturesUsed_ST)   << 2) |
             ((osdVariantFeaturesUsed_HRST) << 3) |
             ((osdVariantFeaturesUsed_Sync) << 4) |
             ((osdVariantFeaturesUsed_TP)   << 5)),
   /* KB begin osekStructSpecVersionVariant (overwritten) */
      (osuint8) osdVariantSystemIntLevel,
      (osuint8) (( osdVariantDerivative << 2 )|( osdVariantORTI << 0 ))
   /* KB end osekStructSpecVersionVariant */

};
/* KB begin osekConstSectionCodeEnd (default) */
/* KB end osekConstSectionCodeEnd */

/* KB begin osekHWosGetHighPrioBit (overwritten) */
/*-+--------------------------------------------------------------------------
 / osGetHighPrioBit
 / Funktion:
 /   - calculates index of first set (priority) bit
 / Parameter:     uiReadyTasks   bit field to search
 / Returnvalue:   index of first set bit (0 = MSB)
 /--------------------------------------------------------------------------*/
#define osGetHighPrioBit(uiReadyTasks) ((osPrioType )osd_clz( uiReadyTasks ))

/* KB end osekHWosGetHighPrioBit */

#if osdStackCheck

 #if osdStackUsageMeasurement
/* KB begin osekHWosFillSystemStackSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osFillInterruptStack
 / Funktion: fills Interrupt / system stack with pattern
 /   - 
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static void osFillInterruptStack(void);
static void osFillInterruptStack(void)
{
   /* CovComment 4:  Function: osFillInterruptStack */
   osStackSizeType i;
   osStackSizeType size=osdSystemStackSize/(osStackSizeType)sizeof(osStackDataType);
   COVER(0x0005)
   for( i=0U; i<size; i++ )
   {
      COVER(0x0006)
      osSystemStack[i] = 0xAAAAAAAAUL;
   }
} /* END OF osFillInterruptStack */

/* KB end osekHWosFillSystemStackSrc */
 #else /* osdStackUsageMeasurement */
/* KB begin osekHWosInitSystemStackSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osInitInterruptStack
 / Funktion: fills Interrupt / system stack with pattern
 /   - 
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

void osInitInterruptStack(void)
{
   /* CovComment 4:  Function: osInitInterruptStack */
   COVER(0x0007)
   osSystemStack[0] = 0xAAAAAAAAUL;
} /* END OF osFillInterruptStack */

/* KB end osekHWosInitSystemStackSrc */
 #endif /* osdStackUsageMeasurement */

 #if (osdStackModel != osdSingleStackOptimized)
  #if (osdNumberOfAllTasks > 0)
   #if osdStackUsageMeasurement
/* KB begin osekHwosFillTaskStacksSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osFillStacks
 / Funktion:
 /   - fills all stacks with a certain pattern (0xAA)
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if (osdStackModel==osdMultiStack)
#define osdNumberOfTasksWithTaskStack osdNumberOfAllTasks
#elif (osdStackModel == osdSingleStackOptimized)
#define osdNumberOfTasksWithTaskStack 0
#elif (osdStackModel == osdSingleStackOptimizedCS)
#define osdNumberOfTasksWithTaskStack 0
#elif (osdStackModel == osdSingleStackOsek)
#define osdNumberOfTasksWithTaskStack osdNumberOfExtendedTasks
#endif
 
static osqFunc1 void osqFunc2 osFillTaskStacks(void);
static osqFunc1 void osqFunc2 osFillTaskStacks(void)
{
   /* CovComment 4:  Function: osFillTaskStacks */
#if (osdNumberOfTasksWithTaskStack > 0)
   osqPRAM1 osqPRAM2 osuint32 osqPRAM3 *stack;
   osTaskIndexType n;
#endif /*(osdNumberOfTasksWithTaskStack > 0)*/

   COVER(0x0008)
   /* fill task stacks */
#if (osdNumberOfTasksWithTaskStack > 0)
   for (n= 0; n < osdNumberOfTasksWithTaskStack; n++)
   {
      COVER(0x0009)
      stack = (osqPRAM1 osqPRAM2 osuint32 osqPRAM3 *)(oskTcbStackTop[n]);
      if (stack != osdNULL)
      {
         COVER(0x000A)
         while (stack > (osuint32*)oskTcbStackBottom[n])
         {
            COVER(0x000B)
            stack--;
            *stack= 0xAAAAaaaaUL;
         }
      }
   }
#endif /*(osdNumberOfTasksWithTaskStack > 0)*/

} /* END OF osFillTaskStacks */
/* KB end osekHwosFillTaskStacksSrc */
   #else /* osdStackUsageMeasurement */
/* KB begin osekHWosInitTaskStacksSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osInitTaskStacks
 / Funktion:
 /   - init the lowest byte of all task stacks with a certain pattern (0xAA)
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if (osdStackModel==osdMultiStack)
#define osdNumberOfTasksWithTaskStack osdNumberOfAllTasks
#elif (osdStackModel == osdSingleStackOptimized)
#define osdNumberOfTasksWithTaskStack 0
#elif (osdStackModel == osdSingleStackOptimizedCS)
#define osdNumberOfTasksWithTaskStack 0
#elif (osdStackModel == osdSingleStackOsek)
#define osdNumberOfTasksWithTaskStack osdNumberOfExtendedTasks
#endif
 
static osqFunc1 void osqFunc2 osInitTaskStacks(void);
static osqFunc1 void osqFunc2 osInitTaskStacks(void)
{
/*prevent warning about pointless comparison in the header of the for-loop*/
#if (osdNumberOfTasksWithTaskStack > 0)

   osTaskIndexType n;

   /* fill task stacks */
   for (n= 0; n < osdNumberOfTasksWithTaskStack; n++)
   {
      *((osuint32*)(oskTcbStackBottom[n])) = 0xaaaaAAAAUL;
   }
#endif

} /* END OF osInitTaskStacks */
/* KB end osekHWosInitTaskStacksSrc */
   #endif /* osdStackUsageMeasurement */
  #endif /* (osdNumberOfAllTasks > 0) */
 #endif /* (osdStackModel != osdSingleStackOptimized) */

#endif /* osdStackCheck && (osdStackModel != osdSingleStackOptimizedCS) */

/*-+--------------------------------------------------------------------------
 / osSchedulePrio
 / Funktion:
 /   - calculate ready task with highest priority
 /   - set osHighReadyTaskPrio and osHighReadyTaskIndex for ready task with highest
 /     priority
 /   - osLockDispatcher must be unlocked (0)
 / Parameter:
 / Returnvalue: osdTRUE, if task with highest priority != active task or no task ready
 /--------------------------------------------------------------------------*/
/* MISRA RULE 14.1 VIOLATION: The function osSchedulePrio might be not called 
 *  in some configurations. Conditional compiling of the function 
 *  would increase the code complexity to unacceptable level (1503).
 */
osqFunc1 osbool osqFunc2 osSchedulePrio(void) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osSchedulePrio */
   /* calculate ready task with highest priority */
   osPrioType hiPriorTask;

#if osdRTSize > 1
   osqPRAM1 osqPRAM2 osPrioFlagType osqPRAM3 *ptr;
   osPrioFlagType uiTestPattern;
#endif

   COVER(0x000C)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x000D)
   osSysErrAssert((osCheckInterruptsDisabled() != 0), osdErrSPInterruptsEnabled)
#endif

#if osdRTSize > 1
   COVER(0x000E)
   ptr= osQReadyPrios;
   uiTestPattern= *ptr;
   hiPriorTask= 0;
   while (!uiTestPattern)
   {
      ptr++;
      if (ptr > &osQReadyPrios[osrRTSize-1])
      {  /* prepare for IDLE-Loop */
         COVER(0x000F)
         osHighReadyTaskPrio = osdNoActivePrio;
         osHighReadyTaskIndex = osdNoActiveTask;
         return osdTRUE;
      }
      ELSEWITHCOVER(0x0010)
      uiTestPattern = *ptr;
      hiPriorTask += osdPrioBitsPerWord;
   }
   COVER(0x0011)
   hiPriorTask += osGetHighPrioBit(uiTestPattern);
#else
   if (osQReadyPrios == 0)
   {  /* prepare for IDLE-Loop */
      COVER(0x0012)
      osHighReadyTaskPrio = osdNoActivePrio;
      osHighReadyTaskIndex = osdNoActiveTask;
      return osdTRUE;
   }
   ELSEWITHCOVER(0x0013)
   hiPriorTask= osGetHighPrioBit(osQReadyPrios);
#endif

#if ((CC == BCC2) || (CC == ECC2))
   COVER(0x0014)
   osHighReadyTaskIndex  = oskQActivationQueues[hiPriorTask][osQReadyTaskHead[hiPriorTask]];
#else
   COVER(0x0015)
   osHighReadyTaskIndex  = osQReadyTask[hiPriorTask];
#endif

   osHighReadyTaskPrio = hiPriorTask;

   /* KB begin osekHWosSchedulePrioReaction (default) */
      if (osHighReadyTaskIndex != osActiveTaskIndex)
      {
         COVER(0x0016)
         return osdTRUE;
      }
      else
      {
         COVER(0x0017)
         return osdFALSE;
      }
   /* KB end osekHWosSchedulePrioReaction */
} /* END OF osSchedulePrio */


/*-+--------------------------------------------------------------------------
 / osInitTCBs
 / Funktion:
 /   - inits all tasks as SUSPENDED
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if (osdNumberOfAllTasks>0)
static osqFunc1 void osqFunc2 osInitTCBs(void);
static osqFunc1 void osqFunc2 osInitTCBs(void)
{
   /* CovComment 4:  Function: osInitTCBs */
   osPrioType np;

#if ((CC==BCC2) || (CC==ECC2) || osdORTIDebug)
   osTaskIndexType nt;

   COVER(0x0018)
   for (nt=0; nt<osrNumberOfAllTasks; nt++)
   {
      COVER(0x0019)
      #if (CC==BCC2) || (CC==ECC2)
         COVER(0x001A)
         osTcbActivationCount[nt]= 0;
      #endif

      #if osdORTIDebug
      COVER(0x001B)
      osTcbActualPrio[nt] = oskTcbHomePrio[nt];
      osTcbORTIServiceId[nt] = osdORTINoService;
      #endif
   }
#endif /* ((CC==BCC2) || (CC==ECC2) || osdORTIDebug) */

#if (osdRTSize > 1)
   COVER(0x001C)
   for (np=0; np<osrRTSize; np++)
   {
      COVER(0x001D)
      osQReadyPrios[np] = 0;
   }
#else
   COVER(0x001E)
   osQReadyPrios = 0;
#endif

   /* all activation queues are empty */
   for (np= 0; np<osrNumberOfPriorities; np++)
   {
   #if (CC==BCC2) || (CC==ECC2)
      COVER(0x001F)
      osQReadyTaskHead[np]= osQReadyTaskTail[np]= 0;
   #else
      COVER(0x0020)
      osQReadyTask[np]= osdNoActiveTask;
   #endif
   }

   /* KB begin osekHWosInitTcbExtension (default) */
   /* KB end osekHWosInitTcbExtension */
} /* END OF osInitTCBs */

#endif

#if osdLib

/*-+--------------------------------------------------------------------------
 / osCheckLibraryVersionAndVariant
 / Funktion:
 /   - checks if version of library and the variant coding are the same
 /     as the user selected in the OIL configurator
 /   - if the variant differs, the system is shut down
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osCheckLibraryVersionAndVariant(void)
{
   /* CovComment 4:  Function: osCheckLibraryVersionAndVariant */
   osqPROM1 osqPROM2 osuint8 osqPROM3 * ptr1;
   osqPROM1 osqPROM2 osuint8 osqPROM3 * ptr2;
   osuint8 n;

   COVER(0x0021)
   /* defined in tcb.c, generated */
   ptr1= (osqPROM1 osqPROM2 osuint8 osqPROM3 *) &oskLibVersionVariant; 
   /* defined in osek.c, always visible */
   ptr2= (osqPROM1 osqPROM2 osuint8 osqPROM3 * ) &oskVersionVariant;    
   
   for (n= 0; n < sizeof(osVersionVariantCodingType); n++)
   {
      if (*ptr1++ != *ptr2++)
      {
         COVER(0x0022)
         osAbortSystem(osdErrCLWrongLibrary);
      }
      ELSEWITHCOVER(0x0023)
   }
} /* END OF osCheckLibraryVersionAndVariant */

#endif


/* KB begin osekHWosStartOSasmSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osStartOSasm
 / Funktion: called at OS startup
 /   - 
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
void osStartOSasm(void)
{
   /* CovComment 4:  Function: osStartOSasm  */
   osuint32 LocalIcrCopy;
   osuint32 tmp;

   os_dsync();

   /* disable global interrupt flag and set system level */
   /* modification of the register ICR by an osd_mtcr(); instruction */
   LocalIcrCopy = osd_mfcr(os_ICR_OFFSET);
   /*reset number of arbitration cycles, interrupt enable flag and current interrupt level*/
   LocalIcrCopy &= 0xFCFF0000UL;
   /*set current interrupt level to system level*/
   LocalIcrCopy |= (osuint32)osdSystemLevel; /* PRQA S 3199 */ /* variable used by assembler */
   #if (osdInterruptVectorTableSize == 3)
      LocalIcrCopy |= 0x03000000UL; /*one interrupt arbitration cycle*/ /* PRQA S 3199 */ /* variable used by assembler */
   #elif (osdInterruptVectorTableSize == 15)
      LocalIcrCopy |= 0x02000000UL; /*two interrupt arbitration cycles*/ /* PRQA S 3199 */ /* variable used by assembler */
   #elif (osdInterruptVectorTableSize == 63)
      LocalIcrCopy |= 0x01000000UL; /*three interrupt arbitration cycles*/ /* PRQA S 3199 */ /* variable used by assembler */
   #elif (osdInterruptVectorTableSize == 255)
      /*four interrupt arbitration cycles*/
   #else
      #error "wrongly defined osdInterruptVectorTableSize"
      /*just double checking osdInterruptVectorTableSize is one of the above values*/
   #endif
   osd_mtcr(os_ICR_OFFSET,LocalIcrCopy);
   /* the required isync instruction is executed by osd_mtcr */

   #if osdStackCheck && (osdStackModel != osdSingleStackOptimizedCS)
   #if osdStackUsageMeasurement
      /* fill interrupt stack with 0xAA pattern */
      osFillInterruptStack();
   #else
      /* fill last word of interrupt stack with 0xAA pattern */
      osInitInterruptStack();     
   #endif
   #endif

   /* switch to system stack */
   tmp = (osuint32) &osSystemStack[ osdSystemStackSize/(osStackSizeType)sizeof(osStackDataType) ]; /* PRQA S 3683 */ /* index is intentionally one larger than array */
   tmp = tmp;/*prevent lint Warning 550 Symbol 'tmp' not accessed*/ /* PRQA S 3199 */ /* Variable used by assembler */
   __asm( "   mov.a  a10, %0 " :: "d"(tmp): "a10" );
   os_dsync();

#if (osdStackModel != osdSingleStackOsek)
   /*prevent from switching to system stack (system runs already on system stack)*/
   osd_mtcr(os_PSW_OFFSET, 0x00000A80UL );
   /*
    * Protection register set 0
    * IO-privilege supervisor
    * On interrupt stack
    * Global register write disabled
    * Call depth counting enabled
    * 6 bit call depth counter
    */
#else
   /*switch to Cat1IsrStack on interrupt entry*/
   osd_mtcr(os_PSW_OFFSET, 0x00000880UL );
   /*
    * Protection register set 0
    * IO-privilege supervisor
    * Not on interrupt stack
    * Global register write disabled
    * Call depth counting enabled
    * 6 bit call depth counter
    */
#endif

   /* free CSA list */
   os_dsync();


   /*
    *  Note: The rslcx-instruction below will destroy register values in lower context.
    *        So from here on, the function is in assembler language, to ensure,
    *        no destroyed registers are used afterwards.
    */
   __asm
   (
/*    while( ( (pcxiraw=osd_mfcr(os_PCXI_OFFSET)) & 0x000FFFFFUL )!=0UL ) */
      "   j osStartOSasm_2\n"
      "\n"
      "osStartOSasm_1:\n"
/*    { */
         /* modification of CSA lists */

/*       osd_mtcr(os_PCXI_OFFSET,pcxiraw&~0x00400000UL); */
         "   mov.u d15,#65535\n"
         "   addih d15,d15,#65471\n"
         "   and   d0,d15\n"
         "   mtcr  #65024,d0\n"

/*       __isync(); */
         "   isync\n"

/*       __rslcx(); */
         "   rslcx\n"
         "   nop      ; CPU_TC.069 workaround\n"

/*    } */
      "\n"
      "osStartOSasm_2:\n"
      "   mfcr  d0,#65024\n" /*d0 = osd_mfcr(os_PCXI_OFFSET)*/
      "   mov.u d15,#65535\n"
      "   addih d15,d15,#15\n"/*d15 = 0x000FFFFF*/
      "   and   d15,d0\n"
      "   jnz   d15,osStartOSasm_1\n"

      /*
       * set PCXI = 0, make sure, the saved PCXI in the oldest CSA is allways 0
       */
/*    osd_mtcr(os_PCXI_OFFSET,0x00000000UL); */
      "   mov16 d15,#0\n"
      "   mtcr  #65024,d15\n"
   );

   #if ( STATUS_LEVEL == EXTENDED_STATUS )
    #if ((osdStackModel==osdMultiStack) || (osdStackModel==osdSingleStackOsek))


      /* one extra csa for return from task, necessary for detection of missing TerminateTask() */
      __asm
      (
         "   mov16.aa a2, a10\n"     /* A2 = SP (A2 in lower context has same position as A10 (SP) in upper context) */

/*       __svlcx(); */
         "   svlcx\n"
/*       osInitialPcxiVal =osd_mfcr(os_PCXI_OFFSET); */
         "   mfcr    d0,#65024\n"    /* D0 = PCXI */

/*       pcxiadr = ((osInitialPcxiVal & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)); */
         "   movh    d1,#15\n"       /* D1  = 0x000f0000 */
         "   and     d15,d1,d0\n"    /* D15 = D1 & D0    */
         "   mov.u   d1,#65535\n"    /* D1  = 0x0000ffff */
         "   and     d2,d0,d1\n"     /* D2  = D0 & D1    */
         "   sh      d15,d15,#12\n"  /* D15 = D15 << 12  */
         "   sh16    d2,#6\n"        /* D2  = D2 << 6    */
         "   or      d15,d2\n"       /* D15|= D2         */

/*       ppcxi = (osuint32*)pcxiadr; */
         "   mov16.a a15,d15\n"      /* A15 = D15         */

      );
      os_dsync(); /* Wait until svlcx has finished context writing before modifying PSW in the stored context */
      __asm
      (

/*       *(ppcxi+1) = 0x00000A80UL;*//* Position of PSW in the saved (upper)context */
         "   mov     d15,#2688\n"    /* D15 = 0x00000A80UL */
         "   st16.w  [a15]4,d15\n"   /* *(A15+4) = D15     */

/*       osInitialPcxiVal |= 0x00c00000UL;*//* modify lower context to upper context */
         "   movh    d15,#192\n"                         /*  d15 = 0x00c00000                 */
         "   or      d15,d0\n"                           /*  D15|= D0                         */
         "   movh.a  a15,#@his(osInitialPcxiVal)\n"      /*  A15 = &osInitialPcxiVal (part 1) */
         "   lea     a15,[a15]@los(osInitialPcxiVal)\n"  /*  A15 = &osInitialPcxiVal (part 2) */
         "   st.w    [a15],d15\n"                        /* *A15 = D15                        */
      );
      os_dsync();
      __asm
      (
/*       osd_mtcr(os_PCXI_OFFSET,osInitialPcxiVal); */
         "   mtcr    #65024,d15\n"                       /* PCXI = osInitialPcxiVal           */

/*       __isync(); */
         "   isync            \n"
      );
    #endif /* ((osdStackModel==osdMultiStack) || (osdStackModel==osdSingleStackOsek)) */   
   #endif /*( STATUS_LEVEL == EXTENDED_STATUS )*/
   
   __asm
   (
         /* enable interrupts by setting global interrupt flag */
/*    __enable(); */
      "   enable\n"
      /* setup software posted interrupt */
/*    osCPU_SRC0 = 0x00001001UL; /\* set of CPUSRC0, priority 1, enabled, CPU service *\/ */
      "   mov    d15,#4097\n"
      "   st16.w [%0],d15\n"
/*    osStartOSc(); */
      "   j      osStartOSc\n"
      :
      :"a"(&osCPU_SRC0) /*Handle address of CPU_SRC0 register to assembler, register %0*/
   );

} /* END OF osStartOSasm */
/* KB end osekHWosStartOSasmSrc */


/*-+--------------------------------------------------------------------------
 / StartOS 
 / =======
 / Syntax void StartOS ( AppModeType <Mode> )
 / Parameter (In):
 / Mode application mode
 / Parameter (Out): none
 / Description: The user can call this system service to start the operating
 / system in a specific mode, see chapter 5, Application modes.
 / Particularities: Only allowed outside of the operating system, therefore
 / implementation specific restrictions may apply. See also
 / chapter 11.3, System start-up, especially with respect to
 / systems where OSEK and OSEKtime coexist. This call does
 / not need to return.
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/
/* MISRA RULE 14.1 not violated: (command 1503) See comment for 8.10 */
/* MISRA RULE 8.10 VIOLATION: (command 1505) The function osStartOSc is general code 
 * and is called from within the implementation specific function osStartOSasm. 
 * That function might be written in assembler or C and be located in the same 
 * file as osStartOSc or another file, dependent on the implementation. Therfore 
 * MISRA checking tools might not see the call and state violation of rule 14.1 
 * wrongly or they see the call and state a violation of rule 8.10 correctly.
 * Anyhow, the function osStartOSc needs to have external linkage as some 
 * implementations have the calling function osStartOSasm in another file.
 */
osqFunc1 void osqFunc2 osStartOSc(void)     /* PRQA S 1503 *//* PRQA S 1505 */
{
   /* CovComment 4:  Function: osStartOSc */
   /* is called (from osStartOSasm) with interrupts disabled up to system level */
#if (osdNumberOfAllTasks > 0)
   osTaskIndexType nt;
#endif

#if (osdNumberOfAllResources > 0) 
#if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
#if (osdNumberOfCat2ISRs > 0)
   ISRType ni;
#endif
#endif
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
#if (osdNumberOfApplications > 0)
   ApplicationType na;
#endif
#endif
   
#if (STATUS_LEVEL == EXTENDED_STATUS) ||        \
    osdORTIDebug                      ||        \
    osdKillTaskIsrPossible            ||        \
    (osdSC== SC3)                     ||        \
    (osdSC== SC4)
#if (osdNumberOfAllResources > 0)
   osResourceIndexType nr;
#endif
#endif

   #if (osdNumberOfCounters > 0)
      #if osdUseCounterAPI
         osuint8 nc;
      #endif
   #endif

#ifdef osdEnableCoverage
   osuint16 i;

   /* during initialization of the coverage array no coverage is possible */
   /* initialize the coverage array */
   for(i=0; i<osdCoverageArraySize; i++)
   {
      osCoverage[i] = 0;
   }
#endif

   COVER(0x0024)

   /* initialise trace if in use */
#ifdef osdUseTrace
   COVER(0x0025)
   osInitTrace();
#endif

   OS_ST_ENTRY()
   OS_ST_START_CRITICAL()

   COVER(0x0026)

   /* Allow cat 1 interrupts (if they hadn't been enabled by the user anyway) */
   /* KB begin osekHWStartAllowCat1 (overwritten) */
   /* KB end osekHWStartAllowCat1 */


/* KB begin dummyVariablesInit (default) */
/* KB end dummyVariablesInit */

#if STATUS_LEVEL == EXTENDED_STATUS
   /* Error checking is done in the define-macro StartOS, of only OSDEFAULTAPPMODE exists */
   /* Parameter checking, Mode is a bit-mask with only just one bit set */
   if (((osActiveAppMode & ((AppModeType)(osActiveAppMode-((AppModeType)1)))) != 0)  ||   /* more than one bit set   */
       (osActiveAppMode == 0)                                         ||   /* zero                    */
       (osActiveAppMode > (1U << (osrNumberOfAppModes - 1)))             ) /* exceeding maximum value */
   {
      COVER(0x0027)
      #if osdErrorHook
      COVER(0x0028)
      osLastError = osdErrSTWrongAppMode; 
      osErrorHook(E_OS_SYS_ABORT);
      #endif

      #if (osdShutdownHook || (osdNumberOfApplications > 0))
      COVER(0x0029)
      osSysShutdownOS(E_OS_SYS_ABORT);
      #else
      COVER(0x002A)
      osSysShutdownOS();
      #endif
   }
   ELSEWITHCOVER(0x002B)
#endif

#if osdLib
   COVER(0x002C)
   osCheckLibraryVersionAndVariant();
#endif

#if (osdNumberOfAllTasks>0)
   COVER(0x002D)
   osInitTCBs();
#else
   COVER(0x002E)
   osQReadyPrios = 0;
#endif



#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)
 #if osdStackCheck
   COVER(0x002F)
  #if (osdNumberOfAllTasks > 0)
   COVER(0x0030)
   #if osdStackUsageMeasurement
   COVER(0x0031)
   /* KB begin osekHWosFillTaskStacks (default) */
   osFillTaskStacks();
   /* KB end osekHWosFillTaskStacks */
   #else
   COVER(0x0032)
   /* KB begin osekHWosInitTaskStacks (default) */
   osInitTaskStacks();
   /* KB end osekHWosInitTaskStacks */
   #endif
  #endif
   osLastActiveTaskIndex = 0;
 #endif
#endif

   osActiveTaskPrio = osHighReadyTaskPrio = osdNoActivePrio; /* mark as invalid for a nested interrupt */
   osActiveTaskIndex = osHighReadyTaskIndex = osdNoActiveTask;

#if ((osdSC == SC3) || (osdSC == SC4))
   COVER(0x0033)
   osInSystemFct = 0;
#endif

#if ( (osdErrorHook!=0) || (osdComErrorHook!=0) )
#if osdComErrorHook
   COVER(0x0034)
   osCOMErrorFlag=osdFALSE;
#endif
   COVER(0x0035)
   osErrorFlag=osdFALSE;
   /* KB begin osekInitGlobalError (default) */
   #if osdORTIDebug
   COVER(0x0036)
   osLastErrorCode=0;
   #endif
   COVER(0x0037)
   osLastError=0;
   /* KB end osekInitGlobalError */
#endif

   osIntNestingDepth = 0;
   osStartDispatcher = 0;
#if (osdFullPreempt || osdMixedPreempt)
   COVER(0x0038)
   osLockDispatcher = 1; /* lock dispatcher during StartOS */
#endif

   /* fullfills [SR:167] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   osActiveISRID = INVALID_ISR;

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x0039)
   osIntAPIStatus = 0;
#endif

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x003A)
#if osdEnableAssertions
   COVER(0x003B)
   /* check memory access */
   osSysErrAssert((osStartDispatcher==0), osdErrSTMemoryError)
#if (osdFullPreempt || osdMixedPreempt)
   COVER(0x003C)
   osSysErrAssert((osLockDispatcher==1), osdErrSTMemoryError)
#endif
   /* assure that OS variables are really linked to RAM, osIntAPIStatus is volatile so compiler optimizations do not disturb */
   osIntAPIStatus = 7; 
   osSysErrAssert((osIntAPIStatus==7), osdErrSTMemoryError)
   osIntAPIStatus = 0;
   osSysErrAssert((osIntAPIStatus==0), osdErrSTMemoryError)
#endif
#if (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))
 #if ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0)
   osResGlobalOccupationCounter = 0;
 #endif
#endif
#endif /* STATUS_LEVEL == EXTENDED_STATUS */

#if (osdNumberOfAllResources > 0)
   COVER(0x003D)
#if (STATUS_LEVEL == EXTENDED_STATUS) || osdORTIDebug
   COVER(0x003E)
   for (nr=0; nr<osrNumberOfAllResources; nr++)
   {
      COVER(0x003F)
      #if (STATUS_LEVEL == EXTENDED_STATUS) && ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))
         COVER(0x0040)
         osResOccupationCounter[nr] = 0;
      #endif
      #if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions && ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))) || osdORTIDebug
         COVER(0x0041)
         osResActualTask[nr] = osdNoActiveTask;
      #endif
   }

#endif /* (STATUS_LEVEL == EXTENDED_STATUS) || osdORTIDebug */
#endif /* (osdNumberOfAllResources > 0) */

#if (osdTimingProtectionUsed != 0)
   COVER(0x0042)
   osInitTimingProtection();
#endif

   
#if (osdNumberOfAllTasks > 0)
   COVER(0x0043)
   /* activate all autostart tasks */
   for (nt= 0; nt < osrNumberOfAllTasks; nt++)
   {
      COVER(0x0044)
      osTcbTaskState[nt] = SUSPENDED;

      #if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x0045)
      #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
      COVER(0x0046)
      osTcbResourceCounter[nt]=0;
      #endif
      #endif
      if ((oskTcbAutoStart[nt] & osActiveAppMode) != 0)
      {
         COVER(0x0047)
         (void) osSysActivateTask(nt); 
      }
      ELSEWITHCOVER(0x0048)
   }
#endif

   #if (osdNumberOfCounters > 0)
      COVER(0x0049)
      #if osdUseCounterAPI
         COVER(0x004A)
         for(nc = 0; nc < osrNumberOfCounters; nc++)
         {
            COVER(0x004B)
            osCounter[nc] = 0;
         }
      #else
         #if (osdSystemCounterNeeded)
            COVER(0x004C)
            osSystemCounter = 0;
         #endif   
      #endif   
   #endif   

#if (osdNumberOfAlarms > 0)
   COVER(0x004D)
   osInitAlarms();
#endif

#if (osdNumberOfScheduleTables > 0)
   COVER(0x004E)
   osInitScheduleTables();
#endif

   /* KB begin osekHWStartupSpecial (overwritten) */
   osIntSaveDisableGlobalCounter = 0;
   #if osdTimingProtectionUsed
   osIntSaveDisableHighLevelCounter = 0;
   #endif
   #if (osdNumberOfInterruptResources > 0)
   #if STATUS_LEVEL == EXTENDED_STATUS
   osInitialIntLevel = 0;
   #endif
   #endif
   #if (osdStackModel == osdSingleStackOsek)
   osCurrentBasicSP = (osuint32) &osSystemStack[ osdSystemStackSize/(osStackSizeType)sizeof(osStackDataType) ]; /* PRQA S 3683 */ /* index is intentionally one larger than array */
   osIsOnSystemStack = 1;
   #endif
   /* KB end osekHWStartupSpecial */


#if (osdNumberOfAllResources > 0)
   COVER(0x004F)
#if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
#if (osdNumberOfCat2ISRs > 0)
   COVER(0x0050)
   for (ni=0; ni<osdNumberOfCat2ISRs; ni++)
   {
      COVER(0x0051)
      osIcbLockedResource[ni] = osdNoResource;
   }
#endif /*osdNumberOfCat2ISRs*/
#if (osdNumberOfAllTasks > 0)
   COVER(0x0052)
   for (nt=0; nt<osdNumberOfAllTasks; nt++)
   {
      COVER(0x0053)
      osTcbLockedResource[nt] = osdNoResource;
   }
#endif /*osdNumberOfAllTasks*/

   for (nr=0; nr<osdNumberOfAllResources; nr++)
   {
      COVER(0x0054)
      osResNextLockedResource[nr] = osdNoResource;
   }

#endif /*(osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4)) */
#endif /* (osdNumberOfAllResources > 0) */

#if ((osdSC== SC3) || (osdSC== SC4))
   COVER(0x0055)
   osInApplHook = 0;

   /* osActiveApplicationID might be sometimes used in the StartupHook.
    * Therefore it must be initialized here, even if it is not always 
    * used before the next assignment. This is no MISRA error. (3198) */
   osActiveApplicationID = INVALID_OSAPPLICATION;   /* PRQA S 3198 */

   /* KB begin osekHWInitProtection (overwritten) */
   COVER(0x0056)
   /* Initialize protection registers for privileged mode */
   
   /* Protection set 0 is used for privileged mode */
   /* Setup data set 0, range 0 for R/W access to all RAM */
   osd_mtcr(os_DPR0_0L_OFFSET, (osuint32)(&_osDataStart)); 
   osd_mtcr(os_DPR0_0U_OFFSET, (osuint32)(&_osDataEnd)); 
   /* Setup data set 0, range 3 for r access to all constants */
   osd_mtcr(os_DPR0_3L_OFFSET, (osuint32)(&_osMemStart)); 
   osd_mtcr(os_DPR0_3U_OFFSET, (osuint32)(&_osMemEnd)); 
   /* Setup access rights for data set 0 */
   osd_mtcr(os_DPM0_OFFSET, (osuint32)(0x400000c0UL));   /* R/W for data, R only for constants */
   
   /* Has the first code range non zero size? */
   if((&_osCodeStart1) != (&_osCodeEnd1))
   {
      /* Assure that code set 0, range 0 is not yet in use, i.e. by the debugger */
      osSysErrAssert((osd_mfcr(os_CPM0_OFFSET) & 0x00000029) == 0, osdErrSTDebuggerUsesCPR0_0);
      /* Setup code set 0, range 0 for execution access */
      osd_mtcr(os_CPR0_0L_OFFSET, (osuint32)(&_osCodeStart1)                         );
      osd_mtcr(os_CPR0_0U_OFFSET, (osuint32)(&_osCodeEnd1)                           );
      osd_mtcr(os_CPM0_OFFSET   , (osuint32)(0x00000080UL | osd_mfcr(os_CPM0_OFFSET)));
      COVER(0x0057)
   }
   ELSEWITHCOVER(0x0058)
   /* Has the second code range non zero size? */
   if((&_osCodeStart2) != (&_osCodeEnd2))
   {
      /* Assure that code set 0, range 1 is not yet in use, i.e. by the debugger */
      osSysErrAssert((osd_mfcr(os_CPM0_OFFSET) & 0x00002900) == 0, osdErrSTDebuggerUsesCPR0_1);
      /* Setup code set 0, range 1 for execution access */
      osd_mtcr(os_CPR0_1L_OFFSET, (osuint32)(&_osCodeStart2)                         );
      osd_mtcr(os_CPR0_1U_OFFSET, (osuint32)(&_osCodeEnd2)                           );
      osd_mtcr(os_CPM0_OFFSET   , (osuint32)(0x00008000UL | osd_mfcr(os_CPM0_OFFSET)));
      COVER(0x0059)
   }
   ELSEWITHCOVER(0x005A)
   
   /* Initialize protection registers for non-privileged mode */
   
   /* Protection set 1 is used for non-privileged mode */
   /* Setup data set 1, range 3 for R access to all constants */
   osd_mtcr(os_DPR1_3L_OFFSET, (osuint32)(&_osMemStart)); 
   osd_mtcr(os_DPR1_3U_OFFSET, (osuint32)(&_osMemEnd)); 
   /* Other ranges are initialized at task or ISR start */
   /* Setup access rights for data set 1 */
   osd_mtcr(os_DPM1_OFFSET, (osuint32)(0x40c0c0c0UL));  /* R/W for all data, R only for constants */
   
   /* Has the first code range non zero size? */
   if((&_osCodeStart1) != (&_osCodeEnd1))
   {
      /* Assure that code set 1, range 0 is not yet in use, i.e. by the debugger */
      osSysErrAssert((osd_mfcr(os_CPM1_OFFSET) & 0x00000029) == 0, osdErrSTDebuggerUsesCPR1_0);
      /* Setup code set 1, range 0 for execution access */
      osd_mtcr(os_CPR1_0L_OFFSET, (osuint32)(&_osCodeStart1)                         );
      osd_mtcr(os_CPR1_0U_OFFSET, (osuint32)(&_osCodeEnd1)                           );
      osd_mtcr(os_CPM1_OFFSET   , (osuint32)(0x00000080UL | osd_mfcr(os_CPM1_OFFSET)));
      COVER(0x005B)
   }
   ELSEWITHCOVER(0x005C)
   /* Has the second code range non zero size? */
   if((&_osCodeStart2) != (&_osCodeEnd2))
   {
      /* Assure that code set 1, range 1 is not yet in use, i.e. by the debugger */
      osSysErrAssert((osd_mfcr(os_CPM1_OFFSET) & 0x00002900) == 0, osdErrSTDebuggerUsesCPR1_1);
      /* Setup code set 0, range 1 for execution access */
      osd_mtcr(os_CPR1_1L_OFFSET, (osuint32)(&_osCodeStart2)                         );
      osd_mtcr(os_CPR1_1U_OFFSET, (osuint32)(&_osCodeEnd2)                           );
      osd_mtcr(os_CPM1_OFFSET   , (osuint32)(0x00008000UL | osd_mfcr(os_CPM1_OFFSET)));
      COVER(0x005D)
   }
   ELSEWITHCOVER(0x005E)
   
   /* enable protection (only necessary for older derivatives) */
   osd_mtcr(os_SYSCON_OFFSET,((osd_mfcr(os_SYSCON_OFFSET) | 0x02))); 
   /* the required isync instruction is executed by osd_mtcr */
   
   /* KB end osekHWInitProtection */
#endif   /* ((osdSC== SC3) || (osdSC== SC4)) */


#if osdStartupHook
   COVER(0x005F)
   /* call hook-routine for system startup (initialization of drivers etc.) */
#if ((osdSC== SC3) || (osdSC== SC4))
   COVER(0x0060)
   osCurrentContext = osdCtxStartupHook;
#endif   

   OS_STHOOK_ENTRY()
   osInterStartupHook();
   OS_STHOOK_EXIT()
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   COVER(0x0061)
#if (osdNumberOfApplications > 0)
   COVER(0x0062)
   osInApplHook = 1;
#if (!osdStartupHook)  /* else already initialized */
   COVER(0x0063)
   /* osCurrentContext might be sometimes used in the StartupHook.
    * Therefore it must be initialized here, even if it is not always 
    * used before the next assignment. This is no MISRA error. (3198) */
   osCurrentContext = osdCtxStartupHook;   /* PRQA S 3198 */
#endif
   for (na=0; na<osdNumberOfApplications; na++)
   {
      if (oskApplStartupHook[na] != 0)
      {
         COVER(0x0064)
         /* osActiveApplicationID might be sometimes used in the StartupHook.
          * Therefore it must be initialized here, even if it is not always 
          * used before the next assignment. This is no MISRA error. (3198) */
         osActiveApplicationID = na;   /* PRQA S 3198 */
         /* KB begin osekHWApplStartupHookStart (overwritten) */
         if (oskApplIsTrusted[osActiveApplicationID] == 0)
         {
            /* set protection for application specific hook */
            osuint32 localSP;
         
            COVER(0x0065)
            /* Get the stack pointer from A10 to localSP */
            __asm("mov.d\t%0, a10":"=d"(localSP));
         
            osd_mtcr(os_DPR1_0L_OFFSET, 0);   /* no task/ISR data */
            osd_mtcr(os_DPR1_0U_OFFSET, 0);
         
            osd_mtcr(os_DPR1_1L_OFFSET, (osuint32)(&osSystemStack));   /* hook ist running on remaining space on system stack */
            osd_mtcr(os_DPR1_1U_OFFSET,localSP);
         
            osd_mtcr(os_DPR1_2L_OFFSET,oskApplMemoryAccessLow[osActiveApplicationID][2]);
            osd_mtcr(os_DPR1_2U_OFFSET,oskApplMemoryAccessHigh[osActiveApplicationID][2]);
         }
         /* KB end osekHWApplStartupHookStart */
         /* KB begin osekHWApplStartupHookCall (default) */
         (*oskApplStartupHook[osActiveApplicationID])();
         /* KB end osekHWApplStartupHookCall */
         /* KB begin osekHWApplStartupHookEnd (overwritten) */
         /* KB end osekHWApplStartupHookEnd */
      }
      ELSEWITHCOVER(0x0066)
   }
   osInApplHook = 0;
   osActiveApplicationID = INVALID_OSAPPLICATION;
#endif
   osCurrentContext = osdCtxTask;
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdSystemCounterNeeded)
   COVER(0x0067)
   osInitTimer(); /* initialization of the hardware timer now after the startup-hook */

#endif /*#if (osdSystemCounterNeeded)*/

#if (osdFullPreempt || osdMixedPreempt)
   COVER(0x0068)
   /* free dispatcher (after osSysActivateTask for AutoStart-Tasks and after StartupHook */
   osLockDispatcher = 0; 
#endif

   OS_ST_END_CRITICAL()
   OS_ST_EXIT()
   /* KB begin osekHWosEnableLevelInStartOS (overwritten) */
   /* enable task level interrupt */
   osEnableLevel();   
   /* KB end osekHWosEnableLevelInStartOS */

#if (osdStackModel == osdSingleStackOptimized) || (osdStackModel == osdSingleStackOptimizedCS)
  /* KB begin osekHWosIdleLoopSingleStackOptimized (overwritten) */
  #if (osdNonPreempt == 0)
     /* On preemptive systems, the dispatcher needs to be called from here only ones */
     if (osStartDispatcher != 0)
     {
        OS_START_DISPATCH()
        /* KB begin osekHWosDispatcher (overwritten) */
        osDispatch()
        /* KB end osekHWosDispatcher */
        COVER(0x0069)
     }
  #endif
  for (;;)
  {
     #if (osdNonPreempt != 0)
        if (osStartDispatcher != 0)
        {
           OS_START_DISPATCH()
           /* KB begin osekHWosDispatcher (overwritten) */
           osDispatch()
           /* KB end osekHWosDispatcher */
           COVER(0x006A)
           /* 
            * as the dispatcher just returns, the macro OS_IDLE_ENTRY() can only be 'called'
            * for non-preemptive systems in case of SingleStackOptimized!
            */
           OS_IDLE_ENTRY()
        }
     #endif
     COVER(0x006B)
     OS_IDLE_PROC()
  }
  /* KB end osekHWosIdleLoopSingleStackOptimized */ /* [SPMF0043:0016] */
#else
   OS_START_DISPATCH()
   /* KB begin osekHWosDispatcher (overwritten) */
   osDispatch()
   /* KB end osekHWosDispatcher */

   /* KB begin osekHWAfterOsDispatchInST (default) */
   /* fullfills [SR:012] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
      osDisableGlobal();
      osAssertFailed_(osdErrSTNoImmediateTaskSwitch);
   #else /* if STATUS_LEVEL ...*/
      for(;;)
      {
      	;
      }
   #endif /* STATUS_LEVEL ... */
   /* KB end osekHWAfterOsDispatchInST */
#endif
} /* END OF osStartOSc */


/* osSysShutdownOS is called internally by the OS, osShutdownOS is the user */
/* API and contains additional checks */

/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:013] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* MISRA RULE 8.10 VIOLATION: (2 times) The function osSysShutdownOS is a general implementation which is used 
 * identically on different plattforms. On some of them, it is called only in osek.c, on others it 
 * is called also from other files. Therefore it is made globally visible. The risk of potential 
 * name collisions is expected to be low.*/
#if (osdShutdownHook || (osdNumberOfApplications > 0))
osqFunc1 void osqFunc2 osSysShutdownOS(StatusType Error)      /* PRQA S 1505 */ 
#else
osqFunc1 void osqFunc2 osSysShutdownOS(void)                  /* PRQA S 1505 */ 
#endif
{
   /* CovComment 4:  Function: osSysShutdownOS */
   /* KB begin osekReadUnusedVarsAndConstsAux (default) */
      osuint8 ucTmp;
   /* KB end osekReadUnusedVarsAndConstsAux */
#if ((osdSC == SC3) || (osdSC == SC4))
#if (osdNumberOfApplications > 0)
   ApplicationType na;
#endif
#endif

   OS_SD_ENTRY()
   COVER(0x006C)
   
   /* KB begin osekHWosDisableGlobal (overwritten) */
   /* disable all interrupts, no nesting */
   osDisableGlobal();
   /* KB end osekHWosDisableGlobal */ /* disable all interrupts */

   /* Necessary to allow interrupt API functions in the shutdown hook 
    * together with timing protection */
   osActiveTaskIndex = osdNoActiveTask;
   osIntNestingDepth = 0;

   /* call shutdown hook routine */
#if ((osdSC== SC3) || (osdSC== SC4))
   COVER(0x006D)
   /* prevent recursive calls of application error hooks */
   if (osCurrentContext != osdCtxShutdownHook)
   {
      COVER(0x006E)
      osCurrentContext = osdCtxShutdownHook;
   
#if (osdNumberOfApplications > 0)
      COVER(0x006F)
      /* call application shutdown hooks */
      osInApplHook = 1;
      for (na=0; na<osdNumberOfApplications; na++)
      {
         if (oskApplShutdownHook[na] != 0)
         {
            COVER(0x0070)
            /* osActiveApplicationID might be sometimes used in the StartupHook.
             * Therefore it must be initialized here, even if it is not always 
             * used before the next assignment. This is no MISRA error. (3198) */
            osActiveApplicationID = na;   /* PRQA S 3198 */
            /* KB begin osekHWApplShutdownHookStart (overwritten) */
            if (oskApplIsTrusted[osActiveApplicationID] == 0)
            {
               /* set protection for application specific hook */
            
               osuint32 localSP;
            
               COVER(0x0071)
               /* Get the stack pointer from A10 to localSP */
               __asm("mov.d\t%0, a10":"=d"(localSP));
            
               osd_mtcr(os_DPR1_0L_OFFSET, 0);   /* no task/ISR data */
               osd_mtcr(os_DPR1_0U_OFFSET, 0);
            
               /* hook ist running on remaining space of system stack */
               osd_mtcr(os_DPR1_1L_OFFSET, (osuint32)(&osSystemStack));
               osd_mtcr(os_DPR1_1U_OFFSET, localSP);
            
               osd_mtcr(os_DPR1_2L_OFFSET,oskApplMemoryAccessLow[osActiveApplicationID][2]);
               osd_mtcr(os_DPR1_2U_OFFSET,oskApplMemoryAccessHigh[osActiveApplicationID][2]);
            }
            /* KB end osekHWApplShutdownHookStart */
            /* KB begin osekHWApplShutdownHookCall (default) */
            (*oskApplShutdownHook[osActiveApplicationID])(Error);
            /* KB end osekHWApplShutdownHookCall */
            /* KB begin osekHWApplShutdownHookEnd (overwritten) */
            /* KB end osekHWApplShutdownHookEnd */
         }
         ELSEWITHCOVER(0x0072)
      }
      osInApplHook = 0;
      osActiveApplicationID = INVALID_OSAPPLICATION;
#endif
   }
   ELSEWITHCOVER(0x0073)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if osdShutdownHook 
   COVER(0x0074)
   /* call system shutdown hook */
   OS_SDHOOK_ENTRY()
   osInterShutdownHook(Error);
   OS_SDHOOK_EXIT()
#endif
   OS_SD_EXIT()
   
   /* KB begin osekReadUnusedVarsAndConsts (default) */
   /* force the compiler to link these 'unused' variables, too */
   ucTmp  = (osuint8)oskVersionVariant.ucMagicNumber1;
   #if osdErrorHook
   ucTmp |= (osuint8)osLastError;
   #endif
   #if osdORTIDebug
   ucTmp |= (osuint8)osTcbActualPrio[0];
    #if osdErrorHook
   ucTmp |= (osuint8)osLastErrorCode;
    #endif
   #endif
   osdDummyRead(ucTmp)
   /* KB end osekReadUnusedVarsAndConsts */
   /* KB begin osekHWosShutdownReactionSrc (overwritten) */
   /*switch off infinite loop warning*/
   #pragma warning 557
   for(;;) 
   {
      ;
   }
   /* KB end osekHWosShutdownReactionSrc */
} /* END OF osSysShutdownOS */

/*-+--------------------------------------------------------------------------
 / ShutdownOS
 / ==========
 /
 / Syntax void ShutdownOS ( StatusType <Error> )
 / Parameter (In):
 / Error error occurred
 / Parameter (Out): none
 / Description: The user can call this system service to abort the overall
 / system (e.g. emergency off). The operating system also calls
 / this function internally, if it has reached an undefined internal
 / state and is no longer ready to run.
 / If a ShutdownHook is configured the hook routine
 / ShutdownHook is always called (with <Error> as argument)
 / before shutting down the operating system.
 / If ShutdownHook returns, further behaviour of ShutdownOS is
 / implementation specific.
 / In case of a system where OSEK OS and OSEKtime OS
 / coexist, ShutdownHook must return.
 / <Error> must be a valid error code supported by OSEK OS. In
 / case of a system where OSEK OS and OSEKtime OS coexist,
 / <Error> might also be a value accepted by OSEKtime OS. In
 / this case, if enabled by an OSEKtime configuration parameter,
 / OSEKtime OS will be shut down after OSEK OS shutdown.
 / Particularities: After this service the operating system is shut down.
 / Allowed at task level, ISR level, in ErrorHook and StartupHook,
 / and also called internally by the operating system.
 / If the operating system calls ShutdownOS it never uses E_OK
 / as the passed parameter value.
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

/* QAC messages 3206,3199: The parameter 'Error' is intentionally used only in certain 
 * configurations. In other configurations, it depends on the plattform specific 
 * definition of osdDummyRead, which of the messages is stated. Both messages are 
 * not relevant for MISRA */
/* MISRA RULE 14.1 VIOLATION: osShutdownOS might be not called in some
 * configuration. Conditional compiling of the function would increase
 * the code complexity to unacceptable level (1503) */ 
osqFunc1 void osqFunc2 osShutdownOS(StatusType Error) /* PRQA S 1503,3206,3199 */
{
   /* CovComment 4:  Function: osShutdownOS */
   OS_SD_ENTRY()
   COVER(0x0075)
   
#if ((osdSC == SC3) || (osdSC == SC4))
 #if (osdNumberOfApplications > 0)
   if ((osActiveApplicationID != INVALID_OSAPPLICATION) && (oskApplIsTrusted[osActiveApplicationID] == 0))
   {  /* ignore calls to ShutdownOS from non trusted applications */
      COVER(0x0076)
      return;
   }
   ELSEWITHCOVER(0x0077)
 #endif
#endif /*((osdSC == SC3) || (osdSC == SC4))*/
   
#if (osdShutdownHook || (osdNumberOfApplications > 0))
   COVER(0x0078)
   osSysShutdownOS(Error);
#else
   COVER(0x0079)
   osdDummyRead(Error)
   osSysShutdownOS();
#endif
}

/* KB begin BNCdispatcherSrc (default) */
#if (osdStackModel == osdSingleStackOptimized) || (osdStackModel == osdSingleStackOptimizedCS)
#if (osdVectorTableInRAM == 1)
#pragma section vector_init
#endif
/*-+--------------------------------------------------------------------------
 / osBNCDispatch
 / Funktion: Dispatcher (Delayed-Interrupt)
 /   - saves old task kontext
 /   - does not contain IDLE-Loop
 /   - switch to new task
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
/* MISRA RULE 14.1 not violated: Function is referenced from interrupt vector table (QAC cannot detect this) */
void  __interrupt(1) osBNCDispatch(void) /* PRQA S 1503 */ 
{
   /* CovComment 4:  Function: osBNCDispatch */
   TaskType   oldActiveTaskIndex; /* [SPMF0043:0007] */
#if (osdORTIDebug && (osdORTIVersion == 200))
   osuint8      ORTIOldServiceId;
#endif

   /* KB begin osekHWosDisableLevel (overwritten) */
   /* disable interrupts up to system level */
   osDisableLevel();
   /* KB end osekHWosDisableLevel */  /* [SPMF0043:0018] */
   if (osHighReadyTaskIndex != osActiveTaskIndex)
   {
      /* handle previous task (if any) */
      if (osActiveTaskIndex != osdNoActiveTask)
      {
#if (osdORTIDebug && (osdORTIVersion == 200))
         ORTIOldServiceId = osORTICurrentServiceId;
         COVER(0x007A)
#endif
#if osdPostTaskHook
         osPostTaskHook();
         COVER(0x007B)
#endif
#if osdNumberOfInternalResources > 0
         /* KB begin osReleaseInternalResource (default) */
         if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
         {
         #if osdRTSize == 1
            COVER(0x007C)
            osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
         #else
            COVER(0x007D)
            osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] &= /*;*/
               ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
         #endif
         #if ((CC == ECC2) || (CC == BCC2))
            COVER(0x007E)
            osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
         #endif
         
         #if osdORTIDebug
            COVER(0x007F)
            osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
         #endif
         
         }
         ELSEWITHCOVER(0x0080)
         
         
         /* KB end osReleaseInternalResource */
         COVER(0x0081)
#endif
         COVER(0x0082)
         osTcbTaskState[osActiveTaskIndex] = READY;
         #if osdTimingProtectionUsed
            COVER(0x0083)
            (void) osTPExeSwitchTo(INVALID_PROCESS);
            osTPSuspendTimerBlock();
         #endif
         /* KB begin BNCdispatcherStackCheck (default) */
         #if osdStackCheck && (osdStackModel == osdSingleStackOptimized)
         if ( *(osuint8*)(osdSingleStackBottom) != 0xAA)
         {
            COVER(0x0084)
            osStackOverflow(); /* we won't return from here */
         }
         ELSEWITHCOVER(0x0085)
         #endif
         /* KB end BNCdispatcherStackCheck */
      }
      ELSEWITHCOVER(0x0086)
      oldActiveTaskIndex = osActiveTaskIndex; /* [SPMF0043:0007] */

      osEnableGlobal();

      /* handle the new task(s) */
      do 
      {
#if (osdORTIDebug && (osdORTIVersion == 200))
         osORTICurrentServiceId = osdORTINoService;
         COVER(0x0087)
#endif
         osActiveTaskIndex  = osHighReadyTaskIndex;
         osActiveTaskPrio   = osHighReadyTaskPrio;
         osTcbTaskState[osActiveTaskIndex] = RUNNING;
#if osdMixedPreempt
         if (oskTcbTaskPreemption[osActiveTaskIndex] == osdTaskTypeNonPreempt)
         {  
            osLockDispatcher = 1;
            COVER(0x0088)
         }
         else
         {  
            osLockDispatcher = 0;
            COVER(0x0089)
         }
#endif
#if osdTimingProtectionUsed
         (void) osTPExeSwitchTo(osActiveTaskIndex);
         COVER(0x008A)
#endif
#if osdPreTaskHook
         osPreTaskHook();
         COVER(0x008B)
#endif
         /* KB begin osOccupyInternalResource (default) */
         #if (osdNumberOfInternalResources > 0)
         if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
         {
            osPrioType IntResCeilingPrio= oskResInternalCeilingPrio[osActiveTaskIndex];
         #if (CC==BCC2) || (CC==ECC2)
            COVER(0x008C)
            oskQActivationQueues[IntResCeilingPrio][0]= osActiveTaskIndex;
         #else
            COVER(0x008D)
            osQReadyTask[IntResCeilingPrio]= osActiveTaskIndex;
         #endif
         
         #if osdRTSize == 1
            COVER(0x008E)
            osQReadyPrios |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
         #else
            COVER(0x008F)
            osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] |= /*;*/
               oskResInternalCeilingPrioMask[osActiveTaskIndex];
         #endif
         
            /* correct osHighReadyTaskPrio if required */
            if(IntResCeilingPrio<osHighReadyTaskPrio)
            {
               COVER(0x0090)
               osHighReadyTaskPrio = IntResCeilingPrio;
               osActiveTaskPrio  =   IntResCeilingPrio;
            }
            else
            {
               COVER(0x0091)
               osActiveTaskPrio  = osHighReadyTaskPrio;
            }
         
         }
         else
         {
            COVER(0x0092)
         #endif /* (osdNumberOfInternalResources > 0) */
         
            COVER(0x0093)
            osActiveTaskPrio  = osHighReadyTaskPrio;
         
         #if (osdNumberOfInternalResources > 0)
         }
         
         #if osdORTIDebug
         COVER(0x0094)
         osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
         #endif
         
         #endif /*(osdNumberOfInternalResources > 0)*/
         
         
         /* KB end osOccupyInternalResource */
         COVER(0x0095)
         /* KB begin osekHWosEnableLevel (overwritten) */
         /* enable task level interrupt */
         osEnableLevel();   
         /* KB end osekHWosEnableLevel */                        /* [SPMF0043:0019] */
         oskTcbTaskStartAddress[osActiveTaskIndex](); /* [SPMF0043:0008] */
         /* KB begin osekHWosDisableLevel (overwritten) */
         /* disable interrupts up to system level */
         osDisableLevel();
         /* KB end osekHWosDisableLevel */
         COVER(0x0096)
#if (STATUS_LEVEL == EXTENDED_STATUS) 
         if(osActiveTaskIndex != osdNoActiveTask && osTcbTaskState[osActiveTaskIndex] == RUNNING)
         {
            osMissingTerminateError();      /* will not return, [SPMF0043:0013] */
         }
#endif /* (STATUS_LEVEL == EXTENDED_STATUS) */
#if osdTimingProtectionUsed
         osTPExeSwitchBack(INVALID_PROCESS);
         /* All timing measurements for resource blocking times are stopped already! */
         COVER(0x0097)
#endif
#if osdMixedPreempt
         osLockDispatcher = 0;
         COVER(0x0098)
#endif
         /* KB begin BNCdispatcherStackCheck (default) */
         #if osdStackCheck && (osdStackModel == osdSingleStackOptimized)
         if ( *(osuint8*)(osdSingleStackBottom) != 0xAA)
         {
            COVER(0x0099)
            osStackOverflow(); /* we won't return from here */
         }
         ELSEWITHCOVER(0x009A)
         #endif
         /* KB end BNCdispatcherStackCheck */
         COVER(0x009B)
      } while (osHighReadyTaskIndex != oldActiveTaskIndex); /* [SPMF0043:0005] [SPMF0043:0006] [SPMF0043:0014] */

      /* handle previous task (if any) */
      osActiveTaskIndex  = oldActiveTaskIndex;
      osActiveTaskPrio   = osHighReadyTaskPrio;
      if (osActiveTaskIndex != osdNoActiveTask) /* [SPMF0043:0015] */
      {
         COVER(0x009C)
         osTcbTaskState[osActiveTaskIndex] = RUNNING;
#if osdTimingProtectionUsed
         (void) osTPExeSwitchTo(osActiveTaskIndex);
         osTPResumeTimerBlock(osActiveTaskIndex);
         COVER(0x009D)
#endif
#if osdPreTaskHook
         osPreTaskHook();
         COVER(0x009E)
#endif
         /* KB begin osOccupyInternalResource (default) */
         #if (osdNumberOfInternalResources > 0)
         if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
         {
            osPrioType IntResCeilingPrio= oskResInternalCeilingPrio[osActiveTaskIndex];
         #if (CC==BCC2) || (CC==ECC2)
            COVER(0x009F)
            oskQActivationQueues[IntResCeilingPrio][0]= osActiveTaskIndex;
         #else
            COVER(0x00A0)
            osQReadyTask[IntResCeilingPrio]= osActiveTaskIndex;
         #endif
         
         #if osdRTSize == 1
            COVER(0x00A1)
            osQReadyPrios |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
         #else
            COVER(0x00A2)
            osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] |= /*;*/
               oskResInternalCeilingPrioMask[osActiveTaskIndex];
         #endif
         
            /* correct osHighReadyTaskPrio if required */
            if(IntResCeilingPrio<osHighReadyTaskPrio)
            {
               COVER(0x00A3)
               osHighReadyTaskPrio = IntResCeilingPrio;
               osActiveTaskPrio  =   IntResCeilingPrio;
            }
            else
            {
               COVER(0x00A4)
               osActiveTaskPrio  = osHighReadyTaskPrio;
            }
         
         }
         else
         {
            COVER(0x00A5)
         #endif /* (osdNumberOfInternalResources > 0) */
         
            COVER(0x00A6)
            osActiveTaskPrio  = osHighReadyTaskPrio;
         
         #if (osdNumberOfInternalResources > 0)
         }
         
         #if osdORTIDebug
         COVER(0x00A7)
         osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
         #endif
         
         #endif /*(osdNumberOfInternalResources > 0)*/
         
         
         /* KB end osOccupyInternalResource */
#if (osdORTIDebug && (osdORTIVersion == 200))
         osORTICurrentServiceId = ORTIOldServiceId;
         COVER(0x00A8)
#endif
      }
      ELSEWITHCOVER(0x00A9)
   }
   ELSEWITHCOVER(0x00AA)

   /* KB begin osekHWosEnableLevel (overwritten) */
   /* enable task level interrupt */
   osEnableLevel();   
   /* KB end osekHWosEnableLevel */  /* [SPMF0043:0020] */
   COVER(0x00AB)
} /* ENDOF osBNCDispatch */
#endif
/* KB end BNCdispatcherSrc */

#if osdStackCheck

/*-+--------------------------------------------------------------------------
 / osStackOverflow
 / Funktion:
 /   - called on task stack overflow: abort system
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
/* MISRA RULE 14.1 VIOLATION: See below at the 8.10 violation */
/* MISRA RULE 8.10 VIOLATION: The function osStackOverflow is a general implementation for different plattforms. 
 * It is called from the task dispatcher (in this file) and from the interrupt wrappers of category 2 ISRs. 
 * Both might be implemented in C or assembly language, dependent on the plattform. For this reason, 
 * MISRA checkers might see only calls from within osek.c or see no call at all. Therefore they wrongly 
 * state a violation of rule 14.1 or 8.10. */
osqFunc1 void osqFunc2 osStackOverflow(void)      /* PRQA S 1505 */ /* PRQA S 1503 */
{
#if ((osdSC == SC3) || (osdSC == SC4))
   ProtectionReturnType proRet;
#endif  /* ((osdSC == SC3) || (osdSC == SC4)) */
   
   /* CovComment 4:  Function: osStackOverflow */
   COVER(0x00AC)
   osDisableGlobal();

#if osdErrorHook
   COVER(0x00AD)
   osLastError = osdErrSOStackOverflow; 
#endif

#if ((osdSC == SC3) || (osdSC == SC4))
   /* fullfills [SR:072] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   COVER(0x00AE)
   proRet = osProtectionHookWrapper(E_OS_STACKFAULT);
   osHandleProtectionErrors(proRet, E_OS_STACKFAULT);
#else  /* ((osdSC == SC3) || (osdSC == SC4)) */
   /* fullfills [SR:071] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   #if osdErrorHook
   COVER(0x00AF)
   osErrorHook(E_OS_STACKFAULT);
   #endif

#if (osdShutdownHook || (osdNumberOfApplications > 0))
   COVER(0x00B0)
   osSysShutdownOS(E_OS_STACKFAULT);
#else
   COVER(0x00B1)
   osSysShutdownOS();
#endif

#endif /* ((osdSC == SC3) || (osdSC == SC4)) */
}


/* KB begin osekHWosSystemStackOverflowSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osIntStackOverflow
 / Funktion:
 /   - called on interrupt stack overflow: abort system
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

void osIntStackOverflow(void)
{
   /* CovComment 4:  Function: osIntStackOverflow */
   COVER(0x00B2)
   osDisableGlobal();

   #if osdErrorHook
   COVER(0x00B3)
   osLastError = osdErrYOStackOverflow; 
   osErrorHook(E_OS_STACKFAULT);
   #endif

   ShutdownOS(E_OS_STACKFAULT);
}
/* KB end osekHWosSystemStackOverflowSrc */
#if osdStackUsageMeasurement
 #if (osdStackModel != osdSingleStackOptimized)
  #if (osdNumberOfAllTasks > 0)
/* KB begin osekOsGetStackUsage (overwritten) */
/*-+--------------------------------------------------------------------------
 / osGetStackUsage
 / Funktion:
 /   - calculate maximum stack usage of task <taskId>
 / Parameter:
 / Returnvalue: calculated stack usage (in byte)
 /--------------------------------------------------------------------------*/

osqFunc1 osStackUsageType osqFunc2 osGetStackUsage(TaskType taskId)
{
   /* CovComment 4:  Function: osGetStackUsage */
#if (osdNumberOfTasksWithTaskStack > 0)
   osqPRAM1 osqPRAM2 osuint8 osqPRAM3 *stack;
#endif /*(osdNumberOfTasksWithTaskStack > 0)*/

   COVER(0x00B4)
#if (osdNumberOfTasksWithTaskStack > 0)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x00B5)
   osSysErrAssert(taskId < osdNumberOfTasksWithTaskStack, osdErrSUWrongTaskID)
#endif
   stack = (osqPRAM1 osqPRAM2 osuint8 osqPRAM3 *)oskTcbStackBottom[taskId]; 
   while ((stack < (osqPRAM1 osqPRAM2 osuint8 osqPRAM3 *)(oskTcbStackTop[taskId])) && (*stack == 0xAA)) 
   {
      COVER(0x00B6)
      stack++;
   }
   return (osStackUsageType)((osqPRAM1 osqPRAM2 osuint8 osqPRAM3 *)(oskTcbStackTop[taskId]) - stack); 
#else /*(osdNumberOfTasksWithTaskStack > 0)*/
   osdDummyRead(taskId) /* prevent from lint warning about unused argument */
   return (osStackUsageType) 0;
#endif /*(osdNumberOfTasksWithTaskStack > 0)*/
} /* end of osGetStackUsage */
/* KB end osekOsGetStackUsage */
  #endif
 #endif /* (osdStackModel == osdSingleStackOptimized) */

/* stack usage measurement is not availble in SingleStackOptimizedCS */
/* KB begin osekStackUsageSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osGetIntStackUsage
 / Funktion:
 /   - calculate maximum stack usage of interrupt stack
 / Parameter:
 / Returnvalue: calculated stack usage (in byte)
 /--------------------------------------------------------------------------*/
osStackUsageType osGetIntStackUsage(void)
{
   /* CovComment 4:  Function: osGetIntStackUsage */
   osuint32 i;
   osStackSizeType size = osdSystemStackSize/(osStackSizeType)sizeof(osStackDataType); 
   COVER(0x00B7)
   for( i = 0UL; ((i < size) && (osSystemStack[i] == 0xAAAAAAAAUL)); i++)
   {
      COVER(0x00B8)
   }
   COVER(0x00B9)
   return (osStackUsageType) (sizeof(osStackDataType)*(size - i)) ;
} /* end of osGetIntStackUsage */
/* KB end osekStackUsageSrc */
#endif /* osdStackUsageMeasurement */

#endif /* osdStackCheck */


/*----------------------------------------------------------------------------
 / osInitialize
 / Funktion:
 /   - Initialize all OS-variables, that are used by OS-API-functions which 
 /     might be used before StartOS() is called. This function should be used,
 /     if variables are not initialized by the startup-code, delivered with
 /     the compiler.
 / Parameter:   none
 / Returnvalue: none
 /--------------------------------------------------------------------------*/

/* MISRA RULE 14.1 not violated: (command 1503) The function osInitialize is general 
 * code, called from within the function osStartOSasm, which might be implemented 
 * in assembly language or in C language dependend on the implementation. Therefore the
 * MISRA checker might not see the call and wrongly state a violation of rule 14.1 */
/* MISRA RULE 8.10 VIOLATION: (command 1505) The function osInitialize is part of the 
 * described API of the OS. Therefore external linkage is required and it depends
 * on the application whether or not the function is ever called. */
osqFunc1 void osqFunc2 osInitialize(void)      /* PRQA S 1503 */ /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osInitialize */
   COVER(0x00BA)
   /* KB begin osekHWosInitialize (overwritten) */
   /* Depending on osdTimingProtectionUsed only one of the variables is used by API-functions */
   #if osdTimingProtectionUsed
      osIntSaveDisableHighLevelCounter = 0;
   #else /*osdTimingProtectionUsed*/
      osIntSaveDisableGlobalCounter = 0;
   #endif /*osdTimingProtectionUsed*/
   /* KB end osekHWosInitialize */
   osIntSaveDisableCounter = 0;
   #if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x00BB)
   osIntAPIStatus = 0;
   #endif
   
   #ifdef osdUseTrace
   COVER(0x00BC)
   osTraceBufferIndex = 0;
   #endif
} 


/* Necessary to Kill a task/ISR and to handle missing TerminateTask in SC3/4 */
#if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))

/*-+--------------------------------------------------------------------------
 / osSysKillResourceList
 / Funktion:
 /   - releases resource ResID and all resources locked by the same task/ISR
 /
 / Attention: function must be called with interrupts disabled!  
 /
 / Parameter: ResId : first resource to be released
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if (osdNumberOfAllResources > 0)
 #if (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))
/* MISRA RULE 8.10 VIOLATION: On some platforms, this function might only be accessed from within this file. 
   Catching all cases would be too difficult and error prone */
osqFunc1 void osqFunc2 osSysKillResourceList(ResourceType ResId) /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osSysKillResourceList */
   COVER(0x00BD)
   while (ResId != osdNoResource)
   {
      COVER(0x00BE)
#if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions && ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))) || osdORTIDebug
      COVER(0x00BF)
      osResActualTask[ResId]= osdNoActiveTask;
#endif
#if STATUS_LEVEL == EXTENDED_STATUS
 #if (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))
      COVER(0x00C0)
      osResOccupationCounter[ResId]= 0; /* not occupied */
  #if ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0)
      osResGlobalOccupationCounter--;
  #endif /* ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0) */
 #endif
#endif

#if (osdNumberOfInterruptResources > 0)
 #if (osdNumberOfTaskResources > 0)
      COVER(0x00C1)
      if(ResId>=osrNumberOfTaskResources)
 #endif      
      {
         COVER(0x00C2)
         /* switch from higher processing level */

         /* KB begin osekDecDispatcherLock (default) */
         #if (osdFullPreempt || osdMixedPreempt)
            COVER(0x00C3)
            osLockDispatcher--;
         #endif
         /* KB end osekDecDispatcherLock */

         #if STATUS_LEVEL == EXTENDED_STATUS
         /* counter needed only for mixed resource system (task, interrupt and internal) */
         
         /* decrement counter only if task is accessing */
         if (osIntNestingDepth == 0)
         {
            COVER(0x00C4)
            osTcbResourceCounter[osActiveTaskIndex]--;
         }
         #endif
         
         #if osdORTIDebug
         COVER(0x00C5)
         osResActualTask[ResId]= osdNoActiveTask;
         #endif

      }
 #if (osdNumberOfTaskResources > 0)
      else
      {
         COVER(0x00C6)
         /* stay on task level - business as usual */     
 #endif
#endif

#if osdRTSize == 1
         COVER(0x00C7)
         osQReadyPrios&= ~oskResCeilingPrioMask[ResId];
#else
         COVER(0x00C8)
         osQReadyPrios[oskResCeilingPrioOffset[ResId]]&= ~oskResCeilingPrioMask[ResId];
#endif

#if (osdNumberOfInterruptResources > 0)
 #if (osdNumberOfTaskResources > 0)
      }
 #endif
#endif
      COVER(0x00C9)
      /* remove resource from linked list */
      ResId = osResNextLockedResource[ResId];

   } /* while (ResId != osdNoResource) */
}
 #endif /* (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)) */
#endif /* (osdNumberOfAllResources > 0) */




/*-+--------------------------------------------------------------------------
 / osSysKillTask
 / Funktion:
 /   - kills task and releases all resources locked by the task
 /   - must be called with interrupts disabled!
 /
 / Parameter:     taskID : task to be killed
 /                issueError : osdTRUE: automatic task termination (missing TerminateTask),
 /                                   only current activation is killed; this is used only
 /                                   in SC3 and SC4
 /                             osdFALSE: Whole task (all activations) are killed.
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if !( (osdSC == SC3) || (osdSC == SC4) )
static void osSysKillTask(TaskType taskID, osbool issueError);
static
#endif
void osSysKillTask(TaskType taskID, osbool issueError)
{
   /* CovComment 4:  Function: osSysKillTask */
#if ((CC == ECC2) || (CC == BCC2))
   osPrioType priority;
#endif

   COVER(0x00CA)
   if (issueError == osdTRUE)
   {
      COVER(0x00CB)
      osAPIError(E_OS_MISSINGEND, osdErrMTMissingTerminateTask);
   }
#if ( (osdErrorHook!=0) || (osdComErrorHook!=0) )
   else
   {
      #if osdErrorHook
      COVER(0x00CC)
      /* Reset osErrorFlag, task might be killed in ErrorHook */
      osErrorFlag = osdFALSE;
      #endif
      #if osdComErrorHook
      COVER(0x00CD)
      osCOMErrorFlag=osdFALSE;
      #endif

   }
#else
   ELSEWITHCOVER(0x00CE)
#endif

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x00CF)
   if (osActiveTaskIndex == taskID)
   {  /* must be called before PostTaskHook to avoid IntAPI errors inside hook */
      COVER(0x00D0)
      osIntAPIStatus = 0;
   }
   ELSEWITHCOVER(0x00D1)
#endif

#if osdPostTaskHook
   if (issueError == osdTRUE)
   {
      COVER(0x00D2)
      /* call PostTaskHook before clearing osActiveTaskIndex! */
      osPostTaskHook();
   }
   ELSEWITHCOVER(0x00D3)
#endif

#if (osdTimingProtectionUsed != 0)
   if (issueError == osdTRUE)
   {
      COVER(0x00D4)
      osTPExeSwitchBack(INVALID_PROCESS);
      osTPSuspendTimerBlock();
   }
   ELSEWITHCOVER(0x00D5)
   osTPCleanListBlock(taskID);
#if(osdSC== SC4)
   if (osActiveTaskIndex != taskID)
   {
      COVER(0x00D6)
      osTPReloadExeBudget(taskID);
   }
   ELSEWITHCOVER(0x00D7)
#endif   
#endif

#if osdNumberOfInternalResources > 0
   if (oskResInternalCeilingPrioMask[taskID] != 0)
   {
   #if osdRTSize == 1
      COVER(0x00D8)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[taskID];
   #else
      COVER(0x00D9)
      osQReadyPrios[oskResInternalCeilingPrioOffset[taskID]] &=
         ~oskResInternalCeilingPrioMask[taskID];
   #endif
   #if ((CC == ECC2) || (CC == BCC2))
      if (osActiveTaskIndex == taskID)
      {
         COVER(0x00DA)
         osActiveTaskPrio=oskTcbHomePrio[taskID];
      }
      ELSEWITHCOVER(0x00DB)
   #endif
   
   #if osdORTIDebug
      COVER(0x00DC)
      osTcbActualPrio[taskID]=oskTcbHomePrio[taskID];
   #endif
   
   }
   ELSEWITHCOVER(0x00DD)
#endif /* osdNumberOfInternalResources > 0 */


#if (osdNumberOfAllResources > 0)
 #if (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))
   if (osTcbLockedResource[taskID] != osdNoResource)
   {
      COVER(0x00DE)
      osSysKillResourceList(osTcbLockedResource[taskID]);
   }
   ELSEWITHCOVER(0x00DF)
 #endif /* (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)) */
#endif /* (osdNumberOfAllResources > 0) */


#if STATUS_LEVEL == EXTENDED_STATUS
   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
      COVER(0x00E0)
      osTcbResourceCounter[taskID] = 0;
   #endif
#endif
#if osdORTIDebug
     COVER(0x00E1)
     osTcbActualPrio[taskID]=oskTcbHomePrio[taskID];
#endif
   osTcbLockedResource[taskID] = osdNoResource;

#if ((CC == ECC2) || (CC == BCC2))
  
   if (issueError == osdTRUE)
   {
      COVER(0x00E2)
      osTcbActivationCount[taskID]--;
   }
   else
   {
      COVER(0x00E3)
      osTcbActivationCount[taskID] = 0;
   }
   
   if (osTcbActivationCount[taskID] != 0)
   {  
      COVER(0x00E4)
      osTcbTaskState[taskID] = PRE_READY;
   }
   else
   {
      COVER(0x00E5)
      osTcbTaskState[taskID] = SUSPENDED;
   }
#else
   COVER(0x00E6)
   osTcbTaskState[taskID] = SUSPENDED;
#endif

  
   #if ((CC == ECC2) || (CC == BCC2))
   COVER(0x00E7)
   priority = oskTcbHomePrio[taskID];
   /* delete task out of the prio queue */
   osQReadyTaskHead[priority]++;
   if (osQReadyTaskHead[priority] > oskQMaxActivations[priority])
   {
      COVER(0x00E8)
      osQReadyTaskHead[priority]= 0;
   }
   ELSEWITHCOVER(0x00E9)
   
   if (osQReadyTaskHead[priority] == osQReadyTaskTail[priority])
   {  /* queue empty */
      COVER(0x00EA)
   #endif /* ((CC == ECC2) || (CC == BCC2)) */
   
   #if (osdRTSize > 1)
      COVER(0x00EB)
      osQReadyPrios[oskTcbReadyPrioOffset[taskID]]&= ~oskTcbReadyPrioMask[taskID];
   #else
      COVER(0x00EC)
      osQReadyPrios&= ~oskTcbReadyPrioMask[taskID];
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
   }
   ELSEWITHCOVER(0x00ED)
   #endif
   
   if (osActiveTaskIndex == taskID)
   {
      COVER(0x00EE)
      #if osdMixedPreempt
         if (oskTcbTaskPreemption[osActiveTaskIndex]==osdTaskTypeNonPreempt) 
         {   
            COVER(0x00EF)
            osLockDispatcher--; 
         } 
         ELSEWITHCOVER(0x00F0)
      #endif

      /* KB begin osekHWosKillRunnigTaskSpecialActionsSrc (overwritten) */
      #if ((osdSC== SC3) || (osdSC== SC4))
      #if (osdNumberOfCat2ISRs > 0)
         COVER(0x00F1)
         /* Are we on interrupt level? */
         if (osIntNestingDepth > 0)
         {
            osuint32 pcxiraw; 
            osuint32 * ppcxi; 
            COVER(0x00F2)
            /* 
             * Yes: manipulate the information, stored for the return to the task level, so that
             *      the dispatcher can become active with exactly this return. Necessary, as the 
             *      terminated running task might hold an interrupt resource.
             */
      
            /* set interrupt level to 0 when returning to task level */
            pcxiraw = osIsrStoredInfo[0].storedPcxi;
            ppcxi = (osuint32*) (((pcxiraw & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)));
            *(ppcxi+1) = (osuint32) &osDispatcherActivator; /* manipulate return address from ISR level to task level */
            *ppcxi &= 0x00FFffff; /* manipulate level on return to task */
         }
         ELSEWITHCOVER(0x00F3)
      #endif /* (osdNumberOfCat2ISRs > 0) */
      #endif /*((osdSC== SC3) || (osdSC== SC4))*/
      /* KB end osekHWosKillRunnigTaskSpecialActionsSrc */

      osActiveTaskIndex = osdNoActiveTask;
      osActiveTaskPrio = osdNoActivePrio;
      osStartDispatcher = 1;
   }
   ELSEWITHCOVER(0x00F4)
} /* END OF osSysKillTask */

#endif /* (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4)) */


#if (STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended)
 #if (osdNumberOfAllTasks > 0)
/* KB begin osekHWosCheckInterruptsEnabledSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osCheckInterruptsEnabled
 / Funktion: check if interrupts enabled
 /   - 
 / Parameter:
 / Returnvalue: TRUE   enabled, 
 /              FALSE  disabled 
 /--------------------------------------------------------------------------*/

osbool osCheckInterruptsEnabled(void)    /* return TRUE on task level */
{
   /* CovComment 4:  Function: osCheckInterruptsEnabled */
   osuint32 icr;
   COVER(0x00F5)
   os_dsync();
   icr=osd_mfcr(os_ICR_OFFSET);
   if((icr & 0x000001ffUL)==0x00000100UL) /*if ie bit enabled and level 0*/
   {
      COVER(0x00F6)
      return osdTRUE;
   }
   COVER(0x00F7)
   return osdFALSE;
} /* END OF osCheckInterruptsEnabled */
/* KB end osekHWosCheckInterruptsEnabledSrc */
 #endif

 #if osdEnableAssertions
/* KB begin osekHWosCheckInterruptsDisabledSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osCheckInterruptsDisabled
 / Funktion: check if interrupts disabled
 /   - 
 / Parameter:
 / Returnvalue: TRUE   disabled, 
 /              FALSE  enabled 
 /--------------------------------------------------------------------------*/
/* MISRA RULE 8.10 VIOLATION: In some configurations, this function may only be used 
   in one file. Catching all cases would make the code to complex and error-prone
 */
osbool osCheckInterruptsDisabled(void) /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osCheckInterruptsDisabled */
   osuint32 icr;
   COVER(0x00F8)
   os_dsync();
   icr=osd_mfcr(os_ICR_OFFSET);
   if((icr & 0x00000100UL)==0x00000000UL) /* check global interrupt Bit */
   {
      COVER(0x00F9)
      return osdTRUE;
   }
   if ((icr & 0x000000ffUL) >= ((osuint32)osdSystemLevel))
   {
      COVER(0x00FA)
      return osdTRUE;
   }
   COVER(0x00FB)
   return osdFALSE;
} /* END OF osCheckInterruptsEnabled */
/* KB end osekHWosCheckInterruptsDisabledSrc */
 #endif
#endif

/* KB begin osekHWosDisableLevelSrc (overwritten) */
/* KB end osekHWosDisableLevelSrc */

/* KB begin osekHWosEnableLevelSrc (overwritten) */
/* KB end osekHWosEnableLevelSrc */

/* KB begin osekHWosSetAlarmLevelSrc (overwritten) */
/* KB end osekHWosSetAlarmLevelSrc */

/* KB begin osekHWosSaveDisableLevelSrc (overwritten) */
/* KB end osekHWosSaveDisableLevelSrc */

/* KB begin osekHWosRestoreEnableLevelSrc (overwritten) */
/* KB end osekHWosRestoreEnableLevelSrc */

/* KB begin osekHWosDisableGlobalSrc (overwritten) */
/* KB end osekHWosDisableGlobalSrc */

/* KB begin osekHWosEnableGlobalSrc (overwritten) */
/* KB end osekHWosEnableGlobalSrc */

/* KB begin osekHWosSaveDisableLevelNestedSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osSaveDisableLevelNested
 / Funktion: save level and disable up to system level, nested
 /           = API function SuspendOSInterrupts 
 /   - 
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
/* MISRA RULE 14.1 VIOLATION: In some configurations, this function may be defined 
   but not used. Catching all configurations would make the code too complex and error-prone */
void osSaveDisableLevelNested(void) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osSaveDisableLevelNested */
   COVER(0x00FC)
   osSysErrAssert(osIntSaveDisableCounter < 255, osdErrSDWrongCounter)
   /* KB begin osekHWosSaveDisableLevelNestedBody (overwritten) */
   if (osIntSaveDisableCounter == 0) 
   {
      osIntLevelType intLevel;
      osuint32 icr;
   
      /* !!! save int-level to local variable before the system level is set */
      os_dsync();                                                                 
      icr = osd_mfcr(os_ICR_OFFSET);                                             
      intLevel = (osIntLevelType)(icr & 0x000000ffUL);                                              
      icr = (icr & 0xFFFFFF00UL) | ((osuint32)osdSystemLevel); /* PRQA S 3199 */ /* variable is used in assembler */                               
      osd_mtcr(os_ICR_OFFSET,icr);                                               
      /* the required isync instruction is executed by osd_mtcr */                                                     
      osSavedIntLevelNested = intLevel;                                               
      COVER(0x00FD)
   }
   osIntSaveDisableCounter++;
   /* KB end osekHWosSaveDisableLevelNestedBody */
}

/* KB end osekHWosSaveDisableLevelNestedSrc */

/* KB begin osekHWosRestoreEnableLevelNestedSrc (overwritten) */
/*-+--------------------------------------------------------------------------
 / osSaveDisableLevelNested
 / Funktion: restore level saved by osSaveDisableLevelNested
 /           = API function ResumeOSInterrupts 
 /   - 
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
 /* MISRA RULE 14.1 VIOLATION: In some configurations, this function may not be used. 
   Catching all cases would make the code to complex and error-prone.
 */
void osRestoreEnableLevelNested(void) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osRestoreEnableLevelNested */
   COVER(0x00FE)
   osSysErrAssert(osIntSaveDisableCounter!=0, osdErrREWrongCounter)
   
   /* KB begin osekHWosRestoreEnableLevelNestedBody (overwritten) */
   osIntSaveDisableCounter--;
   if (osIntSaveDisableCounter == 0)
   {
      COVER(0x00FF)
      osSetLevel(osSavedIntLevelNested);
      /* enable, if zero */
   }
   /* KB end osekHWosRestoreEnableLevelNestedBody */     
}

/* KB end osekHWosRestoreEnableLevelNestedSrc */

/* KB begin osekHWosSaveDisableGlobalNestedSrc (overwritten) */
#if (osdTimingProtectionUsed)
void osSaveDisableGlobalNested(void)
{
   /* CovComment 4:  Function: osSaveDisableGlobalNested */
   COVER(0x0100)
   osSysErrAssert(osIntSaveDisableGlobalCounter < 255, osdErrSGWrongCounter)
   /* KB begin osekHWosSaveDisableGlobalNestedBody (overwritten) */
   if (osIntSaveDisableGlobalCounter == 0) 
   {
      COVER(0x0101)
      os_dsync();
      if((osd_mfcr(os_ICR_OFFSET) & 0x00000100UL) != 0x00000000UL)
      {
      __disable();
      osSavedGlobalIntNested=1;
      COVER(0x0102)
      }
      else
      {
      osSavedGlobalIntNested=0;
      COVER(0x0103)
      }
   }
   osIntSaveDisableGlobalCounter++;
   /* KB end osekHWosSaveDisableGlobalNestedBody */
}
#endif /*(osdTimingProtectionUsed)*/
/* KB end osekHWosSaveDisableGlobalNestedSrc */

/* KB begin osekHWosRestoreEnableGlobalNestedSrc (overwritten) */
#if (osdTimingProtectionUsed)
void osRestoreEnableGlobalNested(void)
{
   /* CovComment 4:  Function: osRestoreEnableGlobalNested */
   COVER(0x0104)
   osSysErrAssert(osIntSaveDisableGlobalCounter!=0, osdErrRGWrongCounter)
   
   /* KB begin osekHWosRestoreEnableGlobalNestedBody (overwritten) */
   osIntSaveDisableGlobalCounter--;
   if (osIntSaveDisableGlobalCounter == 0)
   {  
      COVER(0x0105)
      if(osSavedGlobalIntNested!=0){
         COVER(0x0106)
         __enable();
      }
   }
   /* KB end osekHWosRestoreEnableGlobalNestedBody */     
}
#endif /*(osdTimingProtectionUsed)*/
/* KB end osekHWosRestoreEnableGlobalNestedSrc */

/* KB begin osekHWosDispatcherSrc (overwritten) */

#if ((osdSC== SC3) || (osdSC== SC4))
void osEnterProtectedISR(osuint16 intID)
{
   /* CovComment 4:  Function: osEnterProtectedISR  */
  osuint32 localSP;

  /* Get the stack pointer from A10 and save it */
  __asm("mov.d\t%0, a10":"=d"(localSP));
  osIsrStoredInfo[osIntNestingDepth].stackstart = localSP;

  /* save protection registers */
  osIsrStoredInfo[osIntNestingDepth].dpr1_0l = osd_mfcr(os_DPR1_0L_OFFSET);  /* private data lower address */
  osIsrStoredInfo[osIntNestingDepth].dpr1_0u = osd_mfcr(os_DPR1_0U_OFFSET);  /* private data upper address */
  osIsrStoredInfo[osIntNestingDepth].dpr1_1l = osd_mfcr(os_DPR1_1L_OFFSET);  /* stack lower address */
  osIsrStoredInfo[osIntNestingDepth].dpr1_1u = osd_mfcr(os_DPR1_1U_OFFSET);  /* stack upper address */
  osIsrStoredInfo[osIntNestingDepth].dpr1_2l = osd_mfcr(os_DPR1_2L_OFFSET);  /* application data lower address */
  osIsrStoredInfo[osIntNestingDepth].dpr1_2u = osd_mfcr(os_DPR1_2U_OFFSET);  /* application data upper address */

  if (oskIcbIsTrusted[intID] == 0)
  {
    /* set new protection for interrupt */
    osd_mtcr(os_DPR1_0L_OFFSET,oskISRMemoryAccessLow[intID][0]);     /* ISR private data lower address */
    osd_mtcr(os_DPR1_0U_OFFSET,oskISRMemoryAccessHigh[intID][0]);    /* ISR private data upper address */

    if ((localSP-oskIcbStackSize[intID]) > (osuint32)(&osSystemStack))
    {  /* use configured stacksize */
       osd_mtcr(os_DPR1_1L_OFFSET,localSP-oskIcbStackSize[intID]);       /* ISR stack lower address */
    }
    else
    {  /* limit to start of system stack */
       osd_mtcr(os_DPR1_1L_OFFSET, (osuint32)(&osSystemStack));        /* ISR stack lower address */
    }                                                            
    osd_mtcr(os_DPR1_1U_OFFSET,localSP);                                 /* ISR stack upper address */

    osd_mtcr(os_DPR1_2L_OFFSET,oskISRMemoryAccessLow[intID][2]);     /* application data lower address */
    osd_mtcr(os_DPR1_2U_OFFSET,oskISRMemoryAccessHigh[intID][2]);    /* application data upper address */
  }
}


#endif

#if (osdNumberOfInternalResources > 0)
/*-+--------------------------------------------------------------------------
 / osOccupyInternalResource
 / Funktion: osOccupyInternalResource (occupy internal resource)
 /   - occupies an internal resource, when a task becomes running and
 /     this task is configured to use an internal resource.
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
static void osOccupyInternalResource(void);
static void osOccupyInternalResource(void)
{
/* KB begin osOccupyInternalResource (default) */
#if (osdNumberOfInternalResources > 0)
if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
{
   osPrioType IntResCeilingPrio= oskResInternalCeilingPrio[osActiveTaskIndex];
#if (CC==BCC2) || (CC==ECC2)
   COVER(0x0107)
   oskQActivationQueues[IntResCeilingPrio][0]= osActiveTaskIndex;
#else
   COVER(0x0108)
   osQReadyTask[IntResCeilingPrio]= osActiveTaskIndex;
#endif

#if osdRTSize == 1
   COVER(0x0109)
   osQReadyPrios |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
#else
   COVER(0x010A)
   osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]] |= /*;*/
      oskResInternalCeilingPrioMask[osActiveTaskIndex];
#endif

   /* correct osHighReadyTaskPrio if required */
   if(IntResCeilingPrio<osHighReadyTaskPrio)
   {
      COVER(0x010B)
      osHighReadyTaskPrio = IntResCeilingPrio;
      osActiveTaskPrio  =   IntResCeilingPrio;
   }
   else
   {
      COVER(0x010C)
      osActiveTaskPrio  = osHighReadyTaskPrio;
   }

}
else
{
   COVER(0x010D)
#endif /* (osdNumberOfInternalResources > 0) */

   COVER(0x010E)
   osActiveTaskPrio  = osHighReadyTaskPrio;

#if (osdNumberOfInternalResources > 0)
}

#if osdORTIDebug
COVER(0x010F)
osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
#endif

#endif /*(osdNumberOfInternalResources > 0)*/


/* KB end osOccupyInternalResource */     
}
#endif


#if (osdStackModel == osdMultiStack) || (osdStackModel == osdSingleStackOsek)
/*-+--------------------------------------------------------------------------
 / osIdleLoop
 / Funktion: osIdleLoop (idle loop)
 /   - has no stack
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static void osIdleLoop(void);/*just to keep lint happy, function is only called from below*/
/*lint -esym(752,osIdleLoop)
 * Info 752 local declarator 'osIdleLoop(void)' not referenced
 *
 * This function is never activated the 'normal' way, as it
 * is activated from osDispatcher via a return from interrupt.
 *
 * Suppress the message.
 */

/* MISRA RULE 14.1 not violated: Function is referenced from assembler (QAC cannot detect this) */
static void osIdleLoop(void) /* PRQA S 3219 */ 
{
   /* CovComment 4:  Function: osIdleLoop  */
/*switch off infinite loop warning*/
#pragma warning 557
   
   OS_IDLE_ENTRY()
   COVER(0x0110)
   for(;;)
   {
      #if osdNonPreempt
      if (osStartDispatcher!=0)
      {
         COVER(0x0111)
         osDispatch()
      }
      #endif
      COVER(0x0112)
      OS_IDLE_PROC()
   }
}


#if (STATUS_LEVEL == EXTENDED_STATUS) 
#if ((osdSC== SC3) || (osdSC== SC4))
__asm( "\n"
       "osCallMissingTerminateError:\n"
       "   mov16.aa a2, a10\n"                         /* A2 = SP (A2 in lower context has same position 
                                                                   as A10 (SP) in upper context)            */
       "   svlcx;\n"                                   /* store a lower context                             */
       "   movh.a  a15,#@his(osInitialPcxiVal)\n"      /* A15 = &osInitialPcxiVal (upper address word only) */
       "   ld.w    d0,[a15]@los(osInitialPcxiVal)\n"   /* D0 = osInitialPcxiVal                             */
       "   mtcr    #65024,d0\n"                        /* PCXI = osInitialPcxiVal (change the stored context
                                                          to an upper one)                                  */

/*     pcxiadr = ((osInitialPcxiVal & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)); */
       "   movh    d1,#15\n"                           /* D1  = 0x000f0000                                  */
       "   and     d15,d1,d0\n"                        /* D15 = D1 & D0                                     */
       "   mov.u   d1,#65535\n"                        /* D1  = 0x0000ffff                                  */
       "   and     d2,d0,d1\n"                         /* D2  = D0 & D1                                     */
       "   sh      d15,d15,#12\n"                      /* D15 = D15 << 12                                   */
       "   sh16    d2,#6\n"                            /* D2  = D2 << 6                                     */
       "   or      d15,d2\n"                           /* D15|= D2                                          */

/*     ppcxi = (osuint32*)pcxiadr; */
       "   mov16.a a15,d15\n"                          /* A15 = D15                                         */
       "   dsync\n"                                    /* wait until svlcx has finished                     */
       "   isync\n"                                    /* isync after dsync for older TriCores              */

/*     *(ppcxi+1) = 0x00000A80UL;*//* Position of PSW in the saved (upper)context */
       "   mov     d15,#2688\n"                        /* D15 = 0x00000A80UL                                */
       "   st16.w  [a15]4,d15\n"                       /* *(A15+4) = D15                                    */

       "   j osMissingTerminateError;\n"               /* jump to function osMissingTerminateError          */
     );
#endif /*((osdSC== SC3) || (osdSC== SC4))*/
#endif /*(STATUS_LEVEL == EXTENDED_STATUS)*/

#if ((osdSC== SC3) || (osdSC== SC4))
#if (osdNumberOfCat2ISRs > 0)
/*-+--------------------------------------------------------------------------
 / osDispatcherActivator
 / Funktion: Activates the dispatcher. In case the currently running task is 
 /           terminated from interrupt level, the return address from ISR level 
 /           to task level is manipulated to end in this function. Therefore, 
 /           the terminated task does not become running anymore.
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
static void osDispatcherActivator(void)
{
   OS_START_DISPATCH()
   osDispatch()
}
#endif /* (osdNumberOfCat2ISRs > 0) */
#endif /*((osdSC== SC3) || (osdSC== SC4))*/


#if (osdVectorTableInRAM == 1)
#pragma section vector_init
#endif
/*-+--------------------------------------------------------------------------
 / osDispatcher
 / Funktion: Dispatcher (Delayed-Interrupt)
 /   - saves old task kontext
 /   - does not contain IDLE-Loop
 /   - switch to new task
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
/*
  * interrupt routine for priority 1, generate bisr-instruction to
  * save lower context, set system level and enable global 
  */
#if (osdStackModel == osdSingleStackOsek)
void osDispatcherFunction(void);
#endif /*(osdStackModel != osdSingleStackOsek)*/

/* MISRA RULE 14.1 not violated: (2 times) Function is referenced from assembler code (QAC cannot detect this) */
#if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
void __interrupt(1) __bisr_(osdInterruptVectorTableSize) osDispatcher(void)  /* PRQA S 1503 */
#else
void __interrupt(1) __bisr_(             osdSystemLevel) osDispatcher(void)  /* PRQA S 1503 */
#endif
{
   /* CovComment 4:  Function: osDispatcher  */
   /*lint --e{550} suppress :"Symbol not accessed" as lint can't see variable usage in inline assembly*/
    osuint32 pcxiraw; 
    osuint32 pcxiadr; 
    osuint32 * ppcxi; 

#if (osdStackModel == osdSingleStackOsek)
#if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
    osuint32 tmp; /* PRQA S 3203 */ /* variable is used in assembler code, QAC cannot detect this */
    COVER(0x0113)
    __asm( "mov.d   %0, a10" : "=d"(tmp));
    /* assure that the stack pointer is unchanged, i.e. this function needs no stack */
    osSysErrAssert((tmp == 
       (osuint32)&(osCat1IsrStack[ osdCat1IsrStackSize/sizeof(osStackDataType) ])) , 
       osdErrDIWrongStackPointer) /*lint !e530 "Symbol 'tmp' not initialized", done in inline asm */
#endif /*((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))*/

    COVER(0x0114)
    /*switch to Cat1IsrStack on interrupt entry*/
    osd_mtcr(os_PSW_OFFSET, 0x00000880UL );

    /* Is there a valid stack pointer in osCurrentBasicSP? */
    if (osCurrentBasicSP != 0)
    {
       COVER(0x0115)
       /* Yes: Take this one */
       __asm( "   mov.a  a10, %0 " :: "d"(osCurrentBasicSP): "a10" );
       osCurrentBasicSP = 0; /* currently running on the system stack */
    }
    else
    {
       COVER(0x0116)
       /* No: Get the current stack pointer from the stored context */
       pcxiraw=osd_mfcr(os_PCXI_OFFSET);   /* Get PCXI value */
       pcxiadr=((pcxiraw & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)); 
       ppcxi=(osuint32*)pcxiadr; 
       os_dsync(); /* wait until the context is written completely before reading from it */
       pcxiraw = *ppcxi;                   /* Get the PCXI register out of the lower context */
       pcxiadr=((pcxiraw & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)); 
       ppcxi=(osuint32*)pcxiadr; 
       __asm( "   mov.a  a10, %0 " :: "d"(*(ppcxi+2)): "a10" ); /* Get SP out of the upper context */
    }
    os_dsync();
    osIsOnSystemStack = 1;

    __asm("   j osDispatcherFunction");

}

/* 
 * So in case of osdSingleStackOsek osDispatcher is just a small function that performs the 
 * stack-switch and jumps to the 'real' dispatcher osDispatcherFunction 
 */
void osDispatcherFunction(void)
{
   /* CovComment 4:  Function: osDispatcherFunction  */
   /*lint --e{550} suppress :"Symbol not accessed" as lint can't see variable usage in inline assembly*/
    osuint32 pcxiraw; 
    osuint32 pcxiadr; 
    osuint32 * ppcxi; 
 
#endif /*(osdStackModel != osdSingleStackOsek)*/
    osuint32 tmp; /* PRQA S 3203 */ /* variable is accessed by assembler code, QAC cannot detect this*/

    COVER(0x0117)
    
    osStartDispatcher=0; /* reset osStartDispatcher */
    if (osActiveTaskIndex!=osdNoActiveTask)
    { 
       COVER(0x0118)
         /* valid task exists */
         /* context saving already done by compiler */
         #if (osdORTIDebug && (osdORTIVersion == 200))
            COVER(0x0119)
            osTcbORTIServiceId[osActiveTaskIndex]=osORTICurrentServiceId;  
         #endif

         #if osdStackCheck
             COVER(0x011A)
             /*In case of SingleStackModel, only extended tasks have own stacks to be checked*/
         #if  (osdStackModel == osdSingleStackOsek)
             COVER(0x011B)
         #if (((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))
             COVER(0x011C)
             if( osActiveTaskIndex < osdNumberOfExtendedTasks )
             {
                COVER(0x011D)
                if ( *(osuint32*)(oskTcbStackBottom[osActiveTaskIndex]) != 0xAAAAaaaaUL) 
                {
                   COVER(0x011E)
                   osStackOverflow();
                }
             }
         #endif
         #else /*osdStackModel == osdSingleStackOsek*/
            COVER(0x011F)

             /*In case of multi stack model, all tasks have own stacks to be checked*/
             if ( *(osuint32*)(oskTcbStackBottom[osActiveTaskIndex]) != 0xAAAAaaaaUL)
             { 
                COVER(0x0120)
                osStackOverflow(); 
             } 
         #endif /*osdStackModel == osdSingleStackOsek else*/
         #endif /*osdStackCheck*/

         #if ((osdSC== SC3) || (osdSC== SC4))
            COVER(0x0121)
            /* save the information, whether or not the task is in a system function (can only be a trusted function here) */
            osTcbInSystemFct[osActiveTaskIndex] = osInSystemFct;
            osInSystemFct = 0; /* we are not in a system function any longer */
         #endif

          if (osTcbTaskState[osActiveTaskIndex]==RUNNING) 
          { 
             COVER(0x0122)
             #if osdPostTaskHook
                COVER(0x0123)
                osPostTaskHook(); 
             #endif
             osTcbTaskState[osActiveTaskIndex]=READY; 
          } 
         os_dsync();

         /* save PCXI register */
         osTcbPCXI[osActiveTaskIndex]=(osStackPtrType)osd_mfcr(os_PCXI_OFFSET);
         
         #if osdTimingProtectionUsed
            COVER(0x0124)
            (void) osTPExeSwitchTo(INVALID_PROCESS);
            osTPSuspendTimerBlock();
         #endif

         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            osDisableLevel(); /* Set level _DOWN_ to system level (from maximum level) */
         #endif

    } 
    else /* if (osActiveTaskIndex!=osdNoActiveTask) */
    { 
         COVER(0x0125)

         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            osDisableLevel(); /* Set level _DOWN_ to system level (from maximum level) */
         #endif

         #if osdStackCheck
            COVER(0x0126)
         /*In case of SingleStackModel, only extended tasks have own stacks to be checked*/
         #if  (osdStackModel == osdSingleStackOsek)
            COVER(0x0127)
         #if (((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))
            COVER(0x0128)
            if( osLastActiveTaskIndex < osdNumberOfExtendedTasks )
            {
               COVER(0x0129)
               if ( *(osuint32*)(oskTcbStackBottom[osLastActiveTaskIndex]) != 0xAAAAaaaaUL) 
               {
                  COVER(0x012A)
                  osStackOverflow();
               }
            }
         #endif
         #else
             COVER(0x012B)
             if (*(osuint32*)(oskTcbStackBottom[osLastActiveTaskIndex]) != 0xAAAAaaaaUL)  
             { 
                COVER(0x012C)
                osStackOverflow(); 
             } 
         #endif
         #endif

         os_dsync();

         /* free CSA list */
         #if ( STATUS_LEVEL == EXTENDED_STATUS ) 
            COVER(0x012D)
            for ( pcxiraw=osd_mfcr(os_PCXI_OFFSET); pcxiraw!= osInitialPcxiVal; pcxiraw=osd_mfcr(os_PCXI_OFFSET) ) 
         #else 
            COVER(0x012E)
            for ( pcxiraw=osd_mfcr(os_PCXI_OFFSET); pcxiraw!=0x00000000UL     ; pcxiraw=osd_mfcr(os_PCXI_OFFSET) ) 
         #endif 
         { 
            COVER(0x012F)
            /* modification of CSA lists */
            osd_mtcr(os_PCXI_OFFSET,pcxiraw&~0x00400000UL); 
            /* the required isync instruction is executed by osd_mtcr */ 
            __rslcx(); 
         } 
    } /*if (osActiveTaskIndex!=osdNoActiveTask) else */

    /* In case of SingleStackModel check the system stack always, as the dispatcher
     * runs on this stack
     */
    #if osdStackCheck
    COVER(0x0130)
    #if  (osdStackModel == osdSingleStackOsek)
    COVER(0x0131)
    if( (*((osuint32*)osSystemStack)) != 0xAAAAaaaaUL )
    {
       COVER(0x0132)
       osIntStackOverflow();
    }
    #endif
    #endif /*osdStackCheck*/

    
    COVER(0x0133)
    osActiveTaskPrio  = osHighReadyTaskPrio; 
    osActiveTaskIndex = osHighReadyTaskIndex; 

      /* check for IDLE */
    if (osActiveTaskIndex==osdNoActiveTask) 
    {
       COVER(0x0134)
         #if (osdORTIDebug && (osdORTIVersion == 200))
             COVER(0x0135)
             osORTICurrentServiceId=osdORTINoService; 
         #endif
      
         #if osdMixedPreempt
             COVER(0x0136)
             osLockDispatcher=0; 
         #endif

         #if ((osdSC== SC3) || (osdSC== SC4))
            COVER(0x0137)
            osActiveApplicationID = INVALID_OSAPPLICATION;
            osInSystemFct = 0; /* we are not in a system function, we go into idle state */
         #endif

         /* modification of CSA lists */
         /* setup CSA for idle loop */
         /* reset PCXI to 0 */
         #if ( STATUS_LEVEL == EXTENDED_STATUS )
          COVER(0x0138)
          osd_mtcr(os_PCXI_OFFSET,osInitialPcxiVal); 
         #else
          COVER(0x0139)
          osd_mtcr(os_PCXI_OFFSET,0x00000000UL); 
         #endif
         /* the required isync instruction is executed by osd_mtcr */

         #if (osdStackModel == osdSingleStackOsek)
            COVER(0x013A)
            /* re-initialize osCurrentBasicSP, so after the Idle-Loop we start on an empty system stack */
            osCurrentBasicSP = (osuint32) &osSystemStack[ osdSystemStackSize/(osStackSizeType)sizeof(osStackDataType) ]; /* PRQA S 3683 */ /* index is intentionally one larger than array */
         #endif
         /* 
          * put current stack pointer (a10) into a2, note that a2 has the 
          * same position in the lower context as a10 has in the upper one
          */
          COVER(0x013B)
         __asm( "mov.aa   a2, a10" ::: "a2"); 
         os_dsync();
         __asm("  svlcx"); 

         /* get new PCXI value */
         pcxiraw=osd_mfcr(os_PCXI_OFFSET); 
         pcxiadr=((pcxiraw & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)); 
         ppcxi=(osuint32*)pcxiadr; 

         os_dsync(); /* wait until svlcx has written the full context before overwriting PSW in it */

         /* set initial PSW  */
         *(ppcxi+1)=0x00000880UL; /* supervisor permissions */

         /* set previous CPU interrupt level to 0 */
         pcxiraw&=0x00FFFFFFUL; /* needed by TC1767/97 */ 

         /* change the lower context block to upper context with interrupts enabled */
         pcxiraw|=0x00c00000UL; /* PRQA S 3199 */ /* variable is used from assembler code */
         osd_mtcr(os_PCXI_OFFSET,pcxiraw); 
         /* the required isync instruction is executed by osd_mtcr */

         /* put address of IdleLoop into return address register a11 */
         __asm( "mov.a   a11, %0" :: "d"((osuint32)osIdleLoop): "a11"); 

         /* put initial lower context into CSA list */
         __asm("  svlcx"); 
         /* return restores (lower and upper) context blocks */
         #if  (osdStackModel == osdSingleStackOsek)
            COVER(0x013C)
            /* return in assembly language, as the compiler does not know, we are in an interrupt function*/
            __asm("   rslcx");
            __asm("   nop");
            __asm("   rfe");
         #else
            COVER(0x013D)
            return;  
         #endif
    } /*if (osActiveTaskIndex==osdNoActiveTask) */


   COVER(0x013E)
   #if osdMixedPreempt
       COVER(0x013F)
       if (oskTcbTaskPreemption[osActiveTaskIndex]==osdTaskTypeNonPreempt) 
       {   
          COVER(0x0140)
          osLockDispatcher=1; 
       } 
       else 
       {   
          COVER(0x0141)
          osLockDispatcher=0; 
       } 
   #endif

   #if (osdNumberOfInternalResources > 0)
       COVER(0x0142)
       osOccupyInternalResource(); 
   #endif

   #if ((osdSC== SC3) || (osdSC== SC4))
      /* set application ID */
      osActiveApplicationID = oskTaskToAppl[osActiveTaskIndex];

      /* configure memory protection areas */
      osd_mtcr(os_DPR1_0L_OFFSET,oskTaskMemoryAccessLow[osActiveTaskIndex][0]);
      osd_mtcr(os_DPR1_0U_OFFSET,oskTaskMemoryAccessHigh[osActiveTaskIndex][0]);
      osd_mtcr(os_DPR1_1L_OFFSET,oskTaskMemoryAccessLow[osActiveTaskIndex][1]);
      osd_mtcr(os_DPR1_1U_OFFSET,oskTaskMemoryAccessHigh[osActiveTaskIndex][1]);
      osd_mtcr(os_DPR1_2L_OFFSET,oskTaskMemoryAccessLow[osActiveTaskIndex][2]);
      osd_mtcr(os_DPR1_2U_OFFSET,oskTaskMemoryAccessHigh[osActiveTaskIndex][2]);
   #endif

    if (osTcbTaskState[osActiveTaskIndex]==PRE_READY) 
    { 
         #if (osdORTIDebug && (osdORTIVersion == 200))
            osORTICurrentServiceId=osdORTINoService; 
         #endif

         #if ((osdSC== SC3) || (osdSC== SC4))
            COVER(0x0143)
            /* the current task is PRE_READY, so it is definitely not in a system function */
            osTcbInSystemFct[osActiveTaskIndex] = 0;
         #endif
         /* modification of CSA lists */
         /* reset PCXI */
         #if ( STATUS_LEVEL == EXTENDED_STATUS )
          osd_mtcr(os_PCXI_OFFSET,osInitialPcxiVal); 
         #else
          osd_mtcr(os_PCXI_OFFSET,0x00000000UL); 
         #endif
          __isync(); 
          /*
           * Note: A2 and A3 have the same positions in the lower context, as A10 and A11 
           *       in the upper context. So the values, the task should initially find
           *       in A10 (SP) and A11 (return-address) are moved to A2,A3, the lower
           *       context is saved and afterwards changed to an upper context.
           */
          #if  (osdStackModel == osdSingleStackOsek)
          #if (((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))
             if ( osActiveTaskIndex < osdNumberOfExtendedTasks )
             {
                /* switch to stack of extended task */
                tmp=(osuint32)oskTcbStackTop[osActiveTaskIndex]; /* PRQA S 3198 */ /* variable is accessed by assembler code, QAC cannot detect this*/
                __asm( "mov.a   a2, %0" :: "d"(tmp): "a2");
                /* store current stack-pointer in osCurrentBasicSP */
                __asm( "mov.d   %0, a10" : "=d"(osCurrentBasicSP));
                osIsOnSystemStack = 0; 
             }
             else
          #endif /*(((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))*/
             {
                /* this is a basic task, stay at current position on the system stack */
                __asm( "mov.aa   a2, a10" ::: "a2"); 
                /* store current stack-pointer in osTcbInitialSP[] */
                __asm( "mov.d   %0, a10" : "=d"(tmp)); 
                osTcbInitialSP[osActiveTaskIndex-osdNumberOfExtendedTasks] = (osStackPtrType) tmp;/*lint !e644 !e530*/
                /* lint warning 644: "Variable 'tmp' may not have been initialized" suppressed as init in asm */
                /* lint warning 530: "Symbol 'tmp' not initialized" suppressed as init in asm */
             }
          #else /*(osdStackModel == osdSingleStackOsek)*/
             tmp=(osuint32)oskTcbStackTop[osActiveTaskIndex]; /* PRQA S 3198 */ /* variable is accessed by assembler code, QAC cannot detect this*/ 
             __asm( "mov.a   a2, %0" :: "d"(tmp): "a2");
          #endif

          #if ( STATUS_LEVEL == EXTENDED_STATUS )
          #if ((osdSC== SC3) || (osdSC== SC4))
            __asm
            (
               /* move &osMissingTerminateError to a3 */
                "   movh.a   a3,#@his(osCallMissingTerminateError)\n"
                "   lea      a3,[a3]@los(osCallMissingTerminateError)\n"
            );
         #else /*((osdSC== SC3) || (osdSC== SC4))*/
            __asm
            (
               /* move &osMissingTerminateError to a3 */
                "   movh.a   a3,#@his(osMissingTerminateError)\n"
                "   lea      a3,[a3]@los(osMissingTerminateError)\n"
            );
         #endif /*((osdSC== SC3) || (osdSC== SC4))else*/
         #endif
         /*    __svlcx(); */
         __asm("  svlcx"); 

         /* get new PCXI value */
         pcxiraw=osd_mfcr(os_PCXI_OFFSET); 

         pcxiadr=((pcxiraw & 0x000f0000UL) << 12) | (((pcxiraw & 0x0000ffffUL) << 6)); 

         ppcxi=(osuint32*)pcxiadr; 

         os_dsync(); /* wait until svlcx has finished writing the context before writing of PSW in it */

         /* set initial PSW  */
         #if ((osdSC== SC3) || (osdSC== SC4))
         if (oskTcbIsTrusted[osActiveTaskIndex] == 0)
         {   /* non-trusted: set User-0 mode */
                 *(ppcxi+1)=0x00001081UL;  /* protection register set 1, user-0 permissions, call depth count = 1 */
         }
         else
         {   /* trusted: set supervisor mode */
         #endif         
             #if ( STATUS_LEVEL == EXTENDED_STATUS )
                 *(ppcxi+1)=0x00000881UL; /* protection register set 0, supervisor permissions, call depth count = 1 */
             #else
                 *(ppcxi+1)=0x00000880UL;  /* protection register set 0, supervisor permissions, call depth count = 0 */
             #endif
         #if ((osdSC== SC3) || (osdSC== SC4))
         }
         #endif         

         /* set previous CPU interrupt level to 0 */
         pcxiraw&=0x00FFFFFFUL; /* needed by TC1767/97 */ 
         
         /* change the lower context block to upper context */
         pcxiraw|=0x00c00000UL; /* PRQA S 3199 */ /* variable is accessed by assembler code, QAC cannot detect this */

         osd_mtcr(os_PCXI_OFFSET,pcxiraw); 
         /* the required isync instruction is executed by osd_mtcr */ 

         /* put return address in a11 */
         tmp=(osuint32)oskTcbTaskStartAddress[osActiveTaskIndex]; /* PRQA S 3199 */ /* variable is accessed by assembler code, QAC cannot detect this */

         __asm( "mov.a\ta11, %0" :: "d"(tmp): "a11"); 

         /*    __svlcx(); */
         __asm("  svlcx"); 
         /* return will restore context block */

         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            osDisableGlobal(); /* prevent from timing protection interrupts until task body is reached (rfe-instruction enables) */
         #endif

         #if osdTimingProtectionUsed
            (void)osTPExeSwitchTo(osActiveTaskIndex);
            /* osTPResumeTimerBlock not necessary as the task is started from PRE_READY state */;
         #endif
    } /*if (osTcbTaskState[osActiveTaskIndex]==PRE_READY)*/
    else 
    { 
       #if (osdORTIDebug && (osdORTIVersion == 200))
          osORTICurrentServiceId=osTcbORTIServiceId[osActiveTaskIndex];  
       #endif



       #if  (osdStackModel == osdSingleStackOsek)
       #if (((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0))
          if ( osActiveTaskIndex < osdNumberOfExtendedTasks )
          {
             /* store current stack-pointer in osCurrentBasicSP */
             __asm( "mov.d   %0, a10" : "=d"(osCurrentBasicSP)); 
             osIsOnSystemStack = 0;
          }
       #endif
       #endif
       
       /* restore PCXI */
       osd_mtcr(os_PCXI_OFFSET,(osuint32)osTcbPCXI[osActiveTaskIndex]); 

       __isync(); 

       #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
          osDisableGlobal(); /* prevent from timing protection interrupts until task body is reached (rfe-instruction enables) */
       #endif

       #if osdTimingProtectionUsed
          (void)osTPExeSwitchTo(osActiveTaskIndex);
          osTPResumeTimerBlock(osActiveTaskIndex);
       #endif
       /* restore lower context automatically done by compiler */
    } /*if (osTcbTaskState[osActiveTaskIndex]==PRE_READY) else*/ 

    osTcbTaskState[osActiveTaskIndex]=RUNNING; 

    #if osdPreTaskHook  
       osPreTaskHook(); 
    #endif

    #if ((osdSC== SC3) || (osdSC== SC4))
       COVER(0x0144)
       /* restore the information, whether or not the task is in a system function (can only be a trusted function here) */
       osInSystemFct = osTcbInSystemFct[osActiveTaskIndex];
       osCurrentContext = osdCtxTask;
    #endif

    #if  (osdStackModel == osdSingleStackOsek)
       /* return in assembly language, as the compiler does not know, we are in an interrupt function*/
       __asm("   rslcx");
       __asm("   nop");
       __asm("   rfe");
    #endif
}/* END OF osDispatcher */

#endif /*(osdStackModel == osdMultiStack) || (osdStackModel == osdSingleStackOsek)*/
/* KB end osekHWosDispatcherSrc */

/* KB begin osekHWAdditionalAPIFunctions (overwritten) */
#if (osdNumberOfApplications > 0)
/* return osdTRUE if address range is valid on current stack for application specific error hook */
osbool osApplErrStackAddressParamValid(osuint32 refAddr, osuint32 dataSize)
{
   /* CovComment 4:  Function: osApplErrStackAddressParamValid */

   if ((refAddr >= (osuint32)os_DPR1_1L_OFFSET)
       && ((refAddr+dataSize) <= (osuint32)os_DPR1_1U_OFFSET))
   {
      COVER(0x0145)
      return osdTRUE;
   }
   else
   {
      COVER(0x0146)
      return osdFALSE;
   }
}
#endif
/* KB end osekHWAdditionalAPIFunctions */

/* KB begin osekHWdelayTilEnableTookEffectSrc (default) */
/* KB end osekHWdelayTilEnableTookEffectSrc */


#if ((osdTimingProtectionUsed) || (osdSC== SC3) || (osdSC== SC4))

/* MISRA RULE 8.10 VIOLATION: The function osProtectionHookWrapper is a general implementation which is used 
 * identically on different plattforms. On some of them, it is called only in osek.c, on others it 
 * is called also from other files. Therefore it is made globally visible. The risk of potential 
 * name collisions is expected to be low.*/
ProtectionReturnType osProtectionHookWrapper(StatusType argErr)  /* PRQA S 1505 */
{
#if (osdProtectionHook)
    ProtectionReturnType proRet;
  #if ((osdSC == SC3) || (osdSC == SC4))
    osuint16 savedCtxt;

    COVER(0x0147)
    savedCtxt = osCurrentContext;
    osCurrentContext = osdCtxProtectionHook;
    osInSystemFct++;
  #endif  
#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
    COVER(0x0148)
    osIntAPIStatus = 0;
#endif                        
    COVER(0x0149)
    proRet = ProtectionHook(argErr);
  #if ((osdSC == SC3) || (osdSC == SC4))
    COVER(0x014A)
    osCurrentContext = savedCtxt;
    osInSystemFct--;
  #endif  

    /*
     * Check, if the desired action can be performed and change to another action
     * (in accordance with the standard) if necessary.
     */

    /* OS243: If the reaction is to kill the Task/Category 2 ISR ... */
    if (proRet == PRO_TERMINATETASKISR)
    {
       /* ... and no Task or ISR can be associated with the error, ... */
       if ((osIntNestingDepth == 0) && (osActiveTaskIndex == osdNoActiveTask))
       {
          /* ... the running OS-Application is killed.*/
          COVER(0x014B)
          proRet = PRO_TERMINATEAPPL;
       }
       ELSEWITHCOVER(0x014C)
    }
    ELSEWITHCOVER(0x014D)

    /* OS244: If the reaction is to kill the faulty OS-Application ... */
    if ((proRet == PRO_TERMINATEAPPL) || (proRet == PRO_TERMINATEAPPL_RESTART))
    {
       COVER(0x014E)
       #if ((osdSC == SC3) || (osdSC == SC4))
       /* ... and no OS-Application can be assigned, ... */
       if (osActiveApplicationID == INVALID_OSAPPLICATION)
       {
          COVER(0x014F)
       #else
          COVER(0x0150)
       #endif
          /* ... ShutdownOS()is called. */
          proRet = PRO_SHUTDOWN;
       #if ((osdSC == SC3) || (osdSC == SC4))
       }
       ELSEWITHCOVER(0x0151)
       #endif
    }
    ELSEWITHCOVER(0x0152)

    if ((proRet == PRO_IGNORE) && (argErr != E_OS_PROTECTION_ARRIVAL))
    {
       /* PRO_IGNORE allowed only for protection error E_OS_PROTECTION_ARRIVAL */
       COVER(0x0153)
       proRet = PRO_SHUTDOWN;
    }
    ELSEWITHCOVER(0x0154)

    #if ((osdSC == SC3) || (osdSC == SC4))
       /* Has the protection error occurred in an application specific shutdown hook? */
       if (osCurrentContext == osdCtxShutdownHook)
       {
          /* Yes: Only shutdown possible */
          COVER(0x0155)
          proRet = PRO_SHUTDOWN;
          /* (prevent the system from running further, if the shutdown-decision is already taken) */       
       }
       ELSEWITHCOVER(0x0156)
    #endif
    return proRet;
#else
    return PRO_SHUTDOWN;   /* Shutdown on protection error if no protection available [SR:047] */
#endif
}

/* KB begin osekHWosProtectionTrapSrc (overwritten) */
#if ((osdSC== SC3) || (osdSC== SC4))

/* MISRA RULE 14.1 not violated: Function is called from interrupt table (not recognized by MISRA tool) */
void __trap(1) osProtectionTrap( void ) /* PRQA S 1503 */
{
   COVER(0x0157)
   ProtectionReturnType proRet;
   proRet = osProtectionHookWrapper(E_OS_PROTECTION_MEMORY);
   osHandleProtectionErrors(proRet, E_OS_PROTECTION_MEMORY);
}

/* MISRA RULE 14.1 not violated: Function is called from interrupt table (not recognized by MISRA tool) */
void __trap(2) InstructionTrap(void) /* PRQA S 1503 */
{
   COVER(0x0158)
   ProtectionReturnType proRet;
   proRet = osProtectionHookWrapper(E_OS_PROTECTION_EXCEPTION);
   osHandleProtectionErrors(proRet, E_OS_PROTECTION_EXCEPTION);
}

#endif /*((osdSC== SC3) || (osdSC== SC4))*/
/* KB end osekHWosProtectionTrapSrc */
#endif /*((osdTimingProtectionUsed) || (osdSC== SC3) || (osdSC== SC4))*/

   
   
#if(osdTimingProtectionUsed || (osdSC == SC3) || (osdSC == SC4))
/*-+--------------------------------------------------------------------------
 / osHandleProtectionErrors
 / ====================
 /
 / Syntax: void osHandleProtectionErrors(ProtectionReturnType proRet, 
 /                                       StatusType Fatalerror)
 / Parameter (In): proRet: Protection hook reaction to be executed. 
 /                 Fatalerror: Error code for handled error
 / Return value: none 
 / Description: 
 / Handles the error reaction for a protection error 
 / Scalability Classes: SC2, SC3, SC4
 /--------------------------------------------------------------------------*/

 /* MISRA RULE 8.10 VIOLATION: The function osHandleProtectionErrors is a general implementation which is used 
 * identically on different plattforms. On some of them, it is called only in osek.c, on others it 
 * is called also from other files. Therefore it is made globally visible. The risk of potential 
 * name collisions is expected to be low.*/
void osHandleProtectionErrors(ProtectionReturnType proRet, StatusType Fatalerror)  /* PRQA S 1505 */
{
#if (((osdSC == SC3) || (osdSC == SC4)) && defined(osdProtectionHook))
#if (osdProtReactKillApplAllowed || osdProtReactKillApplRestAllowed) && (osdProtectionHook)
   /* Variable applToKill might be unused in some configurations.
    * To reduce code complexity this function is not removed for complex configurations.
    */
   ApplicationType applToKill;  /* PRQA S 3205 */
#endif
#endif
#if ((osdSC== SC3) || (osdSC== SC4))
   if ((((osdCtxErrorHookFromStartupHook | osdCtxStartupHook) & osCurrentContext) != 0)
       && (proRet != PRO_IGNORE))
   {  /* always Shutdown for protection errors in StartupHook */
      COVER(0x0159)
      proRet = PRO_SHUTDOWN;
   }
   ELSEWITHCOVER(0x015A)
#endif
   
   switch (proRet)
   {
      #if osdProtectionHook   
      #if osdProtReactKillTaskIsrAllowed
         case PRO_TERMINATETASKISR:
            #if osdComErrorHook
            COVER(0x015B)
            osCOMErrorFlag=osdFALSE;
            #endif
            #if osdErrorHook
            COVER(0x015C)
            osErrorFlag = osdFALSE;
            #endif


            /* KB begin osekHWosTPStopTimerExeInTermAppl (default) */
            #if (osdTimingProtectionUsed != 0)
               COVER(0x015D)
               {
                  osProcessType Proc;
                  Proc = osTPExeSwitchTo(INVALID_PROCESS);
                  if (Proc != INVALID_PROCESS)
                  {
                     COVER(0x015E)
                     osTPSuspendTimerBlock();
                     osTPReloadExeBudget(Proc);
                     osTPCleanListBlock(Proc);
                  }
                  ELSEWITHCOVER(0x015F)
               }
            #endif
            /* KB end osekHWosTPStopTimerExeInTermAppl */

            if (osIntNestingDepth == 0)
            {  /* Protection error inside task */
                #if (osdNumberOfAllTasks > 0)
                   COVER(0x0160)
                   osSysKillTask(osActiveTaskIndex, osdFALSE);       /* remove current task */

                   (void)osSchedulePrio();  /* no result check required because running task is terminated */
                   osIntSaveDisableCounter = 0;
                   /* KB begin osekHWInitGlobalDisableCounter (overwritten) */
                   #if osdTimingProtectionUsed
                   osIntSaveDisableHighLevelCounter = 0;
                   #else
                   osIntSaveDisableGlobalCounter = 0;
                   #endif
                   /* KB end osekHWInitGlobalDisableCounter */

                   /* enable task level interrupt */
                   /* KB begin osekHWosEnableLevel (overwritten) */
                   /* enable task level interrupt */
                   osEnableLevel();   
                   /* KB end osekHWosEnableLevel */
                   /* start dispatcher */
                   OS_START_DISPATCH()
   #if (osdTimingProtectionUsed != 0)
                   if (Fatalerror == E_OS_PROTECTION_ARRIVAL)
                   {
                      COVER(0x0161)
                      /* KB begin osekHWosEnableGlobalAndTimProtInterrupt (overwritten) */
                      osEnableGlobal();
                      /* KB end osekHWosEnableGlobalAndTimProtInterrupt */
                      /* normal dispatcher activation as this function was called from task level */
                      /* KB begin osekHWcallosDispatcher (default) */
                         /* KB begin osekHWosDispatcher (overwritten) */
                         osDispatch()
                         /* KB end osekHWosDispatcher */
                      /* KB end osekHWcallosDispatcher */
                   }
                   else if ((Fatalerror == E_OS_PROTECTION_TIME)   || 
                            (Fatalerror == E_OS_PROTECTION_LOCKED)    )
                   {
                      COVER(0x0162)
                      /* return from the timing protectin interrupt with dispatcher activation */
                      /* KB begin osekHWRet2DispFromTimeException (overwritten) */
                      osEnableGlobal();
                      osDispatch()
                      /* KB end osekHWRet2DispFromTimeException */
                   }
                   else
   #else
                   COVER(0x0163)
   #endif
                   {
                      COVER(0x0164)
                      /* return from the memory protection exception with dispatcher activation */
                      /* KB begin osekHWRet2DispFromProtException (overwritten) */
                      osEnableGlobal();
                      osDispatch()
                      /* KB end osekHWRet2DispFromProtException */
                   }
                #endif 
            }
#if (osdNumberOfCat2ISRs > 0)
            else
            {
               /* No: kill the ISR */
#if osdKillTaskIsrPossible
#if (osdTimingProtectionUsed != 0)
               if (Fatalerror == E_OS_PROTECTION_ARRIVAL)
               {
                  COVER(0x0165)
                  /* Nothing to be killed, the ISR is not performed anyhow */
                  return;
               }
#endif
#endif
#if (osdNumberOfAllResources > 0)
               if (osIcbLockedResource[osActiveISRID] != osdNoResource)
               {
                  COVER(0x0166)
 #if (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))
                  osSysKillResourceList(osIcbLockedResource[osActiveISRID]);
 #endif /* (!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)) */
                  osIcbLockedResource[osActiveISRID] = osdNoResource;
               }
               ELSEWITHCOVER(0x0167)
#endif                        
#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
               osIntAPIStatus = 0;
#endif                        
               osIntSaveDisableCounter = 0;
               /* KB begin osekHWInitGlobalDisableCounter (overwritten) */
               #if osdTimingProtectionUsed
               osIntSaveDisableHighLevelCounter = 0;
               #else
               osIntSaveDisableGlobalCounter = 0;
               #endif
               /* KB end osekHWInitGlobalDisableCounter */

#if osdKillTaskIsrPossible
#if (osdTimingProtectionUsed != 0)
               if ((Fatalerror == E_OS_PROTECTION_TIME) || (Fatalerror == E_OS_PROTECTION_LOCKED))
               {
                  COVER(0x0168)
                  /* KB begin osekHWLeaveKilledISRFromTimeException (overwritten) */
                  osLeaveISR();
                  #if osdTimingProtectionUsed
                     osTPExeSwitchBack(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                     osTPResumeTimerBlock(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                  #endif
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
                  /* KB end osekHWLeaveKilledISRFromTimeException */
               }
               else
#endif
#endif
               {
                  COVER(0x0169)
                  /* KB begin osekHWLeaveKilledISRFromProtException (overwritten) */
                  osLeaveISR();
                  #if osdTimingProtectionUsed
                     osTPExeSwitchBack(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                     osTPResumeTimerBlock(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                  #endif
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
                  /* KB end osekHWLeaveKilledISRFromProtException */
               }
            }
#endif /* #if (osdNumberOfCat2ISRs > 0) */
         break;
#endif /* osdProtReactKillTaskIsrAllowed */
#if (((osdSC== SC3) || (osdSC== SC4)) && (osdProtReactKillApplAllowed || osdProtReactKillApplRestAllowed))
#if (osdProtReactKillApplAllowed)
         case PRO_TERMINATEAPPL:
#endif      
#if (osdProtReactKillApplRestAllowed)
         case PRO_TERMINATEAPPL_RESTART:
#endif
            applToKill = osActiveApplicationID;

            /* KB begin osekHWosTPStopTimerExeInTermAppl (default) */
            #if (osdTimingProtectionUsed != 0)
               COVER(0x016A)
               {
                  osProcessType Proc;
                  Proc = osTPExeSwitchTo(INVALID_PROCESS);
                  if (Proc != INVALID_PROCESS)
                  {
                     COVER(0x016B)
                     osTPSuspendTimerBlock();
                     osTPReloadExeBudget(Proc);
                     osTPCleanListBlock(Proc);
                  }
                  ELSEWITHCOVER(0x016C)
               }
            #endif
            /* KB end osekHWosTPStopTimerExeInTermAppl */

            if (proRet == PRO_TERMINATEAPPL)
            {
               COVER(0x016D)
               osSysKillApplikation(applToKill, NO_RESTART);
            }
            else
            {
               COVER(0x016E)
               osSysKillApplikation(applToKill, RESTART);
            }
            
            #if (osdNumberOfAllTasks > 0)
               (void)osSchedulePrio();  /* no result check required because running task is terminated */
            #endif
            osIntSaveDisableCounter = 0;
            /* KB begin osekHWInitGlobalDisableCounter (overwritten) */
            #if osdTimingProtectionUsed
            osIntSaveDisableHighLevelCounter = 0;
            #else
            osIntSaveDisableGlobalCounter = 0;
            #endif
            /* KB end osekHWInitGlobalDisableCounter */

#if (osdNumberOfCat2ISRs > 0)
            COVER(0x016F)
            if (osIntNestingDepth > 0)
            {
#if (osdTimingProtectionUsed != 0)
               if (Fatalerror == E_OS_PROTECTION_ARRIVAL)
               {
                  COVER(0x0170)
                  /* return to the c-wrapper, the user ISR does not become active */
                  return;
               }
               else if ((Fatalerror == E_OS_PROTECTION_TIME)   || 
                        (Fatalerror == E_OS_PROTECTION_LOCKED)    )
               {
                  COVER(0x0171)
                  /* return from the timing protection interrupt with dispatcher activation */
                  /* KB begin osekHWLeaveKilledISRFromTimeException (overwritten) */
                  osLeaveISR();
                  #if osdTimingProtectionUsed
                     osTPExeSwitchBack(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                     osTPResumeTimerBlock(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                  #endif
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
                  /* KB end osekHWLeaveKilledISRFromTimeException */
               }
               else
#else
               COVER(0x0172)
#endif		
               {
                  COVER(0x0173)
                  /* KB begin osekHWLeaveKilledISRFromProtException (overwritten) */
                  osLeaveISR();
                  #if osdTimingProtectionUsed
                     osTPExeSwitchBack(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                     osTPResumeTimerBlock(osIsrStoredInfo[osIntNestingDepth].ProcTP);
                  #endif
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
                  /* KB end osekHWLeaveKilledISRFromProtException */
               }
            }     
            ELSEWITHCOVER(0x0174)
#endif /* (osdNumberOfCat2ISRs > 0) */

            /* KB begin osekHWosEnableLevel (overwritten) */
            /* enable task level interrupt */
            osEnableLevel();   
            /* KB end osekHWosEnableLevel */
            /* start dispatcher */
            OS_START_DISPATCH()
#if (osdTimingProtectionUsed != 0)
            if (Fatalerror == E_OS_PROTECTION_ARRIVAL)
            {
               COVER(0x0175)
               /* normal dispatcher activation as this function was called from task level */
               /* KB begin osekHWcallosDispatcher (default) */
                  /* KB begin osekHWosDispatcher (overwritten) */
                  osDispatch()
                  /* KB end osekHWosDispatcher */
               /* KB end osekHWcallosDispatcher */
            }
            else if ((Fatalerror == E_OS_PROTECTION_TIME)   || 
                     (Fatalerror == E_OS_PROTECTION_LOCKED)    )
            {
               COVER(0x0176)
               /* return from the timing protectin interrupt with dispatcher activation */
               /* KB begin osekHWRet2DispFromTimeException (overwritten) */
               osEnableGlobal();
               osDispatch()
               /* KB end osekHWRet2DispFromTimeException */
            }
            else
#else
            COVER(0x0177)
#endif		
            {
               COVER(0x0178)
               /* return from the memory protectin exception with dispatcher activation */
               /* KB begin osekHWRet2DispFromProtException (overwritten) */
               osEnableGlobal();
               osDispatch()
               /* KB end osekHWRet2DispFromProtException */
            }
            
         break;
      #endif
         case PRO_IGNORE:
            return;
      #endif /* osdProtectionHook */
      
      #if osdProtReactShutdownAllowed
         case PRO_SHUTDOWN:
            #if (osdShutdownHook || (osdNumberOfApplications > 0))
               COVER(0x0179)
               osSysShutdownOS(Fatalerror);
            #else
               COVER(0x017A)
               osSysShutdownOS();
            #endif
           /* MISRA RULE 14.1 VIOLATION: The break statement is not reachable but 
            * increases readability and maintainability (3201) */
            break;   /* PRQA S 3201 */
      #endif
         default:
            #if (osdShutdownHook || (osdNumberOfApplications > 0))
               COVER(0x017B)
               osSysShutdownOS(Fatalerror);
            #else
               COVER(0x017C)
               osSysShutdownOS();
            #endif

   }/* switch (proRet) */
}
#endif /*(osdTimingProtectionUsed || (osdSC == SC3) || (osdSC == SC4))*/
   
#if ((osdSC== SC3) || (osdSC== SC4))
/* KB begin osekHWosSysDisableISRSourceSrc (overwritten) */
osqFunc1 void osqFunc2 osSysDisableISRSource(ISRType isrID)
{
   COVER(0x017D)
   
   (*((volatile osuint32*)((void*)(osSRCtab[isrID])))) &= 0xFFFFEFFFUL;/* clear interrupt enable flag */

}
/* KB end osekHWosSysDisableISRSourceSrc */
#endif /*((osdSC== SC3) || (osdSC== SC4))*/


#ifndef osdEnableAllInterruptsNotUsed
/*-+--------------------------------------------------------------------------
 / osEnableAllInterrupts
 / Function:    implementation of API function EnableAllInterrupts
 / Parameter:   void
 / Returnvalue: void
 /--------------------------------------------------------------------------*/
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/EnableAllInterrupts
 */
osqFunc1 void osqFunc2 osEnableAllInterrupts(void)
{ 
   /* CovComment 4:  Function: osEnableAllInterrupts */
   OS_EA_ENTRY()
   COVER(0x017E)
   osSysErrAssert(osIntAPIStatus==osdIntAPIDisableAll,osdErrEAIntAPIWrongSequence)
#if ((osdEnableAssertions == 0) && (STATUS_LEVEL == EXTENDED_STATUS))
   if (osIntAPIStatus!=osdIntAPIDisableAll)
   {
      COVER(0x017F)
      OS_EA_EXIT()
      return;
   }
   ELSEWITHCOVER(0x0180)
#endif
   osSetIntAPIStatus(0);
#if osdTimingProtectionUsed /* implies osdSC=SC4*/
   COVER(0x0181)
   osTPStopTimerBlock(osdRESID_ALL_INTERRUPTS);
   /* KB begin osekHWosRestoreEnableHighLevelBody (overwritten) */
   COVER(0x0182)
   osRestoreEnableHighLevel();/* This is a define-macro, so no extra call is needed */
   /* KB end osekHWosRestoreEnableHighLevelBody */
#else
   COVER(0x0183)
   /* KB begin osekHWosRestoreEnableGlobalBody (overwritten) */
   if(osSavedGlobalInt!=0)
   {
      COVER(0x0184)
      __enable();
   }
   /* KB end osekHWosRestoreEnableGlobalBody */
#endif
   OS_EA_EXIT()
}
#endif


#ifndef osdDisableAllInterruptsNotUsed
/*-+--------------------------------------------------------------------------
 / osDisableAllInterrupts
 / Function:    implementation of API function DisableAllInterrupts
 / Parameter:   void
 / Returnvalue: void
 /--------------------------------------------------------------------------*/
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/DisableAllInterrupts
 */
osqFunc1 void osqFunc2 osDisableAllInterrupts(void)
{
   /* CovComment 4:  Function: osDisableAllInterrupts */
   OS_DA_ENTRY()
   COVER(0x0185)
   osSysErrAssert(!osIntAPIStatus,osdErrDAIntAPIDisabled)
#if ((osdEnableAssertions == 0) && (STATUS_LEVEL == EXTENDED_STATUS))
   if (osIntAPIStatus!=0)
   {
      COVER(0x0186)
      OS_DA_EXIT()
      return;
   }
   ELSEWITHCOVER(0x0187)
#endif
#if osdTimingProtectionUsed
   COVER(0x0188)
   /* KB begin osekHWosSaveDisableHighLevelBody (overwritten) */
   {
      osIntLevelType intLevel;                                                  
      intLevel = osGetLevel();                                           
      osSetLevel(osdHighLevel);
      osSavedIntHighLevel = intLevel;
   }
   /* KB end osekHWosSaveDisableHighLevelBody */
   osTPStartTimerBlock(osdRESID_ALL_INTERRUPTS);
#else
   COVER(0x0189)
   /* KB begin osekHWosSaveDisableGlobalBody (overwritten) */
      os_dsync();
      if((osd_mfcr(os_ICR_OFFSET) & 0x00000100UL) != 0x00000000UL )
      {
         __disable();
         osSavedGlobalInt=1;
         COVER(0x018A)
      }
      else
      {
         osSavedGlobalInt=0;
         COVER(0x018B)
      }
   /* KB end osekHWosSaveDisableGlobalBody */
#endif   
   osSetIntAPIStatus(osdIntAPIDisableAll);
   OS_DA_EXIT()
}
#endif


#ifndef osdResumeOSInterruptsNotUsed
/*-+--------------------------------------------------------------------------
 / osResumeOSInterrupts
 / Function:    implementation of API function ResumeOSInterrupts
 / Parameter:   void
 / Returnvalue: void
 /--------------------------------------------------------------------------*/
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/ResumeOSInterrupts
 */
osqFunc1 void osqFunc2 osResumeOSInterrupts(void) 
{
   /* CovComment 4:  Function: osResumeOSInterrupts */
   OS_RI_ENTRY()
   COVER(0x018C)
#if (osdEnableAssertions == 0)
   if (osIntSaveDisableCounter==0)
   {
      COVER(0x018D)
      OS_RI_EXIT()
      return;
   }
   ELSEWITHCOVER(0x018E)
#endif
   
   osDecIntAPIStatus();
#if osdTimingProtectionUsed
   if (osIntSaveDisableCounter == 1)
   {
      COVER(0x018F)
      osTPStopTimerBlock(osdRESID_OS_INTERRUPTS);
   }
   ELSEWITHCOVER(0x0190)
#endif
   /* KB begin osekHWosRestoreEnableLevelNestedBody (overwritten) */
   osIntSaveDisableCounter--;
   if (osIntSaveDisableCounter == 0)
   {
      COVER(0x0191)
      osSetLevel(osSavedIntLevelNested);
      /* enable, if zero */
   }
   /* KB end osekHWosRestoreEnableLevelNestedBody */
   OS_RI_EXIT()
}
#endif


#ifndef osdSuspendOSInterruptsNotUsed
/*-+--------------------------------------------------------------------------
 / osSuspendOSInterrupts
 / Function:    implementation of API function SuspendOSInterrupts
 / Parameter:   void
 / Returnvalue: void
 /--------------------------------------------------------------------------*/
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/SuspendOSInterrupts
 */
osqFunc1 void osqFunc2 osSuspendOSInterrupts(void)
{
   /* CovComment 4:  Function: osSuspendOSInterrupts */
   OS_SI_ENTRY()
   COVER(0x0192)
#if (osdEnableAssertions == 0)
   if (osIntSaveDisableCounter==255)
   {
      COVER(0x0193)
      OS_SI_EXIT()
      return;
   }
   ELSEWITHCOVER(0x0194)
#endif
   /* KB begin osekHWosSaveDisableLevelNestedBody (overwritten) */
   if (osIntSaveDisableCounter == 0) 
   {
      osIntLevelType intLevel;
      osuint32 icr;
   
      /* !!! save int-level to local variable before the system level is set */
      os_dsync();                                                                 
      icr = osd_mfcr(os_ICR_OFFSET);                                             
      intLevel = (osIntLevelType)(icr & 0x000000ffUL);                                              
      icr = (icr & 0xFFFFFF00UL) | ((osuint32)osdSystemLevel); /* PRQA S 3199 */ /* variable is used in assembler */                               
      osd_mtcr(os_ICR_OFFSET,icr);                                               
      /* the required isync instruction is executed by osd_mtcr */                                                     
      osSavedIntLevelNested = intLevel;                                               
      COVER(0x0195)
   }
   osIntSaveDisableCounter++;
   /* KB end osekHWosSaveDisableLevelNestedBody */
#if osdTimingProtectionUsed
   COVER(0x0196)
   if (osIntSaveDisableCounter == 1)
   {
      COVER(0x0197)
      osTPStartTimerBlock(osdRESID_OS_INTERRUPTS);
   }
   ELSEWITHCOVER(0x0198)
#endif   
   osIncIntAPIStatus();
   OS_SI_EXIT()
}
#endif


#ifndef osdResumeAllInterruptsNotUsed
/*-+--------------------------------------------------------------------------
 / osResumeAllInterrupts
 / Function:    implementation of API function ResumeAllInterrupts
 / Parameter:   void
 / Returnvalue: void
 /--------------------------------------------------------------------------*/
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/ResumeAllInterrupts
 */
osqFunc1 void osqFunc2 osResumeAllInterrupts(void)
{
   /* CovComment 4:  Function: osResumeAllInterrupts */
   OS_RA_ENTRY()
   COVER(0x0199)
#if (osdEnableAssertions == 0)
#if osdTimingProtectionUsed
   if (osIntSaveDisableHighLevelCounter==0)
   {
      COVER(0x019A)
      OS_RA_EXIT()
      return;
   }
   ELSEWITHCOVER(0x019B)
#else /* osdTimingProtectionUsed */
   if (0==
       /* KB begin osekHWosIntSaveDisableGlobalCounterName (overwritten) */
       osIntSaveDisableGlobalCounter
       /* KB end osekHWosIntSaveDisableGlobalCounterName */
      )
   {
      COVER(0x019C)
      OS_RA_EXIT()
      return;
   }
   ELSEWITHCOVER(0x019D)
#endif /*osdTimingProtectionUsed else*/
#endif /*osdEnableAssertions == 0*/

   osDecIntAPIStatus();
#if osdTimingProtectionUsed
   if (osIntSaveDisableHighLevelCounter == 1) /* first nesting level */
   {
      COVER(0x019E)
      osTPStopTimerBlock(osdRESID_ALL_INTERRUPTS);
   }
   ELSEWITHCOVER(0x019F)
   /* KB begin osekHWosRestoreEnableHighLevelNestedBody (overwritten) */
   COVER(0x01A0)
   osSysErrAssert(osIntSaveDisableHighLevelCounter!=0, osdErrRGWrongCounter);
   osIntSaveDisableHighLevelCounter--;
   if (osIntSaveDisableHighLevelCounter == 0)
   {  
      COVER(0x01A1)
      osSetLevel(osSavedIntHighLevelNested);
      /* enable, if zero */
   }
   ELSEWITHCOVER(0x01A2)
   /* KB end osekHWosRestoreEnableHighLevelNestedBody */
#else
   COVER(0x01A3)
   /* KB begin osekHWosRestoreEnableGlobalNestedBody (overwritten) */
   osIntSaveDisableGlobalCounter--;
   if (osIntSaveDisableGlobalCounter == 0)
   {  
      COVER(0x01A4)
      if(osSavedGlobalIntNested!=0){
         COVER(0x01A5)
         __enable();
      }
   }
   /* KB end osekHWosRestoreEnableGlobalNestedBody */
#endif   
   OS_RA_EXIT()
}
#endif


#ifndef osdSuspendAllInterruptsNotUsed
/*-+--------------------------------------------------------------------------
 / osSuspendAllInterrupts
 / Function:    implementation of API function SuspendAllInterrupts
 / Parameter:   void
 / Returnvalue: void
 /--------------------------------------------------------------------------*/
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/SuspendAllInterrupts
 */
osqFunc1 void osqFunc2 osSuspendAllInterrupts(void)
{
   /* CovComment 4:  Function: osSuspendAllInterrupts */
   OS_PA_ENTRY()
   COVER(0x01A6)
#if (osdEnableAssertions == 0)
#if osdTimingProtectionUsed
   if (osIntSaveDisableHighLevelCounter==255)
   {
      COVER(0x01A7)
      OS_PA_EXIT()
      return;
   }
   ELSEWITHCOVER(0x01A8)
#else /* osdTimingProtectionUsed */
   if (255==
       /* KB begin osekHWosIntSaveDisableGlobalCounterName (overwritten) */
       osIntSaveDisableGlobalCounter
       /* KB end osekHWosIntSaveDisableGlobalCounterName */
      )
   {
      COVER(0x01A9)
      OS_PA_EXIT()
      return;
   }
   ELSEWITHCOVER(0x01AA)
#endif /*osdTimingProtectionUsed else*/
#endif /*osdEnableAssertions == 0*/

#if osdTimingProtectionUsed
   /* KB begin osekHWosSaveDisableHighLevelNestedBody (overwritten) */
   COVER(0x01AB)
   osSysErrAssert(osIntSaveDisableHighLevelCounter < 255, osdErrSGWrongCounter)
   if (osIntSaveDisableHighLevelCounter == 0) 
   {
      osIntLevelType intLevel;
      
      COVER(0x01AC)
      /* save int-level to local variable before the system level is set */
      intLevel = osGetLevel();                                              
      osSetLevel(osdHighLevel);                                 
      osSavedIntHighLevelNested = intLevel;                                               
   }
   ELSEWITHCOVER(0x01AD)
   osIntSaveDisableHighLevelCounter++;
   /* KB end osekHWosSaveDisableHighLevelNestedBody */
   if (osIntSaveDisableHighLevelCounter == 1)
   {
      COVER(0x01AE)
      osTPStartTimerBlock(osdRESID_ALL_INTERRUPTS);
   }
   ELSEWITHCOVER(0x01AF)
#else   
   COVER(0x01B0)
   /* KB begin osekHWosSaveDisableGlobalNestedBody (overwritten) */
   if (osIntSaveDisableGlobalCounter == 0) 
   {
      COVER(0x01B1)
      os_dsync();
      if((osd_mfcr(os_ICR_OFFSET) & 0x00000100UL) != 0x00000000UL)
      {
      __disable();
      osSavedGlobalIntNested=1;
      COVER(0x01B2)
      }
      else
      {
      osSavedGlobalIntNested=0;
      COVER(0x01B3)
      }
   }
   osIntSaveDisableGlobalCounter++;
   /* KB end osekHWosSaveDisableGlobalNestedBody */
#endif   
   osIncIntAPIStatus();
   OS_PA_EXIT()
}
#endif

/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE osek.c */

