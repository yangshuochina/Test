/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekerr.h
|
|  General code: @Version: 5.15@
|  Module version: $vv$=1.43
|
|  Description: definitions of all errors for assertions and OSError
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEKERR_H
#define _OSEKERR_H

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

/* All errors of API services (inherited from OSEK spec) */
/*      E_OK            0  now defined in osek.h */
#define E_OS_ACCESS     1
#define E_OS_CALLEVEL   2
#define E_OS_ID         3
#define E_OS_LIMIT      4
#define E_OS_NOFUNC     5
#define E_OS_RESOURCE   6
#define E_OS_STATE      7
#define E_OS_VALUE      8

/* errors of API services (Autosar specification v3.0) */
/* fullfills [SR:311] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
#define E_OS_SERVICEID            9
#define E_OS_ILLEGAL_ADDRESS      10
#define E_OS_MISSINGEND           11
#define E_OS_DISABLEDINT          12
#define E_OS_STACKFAULT           13
#define E_OS_PROTECTION_MEMORY    14
#define E_OS_PROTECTION_TIME      15
#define E_OS_PROTECTION_ARRIVAL   16
#define E_OS_PROTECTION_LOCKED    17
#define E_OS_PROTECTION_EXCEPTION 18


#define E_OS_SYS_ASSERTION        20
#define E_OS_SYS_ABORT            21
/*E_OS_SYS_DIS_INT replaced by E_OS_DISABLEDINT, no No. 22*/
#define E_OS_SYS_API_ERROR        23
#define E_OS_SYS_ALARM_MANAGEMENT 24
#define E_OS_SYS_WARNING          25


/*
 * Syntax of errornumbers:
 * 0xgfee
 *   ||+--- consecutive error number
 *   |+---- number of function in the function group
 *   +----- number of function group
 */

/* Group: Task management 1 */
/*    function ActivateTask:           AT: 1 */
/*    function TerminateTask:          TT: 2 */
/*    function ChainTask:              HT: 3 */
/*    function Schedule:               SH: 4 */
/*    function GetTaskState:           GS: 5 */
/*    function GetTaskID:              GI: 6 */
/*    function osMissingTerminateError MT: 7 */

#define osdErrATWrongTaskID           0x1101U
#define osdErrATWrongTaskPrio         0x1102U
#define osdErrATMultipleActivation    0x1103U
#define osdErrATIntAPIDisabled        0x1104U
#define osdErrATAlarmMultipleActivation 0x1105U
#define osdErrATNoAccess              0x1106U
#define osdErrATCallContext           0x1107U

#define osdErrTTDisabledInterrupts    0x1201U
#define osdErrTTResourcesOccupied     0x1202U
#define osdErrTTNotActivated          0x1203U
#define osdErrTTOnInterruptLevel      0x1204U
#define osdErrTTNoImmediateTaskSwitch 0x1205U
#define osdErrTTCallContext           0x1206U

#define osdErrHTInterruptsDisabled    0x1301U
#define osdErrHTResourcesOccupied     0x1302U
#define osdErrHTWrongTaskID           0x1303U
#define osdErrHTNotActivated          0x1304U
#define osdErrHTMultipleActivation    0x1305U
#define osdErrHTOnInterruptLevel      0x1306U
#define osdErrHTWrongTaskPrio         0x1307U
#define osdErrHTNoImmediateTaskSwitch 0x1308U
#define osdErrHTCallContext           0x1309U
#define osdErrHTNoAccess              0x130AU

#define osdErrSHInterruptsDisabled    0x1401U
#define osdErrSHOnInterruptLevel      0x1402U
#define osdErrSHScheduleNotAllowed    0x1403U
#define osdErrSHResourcesOccupied     0x1405U
#define osdErrSHCallContext           0x1406U

#define osdErrGSWrongTaskID           0x1501U
#define osdErrGSIntAPIDisabled        0x1502U
#define osdErrGSIllegalAddr           0x1503U
#define osdErrGSCallContext           0x1504U
#define osdErrGSNoAccess              0x1505U

#define osdErrGIIntAPIDisabled        0x1601U
#define osdErrGIIllegalAddr           0x1602U
#define osdErrGICallContext           0x1603U

#define osdErrMTMissingTerminateTask  0x1701U


/* Group: Interrupt handling 2 */
/*    function EnableAllInterrupts         EA: 4 */
/*    function DisableAllInterrupts        DA: 5 */
/*    function ResumeOSInterrupts          RI: 6 */
/*    function SuspendOSInterrupts         SI: 7 */
/*    function osUnhandledException        UE: 8 */
/*    function osSaveDisableLevelNested    SD: 9 */
/*    function osRestoreEnableLevelNested  RE: A */
/*    function osSaveDisableGlobalNested   SG: B */
/*    function osRestoreEnableGlobalNested RG: C */
/*    function ResumeAllInterrupts         RA: D */
/*    function SuspendAllInterrupts        SA: E */

#define osdErrEAIntAPIWrongSequence   0x2401U
#define osdErrDAIntAPIDisabled        0x2501U

#define osdErrUEUnhandledException    0x2801U

#define osdErrSDWrongCounter          0x2901U
#define osdErrREWrongCounter          0x2A01U

#define osdErrSGWrongCounter          0x2B01U
#define osdErrRGWrongCounter          0x2C01U

/* Group: Resource management 3 */
/*    function GetResource            GR:  1 */
/*    function ReleaseResource        RR:  2 */

