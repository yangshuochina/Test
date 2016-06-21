/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osektime.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.41
|
|  Description: osek timer handling
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH        All rights reserved.
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

/* CovComment 0:  file: osektime.c */

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

#if (osdAlarmsOnSystemTimer)
#if (osdUseHeapAlarm != 0)


#define osGetHeapTime(x) (osAlarmTime[x])
#define osGetFirstAlarm() (osAlarmHeap[0])
#define osGetActiveAlarmCount() (osAlarmHeapCount)


/*-+--------------------------------------------------------------------------
 / osInitMinHeap
 / Funktion:
 /   - initialize empty minimum heap
 / Context:
 /   OS initialization.
 /   This function is meant to be OS internal.
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static osqFunc1 void osqFunc2 osInitMinHeap(void);
static osqFunc1 void osqFunc2 osInitMinHeap(void)
{
   /* CovComment 4:  Function: osInitMinHeap */
   COVER(0x061E)
   osAlarmHeapCount = 0;
}


/*-+--------------------------------------------------------------------------
 / osClimbUpMinHeap
 / Funktion:
 /   - move last element up to correct position
 / Context:
 /   Interrupts are disabled up to the system level at least.
 /   This function is meant to be OS internal.
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static osqFunc1 void osqFunc2 osClimbUpMinHeap(osAlarmIndexType i);
static osqFunc1 void osqFunc2 osClimbUpMinHeap(osAlarmIndexType i)
{
   /* CovComment 4:  Function: osClimbUpMinHeap */
   osHeapType hItem;
   osAlarmIndexType iPreIdx;

   COVER(0x061F)
   hItem = osAlarmHeap[i];  /* copy (last) item */
   while (i != 0)
   {
      COVER(0x0620)
      /* calculate index of predecessor */
      iPreIdx = (osAlarmIndexType)(((osAlarmIndexType)(i-1)) >> 1U);
   
      osdIF_COUNTER( osGetHeapTime(hItem), >=, osGetHeapTime(osAlarmHeap[iPreIdx]))
      {
         COVER(0x0621)
         break;   /* correct position found */
      }
      ELSEWITHCOVER(0x0622)
      osdENDIF_COUNTER

      /* swap items */
      osAlarmHeap[i] = osAlarmHeap[iPreIdx];
      i = iPreIdx;
      COVER(0x0623)
   }
   osAlarmHeap[i] = hItem;   /* insert at correct position */
   COVER(0x0624)
}


/*-+--------------------------------------------------------------------------
 / osClimbDownMinHeap
 / Funktion:
 /   - move element i down to correct position
 / Context:
 /   Interrupts are disabled up to the system level at least.
 /   This function is meant to be OS internal.
 / Parameter:     i = element index
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static osqFunc1 void osqFunc2 osClimbDownMinHeap(osAlarmIndexType i);
static osqFunc1 void osqFunc2 osClimbDownMinHeap(osAlarmIndexType i)
{
   /* CovComment 4:  Function: osClimbDownMinHeap */
   osHeapType hItem;
   osAlarmIndexType iPostIdx;
   COVER(0x0625)
   iPostIdx = (((osAlarmIndexType)2*i) + (osAlarmIndexType)1);/*lint !e734*/
   /*
    * Info 734 Loss of precision (assignment)
    * The number of alarms is checked to be less than 2^(sizeof(osAlarmIndexType)-1).
    * So no overflow can occur here.
    */
   hItem = osAlarmHeap[i];  /* copy root item */
   while (iPostIdx < osAlarmHeapCount)
   {
      COVER(0x0626)
      /* MISRA RULE 21.1 not violated: (3 times) QAC may state that the array osAlarmHeap is definitely (3685) or 
       * appropriately (3689) indexed with an out or bounds value. However, QAC cannot see that this is prevented 
       * by the comparison of iPostIdx with osAlarmHeapCount above. The variable osAlarmHeapCount is in the range 
       * of 0..NumberOfAlarms while osAlarmHeap has size NumberOfAlarms+1. */
      osdIF_COUNTER(osGetHeapTime(osAlarmHeap[iPostIdx]), > , osGetHeapTime(osAlarmHeap[iPostIdx+1])) /* PRQA S 3685 */
      {
         COVER(0x0627)
         iPostIdx++;   /* set iPostIdx to smaller successor */

      }
      ELSEWITHCOVER(0x0628)
      osdENDIF_COUNTER

      osdIF_COUNTER(osGetHeapTime(osAlarmHeap[iPostIdx]), >= , osGetHeapTime(hItem)) /* PRQA S 3689 */
      {
         COVER(0x0629)
         break;        /* correct position found */

      }
      ELSEWITHCOVER(0x062A)
      osdENDIF_COUNTER

      osAlarmHeap[i] = osAlarmHeap[iPostIdx]; /* PRQA S 3689 */
      i = iPostIdx;
      iPostIdx = (((osAlarmIndexType)2*i) + (osAlarmIndexType)1);/*lint !e734, same arguments as above*/
   }
   COVER(0x062B)
   osAlarmHeap[i] = hItem;
}


