/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osektrace.c
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.14
|
|  Description: trace functions
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

#if defined USE_QUOTE_INCLUDES
 #include "osek.h"         /* includes testmac2.h which provides the trace functionality */
 #include "osekext.h"
#else
 #include <osek.h>         /* includes testmac2.h which provides the trace functionality */
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

/* CovComment 0:  file: osektrac.c */

/* KB begin osekHWTraceAdditionalInclude (default) */
/* KB end osekHWTraceAdditionalInclude */

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

#ifdef osdUseTrace

/* global variables for trace buffer */
tOsTraceBuffer osTraceBuffer[osdTraceDepth];
osuint16 osTraceBufferIndex;

/* initialise trace - call this function at the beginning of StartOS */
osqFunc1 void osqFunc2 osInitTrace(void)
{
   /* CovComment 4:  Function: osInitTrace */
   osuint16 i;

   COVER(0x0675)
   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */

   /* osdActiveTaskIndexVar is either mapped on osActiveTaskIndex or on osCtrlVars.ossActiveTaskIndex */
   osdActiveTaskIndexVar = osdNoActiveTask;
   
#if !defined osdTraceUseUserTimestamp
   /* osdSystemCounterVar is mapped either on osSystemCounter or on osCounter[0] */
   osdSystemCounterVar = 0U;
#endif
   
   for(i=0;i<osdTraceDepth;i++)
   {
      COVER(0x0676)
      osTraceBuffer[i].stateNo=0xff;
      osTraceBuffer[i].taskNo=0xff;
#ifdef osdTraceUseTimestamp
      COVER(0x0677)
      osTraceBuffer[i].timeStamp=0;
#endif
   }
   COVER(0x0678)
   osTraceBufferIndex=0;

   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
}

#endif /* defined(osdUseTrace) */

/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */
