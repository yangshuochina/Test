/* Kernbauer Version: 1.16 Konfiguration: VStdLib Erzeugungsgangnummer: 45 */


/* KB begin VStd_LLFileHeaderH */
/**********************************************************************************************************************
| Project Name: vstdlib library
|    File Name: vstdlib_ll.h
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
|2006-08-10  01.02.00    Bir    - HL part updated + Update to RI15 (include Interrupt handling)
|2007-02-28  01.03.00    Bir    - HL part updated
|2007-09-05  01.04.00    Bir    - ESCAN00022224: Add Compiler support for GHS
|2007-09-05  01.05.00    Bir    - ESCAN00023626: Add new derivative 1797
|2008-10-31  01.06.00    Bir    - ESCAN00029952: Add new derivative 1767 (and general derivative TC1XXX)
|                               - ESCAN00031266: trap occur due to endinit-protected registers may fail to be written
|2008-10-31  01.07.00    Bir    - ESCAN00032533: Add new derivative 1768
|2009-05-17  01.08.00    Bir    - ESCAN00035061: Get Compiler version out of compiler
|********************************************************************************************************************/
/* KB end VStd_LLFileHeaderH */

/**********************************************************************************************************************
|
| Project Name : vstdlib library
|    File Name : vstdlib_HL.h
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
|2004-05-17  01.01.00    Ml     - no changes
|                        Ml     - changed bugfix into releaseversion
|2004-05-18  01.02.00    Ms/HH  - changed memory qualifier concept
|2004-06-01  01.03.00    Ml     - no changes
|2004-08-06  01.04.00    Ml     - Added macros for local IRQ disable
|                               - Split of library into HL and LL
|2004-08-18  01.05.00    Ml     - Added function qualifier
|2004-09-06  01.06.00    Ml     - ESCAN00009316: fixed parameter type mismatch
|2004-09-27  01.07.00    Ml     - changed memcpy macros
|                        Ml     - changed namingconvention
|2004-09-29  01.08.00    Et     - ESCAN00009692: split VStdMemCpy in VStdRamMemCpy and VStdRomMemCpy   
|2004-10-01  01.09.00    Ml     - ESCAN00009731: usage of standard type qualifiers
|2004-10-26  01.10.00    Ml     - ESCAN00010000: LL parts are split into HW specific files
|2004-11-09  01.11.00    Ml     - no changes
|2004-11-15  01.12.00    Ml     - ESCAN00010533: Added optimized memory functions for HC08
|                        Ml     - ESCAN00010672: Added memory qualifier
|2005-01-24  01.13.00    Ml     - ESCAN00010670: Added memset
|2005-02-25  01.14.00    Ml     - ESCAN00011215  Added VStdInitPowerOn
|2005-04-26  01.15.00    Ml     - ESCAN00012047: Encapsulate VStdInitPowerOn
|            01.16.00    Ml     - ESCAN00012105: Change definition of VStdMemCpy
|2005-05-30  02.00.00    Ml     - ESCAN00013446: Development concept changed
|2005-09-12  02.01.00    Ml     - ESCAN00013512: Fixed Misra warnings
|2005-11-06  02.02.00    Ml     - ESCAN00014183: no changes
|2005-11-28  02.03.00    Ml     - Changed version defines
|                        Ml     - ESCAN00014428: VStdMemClr/Set are now internal functions
|                        Ml     - ESCAN00014409: Added casts for mem function defines
|2005-12-12  02.04.00    Ml/Bus - ESCAN00012774: insert comments for different resource categories
|2006-01-12  02.05.00    Ml     - ESCAN00014897: Changed interrupt handling
|2006-01-14  02.06.00    Ml     - ESCAN00014908: Adapted interrupt control handling for RI1.5
|2006-02-07  02.07.00    Ml     - Added VStdGetInterruptState
|                        Hs     - ESCAN00015802: compatibility defines added
|2006-05-09  02.08.00    Ces    - ESCAN00016289: Insert memory qualifier V_MEMROM3 to VStdRomMemCpy()
|2006-05-24  02.09.00    BWR    - ESCAN00016488: Compiler error in VStdSuspendAllInterrupts and VStdResumeAllInterrupts
|2006-07-16  02.10.00    Ml     - Changed return type of VStdGetInterruptState from uint8 to tVStdIrqStateType
|2006-07-31  02.11.00    Ml     - ESCAN00017133: Changed assertion handling
|2006-08-04  02.12.00    Ml     - ESCAN00017188: Added compatibility for older copy functions
|2006-11-20  02.13.00    Ml     - Removed VStdGetInterruptState
|2006-12-07  02.14.00    Ml     - Changed switch names
|                        Ml     - ESCAN00018889: Fixed code inspection report findings
|                               - ESCAN00018880: added casts to memXXX macros
|2007-01-27  02.15.00    Ml,Ard - ESCAN00019251: VStdLib HL usable for RI1.4 and RI1.5
|2007-02-19  02.16.00    Ml     - ESCAN00019672: no changes
|                        Ml     - ESCAN00019674: Changed names of interrupt lock macros
|*********************************************************************************************************************/