/*-+--------------------------------------------------------------------------
 / osInsertMinHeap
 / Funktion:
 /   - insert new item
 / Context:
 /   Interrupts are disabled up to the system level at least.
 /   This function is meant to be OS internal.
 / Parameter:     newItem = item to insert
 / Returnvalue:
 /--------------------------------------------------------------------------*/

/* MISRA RULE 8.10 VIOLATION: Depending on the configuration the function osInsertMinHeap is called by several modules
 * or only by this module. A static implementation in these cases would increase code complexity, too much.
 */
osqFunc1 void osqFunc2 osInsertMinHeap(osHeapType newItem) /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osInsertMinHeap */
   osAlarmHeap[osAlarmHeapCount] = newItem;  /* insert new item at last position */
   osAlarmHeapCount++;
   osClimbUpMinHeap((osAlarmIndexType)(osAlarmHeapCount-1));  /* climb up until correct position reached */
   COVER(0x062C)
}

#if (osdUseHighResolutionTimer != 0)
/*-+--------------------------------------------------------------------------
 / osInsertMinHeapWithCheck
 / Funktion:
 /   - insert new item with optionally reprogramming the timer
 / Parameter:     newItem = item to insert
 / Returnvalue:   -
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osInsertMinHeapWithCheck(osHeapType newItem)
{
   /* CovComment 4:  Function: osInsertMinHeapWithCheck */
   osAlarmHeap[osAlarmHeapCount++] = newItem;  /* insert new item at last position */
   osClimbUpMinHeap((osAlarmIndexType)(osAlarmHeapCount-1));  /* climb up until correct position reached */
   if (osAlarmHeap[0] == newItem)       /* is the new item the one that will become due next? */
   {
      (void)osSetTimerInterruptPoint(osAlarmTime[newItem]);
      COVER(0x062D)
   }
   ELSEWITHCOVER(0x062E)
}
#endif


/*-+--------------------------------------------------------------------------
 / osRemoveMinItemMinHeap
 / Funktion:
 /   - get minimum item and remove item from heap
 / Context:
 /   Interrupts are disabled up to the system level at least.
 /   This function is meant to be OS internal.
 / Parameter:
 / Returnvalue: index of minimum item
 /--------------------------------------------------------------------------*/

static osqFunc1 osHeapType osqFunc2 osRemoveMinItemMinHeap(void);
static osqFunc1 osHeapType osqFunc2 osRemoveMinItemMinHeap(void)
{
   /* CovComment 4:  Function: osRemoveMinItemMinHeap */
   osHeapType hItem;

   hItem = osAlarmHeap[0];   /* copy first = minimum item */
   osAlarmHeapCount--;
   osAlarmHeap[0] = osAlarmHeap[osAlarmHeapCount];   /* move last element to first position */
   osClimbDownMinHeap(0);   /* climb down until correct position reached */
   COVER(0x062F)
   return hItem;
}