#define osdErrGRWrongResourceID       0x3101U
#define osdErrGRPriorityOccupied      0x3102U
#define osdErrGRResourceOccupied      0x3103U
#define osdErrGRNoAccessRights        0x3104U
#define osdErrGRWrongPrio             0x3105U
#define osdErrGRIntAPIDisabled        0x3106U
#define osdErrGRNoAccess              0x3107U
#define osdErrGRCallContext           0x3108U
#define osdErrGRISRNoAccessRights     0x3109U

#define osdErrRRWrongResourceID       0x3201U
#define osdErrRRCeilingPriorityNotSet 0x3202U
#define osdErrRRWrongTask             0x3203U
#define osdErrRRWrongPrio             0x3204U
#define osdErrRRNotOccupied           0x3206U
#define osdErrRRWrongSequence         0x3207U
#define osdErrRRIntAPIDisabled        0x3208U
#define osdErrRRNoAccess              0x3209U
#define osdErrRRCallContext           0x320AU
#define osdErrRRISRNoAccessRights     0x320BU
                                        

/* Group: Event control 4 */
/*    function SetEvent               SE:  1 */
/*    function ClearEvent             CE:  2 */
/*    function GetEvent               GE:  3 */
/*    function WaitEvent              WE:  4 */

#define osdErrSEWrongTaskID           0x4101U
#define osdErrSENotExtendedTask       0x4102U
#define osdErrSETaskSuspended         0x4103U
#define osdErrSEWrongTaskPrio         0x4104U
#define osdErrSEIntAPIDisabled        0x4105U
#define osdErrSECallContext           0x4106U
#define osdErrSENoAccess              0x4107U

#define osdErrCENotExtendedTask       0x4201U
#define osdErrCEOnInterruptLevel      0x4202U
#define osdErrCEIntAPIDisabled        0x4203U
#define osdErrCECallContext           0x4204U

#define osdErrGEWrongTaskID           0x4301U
#define osdErrGENotExtendedTask       0x4302U
#define osdErrGETaskSuspended         0x4303U
#define osdErrGEIntAPIDisabled        0x4304U
#define osdErrGEIllegalAddr           0x4305U
#define osdErrGECallContext           0x4306U
#define osdErrGENoAccess              0x4307U

#define osdErrWENotExtendedTask       0x4401U
#define osdErrWEResourcesOccupied     0x4402U
#define osdErrWEInterruptsDisabled    0x4403U
#define osdErrWEOnInterruptLevel      0x4404U
#define osdErrWECallContext           0x4405U

/* Group: Alarms 5 */
/*    function GetAlarmBase           GB:  1 */
/*    function GetAlarm               GA:  2 */
/*    function SetRelAlarm            SA:  3 */
/*    function SetAbsAlarm            SL:  4 */
/*    function CancelAlarm            CA:  5 */
/*    function osWorkAlarms           WA:  6 */

#define osdErrGBWrongAlarmID          0x5101U
#define osdErrGBIntAPIDisabled        0x5102U
#define osdErrGBIllegalAddr           0x5103U
#define osdErrGBCallContext           0x5104U
#define osdErrGBNoAccess              0x5105U

#define osdErrGAWrongAlarmID          0x5201U
#define osdErrGANotActive             0x5202U
#define osdErrGAIntAPIDisabled        0x5203U
#define osdErrGAIllegalAddr           0x5204U
#define osdErrGACallContext           0x5205U
#define osdErrGANoAccess              0x5206U

#define osdErrSAWrongAlarmID          0x5301U
#define osdErrSAAlreadyActive         0x5302U
#define osdErrSAWrongCycle            0x5303U
#define osdErrSAWrongDelta            0x5304U
#define osdErrSAIntAPIDisabled        0x5305U
#define osdErrSAZeroIncrement         0x5306U
#define osdErrSACallContext           0x5307U
#define osdErrSANoAccess              0x5308U

#define osdErrSLWrongAlarmID          0x5401U
#define osdErrSLAlreadyActive         0x5402U
#define osdErrSLWrongCycle            0x5403U
#define osdErrSLWrongStart            0x5404U
#define osdErrSLIntAPIDisabled        0x5405U
#define osdErrSLCallContext           0x5406U
#define osdErrSLNoAccess              0x5407U

#define osdErrCAWrongAlarmID          0x5501U
#define osdErrCANotActive             0x5502U
#define osdErrCAIntAPIDisabled        0x5503U
#define osdErrCAAlarmInternal         0x5504U
#define osdErrCACallContext           0x5505U
#define osdErrCANoAccess              0x5506U


/* Group: Operating system execution control 6 */
/*    function osCheckStackOverflow   SO:  1 */
/*    function osSchedulePrio         SP:  2 */
/*    function osGetStackUsage        SU:  3 */
/*    function osCheckLibraryVers..   CL:  4 */
/*    function osErrorHook            EH:  5 */
/*    function StartOS                ST:  6 */

#define osdErrSOStackOverflow         0x6101U

#define osdErrSPInterruptsEnabled     0x6201U

#define osdErrSUWrongTaskID           0x6301U

#define osdErrCLWrongLibrary          0x6401U

#define osdErrEHInterruptsEnabled     0x6501U

#define osdErrSTMemoryError           0x6601U
#define osdErrSTNoImmediateTaskSwitch 0x6602U
#define osdErrSTWrongAppMode          0x6603U
#define osdErrSTMultipleHRSTAutostart 0x6604U