#if !defined(_VSTDLIB_H_)
# define _VSTDLIB_H_

/*********************************************************************************************************************/
/* Defines                                                                                                           */
/*********************************************************************************************************************/

/* KB begin VStdDefine_LLVersion */
/* ##V_CFG_MANAGEMENT ##CQProject : VStdLib_Tricore CQComponent : Implementation */
#define VSTDLIB_TRICORE_VERSION         0x0108
#define VSTDLIB_TRICORE_RELEASE_VERSION 0x00
/* KB end VStdDefine_LLVersion */

/* ##V_CFG_MANAGEMENT ##CQProject : VStdLib__coreHll CQComponent : Implementation */
# define VSTDLIB__COREHLL_VERSION         0x0216
# define VSTDLIB__COREHLL_RELEASE_VERSION 0x00

/* Introduced for comparibility with older VStdLib versions */
# define COMMON_VSTDLIB_VERSION             VSTDLIB__COREHLL_VERSION
# define COMMON_VSTDLIB_RELEASE_VERSION     VSTDLIB__COREHLL_RELEASE_VERSION

/* KB begin VStdDefine_AdditionalDefines */
/*define intrinsic macros for both compiler (used for Interrupt enable/disable)*/
/*lint -emacro( (718,746), Mfcr, Mtcr, DisableInt, EnableInt, Isync) */


#if defined(C_COMP_TASKING_TRICORE_MULTICAN) && !defined(MISRA_CHECK)
# if !defined(COMPILER_VERSION_1) && !defined(COMPILER_VERSION_2) && !defined(COMPILER_VERSION_3)
/* __VERSION__ define is not usable due to compiler versions 3.x expand it as string but 2.x as integer and 1.x does not support it */
#  define COMPILER_VERSION_2 /*set as default*/
# endif
# if defined(COMPILER_VERSION_1)
#  define VStdMfcr(regaddr)     _mfcr((regaddr))
#  define VStdMtcr(regaddr,val) _mtcr((regaddr),(val))
#  define VStdDisableInt()      _disable()
#  define VStdEnableInt()       _enable()
#  define VStdIsync()           _isync()
# elif defined(COMPILER_VERSION_2) || defined(COMPILER_VERSION_3)
#  define VStdMfcr(regaddr)     __mfcr((regaddr))
#  define VStdMtcr(regaddr,val) __mtcr((regaddr),(val))
#  define VStdDisableInt()      __disable()
#  define VStdEnableInt()       __enable()
#  define VStdIsync()           __isync()
# endif
#endif
/*used for lock or unlock special register access*/
#if defined( C_PROCESSOR_INFINEON_TC1766 ) || defined( C_PROCESSOR_INFINEON_TC1796 )
# define VSTD_WDTCON0  (*(vuint32 *)0xf0000020UL)
# define VSTD_WDTCON1  (*(vuint32 *)0xf0000024UL)
#else
/* defined( C_PROCESSOR_INFINEON_TC1767 ) || defined( C_PROCESSOR_INFINEON_TC1768 ) || defined( C_PROCESSOR_INFINEON_TC1797 ) || defined( C_PROCESSOR_INFINEON_TC1XXX )*/
# define VSTD_WDTCON0  (*(vuint32 *)0xF00005F0UL)
# define VSTD_WDTCON1  (*(vuint32 *)0xF00005F4UL)
#endif
/* KB end VStdDefine_AdditionalDefines */

# if !defined(VSTD_API_0)
#  define VSTD_API_0
# endif

# if !defined(VSTD_API_1)
#  define VSTD_API_1
# endif

# if !defined(VSTD_CALLBACK_0)
#  define VSTD_CALLBACK_0
# endif

# if !defined(VSTD_CALLBACK_1)
#  define VSTD_CALLBACK_1
# endif

#  if defined(VSTD_ENABLE_DEFAULT_INTCTRL) || \
      defined(VSTD_ENABLE_OSEK_INTCTRL)    || \
      defined(VSTD_ENABLE_APPL_INTCTRL)
#  define VSTD_ENABLE_INTCTRL_HANDLING
#  define VSTD_HL_ENABLE_SUPPORT_IRQ_FCT  /* compatibility for Autosar */
#  define VSTD_ENABLE_SUPPORT_IRQ_FCT
# else
#  define VSTD_DISABLE_INTCTRL_HANDLING
#  define VSTD_HL_DISABLE_SUPPORT_IRQ_FCT  /* compatibility for Autosar */
#  define VSTD_DISABLE_SUPPORT_IRQ_FCT
# endif

/*********************************************************************************************************************/
/* Error codes for assertions                                                                                        */
/*********************************************************************************************************************/
# define kVStdErrorIntDisableTooOften        ((vuint8)0x01)
# define kVStdErrorIntRestoreTooOften        ((vuint8)0x02)
# define kVStdErrorMemClrInvalidParameter    ((vuint8)0x03)
# define kVStdErrorMemCpyInvalidParameter    ((vuint8)0x04)
# define kVStdErrorFunctionNotSupportedByHw  ((vuint8)0x05)

/* KB begin VStdDefine_AdditionalErrorCodes */
/* KB end VStdDefine_AdditionalErrorCodes */

/*********************************************************************************************************************/
/* Plattforms with optimized VStdLib support                                                                         */
/*********************************************************************************************************************/
/* KB begin VStdDefine_Functionality */
#define VSTD_HL_DISABLE_SUPPORT_MEM_FCT
#define VSTD_HL_DISABLE_MEM_FCT_MAPPING
/*#define VSTD_ENABLE_SUPPORT_IRQ_FCT*/
/* KB end VStdDefine_Functionality */

/*********************************************************************************************************************/
/* Version check                                                                                                     */
/*********************************************************************************************************************/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kVStdMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kVStdSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kVStdReleaseVersion;
/* ROM CATEGORY 4 END*/

/*********************************************************************************************************************/
/* Function like macros                                                                                              */
/*********************************************************************************************************************/
/* KB begin VStdDefine_IrqLockMacros */
/* Macros for global Interrupt enable/disable----------------------------------*/
/* between Disable and Restore there must not be any function calls */
/*lint -emacro( (737), CanLL_GlobalInterruptDisable, CanLL_GlobalInterruptRestore, Mfcr) */
#if defined( VSTD_ENABLE_LOCK_LEVEL )
#  define VStdLL_GlobalInterruptDisable(localInterruptOldStatusPtr)                        \
           *(localInterruptOldStatusPtr) = VStdMfcr( 0xFE2C );         \
           VStdMtcr( 0xFE2C, ((*(localInterruptOldStatusPtr) & 0xFFFFFF00UL) |  \
                            VSTD_INTERRUPT_LOCK_LEVEL) );                                  \
           VStdIsync()
#  define VStdLL_GlobalInterruptRestore(localInterruptOldStatus)                                      \
           VStdMtcr( 0xFE2C, ( (VStdMfcr( 0xFE2C )  & 0xFFFFFF00UL) |  \
                            ((localInterruptOldStatus) & 0x000000FFUL) ) );                           \
           VStdIsync()
#else
#  define VStdLL_GlobalInterruptDisable(localInterruptOldStatusPtr)             \
           *(localInterruptOldStatusPtr) = VStdMfcr( 0xFE2C ); VStdDisableInt()
#  define VStdLL_GlobalInterruptRestore(localInterruptOldStatus)                \
           if (((localInterruptOldStatus) & 0x00000100UL)==0x00000100UL)        \
           VStdEnableInt()
#endif
/* KB end VStdDefine_IrqLockMacros */

/*********************************************************************************************************************/
/* Types                                                                                                             */
/*********************************************************************************************************************/

/* KB begin VStdDeclare_IrqStateType */
typedef vuint32 tVStdIrqStateType;
/* KB end VStdDeclare_IrqStateType */

/*********************************************************************************************************************/
/* Defines                                                                                                           */
/*********************************************************************************************************************/

#  if defined(VSTD_ENABLE_INTCTRL_HANDLING)
#   if defined(VSTD_ENABLE_DEFAULT_INTCTRL)
#    define VStdDeclareGlobalInterruptOldStatus     tVStdIrqStateType localInterruptOldStatus;
#    define VStdPutGlobalInterruptOldStatus(x)      (localInterruptOldStatus = (x))
#    define VStdGetGlobalInterruptOldStatus(x)      ((x) = localInterruptOldStatus)
#    define VStdGlobalInterruptDisable()            VStdLL_GlobalInterruptDisable(&localInterruptOldStatus)
#    define VStdGlobalInterruptRestore()            VStdLL_GlobalInterruptRestore(localInterruptOldStatus)
#   elif defined(VSTD_ENABLE_OSEK_INTCTRL)
#    define VStdDeclareGlobalInterruptOldStatus
#    define VStdPutGlobalInterruptOldStatus(x)
#    define VStdGetGlobalInterruptOldStatus(x)
#    if defined(osdOSEK2_1)
#     define VStdGlobalInterruptDisable()           OsSaveDisableGlobalNested()
#     define VStdGlobalInterruptRestore()           OsRestoreEnableGlobalNested()
#    else
#     define VStdGlobalInterruptDisable()           SuspendAllInterrupts()
#     define VStdGlobalInterruptRestore()           ResumeAllInterrupts()
#    endif
#   elif defined(VSTD_ENABLE_APPL_INTCTRL)
#    define VStdDeclareGlobalInterruptOldStatus
#    define VStdPutGlobalInterruptOldStatus(x)
#    define VStdGetGlobalInterruptOldStatus(x)
#    define VStdGlobalInterruptDisable()           VStdUserNestedDisable()
#    define VStdGlobalInterruptRestore()           VStdUserNestedRestore()
#   else
#    error "Please check VStdLib interrupt configuration!"
#   endif
#   define VStdNestedGlobalInterruptDisable() VStdGlobalInterruptDisable()
#   define VStdNestedGlobalInterruptRestore() VStdGlobalInterruptRestore()
#  endif

