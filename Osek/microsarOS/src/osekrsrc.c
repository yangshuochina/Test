/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekrsrc.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.59
|
|  Description: osek API-functions for handling resources
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

/* CovComment 0:  file: osekrsrc.c */

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

#if ((osdNumberOfAllResources - osdNumberOfInternalResources) > 0)

#if STATUS_LEVEL == EXTENDED_STATUS
#define osdResAccessMaskBits (8U*sizeof(osResAccessMaskType))
#endif

#ifndef osdGetResourceNotUsed
/*-+--------------------------------------------------------------------------
 / GetResource
 / ===========
 /
 / Syntax:          StatusType GetResource ( ResourceType <ResID> )
 / Parameter (In):  ResID Reference to resource
 / Parameter (Out): none
 / Description:     This call serves to enter critical sections in the code 
 /                  that are assigned to the resource referenced by <ResID>. 
 /                  A critical section must always be left using 
 /                  ReleaseResource.
 / Particularities: Nested resource occupation is only allowed if the 
 /                  inner critical sections are completely executed within 
 /                  the surrounding critical section. Occupation of an already
 /                  occupied resource is also strictly forbidden!
 /                  Corresponding calls to GetResource and ReleaseResource
 /                  should appear within the same function on the same function
 /                  level.
 /                  Services which are points of rescheduling for non preemptable
 /                  tasks (TerminateTask, ChainTask, Schedule and WaitEvent,
 /                  must not be used in critical sections.
 /                  Additionally, critical sections must be left before 
 /                  completion of an interrupt service routine.
 /                  Generally speaking, critical sections should be short.
 /                  The service may be called from an ISR and from task level.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Resource <ResID> is invalid, E_OS_ID
 /                  - Attempt to get resource which is already occupied by any 
 /                    task or ISR, or the statically assigned priority of the
 /                    calling task or interrupt routine is higher than the 
 /                    calculated ceiling priority, E_OS_ACCESS
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/GetResource
 */
osqFunc1 StatusType osqFunc2 osGetResource(ResourceType ResId)
{
   /* CovComment 4:  Function: osGetResource */
   /* KB begin osekHWosGRLocalCeilingPrioDecl (default) */
   osPrioType ceilingPrio; /* task priority or interrupt level depending on type of resource */
   /* KB end osekHWosGRLocalCeilingPrioDecl */
#if (osdNumberOfInterruptResources > 0)
   /* KB begin osekHWosGRLocalLevelDecl (overwritten) */
   osPrioType currentLevel;
   /* KB end osekHWosGRLocalLevelDecl */
#endif

   OS_GR_ENTRY()
   COVER(0x0463)
#if STATUS_LEVEL == EXTENDED_STATUS

   COVER(0x0464)
 #if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_GetResource & osCurrentContext) == 0)
   {
      COVER(0x0465)
      #if osdErrorHook 
      COVER(0x0466)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrGRCallContext);
      #if osdErrorHook 
      COVER(0x0467)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0468)
 #endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if ((ResId >= osrNumberOfAllResources)  
 #if osdNumberOfInternalResources>0
  #if (osdNumberOfTaskResources != osdNumberOfAllResources) /* just keep lint happy! */
       /* if osrNumberOfTaskResources == osrNumberOfAllResources the sub-condition below 
        * causes lint to expect ResId might be >= osrNumberOfAllResources in the main 
        * part of the function and therfore out of bound array accesses might occur. 
        * Anyhow, the sub-condition above guarantees that this is detected, an error 
        * message is send and the function returns, so that no out of bounds array
        * accesses can ever occur.*/
       ||  (((ResId < osrNumberOfTaskResources)
   #if  (((osdNumberOfTaskResources - osdNumberOfInternalResources) > 0) || osdLib)
               &&  (ResId >= (osrNumberOfTaskResources - osrNumberOfInternalResources))
   #endif
           ))
  #else /* the lint preprocessor if */
   #if  (((osdNumberOfTaskResources - osdNumberOfInternalResources) > 0) || osdLib)
               ||  (ResId >= (osrNumberOfTaskResources - osrNumberOfInternalResources))
   #endif /* osdNumberOfTaskResources - ... */
  #endif /* the lint preprocessor if */
 #endif /* osdNumberOfInternalResources>0 */
      )
   {
      COVER(0x0469)
      #if osdErrorHook 
      COVER(0x046A)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      #endif
      osAPIError(E_OS_ID, osdErrGRWrongResourceID);
      #if osdErrorHook 
      COVER(0x046B)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x046C)

 #if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplResourceAccess[ResId] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x046D)
      #if osdErrorHook 
      COVER(0x046E)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGRNoAccess);
      #if osdErrorHook 
      COVER(0x046F)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0470)
 #endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x0471)
      #if osdErrorHook 
      COVER(0x0472)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrGRIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x0473)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x0474)

   /* checks if resource is already occupied */
   if (osResOccupationCounter[ResId] != 0)
   {
      COVER(0x0475)
      #if osdErrorHook 
      COVER(0x0476)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGRResourceOccupied);
      #if osdErrorHook 
      COVER(0x0477)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return  E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0478)

