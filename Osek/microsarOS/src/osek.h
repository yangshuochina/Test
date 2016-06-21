/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osek.h
|
|  General code: @Version: 5.15@
|  Module version: $vv$=2.00
|
|  Description: contains osek prototypes and defines
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* MISRA RULE 19.15 VIOLATION: The QAC-Tool states a violation of rule 19.15 with 
 * message 0883 below. Anyhow, there is a double include prevention and therefore,
 * the message is considered to be wrong. */
/* double include preventer */
#ifndef _OSEK_H      /* PRQA S 0883 EOF */ /* see MISRA comment above */
#define _OSEK_H

/*lint -save Messages inhibited in this file, will be re-enabled at the end of file */
/*lint -e539 Did not expect positive indentation */

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

/* autosar conformant information publishing */
#define OS_VENDOR_ID 30        /*Autosar/HIS vendor ID Vector-Informatik*/
#define OS_MODULE_ID 0x01
#define OS_AR_MAJOR_VERSION 3
#define OS_AR_MINOR_VERSION 1
#define OS_AR_PATCH_VERSION 0
/* KB begin vrmReleaseNumber (overwritten) */
/* Source release number */
#define osdVrmMajRelNum 5
#define osdVrmMinRelNum 5
/* KB end vrmReleaseNumber */
#define OS_SW_MAJOR_VERSION osdVrmMajRelNum
#define OS_SW_MINOR_VERSION osdVrmMinRelNum
/* KB begin AutosarPatchVersion (default) */
#define OS_SW_PATCH_VERSION 0
/* KB end AutosarPatchVersion */

/* defines for all conformance classes */
#define BCC1   1U
#define BCC2   2U
#define ECC1   3U
#define ECC2   4U

/* and for all scalability classes */
#define SC1    1U
#define SC2    2U
#define SC3    3U
#define SC4    4U

#define STANDARD_STATUS 0U
#define EXTENDED_STATUS 1U

/* OS type definitions */
/* KB begin osTypeDefinitions (default) */
typedef unsigned char  osuint8;
typedef unsigned short osuint16;
typedef unsigned long  osuint32;
typedef signed   char  osint8;
typedef signed   short osint16;
typedef signed   long  osint32;
/* KB end osTypeDefinitions */

/* Boolean definition */
/* KB begin osekBooleanDef (overwritten) */
#ifndef osbool
 #define osbool osuint8
#endif
/* KB end osekBooleanDef */

/* Type for boolean arrays */
/* KB begin osekBooleanArrayBaseTypeDef (default) */
typedef osbool osBoolArrayBaseType;
/* KB end osekBooleanArrayBaseTypeDef */

#define osdComStandard 0U
#define osdComExtended 1U
/* Are we in a PRQA/QAC/MISRA-check? */
#ifdef PRQA_PTRDIFF_T
 /* YES: Perform some adaptations to the OS */
 /* First the general adaptations: */
 #pragma PRQA_NO_RETURN osAbortSystem
 #pragma PRQA_NO_RETURN osAssertFailed
 #pragma PRQA_NO_RETURN osSysShutdownOS

 /* Then possible plattform specific adaptations: */
 /* KB begin osekHWosPrqaAdaptation (default) */
 /* no platform specific adaptations for PRQA/QAC */
 /* KB end osekHWosPrqaAdaptation */
#endif

/* Known Compilers */
/* KB begin osekCompilerDefines (overwritten) */

#ifndef __CTC__
#error "osCAN TriCore does not work with Tasking Compiler version less than V2.00"
#endif

#define osdCompilerTaskingTriCore 0
/* KB end osekCompilerDefines */

#ifdef osdIncludeLibConfig

   #define osdPreTCB  1U        
   #define osdPostTCB 0U 

   #if (osdLibVariant == 1U)
    #if defined USE_QUOTE_INCLUDES
     #include "oseklib1.h"
    #else
     #include <oseklib1.h>
    #endif
   #endif
   
   #if (osdLibVariant == 2U)
    #if defined USE_QUOTE_INCLUDES
     #include "oseklib2.h"
    #else
     #include <oseklib2.h>
    #endif
   #endif
   
   #if (osdLibVariant == 3U)
    #if defined USE_QUOTE_INCLUDES
     #include "oseklib3.h"
    #else
     #include <oseklib3.h>
    #endif
   #endif
   
   #if (osdLibVariant == 4U)
    #if defined USE_QUOTE_INCLUDES
     #include "oseklib4.h"
    #else
     #include <oseklib4.h>
    #endif
   #endif
   
   #if ((osdLibVariant < 1) || (osdLibVariant > 4))
   #error unknown library variant/configuration file
   #endif

   #undef osdPreTCB 
   #undef osdPostTCB
   
#else /*osdIncludeLibConfig*/

   #define osdPreTCB  1        
   #define osdPostTCB 0 
   #if defined USE_QUOTE_INCLUDES
    #include "tcb.h"  /* include before */
   #else
    #include <tcb.h>  /* include before */
   #endif
   #undef osdPreTCB 
   #undef osdPostTCB

#endif /*osdIncludeLibConfig*/

#define osdFALSE 0U
#define osdTRUE  1U

#define osdNULL ((osqPROM1 osqPROM2 void osqPROM3 *)0)

/* KB begin osekStartOfHModule (default) */
/* KB end osekStartOfHModule */

/* KB begin osekHwMemoryQualifierDefines (overwritten) */

/* osqROM0 extern osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant;  */
/* osqROM0 osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant; */
#define osqROM0
#define osqROM1
#define osqROM2  
#define osqROM3

/* extern osqPROM1 const osqPROM2 char osqPROM3 * osqROM1 const osqROM2 osqROM3 oskTcbTaskName[osdNumberOfAllTasks]; */
#define osqPROM1
#define osqPROM2
#define osqPROM3

/* osqRAM1 volatile osqRAM2 TickType osqRAM3 osSystemCounter; */
/* osqRAM1 osqRAM2 osDispatcherLockType osqRAM3 osLockDispatcher; */
#define osqRAM1
#define osqRAM2
#define osqRAM3 

/* osqPRAM1 char osqPRAM2 * osqPRAM3 osActiveTaskModule; */
#define osqPRAM1
#define osqPRAM2
#define osqPRAM3 

/* osqBOOL1 volatile osqBOOL2 osbool osqBOOL3 osStartDispatcher; */
#define osqBOOL1 
#define osqBOOL2
#define osqBOOL3 

/* osqTcbRAM1 volatile osqTcbRAM2 TickType osqTcbRAM3 osCounter[5]; */
/* extern osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskHead[osdNumberOfPriorities]; */
#define osqTcbRAM1 
#define osqTcbRAM2
#define osqTcbRAM3 

/* extern (osqPTcbRAM1 osqPTcbRAM2 TaskType osqPTcbRAM3 *) osqROM1 const osqROM2 osqROM3 oskQActivationQueues[osdNumberOfPriorities]; */
#define osqPTcbRAM1 
#define osqPTcbRAM2
#define osqPTcbRAM3 

/* osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack1[80]; */
#define osqStack1
#define osqStack2
#define osqStack3

/* osqFunc1 void osqFunc2 Function1(void); */
#define osqFunc1
#define osqFunc2 

/* qualifiers for inline functions (one qualifier for each function) */
#define osqInlineGetHighPrioBit

/* KB end osekHwMemoryQualifierDefines */

/* include hardware specific header files before OSEK Typedefinitions */
/* KB begin osekHWincludeHwHeaderPreTypes (overwritten) */
/* KB end osekHWincludeHwHeaderPreTypes */

#if defined USE_QUOTE_INCLUDES
 #include "osekcov.h"
 #include "osekasrt.h" /* posix types must be defined      */
 #include "osekerr.h"  /* definitions of all error numbers */
#else
 #include <osekcov.h>
 #include <osekasrt.h> /* posix types must be defined      */
 #include <osekerr.h>  /* definitions of all error numbers */
#endif


/* state of tasks */
#define PRE_READY       0
#define RUNNING         1
#define WAITING         2
#define READY           3
#define SUSPENDED       4
#define INVALID_TASK ((TaskType)(0xFFFFFFFFUL))
/* access rights */
#define NO_ACCESS 0U
#define ACCESS 1U


/* invalid ISR ID */
#define INVALID_ISR  ((ISRType)(0xFFFFFFFFUL))
#define INVALID_SCHEDULETABLE ((ScheduleTableType)(0xFFFFFFFFUL))
#define INVALID_PROCESS (((osProcessType)(0xFFFFFFFFUL)))

/* application error code */
#define INVALID_OSAPPLICATION 0xFFU

/* return codes for protection hook */
#ifndef OS_SUPPRESS_PROTHOOK_OLD_RET_VALS
#define PRO_KILLTASKISR           1U
#define PRO_KILLAPPL              2U
#define PRO_KILLAPPL_RESTART      3U
#endif /*OS_SUPPRESS_PROTHOOK_OLD_RET_VALS*/
#define PRO_TERMINATETASKISR      1U
#define PRO_TERMINATEAPPL         2U
#define PRO_TERMINATEAPPL_RESTART 3U
#define PRO_SHUTDOWN              4U
#define PRO_IGNORE                5U

/* Schedule Table status confirms to AS 3.0 */

#define SCHEDULETABLE_STOPPED                 0U
#define SCHEDULETABLE_NEXT                    1U
#define SCHEDULETABLE_WAITING                 2U
#define SCHEDULETABLE_RUNNING                 3U
#define SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS 4U

/* values for AccessType */
#define oskAccess_None 0x0U
#define oskAccess_Read 0x1U
#define oskAccess_Write 0x2U
#define oskAccess_Execute 0x4U
#define oskAccess_Stack 0x8U