/* Group: Schedule Table control 7 */
/*    function StartScheduleTableRel  SR:  1 */
/*    function StartScheduleTableAbs  SS:  2 */
/*    function StopScheduleTable      SP:  3 */
/*    function GetScheduleTableStatus SG:  4 */
/*    function NextScheduleTable      SN:  5 */
/*    function osWorkScheduleTables   WS:  6 */


#define osdErrSRWrongID                0x7101U
#define osdErrSRAlreadyRunningOrNext   0x7102U
#define osdErrSRZeroOffset             0x7103U
#define osdErrSROffsetTooBig           0x7104U
#define osdErrSRIntAPIDisabled         0x7105U
#define osdErrSRCallContext            0x7106U
#define osdErrSRNoAccess               0x7107U
#define osdErrSRTooManyActiveHiResSchT 0x7108U
#define osdErrSRImpliciteSync          0x7109U

#define osdErrSSWrongID                0x7201U
#define osdErrSSAlreadyRunningOrNext   0x7202U
#define osdErrSSTickvalueTooBig        0x7203U
#define osdErrSSIntAPIDisabled         0x7204U
#define osdErrSSCallContext            0x7205U
#define osdErrSSNoAccess               0x7206U
#define osdErrSSTooManyActiveHiResSchT 0x7207U


#define osdErrSPWrongID               0x7301U
#define osdErrSPNotRunning            0x7302U
#define osdErrSPIntAPIDisabled        0x7303U
#define osdErrSPCallContext           0x7304U
#define osdErrSPNoAccess              0x7305U

#define osdErrSGWrongID               0x7401U
#define osdErrSGIntAPIDisabled        0x7402U
#define osdErrSGCallContext           0x7403U
#define osdErrSGNoAccess              0x7404U
#define osdErrSGIllegalAddr           0x7405U

#define osdErrSNWrongCurrentID        0x7501U
#define osdErrSNWrongNextID           0x7502U
#define osdErrSNNotRunning            0x7503U
#define osdErrSNAlreadyRunningOrNext  0x7504U
#define osdErrSNDifferentCounters     0x7505U
#define osdErrSNIntAPIDisabled        0x7506U
#define osdErrSNCallContext           0x7507U
#define osdErrSNNoAccess              0x7508U

#define osdErrWSUnknownAction         0x7601U
#define osdErrWSUnknownReaction       0x7602U

/* Group: other SC1 functions 8 */
/*    function IncrementCounter     IC:  1 */
/*    function GetISRID             II:  2 */

#define osdErrICWrongCounterID        0x8101U
#define osdErrICIntAPIDisabled        0x8102U
#define osdErrICCallContext           0x8103U
#define osdErrICNoAccess              0x8104U

#define osdErrIIIntAPIDisabled        0x8201U
#define osdErrIICallContext           0x8202U

/* Group: other SC2, SC3, SC4 functions 9 */
/*    function CallTrustedFunction        CT:  3 */
/*    function TerminateApplication       TA:  4 */
/*    function SyncScheduleTable          SY:  5 */
/*    function SetScheduleTableAsync      AY:  6 */
/*    function BlockingTimeMonitoring     BM:  7 */
/*    function GetTaskMaxExecutionTime    TE:  8 */
/*    function GetISRMaxExecutionTime     IE:  9 */
/*    function GetTaskMaxBlockingTime     TB:  A */
/*    function GetISRMaxBlockingTime      IB:  B */
/*    function StartScheduleTableSynchron TS:  C */
/*    function ExecutionTimeMonitoring    ET:  D */
/*    function ISR exit                   IX:  E */
/*    function GetTaskMinInterArrivalTime    TM:  0 */
/*    function GetISRMinInterArrivalTime     MI:  F */



#define osdErrCTWrongFctIdx           0x9301U
#define osdErrCTCallContext           0x9302U
#define osdErrCTIntAPIDisabled        0x9303U

#define osdErrTAWrongRestartOption    0x9401U
#define osdErrTACallContext           0x9402U
#define osdErrTAIntAPIDisabled        0x9403U

#define osdErrSYCallContext           0x9501U
#define osdErrSYWrongID               0x9502U
#define osdErrSYNoAccess              0x9503U
#define osdErrSYIntAPIDisabled        0x9504U
#define osdErrSYSTNotRunning          0x9505U
#define osdErrSYGlobalTimeTooBig      0x9506U
#define osdErrSYSyncKindNotExplicit   0x9507U

#define osdErrAYCallContext           0x9601U
#define osdErrAYWrongID               0x9602U
#define osdErrAYNoAccess              0x9603U
#define osdErrAYIntAPIDisabled        0x9604U

#define osdErrBMResAlreadyMeasured    0x9702U
#define osdErrBMInvalidProcessInStart 0x9703U
#define osdErrBMInvalidProcessInStop  0x9704U

#define osdErrTEWrongTaskID           0x9801U
#define osdErrTENoAccess              0x9802U
#define osdErrTEIllegalAddr           0x9803U

#define osdErrIEWrongISRID            0x9901U
#define osdErrIENoAccess              0x9902U
#define osdErrIEIllegalAddr           0x9903U

#define osdErrTBWrongTaskID           0x9A01U
#define osdErrTBWrongBlockType        0x9A02U
#define osdErrTBWrongResourceID       0x9A03U
#define osdErrTBNoAccessToTask        0x9A04U
#define osdErrTBNoAccessToResource    0x9A05U
#define osdErrTBIllegalAddr           0x9A06U

