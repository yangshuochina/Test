/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK / Autosar OS 3.0
|    File Name: atosTProt.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.20
|
|  Description: timing protection functions
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2007-2010 Vector Informatik GmbH          All rights reserved.
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

/* CovComment 0:  file: atosTProt.c */

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


#if ((osdSC == SC2) || (osdSC == SC4))
#if (osdTimingProtectionUsed)

/* general variables of the timing protection */
osqRAM1 osqRAM2 osProcessType      osqRAM3 osTPActiveMeasuredProcess ;
osqRAM1 osqRAM2 osProcessType      osqRAM3 osTPActiveBlockingProcess ;
osqRAM1 osqRAM2 ResourceType       osqRAM3 osTPCurrentResource       [osdNumberOfProcesses];
osqRAM1 static osqRAM2 ResourceType       osqRAM3 osTPLastResource          [osdNumberOfAllResources+2];
osqRAM1 static osqRAM2 osTPTimeType       osqRAM3 osTPBlockTime             [osdNumberOfAllResources+2];

/* MISRA RULE 8.10 VIOLATION: (4 times) The following variables are not declared static because they are used by the ORTI debugging 
 * interface and on some compilers no debugging information is available for static variables. */
/* MISRA RULE 8.8 VIOLATION: (4 times) The following variables are not declared static because they are used by the ORTI debugging 
 * interface and on some compilers no debugging information is available for static variables.
 * No declaration is provided in a header because it is not intended to access them from other C modules */
osqRAM1 osqRAM2 osTPTimeType       osqRAM3 osTPExecutionTime         [osdNumberOfProcesses];                             /* PRQA S 1504 */ /* PRQA S 3408 */
#if osdTimingMeasurement
osqRAM1 osqRAM2 osTPTimeType       osqRAM3 osTPMinRemainingBlockTime [osdNumberOfProcesses][osdNumberOfAllResources+2];  /* PRQA S 1504 */ /* PRQA S 3408 */
osqRAM1 osqRAM2 osTPTimeStampType  osqRAM3 osTPMinInterArrivalTime   [osdNumberOfProcesses];                             /* PRQA S 1504 */ /* PRQA S 3408 */
osqRAM1 osqRAM2 osTPTimeType       osqRAM3 osTPMinRemainingExeTime   [osdNumberOfProcesses];                             /* PRQA S 1504 */ /* PRQA S 3408 */
#endif /* osdTimingMeasurement */

/* MISRA RULE 8.10 VIOLATION: The following variable is not declared static because it is used by the ORTI debugging 
 * interface and on some compilers no debugging information is available for static variables. */
/* MISRA RULE 8.8 VIOLATION: The following variable is not declared static because it is used by the ORTI debugging 
 * interface and on some compilers no debugging information is available for static variables.
 * No declaration is provided in a header because it is not intended to access it from other C modules */
osqRAM1 osqRAM2 osTPTimeStampType  osqRAM3 osTPFrameTimeStamp       [osdNumberOfProcesses];                             /* PRQA S 1504 */ /* PRQA S 3408 */

/* KB begin osekHWTPFunctionsAndDefinitions (overwritten) */

/* variables for simulation of two timers on just one HW-timer */
osqRAM1 static osqRAM2 osTPTimeType osqRAM3 osTP_CompareValue[2];
osqRAM1 static osqRAM2 osuint8      osqRAM3 osTP_ActiveTimerFlags;
osqRAM1 static osqRAM2 osuint8      osqRAM3 osTP_CurrentTimerIdx;


/*-+--------------------------------------------------------------------------
 / osTPInitVirtTimer
 /
 / Funktion:
 /   Initialize the virtual timer handling, which is used to perform the 
 /   two timing protection tasks (execution time monitoring, blocking
 /   time monitoring) with just one timer.
 /   Therefore, two timers are simulated on just one hardware-timer.
 / Context:
 /   Called from within osInitTimingProtection, where interrupts are 
 /   disabled up to system level at least.
 / Parameter:
 /   none
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/
static osqFunc1 void osqFunc2 osTPInitVirtTimer(void);
static osqFunc1 void osqFunc2 osTPInitVirtTimer(void)
{
   /* CovComment 4:  Function: osTPInitVirtTimer */
   COVER(0x07BA)
   osTP_CurrentTimerIdx  = osdTP_NoneIdx;
   osTP_ActiveTimerFlags = osdTP_NoneFlag;
      
   /* Is channel 0 of STM used for the system timer? */
#if (osdSystemTimer == osdSTM_CMP0 )
   COVER(0x07BB)
   /* Yes: use channel 1 instead for timing protection */
   /* disable interrupt request */
   osSTM_SRC1  = 0x00000000UL;
   /*  set channel 1 to compare 32 bit, starting at bit 0 */
   osSTM_CMCON = (osSTM_CMCON & 0x0000FFFFUL) | 0x001F0000UL;
#ifdef USE_TC1796_A_STEP
   /*output selection bit has different meaning on TC1796 A-Step*/
   osSTM_ICR   = (osSTM_ICR & 0xFFFFFF0FUL) | 0x00 ; /* use SRC1, disable compare interrupt */
#else
   osSTM_ICR   = (osSTM_ICR & 0xFFFFFF0FUL) | 0x40 ; /* use SRC1, disable compare interrupt */
#endif
   /* reset interrupt request in the timer */
   osSTM_ISRR  = 0x00000004UL;
   /* reset/enable interrupt request and set interrupt level to the maximum level */
   osSTM_SRC1  = 0x00005000UL | osdInterruptVectorTableSize;

   /* Is channel 1 of STM used for the system timer? */
#elif (osdSystemTimer == osdSTM_CMP1 )
   COVER(0x07BC)
   /* Yes: use channel 0 instead for timing protection */
   /* disable interrupt request */
   osSTM_SRC0  = 0x00000000UL;
   /*  set channel 0 to compare 32 bit, starting at bit 0 */
   osSTM_CMCON = (osSTM_CMCON & 0xFFFF0000UL) | 0x0000001FUL;
#ifdef USE_TC1796_A_STEP
   /*output selection bit has different meaning on TC1796 A-Step*/
   osSTM_ICR   = ( osSTM_ICR& 0xFFFFFFF0UL) |  0x4 ; /* use SRC0, disable compare interrupt*/
#else
   osSTM_ICR   = ( osSTM_ICR& 0xFFFFFFF0UL) |  0x0 ; /* use SRC0, disable compare interrupt*/
#endif
   /* reset interrupt request */
   osSTM_ISRR  = 0x00000001UL;
   /* reset/enable interrupt request and set interrupt level to the maximum level */
   osSTM_SRC0  = 0x00005000UL | osdInterruptVectorTableSize;

#else /* osdSystemTimer... */
#error "Internal Error: system timer not supported (timing protection)"
#endif /* (osdSystemTimer == ...) */
}

#if (osdSystemTimer == osdSTM_CMP0)
#define osTP_STM_CMP osSTM_CMP1
#define osTP_STM_SRC osSTM_SRC1
#define osTP_ResetPendigFlagVal 0x00000004UL
#elif (osdSystemTimer == osdSTM_CMP1)
#define osTP_STM_CMP osSTM_CMP0
#define osTP_STM_SRC osSTM_SRC0
#define osTP_ResetPendigFlagVal 0x00000001UL
#endif

/*-+--------------------------------------------------------------------------
 / osTPStartVirtTimer
 /
 / Funktion:
 /   start a virtual timer
 / Context:
 /   interrupts are globally disabled or the processor is on the highest
 /   interrupt level
 / Parameter:
 /   TimerNo: Number of the virtual timer to be started
 /   RelTime: Relative time, until the timer event has to occur
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

static inline osqFunc1 void osqFunc2 osTPStartVirtTimer(osuint8 TimerNo, osuint32 RelTime);
static inline osqFunc1 void osqFunc2 osTPStartVirtTimer(osuint8 TimerNo, osuint32 RelTime)
{
   /* CovComment 4:  Function: osTPStartVirtTimer */
   TickType   CurrentTime;
   osTickTypeTP CurrTime    ;
   osTickTypeTP NewTime   ;

   COVER(0x07BD)
   CurrentTime = osSTM_TIM0; /* take this time as reference */

   osTP_CompareValue[TimerNo] = CurrentTime + RelTime;
   osTP_ActiveTimerFlags |= 1U << TimerNo;

   if (osTP_ActiveTimerFlags==(osdTP_BlockFlag|osdTP_ExeFlag))
   {  /* The other timer is active, too. Which one is first ? */
      COVER(0x07BE)

      NewTime.unsignedNumber = RelTime;
      CurrTime.unsignedNumber = osTP_CompareValue[osTP_CurrentTimerIdx] - CurrentTime;

      if (CurrTime.signedNumber <= NewTime.signedNumber)
      { /* The time that is already set up is closer. Do nothing, everything is okay */
         COVER(0x07BF)
         return;
      }
      ELSEWITHCOVER(0x07C0)
   }

   /* Virtual timer got just started or the new time is shorter than the other time
      Let's put the new value into HW and start it 
   */
   COVER(0x07C1)
   osTP_STM_CMP = osTP_CompareValue[TimerNo];
   #if (osdSystemTimer == osdSTM_CMP0 )
      osSTM_ICR   = osSTM_ICR | 0x10 ;               /* use SRC1, enable compare interrupt */
   #else 
      osSTM_ICR   = osSTM_ICR | 0x01 ;               /* use SRC0, enable compare interrupt*/
   #endif
   /* make the new timer value the current one */
   osTP_CurrentTimerIdx = TimerNo;
   /* Is the compare value already reached? */
   if ((osTP_CompareValue[TimerNo] - osSTM_TIM0) > RelTime)
   {
      COVER(0x07C2)
      /* Yes: simply trigger the interrupt */
      osTP_STM_SRC |= 0x8000UL;
   }
}