/* macros for AccessType */
#define OSMEMORY_IS_READABLE(x) (((x) & oskAccess_Read)? 1 : 0)
#define OSMEMORY_IS_WRITEABLE(x) (((x) & oskAccess_Write)? 1 : 0)
#define OSMEMORY_IS_EXECUTABLE(x) (((x) & oskAccess_Execute)? 1 : 0)
#define OSMEMORY_IS_STACKSPACE(x) (((x) & oskAccess_Stack)? 1 : 0)

/* values for RestartType */
#define RESTART    1
#define NO_RESTART 2

/* SCHEDULETABLE AUTOSTART TYPE */
#define osdScheduleTableAutostartNO                  0U
#define osdScheduleTableAutostartABSOLUT             1U
#define osdScheduleTableAutostartRELATIVE            2U
#define osdScheduleTableAutostartSYNCHRON            3U

/* ALARM AUTOSTART TYPE */
#define osdAlarmAutostartNO                          0U
#define osdAlarmAutostartABSOLUTE                    1U
#define osdAlarmAutostartRELATIVE                    2U

#define OSINVALIDAPPMODE                             0U

/* KB begin osekHwTASKmacro (overwritten) */
/* OSEK-conform task-function-macro */
#define TASK(x) void x##func(void)
/* KB end osekHwTASKmacro */
/* KB begin osekHWISRmacro (default) */
/* OSEK-conform interrupt-function-macro */
#define ISR(x) void x##func(void)
/* KB end osekHWISRmacro */
/* KB begin osekHWAlrmCallbckmacro (default) */
/* OSEK-conform function-macro for alarm callbacks */
#define ALARMCALLBACK(x) void x(void)
/* KB end osekHWAlrmCallbckmacro */
/* Default application mode is generated (OSEK 2.2) */

#if ((osdSC== SC3) || (osdSC== SC4))
/* context constants */
#define osdCtxTask           ((osuint16)0x0001)
#define osdCtxISRCat2        ((osuint16)0x0002)
#define osdCtxPreTaskHook    ((osuint16)0x0004)
#define osdCtxPostTaskHook   ((osuint16)0x0008)
#define osdCtxErrorHook      ((osuint16)0x0010)
#define osdCtxApplErrorHook  ((osuint16)0x0020)
#define osdCtxEachErrorHook  ((osuint16)0x0030)  /* system and appl spec. ErrorHook */
#define osdCtxStartupHook    ((osuint16)0x0040)
#define osdCtxShutdownHook   ((osuint16)0x0080)
#define osdCtxProtectionHook ((osuint16)0x0100)
/* context constants for COM */
#define osdCtxCOMErrorHook   ((osuint16)0x0200)
#define osdCtxCOMCallback    ((osuint16)0x0400)
#define osdCtxStartComExtens ((osuint16)0x0800)
/* additional flag for ErrorHook called from StartupHook */
#define osdCtxErrorHookFromStartupHook ((osuint16)0x1000)

/* allowed call context for each API function */
#define osdValidCtx_ActivateTask             (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_GetResource              (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_ReleaseResource          (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_SetEvent                 (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_SetRelAlarm              (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_SetAbsAlarm              (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_CancelAlarm              (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_CallTrustedFunction      (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_StartScheduleTableRel    (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_StartScheduleTableAbs    (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_StopScheduleTable        (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_NextScheduleTable        (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_SyncScheduleTable        (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_GetScheduleTableStatus   (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_SetScheduleTableAsync    (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_IncrementCounter         (  osdCtxTask           | osdCtxISRCat2        )
#define osdValidCtx_TerminateTask            (  osdCtxTask                                  )
#define osdValidCtx_ChainTask                (  osdCtxTask                                  )
#define osdValidCtx_Schedule                 (  osdCtxTask                                  )
#define osdValidCtx_ClearEvent               (  osdCtxTask                                  )
#define osdValidCtx_WaitEvent                (  osdCtxTask                                  )
#define osdValidCtx_GetTaskID                (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxPreTaskHook        \
                                              | osdCtxPostTaskHook   | osdCtxProtectionHook     \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_GetTaskState             (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxPreTaskHook        \
                                              | osdCtxPostTaskHook   | osdCtxCOMErrorHook   )
#define osdValidCtx_GetEvent                 (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxPreTaskHook        \
                                              | osdCtxPostTaskHook                          )
#define osdValidCtx_GetAlarmBase             (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxPreTaskHook        \
                                              | osdCtxPostTaskHook                          )
#define osdValidCtx_GetAlarm                 (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxPreTaskHook        \
                                              | osdCtxPostTaskHook                          )   
#define osdValidCtx_GetActiveApplicationMode (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxPreTaskHook        \
                                              | osdCtxPostTaskHook   | osdCtxStartupHook        \
                                              | osdCtxShutdownHook                          )
#define osdValidCtx_ShutdownOS               (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxStartupHook        \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_GetISRID                 (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxProtectionHook     \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_CheckISRMemoryAccess     (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxProtectionHook     \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_CheckTaskMemoryAccess    (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxProtectionHook     \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_CheckObjectAccess        (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxProtectionHook     \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_CheckObjectOwnership     (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxEachErrorHook  | osdCtxProtectionHook     \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_TerminateApplication     (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxApplErrorHook                         )
#define osdValidCtx_StartScheduleTableSynchron (  osdCtxTask           | osdCtxISRCat2      )
/* 
   Remarks:  
   - No checks for interrupt enable/disable functions because these functions
     can be called inside Cat1 ISRs (which can occur inside each context)
   - No context mask for StartOS, because StartOS is only allowed outside
     the defined call contexts
*/
/* allowed call context for each COM-API function */
#define osdValidCtx_StartCOM                 (  osdCtxTask           | osdCtxISRCat2)
#define osdValidCtx_StopCOM                  (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxErrorHook      | osdCtxShutdownHook       \
                                              | osdCtxCOMErrorHook                          )
#define osdValidCtx_SendMessage              (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxCOMCallback                           )
#define osdValidCtx_ReceiveMessage           (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxCOMCallback                           )
#define osdValidCtx_GetMessageStatus         (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxCOMErrorHook   | osdCtxCOMCallback    )
#define osdValidCtx_InitMessage              (  osdCtxTask           | osdCtxISRCat2            \
                                              | osdCtxCOMErrorHook   | osdCtxCOMCallback        \
                                              | osdCtxStartComExtens                        )
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

/* implementation specific constants */
/* KB begin osekConstants (overwritten) */
#define osdPrioBitsPerWord 32
/* KB end osekConstants */


/* typesdefs  */
/* KB begin osekHWosTaskFctType (overwritten) */
typedef void (*osTaskFctType)(void);
/* KB end osekHWosTaskFctType */

typedef osuint16              osStackUsageType;
typedef osuint8               osTaskStateType;
/* KB begin osekHWosStackPtrType (default) */
typedef void *              osStackPtrType;
/* KB end osekHWosStackPtrType */
typedef osuint8               osResCounterType;
typedef osuint32             TickType;
typedef osint32              osTickTimeDiffType;

/* KB begin osDebugLineType (default) */
typedef int osLineNumberType;
/* KB end osDebugLineType */

/* implementation specific types (OSEK types) */
/* KB begin osekTypes (overwritten) */
/*
    !!! Max priorities: Min(2^(sizeof(osPrioType)*8), 2^(sizeof(osPrioOffsetType)*8) * sizeof(osPrioFlagType)*8)
*/
typedef osuint16                 osPrioType;         /* has to be unsigned (invalid is higher than others) */
typedef osuint8                  osPrioOffsetType;
typedef osuint32                 osStdFlagType; /* Standard-Flag-Type e.g. for AutoStartFlagField of alarms */
typedef osStdFlagType            osPrioFlagType;
typedef osStdFlagType            osResAccessMaskType;
typedef osuint8                  osIntLevelType;
typedef osuint8                  osTaskPreemptionType;
typedef osuint8                  osTaskFlagType;
typedef osuint8                  osQEntryCountType;
typedef osuint8                  osActivationCountType;
typedef osuint32                 osStackDataType;  
typedef osuint16                 osStackSizeType;      /* maximum stacksize is 64k */
typedef osuint16                 osTaskIndexType;  
typedef osuint16                 osResourceIndexType;  
typedef osuint16                 osAlarmIndexType; 

/* KB end osekTypes */

/* Autosar types */
/* KB begin atosTypes (overwritten) */
typedef osuint16                 osProcessType;
typedef osuint16                 ISRType;
typedef osuint16                 ScheduleTableType;
typedef osuint8                  ScheduleTableStatusType;
typedef ScheduleTableStatusType* ScheduleTableStatusRefType;
typedef osuint8                  RestartType;
typedef osuint8                  CounterType;
#if ((osdSC== SC2) || (osdSC== SC3) || (osdSC== SC4))
typedef osuint8                  ProtectionReturnType;
#endif
#if ((osdSC== SC3) || (osdSC== SC4))
typedef osuint8                  ApplicationType; 
typedef osuint16                 TrustedFunctionIndexType;
typedef void*                    TrustedFunctionParameterRefType;
typedef void (*TrustedFunctionType)(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams);
typedef osuint8                  AccessType;
typedef osuint32                 ApplAccessMaskType;
typedef osuint8                  ObjectAccessType; 
typedef osuint8                  ObjectTypeType;
typedef osuint16                 ObjectIDType;
typedef osuint32                 MemoryAddressType;
typedef const void*              MemoryStartAddressType;
typedef osuint32                 MemorySizeType;
#endif

typedef osuint32                 PhysicalTimeType;
#if defined _MSC_VER
/* This is included into the generator source */
typedef unsigned __int64       osTPTimeStampType;
#define osdMaxAllowedTimeStamp 0x00FFffffFFFFffff /* 56 Bit Timer */
#else
typedef unsigned long long       osTPTimeStampType;
#define osdMaxAllowedTimeStamp 0x00FFffffFFFFffffULL /* 56 Bit Timer */
#endif 
typedef osuint32                 osTPTimeType;
typedef osTPTimeType*            osTPTimeRefType;
typedef osTPTimeStampType*       osTPTimeStampRefType;
typedef osuint32                 osTPTimeDiffType;
typedef union
{
   osTPTimeType           unsignedNumber;
   osint32                signedNumber;
} osTickTypeTP;


