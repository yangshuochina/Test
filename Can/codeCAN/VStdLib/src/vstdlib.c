/* Kernbauer Version: 1.16 Konfiguration: VStdLib Erzeugungsgangnummer: 45 */


/* KB begin VStd_LLFileHeaderC */
/**********************************************************************************************************************
| Project Name: vstdlib library
|    File Name: vstdlib_ll.c
|
|  Description        : lowlevel part of the implementation of standard 
|                       Vector functions.
|                       TriCore specific implementation supports 1766,1767,1768,1796,1797,1XXX
|                               
|----------------------------------------------------------------------------------------------------------------------
|               C O P Y R I G H T
|----------------------------------------------------------------------------------------------------------------------
|   Copyright (c) by Vector Informatik GmbH.     All rights reserved.
|
|   This software is copyright protected and proprietary to 
|   Vector Informatik GmbH. Vector Informatik GmbH grants to you 
|   only those rights as set out in the license conditions. All 
|   other rights remain with Vector Informatik GmbH.
|
|----------------------------------------------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|----------------------------------------------------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------------------------------------------------
| Ml           Patrick Markl             Vector Informatik GmbH
| Hs           Hannes Haas               Vector Informatik GmbH
| Bir          Holger Birke              Vector Informatik GmbH
|----------------------------------------------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|----------------------------------------------------------------------------------------------------------------------
| Date       Version   Author  Description
|----------  --------  ------  ----------------------------------------------------------------------------------------
|2006-01-13  01.00.00    Ml     - Creation
|2006-03-24  01.01.00    Hs     - HL part updated
|2006-08-10  01.02.00    Bir    - HL part updated + update to RI15 (include Interrupthandling)
|2007-02-28  01.03.00    Bir    - HL part updated
|2007-09-05  01.04.00    Bir    - ESCAN00022224: Add Compiler support for GHS
|2007-09-05  01.05.00    Bir    - ESCAN00023626: Add new derivative 1797
|2008-10-31  01.06.00    Bir    - ESCAN00029952: Add new derivative 1767
|                               - ESCAN00031266: trap occur due to endinit-protected registers may fail to be written
|2008-10-31  01.07.00    Bir    - ESCAN00032533: Add new derivative 1768
|2009-05-17  01.08.00    Bir    - ESCAN00035061: Get Compiler version out of compiler
|*********************************************************************************************************************/
/* KB end VStd_LLFileHeaderC */