/*-+--------------------------------------------------------------------------
 / osTPStopVirtTimer
 /
 / Funktion:
 /   stop a virtual timer
 / Context:
 /   interrupts are globally disabled or the processor is on the highest
 /   interrupt level
 / Parameter:
 /   TimerNo: Number of the virtual timer to be stopped
 / Returnvalue:
 /   remainig time from now, to the time, the stopped timer event was
 /   once planned for.
 /--------------------------------------------------------------------------*/
static inline osqFunc1 osTPTimeDiffType osqFunc2 osTPStopVirtTimer(osuint8 TimerNo);
static inline osqFunc1 osTPTimeDiffType osqFunc2 osTPStopVirtTimer(osuint8 TimerNo)
{
   /* CovComment 4:  Function: osTPStopVirtTimer */
   osTPTimeDiffType  timediff;

   COVER(0x07C3)
   /* Set this virtual timer invalid */
   osTP_ActiveTimerFlags &= (osuint8)~((osuint8)(1 << TimerNo));
   
   if (osTP_ActiveTimerFlags == osdTP_NoneFlag)
   {
      /* No more timing measurement, stop the HW compare */
      COVER(0x07C4)
      osTP_CurrentTimerIdx=osdTP_NoneIdx;
      #if (osdSystemTimer == osdSTM_CMP0 )
         osSTM_ICR   = osSTM_ICR & 0xFFFFFFEFUL; /* use SRC1, disable compare interrupt */
      #else 
         osSTM_ICR   = osSTM_ICR & 0xFFFFFFFEUL; /* use SRC0, disable  compare interrupt*/
      #endif
      osSTM_ISRR = osTP_ResetPendigFlagVal;
      osTP_STM_SRC |= 0x4000UL;
   } 
   else
   {
      /* The other timer is still active */
      COVER(0x07C5)
      if (osTP_ActiveTimerFlags == osdTP_ExeFlag)
      {
         osTP_CurrentTimerIdx=osdTP_ExeIdx;
         COVER(0x07C6)
      }
      else
      {
         osTP_CurrentTimerIdx=osdTP_BlockIdx;
         COVER(0x07C7)
      }

      /* put the new time into the hardware (HW is still running) */
      osTP_STM_CMP = osTP_CompareValue[osTP_CurrentTimerIdx];
   
      /* delete a possibly pendig interrupt (might have occured due to the stopped virtual timer)*/
      osSTM_ISRR = osTP_ResetPendigFlagVal;
      osTP_STM_SRC |= 0x4000UL;

      /* Is the compare value already reached? */
      timediff=osTP_CompareValue[osTP_CurrentTimerIdx] - osSTM_TIM0;
      if (timediff > 0x7FFFFFFFUL)
      {
         COVER(0x07C8)
         /* Yes: simply trigger the interrupt */
         osTP_STM_SRC |= 0x8000UL;
      }
   }
   
   /* Calculate and return remaining time of the timer */
   timediff=osTP_CompareValue[TimerNo] - osSTM_TIM0;
   if (timediff > 0x7FFFFFFFUL)
   {
      COVER(0x07C9)
      timediff=0;
   }
   return timediff;
   
}
/* KB end osekHWTPFunctionsAndDefinitions */


/*-+--------------------------------------------------------------------------
 / osInitTimingProtection
 /
 / Funktion:
 /   Initializes the variables, used for execution time monitoring, blocking
 /   time monitoring, arrival rate monitoring and frame time generation.
 / Context:
 /   Interrupts have to be disabled up to the system level at least.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   none
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osInitTimingProtection(void)
{
   /* CovComment 4:  Function: osInitTimingProtection */
   osuint32 n;
#if osdTimingMeasurement
   osuint32 k;
#endif
   osTPTimeStampType CurrentTime;
   /* KB begin osTPInitTimingProtectionEntry (default) */
   /* KB end osTPInitTimingProtectionEntry */
   /* KB begin osTPGetArrivalTimeStamp (overwritten) */
   CurrentTime  = ((osTPTimeStampType)(osSTM_TIM0));
   /* MISRA RULE 12.8 not violated: Misra tool does not recognize 64 bit number
   */
   CurrentTime |= ((osTPTimeStampType)(osSTM_CAP))<<32;    /* PRQA S 0501 */
   /* KB end osTPGetArrivalTimeStamp */
   COVER(0x07CA)
   /* setup different values for all processes*/
   for (n = 0; n < osdNumberOfProcesses; n++)
   {
      COVER(0x07CB)
      /* set the time of the last arrival so that new arrivals become possible right now */
      osTPFrameTimeStamp[n] = CurrentTime - oskTPFrameTime[n];
      osTPFrameTimeStamp[n] &= osdMaxAllowedTimeStamp;

      /* set up the remaining execution time */
      osTPExecutionTime[n] = oskTPExecutionTime[n];

#if osdTimingMeasurement
      /* set minimum remaining execution time to the maximum */
      osTPMinRemainingExeTime[n] = oskTPExecutionTime[n];
#endif /* osdTimingMeasurement */

      /* set currently measured resource */
      osTPCurrentResource[n] = osdNoResource;

#if osdTimingMeasurement
      osTPMinInterArrivalTime[n] = osdMaxAllowedTimeStamp;
      
      /* setup values for all resources and interrupt locks */
      for (k = 0; k < (osdNumberOfAllResources + 2) ; k++)
      {
         COVER(0x07CC)
         /* set minimum remainig blocking time to the maximum */
         osTPMinRemainingBlockTime[n][k] = oskTPBlockTime[n][k];
      }
#endif /* osdTimingMeasurement */
   }

   COVER(0x07CD)
   osTPActiveMeasuredProcess = INVALID_PROCESS;
   osTPActiveBlockingProcess = INVALID_PROCESS;

   /* KB begin osInitTimingProtectionStartHwTimer (overwritten) */
      osTPInitVirtTimer();
   /* KB end osInitTimingProtectionStartHwTimer */

} /* END OF osInitTimingProtection */


/*-+--------------------------------------------------------------------------
 / osTPExeSwitchBack
 /
 / Funktion:
 /   Stops the currently running execution time monitoring/measurement and 
 /   resets the remaining time for the monitored/measured process. So when 
 /   the monitoring/measurement for this process is started later on, the 
 /   full time is available again.
 /   When the parameter OldProcess is not osdInvalidProcess, and timing 
 /   protection is configured for the process, osTPExeSwitchBack starts 
 /   the execution time monitoring/measurement on the remaining time that 
 /   was stored before for that process.
 / Context:
 /   Must be called with interrupts disabled up to system level at least.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   OldProcess             The process ID of the task/ISR, the execution 
 /                          time shall be resumed for. This parameter might
 /                          be identical to the ID of the currently 
 /                          monitored/measured process.
 / Returnvalue:
 /   None
 /--------------------------------------------------------------------------*/


osqFunc1 void osqFunc2 osTPExeSwitchBack( osProcessType OldProcess )
{
   /* CovComment 4:  Function: osTPExeSwitchBack */

   osTPTimeType RemainTime;
   osTPTimeType StartTime;

   /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* global disabling also disables timing protection on this processor */
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

#if ( !(osdKillTaskIsrPossible || (osdSC == SC3) || (osdSC == SC4)))
   osSysErrAssert(osTPActiveMeasuredProcess != INVALID_PROCESS, osdErrETNoCurrentProcess)
#else
   if (osTPActiveMeasuredProcess != INVALID_PROCESS)
   {
#endif
      /* Is execution time monitoring/measurement configured for this task/ISR? */
      if (oskTPExecutionTimeCheck[osTPActiveMeasuredProcess] != osdTPNoMeasurement)
      {
         /* YES: Stop the current monitoring/measurement and store remaining time*/
         /* KB begin osTPStopTimerExeHw (overwritten) */
         RemainTime = osTPStopVirtTimer(osdTP_ExeIdx);
         /* KB end osTPStopTimerExeHw */

#if osdTimingMeasurement
         /* Is the remaining time minimal? */
         if (RemainTime < osTPMinRemainingExeTime[osTPActiveMeasuredProcess])
         {
            COVER(0x07CE)
            /* Yes: Store in in osTPMinRemainTime[] */
            osTPMinRemainingExeTime[osTPActiveMeasuredProcess] = RemainTime;
         }
         ELSEWITHCOVER(0x07CF)
#endif /* osdTimingMeasurement */
         
         /* reload the remaining execution time for this process */
         osTPExecutionTime[osTPActiveMeasuredProcess] = oskTPExecutionTime[osTPActiveMeasuredProcess];

      }
      ELSEWITHCOVER(0x07D0)

#if (osdKillTaskIsrPossible || (osdSC == SC3) || (osdSC == SC4))
   }/* if (osTPActiveMeasuredProcess != INVALID_PROCESS) */
   ELSEWITHCOVER(0x07D1)
#endif

   /* set the currently measured process */
   osTPActiveMeasuredProcess = OldProcess;

   /* Is this a valid process? (The next process to be measured/monitored might not yet be known) */
   if (osTPActiveMeasuredProcess != INVALID_PROCESS)
   {
      /* YES: Do we need any timing measurement/protection for this process?*/
      if (oskTPExecutionTimeCheck[osTPActiveMeasuredProcess] != osdTPNoMeasurement)
      {
         COVER(0x07D2)
         /* Yes: load remaining execution time */
         StartTime = osTPExecutionTime[osTPActiveMeasuredProcess];

         /* Start the timer*/
         /* KB begin osTPStartTimerExeHw (overwritten) */
         osTPStartVirtTimer(osdTP_ExeIdx, StartTime);
         /* KB end osTPStartTimerExeHw */
      }
      ELSEWITHCOVER(0x07D3)
   }
   ELSEWITHCOVER(0x07D4)
   
   /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */

} /* END OF osTPExeSwitchBack */

   