#endif /* STATUS_LEVEL == EXTENDED_STATUS */

   /* KB begin osekHWosGRCeilingPrioAssignment (default) */
   COVER(0x0479)
   ceilingPrio= oskResCeilingPrio[ResId];
   /* KB end osekHWosGRCeilingPrioAssignment */

#if (osdNumberOfInterruptResources > 0)

 #if (osdNumberOfTaskResources > 0)
   if(ResId>=osrNumberOfTaskResources)
   {
      COVER(0x047A)
 #else
   COVER(0x047B)
 #endif /* (osdNumberOfTaskResources > 0) */
      /* switch to higher processing level */
      #if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x047C)
      /* KB begin osekHWosGRCompareIntLevel (overwritten) */
      /* check if level and ceiling level/prio match */
      if(osGetInitialLevel() > ceilingPrio)
      {
         COVER(0x047D)
         osAPIError(E_OS_ACCESS, osdErrGRWrongPrio);
         OS_GR_EXIT()
         return  E_OS_ACCESS;
      }
      /* KB end osekHWosGRCompareIntLevel */

      #endif /* STATUS_LEVEL == EXTENDED_STATUS */

      /* KB begin osekHWosSaveCurrentLevelLocal (default) */
      COVER(0x047E)
      currentLevel = osGetLevel();
      /* KB end osekHWosSaveCurrentLevelLocal */

      /* KB begin osekHWosDisableLevel (overwritten) */
      /* disable interrupts up to system level */
      osDisableLevel();
      /* KB end osekHWosDisableLevel */      
      OS_GR_START_CRITICAL()
      /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      #endif    
      /* KB end osekHWosDisableTimProtInterrupt */

      #if osdTimingProtectionUsed
      COVER(0x047F)
      osTPStartTimerBlock(ResId);
      #endif /* osdTimingProtectionUsed */

      /* KB begin osekHWosSaveCurrentLevelGlobal (overwritten) */
      osResSavedPrio[ResId] = currentLevel;
      /* KB end osekHWosSaveCurrentLevelGlobal */

      /* KB begin osekIncDispatcherLock (default) */
      #if (osdFullPreempt || osdMixedPreempt)
         COVER(0x0480)
         osLockDispatcher++;
      #endif
      /* KB end osekIncDispatcherLock */      
      #if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x0481)
      osResGlobalOccupationCounter++;
      osResOccupationCounter[ResId] = osResGlobalOccupationCounter;
      #endif /* STATUS_LEVEL == EXTENDED_STATUS */

      #if STATUS_LEVEL == EXTENDED_STATUS
      /* counter needed only for mixed resource system (task, interrupt and internal) */
      
      /* increment counter only if task is accessing */
      if (osIntNestingDepth == 0)
      {
         COVER(0x0482)
         osTcbResourceCounter[osActiveTaskIndex]++;
      }
      ELSEWITHCOVER(0x0483)
      #endif /* STATUS_LEVEL == EXTENDED_STATUS */
   

      #if osdORTIDebug
      if (osIntNestingDepth == 0)
      {
         COVER(0x0484)
         /* save taskindex */
         osResActualTask[ResId]= osActiveTaskIndex;
      }
      ELSEWITHCOVER(0x0485)
      #endif /* osdORTIDebug */

 #if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
      COVER(0x0486)
      if (osIntNestingDepth == 0)
      {    /* Called from task level */
           COVER(0x0487)
           /* link all previously locked resources to new one */
           osResNextLockedResource[ResId] = osTcbLockedResource[osActiveTaskIndex];
           /* new one is the last locked resource */
           osTcbLockedResource[osActiveTaskIndex] = ResId;
      }
      else
      {    /* Called from ISR level */
           COVER(0x0488)
           /* link all previously locked resources to new one */
           osResNextLockedResource[ResId] = osIcbLockedResource[osActiveISRID];
           /* new one is the last locked resource */
           osIcbLockedResource[osActiveISRID] = ResId;
      }
 #endif /* (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4)) */

      COVER(0x0489)
      /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      #endif   
      /* KB end osekHWosEnableTimProtInterrupt */
      OS_GR_END_CRITICAL()
      /* KB begin osekHWosSetCeilingLevel (overwritten) */
      /* set level if ceiling level is above current level */
      if(currentLevel < ceilingPrio)
      {
         COVER(0x048A)
         osSetLevel(ceilingPrio);
      }
      else
      {
         COVER(0x048B)
         osSetLevel(currentLevel);
      }
      /* KB end osekHWosSetCeilingLevel */      
 #if (osdNumberOfTaskResources > 0) 
   }
   else
   {
      COVER(0x048C)
      /* stay on task level - business as usual */
      
 #endif /* (osdNumberOfTaskResources > 0) */
#endif /* (osdNumberOfInterruptResources > 0) */

#if (osdNumberOfTaskResources > 0)

 #if STATUS_LEVEL == EXTENDED_STATUS
      if (osIntNestingDepth != 0)
      {
         COVER(0x048D)
         #if osdErrorHook 
         COVER(0x048E)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         #endif /* osdErrorHook */
         osAPIError(E_OS_ACCESS, osdErrGRISRNoAccessRights);
         #if osdErrorHook 
         COVER(0x048F)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif /* osdErrorHook */
         OS_GR_EXIT()
         return E_OS_ACCESS;
      }
      ELSEWITHCOVER(0x0490)
      if (oskTcbHomePrio[osActiveTaskIndex] < ceilingPrio)
      {
         COVER(0x0491)
         #if osdErrorHook 
         COVER(0x0492)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         #endif /* osdErrorHook */
         osAPIError(E_OS_ACCESS, osdErrGRWrongPrio);
         #if osdErrorHook 
         COVER(0x0493)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif /* osdErrorHook */
         OS_GR_EXIT()
         return  E_OS_ACCESS;
      }
      ELSEWITHCOVER(0x0494)

  #if osdEnableAssertions
   COVER(0x0495)
   #if !osdLib
   COVER(0x0496)
      /* checks access-rights of the task */
      if ((oskResAccessMask[ResId][osActiveTaskIndex/osdResAccessMaskBits] &
           ((((osResAccessMaskType)1U) << (osdResAccessMaskBits-1U)) >> 
            (osuint8)(osActiveTaskIndex % osdResAccessMaskBits))) == 0U)
      {
         COVER(0x0497)
         #if osdErrorHook 
         COVER(0x0498)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         #endif /* osdErrorHook */
         osAPIError(E_OS_SYS_API_ERROR, osdErrGRNoAccessRights);
         #if osdErrorHook 
         COVER(0x0499)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif /* osdErrorHook */
         OS_GR_EXIT()
         return  E_OS_SYS_API_ERROR;
      }
      ELSEWITHCOVER(0x049A)
   #else /* osdLib */
   {
      const osResAccessMaskType *maskPtr;
      osResAccessMaskType mask;

      COVER(0x049B)
      maskPtr= &oskResAccessMask[0][0];
      maskPtr+= ResId * (((osrNumberOfAllTasks-1)/osdResAccessMaskBits)+1)
                + osActiveTaskIndex/osdResAccessMaskBits;

      mask= *maskPtr;
      if ((mask & ((((osResAccessMaskType)1U) << (osdResAccessMaskBits-1U)) >> 
                   (osuint8)(osActiveTaskIndex % osdResAccessMaskBits))) == 0)
      {
         COVER(0x049C)
         #if osdErrorHook 
         COVER(0x049D)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         #endif /* osdErrorHook */
         osAPIError(E_OS_SYS_API_ERROR, osdErrGRNoAccessRights);
         #if osdErrorHook 
         COVER(0x049E)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif /* osdErrorHook */
         OS_GR_EXIT()
         return  E_OS_SYS_API_ERROR;
      }
      ELSEWITHCOVER(0x049F)
   }
   #endif /* osdLib */
  #endif /* assertions */

 #else /* STATUS_LEVEL == STANDARD_STATUS */
   if (osIntNestingDepth != 0)
   {
      COVER(0x04A0)
      return E_OK;
   }
   ELSEWITHCOVER(0x04A1)
 #endif /* STANDARD_STATUS */

   /* next assertion checks if there is no bit set in the ready bits */
 #if osdRTSize == 1
   COVER(0x04A2)
   osSysErrAssert(((osQReadyPrios & oskResCeilingPrioMask[ResId]) == 0U), osdErrGRPriorityOccupied)
 #else /* osdRTSize == 0 */
   COVER(0x04A3)
   osSysErrAssert(((osQReadyPrios[oskResCeilingPrioOffset[ResId]] & 
                    oskResCeilingPrioMask[ResId]) == 0U), osdErrGRPriorityOccupied)  /* NOCOV */
 #endif /* osdRTSize == 1 */

   COVER(0x04A4)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_GR_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   #if osdTimingProtectionUsed
   COVER(0x04A5)
   osTPStartTimerBlock(ResId);
   #endif /* osdTimingProtectionUsed */

 #if (CC==BCC2) || (CC==ECC2)
   COVER(0x04A6)
   oskQActivationQueues[ceilingPrio][0]= osActiveTaskIndex;
 #else /* BCC1 || ECC1 */
   COVER(0x04A7)
   osQReadyTask[ceilingPrio]= osActiveTaskIndex;
 #endif /* (CC==BCC2) || (CC==ECC2) */

   osResSavedPrio[ResId]= osActiveTaskPrio;

   if (ceilingPrio < osActiveTaskPrio)
   {
      COVER(0x04A8)
      osActiveTaskPrio= ceilingPrio;
   }
   ELSEWITHCOVER(0x04A9)

   if (ceilingPrio < osHighReadyTaskPrio)
   {
      COVER(0x04AA)
      osHighReadyTaskPrio = ceilingPrio;
      osHighReadyTaskIndex = osActiveTaskIndex;
   }
   ELSEWITHCOVER(0x04AB)

 #if osdORTIDebug
   COVER(0x04AC)
   osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
 #endif /* osdORTIDebug */

 #if osdRTSize == 1
   COVER(0x04AD)
   osQReadyPrios |= oskResCeilingPrioMask[ResId];
 #else /* osdRTSize == 0 */
   COVER(0x04AE)
   osQReadyPrios[oskResCeilingPrioOffset[ResId]] |= oskResCeilingPrioMask[ResId];
 #endif /* osdRTSize == 1 */

 #if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x04AF)
   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
   COVER(0x04B0)
   /* counter needed only for mixed resource system (task, interrupt and internal) */
   
   /* increment counter only if task is accessing */
    #if (osdNumberOfInterruptResources > 0)
   COVER(0x04B1)
   if (osIntNestingDepth == 0)
    #endif /* (osdNumberOfInterruptResources > 0) */
   {
      COVER(0x04B2)
      osTcbResourceCounter[osActiveTaskIndex]++;
   }
    #if (osdNumberOfInterruptResources > 0)
   ELSEWITHCOVER(0x04B3)
    #else
   COVER(0x04B4)
    #endif /* (osdNumberOfInterruptResources > 0) */
   #endif /* (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0) */

   osResGlobalOccupationCounter++;
 #endif /* STATUS_LEVEL == EXTENDED_STATUS */

 #if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
   COVER(0x04B5)
   /* link all previously locked resources to new one */
   osResNextLockedResource[ResId] = osTcbLockedResource[osActiveTaskIndex];
   /* new one is the last locked resource */
   osTcbLockedResource[osActiveTaskIndex] = ResId;
 #endif /* (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4)) */

   COVER(0x04B6)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_GR_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

 #if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug
   COVER(0x04B7)
   osResActualTask[ResId]= osActiveTaskIndex;
 #endif /* ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug */
 #if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x04B8)
   osResOccupationCounter[ResId]= osResGlobalOccupationCounter;
 #endif /* STATUS_LEVEL == EXTENDED_STATUS */

 #if (osdNumberOfInterruptResources > 0)

   }
      
 #endif /* (osdNumberOfInterruptResources > 0) */