#define osdErrIBWrongISRID            0x9B01U
#define osdErrIBWrongBlockType        0x9B02U
#define osdErrIBWrongResourceID       0x9B03U
#define osdErrIBNoAccessToISR         0x9B04U
#define osdErrIBNoAccessToResource    0x9B05U
#define osdErrIBIllegalAddr           0x9B06U

#define osdErrTSCallContext            0x9C01U
#define osdErrTSWrongID                0x9C02U
#define osdErrTSNoAccess               0x9C03U
#define osdErrTSIntAPIDisabled         0x9C04U
#define osdErrTSSTAlreadyRunning       0x9C05U
#define osdErrTSGlobalTimeTooBig       0x9C06U
#define osdErrTSTooManyActiveHiResSchT 0x9C07U
#define osdErrTSSyncKindNotExplicit    0x9C08U

#define osdErrETNoCurrentProcess       0x9D01U

#define osdErrIXResourcesOccupied      0x9E01U
#define osdErrIXIntAPIDisabled         0x9E02U

#define osdErrTMWrongTaskID            0x9001U
#define osdErrTMNoAccess              0x9002U
#define osdErrTMIllegalAddr           0x9003U

#define osdErrMIWrongISRID            0x9F01U
#define osdErrMINoAccess              0x9F02U
#define osdErrMIIllegalAddr           0x9F03U

/* Group: Platform specific error codes A */
/* KB begin osekHwErrorCodes (overwritten) */
/*    function osIntStackOverflow      YO:  1 */
/*    function osTimerInterrupt        TI:  2 */
/*    function BlockingTimeMeaurement  BM:  3 */
/*    function osDispatcher            DI:  4 */
/*    function cat2 interrupt wrapper  IW:  5 */
/*    function StartOS                 ST:  6 */
/*    function SystemCallTrap (trap 6) T6:  7 */
#define osdErrYOStackOverflow         0xA101U
#define osdErrTITickTime              0xA201U
#define osdErrBMTimeTooBigToMeasure   0xA301U
#define osdErrDIWrongStackPointer     0xA401U
#define osdErrIWWrongStackPointer     0xA501U
#define osdErrSTDebuggerUsesCPR0_0    0xA601U
#define osdErrSTDebuggerUsesCPR0_1    0xA602U
#define osdErrSTDebuggerUsesCPR1_0    0xA603U
#define osdErrSTDebuggerUsesCPR1_1    0xA604U
#define osdErrT6WrongTrapIdNumber     0xA701U
/* KB end osekHwErrorCodes */

/* Group: messages B */
/*    function StartCOM               SC:  1 */
/*    function StopCOM                TC:  2 */
/*    function SendMessage            SM:  3 */
/*    function ReceiveMessage         RM:  4 */
/*    function GetMessageStatus       MS:  5 */
/*    function GetCOMApplicationMode  AM:  6 */
/*    function InitMessage            IM:  7 */
/*    function COMErrorHook           CR:  8 */

#define osdErrSCWrongModeID           0xB101U
#define osdErrSCCallContext           0xB102U

#define osdErrTCWrongModeID           0xB201U
#define osdErrTCCallContext           0xB202U

#define osdErrSMWrongID               0xB301U
#define osdErrSMNoAccess              0xB302U
#define osdErrSMCallContext           0xB303U

#define osdErrRMWrongID               0xB401U
#define osdErrRMLimit                 0xB402U
#define osdErrRMNoMessage             0xB403U
#define osdErrRMNoAccess              0xB404U
#define osdErrRMIllegalAddr           0xB405U
#define osdErrRMCallContext           0xB406U

#define osdErrMSWrongID               0xB501U
#define osdErrMSLimit                 0xB502U
#define osdErrMSNoMessage             0xB503U
#define osdErrMSNoAccess              0xB504U
#define osdErrMSCallContext           0xB505U

#define osdErrIMWrongID               0xB701U
#define osdErrIMNoAccess              0xB702U
#define osdErrIMCallContext           0xB703U

#define osdErrCRInterruptsEnabled     0xB801U