/*-+--------------------------------------------------------------------------
 / osTPExeSwitchTo
 /
 / Funktion:
 /    Stops a currently running execution time monitoring/measurement and 
 /    stores the remaining time for the monitored/measured process for the 
 /    time when it becomes active again.
 /    When the parameter NewProcess is configured for timing protection, 
 /    osTPExeSwitchTo starts the execution time monitoring/measurement on 
 /    the remaining time that was stored before.
 / Context:
 /    Must be called with interrupts disabled up to the system level at least.
 /    This function is meant for OS internal usage.
 / Parameter:
 /    NewProcess:
 /       The process ID of the task/ISR, the execution time shall be 
 /       monitored/measured for
 / Returnvalue:
 /    The process ID of the task/ISR, the execution time has been 
 /    monitored/measured for until the call of this function. If no 
 /    execution time has been moni-tored/measured before the call, 
 /    osdInvalidProcess is returned instead.
 /--------------------------------------------------------------------------*/

osqFunc1 osProcessType osqFunc2 osTPExeSwitchTo(osProcessType NewProcess)
{
   /* CovComment 4:  Function: osTPExeSwitchTo */
   osTPTimeType StartTime;
   osTPTimeType RemainTime;
   osProcessType retVal;

   /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* global disabling also disables timing protection on this processor */
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

   /* Is the currently measured process valid? (IdleLoop and dispatcher use invalid procedure ID) */
   if (osTPActiveMeasuredProcess != INVALID_PROCESS)
   {
      /* YES: Is execution time monitoring/measurement configured for this task/ISR? */
      if (oskTPExecutionTimeCheck[osTPActiveMeasuredProcess] != osdTPNoMeasurement)
      {
         /* YES: Stop the current monitoring/measurement and store remaining time*/
         /* KB begin osTPStopTimerExeHw (overwritten) */
         RemainTime = osTPStopVirtTimer(osdTP_ExeIdx);
         /* KB end osTPStopTimerExeHw */

         /* store the remainig time for when the process is resumed to */
         osTPExecutionTime[osTPActiveMeasuredProcess] = RemainTime;
         COVER(0x07D5)
      }
      ELSEWITHCOVER(0x07D6)
   }
   ELSEWITHCOVER(0x07D7)

   /* set the currently measured process */
   retVal = osTPActiveMeasuredProcess;
   osTPActiveMeasuredProcess = NewProcess;

   /* Is the new process valid? (IdleLoop and dispatcher use invalid procedure ID) */
   if (osTPActiveMeasuredProcess != INVALID_PROCESS)
   {
      /* YES: Do we need any timing measurement/protection for this process?*/
      if (oskTPExecutionTimeCheck[osTPActiveMeasuredProcess] != osdTPNoMeasurement)
      {
         COVER(0x07D8)
         /* Yes: load timer value from context and start the timer */
         StartTime = osTPExecutionTime[osTPActiveMeasuredProcess];

         /* KB begin osTPStartTimerExeHw (overwritten) */
         osTPStartVirtTimer(osdTP_ExeIdx, StartTime);
         /* KB end osTPStartTimerExeHw */
      }
      ELSEWITHCOVER(0x07D9)
   }
   ELSEWITHCOVER(0x07DA)
   /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */

   return retVal;
} /* END OF osTPExeSwitchTo */

/*-+--------------------------------------------------------------------------
 / osTPArrivalAllowed
 /
 / Funktion:
 /    If no timing protection is configured for the process 'ArrivedProcess',
 /    this service returns TRUE always. Otherwise it checks whether the 
 /    arrival of the process is allowed (time between last arrival and now 
 /    is big enough). If so, the global arrival information is updated so 
 /    that further arrivals within the frame time fail. If the current 
 /    arrival is not allowed, the ProtectionHook is called and the 
 /    respective reac-tion (if any) is performed. So this function might not 
 /    return (protection hook reac-tion is to kill the faulty task/ISR, the 
 /    faulty application or to shutdown the OS). In case the protection hook
 /    reaction is to do nothing, this function returns and the caller has to
 /    handle the situation.   
 / Context:
 /    Must be called with interrupts disabled up to the system level at least.
 /    This function is meant for OS internal usage.
 / Parameter:
 /    ArrivedProcess:
 /       The process ID of the task/ISR, that has to be activated/released
 / Returnvalue:
 /    osdTRUE:  Arrival is allowed
 /    osdFALSE: Arrival is disallowed
 /--------------------------------------------------------------------------*/

osqFunc1 osbool osqFunc2 osTPArrivalAllowed(osProcessType ArrivedProcess)
{
   /* CovComment 4:  Function: osTPArrivalAllowed */
   osTPTimeStampType CurrentTime;
   osTPTimeStampType CurrentInterArrivalTime;
   ProtectionReturnType proRet;
    

   /* Is frame time monitoring/measurement configured for this task/ISR? */
   /* (frame time monitoring/measurement are always configured together) */
   if (oskTPExecutionTimeCheck[ArrivedProcess] == osdTPNoMeasurement)
   {
      COVER(0x07DB)
      /* NO: No timing protection, arrival always allowed */
      return osdTRUE;
   }
   ELSEWITHCOVER(0x07DC)

   /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* global disabling also disables timing protection on this processor */
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

   /* KB begin osTPGetArrivalTimeStamp (overwritten) */
   CurrentTime  = ((osTPTimeStampType)(osSTM_TIM0));
   /* MISRA RULE 12.8 not violated: Misra tool does not recognize 64 bit number
   */
   CurrentTime |= ((osTPTimeStampType)(osSTM_CAP))<<32;    /* PRQA S 0501 */
   /* KB end osTPGetArrivalTimeStamp */

   
   CurrentInterArrivalTime = CurrentTime - osTPFrameTimeStamp[ArrivedProcess];
   CurrentInterArrivalTime &= osdMaxAllowedTimeStamp;

#if osdTimingMeasurement
   if(CurrentInterArrivalTime < osTPMinInterArrivalTime[ArrivedProcess])
   {
      COVER(0x07DD)
      osTPMinInterArrivalTime[ArrivedProcess] = CurrentInterArrivalTime;
   }
   ELSEWITHCOVER(0x07DE)
#endif /* osdTimingMeasurement */

   if ((CurrentInterArrivalTime > oskTPFrameTime[ArrivedProcess]) || (oskTPExecutionTimeCheck[ArrivedProcess] == osdTPOnlyMeasure))
   {
      COVER(0x07DF)
      osTPFrameTimeStamp[ArrivedProcess] = CurrentTime;
      /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
      return osdTRUE;
   }
   ELSEWITHCOVER(0x07E0)
   
   #if osdTimingProtectionUsed
   proRet = osProtectionHookWrapper(E_OS_PROTECTION_ARRIVAL);
   osHandleProtectionErrors(proRet, E_OS_PROTECTION_ARRIVAL);
   #endif
   /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
   COVER(0x07E1)
   return osdFALSE;

} /* END OF osTPArrivalAllowed */