#ifndef osdCancelAlarmNotUsed

/*-+--------------------------------------------------------------------------
 / osRemoveItemMinHeap
 / Funktion:
 /   - find and remove Item
 / Context:
 /   Interrupts are disabled up to the system level at least.
 /   This function is meant to be OS internal.
 / Parameter:    aItem   = Item to remove
 / Returnvalue:  osdTRUE = Item found
 /--------------------------------------------------------------------------*/

osqFunc1 osuint8 osqFunc2 osRemoveItemMinHeap(osHeapType aItem)
{
   /* CovComment 4:  Function: osRemoveItemMinHeap */
   osAlarmIndexType iPreIdx;
   osAlarmIndexType i = 0;

   COVER(0x0630)
   osAlarmHeap[osAlarmHeapCount] = aItem;
   /* now the following loop will always terminate without range control */

   for (;;)
   {
      if (osAlarmHeap[i] == aItem)
      {
         COVER(0x0631)
         break;   /* found! */
      }
      ELSEWITHCOVER(0x0632)
      i++;
   }
#if osdEnableAssertions
   if (i >= osAlarmHeapCount)
   {
      COVER(0x0633)
       return 0;/*heap item not found*/
   }
   ELSEWITHCOVER(0x0634)
#endif
   COVER(0x0635)
   osAlarmHeapCount--;
   osAlarmHeap[i] = osAlarmHeap[osAlarmHeapCount];  /* move last item to found item */
   if (i > 0) /* (if heap element has predecessor) check array boundaries */
   {
      COVER(0x0636)
      iPreIdx =  (osAlarmIndexType)(((osAlarmIndexType)(i-1)) >> 1U);

      osdIF_COUNTER(osGetHeapTime(osAlarmHeap[i]), < , osGetHeapTime(osAlarmHeap[iPreIdx]))
      {
         (void) osClimbUpMinHeap(i);
         COVER(0x0637)
         return 1;
      }
      ELSEWITHCOVER(0x0638)
      osdENDIF_COUNTER
   }
#if (osdUseHighResolutionTimer != 0)
   else
   {
      if (osAlarmHeapCount == 0)
      {
         COVER(0x0639)
         return 3; /*heap empty*/
      }
      ELSEWITHCOVER(0x063A)
      osClimbDownMinHeap(i); /* climb down until correct position reached */
      COVER(0x063B)
      return 2;/*minimum heap item changed*/
   }
#else
   ELSEWITHCOVER(0x063C)
#endif
   
   osClimbDownMinHeap(i);  /* climb down until correct position reached */
   COVER(0x063D)
   return 1;
}
#endif /* osdCancelAlarmNotUsed */



#if (0==osdUserDefinedSystemTimer)
/*-+--------------------------------------------------------------------------
 / osWorkAlarms
 / Funktion:
 /   - inserts or reinserts an alarm into timer class
 / Context:
 /   Interrupts are enabled.
 /   This function is meant to be OS internal.
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
/* MISRA RULE 8.10 VIOLATION: The function osWorkAlarms is general code for 
 * differend platform specific OS implementations. Depending on the implementation,
 * the calling function might be located in the same file or be generated 
 * (i.e. in tcb.c). Making the function static for only some implementations would
 * increase code complexity too much. */