/* definitions to save the parameters in global variables before calling the service */
#if osdParameterAccessMacros && osdErrorHook
   #define osSaveActivateTask_TaskID(x)                      (osAPIParam1.osTaskType = (x));
   #define osSaveChainTask_TaskID(x)                         (osAPIParam1.osTaskType = (x));
   #define osSaveGetTaskID_TaskID(x)                         (osAPIParam1.osTaskRefType = (x));
   #define osSaveGetTaskState_TaskID(x)                      (osAPIParam1.osTaskType = (x));
   #define osSaveGetTaskState_State(y)                       (osAPIParam2.osTaskStateRefType = (y));
   #define osSaveGetResource_ResID(x)                        (osAPIParam1.osResourceType = (x));
   #define osSaveReleaseResource_ResID(x)                    (osAPIParam1.osResourceType = (x));
   #define osSaveSetEvent_TaskID(x)                          (osAPIParam1.osTaskType = (x));
   #define osSaveSetEvent_Mask(y)                            (osAPIParam2.osEventMaskType = (y));
   #define osSaveGetEvent_TaskID(x)                          (osAPIParam1.osTaskType = (x));
   #define osSaveGetEvent_Event(y)                           (osAPIParam2.osEventMaskRefType = (y));
   #define osSaveClearEvent_Mask(x)                          (osAPIParam1.osEventMaskType = (x));
   #define osSaveWaitEvent_Mask(x)                           (osAPIParam1.osEventMaskType = (x));
   #define osSaveGetAlarmBase_AlarmID(x)                     (osAPIParam1.osAlarmType = (x));
   #define osSaveGetAlarmBase_Info(y)                        (osAPIParam2.osAlarmBaseRefType = (y));
   #define osSaveSetRelAlarm_AlarmID(x)                      (osAPIParam1.osAlarmType = (x)) ;
   #define osSaveSetRelAlarm_increment(y)                    (osAPIParam2.osTickType = (y));
   #define osSaveSetRelAlarm_cycle(z)                        (osAPIParam3.osTickType = (z));
   #define osSaveSetAbsAlarm_AlarmID(x)                      (osAPIParam1.osAlarmType = (x));
   #define osSaveSetAbsAlarm_start(y)                        (osAPIParam2.osTickType = (y));
   #define osSaveSetAbsAlarm_cycle(z)                        (osAPIParam3.osTickType = (z));
   #define osSaveCancelAlarm_AlarmID(x)                      (osAPIParam1.osAlarmType = (x));
   #define osSaveGetAlarm_AlarmID(x)                         (osAPIParam1.osAlarmType = (x));
   #define osSaveGetAlarm_Tick(y)                            (osAPIParam2.osTickRefType = (y));
   #define osSaveIncrementCounter_CounterID(x)               (osAPIParam1.osCounterType = (x));
   #define osSaveStartScheduleTableRel_ScheduleTableID(x)    (osAPIParam1.osScheduleTableType = (x));
   #define osSaveStartScheduleTableRel_Offset(y)             (osAPIParam2.osTickType = (y));
   #define osSaveStartScheduleTableAbs_ScheduleTableID(x)    (osAPIParam1.osScheduleTableType = (x));
   #define osSaveStartScheduleTableAbs_Tickvalue(y)          (osAPIParam2.osTickType = (y));
   #define osSaveStopScheduleTable_ScheduleTableID(x)        (osAPIParam1.osScheduleTableType = (x));
   #define osSaveNextScheduleTable_ScheduleTableID_current(x)(osAPIParam1.osScheduleTableType = (x));
   #define osSaveNextScheduleTable_ScheduleTableID_next(y)   (osAPIParam2.osScheduleTableType = (y));
   #define osSaveGetScheduleTableStatus_ScheduleID(x)        (osAPIParam1.osScheduleTableType = (x));
   #define osSaveGetScheduleTableStatus_ScheduleStatus(y)    (osAPIParam2.osScheduleTableStatusRefType = (y));
#if (osdScheduleTableSyncUsed != 0)
   #define osSaveStartScheduleTableSynchron_ScheduleTableID(x) (osAPIParam1.osScheduleTableType = (x));
   #define osSaveSyncScheduleTable_ScheduleID(x)             (osAPIParam1.osScheduleTableType = (x));
   #define osSaveSyncScheduleTable_GlobalTime(y)             (osAPIParam2.osTickType = (y));
   #define osSaveSetScheduleTableAsync_ScheduleID(x)         (osAPIParam1.osScheduleTableType = (x));
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   #define osSaveCallTrustedFunction_FunctionIndex(x)   (osAPIParam1.osFunctionIndexType = (x));
   #define osSaveCallTrustedFunction_FunctionParams(y)  (osAPIParam2.osTrustedFunctionParameterRefType = (y));
   #define osSaveTerminateApplication_RestartOption(x)  (osAPIParam1.osRestartType = (x));
#endif

   #define osSaveGetTaskMaxExecutionTime_TaskID(x)           (osAPIParam1.osTaskType     = (x));
   #define osSaveGetTaskMaxExecutionTime_MaxTime(y)          (osAPIParam2.osTimeRefType  = (y));
   #define osSaveGetISRMaxExecutionTime_ISRID(x)             (osAPIParam1.osISRType      = (x));
   #define osSaveGetISRMaxExecutionTime_MaxTime(y)           (osAPIParam2.osTimeRefType  = (y));
   #define osSaveGetTaskMinInterArrivalTime_TaskID(x)        (osAPIParam1.osTaskType     = (x));
   #define osSaveGetTaskMinInterArrivalTime_MinTime(y)       (osAPIParam2.osUnionTPTimeStampRefType  = (y));
   #define osSaveGetISRMinInterArrivalTime_ISRID(x)          (osAPIParam1.osISRType      = (x));
   #define osSaveGetISRMinInterArrivalTime_MinTime(y)        (osAPIParam2.osUnionTPTimeStampRefType  = (y));
   #define osSaveGetTaskMaxBlockingTime_TaskID(x)            (osAPIParam1.osTaskType     = (x));
   #define osSaveGetTaskMaxBlockingTime_BlockType(y)         (osAPIParam2.osBlockType    = (y));
   #define osSaveGetTaskMaxBlockingTime_ResourceID(z)        (osAPIParam3.osResourceType = (z));
   #define osSaveGetTaskMaxBlockingTime_MaxTime(z1)          (osAPIParam4                = (z1));
   #define osSaveGetISRMaxBlockingTime_ISRID(x)              (osAPIParam1.osISRType      = (x));
   #define osSaveGetISRMaxBlockingTime_BlockType(y)          (osAPIParam2.osBlockType    = (y));
   #define osSaveGetISRMaxBlockingTime_ResourceID(z)         (osAPIParam3.osResourceType = (z));
   #define osSaveGetISRMaxBlockingTime_MaxTime(z1)           (osAPIParam4                = (z1));