/*-+--------------------------------------------------------------------------
 / osTPStartTimerBlock
 /
 / Funktion:
 /    If timing protection for this resource has been configured, this
 /    function starts the blocking time monitoring/measurement for this 
 /    resource. In case a blocking time measurement has already been active
 /    at the time this function is called, the function checks which of both
 /    has the smaller remaining time. In case, the new one has the smaller
 /    time, the old one is put onto the blocking time protection stack and 
 /    the new one is written into the timer. If the old one has the smaller
 /    time, the new one needs not to be monitored but measurement is 
 /    necessary. Therefore, no change of the Blocking Time protection stack
 /    is performed but a time stamp is stored for the new resource.
 / Context:
 /    Must be called with interrupts disabled up to the system level at least.
 /    This function is meant for OS internal usage.
 / Parameter:
 /   res: The (extended) resource ID, the blocking time monitoring/measurement
 /        shall be started for. Extended resource ID means, that IDs are added
 /        for disabling all interrupts and disabling OS-interrupts.
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osTPStartTimerBlock(ResourceType res)
{
   /* CovComment 4:  Function: osTPStartTimerBlock */

   osTPTimeType RemainTime;
   osTPTimeType NewTime;
   osProcessType Proc;
   /* KB begin osTPStartTimerBlockHwLocalVar (overwritten) */
   /* KB end osTPStartTimerBlockHwLocalVar */

   if (osIntNestingDepth > 0)
   {
      COVER(0x07E2)
      Proc = osdNumberOfAllTasks + osActiveISRID;
   }
   else
   {
      if (osActiveTaskIndex == osdNoActiveTask)
      {
         /* No timing protection in StartupHook and ShutdownHook */
         COVER(0x07E3)
         osTPActiveBlockingProcess = INVALID_PROCESS;/* end previous protection */
         return;
      }
      ELSEWITHCOVER(0x07E4)
      Proc = osActiveTaskIndex;
   }
   osSysErrAssert((Proc<osdNumberOfProcesses), osdErrBMInvalidProcessInStart)
   
   if (oskTPBlockTimeCheck[Proc][res] != osdTPNoMeasurement)
   {
      COVER(0x07E5)
      /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* global disabling also disables timing protection on this processor */
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

      osTPActiveBlockingProcess = Proc;

      osSysErrAssert(osTPCurrentResource[osTPActiveBlockingProcess] != res, osdErrBMResAlreadyMeasured)

      /* Is there a blocking time measurement currently running for this process? */
      if (osTPCurrentResource[osTPActiveBlockingProcess] != osdNoResource) 
      {
         /* Yes: Stop it, compute remaining time, compare with the new time */
         /* KB begin osTPStartTimerBlockHwStop (overwritten) */
         RemainTime = osTPStopVirtTimer(osdTP_BlockIdx);
         /* KB end osTPStartTimerBlockHwStop */

         /* Store remainig time for the resource */
         osTPBlockTime[osTPCurrentResource[osTPActiveBlockingProcess]] = RemainTime;

         /* Is the new time smaller than the current one? */
         if (oskTPBlockTime[osTPActiveBlockingProcess][res] < RemainTime)
         {
            COVER(0x07E6)
            /* Yes: new time is shorter, start the timer with the new time */  
            NewTime = oskTPBlockTime[osTPActiveBlockingProcess][res];  
            /* KB begin osTPStartTimerBlockHwStartNew (overwritten) */
            osTPStartVirtTimer(osdTP_BlockIdx, NewTime);
            /* KB end osTPStartTimerBlockHwStartNew */

            /* Update links and blocking time */
            osTPLastResource[res] = osTPCurrentResource[osTPActiveBlockingProcess];
            osTPCurrentResource[osTPActiveBlockingProcess] = res;
            osTPBlockTime[res] = oskTPBlockTime[osTPActiveBlockingProcess][res];
         }
         else
         {
            COVER(0x07E7)
            /* No: old time is shorter, re-start the timer with this one */
            /* KB begin osTPStartTimerBlockHwStartOld (overwritten) */
            osTPStartVirtTimer(osdTP_BlockIdx, RemainTime);
            /* KB end osTPStartTimerBlockHwStartOld */
            
            /* RemainingTime(res,stop) = MaxBlockingTime(res)
             *                           - RemainingTime(osTPCurrentResource,now) 
             *                           + RemainingTime(osTPCurrentResource,stop)*/
            osTPBlockTime[res] = oskTPBlockTime[osTPActiveBlockingProcess][res] - RemainTime;
         }
      }
      else
      {
         COVER(0x07E8)
         /* No: start the first blocking time measurement (for this process) */
         NewTime = oskTPBlockTime[osTPActiveBlockingProcess][res];  
         /* KB begin osTPStartTimerBlockHwStartNew (overwritten) */
         osTPStartVirtTimer(osdTP_BlockIdx, NewTime);
         /* KB end osTPStartTimerBlockHwStartNew */

         /* Update links and blocking time */
         osTPLastResource[res] = osTPCurrentResource[osTPActiveBlockingProcess];
         osTPCurrentResource[osTPActiveBlockingProcess] = res;
         osTPBlockTime[res] = oskTPBlockTime[osTPActiveBlockingProcess][res];
      }

      /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
   }
   ELSEWITHCOVER(0x07E9)
} /* END OF osTPStartTimerBlock */


/*-+--------------------------------------------------------------------------
 / osTPStopTimerBlock
 /
 / Funktion:
 /    This function immediately returns when no blocking time monitoring/mea-
 /    surement is currently active or when no timing protection is configured
 /    for the resource "res". If the function does not immediately return, it
 /    checks if the resource "res" is currently programmed into the timer. If
 /    yes, it stops the current monitoring/measurement and corrects the 
 /    blocking time of the next monitoring/measurement in the stack (if any).
 /    If the blocking time measurement/monitoring for "res" is not programmed
 /    in to the timer, the resource "res" is not monitored because a 
 /    monitoring with shorter time has already been active when 
 /    osTPStartTimerBlock was called for "res". Anyhow, a measurement needs
 /    to be performed, so the blocking time for "res" is computed from the 
 /    time stamp that was stored by osTPStartTimerBlock and the current 
 /    remaining time for the running blocking time monitoring/measurements.
 / Context:
 /   Must be called with interrupts disabled up to the system level at least.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   res: The (extended) resource ID, the blocking time monitoring/measurement
 /        shall be stopped for. Extended resource ID means, that IDs are added
 /        for disabling all interrupts and disabling OS-interrupts.
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

 /* MISRA RULE 14.1 VIOLATION: In some very rare configurations, this function might not be called.
    Catching all cases is very complex and thus error prone */
osqFunc1 void osqFunc2 osTPStopTimerBlock(ResourceType res)  /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osTPStopTimerBlock */

   osTPTimeType RemainTime;
   osTPTimeType BlockedTime;
   /* KB begin osTPStopTimerBlockLocalVar (overwritten) */
   /* KB end osTPStopTimerBlockLocalVar */

   /* Is there a blocking time measurement currently running? */
   if (osTPActiveBlockingProcess == INVALID_PROCESS)
   {
      COVER(0x07EA)
      /* No: Nothing to stop, return */
      return; 
   }
   ELSEWITHCOVER(0x07EB)
   osSysErrAssert((osTPActiveBlockingProcess < osdNumberOfProcesses), osdErrBMInvalidProcessInStop)
   
   /* Is the blocking time for res measured or monitored? */
   if (oskTPBlockTimeCheck[osTPActiveBlockingProcess][res] == osdTPNoMeasurement)
   {
      COVER(0x07EC)
      /* NEITHER: Nothing to stop, return */
      return; 
   }
   ELSEWITHCOVER(0x07ED)

   /* Is the blocking time for res directly measured (master)? */
   if (osTPCurrentResource[osTPActiveBlockingProcess] == res)  
   {
      /* Yes: stop the current measurement */

      /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* global disabling also disables timing protection on this processor */
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

      /* KB begin osTPStopTimerBlockHwStop (overwritten) */
      RemainTime = osTPStopVirtTimer(osdTP_BlockIdx);
      /* KB end osTPStopTimerBlockHwStop */ 

#if osdTimingMeasurement
      /* Is this the smallest remainig time?*/
      if (RemainTime < osTPMinRemainingBlockTime[osTPActiveBlockingProcess][res])
      {
         COVER(0x07EE)
         /* Yes: store it for the blocking time measurement*/
         osTPMinRemainingBlockTime[osTPActiveBlockingProcess][res] = RemainTime;
      }
      ELSEWITHCOVER(0x07EF)
#endif /* osdTimingMeasurement */

      /* Make the last measurement the current one */
      osTPCurrentResource[osTPActiveBlockingProcess] = osTPLastResource[osTPCurrentResource[osTPActiveBlockingProcess]]; 

      /* Do we have to resume a measurement? */
      if (osTPCurrentResource[osTPActiveBlockingProcess] != osdNoResource)
      {
         COVER(0x07F0)
         /* Yes: resume */

         /* Compute the blocking time for the stopped measurement */
         BlockedTime = oskTPBlockTime[osTPActiveBlockingProcess][res] - RemainTime;

         /* reduce the remaining time for the interrupted measurement */
         osTPBlockTime[osTPCurrentResource[osTPActiveBlockingProcess]] -= BlockedTime;

         /* get remaining time for the preempted measurement */
         RemainTime = osTPBlockTime[osTPCurrentResource[osTPActiveBlockingProcess]];

         /* KB begin osTPStopTimerBlockHwStart (overwritten) */
         osTPStartVirtTimer(osdTP_BlockIdx, RemainTime);
         /* KB end osTPStopTimerBlockHwStart */

      }
      else
      {
         COVER(0x07F1)
         /* No measurement is currently active */
         osTPActiveBlockingProcess = INVALID_PROCESS;
      }
      /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
   }
   else
   {
      COVER(0x07F2)
      /* No: res is measured as a slave of the current resource */
      
      /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* global disabling also disables timing protection on this processor */
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

      /* Stop the current blocking time measurement, just to get the remaining time */
      /* KB begin osTPStopTimerBlockHwStop (overwritten) */
      RemainTime = osTPStopVirtTimer(osdTP_BlockIdx);
      /* KB end osTPStopTimerBlockHwStop */ 

      /* RemainingTime(res,now) = MaxBlockingTime(res)
       *                          - RemainingTime(osTPCurrentResource,start)
       *                          + RemainingTime(osTPCurrentResource,now)  */
      RemainTime = osTPBlockTime[res] + RemainTime;      

#if osdTimingMeasurement
      /* Is this the smallest remainig time?*/
      if (RemainTime < osTPMinRemainingBlockTime[osTPActiveBlockingProcess][res])
      {
         COVER(0x07F3)
         /* Yes: store it for the blocking time measurement*/
         osTPMinRemainingBlockTime[osTPActiveBlockingProcess][res] = RemainTime;
      }
      ELSEWITHCOVER(0x07F4)
#endif /* osdTimingMeasurement */
      
      /* restart the current blocking time measurement*/
      /* KB begin osTPStopTimerBlockHwStart (overwritten) */
      osTPStartVirtTimer(osdTP_BlockIdx, RemainTime);
      /* KB end osTPStopTimerBlockHwStart */

      /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
      
   }

} /* END OF osTPStopTimerBlock */

