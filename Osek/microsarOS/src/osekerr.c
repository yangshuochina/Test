/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekerr.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.37
|
|  Description: functions for handling of errors
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

/* CovComment 0:  file: osekerr.c */

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

/*-+--------------------------------------------------------------------------
 / osAbortSystem
 / Funktion:
 /              - called from system assertion osSysErrFail
 / Parameter:   uiError  error code
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
/* MISRA RULE 8.10 VIOLATION: Depending on the configuration, this function may be called only in this module.
 * Declaring this function static for some configurations would increase code complexity, too much.
 */
osqFunc1 void osqFunc2 osAbortSystem(osuint16 uiError) /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osAbortSystem */
   osDisableGlobal();

   #if osdErrorHook
   COVER(0x0379)
   osLastError = uiError; 
   osErrorHook(E_OS_SYS_ABORT);
   #else
   COVER(0x037A)
   /* KB begin dummyAssignAbortSystem (default) */
   osdDummyRead(uiError)
   /* KB end dummyAssignAbortSystem */
   #endif

   #if (osdShutdownHook || (osdNumberOfApplications > 0))
   COVER(0x037B)
   osSysShutdownOS(E_OS_SYS_ABORT);
   #else
   COVER(0x037C)
   osSysShutdownOS();
   #endif

} /* END OF osAbortSystem */

/*-+--------------------------------------------------------------------------
 / osAssertFailed
 / Funktion:
 /              - called if assertion failed, interrupts are disabled in 
 /                assertion
 / Parameter:    
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
#if (((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended)) && (osdEnableAssertions==1))
osqFunc1 void osqFunc2 osAssertFailed(void)
{
   /* CovComment 4:  Function: osAssertFailed */
   COVER(0x037D)
   #if osdErrorHook
   COVER(0x037E)
   osErrorHook(E_OS_SYS_ASSERTION);
   #endif
   COVER(0x037F)
   /* system is now shut down */

   #if (osdShutdownHook || (osdNumberOfApplications > 0))
   COVER(0x0380)
   osSysShutdownOS(E_OS_SYS_ASSERTION);
   #else
   COVER(0x0381)
   osSysShutdownOS();
   #endif

} /* END OF osAssertFailed */
#endif

/*-+--------------------------------------------------------------------------
 / osErrorHook
 / Funktion:
 /              - wrap Errorhook: disable interrupts and prevent recursive call
 / Parameter:   StatusType ErrorCode
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/

#if osdErrorHook

osqFunc1 void osqFunc2 osErrorHook(StatusType ErrorCode)
{  
   /* CovComment 4:  Function: osErrorHook */
#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   osuint8 osSavedAPIStatus;
#endif
#if ((osdSC== SC3) || (osdSC== SC4))
   osuint16 savedContext;
#endif
   
/* KB begin osekHWosErrorHookAddVars (default) */
/* KB end osekHWosErrorHookAddVars */
   COVER(0x0382)
   OS_ERHOOK_ENTRY()

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x0383)
   /* save and reset API status to prevent IntAPI errors inside errorhook */
   osSavedAPIStatus = osIntAPIStatus;
   osSetIntAPIStatus(0);