osqFunc1 void osqFunc2 osWorkAlarms(void)   /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osWorkAlarms */
   AlarmType alarmID;
   osHeapType t;

   COVER(0x063E)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_TI_START_CRITICAL()
   while (osGetActiveAlarmCount() != 0)
   {
      COVER(0x063F)
      alarmID = osGetFirstAlarm();

      /* is timer expired? */
      osdIF_COUNTER(osGetSystemCounter(), >= , osAlarmTime[alarmID])
      {
         COVER(0x0640)
         t = osRemoveMinItemMinHeap();  /* remove alarm */
         /* alarm has become due now: work actions */

#if (osdNumberOfVarAlarms > 0)
         COVER(0x0641)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
         COVER(0x0642)
         if (alarmID < osrNumberOfVarAlarms)
         {
            COVER(0x0643)
#endif
            if (osAlarmCycleTime[alarmID] != 0)
            {
               COVER(0x0644)
               /* cyclic alarm */
               osAlarmTime[alarmID] += osAlarmCycleTime[alarmID];
               osInsertMinHeap(t);      /* reinsert      */
            }
            else
            {
               COVER(0x0645)
               osAlarmActivated[alarmID]= 0;
            }
#endif
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
            COVER(0x0646)
#if (osdNumberOfVarAlarms > 0)
            COVER(0x0647)
         }
         else
#else
         COVER(0x0648)
#endif
         {
            if (oskStaticAlarmCycleTime[alarmID] != 0)
            {
               /* cyclic timer  */
               COVER(0x0649)
               osAlarmTime[alarmID] += oskStaticAlarmCycleTime[alarmID];
               osInsertMinHeap(t);      /* reinsert      */
            }
            else
            {
               COVER(0x064A)
               osAlarmActivated[alarmID]= 0;
            }
         }
#else
         COVER(0x064B)
#endif

#if (osdNumberOfAlarmSetEvent > 0)
         if (oskAlarmAction[alarmID] == osdAlarmActionSetEvent)
         {
            COVER(0x064C)
            /* event timer */
            (void) osSysSetEvent(oskAlarmTask[alarmID], oskAlarmEvent[alarmID]); 
         }
         else /* alarm action is not set event */
         {
            COVER(0x064D)
#else 
            COVER(0x064E)
#endif
#if (osdNumberOfAlarmCallback > 0)
            if (oskAlarmAction[alarmID] == osdAlarmActionCallback)
            {
               COVER(0x064F)
               (oskAlarmCallback[alarmID])(); 
            }
            else /* alarm action is not alarm callback */
            {
               COVER(0x0650)
#else 
               COVER(0x0651)
#endif
#if (osdNumberOfAlarmIncrementCounter > 0)
               if (oskAlarmAction[alarmID] == osdAlarmActionIncrementCounter)
               {
                  COVER(0x0652)
                  /* KB begin osekHWosRestoreEnableLevel (overwritten) */
                  /* restore level saved by osSaveDisableLevel */
                  osRestoreEnableLevel();
                  /* KB end osekHWosRestoreEnableLevel */
                  osIncrementCounter(oskAlarmIncrementCounterID[alarmID]);
                  /* KB begin osekHWosSaveDisableLevel (overwritten) */
                  /* save level and disable up to system level, no nesting */
                  osSaveDisableLevel();
                  /* KB end osekHWosSaveDisableLevel */
               }
               else /* alarm action is not increment counter */
               {
                  COVER(0x0653)
#else 
                  COVER(0x0654)
#endif
#if (osdNumberOfAlarmActivateTask > 0)
                  COVER(0x0655)
                  (void) osSysActivateTask(oskAlarmTask[alarmID]);
#endif
#if (osdNumberOfAlarmIncrementCounter > 0)
               } /* end of else path of (oskAlarmAction[alarmID] == osdAlarmActionIncrementCounter) */
#endif
#if (osdNumberOfAlarmCallback > 0)
            } /* end of else path of (oskAlarmAction[alarmID] == osdAlarmActionCallback) */
#endif
#if (osdNumberOfAlarmSetEvent > 0)
         } /* end of else path of (oskAlarmAction[alarmID] == osdAlarmActionSetEvent) */
#endif
      }
      else /* osdIF_COUNTER */
      {
#if (osdUseHighResolutionTimer != 0)
         COVER(0x0656)
         if ( osSetTimerInterruptPoint( osAlarmTime[alarmID] ))
         {
            COVER(0x0657)
#else
            COVER(0x0658)
#endif
            break;
#if (osdUseHighResolutionTimer != 0)
         }
#endif
      }osdENDIF_COUNTER
      COVER(0x0659)
      /* After each activated alarm, higher prio interrupts are enabled for 
         a brief moment. Thus, outstanding interrupts are executed. This 
         decreases interrupt latency.
      */
      OS_TI_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel (overwritten) */
      /* restore level saved by osSaveDisableLevel */
      osRestoreEnableLevel();
      /* KB end osekHWosRestoreEnableLevel */
      /* KB begin osekHWdelayTilEnableTookEffect (default) */
      /* KB end osekHWdelayTilEnableTookEffect */
      /* KB begin osekHWosSaveDisableLevel (overwritten) */
      /* save level and disable up to system level, no nesting */
      osSaveDisableLevel();
      /* KB end osekHWosSaveDisableLevel */
      OS_TI_START_CRITICAL()
   }/*while*/

#if (osdUseHighResolutionTimer != 0)
   if (osGetActiveAlarmCount() == 0)
   {
      COVER(0x065A)
      osClearTimerInterruptPoint();
   }
   ELSEWITHCOVER(0x065B)
#endif

   OS_TI_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */
   COVER(0x065C)

} /* END OF osWorkAlarms */
#endif /* (0==osdUserDefinedSystemTimer) */

#endif /*osdUseHeapAlarm != 0*/
#endif /*osdAlarmsOnSystemTimer*/

#if (osdNumberOfAlarms > 0)

/*-+--------------------------------------------------------------------------
 / osInitAlarms
 / Funktion:
 /   - prepares the RAM part of all alarms
 / Context:
 /   OS initialization.
 /   This function is meant to be OS internal.
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osInitAlarms(void)
{
   /* CovComment 4:  Function: osInitAlarms */
   osAlarmIndexType alarmID;
   COVER(0x065D)
   /* counters were already initialized (in StartOSc) */

   #if (osdUseHeapAlarm != 0) && (osdAlarmsOnSystemTimer)
   COVER(0x065E)
   osInitMinHeap();
   #endif

   for (alarmID= 0; alarmID < osrNumberOfAlarms; alarmID++)
   {
      if ((oskAlarmAutoStart[alarmID] & osActiveAppMode) != 0)
      {
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID];
#if (osdUseHeapAlarm != 0) && (osdAlarmsOnSystemTimer)
         osAlarmActivated[alarmID] = 1;
#if (osdNumberOfVarHeapAlarms > 0)
#if (osrNumberOfVarHeapAlarms != osrNumberOfAlarms)
         if (alarmID < osrNumberOfVarHeapAlarms)
         {
#endif
            osInsertMinHeap(alarmID);
            COVER(0x065F)
#if (osrNumberOfVarHeapAlarms != osrNumberOfAlarms)
         }
         ELSEWITHCOVER(0x0660)
#endif
#endif
#if (osdNumberOfStatHeapAlarms > 0)
         if ((alarmID >= osrNumberOfVarAlarms) && (alarmID < osrNumberOfVarAlarms+osrNumberOfStatHeapAlarms))
         {
            osInsertMinHeap(alarmID);
            COVER(0x0661)
         }
         ELSEWITHCOVER(0x0662)
#endif
#endif
      }
      else /* no autostart for this alarm */
      {
#if (osdUseHeapAlarm != 0)
         COVER(0x0663)
         osAlarmActivated[alarmID] = 0;
#else
         COVER(0x0664)
         osAlarmTime[alarmID] = 0;
#endif
      }
   } /* for (alarmID= 0; alarmID < osrNumberOfAlarms; alarmID++) */

#if (osdNumberOfVarAlarms > 0)

   /*complete the init of all variable alarms*/
   for (alarmID= 0; alarmID < osrNumberOfVarAlarms; alarmID++)
   {
      if ((oskAlarmAutoStart[alarmID] & osActiveAppMode) != 0)
      {
         osAlarmCycleTime[alarmID] = oskStaticAlarmCycleTime[alarmID];
         COVER(0x0665)
      }
      else
      {
         osAlarmCycleTime[alarmID] = 0;
         COVER(0x0666)
      }
   }
   COVER(0x0667)
 #endif
} /* END OF osInitAlarms */