/*-+--------------------------------------------------------------------------
 / osTPSuspendTimerBlock
 /
 / Funktion:
 /    Suspends all currently running blocking time monitorings/measurements.
 /    So the task/ISR still holds the respective resources but the 
 /    monitoring/measurement shall not be active because the task/ISR is 
 /    preempted/interrupted.
 / Context:
 /    The function disables up to the highest level (global) internally. 
 /    Interrupts might be enabled when this function is called.
 /    This function is meant for OS internal usage.
 / Parameter:
 /    none
 / Returnvalue:
 /    none
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osTPSuspendTimerBlock(void)
{
   /* CovComment 4:  Function: osTPSuspendTimerBlock */

   osTPTimeType RemainTime;
   /* KB begin osTPSuspendTimerBlockLocalVar (overwritten) */
   /* KB end osTPSuspendTimerBlockLocalVar */

   /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* global disabling also disables timing protection on this processor */
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

   /* Is there a blocking time measurement to be suspended? */
   if (osTPActiveBlockingProcess == INVALID_PROCESS)
   {
      COVER(0x07F5)
      /* No: nothing to suspend */
      /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
      return;
   }
   ELSEWITHCOVER(0x07F6)
   /* KB begin osTPSuspendTimerBlockStop (overwritten) */
   RemainTime = osTPStopVirtTimer(osdTP_BlockIdx);
   /* KB end osTPSuspendTimerBlockStop */

   osTPBlockTime[osTPCurrentResource[osTPActiveBlockingProcess]] = RemainTime;

   /* All blocking time measurements are suspended, none is active */
   osTPActiveBlockingProcess = INVALID_PROCESS;

   /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */

} /* END OF osTPSuspendTimerBlock */

/*-+--------------------------------------------------------------------------
 / osTPResumeTimerBlock
 /
 / Funktion:
 /   Resumes all blocking time monitorings/measurements of the process 'Proc'.
 / Context:
 /   Interrupts are globally disabled.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   proc: The identifier of the process, the blocking time measurement 
 /         shall be resumed for
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osTPResumeTimerBlock(osProcessType Proc)
{
   /* CovComment 4:  Function: osTPResumeTimerBlock */

   osTPTimeType RemainTime;

   /* Is there a valid process, to resume to? */
   if (Proc == INVALID_PROCESS)
   {
      COVER(0x07F7)
      /* No: nothing to resume, return */
      return;
   }
   ELSEWITHCOVER(0x07F8)

   /* KB begin osekHWosSaveDisableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* global disabling also disables timing protection on this processor */
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   /* KB end osekHWosSaveDisableGlobalNestedAndTimProtInterrupt */

   /* Is there a suspended blocking time measurement for the current process? */
   if (osTPCurrentResource[Proc] == osdNoResource)
   {
      COVER(0x07F9)
      /* NO: nothing to do */
      /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */
      return;
   }
   ELSEWITHCOVER(0x07FA)

   osTPActiveBlockingProcess = Proc;

   RemainTime = osTPBlockTime[osTPCurrentResource[Proc]];
   
   /* restart interrupted measurement */
   /* KB begin osTPResumeTimerBlockHwStart (overwritten) */
   osTPStartVirtTimer(osdTP_BlockIdx, RemainTime);
   /* KB end osTPResumeTimerBlockHwStart */

   /* KB begin osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt (overwritten) */
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   /* KB end osekHWosRestoreEnableGlobalNestedAndTimProtInterrupt */

} /* END OF osTPResumeTimerBlock */

#if (osdKillTaskIsrPossible && osdTimingProtectionUsed) || (osdSC == SC3) || (osdSC == SC4)
/*-+--------------------------------------------------------------------------
 / osTPCleanListBlock
 /
 / Funktion:
 /   Empties the list of resources that are measured/monitored for the 
 /   process 'Proc'.
 / Context:
 /   Interrupts may be enabled or disabled, no restriction. Blocking time 
 /   measurement has already been suspended for the specified process by 
 /   means of the function osTPSuspendTimerBlock.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   proc: The Identifier of the process, the blocking blocking list shall
 /         be cleand for.
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osTPCleanListBlock(osProcessType proc)
{
   /* CovComment 4:  Function: osTPCleanListBlock */

   COVER(0x07FB)
   /* simply destroy the link to the list */
   osTPCurrentResource[proc] = osdNoResource;

} /* END OF osTPCleanListBlock */
#endif

#if ( (osdKillTaskIsrPossible || (osdSC == SC4)) && (osdTimingProtectionUsed != 0) ) || ( (osdTimingProtectionUsed != 0) && (osdProtectionHook != 0) && (osdProtReactKillTaskIsrAllowed != 0) )
/*-+--------------------------------------------------------------------------
 / osTPReloadExeBudget
 /
 / Funktion:
 /   Reloads the execution time budget of the process given as a parameter
 / Context:
 /   Interrupts may be enabled or disabled, no restriction. Execution time 
 /   measurement has already been suspended for the currently running process by 
 /   means of the function osTPExeSwitchBack.
 /   This function is meant for OS internal usage.
 / Parameter:
 /   proc: The Identifier of the process, the execution time budget shall be
 /         reloaded for.
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/


/* MISRA RULE 14.1 VIOLATION: This function might be unused in some configurations.
 * To reduce code complexity this function is not removed for complex configurations. (1503)
 */
osqFunc1 void osqFunc2 osTPReloadExeBudget(osProcessType proc) /* PRQA S 1503 */
{
   /* CovComment 4:  Function: osTPReloadExeBudget */

#if osdTimingMeasurement
   if (osTPExecutionTime[proc] < osTPMinRemainingExeTime[proc])
   {
      COVER(0x07FC)
      osTPMinRemainingExeTime[proc] = osTPExecutionTime[proc];
   }
   ELSEWITHCOVER(0x07FD)
#endif /* osdTimingMeasurement */
         
   /* reload the remaining execution time for this process */
   osTPExecutionTime[proc] = oskTPExecutionTime[proc];
}
#endif

#if osdTimingMeasurement
/*-+--------------------------------------------------------------------------
 / GetTaskMaxExecutionTime
 /
 / Funktion:
 /   Computes the maximum execution time of a task per activation/release.
 /   All activations/releses that have been finished by a call of 
 /   TerminateTask, ChainTask or WaitEvent are considered.
 / Context:
 /   No restrictions.
 /   This is an additional OS-API function, not specified by OSEK-OS or
 /   AutosarOS.
 / Parameter:
 /   TaskID:   The Identifier of the task, the maximum execution time shall
 /             be computed for.
 /   *MaxTime: The variable, the computed maximum execution time is returned
 /             to.
 / Returnvalue:
 /   E_OK:    A valid maximum execution time could be computed.
 /   E_OS_ID: TaskID is not a valid task identifier.
 /--------------------------------------------------------------------------*/

#ifndef osdGetTaskMaxExecutionTimeNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskMaxExecutionTime(TaskType TaskID,  osTPTimeRefType MaxTime)
{
   /* CovComment 4:  Function: osGetTaskMaxExecutionTime */
   COVER(0x07FE)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x07FF)
#if osdNumberOfAllTasks > 0
   COVER(0x0800)
   if (TaskID >= osdNumberOfAllTasks)
#else
   osdDummyRead(TaskID)
   osdDummyRead(MaxTime)
#endif
   {
      COVER(0x0801)
      #if osdErrorHook 
         COVER(0x0802)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskMaxExecutionTime_TaskID(TaskID)
         osSaveGetTaskMaxExecutionTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ID, osdErrTEWrongTaskID);
      #if osdErrorHook 
         COVER(0x0803)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ID;
   }
#if osdNumberOfAllTasks > 0 /* prevent unreachable code */
   ELSEWITHCOVER(0x0804)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplTaskAccess[TaskID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0805)
      #if osdErrorHook 
         COVER(0x0806)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskMaxExecutionTime_TaskID(TaskID)
         osSaveGetTaskMaxExecutionTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrTENoAccess);
      #if osdErrorHook 
         COVER(0x0807)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0808)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      COVER(0x0809)
      if (osAddressParamValid((osuint32)MaxTime, (osuint32)sizeof(osuint32)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
         COVER(0x080A)
          #if osdErrorHook 
             COVER(0x080B)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveGetTaskMaxExecutionTime_TaskID(TaskID)
             osSaveGetTaskMaxExecutionTime_MaxTime(MaxTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrTEIllegalAddr);
          #if osdErrorHook 
             COVER(0x080C)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x080D)
   }
   ELSEWITHCOVER(0x080E)
#endif /* osdSC== ... */
#endif /* osdNumberOfAllTasks ... */
#endif /* STATUS_LEVEL ... */
   
/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfAllTasks > 0))
   COVER(0x080F)
   *MaxTime = oskTPExecutionTime[TaskID] - osTPMinRemainingExeTime[TaskID];
   
   return E_OK;
#endif

}/* END of function: GetTaskMaxExecutionTime */

#endif /* osdGetTaskMaxExecutionTimeNotUsed */