#else /* osdParameterAccessMacros && osdErrorHook */

   #define osSaveActivateTask_TaskID(x)  
   #define osSaveChainTask_TaskID(x)     
   #define osSaveGetTaskID_TaskID(x)      
   #define osSaveGetTaskState_TaskID(x)  
   #define osSaveGetTaskState_State(y)  
   #define osSaveGetResource_ResID(x)
   #define osSaveReleaseResource_ResID(x)
   #define osSaveSetEvent_TaskID(x)     
   #define osSaveSetEvent_Mask(y)     
   #define osSaveGetEvent_TaskID(x)     
   #define osSaveGetEvent_Event(y)        
   #define osSaveClearEvent_Mask(x)      
   #define osSaveWaitEvent_Mask(x)       
   #define osSaveGetAlarmBase_AlarmID(x)    
   #define osSaveGetAlarmBase_Info(y)     
   #define osSaveSetRelAlarm_AlarmID(x)   
   #define osSaveSetRelAlarm_increment(y)    
   #define osSaveSetRelAlarm_cycle(z)     
   #define osSaveSetAbsAlarm_AlarmID(x)    
   #define osSaveSetAbsAlarm_start(y)    
   #define osSaveSetAbsAlarm_cycle(z)    
   #define osSaveCancelAlarm_AlarmID(x)   
   #define osSaveGetAlarm_AlarmID(x)      
   #define osSaveGetAlarm_Tick(y)         
   #define osSaveIncrementCounter_CounterID(x)
   #define osSaveStartScheduleTableRel_ScheduleTableID(x)
   #define osSaveStartScheduleTableRel_Offset(y)
   #define osSaveStartScheduleTableAbs_ScheduleTableID(x)
   #define osSaveStartScheduleTableAbs_Tickvalue(y)
   #define osSaveStopScheduleTable_ScheduleTableID(x)
   #define osSaveNextScheduleTable_ScheduleTableID_current(x)
   #define osSaveNextScheduleTable_ScheduleTableID_next(y)
   #define osSaveGetScheduleTableStatus_ScheduleID(x)
   #define osSaveGetScheduleTableStatus_ScheduleStatus(y)
#if (osdScheduleTableSyncUsed != 0)
   #define osSaveStartScheduleTableSynchron_ScheduleTableID(x) 
   #define osSaveSyncScheduleTable_ScheduleID(x)
   #define osSaveSyncScheduleTable_GlobalTime(x)
   #define osSaveSetScheduleTableAsync_ScheduleID(x)
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   #define osSaveCallTrustedFunction_FunctionIndex(x)
   #define osSaveCallTrustedFunction_FunctionParams(y)
   #define osSaveTerminateApplication_RestartOption(x)
#endif

   #define osSaveGetTaskMaxExecutionTime_TaskID(x)
   #define osSaveGetTaskMaxExecutionTime_MaxTime(y)
   #define osSaveGetISRMaxExecutionTime_ISRID(x)
   #define osSaveGetISRMaxExecutionTime_MaxTime(y)
   #define osSaveGetTaskMinInterArrivalTime_TaskID(x)
   #define osSaveGetTaskMinInterArrivalTime_MinTime(y)
   #define osSaveGetISRMinInterArrivalTime_ISRID(x)
   #define osSaveGetISRMinInterArrivalTime_MinTime(y)
   #define osSaveGetTaskMaxBlockingTime_TaskID(x)
   #define osSaveGetTaskMaxBlockingTime_BlockType(y)
   #define osSaveGetTaskMaxBlockingTime_ResourceID(z)
   #define osSaveGetTaskMaxBlockingTime_MaxTime(z1)
   #define osSaveGetISRMaxBlockingTime_ISRID(x)
   #define osSaveGetISRMaxBlockingTime_BlockType(y)
   #define osSaveGetISRMaxBlockingTime_ResourceID(z)
   #define osSaveGetISRMaxBlockingTime_MaxTime(z1)

#endif /* osdParameterAccessMacros && osdErrorHook else */


#if osdComUseParameterAccess && osdComErrorHook
   #define osSaveStartCOM_ApplicationModeID(x)   (osAPIParam1.osCOMApplicationModeType = (x));
   #define osSaveStopCOM_ApplicationModeID(x)    (osAPIParam1.osCOMApplicationModeType = (x));
   #define osSaveSendMessage_Message(x)          (osAPIParam1.osMessageIdentifier = (x));
   #define osSaveSendMessage_DataRef(x)          (osAPIParam2.osApplicationDataRef = (x));
   #define osSaveReceiveMessage_Message(x)       (osAPIParam1.osMessageIdentifier = (x));
   #define osSaveReceiveMessage_DataRef(x)       (osAPIParam2.osApplicationDataRef = (x));
   #define osSaveInitMessage_Message(x)          (osAPIParam1.osMessageIdentifier = (x));
   #define osSaveInitMessage_DataRef(x)          (osAPIParam2.osApplicationDataRef = (x));
   #define osSaveGetMessageStatus_Message(x)     (osAPIParam1.osMessageIdentifier = (x));
#else
   #define osSaveStartCOM_ApplicationModeID(x)
   #define osSaveStopCOM_ApplicationModeID(x)
   #define osSaveSendMessage_Message(x)
   #define osSaveSendMessage_DataRef(x)
   #define osSaveReceiveMessage_Message(x)
   #define osSaveReceiveMessage_DataRef(x)
   #define osSaveInitMessage_Message(x)
   #define osSaveInitMessage_DataRef(x)
   #define osSaveGetMessageStatus_Message(x)
#endif