/**********************************************************************************************************************
|
| Project Name : vstdlib library
|    File Name : vstdlib_HL.c
|
|  Description : Highlevel part of the implementation of standard Vector functions
|                               
|----------------------------------------------------------------------------------------------------------------------
|               C O P Y R I G H T
|----------------------------------------------------------------------------------------------------------------------
|   Copyright (c) by Vector Informatik GmbH.     All rights reserved.
|
|   This software is copyright protected and proprietary to 
|   Vector Informatik GmbH. Vector Informatik GmbH grants to you 
|   only those rights as set out in the license conditions. All 
|   other rights remain with Vector Informatik GmbH.
|
|----------------------------------------------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|----------------------------------------------------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------------------------------------------------
| Ml           Patrick Markl             Vector Informatik GmbH
| HH           Hartmut Hoerner           Vector Informatik GmbH
| Ms           Gunnar Meiss              Vector Informatik GmbH
| Et           Thomas Ebert              Vector Informatik GmbH
| Bus          Sabine Buecherl           Vector Informatik GmbH
| Hs           Hannes Haas               Vector Informatik GmbH
| BWR          Brandon Root              Vector CANtech Inc.
| Ard          Thomas Arnold             Vector Informatik GmbH
|----------------------------------------------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|----------------------------------------------------------------------------------------------------------------------
| Date       Version   Author  Description
|----------  --------  ------  ----------------------------------------------------------------------------------------
|2003-09-06  00.01.00    Ml     - Creation
|                        Ml     - Added HC12 Cosmic
|2003-10-07  00.02.00    Ml/HH  - v_lib input merged
|2004-04-10  00.03.00    Ml     - Added headerfile
|                        Ml     - Added HC08 Cosmic
|                        Ml     - removed channel dependence for assertions
|                        Ml     - Added function to retrieve the IRQ state
|                        Ml     - Added VSTD_FULL_LIB_SUPPORT switch
|2004-04-27  01.00.00    Ml     - resolved issues found in code inspection
|2004-05-17  01.01.00    Ml     - added assertion
|                        Ml     - added define section
|                        Ml     - changed memoryqualifier
|                        Ml     - changed bugfix into releaseversion
|                        Ml     - added check for correct v_def.h version
|2004-05-18  01.02.00    Ms/HH  - changed memory qualifier concept
|2004-06-01  01.03.00    Ml     - corrected define for IRQ functions
|2004-08-06  01.04.00    Ml     - Split of library into HL and LL
|2004-08-18  01.05.00    Ml     - Added function qualifier
|2004-09-06  01.06.00    Ml     - ESCAN00009316: no changes
|2004-09-27  01.07.00    Ml     - no changes
|                        Ml     - changed namingconvention     
|2004-09-29  01.08.00    Et     - ESCAN00009692: split VStdMemCpy in VStdRamMemCpy and VStdRomMemCpy   
|2004-10-01  01.09.00    Ml     - ESCAN00009731: usage of standard type qualifiers
|2004-10-26  01.10.00    Ml     - ESCAN00010000: LL parts are split into HW specific files
|2004-11-09  01.11.00    Ml     - added VStd_LLPostInclude
|2004-11-15  01.12.00    Ml     - ESCAN00010533: Added optimized memory functions for HC08
|                        Ml     - ESCAN00010672: Added memory qualifier
|2005-01-24  01.13.00    Ml     - ESCAN00010670: Added memset
|2005-02-25  01.14.00    Ml     - ESCAN00011215: Added VStdInitPowerOn
|2005-04-26  01.15.00    Ml     - ESCAN00012047: Encapsulate VStdInitPowerOn
|            01.16.00    Ml     - ESCAN00012105: no changes
|2005-05-30  02.00.00    Ml     - ESCAN00013446: Development concept changed
|2005-09-12  02.01.00    Ml     - ESCAN00013512: Fixed Misra warnings
|2005-11-06  02.02.00    Ml     - ESCAN00014183: Added token for defines and types
|2005-11-28  02.03.00    Ml     - Changed version defines
|                        Ml     - ESCAN00014428: VStdMemClr/Set are now internal functions
|                        Ml     - ESCAN00014409: No changes       
|2005-12-12  02.04.00    Ml/Bus - ESCAN00012774: insert comments for different resource categories
|2006-01-12  02.05.00    Ml     - ESCAN00014897: Changed interrupt handling
|2006-01-14  02.06.00    Ml     - ESCAN00014908: Adapted interrupt control handling for RI1.5
|2006-02-07  02.07.00    Ml     - Added VStdGetInterruptState
|                        Hs     - ESCAN00015802: no changes
|2006-05-09  02.08.00    Ces    - ESCAN00016289: Insert memory qualifier V_MEMROM3 to VStdRomMemCpy()
|2006-05-24  02.09.00    BWR    - ESCAN00016488: Compiler error in VStdSuspendAllInterrupts and VStdResumeAllInterrupts
|2006-07-16  02.10.00    Ml     - Changed return type of VStdGetInterruptState from uint8 to tVStdIrqStateType
|2006-07-31  02.11.00    Ml     - ESCAN00017133: Changed assertion handling
|2006-08-04  02.12.00    Ml     - ESCAN00017188: no changes
|2006-11-20  02.13.00    Ml     - Removed VStdGetInterruptState
|2006-12-07  02.14.00    Ml     - Changed switch names
|                        Ml     - ESCAN00018889: Fixed code inspection report findings
|                               - ESCAN00018880: No changes
|2007-01-27  02.15.00    Ml,Ard - ESCAN00019251: VStdLib HL usable for RI1.4 and RI1.5
|2007-02-19  02.16.00    Ml     - ESCAN00019672: include os.h for Autosar
|                        Ml     - ESCAN00019674: Changed names of interrupt lock macros
|*********************************************************************************************************************/

#include "v_cfg.h"
#include "v_def.h"

#if defined(V_OSTYPE_AUTOSAR)
# include "os.h"
#endif


#include "osek.h"
/* KB begin VStd_LLPostInclude */
/* KB end VStd_LLPostInclude */

/*********************************************************************************************************************/
/* Version check                                                                                                     */
/*********************************************************************************************************************/

/* KB begin VStd_LLVersionCheck */
#if(VSTDLIB_TRICORE_VERSION != 0x0108)
# error "Lowlevel Source and Header files are inconsistent!"
#endif
#if(VSTDLIB_TRICORE_RELEASE_VERSION != 0x00)
# error "Lowlevel Source and Header file are inconsistent!"
#endif
/* KB end VStd_LLVersionCheck */

/* Highlevel versioncheck */
#if(VSTDLIB__COREHLL_VERSION != 0x0216)
# error "HighLevel Source and Header files are inconsistent!"
#endif

#if(VSTDLIB__COREHLL_RELEASE_VERSION != 0x00)
# error "HighLevel Source and Header file are inconsistent!"
#endif

/* V_def.h Compatibility Check */
#if(COMMON_VDEF_VERSION < 0x0220)
# error "VstdLib requires at least v_def.h in version 2.20.00!"
#endif

/* KB begin VStd_LLCompilerPlatformCheck */
#if defined (C_COMP_TASKING_TRICORE_MULTICAN)   
#else
# error "Platform / Compiler not supported!"
#endif
/* KB end VStd_LLCompilerPlatformCheck */