#endif /* (osdNumberOfTaskResources > 0) */

   COVER(0x04B9)
   OS_GR_EXIT()
   return E_OK;
} /* END OF osGetResource */
#endif /* osdGetResourceNotUsed */


#ifndef osdReleaseResourceNotUsed
/*-+--------------------------------------------------------------------------
 / ReleaseResource
 / ===============
 /
 / Syntax:          StatusType ReleaseResource ( ResourceType <ResID> )
 / Parameter (In):  ResID Reference to resource
 / Parameter (Out): none
 / Description:     ReleaseResource is the counterpart of GetResource and
 /                  serves to leave critical sections in the code that are 
 /                  assigned to the resource referenced by <ResID>.
 / Particularities: Resources have to be released in the opposit order of
 /                  occupation.  
 /                  Corresponding calls to GetResource and ReleaseResource
 /                  should appear within the same function on the same function
 /                  level.
 /                  Services which are points of rescheduling for non preemptable
 /                  tasks (TerminateTask, ChainTask, Schedule and WaitEvent,
 /                  must not be used in critical sections.
 /                  Additionally, critical sections must be left before 
 /                  completion of an interrupt service routine.
 /                  Generally speaking, critical sections should be short.
 /                  The service may be called from an ISR and from task level.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Resource <ResID> is invalid, E_OS_ID
 /                  - Attempt to release a resource which is not occupied by 
 /                    any task or ISR, or another resource has to be released 
 /                    before, E_OS_NOFUNC
 /                  - Attempt to release a resource which has a lower ceiling 
 /                    priority than the statically assigned priority of the 
 /                    calling task or interrupt servide routine, E_OS_ACCESS
 /                  - Call with disabled interrupts, E_OS_DISABLEDINT
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/ReleaseResource
 */