#if osdComErrorHook
   #if osdComUseGetServiceID
   #define COMServiceId_StartCOM                0xB100U
   #define COMServiceId_StopCOM                 0xB200U
   #define COMServiceId_SendMessage             0xB300U
   #define COMServiceId_ReceiveMessage          0xB400U
   #define COMServiceId_GetMessageStatus        0xB500U
   #define COMServiceId_GetCOMApplicationMode   0xB600U
   #define COMServiceId_InitMessage             0xB700U
   #endif

   #if osdComUseParameterAccess
   #define COMError_StartCOM_Mode()       (osAPIParam1.osCOMApplicationModeType)
   #define COMError_StopCOM_Mode()        (osAPIParam1.osCOMApplicationModeType)
   #define COMError_SendMessage_Message() (osAPIParam1.osMessageIdentifier)
   #define COMError_SendMessage_DataRef() (osAPIParam2.osApplicationDataRef)
   #define COMError_ReceiveMessage_Message() (osAPIParam1.osMessageIdentifier)
   #define COMError_ReceiveMessage_DataRef() (osAPIParam2.osApplicationDataRef)
   #define COMError_InitMessage_Message() (osAPIParam1.osMessageIdentifier)
   #define COMError_InitMessage_DataRef() (osAPIParam2.osApplicationDataRef)
   #define COMError_GetMessageStatus_Message() (osAPIParam1.osMessageIdentifier)
   #endif

   #define COMErrorGetServiceId()       ((COMServiceIdType) (osLastError & 0xFF00U))
#endif

 #if osdErrorHook
  #if osdGetServiceIdMacros
   
   #define OSServiceId_ActivateTask          0x1100U
   #define OSServiceId_TerminateTask         0x1200U
   #define OSServiceId_ChainTask             0x1300U
   #define OSServiceId_Schedule              0x1400U
   #define OSServiceId_GetTaskState          0x1500U
   #define OSServiceId_GetTaskID             0x1600U

   #define OSServiceId_EnableAllInterrupts   0x2400U
   #define OSServiceId_DisableAllInterrupts  0x2500U
   #define OSServiceId_ResumeOSInterrupts    0x2A00U
   #define OSServiceId_SuspendOSInterrupts   0x2900U
   #define OSServiceId_ResumeAllInterrupts   0x2C00U
   #define OSServiceId_SuspendAllInterrupts  0x2B00U

   #define OSServiceId_GetResource           0x3100U
   #define OSServiceId_ReleaseResource       0x3200U

   #define OSServiceId_SetEvent              0x4100U
   #define OSServiceId_ClearEvent            0x4200U
   #define OSServiceId_GetEvent              0x4300U
   #define OSServiceId_WaitEvent             0x4400U

   #define OSServiceId_GetAlarmBase          0x5100U
   #define OSServiceId_GetAlarm              0x5200U
   #define OSServiceId_SetRelAlarm           0x5300U
   #define OSServiceId_SetAbsAlarm           0x5400U
   #define OSServiceId_CancelAlarm           0x5500U

   #define OSServiceId_StartScheduleTableRel  0x7100U
   #define OSServiceId_StartScheduleTableAbs  0x7200U
   #define OSServiceId_StopScheduleTable      0x7300U
   #define OSServiceId_GetScheduleTableStatus 0x7400U
   #define OSServiceId_NextScheduleTable      0x7500U

   #define OSServiceId_IncrementCounter        0x8100U
   #define OSServiceId_GetISRID                0x8200U

   #define OSServiceId_CallTrustedFunction     0x9300U
   #define OSServiceId_TerminateApplication    0x9400U
   #define OSServiceId_SyncScheduleTable       0x9500U
   #define OSServiceId_SetScheduleTableAsync   0x9600U
   #define OSServiceId_GetTaskMaxExecutionTime 0x9800U
   #define OSServiceId_GetISRMaxExecutionTime  0x9900U
   #define OSServiceId_GetTaskMaxBlockingTime  0x9A00U
   #define OSServiceId_GetISRMaxBlockingTime   0x9B00U
   #define OSServiceId_StartScheduleTableSynchron   0x9C00U
   #define OSServiceId_GetTaskMinInterArrivalTime 0x9000U
   #define OSServiceId_GetISRMinInterArrivalTime  0x9F00U


   /* fullfills [SR:154] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   #define OSErrorGetServiceId()       ((OSServiceIdType) (osLastError & 0xFF00U))

  #endif

   /* fullfills [SR:154] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   #define OSErrorGetosCANError()      osLastError

   #if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended)) && (!defined(osdSuppressFilenames) )
   /* fullfills [SR:154] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
    #define OSErrorGetosCANLineNumber() osActiveTaskLineNumber
    #define OSErrorGetosCANModulName()  osActiveTaskModule
   #endif

  #if osdParameterAccessMacros 
   /* fullfills [SR:154] of $\AutosarOS\general\Generation5\_doc\12_SystemRequirements\RequirementsSpec_MicrosarOS.doc */
   #define OSError_ActivateTask_TaskID()                      (osAPIParam1.osTaskType) 
   #define OSError_ChainTask_TaskID()                         (osAPIParam1.osTaskType) 
   #define OSError_GetTaskID_TaskID()                         (osAPIParam1.osTaskRefType) 
   #define OSError_GetTaskState_TaskID()                      (osAPIParam1.osTaskType) 
   #define OSError_GetTaskState_State()                       (osAPIParam2.osTaskStateRefType) 

   #define OSError_GetResource_ResID()                        (osAPIParam1.osResourceType) 
   #define OSError_ReleaseResource_ResID()                    (osAPIParam1.osResourceType) 

   #define OSError_SetEvent_TaskID()                          (osAPIParam1.osTaskType) 
   #define OSError_SetEvent_Mask()                            (osAPIParam2.osEventMaskType) 
   #define OSError_GetEvent_TaskID()                          (osAPIParam1.osTaskType) 
   #define OSError_GetEvent_Event()                           (osAPIParam2.osEventMaskRefType) 
   #define OSError_ClearEvent_Mask()                          (osAPIParam1.osEventMaskType) 
   #define OSError_WaitEvent_Mask()                           (osAPIParam1.osEventMaskType) 

   #define OSError_GetAlarmBase_AlarmID()                     (osAPIParam1.osAlarmType) 
   #define OSError_GetAlarmBase_Info()                        (osAPIParam2.osAlarmBaseRefType) 
   #define OSError_SetRelAlarm_AlarmID()                      (osAPIParam1.osAlarmType) 
   #define OSError_SetRelAlarm_increment()                    (osAPIParam2.osTickType) 
   #define OSError_SetRelAlarm_cycle()                        (osAPIParam3.osTickType)
   #define OSError_SetAbsAlarm_AlarmID()                      (osAPIParam1.osAlarmType) 
   #define OSError_SetAbsAlarm_start()                        (osAPIParam2.osTickType) 
   #define OSError_SetAbsAlarm_cycle()                        (osAPIParam3.osTickType)
   #define OSError_CancelAlarm_AlarmID()                      (osAPIParam1.osAlarmType) 
   #define OSError_GetAlarm_AlarmID()                         (osAPIParam1.osAlarmType) 
   #define OSError_GetAlarm_Tick()                            (osAPIParam2.osTickRefType)
 
   #define OSError_StartScheduleTableRel_ScheduleTableID()    (osAPIParam1.osScheduleTableType)
   #define OSError_StartScheduleTableRel_Offset()             (osAPIParam2.osTickType)
   #define OSError_StartScheduleTableAbs_ScheduleTableID()    (osAPIParam1.osScheduleTableType)
   #define OSError_StartScheduleTableAbs_Tickvalue()          (osAPIParam2.osTickType)
   #define OSError_StopScheduleTable_ScheduleTableID()        (osAPIParam1.osScheduleTableType)
   #define OSError_NextScheduleTable_ScheduleTableID_current()(osAPIParam1.osScheduleTableType)
   #define OSError_NextScheduleTable_ScheduleTableID_next()   (osAPIParam2.osScheduleTableType)
   #define OSError_IncrementCounter_CounterID()               (osAPIParam1.osCounterType)
   #define OSError_GetScheduleTableStatus_ScheduleID()        (osAPIParam1.osScheduleTableType)
   #define OSError_GetScheduleTableStatus_ScheduleStatus()    (osAPIParam2.osScheduleTableStatusRefType)
