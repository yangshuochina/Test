/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekcov.h
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.02
|
|  Description: contains the implementation of the coverage macro
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2007-2010       Vector Informatik GmbH    All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEKCOV_H
#define _OSEKCOV_H

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

/* KB begin osekCoverageMacro (default) */
#if defined osdEnableCoverage
   /* size of coverage array */
   #define osdCoverageArraySize ((2308 / 8) + 1)

   /* external of coverage array */
   extern osqRAM1 osqRAM2 osuint8 osqRAM3 osCoverage[osdCoverageArraySize];

   /* coverage enabled -> the macros set the selected bit in the array */
   #define COVER(x)                     osCoverage[(x) >> 3U] |= (0x1U << ((x) & 0x7U));
   #define ELSEWITHCOVER(x)             else { COVER(x) }
#else
   /* no coverage -> macros are empty */
   #define COVER(x)
   #define ELSEWITHCOVER(x)
#endif
/* KB end osekCoverageMacro */

#endif /* double include preventer */