osqFunc1 StatusType osqFunc2 osReleaseResource(ResourceType ResId)
{
   /* CovComment 4:  Function: osReleaseResource */
   OS_RR_ENTRY()
   COVER(0x04BA)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x04BB)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_ReleaseResource & osCurrentContext) == 0)
   {
      COVER(0x04BC)
      #if osdErrorHook 
      COVER(0x04BD)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrRRCallContext);
      #if osdErrorHook 
      COVER(0x04BE)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x04BF)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if ((ResId >= osrNumberOfAllResources)  
#if osdNumberOfInternalResources>0
#if (osdNumberOfTaskResources != osdNumberOfAllResources) /* just keep lint happy! */
       /* if osrNumberOfTaskResources == osrNumberOfAllResources the sub-condition below 
        * causes lint to expect ResId might be >= osrNumberOfAllResources in the main 
        * part of the function and therfore out of bound array accesses might occur. 
        * Anyhow, the sub-condition above guarantees that this is detected, an error 
        * message is send and the function returns, so that no out of bounds array
        * accesses can ever occur.*/
       ||  (((ResId < osrNumberOfTaskResources)
#if  (((osdNumberOfTaskResources - osdNumberOfInternalResources) > 0) || osdLib)
               &&  (ResId >= (osrNumberOfTaskResources - osrNumberOfInternalResources))
#endif
           ))
#else /* the lint preprocessor if */
#if  (((osdNumberOfTaskResources - osdNumberOfInternalResources) > 0) || osdLib)
               ||  (ResId >= (osrNumberOfTaskResources - osrNumberOfInternalResources))
#endif /* osdNumberOfTaskResources - ... */
#endif /* the lint preprocessor if */
#endif /* osdNumberOfInternalResources>0 */
      )
   {
      COVER(0x04C0)
      #if osdErrorHook 
      COVER(0x04C1)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif
      osAPIError(E_OS_ID, osdErrRRWrongResourceID);
      #if osdErrorHook 
      COVER(0x04C2)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x04C3)

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplResourceAccess[ResId] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x04C4)
      #if osdErrorHook 
      COVER(0x04C5)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif
      osAPIError(E_OS_ACCESS, osdErrRRNoAccess);
      #if osdErrorHook 
      COVER(0x04C6)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x04C7)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   if (osIntAPIStatus != 0)
   {
      COVER(0x04C8)
      #if osdErrorHook 
      COVER(0x04C9)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif
      osAPIError(E_OS_DISABLEDINT, osdErrRRIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x04CA)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_DISABLEDINT;
   }
   ELSEWITHCOVER(0x04CB)


   if (osResOccupationCounter[ResId] == 0)
   {
      COVER(0x04CC)
      #if osdErrorHook 
      COVER(0x04CD)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif
      osAPIError(E_OS_NOFUNC, osdErrRRNotOccupied);
      #if osdErrorHook 
      COVER(0x04CE)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x04CF)
   
   if (osResOccupationCounter[ResId] != osResGlobalOccupationCounter)
   {
      COVER(0x04D0)
      #if osdErrorHook 
      COVER(0x04D1)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif
      osAPIError(E_OS_NOFUNC, osdErrRRWrongSequence);
      #if osdErrorHook 
      COVER(0x04D2)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x04D3)