#if (osdScheduleTableSyncUsed != 0)
   #define OSError_StartScheduleTableSynchron_ScheduleTableID(x) (osAPIParam1.osScheduleTableType)
   #define OSError_SyncScheduleTable_ScheduleID()             (osAPIParam1.osScheduleTableType)
   #define OSError_SyncScheduleTable_GlobalTime()             (osAPIParam2.osTickType)
   #define OSError_SetScheduleTableAsync_ScheduleID()         (osAPIParam1.osScheduleTableType)
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   #define OSError_CallTrustedFunction_FunctionIndex()        (osAPIParam1.osFunctionIndexType) 
   #define OSError_CallTrustedFunction_FunctionParams()       (osAPIParam2.osTrustedFunctionParameterRefType) 
   #define OSError_TerminateApplication_RestartOption()       (osAPIParam1.RestartType) 
#endif
   #define OSError_GetTaskMaxExecutionTime_TaskID()           (osAPIParam1.osTaskType)
   #define OSError_GetTaskMaxExecutionTime_MaxTime()          (osAPIParam2.osTimeRefType)
   #define OSError_GetISRMaxExecutionTime_ISRID()             (osAPIParam1.osISRType)
   #define OSError_GetISRMaxExecutionTime_MaxTime()           (osAPIParam2.osTimeRefType)
   #define OSError_GetTaskMinInterArrivalTime_TaskID()        (osAPIParam1.osTaskType)
   #define OSError_GetTaskMinInterArrivalTime_MinTime()       (osAPIParam2.osTimeRefType)
   #define OSError_GetISRMinInterArrivalTime_ISRID()          (osAPIParam1.osISRType)
   #define OSError_GetISRMinInterArrivalTime_MinTime()        (osAPIParam2.osTimeRefType)
   #define OSError_GetTaskMaxBlockingTime_TaskID()            (osAPIParam1.osTaskType)
   #define OSError_GetTaskMaxBlockingTime_BlockType()         (osAPIParam2.osBlockType)
   #define OSError_GetTaskMaxBlockingTime_ResourceID()        (osAPIParam3.osResourceType)
   #define OSError_GetTaskMaxBlockingTime_MaxTime()           (osAPIParam4)
   #define OSError_GetISRMaxBlockingTime_ISRID()              (osAPIParam1.osISRType)
   #define OSError_GetISRMaxBlockingTime_BlockType()          (osAPIParam2.osBlockType)
   #define OSError_GetISRMaxBlockingTime_ResourceID()         (osAPIParam3.osResourceType)
   #define OSError_GetISRMaxBlockingTime_MaxTime()            (osAPIParam4)
  #endif
 #endif


/*lint -restore re-enable messages*/

#endif /* double include preventer */

/* END OF HEADER osekerr.h */