#endif

   /* KB begin osekAssignGlobalError (default) */
   #if osdORTIDebug
   osLastErrorCode=ErrorCode;
   #endif
   /* KB end osekAssignGlobalError */

   if (osErrorFlag == osdFALSE)
   {
      COVER(0x0384)
      osErrorFlag=osdTRUE; /* first entry in ErrorHook, lock ErrorHook */

#if (STATUS_LEVEL == EXTENDED_STATUS)
      COVER(0x0385)
/* MISRA RULE 16.2 VIOLATION: The (indirectly) recursive call of function osErrorHook is prevented
 * by the variable osErrorFlag.
 */
      osSysErrAssert((osCheckInterruptsDisabled() != 0), osdErrEHInterruptsEnabled) /* PRQA S 1580, 1520 */
#endif

      /* KB begin osekHWosErrorHookStartActions (default) */
      /* KB end osekHWosErrorHookStartActions */

#if ((osdSC== SC3) || (osdSC== SC4))
      savedContext = osCurrentContext;
      /* osCurrentContext might be sometimes used in the ErrorHook.
       * Therefore it must be initialized here, even if it is not always 
       * used before the next assignment. This is no MISRA error. (3198) */
      if (savedContext == osdCtxStartupHook)
      {
         COVER(0x0386)
         osCurrentContext = osdCtxErrorHook | osdCtxErrorHookFromStartupHook;   /* PRQA S 3198 */
      }
      else
      {
         COVER(0x0387)
         osCurrentContext = osdCtxErrorHook;   /* PRQA S 3198 */
      }
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

      osInterErrorHook(ErrorCode);

#if ((osdSC== SC3) || (osdSC== SC4))
      COVER(0x0388)
#if (osdNumberOfApplications > 0)
      /* Call application specific ErrorHook only if the error occured in an application specific context */
      if (0 == (savedContext & (osdCtxPreTaskHook | osdCtxPostTaskHook | osdCtxProtectionHook)))
      {
         /* osCurrentContext might be sometimes used in the ErrorHook.
          * Therefore it must be initialized here, even if it is not always 
          * used before the next assignment. This is no MISRA error. (3198) */
         if (savedContext == osdCtxStartupHook)
         {
            COVER(0x0389)
            osCurrentContext = osdCtxApplErrorHook | osdCtxErrorHookFromStartupHook;   /* PRQA S 3198 */
         }
         else
         {
            COVER(0x038A)
            osCurrentContext = osdCtxApplErrorHook;   /* PRQA S 3198 */
         }
         osInApplHook = 1;
         if ((osActiveApplicationID != INVALID_OSAPPLICATION) && (oskApplErrorHook[osActiveApplicationID] != 0))
         {  /* call application specific ErrorHook (after system ErrorHook!) */
            COVER(0x038B)
            /* KB begin osekHWApplErrorHookStart (overwritten) */
            if (oskApplIsTrusted[osActiveApplicationID] == 0)
            {
               /* set protection for application specific hook */
               osuint32 localSP;
            
               COVER(0x038C)
               /* Get the stack pointer from A10 to localSP */
               __asm("mov.d\t%0, a10":"=d"(localSP));
            
               /* save protection registers to be modified */
               osSaved_DPR1_1L = osd_mfcr(os_DPR1_1L_OFFSET);
               osSaved_DPR1_1U = osd_mfcr(os_DPR1_1U_OFFSET);
            
               osd_mtcr(os_DPR1_1L_OFFSET, (osuint32)(&osSystemStack));   /* hook ist running on remaining space on system stack */
               osd_mtcr(os_DPR1_1U_OFFSET,localSP);
            }
            /* KB end osekHWApplErrorHookStart */
            /* KB begin osekHWApplErrorHookCall (default) */
            (*oskApplErrorHook[osActiveApplicationID])(ErrorCode);
            /* KB end osekHWApplErrorHookCall */
            /* KB begin osekHWApplErrorHookEnd (overwritten) */
            if (oskApplIsTrusted[osActiveApplicationID] == 0)
            {
               COVER(0x038D)
               /* restore modified protection registers */
               osd_mtcr(os_DPR1_1L_OFFSET, osSaved_DPR1_1L);    
               osd_mtcr(os_DPR1_1U_OFFSET, osSaved_DPR1_1U);
            }
            /* KB end osekHWApplErrorHookEnd */
         }
         ELSEWITHCOVER(0x038E)
         osInApplHook = 0;
      }
      ELSEWITHCOVER(0x038F)
#endif
      osCurrentContext = savedContext;
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

      /* KB begin osekHWosErrorHookEndActions (default) */
      /* KB end osekHWosErrorHookEndActions */

      osErrorFlag=osdFALSE; /* unlock ErrorHook */
   }
   ELSEWITHCOVER(0x0390)

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x0391)
   /* restore old IntAPI status */
   osSetIntAPIStatus(osSavedAPIStatus);