#endif

#if (osdNumberOfInterruptResources > 0)
#if (osdNumberOfTaskResources > 0)
   if(ResId>=osrNumberOfTaskResources)
   {
      COVER(0x04D4)
#else
   COVER(0x04D5)
#endif
      /* switch from higher processing level */
      #if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x04D6)
      /* KB begin osekHWosRRCompareIntLevel (overwritten) */
      /* check if level and ceiling level/prio match */
      if(osGetInitialLevel() > oskResCeilingPrio[ResId])
      {
         COVER(0x04D7)
         osAPIError(E_OS_ACCESS, osdErrRRWrongPrio);
         OS_RR_EXIT()
         return  E_OS_ACCESS;
      }
            
      /* KB end osekHWosRRCompareIntLevel */
      #endif

      /* KB begin osekHWosDisableLevel (overwritten) */
      /* disable interrupts up to system level */
      osDisableLevel();
      /* KB end osekHWosDisableLevel */      
      OS_RR_START_CRITICAL()
      /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      #endif    
      /* KB end osekHWosDisableTimProtInterrupt */

      #if osdTimingProtectionUsed
      COVER(0x04D8)
      osTPStopTimerBlock(ResId);
      #endif

      /* KB begin osekDecDispatcherLock (default) */
      #if (osdFullPreempt || osdMixedPreempt)
         COVER(0x04D9)
         osLockDispatcher--;
      #endif
      /* KB end osekDecDispatcherLock */

#if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
      if (osIntNestingDepth == 0)
      {  /* called from task level */
         COVER(0x04DA)
         /* remove resource from linked list */
         osTcbLockedResource[osActiveTaskIndex] = osResNextLockedResource[ResId];
      }
      else
      {  /* called from ISR level */
         COVER(0x04DB)
         /* remove resource from linked list */
         osIcbLockedResource[osActiveISRID] = osResNextLockedResource[ResId];
      }
#endif /* (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4)) */

      #if STATUS_LEVEL == EXTENDED_STATUS
      /* counter needed only for mixed resource system (task, interrupt and internal) */
      
      /* increment counter only if task is accessing */
      if (osIntNestingDepth == 0)
      {
         COVER(0x04DC)
         osTcbResourceCounter[osActiveTaskIndex]--;
      }
      ELSEWITHCOVER(0x04DD)

      osResGlobalOccupationCounter--;
      osResOccupationCounter[ResId]= 0;
      #endif
      
      #if osdORTIDebug
      COVER(0x04DE)
      osResActualTask[ResId]= osdNoActiveTask;
      #endif
      
      #if osdFullPreempt || osdMixedPreempt
      COVER(0x04DF)
      if (osLockDispatcher == 0)
      {
         if (osSchedulePrio() != 0)
         {
            COVER(0x04E0)
            OS_RR_END_CRITICAL()
            /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
            #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
            osRestoreEnableGlobalNested();
            /* KB end osekHWosRestoreEnableGlobalNested */
            #endif   
            /* KB end osekHWosEnableTimProtInterrupt */
            OS_RR_EXIT()
            /* start dispatcher */
            OS_START_DISPATCH()
            /* KB begin osekHWosRestoreSavedLevel (default) */
            COVER(0x04E1)
            osSetLevel((osIntLevelType)osResSavedPrio[ResId]);
            /* KB end osekHWosRestoreSavedLevel */      
            /* KB begin osekHWcallosDispatcher (default) */
               /* KB begin osekHWosDispatcher (overwritten) */
               osDispatch()
               /* KB end osekHWosDispatcher */
            /* KB end osekHWcallosDispatcher */
            return E_OK;
         }
         ELSEWITHCOVER(0x04E2)
      }
      else
      {
         if (osSchedulePrio() != 0)
         {
            COVER(0x04E3)
            /* KB begin osekHWosSetStartDispatcherRR (default) */
            osStartDispatcher=osdTRUE;
            /* KB end osekHWosSetStartDispatcherRR */
         }
         ELSEWITHCOVER(0x04E4)
      }
      #endif

      OS_RR_END_CRITICAL()
      /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      #endif   
      /* KB end osekHWosEnableTimProtInterrupt */
      /* KB begin osekHWosRestoreSavedLevel (default) */
      COVER(0x04E5)
      osSetLevel((osIntLevelType)osResSavedPrio[ResId]);
      /* KB end osekHWosRestoreSavedLevel */      
#if (osdNumberOfTaskResources > 0)
   }
   else
   {
      COVER(0x04E6)
      /* stay on task level - business as usual */
      
#endif
#endif

#if (osdNumberOfTaskResources > 0)
   if (osIntNestingDepth != 0)
   {
#if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x04E7)
      #if osdErrorHook 
      COVER(0x04E8)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif /* osdErrorHook */
      osAPIError(E_OS_ACCESS, osdErrRRISRNoAccessRights);
      #if osdErrorHook 
      COVER(0x04E9)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif /* osdErrorHook */
      OS_RR_EXIT()
      return E_OS_ACCESS;
#else /*STATUS_LEVEL == EXTENDED_STATUS*/
      COVER(0x04EA)
      return E_OK;
#endif /*STATUS_LEVEL == EXTENDED_STATUS else*/
   }
   ELSEWITHCOVER(0x04EB)

#if STATUS_LEVEL == EXTENDED_STATUS
   if (oskTcbHomePrio[osActiveTaskIndex] < oskResCeilingPrio[ResId])
   {
      COVER(0x04EC)
      #if osdErrorHook 
      COVER(0x04ED)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      #endif /* osdErrorHook */
      osAPIError(E_OS_ACCESS, osdErrRRWrongPrio);
      #if osdErrorHook 
      COVER(0x04EE)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif /* osdErrorHook */
      OS_RR_EXIT()
      return  E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x04EF)
#endif /*STATUS_LEVEL == EXTENDED_STATUS*/

#if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug
   COVER(0x04F0)
   /* next assertion checks if releasing task is the same as the occupiing task */
   osSysErrAssert(osResActualTask[ResId] == osActiveTaskIndex, osdErrRRWrongTask)
#endif

   /* next assertion checks if there is a bit set in the ready bits */
#if osdRTSize == 1
   COVER(0x04F1)
   osSysErrAssert(((osQReadyPrios & oskResCeilingPrioMask[ResId]) != 0U), 
      osdErrRRCeilingPriorityNotSet) /* NOCOV */
#else
   COVER(0x04F2)
   osSysErrAssert(((osQReadyPrios[oskResCeilingPrioOffset[ResId]] &
      oskResCeilingPrioMask[ResId]) != 0U), osdErrRRCeilingPriorityNotSet) /* NOCOV */
#endif

#if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug
   COVER(0x04F3)
   osResActualTask[ResId]= osdNoActiveTask;
#endif
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x04F4)

   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
   COVER(0x04F5)
   /* counter needed only for mixed resource system (task, interrupt and internal) */
   
   /* decrement counter always, task level is assured by osdErrRRISRNoAccessRights above */
   osTcbResourceCounter[osActiveTaskIndex]--;
   #endif /* (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0) */

   osResOccupationCounter[ResId]= 0; /* not occupied */
#endif
   
   COVER(0x04F6)
   /* KB begin osekHWosSaveDisableLevel (overwritten) */
   /* save level and disable up to system level, no nesting */
   osSaveDisableLevel();
   /* KB end osekHWosSaveDisableLevel */
   OS_RR_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   #if osdTimingProtectionUsed
   COVER(0x04F7)
   osTPStopTimerBlock(ResId);
   #endif
   
#if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
   COVER(0x04F8)
   /* remove resource from linked list */
   osTcbLockedResource[osActiveTaskIndex] = osResNextLockedResource[ResId];
#endif /* (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4)) */

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x04F9)
   osResGlobalOccupationCounter--;
#endif

   osActiveTaskPrio= osResSavedPrio[ResId];

#if osdORTIDebug
   COVER(0x04FA)
   osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
#endif

#if osdRTSize == 1
   COVER(0x04FB)
   osQReadyPrios&= ~oskResCeilingPrioMask[ResId];
#else
   COVER(0x04FC)
   osQReadyPrios[oskResCeilingPrioOffset[ResId]]&= ~oskResCeilingPrioMask[ResId];
#endif


#if osdFullPreempt || osdMixedPreempt
   if (osLockDispatcher == 0)
   {
      if (osSchedulePrio() != 0)
      {
         COVER(0x04FD)
         OS_RR_END_CRITICAL()
         OS_RR_EXIT()

         /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
         osRestoreEnableGlobalNested();
         /* KB end osekHWosRestoreEnableGlobalNested */
         #endif   
         /* KB end osekHWosEnableTimProtInterrupt */
         /* KB begin osekHWosRestoreEnableLevel (overwritten) */
         /* restore level saved by osSaveDisableLevel */
         osRestoreEnableLevel();
         /* KB end osekHWosRestoreEnableLevel */

         /* start dispatcher */
         OS_START_DISPATCH()
         /* KB begin osekHWcallosDispatcher (default) */
            /* KB begin osekHWosDispatcher (overwritten) */
            osDispatch()
            /* KB end osekHWosDispatcher */
         /* KB end osekHWcallosDispatcher */
         return E_OK;
      }
      ELSEWITHCOVER(0x04FE)
   }
   ELSEWITHCOVER(0x04FF)
#endif
   OS_RR_END_CRITICAL()
   OS_RR_EXIT()

   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   /* KB begin osekHWosRestoreEnableLevel (overwritten) */
   /* restore level saved by osSaveDisableLevel */
   osRestoreEnableLevel();
   /* KB end osekHWosRestoreEnableLevel */

#if (osdNumberOfInterruptResources > 0)

   }
      
#endif
#endif

   return E_OK;
} /* END OF osReleaseResource */
#endif

#else
   /* avoid error message for empty file */
   /* KB begin osekHWosOsekRsrcDummy (default) */
   /* KB end osekHWosOsekRsrcDummy */
#endif /* ((osdNumberOfAllResources-osdNumberOfInternalResources) > 0) */


/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */
/* END OF MODULE osekrsrc.c */