#endif  /*!osdNumberOfAlarms*/

#if (osdSystemCounterNeeded != 0)
/* KB begin osekHWInitTimerSrc (overwritten) */
/* KB end osekHWInitTimerSrc */

/* KB begin osekHWTimerInterruptSrc (overwritten) */
#if (0==osdUseHighResolutionTimer) /* only for fixtix timer (hires timer code is generated)*/
/*-+--------------------------------------------------------------------------
 / osTimerInterrupt
 / Funktion:
 /   - category 2 Timer-Interrupt function
 /   - increments the system counter and checks the Alarms
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

/* 
   as the user can select the timer, this code has to be gerated for the
   library
*/

#if (osdSystemTimerModule == osdSTM)
static inline void osReloadSTM(void);
static inline void osReloadSTM(void)
{
   /* CovComment 4:  Function: osReloadSTM (inline function) */
   osuint32 LastCmpVal;
   COVER(0x0668)
#if (osdSystemTimer == osdSTM_CMP0 )
   COVER(0x0669)
   LastCmpVal = osSTM_CMP0;
   osSTM_CMP0  = LastCmpVal + osdSystemTimerReloadValue;
#elif (osdSystemTimer == osdSTM_CMP1 )
   COVER(0x066A)
   LastCmpVal = osSTM_CMP1;
   osSTM_CMP1  = LastCmpVal + osdSystemTimerReloadValue;
#endif /* (osdSystemTimer == ...) */

#ifdef DEBUG_USING_BREAKPOINTS
   if(( osSTM_TIMER_READ - LastCmpVal ) >= osdSystemTimerReloadValue )
   {
      COVER(0x066B)
      /*time for next IRQ already reached - just choose a later time*/
#if (osdSystemTimer == osdSTM_CMP0 )
      COVER(0x066C)
      osSTM_CMP0  = osSTM_TIMER_READ + osdSystemTimerReloadValue;
#elif (osdSystemTimer == osdSTM_CMP1 )
      COVER(0x066D)
      osSTM_CMP1  = osSTM_TIMER_READ + osdSystemTimerReloadValue;
#endif /* (osdSystemTimer == ...) */
   }
#else /*DEBUG_USING_BREAKPOINTS*/

#if STATUS_LEVEL == EXTENDED_STATUS
   if(( osSTM_TIMER_READ - LastCmpVal ) >= osdSystemTimerReloadValue )
   {
      COVER(0x066E)
      /*time for next IRQ already reached - Error*/
      osAbortSystem(osdErrTITickTime);
   }
#endif /*STATUS_LEVEL == EXTENDED_STATUS*/

#endif /*DEBUG_USING_BREAKPOINTS*/
} /* END OF osReloadSTM */
#endif

ISR(osTimerInt) 
{  
   /* CovComment 4:  Function: Timer interrupt */
   OS_TI_ENTRY();

   COVER(0x066F)
#if (osdSystemTimerModule == osdSTM)
   COVER(0x0670)
   osReloadSTM();
#endif

#if osdUseCounterAPI
   COVER(0x0671)
   osCounter[SystemTimer]++;
#else
   COVER(0x0672)
   osSystemCounter++;
#endif

#if (osdAlarmsOnSystemTimer > 0)
   COVER(0x0673)
   osEnableGlobal();

   osWorkAlarms();

   osDisableGlobal();
#endif

#if (osdScheduleTablesOnSystemTimer == 1)
   COVER(0x0674)
   osWorkScheduleTables(SystemTimer);
#endif

   OS_TI_EXIT()
} /* END OF osTimerInterrupt */
#endif /* fixtix timer*/
/* KB end osekHWTimerInterruptSrc */

#endif /*(osdSystemCounterNeeded != 0)*/

/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE osektime.c */