#endif

   COVER(0x0392)
   OS_ERHOOK_EXIT()
} /* END OF osErrorHook */
#endif
                  

/*-+--------------------------------------------------------------------------
 / osMissingTerminateError
 / Funktion:
 /              - called on return from task function without TerminateTask
 /              - shutdown system
 / Parameter:
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
#if (STATUS_LEVEL == EXTENDED_STATUS) 
/* MISRA RULE 14.1 not violated: The OS sets the return addresses of all tasks to the 
 * address of the function osMissingTerminateError, so it becomes active whenever 
 * a tasks returns without calling TerminateTask(). MISRA-checkers will typcally 
 * be unable to detect this.
 */
osqFunc1 void osqFunc2 osMissingTerminateError(void)      /* PRQA S 1503 */ 
{
   /* CovComment 4:  Function: osMissingTerminateError */
#if ((osdSC== SC1) || (osdSC== SC2))
   COVER(0x0393)
   osDisableGlobal();

   #if osdErrorHook
   COVER(0x0394)
   osLastError = osdErrMTMissingTerminateTask; 
   osErrorHook(E_OS_MISSINGEND);
   #endif

   ShutdownOS(E_OS_MISSINGEND);
#else  /* SC3 or SC4 */

   /* KB begin osekHWosEnterPrivilegedMode (default) */
   /* KB end osekHWosEnterPrivilegedMode */

   COVER(0x0395)
   osDisableGlobal();
   osSysKillTask(osActiveTaskIndex, osdTRUE);

   /* for TerminateTask, osSchedulePrio returns always osdTRUE! */
   /* (same task is never running after TerminateTask) */
   #if (osdNumberOfAllTasks > 0)
      (void) osSchedulePrio(); 
   #endif   

   osIntSaveDisableCounter = 0;
   /* KB begin osekHWInitGlobalDisableCounter (overwritten) */
   #if osdTimingProtectionUsed
   osIntSaveDisableHighLevelCounter = 0;
   #else
   osIntSaveDisableGlobalCounter = 0;
   #endif
   /* KB end osekHWInitGlobalDisableCounter */

   /* KB begin osekHWosEnableLevel (overwritten) */
   /* enable task level interrupt */
   osEnableLevel();   
   /* KB end osekHWosEnableLevel */
   osEnableGlobal();

   /* start dispatcher */
   OS_START_DISPATCH()
   /* KB begin osekHWcallosDispatcher (default) */
      /* KB begin osekHWosDispatcher (overwritten) */
      osDispatch()
      /* KB end osekHWosDispatcher */
   /* KB end osekHWcallosDispatcher */
#endif
}
#endif

/*-+--------------------------------------------------------------------------
 / osUnhandledException
 / Funktion:
 /              - called if a unhandled exception occures
 / Parameter:
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
/* MISRA RULE 8.10 not violated: see justification for rule 14.1 below */
/* MISRA RULE 14.1 not violated: The function osUnhandledException is a general handler 
 * for all unconfigured interrupts. MISRA-checkers can typically not evaluate the 
 * link from the interrupt vector table to this function.
 */
osqFunc1 void osqFunc2 osUnhandledException(void)      /* PRQA S 1503 */ /* PRQA S 1505 */
{
   /* CovComment 4:  Function: osUnhandledException */
   COVER(0x0396)
   osAbortSystem(osdErrUEUnhandledException);

} /* END OF osUnhandledException */


/* KB begin osekHWosISRUnhandledExeptionSrc (overwritten) */
/* KB end osekHWosISRUnhandledExeptionSrc */


/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */

/* END OF MODULE osekerr.c */