/* constants for simulation of two timers on just one HW-timer */
#define osdTP_NoneIdx   ((osuint8)-1)
#define osdTP_ExeIdx    ((osuint8)0)
#define osdTP_BlockIdx  ((osuint8)1)
#define osdTP_NoneFlag  ((osuint8)0)
#define osdTP_ExeFlag   ((osuint8)(1<<osdTP_ExeIdx))
#define osdTP_BlockFlag ((osuint8)(1<<osdTP_BlockIdx))
/* KB end atosTypes */

#if ((osdSC== SC3) || (osdSC== SC4))
/* possible values of ObjectTypeType */
#define OBJECT_TASK 0
#define OBJECT_ISR 1
#define OBJECT_ALARM 2
#define OBJECT_RESOURCE 3
#define OBJECT_COUNTER 4
#define OBJECT_SCHEDULETABLE 5
#define OBJECT_MESSAGE 6
#define osdNumberOfObjectTypes 7
#endif

/* OSEK types */
/* implementation specific OSEK types */
/* KB begin osekOSEKTypes (overwritten) */
typedef osTaskIndexType      TaskType;

typedef osResourceIndexType  ResourceType;
typedef osAlarmIndexType     AlarmType;

#define osdAutomaticEventMaskType /* enables automatic size calculation */

typedef osuint8              osRTSizeType; /* typedef for library */
/* KB end osekOSEKTypes */
/* KB begin osekDispatcherLockType (default) */
typedef osuint8               osDispatcherLockType;
/* KB end osekDispatcherLockType */

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
typedef osuint8               StatusType;
#define E_OK 0
#endif
typedef osuint8              TaskStateType;
typedef osuint8              AppModeType;
typedef TaskType            *TaskRefType;
typedef TaskStateType       *TaskStateRefType;
typedef TickType            *TickRefType;
typedef EventMaskType       *EventMaskRefType;
typedef ISRType             *ISRRefType;

#if ((osdSC== SC3) || (osdSC== SC4))

/* KB begin osekHWosStartupHookType (default) */
typedef void                (*osStartupHookType)(void);
/* KB end osekHWosStartupHookType */
/* KB begin osekHWosShutdownHookType (default) */
typedef void                (*osShutdownHookType)(StatusType Error);
/* KB end osekHWosShutdownHookType */
/* KB begin osekHWosErrorHookType (default) */
typedef void                (*osErrorHookType)(StatusType Error);
/* KB end osekHWosErrorHookType */

#endif /*((osdSC== SC3) || (osdSC== SC4))*/

/* KB begin osekOSEKdeclarations (default) */
#define DeclareTask(TaskId)     extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed1##TaskId
#define DeclareResource(ResId)  extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed2##ResId
#define DeclareEvent(Event)     extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed3##Event
#define DeclareAlarm(AlarmId)   extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed4##AlarmId
/* KB end osekOSEKdeclarations */    /* DeclareTask, DeclareAlarm,... */

/* structure for AlarmBase */
typedef struct
{
   TickType maxallowedvalue;/* Maximum possible allowed count value (for example */
                            /* in ticks or angle degrees) */
   TickType ticksperbase;   /* Number of ticks required to reach a counter-specific */
                            /* (significant) unit. */
   TickType mincycle;       /* Minimum allowed number of ticks for a cyclic alarm */
                            /* (only for systems with Extended Status). */
} AlarmBaseType;

typedef AlarmBaseType     *AlarmBaseRefType;

typedef AlarmType         osHeapType;

typedef osuint16          OSServiceIdType;
typedef void              (*osAlarmCallbackType)(void);
typedef osuint8           BlockTypeType;


#if defined USE_QUOTE_INCLUDES
 #include "osekcom.h"
#else
 #include <osekcom.h>
#endif

typedef union
{
   TaskType          osTaskType;
   TaskRefType       osTaskRefType;
   ResourceType      osResourceType;
   EventMaskType     osEventMaskType;
   AlarmType         osAlarmType;
   CounterType       osCounterType;
   ScheduleTableType osScheduleTableType;
   ISRType           osISRType;
#if ((osdSC== SC3) || (osdSC== SC4))
   TrustedFunctionIndexType osFunctionIndexType;
   RestartType       osRestartType;
#endif
#if osdComUseParameterAccess
   COMApplicationModeType osCOMApplicationModeType;
   MessageIdentifier      osMessageIdentifier;
#endif
   osuint32          *osTimeRefType;
} osAPIParam1Type;

typedef union
{
   TaskStateRefType           osTaskStateRefType;
   EventMaskType              osEventMaskType;
   EventMaskRefType           osEventMaskRefType;
   AlarmBaseRefType           osAlarmBaseRefType;
   TickType                   osTickType;
   TickRefType                osTickRefType;
   ScheduleTableType          osScheduleTableType;
   ScheduleTableStatusRefType osScheduleTableStatusRefType;
#if ((osdSC== SC3) || (osdSC== SC4))
   TrustedFunctionParameterRefType osTrustedFunctionParameterRefType;
#endif
#if osdComUseParameterAccess
   ApplicationDataRef     osApplicationDataRef;
#endif
   BlockTypeType              osBlockType;
   osuint32                   *osTimeRefType;
   osTPTimeStampRefType       osUnionTPTimeStampRefType;
} osAPIParam2Type;

typedef union
{
   TickType     osTickType;
   ResourceType osResourceType;

} osAPIParam3Type;



/* Schedule Table related */
typedef TickType osSTOffsetType;
typedef TickType GlobalTimeTickType;
typedef osuint16 osSTIndexType;


#if (osdTimingProtectionUsed)

/* possible values for type BlockTypeType */
#define OS_ALL_INTERRUPTS 1
#define OS_OS_INTERRUPTS  2
#define OS_RESOURCE       3

/* Additional resource-IDs for blocking-time measurement*/
#define osdRESID_OS_INTERRUPTS ((osdNumberOfAllResources) + 0)
#define osdRESID_ALL_INTERRUPTS ((osdNumberOfAllResources) + 1)

#endif




#if osdORTIDebug
 #define osdTestMacros 1
#endif

#ifdef osdUseTrace
 #define osdTestMacros 2
#endif

#ifndef osdTestMacros
 #if defined USE_QUOTE_INCLUDES
  #include "emptymac.h"
 #else
  #include <emptymac.h>
 #endif
#else                           
 #if (osdTestMacros == 1)
  #if defined USE_QUOTE_INCLUDES
   #include "testmac1.h"
  #else
   #include <testmac1.h>
  #endif
 #endif
 #if (osdTestMacros == 2)
  #if defined USE_QUOTE_INCLUDES
   #include "testmac2.h"
  #else
   #include <testmac2.h>
  #endif
 #endif
 #if (osdTestMacros == 3)
  #if defined USE_QUOTE_INCLUDES
   #include "testmac3.h"
  #else
   #include <testmac3.h>
  #endif
 #endif
 #if (osdTestMacros == 4)
  #if defined USE_QUOTE_INCLUDES
   #include "testmac4.h"
  #else
   #include <testmac4.h>
  #endif
 #endif
#endif /*osdTestMacros*/


/* KB begin osekHWVariableSectionStartHeader (overwritten) */
/* KB end osekHWVariableSectionStartHeader */

/* KB begin osekCtrlVarDecl (default) */
/* control variables */
extern osqRAM1           osqRAM2  osPrioType     osqRAM3 osActiveTaskPrio;     /* Priority of actually active task */
extern osqRAM1           osqRAM2  TaskType       osqRAM3 osActiveTaskIndex;    /* indicates the currently running task */
extern osqRAM1           osqRAM2  osPrioType     osqRAM3 osHighReadyTaskPrio;  /* Priority of ready task with highest priority */
extern osqRAM1           osqRAM2  TaskType       osqRAM3 osHighReadyTaskIndex; /* ready task with highest priority */
extern osqBOOL1 volatile osqBOOL2 osbool         osqBOOL3 osStartDispatcher;
extern osqRAM1           osqRAM2  osuint8        osqRAM3 osIntNestingDepth;
extern osqRAM1           osqRAM2  osuint8        osqRAM3 osIntSaveDisableCounter;

#if (osdTimingProtectionUsed != 0)
extern osqRAM1           osqRAM2  osIntLevelType osqRAM3 osSavedIntHighLevel;
/* on this platform, osSavedIntHighLevelNested is static */
extern osqRAM1           osqRAM2  osuint8        osqRAM3 osIntSaveDisableHighLevelCounter;
#endif

/* KB end osekCtrlVarDecl */

/* error handling                      */
#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
#ifndef osdSuppressFilenames 
/* variables for error handling */
extern osqRAM1 osqRAM2 osLineNumberType osqRAM3 osActiveTaskLineNumber;

/* MISRA RULE 6.3 VIOLATION: The variable osActiveTaskModule is used to store the name of the 
 * file where the last OS-API-service has been called. This is performed by usage of the 
 * __FILE__ macro. Unfortunately some compilers warn if the type is not a character pointer. 
 * Usage of char* is save here as no computation on the value is intended. 
 */
extern osqPROM1 const osqPROM2 char osqPROM3 * osqPRAM3 osActiveTaskModule; /* PRQA S 5013 */
#endif
#endif

#if osdORTIDebug
extern osqRAM1 volatile osqRAM2 osuint8 osqRAM3 osORTICurrentServiceId;
#endif

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
extern osqRAM1 volatile osqRAM2 osuint8 osqRAM3 osIntAPIStatus;
#endif

/* KB begin osekHWDeclareOsSystemCounterExtern (default) */
#if osdSystemCounterNeeded
#if (osdUseCounterAPI==0)
extern osqRAM1 volatile osqRAM2 TickType osqRAM3 osSystemCounter;
#endif
#endif
/* KB end osekHWDeclareOsSystemCounterExtern */


