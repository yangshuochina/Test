/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: Os.h
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.07
|
|  Description: standard header of the OS, to be included by the application
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2006-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* fullfills [SR:309] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:308] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* double include preventer */
#ifndef _OS_H
#define _OS_H

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

#if defined USE_QUOTE_INCLUDES
 #include "Os_Cfg.h"
#else
 #include <Os_Cfg.h>
#endif

#if (osdTypeHeaderInclude != 0)
 #if defined USE_QUOTE_INCLUDES
  #include "Std_Types.h"
 #else
  #include <Std_Types.h>
 #endif
/* there is no version check of this file since the OS is independent of these types */
#endif

#ifndef osdIncludeLibConfig
 #if (osdNumberOfAllMessages > 0) || (osdNumberOfTrustedFunctions > 0)
  #if defined USE_QUOTE_INCLUDES
   #include "usrostyp.h"
  #else
   #include <usrostyp.h>
  #endif
 #endif

 #if defined USE_QUOTE_INCLUDES
  #include "trustfct.h"
 #else
  #include <trustfct.h>
 #endif
#endif


#if defined USE_QUOTE_INCLUDES
 #include "msg.h"
#else
 #include <msg.h>
#endif

#endif/* double include preventer */

/* END OF HEADER Os.h */