/* per default all memclr and memcpy functions are mapped on one routine which
   does not support different memory models */
#  define VStdMemSet(d, p, l)              VStdMemSetInternal((void*)(d), (p), (l))
#  define VStdMemNearSet(d, p, l)          VStdMemSetInternal((void*)(d), (p), (l))
#  define VStdMemFarSet(d, p, l)           VStdMemSetInternal((void*)(d), (p), (l))
#  define VStdMemClr(d, l)                 VStdMemClrInternal((void*)(d), (l))
#  define VStdMemNearClr(d, l)             VStdMemClrInternal((void*)(d), (l))
#  define VStdMemFarClr(d, l)              VStdMemClrInternal((void*)(d), (l))
#  define VStdMemCpyRamToRam(d, s, l)      VStdRamMemCpy((void*)(d), (void*)(s), (l))
#  define VStdMemCpyRomToRam(d, s, l)      VStdRomMemCpy((void*)(d), (V_MEMROM1 void V_MEMROM2 V_MEMROM3 *)(s), (l))
#  define VStdMemCpyRamToNearRam(d, s, l)  VStdRamMemCpy((void*)(d), (void*)(s), (l))
#  define VStdMemCpyRomToNearRam(d, s, l)  VStdRomMemCpy((void*)(d), (V_MEMROM1 void V_MEMROM2 V_MEMROM3 *)(s), (l))
#  define VStdMemCpyRamToFarRam(d, s, l)   VStdRamMemCpy((void*)(d), (void*)(s), (l))
#  define VStdMemCpyRomToFarRam(d, s, l)   VStdRomMemCpy((void*)(d), (V_MEMROM1 void V_MEMROM2 V_MEMROM3 *)(s), (l))

/*********************************************************************************************************************/
/* Compatibility                                                                                                     */
/*********************************************************************************************************************/
# define VStdMemCpy(d,s,l)     VStdMemCpyRamToRam((d),(s),(l))




/*********************************************************************************************************************/
/* Prototypes of functions                                                                                           */
/*********************************************************************************************************************/

/* initialization */
/* CODE CATEGORY 4 START*/
VSTD_API_0 void VSTD_API_1 VStdInitPowerOn(void);
/* CODE CATEGORY 4 END*/
/* function for moving data */
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdMemClrInternal(void *pDest, vuint16 nCnt);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdMemSetInternal(void *pDest, vuint8 nPattern, vuint16 nCnt);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdRamMemCpy(void *pDest, void *pSrc, vuint16 nCnt);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdRomMemCpy(void *pDest, V_MEMROM1 void V_MEMROM2 V_MEMROM3 *pSrc, vuint16 nCnt);
/* CODE CATEGORY 1 END*/

/* Declaration of additional memory manipulation functions */
/* KB begin VStdDeclare_AdditionalMemoryFunction */
/* KB end VStdDeclare_AdditionalMemoryFunction */

#  if defined(VSTD_ENABLE_INTCTRL_HANDLING)
/* interrupt control functions */
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdSuspendAllInterrupts(void);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
VSTD_API_0 void VSTD_API_1 VStdResumeAllInterrupts(void);
/* CODE CATEGORY 1 END*/
/* Declaration of additional Irq Support functions */
/* KB begin VStdDeclare_AdditionalIrqSupportFunction */
/* CODE CATEGORY 3 START*/
void VStdUnlockInit(void);
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
void VStdLockInit(void);
/* CODE CATEGORY 3 END*/
/* KB end VStdDeclare_AdditionalIrqSupportFunction */
#  endif

/*********************************************************************************************************************/
/* Callback functions                                                                                                */
/*********************************************************************************************************************/
# if defined(VSTD_ENABLE_DEBUG_SUPPORT)
/* CODE CATEGORY 4 START*/
VSTD_CALLBACK_0 void VSTD_CALLBACK_1 ApplVStdFatalError(vuint8 nErrorNumber);
/* CODE CATEGORY 4 END*/
# endif

#endif
/************   Organi, Version 3.8.0 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.9.0 Vector-Informatik GmbH  ************/