/* KB begin osekHWDefineNegateTick (default) */
/* KB end osekHWDefineNegateTick */

/* KB begin osekHWDefineDummyRead (overwritten) */
#define osdDummyRead(x)          /* to avoid compiler warning */
/* KB end osekHWDefineDummyRead */

extern osqRAM1 osqRAM2 osuint8 osqRAM3 osActiveAppMode;

#if (osdParameterAccessMacros && osdErrorHook) || (osdComUseParameterAccess && osdComErrorHook)
extern osqRAM1 osqRAM2 osAPIParam1Type osqRAM3 osAPIParam1;
extern osqRAM1 osqRAM2 osAPIParam2Type osqRAM3 osAPIParam2;
extern osqRAM1 osqRAM2 osAPIParam3Type osqRAM3 osAPIParam3;
extern osqPRAM1 osuint32 osqPRAM2 * osqPRAM3 osAPIParam4;
#endif

#if ( (osdErrorHook!=0) || (osdComErrorHook!=0) )
/* KB begin osekExternGlobalErrorVariables (default) */
#if osdORTIDebug 
extern osqRAM1 osqRAM2 StatusType osqRAM3 osLastErrorCode;
#endif
extern osqRAM1 osqRAM2 osuint16 osqRAM3 osLastError;
/* KB end osekExternGlobalErrorVariables */
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
extern osqBOOL1 volatile osqBOOL2 osbool osqBOOL3 osInApplHook;
#endif

/* Id of the currently running (cat2) ISR */
extern osqRAM1 osqRAM2 ISRType osqRAM3 osActiveISRID;

#if (osdUseCOM != 0)
extern osqTcbRAM1 osqTcbRAM2 osqTcbRAM3 COMApplicationModeType osCOMStartupMode;
#endif

/* KB begin osekHWVariableSectionEndHeader (overwritten) */
/* KB end osekHWVariableSectionEndHeader */


osqFunc1 void osqFunc2 osAbortSystem(osuint16 uiError);

/* KB begin osekBNCdispatcherHeader (default) */
#if (osdStackModel == osdSingleStackOptimized) || (osdStackModel == osdSingleStackOptimizedCS)
osqFunc1 void osqFunc2 osBNCDispatch(void);
#endif
/* KB end osekBNCdispatcherHeader */

/* KB begin osekBNCTerminateTask (default) */
#if (osdStackModel == osdSingleStackOptimized) || (osdStackModel == osdSingleStackOptimizedCS)
 #ifndef osdTerminateTaskNotUsed
  #if (STATUS_LEVEL == EXTENDED_STATUS) && (!defined osdSuppressFilenames)
   /* [SPMF0043:0009] */
   #define TerminateTask() do{ osActiveTaskLineNumber = __LINE__; \
                               osActiveTaskModule = __FILE__;     \
                               if(osTerminateTask() == E_OK)      \
                               {                                  \
                                  return;                         \
                               }                                  \
                           }while(osdFALSE)
  #else
   /* [SPMF0043:0009] */
   #define TerminateTask()            \
    do{                               \
       if(osTerminateTask() == E_OK)  \
       {                              \
          return;                     \
       }                              \
    }while(osdFALSE)
  #endif
 #endif

 #ifndef osdChaintTaskNotUsed
  #if (STATUS_LEVEL == EXTENDED_STATUS) && (!defined osdSuppressFilenames)
   /* [SPMF0043:0010] */
   #define ChainTask(x) do{ osActiveTaskLineNumber = __LINE__; \
                            osActiveTaskModule = __FILE__;     \
                            if(osChainTask(x) == E_OK)         \
                            {                                  \
                               return; /* [SPMF0043:0012] */   \
                            }                                  \
                        }while(osdFALSE)
  #else
   /* [SPMF0043:0010] */
   #define ChainTask(x)               \
    do{                               \
       if(osChainTask(x) == E_OK)     \
       {                              \
          return; /*[SPMF0043:0012] */\
       }                              \
    }while(osdFALSE)
  #endif
 #endif

#endif

/* KB end osekBNCTerminateTask */

/* KB begin osekBNCdispatcherStackAccess (default) */
#if (osdStackModel == osdSingleStackOptimized)
 #define osdSingleStackBottom osSystemStack
 #define osdSingleStackTop (osdSystemStackSize + (osuint8*)osSystemStack)
#endif
/* KB end osekBNCdispatcherStackAccess */

/* necessary for task/ISR-kill and to handle a missing TerminateTask in SC3/4 */
#if (osdKillTaskIsrPossible || (osdSC== SC3) || (osdSC== SC4))
osqFunc1 void osqFunc2 osSysKillResourceList(ResourceType ResId);
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
extern osqRAM1 osqRAM2 osuint16 osqRAM3 osCurrentContext;      /* current context */

#if (osdNumberOfApplications > 0)
extern osqRAM1 osqRAM2 ApplicationType osqRAM3 osActiveApplicationID;
#endif
#endif /*((osdSC== SC3) || (osdSC== SC4))*/


#if osdTimingProtectionUsed
/* KB begin osTPTimerPrototypes (default) */
osqFunc1 void osqFunc2 osTPStartTimerBlock(ResourceType res);
osqFunc1 void osqFunc2 osTPStopTimerBlock(ResourceType res);

#if osdTimingMeasurement
 #ifndef osdGetTaskMaxExecutionTimeNotUsed
   osqFunc1 StatusType osqFunc2 osGetTaskMaxExecutionTime(TaskType TaskID,  osTPTimeRefType MaxTime);
 #endif
 #ifndef osdGetISRMaxExecutionTimeNotUsed
   osqFunc1 StatusType osqFunc2 osGetISRMaxExecutionTime(ISRType ISRID, osTPTimeRefType MaxTime);
 #endif
 #ifndef osdGetTaskMinInterArrivalTimeNotUsed
   osqFunc1 StatusType osqFunc2 osGetTaskMinInterArrivalTime(TaskType TaskID, osTPTimeStampRefType MinTime);
 #endif
 #ifndef osdGetISRMinInterArrivalTimeNotUsed
   osqFunc1 StatusType osqFunc2 osGetISRMinInterArrivalTime(ISRType ISRID, osTPTimeStampRefType MinTime);
 #endif
 #ifndef osdGetTaskMaxBlockingTimeNotUsed
   osqFunc1 StatusType osqFunc2 osGetTaskMaxBlockingTime(TaskType TaskID, BlockTypeType BlockType, 
                                                         ResourceType ResourceID, osTPTimeRefType MaxTime);
 #endif
 #ifndef osdGetISRMaxBlockingTimeNotUsed
   osqFunc1 StatusType osqFunc2 osGetISRMaxBlockingTime(ISRType ISRID, BlockTypeType BlockType, 
                                                        ResourceType ResourceID, osTPTimeRefType MaxTime);
 #endif
#endif /* osdTimingMeasurement */
/* KB end osTPTimerPrototypes */
#endif /* osdTimingProtectionUsed */


/* fullfills [SR:001] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* OSEK-Prototypes */
/* system services */
#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdActivateTaskNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ActivateTask(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysActivateTask(x))
#else
#define ActivateTask(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osActivateTask(x))
#endif
#endif

#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)
#ifndef osdTerminateTaskNotUsed
#if ((osdSC == SC3) || (osdSC == SC4))
#define TerminateTask() (osActiveTaskLineNumber = __LINE__,\
   osActiveTaskModule = __FILE__,\
   osPsysTerminateTask())
#else
#define TerminateTask() (osActiveTaskLineNumber = __LINE__,\
   osActiveTaskModule = __FILE__,\
   osTerminateTask())
#endif
#endif

#ifndef osdChainTaskNotUsed
#if ((osdSC == SC3) || (osdSC == SC4))
#define ChainTask(x) (osActiveTaskLineNumber = __LINE__,\
   osActiveTaskModule = __FILE__,\
   osPsysChainTask(x))
#else
#define ChainTask(x) (osActiveTaskLineNumber = __LINE__,\
   osActiveTaskModule = __FILE__,\
   osChainTask(x))
#endif
#endif

#endif

#ifndef osdScheduleNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define Schedule() (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSchedule())
#else
#define Schedule() (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSchedule())
#endif
#endif

#ifndef osdGetTaskIDNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskID(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetTaskID(x))
#else
#define GetTaskID(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskID(x))
#endif
#endif

#ifndef osdGetTaskStateNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskState(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetTaskState((x), (y)))
#else
#define GetTaskState(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskState((x), (y)))
#endif
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdActivateTaskNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ActivateTask(x) (osPsysActivateTask(x))
#else
#define ActivateTask(x) (osActivateTask(x))
#endif
#endif
#if (osdStackModel != osdSingleStackOptimized) && (osdStackModel != osdSingleStackOptimizedCS)
#ifndef osdTerminateTaskNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define TerminateTask() osPsysTerminateTask()
#else
#define TerminateTask() osTerminateTask()
#endif
#endif
#ifndef osdChainTaskNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ChainTask(x) (osPsysChainTask(x))
#else
#define ChainTask(x) (osChainTask(x))
#endif
#endif
#endif
#ifndef osdScheduleNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define Schedule() osPsysSchedule()
#else
#define Schedule() osSchedule()
#endif
#endif
#ifndef osdGetTaskIDNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskID(x) (osPsysGetTaskID(x))
#else
#define GetTaskID(x) (osGetTaskID(x))
#endif
#endif
#ifndef osdGetTaskStateNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskState(x, y) (osPsysGetTaskState((x), (y)))
#else
#define GetTaskState(x, y) (osGetTaskState((x), (y)))
#endif
#endif

#endif /*STATUS_LEVEL...*/