/*-+--------------------------------------------------------------------------
 / GetISRMaxExecutionTime
 /
 / Funktion:
 /   Computes the maximum execution time of an ISR per arrival. All arrivals
 /   are considered that have been handled by the ISR and the ISR has 
 /   already returned for.
 / Context:
 /   no restrictions
 /   This is an additional OS-API function, not specified by OSEK-OS or
 /   AutosarOS.
 / Parameter:
 /   ISRID:    The Identifier of the ISR, the maximum execution time shall
 /             be computed for
 /   *MaxTime: The variable, the computed maximum execution time is returned
 /             to.
 / Returnvalue:
 /   E_OK:    A valid maximum execution time could be computed.
 /   E_OS_ID: ISRID is not a valid ISR identifier.
 /--------------------------------------------------------------------------*/

#ifndef osdGetISRMaxExecutionTimeNotUsed
osqFunc1 StatusType osqFunc2 osGetISRMaxExecutionTime(ISRType ISRID, osTPTimeRefType MaxTime)
{
   /* CovComment 4:  Function: osGetISRMaxExecutionTime */
   COVER(0x0810)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x0811)
#if osdNumberOfCat2ISRs > 0
   COVER(0x0812)
   if (ISRID >= osdNumberOfCat2ISRs)
#else
   osdDummyRead(ISRID)
   osdDummyRead(MaxTime)
#endif
   {
      COVER(0x0813)
      #if osdErrorHook 
         COVER(0x0814)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetISRMaxExecutionTime_ISRID(ISRID)
         osSaveGetISRMaxExecutionTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ID, osdErrIEWrongISRID);
      #if osdErrorHook 
         COVER(0x0815)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ID;
   }
#if osdNumberOfCat2ISRs > 0
   ELSEWITHCOVER(0x0816)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplISRAccess[ISRID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0817)
      #if osdErrorHook 
         COVER(0x0818)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetISRMaxExecutionTime_ISRID(ISRID)
         osSaveGetISRMaxExecutionTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrIENoAccess);
      #if osdErrorHook 
         COVER(0x0819)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x081A)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)MaxTime, (osuint32)sizeof(osuint32)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
          COVER(0x081B)
          #if osdErrorHook 
             COVER(0x081C)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveGetISRMaxExecutionTime_ISRID(ISRID)
             osSaveGetISRMaxExecutionTime_MaxTime(MaxTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrIEIllegalAddr);
          #if osdErrorHook 
             COVER(0x081D)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x081E)
   }
   ELSEWITHCOVER(0x081F)
#endif /* osdSC== ... */
#endif /* osdNumberOfCat2ISRs ... */
#endif /* STATUS_LEVEL ... */
/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfCat2ISRs > 0))
   COVER(0x0820)
   *MaxTime = oskTPExecutionTime[osdNumberOfAllTasks + ISRID] - 
              osTPMinRemainingExeTime[osdNumberOfAllTasks + ISRID];

   return E_OK;
#endif

}/* END of function: GetISRMaxExecutionTime */

#endif /* osdGetISRMaxExecutionTimeNotUsed */


/*-+--------------------------------------------------------------------------
 / GetTaskMinInterArrivalTime
 /
 / Funktion:
 /   Computes the minimum time between tasks activations
 / Context:
 /   No restrictions.
 /   This is an additional OS-API function, not specified by OSEK-OS or
 /   AutosarOS.
 / Parameter:
 /   TaskID:   The Identifier of the task, the minimum interarrival time shall
 /             be computed for.
 /   *MinTime: The variable, the computed minimum interarrival time is returned
 /             to.
 / Returnvalue:
 /   E_OK:    A valid minimum interarrival time could be computed.
 /   E_OS_ID: TaskID is not a valid task identifier.
 /--------------------------------------------------------------------------*/

#ifndef osdGetTaskMinInterArrivalTimeNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskMinInterArrivalTime(TaskType TaskID,  osTPTimeStampRefType MinTime)
{
   /* CovComment 4:  Function: osGetTaskMinInterArrivalTime */
   COVER(0x0821)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x0822)
#if osdNumberOfAllTasks > 0
   COVER(0x0823)
   if (TaskID >= osdNumberOfAllTasks)
#else
   osdDummyRead(TaskID)
   osdDummyRead(MinTime)
#endif
   {
      COVER(0x0824)
      #if osdErrorHook 
         COVER(0x0825)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskMinInterArrivalTime_TaskID(TaskID)
         osSaveGetTaskMinInterArrivalTime_MinTime(MinTime)
      #endif
      osAPIError(E_OS_ID, osdErrTMWrongTaskID);
      #if osdErrorHook 
         COVER(0x0826)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ID;
   }
#if osdNumberOfAllTasks > 0 /* prevent unreachable code */
   ELSEWITHCOVER(0x0827)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplTaskAccess[TaskID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0828)
      #if osdErrorHook 
         COVER(0x0829)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskMinInterArrivalTime_TaskID(TaskID)
         osSaveGetTaskMinInterArrivalTime_MinTime(MinTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrTMNoAccess);
      #if osdErrorHook 
         COVER(0x082A)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x082B)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      COVER(0x082C)
      if (osAddressParamValid((osuint32)MinTime, (osuint32)sizeof(osuint32)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
         COVER(0x082D)
          #if osdErrorHook 
             COVER(0x082E)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveGetTaskMinInterArrivalTime_TaskID(TaskID)
             osSaveGetTaskMinInterArrivalTime_MinTime(MinTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrTMIllegalAddr);
          #if osdErrorHook 
             COVER(0x082F)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x0830)
   }
   ELSEWITHCOVER(0x0831)
#endif /* osdSC== ... */
#endif /* osdNumberOfAllTasks ... */
#endif /* STATUS_LEVEL ... */
   
/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfAllTasks > 0))
   COVER(0x0832)
   *MinTime = oskTPFrameTime[TaskID];
   
   return E_OK;
#endif

}/* END of function: GetTaskMinInterArrivalTime */

#endif /* osdGetTaskMinInterArrivalTimeNotUsed */


/*-+--------------------------------------------------------------------------
 / GetISRMinInterArrivalTime
 /
 / Funktion:
 /   Computes the minimum time between successive interrupts.
 / Context:
 /   no restrictions
 /   This is an additional OS-API function, not specified by OSEK-OS or
 /   AutosarOS.
 / Parameter:
 /   ISRID:    The Identifier of the ISR, the minimum interarrival time shall
 /             be computed for
 /   *MinTime: The variable, the computed minimum interarrival time is returned
 /             to.
 / Returnvalue:
 /   E_OK:    A valid minimum interarrival time could be computed.
 /   E_OS_ID: ISRID is not a valid ISR identifier.
 /--------------------------------------------------------------------------*/

#ifndef osdGetISRMinInterArrivalTimeNotUsed
osqFunc1 StatusType osqFunc2 osGetISRMinInterArrivalTime(ISRType ISRID, osTPTimeStampRefType MinTime)
{
   /* CovComment 4:  Function: osGetISRMinInterArrivalTime */
   COVER(0x0833)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x0834)
#if osdNumberOfCat2ISRs > 0
   COVER(0x0835)
   if (ISRID >= osdNumberOfCat2ISRs)
#else
   osdDummyRead(ISRID)
   osdDummyRead(MinTime)
#endif
   {
      COVER(0x0836)
      #if osdErrorHook 
         COVER(0x0837)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetISRMinInterArrivalTime_ISRID(ISRID)
         osSaveGetISRMinInterArrivalTime_MinTime(MinTime)
      #endif
      osAPIError(E_OS_ID, osdErrMIWrongISRID);
      #if osdErrorHook 
         COVER(0x0838)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ID;
   }
#if osdNumberOfCat2ISRs > 0
   ELSEWITHCOVER(0x0839)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplISRAccess[ISRID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x083A)
      #if osdErrorHook 
         COVER(0x083B)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetISRMinInterArrivalTime_ISRID(ISRID)
         osSaveGetISRMinInterArrivalTime_MinTime(MinTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrMINoAccess);
      #if osdErrorHook 
         COVER(0x083C)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x083D)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)MinTime, (osuint32)sizeof(osuint32)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
          COVER(0x083E)
          #if osdErrorHook 
             COVER(0x083F)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveGetISRMinInterArrivalTime_ISRID(ISRID)
             osSaveGetISRMinInterArrivalTime_MinTime(MinTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrMIIllegalAddr);
          #if osdErrorHook 
             COVER(0x0840)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x0841)
   }
   ELSEWITHCOVER(0x0842)
#endif /* osdSC== ... */
#endif /* osdNumberOfCat2ISRs ... */
#endif /* STATUS_LEVEL ... */
/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfCat2ISRs > 0))
   COVER(0x0843)
   *MinTime = oskTPFrameTime[osdNumberOfAllTasks + ISRID];

   return E_OK;
#endif

}/* END of function: GetISRMinInterArrivalTime */

#endif /* osdGetISRMinInterArrivalTimeNotUsed */


/*-+--------------------------------------------------------------------------
 / GetTaskMaxBlockingTime
 /
 / Funktion:
 /   Computes the maximum blocking time for a pair of task and resource.
 /   Instead of a resource, an interrupt lock can be selected.
 / Context:
 /   no restrictions
 /   This is an additional OS-API function, not specified by OSEK-OS or
 /   AutosarOS.
 / Parameter:
 /   TaskID:     The Identifier of the task.
 /   BlockType:  One of OS_ALL_INTERRUPTS, OS_OS_INTERRUPTS or OS_RESOURCE.
 /   ResourceID: If BlockType==OS_RESOURCE, the identifier of the resource.
 /               If BlockType!=OS_RESOURCE, any value (not considered).
 /   *MaxTime:   The variable, the computed maximum time is returned to.
 / Returnvalue:
 /   E_OK:    A valid maximum execution time could be computed.
 /   E_OS_ID: TaskID is not a valid task identifier or ResourceID is not
 /            a valid resource identifier.
 /--------------------------------------------------------------------------*/