/*********************************************************************************************************************/
/* Constants                                                                                                         */
/*********************************************************************************************************************/
/* ROM CATEGORY 4 START*/
/* KB begin VStdDeclare_LocalConst */
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kVStdMainVersion    = (vuint8)(((vuint16)VSTDLIB_TRICORE_VERSION) >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kVStdSubVersion     = (vuint8)(((vuint16)VSTDLIB_TRICORE_VERSION) & 0x00FF);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kVStdReleaseVersion = (vuint8)( (vuint16)VSTDLIB_TRICORE_RELEASE_VERSION);
/* KB end VStdDeclare_LocalConst */
/* ROM CATEGORY 4 END*/

/*********************************************************************************************************************/
/* Local Types                                                                                                       */
/*********************************************************************************************************************/
/* RAM CATEGORY 1 START*/
/* KB begin VStdDeclare_LocalTypes */
/* KB end VStdDeclare_LocalTypes */
/* RAM CATEGORY 1 END*/


/*********************************************************************************************************************/
/* Defines                                                                                                           */
/*********************************************************************************************************************/
/* KB begin VStdDefine_IrqStateMask */
/* KB end VStdDefine_IrqStateMask */
/* KB begin VStdDefine_LocalDefines */

#define VSTD_STATIC_DECL static

/* KB end VStdDefine_LocalDefines */

#if !defined(VSTD_STATIC_DECL)
# define VSTD_STATIC_DECL static
#endif 


#define DET_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"


/*********************************************************************************************************************/
/* Local data definitions                                                                                            */
/*********************************************************************************************************************/
# if defined(VSTD_ENABLE_INTCTRL_HANDLING)
#  if defined(VSTD_ENABLE_DEFAULT_INTCTRL)
/* RAM CATEGORY 1 START*/
VSTD_STATIC_DECL tVStdIrqStateType vstdInterruptOldStatus; /* old IRQ state, variable not static because of some inline asm!! */
/* RAM CATEGORY 1 END*/
#  endif
#  if defined(VSTD_ENABLE_DEFAULT_INTCTRL) || defined(VSTD_ENABLE_APPL_INTCTRL) 
/* RAM CATEGORY 1 START*/
static vsintx vstdInterruptCounter;   /* interrupt counter */
/* RAM CATEGORY 1 END*/
#  endif
# endif

#define DET_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/* KB begin VStdDeclare_LocalVariables */
/* KB end VStdDeclare_LocalVariables */

/*********************************************************************************************************************/
/* Local function declarations                                                                                       */
/*********************************************************************************************************************/
/* KB begin VStdDeclare_LocalFunctions */
/* KB end VStdDeclare_LocalFunctions */

/*********************************************************************************************************************/
/* Macros                                                                                                            */
/*********************************************************************************************************************/

#if defined(VSTD_ENABLE_DEBUG_SUPPORT)
# define VStdAssert(p,e)       if(!(p)){ ApplVStdFatalError(e); }
#else
# define VStdAssert(p,e)
#endif

/*********************************************************************************************************************/
/* Functions                                                                                                         */
/*********************************************************************************************************************/

/*********************************************************************************************************************
Name          : VStdInitPowerOn
Called by     : init routine
Preconditions : none
Parameters    : none
Return values : none
Description   : initializes the global data of the VStdLib
*********************************************************************************************************************/
/* CODE CATEGORY 4 START*/
VSTD_API_0 void VSTD_API_1 VStdInitPowerOn(void)
{
  /* KB begin VStdInitPowerOn_InitLocalParameter */
  /* KB end VStdInitPowerOn_InitLocalParameter */

# if defined(VSTD_ENABLE_INTCTRL_HANDLING)
#  if defined(VSTD_ENABLE_DEFAULT_INTCTRL)
    vstdInterruptOldStatus = (tVStdIrqStateType)0;
#  endif
#  if defined(VSTD_ENABLE_DEFAULT_INTCTRL) || defined(VSTD_ENABLE_APPL_INTCTRL) 
  vstdInterruptCounter = (vsintx)0;
#  endif
# endif
  
  /* KB begin VStdInitPowerOn */
  /* KB end VStdInitPowerOn */
}
/* CODE CATEGORY 4 END*/

/*********************************************************************************************************************
Name          : VStdMemSetInternal
Called by     : -
Preconditions : -
Parameters    : void *pDest         : destination pointer          
                vuint8 nPattern     : the value which is used for filling
                vuint16 nCnt        : number of bytes to be filled 
Return values : none
Description   : fills nCnt bytes from address pDest on with the nPattern value
*********************************************************************************************************************/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdMemSetInternal(void *pDest, vuint8 nPattern, vuint16 nCnt)
{
# if defined(VSTD_HL_ENABLE_SUPPORT_MEM_FCT)
  /* KB begin VStdMemSet_FillMemory */
  /* KB end VStdMemSet_FillMemory */
# else 
  /* Defaultimplementation */
  VStdAssert(pDest!=V_NULL, kVStdErrorMemClrInvalidParameter);
  while(nCnt > (vuint16)0x0000)
  {  
    nCnt--;
    ((vuint8*)pDest)[nCnt] = nPattern;
  } 
# endif
}
/* CODE CATEGORY 1 END*/

/*********************************************************************************************************************
Name          : VStdMemClr
Called by     : -
Preconditions : -
Parameters    : void *pDest         : destination pointer          
                vuint16 nCnt        : number of bytes to be filled 
Return values : none
Description   : fills nCnt bytes from address pDest on with 0x00 pattern
*********************************************************************************************************************/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdMemClrInternal(void *pDest, vuint16 nCnt)
{
# if defined(VSTD_HL_ENABLE_SUPPORT_MEM_FCT)
  /* KB begin VStdMemClr_ClearMemory */
  /* KB end VStdMemClr_ClearMemory */
# else 
  /* Defaultimplementation */
  VStdAssert(pDest!=V_NULL, kVStdErrorMemClrInvalidParameter);
  while(nCnt > (vuint16)0x0000)
  {  
    nCnt--;
    ((vuint8*)pDest)[nCnt] = (vuint8)0x00;
  } 
# endif
}
/* CODE CATEGORY 1 END*/

/*********************************************************************************************************************
Name          : VStdRamMemCpy
Called by     : -
Preconditions : -
Parameters    : void *pDest       : RAM destination pointer          
                void* pSrc        : RAM source pointer               
                vuint16      nCnt : number of bytes to be copied 
Return values : none
Description   : moves nCnt bytes from RAM pSrc to RAM pDest
*********************************************************************************************************************/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdRamMemCpy(void *pDest, void* pSrc, vuint16 nCnt)
{
# if defined(VSTD_HL_ENABLE_SUPPORT_MEM_FCT)
  /* KB begin VStdRamMemCpy_CopyMemory */
  /* KB end VStdRamMemCpy_CopyMemory */
# else
  /* Defaultimplementation */
  VStdAssert(pDest!=V_NULL, kVStdErrorMemCpyInvalidParameter);
  VStdAssert(pSrc!=V_NULL, kVStdErrorMemCpyInvalidParameter);
  while(nCnt > (vuint16)0x0000)
  {  
    nCnt--;
    ((vuint8*)pDest)[nCnt] = ((vuint8*)pSrc)[nCnt];
  } 
# endif
}
/* CODE CATEGORY 1 END*/

/*********************************************************************************************************************
Name          : VStdRomMemCpy
Called by     : -
Preconditions : -
Parameters    : void *pDest       : ROM destination pointer          
                void* pSrc        : ROM source pointer               
                vuint16      nCnt : number of bytes to be copied 
Return values : none
Description   : moves nCnt bytes from ROM pSrc to RAM pDest
*********************************************************************************************************************/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdRomMemCpy(void *pDest, V_MEMROM1 void V_MEMROM2 V_MEMROM3 *pSrc, vuint16 nCnt)
{  
# if defined(VSTD_HL_ENABLE_SUPPORT_MEM_FCT)
  /* KB begin VStdRomMemCpy_CopyMemory */
  /* KB end VStdRomMemCpy_CopyMemory */
# else
  /* Defaultimplementation */
  VStdAssert(pDest!=V_NULL, kVStdErrorMemCpyInvalidParameter);
  VStdAssert(pSrc!=V_NULL, kVStdErrorMemCpyInvalidParameter);
  while(nCnt > (vuint16)0x0000)
  {
    nCnt--;
    ((vuint8*)pDest)[nCnt] = ((V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 *)pSrc)[nCnt];
  } 
# endif
}
/* CODE CATEGORY 1 END*/


/*********************************************************************************************************************/
/* Additional functions for memory manipulation                                                                      */
/*********************************************************************************************************************/
/* KB begin VStdDefine_AdditionalMemoryFunction */
/* KB end VStdDefine_AdditionalMemoryFunction */

/*********************************************************************************************************************
The functions below require a plattform specific implementation, therefore 
they are not available for the standard implementation of the library!
*********************************************************************************************************************/
# if defined(VSTD_ENABLE_INTCTRL_HANDLING)
/*********************************************************************************************************************
Name          : VStdSuspendAllInterrupts
Called by     : -
Preconditions : -
Parameters    : none
Return values : none
Description   : disables the global IRQ and saves the current IRQ state
*********************************************************************************************************************/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdSuspendAllInterrupts(void)
{
  VStdDeclareGlobalInterruptOldStatus
#   if defined(VSTD_ENABLE_OSEK_INTCTRL)
  VStdGlobalInterruptDisable();
  VStdGetGlobalInterruptOldStatus(vstdInterruptOldStatus);
#   else
  VStdAssert(vstdInterruptCounter < 127, kVStdErrorIntDisableTooOften);
  if(vstdInterruptCounter == 0) 
  {
    VStdGlobalInterruptDisable();
    VStdGetGlobalInterruptOldStatus(vstdInterruptOldStatus);
  }
  vstdInterruptCounter++;
#   endif
}
/* CODE CATEGORY 1 END*/

/*********************************************************************************************************************
Name          : VStdResumeAllInterrupts
Called by     : -
Preconditions : -
Parameters    : none
Return values : none
Description   : restores the previous IRQ state
*********************************************************************************************************************/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdResumeAllInterrupts(void)
{
  VStdDeclareGlobalInterruptOldStatus
#   if defined(VSTD_ENABLE_OSEK_INTCTRL)
  VStdPutGlobalInterruptOldStatus(vstdInterruptOldStatus);
  VStdGlobalInterruptRestore();
#   else
  VStdAssert(vstdInterruptCounter > 0, kVStdErrorIntRestoreTooOften);
  vstdInterruptCounter--;
  if(vstdInterruptCounter == 0)
  {
    VStdPutGlobalInterruptOldStatus(vstdInterruptOldStatus);
    VStdGlobalInterruptRestore();
  }
#   endif
}
/* CODE CATEGORY 1 END*/

/*********************************************************************************************************************/
/* Additional functions for interrupt manipulation                                                                   */
/*********************************************************************************************************************/
/* KB begin VStdDefine_AdditionalIrqSupportFunction */
/****************************************************************************
| NAME:             CanUnlockWdtcon
| CALLED BY:        
| PRECONDITIONS:    Interrupts must be disabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Unlock ENDINIT in register WDRCON0
****************************************************************************/
/* CODE CATEGORY 3 START*/
void VStdUnlockInit(void)
{
  vuint32 wdtcon0, wdtcon1;
  wdtcon0 = VSTD_WDTCON0;
  wdtcon1 = VSTD_WDTCON1;
  wdtcon0 &= 0xFFFFFF01UL;
  wdtcon0 |= 0x000000F0UL;
  wdtcon0 |= (wdtcon1 & 0x0000000CUL);
  VStdIsync(); /*maybe problems with timing*/
  VSTD_WDTCON0 = wdtcon0;
  wdtcon0 &= 0xFFFFFFF0UL;
  wdtcon0 |= 0x00000002UL;
  VStdIsync(); /*maybe problems with timing*/
  VSTD_WDTCON0 = wdtcon0;
  VStdIsync(); /*maybe problems with timing*/
  wdtcon0 = VSTD_WDTCON0; /*read back to spend time waiting for Endinit bit is cleared (Note in tech. ref.)*/
}
/* CODE CATEGORY 3 END*/
/****************************************************************************
| NAME:             CanLockWdtcon
| CALLED BY:        
| PRECONDITIONS:    Interrupts must be disabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Lock ENDINIT in register WDRCON0
****************************************************************************/
/* CODE CATEGORY 3 START*/
void VStdLockInit(void)
{
  vuint32 wdtcon0, wdtcon1;
  wdtcon0 = VSTD_WDTCON0;
  wdtcon1 = VSTD_WDTCON1;
  wdtcon0 &= 0xFFFFFF01UL;
  wdtcon0 |= 0x000000F0UL;
  wdtcon0 |= (wdtcon1 & 0x0000000CUL);
  VStdIsync(); /*maybe problems with timing*/
  VSTD_WDTCON0 = wdtcon0;
  wdtcon0 &= 0xFFFFFFF0UL;
  wdtcon0 |= 0x00000003UL;
  VStdIsync(); /*maybe problems with timing*/
  VSTD_WDTCON0 = wdtcon0;
  VStdIsync(); /*maybe problems with timing*/
}
/* CODE CATEGORY 3 END*/
/* KB end VStdDefine_AdditionalIrqSupportFunction */

# endif
/************   Organi, Version 3.8.0 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.9.0 Vector-Informatik GmbH  ************/