#ifndef osdActivateTaskNotUsed
osqFunc1 StatusType osqFunc2 osActivateTask( TaskType taskIndex);
#endif
#ifndef osdTerminateTaskNotUsed
osqFunc1 StatusType osqFunc2 osTerminateTask ( void );
#endif
#ifndef osdChainTaskNotUsed
osqFunc1 StatusType osqFunc2 osChainTask ( TaskType taskIndex );
#endif
#ifndef osdScheduleNotUsed
osqFunc1 StatusType osqFunc2 osSchedule ( void );              
#endif
#ifndef osdGetTaskIDNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskID ( TaskRefType TaskId); 
#endif
#ifndef osdGetTaskStateNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskState(TaskType TaskId, TaskStateRefType State);
#endif


/* interrupt handling */

osqFunc1 void osqFunc2 osAssertFailed(void);
/* KB begin osekHwOsekGlobalPrototypes (overwritten) */

void osSaveDisableLevelNested(void);
void osRestoreEnableLevelNested(void);


/* workaround for hardware problem CPU_TC.034/COR17 */
/* MISRA RULE 14.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is a inline function. Misra tool cannot deal with this */
static inline void os_dsync(void) /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */
{ 
#if (osdTC_CORE_VERSION < 0x130)
   __asm(
      "dsync;"
      "\n\tisync;/* Infineon recommends an isync after each dsync*/"
   );
#else
   __asm(
      "dsync;"
   );
#endif
}

/* 
 * The intrinsic functions below are specified with int type 
 * for parameters and return values in the compiler manual.
 * As this functions simply work on register values and we would
 * like to see register values as unsigned, we use the macros below
 * to perform the type cast. This way we keep lint happy.
 */
#define osd_mfcr(x) ((osuint32) __mfcr((osint32) (x)))
/* osd_mtcr(x,y) : execute an mtcr instruction properly
   the dsync is needed because: This is a workaround for silicun bugs CPU_TC.105 and CPU_TC.106 
                                Needed on Tasking VX v3.2 since it does only warn, but not work-around
   the isync is needed because: The HW manual states: "An MTCR instruction should be followed by an ISYNC instruction"
*/
#define osd_mtcr(x,y) __asm( "dsync              \n\t" \
                             "mtcr %0,%1         \n\t" \
                             "isync"                   \
                             :                         \
                             : "i" (x), "d" (y)        \
                             :                         \
                           )                           

#define osd_clz(x) ((osuint32) __clz((osint32) (x)))
                               
/* disable all interrupts, no nesting */
#define osDisableGlobal() __disable()



/* The dsync and isync instructions are included in the osd_mtcr macro */
#define osSetLevel(lvl) osd_mtcr(os_ICR_OFFSET,((osd_mfcr(os_ICR_OFFSET) & 0xFFFFFF00UL) | (osuint32) (lvl)))

#if osdTimingProtectionUsed
/* save level and disable up to system level, no nesting */
extern osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntHighLevel;
/* MISRA RULE 14.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 8.1 not violated: This is a inline function. Misra tool cannot deal with this */
/* MISRA RULE 5.5 not violated: This is a inline function. Misra tool cannot deal with this */
static inline void osSaveDisableHighLevel(void) /* PRQA S 3450 */ /* PRQA S 3219 */ /* PRQA S 1527 */                                                
{ 
   osIntLevelType intLevel;                                                  
   osuint32 icr;  
   os_dsync();                                                                 
   icr = osd_mfcr(os_ICR_OFFSET);                                             
   intLevel = (osIntLevelType)(icr & 0x000000ffUL);                                           
   icr = (icr & 0xFFFFFF00UL) | osdHighLevel;  /* PRQA S 3199 */ /* value of icr is used in assembler */
   osd_mtcr(os_ICR_OFFSET,icr);      /* the required isync instruction is executed by osd_mtcr */
   osSavedIntHighLevel = intLevel;                                               
}

/* restore level saved by osSaveDisableHighLevel */
#define osRestoreEnableHighLevel() osSetLevel(osSavedIntHighLevel)
#endif


/* KB end osekHwOsekGlobalPrototypes */ 

#ifndef osdEnableAllInterruptsNotUsed
osqFunc1 void osqFunc2 osEnableAllInterrupts(void);
#endif

#ifndef osdDisableAllInterruptsNotUsed
osqFunc1 void osqFunc2 osDisableAllInterrupts(void);
#endif

#ifndef osdResumeOSInterruptsNotUsed
osqFunc1 void osqFunc2 osResumeOSInterrupts(void);
#endif

#ifndef osdSuspendOSInterruptsNotUsed
osqFunc1 void osqFunc2 osSuspendOSInterrupts(void);
#endif

#ifndef osdResumeAllInterruptsNotUsed
osqFunc1 void osqFunc2 osResumeAllInterrupts(void);
#endif

#ifndef osdSuspendAllInterruptsNotUsed
osqFunc1 void osqFunc2 osSuspendAllInterrupts(void);
#endif


#define osdIntAPIDisableAll 0x80U

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   #define osSetIntAPIStatus(x) (osIntAPIStatus = (x))
   #define osIncIntAPIStatus() (osIntAPIStatus++)
   #define osDecIntAPIStatus() (osIntAPIStatus--)
#else
   #define osSetIntAPIStatus(x)
   #define osIncIntAPIStatus()
   #define osDecIntAPIStatus()
#endif

#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdEnableAllInterruptsNotUsed
   #if ((osdSC == SC2) ||  (osdSC == SC1))
      #define EnableAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osEnableAllInterrupts())
   #else
      #define EnableAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osPsysEnableAllInterrupts())
   #endif
#endif

#ifndef osdDisableAllInterruptsNotUsed
   #if ((osdSC == SC2) || (osdSC == SC1))
      #define DisableAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osDisableAllInterrupts())
   #else
      #define DisableAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osPsysDisableAllInterrupts())
   #endif
#endif

#ifndef osdResumeOSInterruptsNotUsed
   #if ((osdSC== SC2) || (osdSC== SC1))
      #define ResumeOSInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osResumeOSInterrupts())
   #else
      #define ResumeOSInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osPsysResumeOSInterrupts())
   #endif
#endif

#ifndef osdSuspendOSInterruptsNotUsed
   #if ((osdSC == SC2) || (osdSC == SC1))
      #define SuspendOSInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osSuspendOSInterrupts())
   #else
      #define SuspendOSInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osPsysSuspendOSInterrupts())
   #endif
#endif

#ifndef osdResumeAllInterruptsNotUsed
   #if ((osdSC== SC2) || (osdSC== SC1))
      #define ResumeAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osResumeAllInterrupts())
   #else
      #define ResumeAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osPsysResumeAllInterrupts())
   #endif
#endif

#ifndef osdSuspendAllInterruptsNotUsed
   #if ((osdSC == SC2) || (osdSC == SC1))
      #define SuspendAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osSuspendAllInterrupts())
   #else
      #define SuspendAllInterrupts() (osActiveTaskLineNumber = __LINE__, \
         osActiveTaskModule = __FILE__,  \
         osPsysSuspendAllInterrupts())
   #endif
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdEnableAllInterruptsNotUsed
   #if ((osdSC == SC2) ||  (osdSC == SC1))
      #define EnableAllInterrupts() (osEnableAllInterrupts())
   #else
      #define EnableAllInterrupts() (osPsysEnableAllInterrupts())
   #endif
#endif

#ifndef osdDisableAllInterruptsNotUsed
   #if ((osdSC == SC2) || (osdSC == SC1))
      #define DisableAllInterrupts() (osDisableAllInterrupts())
   #else
      #define DisableAllInterrupts() (osPsysDisableAllInterrupts())
   #endif
#endif

#ifndef osdResumeOSInterruptsNotUsed
   #if ((osdSC== SC2) || (osdSC== SC1))
      #define ResumeOSInterrupts() (osResumeOSInterrupts())
   #else
      #define ResumeOSInterrupts() (osPsysResumeOSInterrupts())
   #endif
#endif

#ifndef osdSuspendOSInterruptsNotUsed
   #if ((osdSC == SC2) || (osdSC == SC1))
      #define SuspendOSInterrupts() (osSuspendOSInterrupts())
   #else
      #define SuspendOSInterrupts() (osPsysSuspendOSInterrupts())
   #endif
#endif

#ifndef osdResumeAllInterruptsNotUsed
   #if ((osdSC== SC2) || (osdSC== SC1))
      #define ResumeAllInterrupts() (osResumeAllInterrupts())
   #else
      #define ResumeAllInterrupts() (osPsysResumeAllInterrupts())
   #endif
#endif

#ifndef osdSuspendAllInterruptsNotUsed
   #if ((osdSC == SC2) || (osdSC == SC1))
      #define SuspendAllInterrupts() (osSuspendAllInterrupts())
   #else
      #define SuspendAllInterrupts() (osPsysSuspendAllInterrupts())
   #endif
#endif
#endif /*STATUS_LEVEL...*/


/* resource management */

#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdGetResourceNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetResource(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetResource(x))
#else
#define GetResource(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetResource(x))
#endif
#endif


#ifndef osdReleaseResourceNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ReleaseResource(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysReleaseResource(x))
#else
#define ReleaseResource(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osReleaseResource(x))
#endif
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdGetResourceNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetResource(x) (osPsysGetResource(x))
#else
#define GetResource(x) (osGetResource(x))
#endif
#endif
#ifndef osdReleaseResourceNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ReleaseResource(x) (osPsysReleaseResource(x))
#else
#define ReleaseResource(x) (osReleaseResource(x))
#endif
#endif

#endif /*STATUS_LEVEL...*/


#ifndef osdGetResourceNotUsed
osqFunc1 StatusType osqFunc2 osGetResource(ResourceType ResId);
#endif
#ifndef osdReleaseResourceNotUsed
osqFunc1 StatusType osqFunc2 osReleaseResource(ResourceType ResId); 
#endif

/* KB begin osekSemaphores (default) */
/* KB end osekSemaphores */

/* event control */
#if (CC == ECC1) || (CC == ECC2)

#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdSetEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetEvent(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSetEvent((x), (y)))
#else
#define SetEvent(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetEvent((x), (y)))
#endif
#endif

#ifndef osdGetEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetEvent(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetEvent((x), (y)))
#else
#define GetEvent(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetEvent((x), (y)))
#endif
#endif

#ifndef osdClearEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ClearEvent(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysClearEvent(x))
#else
#define ClearEvent(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osClearEvent(x))
#endif
#endif
   
#ifndef osdWaitEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define WaitEvent(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysWaitEvent(x))
#else
#define WaitEvent(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osWaitEvent(x))
#endif
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdSetEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetEvent(x, y) (osPsysSetEvent((x), (y)))
#else
#define SetEvent(x, y) (osSetEvent((x), (y)))
#endif
#endif

#ifndef osdGetEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetEvent(x, y) (osPsysGetEvent((x), (y)))
#else
#define GetEvent(x, y) (osGetEvent((x), (y)))
#endif
#endif

#ifndef osdClearEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define ClearEvent(x) (osPsysClearEvent(x))
#else
#define ClearEvent(x) (osClearEvent(x))
#endif
#endif

#ifndef osdWaitEventNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define WaitEvent(x) (osPsysWaitEvent(x))
#else
#define WaitEvent(x) (osWaitEvent(x))
#endif
#endif

#endif /*STATUS_LEVEL...*/

#ifndef osdSetEventNotUsed
osqFunc1 StatusType osqFunc2 osSetEvent(TaskType taskIndex, EventMaskType setMask);
#endif
#ifndef osdClearEventNotUsed
osqFunc1 StatusType osqFunc2 osClearEvent (EventMaskType Mask );                  
#endif
#ifndef osdGetEventNotUsed
osqFunc1 StatusType osqFunc2 osGetEvent(TaskType taskIndex, EventMaskRefType Event); 
#endif
#ifndef osdWaitEventNotUsed
osqFunc1 StatusType osqFunc2 osWaitEvent(EventMaskType mask);                    
#endif

#else /*CC...*/
#ifndef osdSetEventNotUsed
#define SetEvent(x, y) (Event API used with wrong conformance class)
#endif
#ifndef osdClearEventNotUsed
#define ClearEvent (x) (Event API used with wrong conformance class)
#endif
#ifndef osdGetEventNotUsed
#define GetEvent(x, y) (Event API used with wrong conformance class)
#endif
#ifndef osdWaitEventNotUsed
#define WaitEvent(x)   (Event API used with wrong conformance class)
#endif

#endif /*CC...*/


/* alarms        */
#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdGetAlarmBaseNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetAlarmBase(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetAlarmBase((x), (y)))
#else
#define GetAlarmBase(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetAlarmBase((x), (y)))
#endif
#endif

#ifndef osdSetRelAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetRelAlarmDefine1 (default) */
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetRelAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSetRelAlarm((x), (y), (z)))
#else
#define SetRelAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetRelAlarm((x), (y), (z)))
#endif
/* KB end osekHwSetRelAlarmDefine1 */
#else
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetRelAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSetRelAlarm(x))
#else
#define SetRelAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetRelAlarm(x))
#endif
#endif
#endif

#ifndef osdSetAbsAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetAbsAlarmDefine1 (default) */
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetAbsAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSetAbsAlarm((x), (y), (z)))
#else
#define SetAbsAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetAbsAlarm((x), (y), (z)))
#endif
/* KB end osekHwSetAbsAlarmDefine1 */
#else
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetAbsAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSetAbsAlarm(x))
#else
#define SetAbsAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetAbsAlarm(x))
#endif
#endif
#endif

#ifndef osdCancelAlarmNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define CancelAlarm(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysCancelAlarm(x))
#else
#define CancelAlarm(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osCancelAlarm(x))
#endif
#endif

#ifndef osdIncrementCounterNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define IncrementCounter(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysIncrementCounter(x))
#else   
#define IncrementCounter(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osIncrementCounter(x))
#endif
#endif

#ifndef osdGetAlarmNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetAlarm(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetAlarm((x), (y)))
#else
#define GetAlarm(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetAlarm((x), (y)))
#endif
#endif

#ifndef osdStartScheduleTableRelNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StartScheduleTableRel(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysStartScheduleTableRel((x), (y)))
#else
#define StartScheduleTableRel(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osStartScheduleTableRel((x), (y)))
#endif   
#endif

#ifndef osdStartScheduleTableAbsNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StartScheduleTableAbs(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysStartScheduleTableAbs((x), (y)))
#else
#define StartScheduleTableAbs(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osStartScheduleTableAbs((x), (y)))
#endif
#endif

#ifndef osdStopScheduleTableNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StopScheduleTable(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysStopScheduleTable((x)))
#else
#define StopScheduleTable(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osStopScheduleTable((x)))
#endif
#endif

#ifndef osdNextScheduleTableNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define NextScheduleTable(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysNextScheduleTable((x), (y)))
#else
#define NextScheduleTable(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osNextScheduleTable((x), (y)))
#endif
#endif

#ifndef osdGetScheduleTableStatusNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetScheduleTableStatus(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetScheduleTableStatus((x), (y)))
#else
#define GetScheduleTableStatus(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetScheduleTableStatus((x), (y)))
#endif
#endif

#if (osdScheduleTableSyncUsed != 0)

#ifndef osdSyncScheduleTableNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define SyncScheduleTable(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSyncScheduleTable((x), (y)))
#else
#define SyncScheduleTable(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSyncScheduleTable((x), (y)))
#endif
#endif

#ifndef osdSetScheduleTableAsyncNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetScheduleTableAsync(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysSetScheduleTableAsync(x))
#else
#define SetScheduleTableAsync(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetScheduleTableAsync(x))
#endif
#endif

#ifndef osdStartScheduleTableSynchronNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StartScheduleTableSynchron(x) (osActiveTaskLineNumber= __LINE__,    \
                                       osActiveTaskModule= __FILE__,        \
                                       osPsysStartScheduleTableSynchron(x))
#else
#define StartScheduleTableSynchron(x) (osActiveTaskLineNumber= __LINE__,\
                                       osActiveTaskModule= __FILE__,    \
                                       osStartScheduleTableSynchron(x))
#endif
#endif

#endif /* #if (osdScheduleTableSyncUsed != 0) */

#if osdTimingProtectionUsed

#ifndef osdGetTaskMaxExecutionTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskMaxExecutionTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetTaskMaxExecutionTime(x,y))
#else
#define GetTaskMaxExecutionTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskMaxExecutionTime(x,y))
#endif
#endif

#ifndef osdGetISRMaxExecutionTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRMaxExecutionTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetISRMaxExecutionTime(x,y))
#else
#define GetISRMaxExecutionTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetISRMaxExecutionTime(x,y))
#endif
#endif

#ifndef osdGetTaskMinInterArrivalTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskMinInterArrivalTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetTaskMinInterArrivalTime(x,y))
#else
#define GetTaskMinInterArrivalTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskMinInterArrivalTime(x,y))
#endif
#endif

#ifndef osdGetISRMinInterArrivalTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRMinInterArrivalTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetISRMinInterArrivalTime(x,y))
#else
#define GetISRMinInterArrivalTime(x,y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetISRMinInterArrivalTime(x,y))
#endif
#endif

#ifndef osdGetTaskMaxBlockingTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskMaxBlockingTime(x,y,z,z1) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetTaskMaxBlockingTime(x,y,z,z1))
#else
#define GetTaskMaxBlockingTime(x,y,z,z1) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskMaxBlockingTime(x,y,z,z1))
#endif
#endif

#ifndef osdGetISRMaxBlockingTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRMaxBlockingTime(x,y,z,z1) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osPsysGetISRMaxBlockingTime(x,y,z,z1))
#else
#define GetISRMaxBlockingTime(x,y,z,z1) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetISRMaxBlockingTime(x,y,z,z1))
#endif
#endif


#endif /*osdTimingProtectionUsed*/

#else /*STATUS_LEVEL...*/

#ifndef osdGetAlarmBaseNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetAlarmBase(x, y) (osPsysGetAlarmBase((x), (y)))
#else
#define GetAlarmBase(x, y) (osGetAlarmBase((x), (y)))
#endif
#endif

#ifndef osdSetRelAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetRelAlarmDefine2 (default) */
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetRelAlarm(x, y, z) (osPsysSetRelAlarm((x), (y), (z)))
#else
#define SetRelAlarm(x, y, z) (osSetRelAlarm((x), (y), (z)))
#endif
/* KB end osekHwSetRelAlarmDefine2 */
#else
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetRelAlarm(x, y, z) (osPsysSetRelAlarm(x))
#else
#define SetRelAlarm(x, y, z) (osSetRelAlarm(x))
#endif
#endif
#endif

#ifndef osdSetAbsAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetAbsAlarmDefine2 (default) */
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetAbsAlarm(x, y, z) (osPsysSetAbsAlarm((x), (y), (z)))
#else
#define SetAbsAlarm(x, y, z) (osSetAbsAlarm((x), (y), (z)))
#endif
/* KB end osekHwSetAbsAlarmDefine2 */
#else
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetAbsAlarm(x, y, z) (osPsysSetAbsAlarm(x))
#else
#define SetAbsAlarm(x, y, z) (osSetAbsAlarm(x))
#endif
#endif
#endif

#ifndef osdCancelAlarmNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define CancelAlarm(x) (osPsysCancelAlarm(x))
#else
#define CancelAlarm(x) (osCancelAlarm(x))
#endif
#endif
   
#ifndef osdIncrementCounterNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define IncrementCounter(x) (osPsysIncrementCounter(x))
#else
#define IncrementCounter(x) (osIncrementCounter(x))
#endif
#endif

#ifndef osdGetAlarmNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetAlarm(x, y) (osPsysGetAlarm((x), (y)))
#else
#define GetAlarm(x, y) (osGetAlarm((x), (y)))
#endif
#endif


#ifndef osdStartScheduleTableRelNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StartScheduleTableRel(x, y) osPsysStartScheduleTableRel((x), (y))
#else
#define StartScheduleTableRel(x, y) osStartScheduleTableRel((x), (y))
#endif
#endif

#ifndef osdStartScheduleTableAbsNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StartScheduleTableAbs(x, y) osPsysStartScheduleTableAbs((x), (y))
#else
#define StartScheduleTableAbs(x, y) osStartScheduleTableAbs((x), (y))
#endif
#endif

#ifndef osdStopScheduleTableNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StopScheduleTable(x) osPsysStopScheduleTable((x))
#else
#define StopScheduleTable(x) osStopScheduleTable((x))
#endif
#endif

#ifndef osdNextScheduleTableNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define NextScheduleTable(x, y) osPsysNextScheduleTable((x), (y))
#else
#define NextScheduleTable(x, y) osNextScheduleTable((x), (y))
#endif
#endif

#ifndef osdGetScheduleTableStatusNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetScheduleTableStatus(x, y) osPsysGetScheduleTableStatus((x), (y))
#else
#define GetScheduleTableStatus(x, y) osGetScheduleTableStatus((x), (y))
#endif
#endif

#if (osdScheduleTableSyncUsed != 0)

#ifndef osdSyncScheduleTableNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define SyncScheduleTable(x, y) osPsysSyncScheduleTable((x), (y))
#else
#define SyncScheduleTable(x, y) osSyncScheduleTable((x), (y))
#endif
#endif

#ifndef osdSetScheduleTableAsyncNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define SetScheduleTableAsync(x) osPsysSetScheduleTableAsync(x)
#else
#define SetScheduleTableAsync(x) osSetScheduleTableAsync(x)
#endif
#endif

#ifndef osdStartScheduleTableSynchronNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define StartScheduleTableSynchron(x) osPsysStartScheduleTableSynchron(x)
#else
#define StartScheduleTableSynchron(x) osStartScheduleTableSynchron(x)
#endif
#endif


#endif /*osdScheduleTableSyncUsed*/


#if osdTimingProtectionUsed

#ifndef osdGetTaskMaxExecutionTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskMaxExecutionTime(x,y) osPsysGetTaskMaxExecutionTime(x,y)
#else
#define GetTaskMaxExecutionTime(x,y) osGetTaskMaxExecutionTime(x,y)
#endif
#endif

#ifndef osdGetISRMaxExecutionTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRMaxExecutionTime(x,y) osPsysGetISRMaxExecutionTime(x,y)
#else
#define GetISRMaxExecutionTime(x,y) osGetISRMaxExecutionTime(x,y)
#endif
#endif

#ifndef osdGetTaskMinInterArrivalTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskMinInterArrivalTime(x,y) osPsysGetTaskMinInterArrivalTime(x,y)
#else
#define GetTaskMinInterArrivalTime(x,y) osGetTaskMinInterArrivalTime(x,y)
#endif
#endif

#ifndef osdGetISRMinInterArrivalTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRMinInterArrivalTime(x,y) osPsysGetISRMinInterArrivalTime(x,y)
#else
#define GetISRMinInterArrivalTime(x,y) osGetISRMinInterArrivalTime(x,y)
#endif
#endif
#ifndef osdGetTaskMaxBlockingTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetTaskMaxBlockingTime(x,y,z,z1) osPsysGetTaskMaxBlockingTime(x,y,z,z1)
#else
#define GetTaskMaxBlockingTime(x,y,z,z1) osGetTaskMaxBlockingTime(x,y,z,z1)
#endif
#endif

#ifndef osdGetISRMaxBlockingTimeNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRMaxBlockingTime(x,y,z,z1) osPsysGetISRMaxBlockingTime(x,y,z,z1)
#else
#define GetISRMaxBlockingTime(x,y,z,z1) osGetISRMaxBlockingTime(x,y,z,z1)
#endif
#endif


#endif /*osdTimingProtectionUsed*/


#endif /*STATUS_LEVEL...*/

#ifndef osdGetAlarmBaseNotUsed
osqFunc1 StatusType osqFunc2 osGetAlarmBase(AlarmType AlarmId, AlarmBaseRefType Info);
#endif
#ifndef osdSetRelAlarmNotUsed
/* KB begin osekHWosSetRelAlarmPrototype (default) */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID, TickType deltaTicks,
               TickType cycle);
#else
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID);
#endif
/* KB end osekHWosSetRelAlarmPrototype */
#endif
#ifndef osdSetAbsAlarmNotUsed
/* KB begin osekHWosSetAbsAlarmPrototype (default) */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID, TickType start,
               TickType cycle);
#else
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID);
#endif
/* KB end osekHWosSetAbsAlarmPrototype */
#endif
#ifndef osdCancelAlarmNotUsed
osqFunc1 StatusType osqFunc2 osCancelAlarm(AlarmType alarmID);
#endif
/* fullfills [SR:315] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
#ifndef osdIncrementCounterNotUsed
osqFunc1 StatusType osqFunc2 osIncrementCounter(CounterType CounterID);
#endif
#ifndef osdGetAlarmNotUsed
osqFunc1 StatusType osqFunc2 osGetAlarm(AlarmType alarmID, TickRefType diffTickTime);
#endif


#ifndef osdStartScheduleTableRelNotUsed
osqFunc1 StatusType osqFunc2 osStartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset);
#endif
#ifndef osdStartScheduleTableAbsNotUsed
osqFunc1 StatusType osqFunc2 osStartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Tickvalue);
#endif

#ifndef osdNextScheduleTableNotUsed
osqFunc1 StatusType osqFunc2 osNextScheduleTable(ScheduleTableType ScheduleTableID_current, ScheduleTableType  ScheduleTableID_next);
#endif
#ifndef osdGetScheduleTableStatusNotUsed
osqFunc1 StatusType osqFunc2 osGetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus);
#endif
#ifndef osdStopScheduleTableNotUsed
osqFunc1 StatusType osqFunc2 osStopScheduleTable(ScheduleTableType ScheduleTableID);
#endif

#if (osdScheduleTableSyncUsed != 0)
#ifndef osdSyncScheduleTableNotUsed
osqFunc1 StatusType osqFunc2 osSyncScheduleTable(ScheduleTableType SchedTableID, GlobalTimeTickType GlobalTime);
#endif
#ifndef osdSetScheduleTableAsyncNotUsed
osqFunc1 StatusType osqFunc2 osSetScheduleTableAsync(ScheduleTableType ScheduleID);
#endif
#ifndef osdStartScheduleTableSynchronNotUsed
osqFunc1 StatusType osqFunc2 osStartScheduleTableSynchron(ScheduleTableType ScheduleTableID);
#endif
#endif /* (osdScheduleTableSyncUsed != 0) */


#if ((osdSC== SC3) || (osdSC== SC4))
#ifndef osdGetApplicationIDNotUsed
#define GetApplicationID() (osPsysGetApplicationID())
osqFunc1 ApplicationType osqFunc2 osGetApplicationID ( void );
#endif

#ifndef osdCheckTaskMemoryAccessNotUsed
#define CheckTaskMemoryAccess(x, y, z) (osPsysCheckTaskMemoryAccess((x), (y), (z)))
osqFunc1 AccessType osqFunc2 osCheckTaskMemoryAccess (TaskType TaskID,
                        MemoryStartAddressType Address, MemorySizeType Size);
#endif

#ifndef osdCheckISRMemoryAccessNotUsed
#define CheckISRMemoryAccess(x, y, z) (osPsysCheckISRMemoryAccess((x), (y), (z)))
osqFunc1 AccessType osqFunc2 osCheckISRMemoryAccess (ISRType ISRID,
                        MemoryStartAddressType Address, MemorySizeType Size);
#endif

#ifndef osdCheckObjectOwnershipNotUsed
#define CheckObjectOwnership(x, y) (osPsysCheckObjectOwnership((x), (y)))
osqFunc1 ApplicationType osqFunc2 osCheckObjectOwnership(ObjectTypeType ObjectType,
                                                         ObjectIDType ObjectID);
#endif

#ifndef osdCheckObjectAccessNotUsed
#define CheckObjectAccess(x, y, z) (osPsysCheckObjectAccess((x), (y), (z)))
osqFunc1 ObjectAccessType osqFunc2 osCheckObjectAccess(ApplicationType   ApplID,
                                                     ObjectTypeType    ObjectType,
                                                     ObjectIDType      ObjectId);
#endif

#ifndef osdCallTrustedFunctionNotUsed
#define CallTrustedFunction(x, y) (osPsysCallTrustedFunction((x), (y)))
osqFunc1 StatusType osqFunc2 osCallTrustedFunction(TrustedFunctionIndexType FunctionIndex,
                                                         TrustedFunctionParameterRefType FunctionParams);
#endif

#ifndef osdTerminateApplicationNotUsed
#if (osdNumberOfApplications > 0)
#define TerminateApplication(x) (osPsysTerminateApplication((x)))
osqFunc1 StatusType osqFunc2 osTerminateApplication(RestartType RestartOption);
#endif /* (osdNumberOfApplications > 0) */
#endif

#endif  /* ((osdSC== SC3) || (osdSC== SC4)) */


/* definitions for static alarms */
/* these macros should be used as parameters for SetRelAlarm/SetAbsAlarm in case of static alarms */
#define OS_STATIC_ALARM_TIME 0
#define OS_STATIC_CYCLE_TIME 0

/* Operating system execution control */
osqFunc1 void osqFunc2 osShutdownOS(StatusType Error);
#if ((osdSC== SC3) || (osdSC== SC4))
#define ShutdownOS(x) (osPsysShutdownOS(x))
#else
#define ShutdownOS(x) (osShutdownOS(x))
#endif

osqFunc1 void osqFunc2 osStartOSasm(void);
osqFunc1 void osqFunc2 osErrorHook(StatusType ErrorCode);

/* Definition of StartOS(). */
/* Error-Checking is done is osStartOSc */
#define StartOS(x)                              \
   osActiveAppMode=(x);                         \
   osStartOSasm()