#ifndef osdGetTaskMaxBlockingTimeNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskMaxBlockingTime(TaskType TaskID, BlockTypeType BlockType, 
                                                      ResourceType ResourceID, osTPTimeRefType MaxTime)
{
   /* CovComment 4:  Function: osGetTaskMaxBlockingTime */
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x0844)
#if osdNumberOfAllTasks > 0
   COVER(0x0845)
   if (TaskID >= osdNumberOfAllTasks)
#else
   osdDummyRead(TaskID)
   osdDummyRead(BlockType)
   osdDummyRead(ResourceID)
   osdDummyRead(MaxTime)
#endif
   {
      COVER(0x0846)
      #if osdErrorHook 
         COVER(0x0847)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskMaxBlockingTime_TaskID(TaskID)
         osSaveGetTaskMaxBlockingTime_BlockType(BlockType)
         osSaveGetTaskMaxBlockingTime_ResourceID(ResourceID)
         osSaveGetTaskMaxBlockingTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ID, osdErrTBWrongTaskID);
      #if osdErrorHook 
         COVER(0x0848)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ID;
   }
#if osdNumberOfAllTasks > 0 /* prevent unreachable code */
   ELSEWITHCOVER(0x0849)
#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplTaskAccess[TaskID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x084A)
      #if osdErrorHook 
         COVER(0x084B)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetTaskMaxBlockingTime_TaskID(TaskID)
         osSaveGetTaskMaxBlockingTime_BlockType(BlockType)
         osSaveGetTaskMaxBlockingTime_ResourceID(ResourceID)
         osSaveGetTaskMaxBlockingTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrTBNoAccessToTask);
      #if osdErrorHook 
         COVER(0x084C)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x084D)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)MaxTime, (osuint32)sizeof(osuint32)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
          COVER(0x084E)
          #if osdErrorHook 
            COVER(0x084F)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
            osSaveGetTaskMaxBlockingTime_TaskID(TaskID)
            osSaveGetTaskMaxBlockingTime_BlockType(BlockType)
            osSaveGetTaskMaxBlockingTime_ResourceID(ResourceID)
            osSaveGetTaskMaxBlockingTime_MaxTime(MaxTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrTBIllegalAddr);
          #if osdErrorHook 
            COVER(0x0850)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x0851)
   }
   ELSEWITHCOVER(0x0852)
#endif /* osdSC== ... */
#endif /* osdNumberOfAllTasks ... */
#endif /* STATUS_LEVEL ... */

/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfAllTasks > 0))

   switch (BlockType)
   {
      case OS_ALL_INTERRUPTS:
         COVER(0x0853)
         *MaxTime = oskTPBlockTime[TaskID][osdRESID_ALL_INTERRUPTS] - 
                    osTPMinRemainingBlockTime[TaskID][osdRESID_ALL_INTERRUPTS];
         break;
      case OS_OS_INTERRUPTS:
         COVER(0x0854)
         *MaxTime = oskTPBlockTime[TaskID][osdRESID_OS_INTERRUPTS] - 
                    osTPMinRemainingBlockTime[TaskID][osdRESID_OS_INTERRUPTS];
         break;
      case OS_RESOURCE:
         COVER(0x0855)
#if (STATUS_LEVEL == EXTENDED_STATUS)
         COVER(0x0856)
#if osdNumberOfAllResources > 0
         if (ResourceID >= osdNumberOfAllResources)
         {
            COVER(0x0857)
#endif
             #if osdErrorHook 
                COVER(0x0858)
                /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
                /* = API function SuspendOSInterrupts */
                osSaveDisableLevelNested();
                /* KB end osekHWosSaveDisableLevelNested */
                osSaveGetTaskMaxBlockingTime_TaskID(TaskID)
                osSaveGetTaskMaxBlockingTime_BlockType(BlockType)
                osSaveGetTaskMaxBlockingTime_ResourceID(ResourceID)
                osSaveGetTaskMaxBlockingTime_MaxTime(MaxTime)
             #endif
             osAPIError(E_OS_ID, osdErrTBWrongResourceID);
             #if osdErrorHook 
                COVER(0x0859)
                /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
                /* = API function ResumeOSInterrupts */
                osRestoreEnableLevelNested();
                /* KB end osekHWosRestoreEnableLevelNested */
             #endif
             return E_OS_ID;
#if osdNumberOfAllResources > 0  /* prevent unreachable code */
         }
         ELSEWITHCOVER(0x085A)
#if ((osdSC == SC3) || (osdSC == SC4))
         /* check access rights for this object */
         if (((oskApplResourceAccess[ResourceID] & (1UL << osActiveApplicationID)) == 0))
         {
            COVER(0x085B)
            #if osdErrorHook 
               COVER(0x085C)
               /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
               /* = API function SuspendOSInterrupts */
               osSaveDisableLevelNested();
               /* KB end osekHWosSaveDisableLevelNested */
               osSaveGetTaskMaxBlockingTime_TaskID(TaskID)
               osSaveGetTaskMaxBlockingTime_BlockType(BlockType)
               osSaveGetTaskMaxBlockingTime_ResourceID(ResourceID)
               osSaveGetTaskMaxBlockingTime_MaxTime(MaxTime)
            #endif
            osAPIError(E_OS_ACCESS, osdErrTBNoAccessToResource);
            #if osdErrorHook 
               COVER(0x085D)
               /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
               /* = API function ResumeOSInterrupts */
               osRestoreEnableLevelNested();
               /* KB end osekHWosRestoreEnableLevelNested */
            #endif
            return E_OS_ACCESS;
         }
         ELSEWITHCOVER(0x085E)
#endif /* osdSC== ... */
#endif
#endif /* STATUS_LEVEL ... */

/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfAllResources > 0))
         COVER(0x085F)
         *MaxTime = oskTPBlockTime[TaskID][ResourceID] - 
                    osTPMinRemainingBlockTime[TaskID][ResourceID];
         break;
#endif
      default:
         COVER(0x0860)
#if (STATUS_LEVEL == EXTENDED_STATUS)
         COVER(0x0861)
         #if osdErrorHook 
            COVER(0x0862)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveGetTaskMaxBlockingTime_TaskID(TaskID)
            osSaveGetTaskMaxBlockingTime_BlockType(BlockType)
            osSaveGetTaskMaxBlockingTime_ResourceID(ResourceID)
            osSaveGetTaskMaxBlockingTime_MaxTime(MaxTime)
         #endif
         osAPIError(E_OS_ID, osdErrTBWrongResourceID);
         #if osdErrorHook 
            COVER(0x0863)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         return E_OS_ID;
#else  /* STATUS_LEVEL ... */
         break;
#endif /* STATUS_LEVEL ... */
   } /* switch */
   COVER(0x0864)
   return E_OK;
#endif /* ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfAllTasks > 0)) */

}/* END of function: GetTaskMaxBlockingTime */

#endif /* osdGetTaskMaxBlockingTimeNotUsed */

/*-+--------------------------------------------------------------------------
 / GetISRMaxBlockingTime
 /
 / Funktion:
 /   Computes the maximum blocking time for a pair of ISR and resource.
 /   Instead of a resource, an interrupt lock can be selected.
 / Context:
 /   no restrictions
 /   This is an additional OS-API function, not specified by OSEK-OS or
 /   AutosarOS.
 / Parameter:
 /   ISRID:      The Identifier of the ISR.
 /   BlockType:  One of OS_ALL_INTERRUPTS, OS_OS_INTERRUPTS or OS_RESOURCE.
 /   ResourceID: If BlockType==OS_RESOURCE, the identifier of the resource.
 /               If BlockType!=OS_RESOURCE, any value (not considered).
 /   *MaxTime:   The variable, the computed maximum time is returned to.
 / Returnvalue:
 /   E_OK:    A valid maximum execution time could be computed.
 /   E_OS_ID: ISRID is not a valid ISR identifier or ResourceID is not a 
 /            resource identifier valid.
 /--------------------------------------------------------------------------*/

#ifndef osdGetISRMaxBlockingTimeNotUsed
osqFunc1 StatusType osqFunc2 osGetISRMaxBlockingTime(ISRType ISRID, BlockTypeType BlockType, 
                                                     ResourceType ResourceID, osTPTimeRefType MaxTime)
{
   /* CovComment 4:  Function: osGetISRMaxBlockingTime */
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x0865)
#if osdNumberOfCat2ISRs > 0
   COVER(0x0866)
   if (ISRID >= osdNumberOfCat2ISRs)
#else
   osdDummyRead(ISRID)
   osdDummyRead(BlockType)
   osdDummyRead(ResourceID)
   osdDummyRead(MaxTime)
#endif
   {
      COVER(0x0867)
      #if osdErrorHook 
         COVER(0x0868)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetISRMaxBlockingTime_ISRID(ISRID)
         osSaveGetISRMaxBlockingTime_BlockType(BlockType)
         osSaveGetISRMaxBlockingTime_ResourceID(ResourceID)
         osSaveGetISRMaxBlockingTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ID, osdErrIBWrongISRID);
      #if osdErrorHook 
         COVER(0x0869)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ID;
   }