/* fullfills [SR:006] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
osqFunc1 void osqFunc2 osInitialize(void);

#ifndef osdGetActiveApplModeNotUsed
/* return application mode depending on number of AppModes */
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
 #if ((osdSC== SC3) || (osdSC== SC4))
  #define GetActiveApplicationMode() (((osdValidCtx_GetActiveApplicationMode & osCurrentContext) != 0) ? osActiveAppMode : OSINVALIDAPPMODE)
 #else
  #define GetActiveApplicationMode() (osActiveAppMode)
 #endif
#endif

/* fullfills [SR:166] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
/* fullfills [SR:162] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
#ifndef osdGetISRIDNotUsed
#if ((osdSC== SC3) || (osdSC== SC4))
#define GetISRID() (((osdValidCtx_GetISRID & osCurrentContext) != 0) ? osActiveISRID : INVALID_ISR)
#else
#define GetISRID() (osActiveISRID)
#endif
#endif

/* KB begin osekCallBackSectionStart (default) */
/* KB end osekCallBackSectionStart */

/* hook routines */
osqFunc1 void osqFunc2 ErrorHook (StatusType Error);
osqFunc1 void osqFunc2 StartupHook (void);
osqFunc1 void osqFunc2 ShutdownHook (StatusType Error);

#if osdPreTaskHook
osqFunc1 void osqFunc2 PreTaskHook (void);
#endif
#if osdPostTaskHook
osqFunc1 void osqFunc2 PostTaskHook (void);
#endif

#if ((osdSC == SC2) || (osdSC == SC3) || (osdSC == SC4))
ProtectionReturnType ProtectionHook(StatusType Fatalerror);
#endif

/* KB begin osekCallBackSectionEnd (default) */
/* KB end osekCallBackSectionEnd */

#if defined osdTestMacros || osdLib || (osdSC== SC3) || (osdSC== SC4)
 #if osdPreTaskHook
  osqFunc1 void osqFunc2 osInterPreTaskHook (void);
 #endif
 #if osdPostTaskHook
  osqFunc1 void osqFunc2 osInterPostTaskHook (void);
 #endif
#endif

#if osdStackCheck
 #if osdStackUsageMeasurement
  osqFunc1 osStackUsageType osqFunc2 osGetStackUsage(TaskType taskId);
  /* KB begin osekStackUsagePrototypes (overwritten) */
  /* implementation specific prototypes for stack usage calculation */
  osStackUsageType osGetIntStackUsage(void);
  /* KB end osekStackUsagePrototypes */
 #endif
#endif

/* KB begin osekHWosExternGetSystemCounter (default) */
#if (osdUseCounterAPI!=0)
 #define osdSystemCounterVar osCounter[SystemTimer]
#else
 #define osdSystemCounterVar osSystemCounter
#endif
#if osdUseHighResolutionTimer && (osdAlarmsOnSystemTimer)
 /* osGetSystemCounter() is generated*/
#else
 #define osGetSystemCounter() osdSystemCounterVar
#endif
/* KB end osekHWosExternGetSystemCounter */

#if (defined osdUseGeneratedFastAlarm) || (0==osdAlarmsOnSystemTimer)
#define osdUseHeapAlarm 0
#else
#define osdUseHeapAlarm 1
#endif

#if !defined osdProvideTimeStamp
#define osdProvideTimeStamp 0
#endif

/* structure for version and variant coding */
typedef struct
{
   osuint8 ucMagicNumber1;     /* magic number (e.g. for identification in hex files) */
   osuint8 ucMagicNumber2;     /* defined as osuint8 for independancy of byte order   */
   osuint8 ucMagicNumber3;     /*                                                     */
   osuint8 ucMagicNumber4;     /*                                                     */
   osuint8 ucSysVersionMaj;    /* version of operating system, Major                  */
   osuint8 ucSysVersionMin;    /* version of operating system, Minor                  */
   osuint8 ucGenVersionMaj;    /* version of code generator                           */
   osuint8 ucGenVersionMin;    /* version of code generator                           */
   osuint8 ucSysVariant1;      /* general variant coding 1                            */
   osuint8 ucSysVariant2;      /* general variant coding 2                            */
   /* KB begin osekTypeSpecVersionVariant (overwritten) */
   /* typedef of implementation specific variant coding */
   osuint8 ucSpecVariant1;/* system interrupt level */
   osuint8 ucSpecVariant2;/* bit 0-1 ORTI-variant, bit 2-3 derivative*/
   /* KB end osekTypeSpecVersionVariant */
} osVersionVariantCodingType;

/* KB begin osekConstSectionHeaderStart (default) */
/* KB end osekConstSectionHeaderStart */
osqROM0 extern osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant;
/* KB begin osekConstSectionHeaderEnd (default) */
/* KB end osekConstSectionHeaderEnd */


/* setup defines for filling up the variant coding structure */
#if CC == BCC1
   #define osdVariantCC 0U
#elif CC == BCC2
   #define osdVariantCC 1U
#elif CC == ECC1
   #define osdVariantCC 2U
#elif CC == ECC2
   #define osdVariantCC 3U
#else
#error wrong conformance class
#endif

#if STATUS_LEVEL == EXTENDED_STATUS
   #define osdVariantStatus 1U
#else
   #define osdVariantStatus 0U
#endif

#if osdNonPreempt
   #define osdVariantSchedule 0U
#elif osdFullPreempt
   #define osdVariantSchedule 1U
#elif osdMixedPreempt
   #define osdVariantSchedule 2U
#else
#error wrong scheduling policy
#endif


#if osdStackCheck
   #define osdVariantStackcheck 1U
#else
   #define osdVariantStackcheck 0U
#endif


#ifndef osdSuppressFilenames
   #define osdVariantModulnamesEnabled 1U
#else
   #define osdVariantModulnamesEnabled 0U
#endif

#if osdEnableAssertions
   #define osdVariantAssertionsEnabled 1U
#else
   #define osdVariantAssertionsEnabled 0U
#endif

#if (osdSC== SC1)
   #define osdVariantSC 0U
#elif (osdSC== SC2)
   #define osdVariantSC 1U
#elif (osdSC== SC3)
   #define osdVariantSC 2U
#elif (osdSC== SC4)
   #define osdVariantSC 3U
#else
#error wrong scalability class
#endif

#if !osdLib
#if (osdNumberOfScheduleTables>0)
   #define osdVariantFeaturesUsed_ST 1U
#else 
   #define osdVariantFeaturesUsed_ST 0U
#endif
#if (osdNumberOfHiResSchedTabs>0)
   #define osdVariantFeaturesUsed_HRST 1U
#else 
   #define osdVariantFeaturesUsed_HRST 0U
#endif
#else
   #define osdVariantFeaturesUsed_ST 0U
   #define osdVariantFeaturesUsed_HRST 0U
#endif
#if (osdScheduleTableSyncUsed)
   #define osdVariantFeaturesUsed_Sync 1U
#else 
   #define osdVariantFeaturesUsed_Sync 0U
#endif
#if (osdTimingProtectionUsed)
   #define osdVariantFeaturesUsed_TP   1U
#else 
   #define osdVariantFeaturesUsed_TP   0U
#endif


/* KB begin osekDefineSpecVersionVariant (overwritten) */
#if osdTC1775
   #define osdVariantDerivative 0
#elif osdTC1765
   #define osdVariantDerivative 1
#elif osdTC1796
   #define osdVariantDerivative 2
#elif osdTC1766
   #define osdVariantDerivative 3
#elif osdTC1797
   #define osdVariantDerivative 4
#elif osdTC1767
   #define osdVariantDerivative 5
#else
   #error Unknown derivative specified
#endif

#define osdVariantSystemIntLevel osdSystemLevel

#if osdORTIEnabled
#if (osdORTIVersion==200)
   #define osdVariantORTI 1     /* ORTI 2.0 */
#else
#if osdORTIDebug
   #define osdVariantORTI 3     /* ORTI 2.1, Additional */
#else
   #define osdVariantORTI 2     /* ORTI 2.1, Standard */
#endif  /* osdORTIDebug */
#endif  /* (osdORTIVersion==200) */
#else
   #define osdVariantORTI 0
#endif   /* osdORTIEnabled */
/* KB end osekDefineSpecVersionVariant */


#ifdef osdIncludeLibConfig

 /* include now tcb.h, but only for the application code
  * include both parts of the tcb, pre and post
  */

 #define osdPreTCB  0
 #define osdPostTCB 1

 #if (osdLibVariant == 1)
  #if defined USE_QUOTE_INCLUDES
   #include "oseklib1.h"
  #else
   #include <oseklib1.h>
  #endif
 #endif
   
 #if (osdLibVariant == 2)
  #if defined USE_QUOTE_INCLUDES
   #include "oseklib2.h"
  #else
   #include <oseklib2.h>
  #endif
 #endif
   
 #if (osdLibVariant == 3)
  #if defined USE_QUOTE_INCLUDES
   #include "oseklib3.h"
  #else
   #include <oseklib3.h>
  #endif
 #endif
   
 #if (osdLibVariant == 4)
  #if defined USE_QUOTE_INCLUDES
   #include "oseklib4.h"
  #else
   #include <oseklib4.h>
  #endif
 #endif

 #undef osdPreTCB 
 #undef osdPostTCB
      

#else

 #define osdPreTCB  0
 #define osdPostTCB 1
 #if defined USE_QUOTE_INCLUDES
  #include "tcb.h"
 #else
  #include <tcb.h>
 #endif
 #undef osdPreTCB 
 #undef osdPostTCB

#endif

/* include hardware specific header files after OSEK Typedefinitions */
/* KB begin osekHWincludeHwHeaderPostTypes (overwritten) */
/* KB end osekHWincludeHwHeaderPostTypes */

/* KB begin osekEndOfHModule (default) */
/* KB end osekEndOfHModule */

/*lint -restore re-enable messages*/

#endif/* double include preventer */

/* END OF HEADER osek.h */