#if osdNumberOfCat2ISRs > 0 /* prevent unreachable code */
   ELSEWITHCOVER(0x086A)
#if ((osdSC == SC3) || (osdSC == SC4))
   /* check access rights for this object */
   if (((oskApplISRAccess[ISRID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x086B)
      #if osdErrorHook 
         COVER(0x086C)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetISRMaxBlockingTime_ISRID(ISRID)
         osSaveGetISRMaxBlockingTime_BlockType(BlockType)
         osSaveGetISRMaxBlockingTime_ResourceID(ResourceID)
         osSaveGetISRMaxBlockingTime_MaxTime(MaxTime)
      #endif
      osAPIError(E_OS_ACCESS, osdErrIBNoAccessToISR);
      #if osdErrorHook 
         COVER(0x086D)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x086E)

   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)MaxTime, (osuint32)sizeof(osuint32)) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
          COVER(0x086F)
          #if osdErrorHook 
             COVER(0x0870)
             /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
             /* = API function SuspendOSInterrupts */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
            osSaveGetISRMaxBlockingTime_ISRID(ISRID)
            osSaveGetISRMaxBlockingTime_BlockType(BlockType)
            osSaveGetISRMaxBlockingTime_ResourceID(ResourceID)
            osSaveGetISRMaxBlockingTime_MaxTime(MaxTime)
          #endif
          osAPIError(E_OS_ILLEGAL_ADDRESS, osdErrIBIllegalAddr);
          #if osdErrorHook 
             COVER(0x0871)
             /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
             /* = API function ResumeOSInterrupts */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          return E_OS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x0872)
   }
   ELSEWITHCOVER(0x0873)
#endif /* osdSC== ... */
#endif /* osdNumberOfCat2ISRs ... */
#endif /* STATUS_LEVEL ... */

/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfCat2ISRs > 0))

   switch (BlockType)
   {
      case OS_ALL_INTERRUPTS:
         COVER(0x0874)
         *MaxTime = oskTPBlockTime[osdNumberOfAllTasks + ISRID][osdRESID_ALL_INTERRUPTS] - 
                    osTPMinRemainingBlockTime[osdNumberOfAllTasks + ISRID][osdRESID_ALL_INTERRUPTS];
         break;
      case OS_OS_INTERRUPTS:
         COVER(0x0875)
         *MaxTime = oskTPBlockTime[osdNumberOfAllTasks + ISRID][osdRESID_OS_INTERRUPTS] - 
                    osTPMinRemainingBlockTime[osdNumberOfAllTasks + ISRID][osdRESID_OS_INTERRUPTS];
         break;
      case OS_RESOURCE:
         COVER(0x0876)
#if (STATUS_LEVEL == EXTENDED_STATUS)
         COVER(0x0877)
#if osdNumberOfAllResources > 0
         COVER(0x0878)
         if (ResourceID >= osdNumberOfAllResources)
#endif
         {
             COVER(0x0879)
             #if osdErrorHook 
                /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
                /* = API function SuspendOSInterrupts */
                osSaveDisableLevelNested();
                /* KB end osekHWosSaveDisableLevelNested */
                osSaveGetISRMaxBlockingTime_ISRID(ISRID)
                osSaveGetISRMaxBlockingTime_BlockType(BlockType)
                osSaveGetISRMaxBlockingTime_ResourceID(ResourceID)
                osSaveGetISRMaxBlockingTime_MaxTime(MaxTime)
             #endif
             osAPIError(E_OS_ID, osdErrIBWrongResourceID);
             #if osdErrorHook 
                COVER(0x087A)
                /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
                /* = API function ResumeOSInterrupts */
                osRestoreEnableLevelNested();
                /* KB end osekHWosRestoreEnableLevelNested */
             #endif
             return E_OS_ID;
         }
#if osdNumberOfAllResources > 0 /* prevent unreachable code */
         ELSEWITHCOVER(0x087B)
#if ((osdSC == SC3) || (osdSC == SC4))
         /* check access rights for this object */
         if (((oskApplResourceAccess[ResourceID] & (1UL << osActiveApplicationID)) == 0))
         {
            COVER(0x087C)
            #if osdErrorHook 
               COVER(0x087D)
               /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
               /* = API function SuspendOSInterrupts */
               osSaveDisableLevelNested();
               /* KB end osekHWosSaveDisableLevelNested */
               osSaveGetISRMaxBlockingTime_ISRID(ISRID)
               osSaveGetISRMaxBlockingTime_BlockType(BlockType)
               osSaveGetISRMaxBlockingTime_ResourceID(ResourceID)
               osSaveGetISRMaxBlockingTime_MaxTime(MaxTime)
            #endif
            osAPIError(E_OS_ACCESS, osdErrIBNoAccessToResource);
            #if osdErrorHook 
               COVER(0x087E)
               /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
               /* = API function ResumeOSInterrupts */
               osRestoreEnableLevelNested();
               /* KB end osekHWosRestoreEnableLevelNested */
            #endif
            return E_OS_ACCESS;
         }
         ELSEWITHCOVER(0x087F)
#endif /* osdSC== ... */
#endif
#endif /* STATUS_LEVEL ... */
         
/* prevent unreachable code */
#if ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfAllResources > 0))
         COVER(0x0880)
         *MaxTime = oskTPBlockTime[osdNumberOfAllTasks + ISRID][ResourceID] - 
                    osTPMinRemainingBlockTime[osdNumberOfAllTasks + ISRID][ResourceID];
         break;
#endif
      default:
         COVER(0x0881)
#if (STATUS_LEVEL == EXTENDED_STATUS)
         COVER(0x0882)
         #if osdErrorHook 
            COVER(0x0883)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
            osSaveGetISRMaxBlockingTime_ISRID(ISRID)
            osSaveGetISRMaxBlockingTime_BlockType(BlockType)
            osSaveGetISRMaxBlockingTime_ResourceID(ResourceID)
            osSaveGetISRMaxBlockingTime_MaxTime(MaxTime)
         #endif
         osAPIError(E_OS_ID, osdErrIBWrongResourceID);
         #if osdErrorHook 
            COVER(0x0884)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         return E_OS_ID;
#else  /* STATUS_LEVEL ... */
         break;
#endif /* STATUS_LEVEL ... */
   }/* switch */
   COVER(0x0885)
   return E_OK;
#endif /* ((STATUS_LEVEL != EXTENDED_STATUS) || (osdNumberOfCat2ISRs > 0)) */

}/* END of function: GetISRMaxBlockingTime */

#endif /* osdGetISRMaxBlockingTimeNotUsed */
#endif /* osdTimingMeasurement */



/* KB begin osekHWTPFunctions (overwritten) */

/*-+--------------------------------------------------------------------------
 / osTPVirtTimerIsr
 /
 / Funktion:
 /   Interrupt function for the virtual timer handling. Checks for the 
 /   Identifier of the current virtual timer and performs the actions, 
 /   necessary for this timer.
 / Context:
 /   Highest Interrupt Level.
 / Parameter:
 /   none
 / Returnvalue:
 /   none
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osTPVirtTimerIsr(void)
{
   /* CovComment 4:  Function: osTPVirtTimerIsr */
   ProtectionReturnType proRet;
   
   COVER(0x0886)
   if (osTP_CurrentTimerIdx==osdTP_ExeIdx)
   {
      COVER(0x0887)
      if (oskTPExecutionTimeCheck[osTPActiveMeasuredProcess] == osdTPProtection)
      { 
         COVER(0x0888)
         proRet = osProtectionHookWrapper(E_OS_PROTECTION_TIME);
         osHandleProtectionErrors(proRet, E_OS_PROTECTION_TIME);
      }
#if osdTimingMeasurement
      else
      {
         COVER(0x0889)
         /* give blocking time the possibility to become the current one*/
         (void) osTPStopVirtTimer(osdTP_ExeIdx);
         osTPMinRemainingExeTime[osTPActiveMeasuredProcess] = 0; /* mark as overflow */
         /* reload the remaining execution time for this process */
         osTPExecutionTime[osTPActiveMeasuredProcess] = oskTPExecutionTime[osTPActiveMeasuredProcess];
         /* restart the execution timer to have the virual timers in the same state as before */
         osTPStartVirtTimer(osdTP_ExeIdx, oskTPExecutionTime[osTPActiveMeasuredProcess]);
      }
      COVER(0x088A)
#else
      ELSEWITHCOVER(0x088B)
#endif /*osdTimingMeasurement*/
   }
   else
   {  /* case osTP_CurrentTimerIdx==osdTP_BlockIdx */
      /* time is over */
      COVER(0x088C)
      if (oskTPBlockTimeCheck[osTPActiveBlockingProcess][osTPCurrentResource[osTPActiveBlockingProcess]] 
          == osdTPProtection)
      {
         COVER(0x088D)
         proRet = osProtectionHookWrapper(E_OS_PROTECTION_LOCKED);
         osHandleProtectionErrors(proRet, E_OS_PROTECTION_LOCKED);
      }
      else
      {
         COVER(0x088E)
         /* Error: Blocking time is to big to measure */
         osAbortSystem(osdErrBMTimeTooBigToMeasure);
      }
   }

}/*osTPVirtTimerIsr()*/

/* KB end osekHWTPFunctions */

#endif /* osdTimingProtectionUsed */
#endif /* ((osdSC == SC2) || (osdSC == SC4)) */


/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE timeprot.c */
