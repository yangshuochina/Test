/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* STARTSINGLE_OF_MULTIPLE */


/* ***************************************************************************
| Project Name: TricoreMulticanHll
|
|  Description: Implementation of the CAN driver
|               Target systems:        TriCore
|               supported derivatives: TC1797, TC1796, TC1766, TC1767, TC1768
|                                      TC1xxx (generic)
|               supported Compiler:    GNU / Tasking / GHS                  */
/* ---------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 1996-2010 by Vector Informatik GmbH.       All rights reserved.
|
| This software is copyright protected and proprietary 
| to Vector Informatik GmbH. Vector Informatik GmbH 
| grants to you only those rights as set out in the 
| license conditions. All other rights remain with 
| Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Bir          Holger Birke              Vector Informatik GmbH
| Ces          Senol Cendere             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------  ----------------------------------------------------
| 2003-09-08 1.00.00 Bir  - first implementation, Review modify
| 2004-01-24 1.01.00 Bir  - add sleepmode
|                         - MultiBasics
|                         - Workaround CAN_TC.21
| 2004-01-23 1.02.00 Bir  - Individual polling + Functions
|                         - lint
|                         - No Lock of HwObj with MsgVal
|                         - Use Hw-Fifo
|                         - Buffer Rx Basic+Full for consistency, adapt RDSmakros
|                         - 2nd review, FDRreg Initworkaround
| 2004-03-17 1.03.00 Bir  - lint
| 2004-04-28 1.04.00 Bir  - add C_ENABLE_MULTIPLE_CONFIG for Interrupt init
| 2004-08-18 1.05.00 Bir  - B-Step: capsulate ALERT bit in ISR
|                         - runtime optimization: HWloop check improved
|                         - runtime optimization: Optimize ISR loop, initialize MOFGPR for all objects
|                         - ESCAN00009248: MSPND bitdelete without Read-Modify-Write
|                         - ESCAN00009248: Lost of CAN Interrupts
|                         - New Feautre: Switch to disable nested CAN-interrupts (C_ENABLE_NESTED_INTERRUPTS)
|                         - New Feautre: Switch for use of SRC register C_ENABLE_CAN_INTERRUPT_CONTROL
|                         - ESCAN00009488: Naming Conventions
|                         - Individual Basic Tx do not work in queue
|                         - ESCAN00009610: Issue in Sleep, Wakeup functionality
| 2004-10-26 1.06.00 Bir  - minor changes (AutoSar)
|                         - minor changes using no standard types
|                         - adapted to new HL RI1.4 V1.30
|                         - ESCAN00010510: support mixed id
|                         - ESCAN00010508: misra complience
|                         - ESCAN00010509: support runtime measurement
|                         - ESCAN00010440: improve runtime
|                         - encapsulate CAN-cell access while CanTask()
| 2004-10-26 1.07.00 Bir  - Version comment wrong for search in Clearquest
|                         - ESCAN00010917: Support B-Step Hardware TC1796 and TC1766
| 2004-04-21 1.08.00 Bir  - ESCAN00012007: Lost of transmit confirmation / stop sending in Polling mode
|                         - Adapt to RI1.4 HL1.34: change C_ENABLE_MULTIPLE_CONFIG to C_ENABLE_MULTI_ECU_CONFIG
|                         - Issue in "Multiple Config" for Alert (BusOff) Interrupt
|                         - ESCAN00012488: Interrupt Service Routine is called for mailboxes configured in polling mode
|                         - ESCAN00012899: Multiple Configuration and Identity expansions
| 2004-08-15 1.09.00 Bir  - ESCAN00013203: internal comments in source file
|                         - ESCAN00013414: Use receive temporary buffer as a register buffer
|                         - ESCAN00013236: Compile Error: undeclared identifier, if individual polling enabled
| 2006-01-05 1.10.00 Bir  - ESCAN00014784: Code/Memory Category Comments needed
| 2006-04-18 1.10.01 Bir  - ESCAN00016030: wrong interrupt initialisation
|                         - Code Review update
| 2006-07-12 1.11.00 Bir  - Update HighLevel ASR 2.06 (no changes)
| 2006-08-16 1.12.00 Bir  - Update HighLevel ASR 2.07 (OSEK mapping of interrupt lock functions)
| 2006-03-08 2.00.00 Bir  - Adapt to new RI1.5
|                         - new HL
|                         - ESCAN00016966: pure polling system without any ISR
|                         - Code Category for Low Level variables
|                         - Add New Platform XC2000 derivative XC2287
|                         - reduce RAM memory consumption (remove interrupt masks)
| 2007-01-25 2.01.00 Bir  - ESCAN00019203: loss of tx interrupt (change CanLL_InitEnd)
|                         - ESCAN00019307: Review reworks for RI15
|                         - FullCAN Extended Id masking also for pure extended
|                         - ESCAN00021117: wrong memory mapping comment for CanTxIdentityAssignment
|                         - ESCAN00021151: Use HW-node interrupts instead of logical channel interrupts
|                         - Update to new template (ISR encapsulate by ASR2.1 switches)
|                         - ESCAN00036425: Unused channels activated - side effects to used channels
|                         - ESCAN00039248: Error Frames during initialization
| 2007-09-25 2.02.00 Ces  - ESCAN00022662: support compiler Tasking V8.6R3 for XC2000
|                         - updated HLL component to V2.05.00
|                         - support configuration tool GENy for XC2000
| 2007-10-29 2.03.00 Bir  - ESCAN00023032: Add Switch C_ENABLE_MULTIPLE_DRIVERS
| 2007-12-04 2.03.01 Bir  - ESCAN00023531: Init only used channels to avoid side effects on unused
| 2007-12-10 2.04.00 Bir  - ESCAN00023626: Add new derivative 1797 (changed addressoffsets)
|                         - ESCAN00023538: Generate table canPhysPrio and canPhysGroup
| 2008-02-04 2.04.01 Bir  - ESCAN00024354: Update HL-ASR (no change here)
| 2008-02-14 2.05.00 Bir  - ESCAN00024530: MEMORY_CAN for XC2000 near model to far
|                         - Update HL-ASR 3.0 (no change here)
| 2008-02-14 2.06.00 Bir  - review reworks
|                         - ESCAN00025994: Postbuild for table "CanInterruptPendingMask" (linktime before)
| 2008-07-04 2.07.00 Bir  - ESCAN00028128, ESCAN00028130: CAN DRV might execute the RX nofication 2times for 1msg
|                         - ESCAN00029952: support 1767
|                         - ESCAN00029987: compiler warning unused variable "copy_cnt"
| 2008-09-23 2.08.00 Bir  - ESCAN00030164: support XC2361
|                           ESCAN00029997: missing hardware loop check
| 2008-12-04 2.08.01 Bir  - ESCAN00030164: check generated mailbox and node amount for generic platforms
| 2008-12-12 2.08.02 Bir  - ESCAN00031987: write on illlegal RAM address
| 2009-01-15 2.09.00 Bir  - ESCAN00032396: Add derivative 1768
|                         - ESCAN00032482: Add extended RAM check feature
|                         - ESCAN00033087: AutoSar only: Software Trap due to access NULL-pointer
| 2009-03-02 2.10.00 Bir  - ESCAN00033517: Add XC2000 platform for ASR + derivative XC2336B, XC2363B
| 2009-04-20 2.11.00 Bir  - ESCAN00034643: Add compiler and memory abstraction + update to ASR Core 3.08
|                         - ESCAN00036146: Runtime optimization
| 2009-07-22 2.11.01 Bir  - Remove compiler abstraction for automatic variable / review rework
|                         - ESCAN00036616: activate Clock inside CanInit() to avoid problems when called in sleep mode
| 2009-09-04 2.11.02 Bir  - ESCAN00038205: FullCAN Rx with standard ID will be masked like extended ID
| 2010-01-19 2.12.00 Bir  - ESCAN00040216: ASR: Support Highend feature (Indiv.Polling, RxQueue, Multiple BasicCAN)
|                         - rework for Vector coding rules
| 2010-03-03 2.13.00 Bir  - ESCAN00041312: Support Remote Frames
| 2010-03-29 2.14.00 Bir  - ESCAN00041897: support new derivative XC2267M
|                         - ESCAN00041898: Add Group priority extension bit GPX
|                         - ESCAN00041932: Can_TransmitRemote() will not send extended ID
|************************************************************************* */
/*****************************************************************************
|
|    ************    Version and change information of      **********        
|    ************    high level part only                   **********        
|
|    Please find the version number of the whole module in the previous 
|    File header.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
| Pl           Georg Pfluegel            Vector Informatik GmbH
| Vg           Frank Voorburg            Vector CANtech, Inc.
| An           Ahmad Nasser              Vector CANtech, Inc.
| Ml           Patrick Markl             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------  ----------------------------------------------------
| 19-Jan-01  0.02  Ht     - derived form C16x V3.3
| 18-Apr-01  1.00  Pl     - derived for ARM7 TDMI
| 02-May-01  1.01  Ht     - adaption to LI1.2
|                         - change from code doupling to indexed
| 31-Oct-01  1.02  Ht     - support hash search
|                  Ht     - optimisation for message access (hardware index)
|                  Vg     - adaption for PowerPC
| 07-Nov-01  1.03  Ht     - remove some comments
|                         - support of basicCAN polling extended
| 12-Dez-01  1.04  Ht     - avoid compiler warnings for KEIL C166
|                         - ESCAN00001881: warning in CanInitPowerOn
|                         - ESCAN00001913: call of CanLL_TxEnd()
|                  Fz     - ESCAN00001914: CanInterruptRestore changed for M32C
| 02-Jan-02  1.05  Ht     - ESCAN00002009: support of polling mode improved
|                         - ESCAN00002010: Prototype of CanHL_TxConfirmation() 
|                                          not available in every case.
| 12-Feb-02  1.06 Pl      - ESCAN00002279: - now it is possible to use only the error-task without the tx-task
|                                          - support of the makro  REENTRANT
|                                          - support of the makro C_HL_ENABLE_RX_INFO_STRUCT_PTR
|                                          - For better performance for the T89C51C there is a switch-case
|                                            instruction for direct call of the PreTransmitfunction
|                                          - add C_ENABLE_RX_BASICCAN_POLLING in CanInitPowerOn
| 18-Mai-02  1.07 Ht      - ESCAN....    : support Hash search with FullCAN controller
|                         - ESCAN00002707: Reception could went wrong if IL and Hash Search
|                         - ESCAN00002893: adaption to LI 1.3
| 29-Mai-02  1.08 Ht      - ESCAN00003028: Transmission could fail in Polling mode
|                         - ESCAN00003082: call Can_LL_TxEnd() in CanMsgTransmit()
|                         - ESCAN00003083: Hash search with extended ID
|                         - ESCAN00003084: Support C_COMP_METROWERKS_PPC
|                         - ESCAN00002164: Temporary vaiable "i" not defined in function CanBasicCanMsgReceived
|                         - ESCAN00003085: support C_HL_ENABLE_IDTYPE_IN_ID
| 25-Jun     1.08.01 Vg   - Declared localInterruptOldFlag in CanRxTask()
|                         - Corrected call to CanWakeUp for multichannel
| 11-Jul-02  1.08.02 Ht   - ESCAN00003203: Hash Search routine does not work will with extended IDs
|                         - ESCAN00003205: Support of ranges could went wrong on some platforms
| 08-Aug-02  1.08.03 Ht   - ESCAN00003447: Transmission without databuffer and pretransmit-function 
| 08-Aug-02  1.08.04 An   - Added support to Green Hills
| 09-Sep-02  1.09    Ht   - ESCAN00003837: code optimication with KernelBuilder
|                         - ESCAN00004479: change the place oft the functioncall of CanLL_TxCopyMsgTransmit
|                                          in CanMsgTransmit 
| 2002-12-06 1.10    Ht   -                Support consistancy for polling tasks
|                         - ESCAN00004567: Definiton of V_NULL pointer
|                         -                remove include of string.h
|                         -                support Dummy functions for indirect function call 
|                         -                optimization for one single Tx mail box
| 2003-02-04 1.11    Ht   -                optimization for polling mode
| 2003-03-19 1.12    Ht   - ESCAN00005152: optimization of CanInit() in case of Direct Tx Objects
|                         - ESCAN00005143: CompilerWarning about function prototype 
|                                          CanHL_ReceivedRxHandle() and CanHL_IndRxHandle
|                         - ESCAN00005130: Wrong result of Heash Search on second or higher channel               
| 2003-05-12 1.13    Ht   - ESCAN00005624: support CantxMsgDestroyed for multichannel system
|                         - ESCAN00005209: Support confirmation and indication flags for EasyCAN4
|                         - ESCAN00004721: Change data type of Handle in CanRxInfoStruct
| 2003-06-18 1.20   Ht    - ESCAN00005908: support features of RI1.4
|                         - ESCAN: Support FJ16LX-Workaround for multichannel system
|                         - ESCAN00005671: Dynamic Transmit Objects: ID in extended ID frames is wrong
|                         - ESCAN00005863: Notification about cancelation failes in case of CanTxMsgDestroyed
| 2003-06-30 1.21   Ht   - ESCAN00006117: Common Confirmation Function: Write access to wrong memory location
|                        - ESCAN00006008: CanCanInterruptDisable in case of polling
|                        - ESCAN00006118: Optimization for Mixed ID and ID type in Own Register or ID type in ID Register
|                        - ESCAN00006100: transmission with wrong ID in mixed ID mode
|                        - ESCAN00006063: Undesirable hardware dependency for 
|                                         CAN_HL (CanLL_RxBasicTxIdReceived)
| 2003-09-10 1.22   Ht   - ESCAN00006853: Support V_MEMROM0
|                        - ESCAN00006854: suppress some Compiler warnings
|                        - ESCAN00006856: support CanTask if only Wakeup in polling mode
|                        - ESCAN00006857: variable newDLC not defined in case of Variable DataLen
| 2003-10-14 1.23   Ht   - ESCAN00006858: support BrsTime for internal runtime measurement
|                        - ESCAN00006860: support Conditional Msg Receive
|                        - ESCAN00006865: support "Cancel in HW" with CanTask
|                        - ESCAN00006866: support Direct Tx Objects
|                        - ESCAN00007109: support new memory qualifier for const data and pointer to const
| 2004-01-05 1.24   Ml   - ESCAN00007206: resolved preprocessor error for Hitachi compiler
|                   Ml   - ESCAN00007254: several changes
| 2004-02-06 1.25   Ml   - ESCAN00007281: solved compilerwarning
|                   Ml   - removed compiler warnings
| 2004-02-21 1.26   Ml   - ESCAN00007670: CAN RAM check
|                   Ml   - ESCAN00007671: fixed dyn Tx object issue
|                   Ml   - ESCAN00007764: added possibility to adjust Rx handle in LL drv
|                   Ml   - ESCAN00007681: solved compilerwarning in CanHL_IndRxHandle
|                   Ml   - ESCAN00007272: solved queue transmission out of LowLevel object
|                   Ml   - ESCAN00008064: no changes
| 2004-04-16 1.27   Ml   - ESCAN00008204: no changes
|                   Ml   - ESCAN00008160: no changes
|                   Ml   - ESCAN00008266: changed name of parameter of function CanTxGetActHandle
|                   Fz   - ESCAN00008272: Compiler error due to missing array canPollingTaskActive
| 2004-05-10 1.28   Fz   - ESCAN00008328: Compiler error if cancel in hardware is active
|                        - ESCAN00008363: Hole closed when TX in interrupt and cancel in HW is used
|                        - ESCAN00008365: Switch C_ENABLE_APPLCANPREWAKEUP_FCT added
|                        - ESCAN00008391: Wrong parameter macro used in call of 
|                                         CanLL_WakeUpHandling
| 2004-05-24 1.29   Ht   - ESCAN00008441: Interrupt not restored in case of internal error if TX Polling is used
| 2004-09-21 1.30   Ht   - ESCAN00008914: CAN channel may stop transmission for a certain time
|                        - ESCAN00008824: check of reference implementation version added
|                        - ESCAN00008825: No call of ApplCanMsgCancelNotification during CanInit()
|                        - ESCAN00008826: Support asssertions for "Conditional Message Received"  
|                   Ml   - ESCAN00008752: Added function qualifier macros
|                   Ht   - ESCAN00008823: compiler error due to array size 0
|                        - ESCAN00008977: label without instructions
|                        - ESCAN00009485: Message via Normal Tx Object will not be sent  
|                        - ESCAN00009497: support of CommonCAN and RX queue added
|                        - ESCAN00009521: Inconsitancy in total polling mode
| 2004-09-28 1.31   Ht   - ESCAN00009703: unresolved functions CAN_POLLING_IRQ_DISABLE/RESTORE()
| 2004-11-25 1.32   Ht   - move fix for ESCAN00007671 to CAN-LL of DrvCan_MpcToucanHll
|                        - ESCAN00010350: Dynamic Tx messages are send always with Std. ID
|                        - ESCAN00010388: ApplCanMsgConfirmed will only be called if realy transmitted
|                    Ml  - ESCAN00009931: The HardwareLoopCheck should have a channelparameter in multichannel systems.
|                    Ml  - ESCAN00010093: lint warning: function type inconsistent "CanCheckMemory"
|                    Ht  - ESCAN00010811: remove Misra and compiler warnings
|                        - ESCAN00010812: support Multi ECU
|                        - ESCAN00010526: CAN interrupts will be disabled accidently
|                        - ESCAN00010584: ECU may crash or behave strange with Rx queue active
| 2005-01-20 1.33    Ht  - ESCAN00010877: ApplCanMsgTransmitConf() is called erronemous
| 2005-03-03 1.34    Ht  - ESCAN00011139: Improvement/Correction of C_ENABLE_MULTI_ECU_CONFIG
|                        - ESCAN00011511: avoid PC-Lint warnings
|                        - ESCAN00011512: copy DLC in case of variable Rx Datalen
|                        - ESCAN00010847: warning due to missing brakets in can_par.c at CanChannelObject
| 2005-05-23 1.35   Ht   - ESCAN00012445: compiler error "V_MEMROMO undefined"in case of multi ECU
|                        - ESCAN00012350: Compiler Error "Illegal token channel"
| 2005-07-06 1.36   Ht   - ESCAN00012153: Compile Error: missing declaration of variable i
|                        - ESCAN00012460: Confirmation of LowLevel message will run into assertion (C_ENABLE_MULTI_ECU_PHYS enabled)
|                        - support Testpoints for CanTestKit
| 2005-07-14 1.37   Ht   - ESCAN00012892: compile error due to missing logTxObjHandle
|                        - ESCAN00012998: Compile Error: missing declaration of txHandle in CanInit()
|                        - support Testpoints for CanTestKit for FullCAN controller
| 2005-09-21 1.38   Ht   - ESCAN00013597: Linker error: Undefined symbol 'CanHL_IndRxHandle'
| 2005-11-10 1.39.00 Ht  - ESCAN00014331: Compile error due to missing 'else' in function CanTransmit
|
| 2005-04-26 2.00.00  Ht - ESCAN00016698: support RI1.5
|                        - ESCAN00014770: Cosmic compiler reports truncating assignement
|                        - ESCAN00016191: Compiler warning about unused variable in CanTxTask
|
| 2007-01-23 2.01.00  Ht - ESCAN00017279: Usage of SingleGlobalInterruptDisable lead to assertion in OSEK
|                        - ESCAN00017148: Compile error in higher layer due to missing declaration of CanTxMsgHandleToChannel
| 2007-03-14 2.02.00  Ht - ESCAN00019825: error directives added and misra changes
|                        - ESCAN00019827: adaption to never version of VStdLib.
|                        - ESCAN00019619: V_CALLBACK_1 and V_CALLBACK_2 not defined
|                        - ESCAN00019953: Handling of FullCAN reception in interrupt instead of polling or vice versa.
|                        - ESCAN00019958: CanDynTxObjSetId() or CanDynTxObjSetExtId() will run into assertion
| 2007-03-26 2.03.00  Ht - ESCAN00019988: Compile warnings in can_drv.c
|                        - ESCAN00018831: polling mode: function prototype without function implemenation (CanRxFullCANTask + CanRxBasicCANTask)
| 2007-04-20 2.04.00  dH - ESCAN00020299: user assertion fired irregularly due to unknown parameter (in case of CommonCAN)
| 2007-05-02 2.05.00  Ht - ESCAN00021069: Handling of canStatus improved, usage of preprocessor defines unified
|                        - ESCAN00021070: support relocation of HW objects in case of Multiple configuration
|                        - ESCAN00021166: Compiler Warnings: canHwChannel & canReturnCode not used in CanGetStatus
|                        - ESCAN00021223: CanCanInterruptDisabled called during Sleepmode in CanWakeupTask
|                        - ESCAN00022048: Parameter of ApplCancorruptMailbox is hardware channel instead of logical channel - Error directive added
| 2007-11-12 2.06.00  Ht - ESCAN00023188: support CAN Controller specific polling sequence for BasicCAN objects
|                        - ESCAN00023208: Compile issue about undefined variable kCanTxQueuePadBits in the CAN driver in Bit based Tx queue
| 2008-10-20 2.07.00  Ht - ESCAN00023010: support disabled mailboxes in case of extended RAM check
|                        - ESCAN00025706: provide C_SUPPORTS_MULTI_ECU_PHYS
|                        - ESCAN00026120: compiler warnings found on DrvCan_V85xAfcanHll RI 1.5     ##Ht: reviewed 2008-09-03
|                        - ESCAN00026322: ApplCanMsgNotMatched not called in special configuration
|                        - ESCAN00026413: Add possibility to reject remote frames received by FullCAN message objects
|                        - ESCAN00028758: CAN HL must support QNX
|                        - ESCAN00029788: CommonCAN for Driver which support only one Tx object improved (CanInit()).
|                        - ESCAN00029889: Compiler warning about uninitialized variable canHwChannel in CanCanInterruptDisable/Restore()
|                        - ESCAN00029891: Compiler warning: variable "rxHandle" was set but never used
|                        - ESCAN00029929: Support Extended ID Masking for Tx Fullcan messages 
|                        - ESCAN00030371: Improvements (assertions, misra)
|                        - ESCAN00027931: Wrong check of "queueBitPos" size
| 2009-04-08 2.08.00  Ht - ESCAN00034492: no automatic remove of CanCanInterruptDisable/Restore
|                        - ESCAN00031816: CANRANGExIDTYPE can be removed and direct expression used
|                        - ESCAN00032027: CanMsgTransmit shall support tCanMsgTransmitStruct pointer accesses to far RAM
|                        - ESCAN00034488: Postfix for unsigned const in perprocessor directives are not supported by all Compiler (ARM-Compiler 1.2)
| 2009-06-04 2.08.01  Ht - ESCAN00035426: Compiler warning about truncation in CanGetStatus removed
| 2009-10-21 2.09.00  Ht - ESCAN00036258: Compiler warning about "CanHL_ReceivedRxHandle" was declared but never referenced
|                        - ESCAN00038642: Support reentrant functions for compiler with own keyword
|                        - ESCAN00038645: support new switch C_ENABLE_UPDATE_BASE_ADDRESS
| 2010-02-01 2.10.00  Ht - ESCAN00036260: Support configuartion without static Tx messages and only one channel (remove compiler warning)
|                        - ESCAN00040478: Handle update of virtual CanBaseAdress in accordance to QNX documentation
|                        - ESCAN00039235: Compiler Warning: Narrowing or Signed-to-Unsigned type conversion

|
|
|    ************    Version and change information of      **********        
|    ************    high level part only                   **********        
|
|    Please find the version number of the whole module in the previous 
|    File header.
|
|***************************************************************************/

#define C_DRV_INTERNAL

/* PRQA S 3453 EOF */      /* suppress messages: a function should probably used instead of function like macro. misra 19.7 */
/* PRQA S 3109 EOF */      /* suppress messages about empty statements. misra 14.3 */
/* PRQA S 2006 EOF */      /* suppress misra message about multiple return. misra 14.7 */


/*lint -function(exit,ApplCanFatalError)*/

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/

#include "can_inc.h"


/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if( DRVCAN_TRICOREMULTICANHLL_VERSION != 0x0214u)
# error "Source and Header file are inconsistent!"
#endif

#if( DRVCAN_TRICOREMULTICANHLL_RELEASE_VERSION != 0x00u)
# error "Source and Header file are inconsistent!"
#endif



#if( C_VERSION_REF_IMPLEMENTATION != 0x150)
# error "Generated Data and CAN driver source file are inconsistent!"
#endif

#if( DRVCAN__COREHLL_VERSION != 0x0210)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN__COREHLL_RELEASE_VERSION != 0x00)
# error "Source and Header file are inconsistent!"
#endif

#if ( ( DRVCAN__HLLTXQUEUEBIT_VERSION != 0x0106) )
# error "TxQueue Source and Header Version inconsistent!"
#endif
#if ( ( DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION != 0x02) )
# error "TxQueue Source and Header Version inconsistent!"
#endif


#if defined( DRVCAN__HLLTXQUEUEBIT_VERSION )
# if ( ( DRVCAN__HLLTXQUEUEBIT_VERSION != 0x0106) || \
       ( DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION != 0x02)  )
#  error "TxQueue Version inconsistent!"
# endif

/* defines to satisfy MISRA checker tool */
# define DRVCAN__HLLTXQUEUEBYTE_VERSION 0x0000
# define DRVCAN__HLLTXQUEUEBYTE_RELEASE_VERSION 0x00

#else
# if defined( DRVCAN__HLLTXQUEUEBYTE_VERSION )
#  if ( ( DRVCAN__HLLTXQUEUEBYTE_VERSION != 0x0104) || \
       ( DRVCAN__HLLTXQUEUEBYTE_RELEASE_VERSION != 0x00)  )
#   error "TxQueue Version inconsistent!"
#  endif
# else
#  error "No TxQueue available"
# endif

/* defines to satisfy MISRA checker tool */
# define DRVCAN__HLLTXQUEUEBIT_VERSION 0x0000
# define DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION 0x00

#endif


/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/


/* ##RI-1.1: Object parameters for Tx-Observe functions */
/* status of transmit objects */
#define kCanBufferFree                                     ((CanTransmitHandle)0xFFFFFFFFU)   /* mark a transmit object is free */
#define kCanBufferCancel                                   ((CanTransmitHandle)0xFFFFFFFEU)   /* mark a transmit object as canceled */
#define kCanBufferMsgDestroyed                             ((CanTransmitHandle)0xFFFFFFFDU)   /* mark a transmit object as destroyed */
/* reserved value because of definition in header:    0xFFFFFFFCU */
/* valid transmit message handle:   0x0 to kCanNumberOfTxObjects   */

/* return values */ 
#define kCanHlFinishRx                                     ((vuint8)0x00)
#define kCanHlContinueRx                                   ((vuint8)0x01)

#define  CANHL_TX_QUEUE_BIT
/* Define chiphardware                     */
/* Constants concerning can chip registers */
/* control registers for msg objects       */
#if !defined( kCanISRPrio_0 ) /* compatibility for 1 channel exception */
# if defined( kCanISRPrio )   /* old expresson used by CANGen for single channel system */
# define kCanISRPrio_0 kCanISRPrio
# endif
#endif
#if !defined( kCanISRGroup_0 ) /* compatibility for 1 channel exception */
# if defined( kCanISRGroup )   /* old expresson used by CANGen for single channel system */
# define kCanISRGroup_0 kCanISRGroup
# endif
#endif
/* define unused node-priorities (fill table) */
#if !defined( kCanISRPrio_0 )
# define kCanISRPrio_0 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_1 )
# define kCanISRPrio_1 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_2 )
# define kCanISRPrio_2 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_3 )
# define kCanISRPrio_3 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_4 )
# define kCanISRPrio_4 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_5 )
# define kCanISRPrio_5 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_6 )
# define kCanISRPrio_6 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRPrio_7 )
# define kCanISRPrio_7 0x00  /* unused node - no prio available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_0 )
# define kCanISRGroup_0 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_1 )
# define kCanISRGroup_1 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_2 )
# define kCanISRGroup_2 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_3 )
# define kCanISRGroup_3 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_4 )
# define kCanISRGroup_4 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_5 )
# define kCanISRGroup_5 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_6 )
# define kCanISRGroup_6 0x00  /* unused node - no group available (first is always available - see above) */
#endif
#if !defined( kCanISRGroup_7 )
# define kCanISRGroup_7 0x00  /* unused node - no group available (first is always available - see above) */
#endif

/* no compiler abstraction here pointers are always 32 bit and cast does not work for Tasking compiler */
#define CANLL_CELL_PTR       ((tCanCellStructPtr) kCanBaseAdr)
#define CANLL_SRC_OFFSET     (15)
#define CANLL_TRUE           ((vuint8)0x01)
#define CANLL_FALSE          ((vuint8)0x00)
/* for nodes */
#define C_NCR_INIT           ((vuint16)0x0001)   /* Offline(stop) and busoff information */
#define C_NCR_TxRxIE         ((vuint16)0x0002)   /* enable Tx,Rx interrupts */
#define C_NCR_ErrIE          ((vuint16)0x0008)   /* enable Error interrupt */
#define C_NCR_IE             ((vuint16)0x000E)   /* enable all interrupt */
#define C_NCR_CCE            ((vuint16)0x0040)   /* bittiming, ports, errorregs can be written */
#define C_NSR_LEC            ((vuint16)0x0003)   /* error code */
#define C_NSR_TXOK           ((vuint16)0x0008)   /* tx is done */
#define C_NSR_RXOK           ((vuint16)0x0010)   /* rx occure */
#define C_NSR_ALERT          ((vuint16)0x0020)   /* boff, ewarn, list err, init set ... warning about */
#define C_NSR_EWRN           ((vuint16)0x0040)   /* Warninglevel reached */
#define C_NSR_BOFF           ((vuint16)0x0080)   /* Busoff detected */
/* Object Control Bits */
#define C_MOCTR_RXPND        ((vuint16)0x0001)   /* Pending Rx */
#define C_MOCTR_TXPND        ((vuint16)0x0002)   /* pending Tx */
#define C_MOCTR_RXUPD        ((vuint16)0x0004)   /* Rx updated */
#define C_MOCTR_NEWDAT       ((vuint16)0x0008)   /* New Data in MsgObject */
#define C_MOCTR_MSGLOST      ((vuint16)0x0010)   /* 2. NEWDAT appears (used for Overrun notification) */
#define C_MOCTR_MSGVAL       ((vuint16)0x0020)   /* MsgObject contains valid data */
#define C_MOCTR_RTSEL        ((vuint16)0x0040)   /* Rx/Tx selected */
#define C_MOCTR_RXEN         ((vuint16)0x0080)   /* ready for rx */
#define C_MOCTR_TXREQ        ((vuint16)0x0100)   /* request tx */
#define C_MOCTR_TXEN0        ((vuint16)0x0200)   /* user enable tx */
#define C_MOCTR_TXEN1        ((vuint16)0x0400)   /* hw enable tx */
#define C_MOCTR_DIR          ((vuint16)0x0800)   /* direction tx=1 or rx=0 */
#define C_MOCTR_ALL          ((vuint16)0x0FFF)

#define C_MOFCR_MODE_NORMAL  (0x00)  /* Standard Message Object (Tx or Rx) */
#define C_MOFCR_MODE_RXBASE  (0x01)  /* Fifo Base Object for Rx */
#define C_MOFCR_IE_RX        (0x01)  /* Enable Rx-Interrupt for this Object */
#define C_MOFCR_IE_TX        (0x02)  /* Enable Tx-Interrupt for this Object */
/* Overrun Interrupt only usefull when using different Interrupts */
#define C_MOFCR_IE_OVR       (0x04)  /* Enable Overrun-Interrupt for this Object */
#define C_MOFCR_IE_SDT       (0x40)  /* Single data transfer enable */
#define C_MOFCR_IE_STT       (0x80)  /* Single transmit enable */
#define C_MOFCR_DLC          C_DLC_M         /* Mask for DLC */

#define C_MOAMRHI_CLOSE      ((vuint16)0x1FFFU) /* exact match */
#define C_MOAMRHI_OPEN       ((vuint16)0x0000U) /* all match */
#define C_MOAMRLO_CLOSE      ((vuint16)0xFFFFU) /* exact match */
#define C_MOAMRLO_OPEN       ((vuint16)0x0000U) /* all match */
/* attention for Workaround TC.21 the Prio must be higher than for dummy */
/* for Workaround TC.23 Prio have to be equal over all channel and objects */
#define C_MOARHI_PRIO_DUMMY  ((vuint16)0x4000)
/* may cause problems Errata TC.23 because of Tx filtering */
/* general */
#define C_ICR_IE_MASK        (0x00001000UL)
#define C_ICR_CR_INIT        (0x00005000UL) /* clear pending and enable this service */
#define C_ICR_CLR_IPND       (0x00004000UL) /* clear pending request */
#define C_ICR_IPND           (0x00002000UL) /* pending mask */

#define C_PAN_BUSY           ((vuint16)0x0100)
#define C_PAN_ALLOC_TO_LIST  ((vuint16)0x0002)
#define C_PAN_RESET_LIST     ((vuint16)0x0001)

#define C_CLC_IS_DISABLE     ((vuint16)0x0002)
#define C_CLC_DISABLE        ((vuint16)0x0001)
#define C_CLC_START          ((vuint16)0x0008)


/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

#if !(defined( C_HL_DISABLE_RX_INFO_STRUCT_PTR ) || defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR ))
# define C_HL_ENABLE_RX_INFO_STRUCT_PTR
#endif

#if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
# define CAN_HL_P_RX_INFO_STRUCT(channel)                  (pCanRxInfoStruct)  
# define CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)           (pCanRxInfoStruct->Handle)  
#else
# define CAN_HL_P_RX_INFO_STRUCT(channel)                  (&canRxInfoStruct[channel])
# define CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)           (canRxInfoStruct[channel].Handle)  
#endif 


/*disabled - lint -emacro( (572,778), C_RANGE_MATCH) */


#if defined( C_SINGLE_RECEIVE_CHANNEL )
# if (kCanNumberOfUsedCanRxIdTables == 1)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  (  ((idRaw0) & (tCanRxId0)~MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  (  ((idRaw0) & (tCanRxId0)~MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 2)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 3)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDSTD2(mask)) == MK_RX_RANGE_CODE_IDSTD2(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDEXT2(mask)) == MK_RX_RANGE_CODE_IDEXT2(code) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 4)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDSTD2(mask)) == MK_RX_RANGE_CODE_IDSTD2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDSTD3(mask)) == MK_RX_RANGE_CODE_IDSTD3(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDEXT2(mask)) == MK_RX_RANGE_CODE_IDEXT2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDEXT3(mask)) == MK_RX_RANGE_CODE_IDEXT3(code) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 5)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDSTD2(mask)) == MK_RX_RANGE_CODE_IDSTD2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDSTD3(mask)) == MK_RX_RANGE_CODE_IDSTD3(code) ) && \
                    ( ((idRaw4) & (tCanRxId4)~ MK_RX_RANGE_MASK_IDSTD4(mask)) == MK_RX_RANGE_CODE_IDSTD4(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDEXT2(mask)) == MK_RX_RANGE_CODE_IDEXT2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDEXT3(mask)) == MK_RX_RANGE_CODE_IDEXT3(code) ) && \
                    ( ((idRaw4) & (tCanRxId4)~ MK_RX_RANGE_MASK_IDEXT4(mask)) == MK_RX_RANGE_CODE_IDEXT4(code) ) )
# endif
#else     /* C_MULTIPLE_RECEIVE_CHANNEL */

# if (kCanNumberOfUsedCanRxIdTables == 1)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                (  ((idRaw0) & (tCanRxId0)~((mask).Id0)) == ((code).Id0) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 2)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~((mask).Id0)) == ((code).Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~((mask).Id1)) == ((code).Id1) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 3)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~((mask).Id0)) == ((code).Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~((mask).Id1)) == ((code).Id1) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~((mask).Id2)) == ((code).Id2) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 4)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~((mask).Id0)) == ((code).Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~((mask).Id1)) == ((code).Id1) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~((mask).Id2)) == ((code).Id2) ) &&\
                                  ( ((idRaw3) & (tCanRxId3)~((mask).Id3)) == ((code).Id3) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 5)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~((mask).Id0)) == ((code).Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~((mask).Id1)) == ((code).Id1) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~((mask).Id2)) == ((code).Id2) ) &&\
                                  ( ((idRaw3) & (tCanRxId3)~((mask).Id3)) == ((code).Id3) ) &&\
                                  ( ((idRaw4) & (tCanRxId4)~((mask).Id4)) == ((code).Id4) ) )
# endif
#endif


#if (kCanNumberOfUsedCanRxIdTables == 1)
# define CAN_RX_IDRAW_PARA                                 idRaw0
#endif
#if (kCanNumberOfUsedCanRxIdTables == 2)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1
#endif
#if (kCanNumberOfUsedCanRxIdTables == 3)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1,idRaw2
#endif
#if (kCanNumberOfUsedCanRxIdTables == 4)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1,idRaw2,idRaw3
#endif
#if (kCanNumberOfUsedCanRxIdTables == 5)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1,idRaw2,idRaw3,idRaw4
#endif


#if defined( C_SINGLE_RECEIVE_CHANNEL )
# define channel                                           ((CanChannelHandle)0)
# define canHwChannel                                      ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STARTINDEX(channel)             ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STOPINDEX(channel)              ((CanChannelHandle)0)
# define CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel)        (kCanMsgTransmitObj)
# define CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)           (kCanHwTxNormalIndex)

/* Offset which has to be added to change the hardware Tx handle into a logical handle, which is unique over all channels */
/*        Tx-Hardware-Handle - CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) */
# define CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)          ((vsintx)0-kCanHwTxStartIndex)

# define CAN_HL_TX_STARTINDEX(channel)                     ((CanTransmitHandle)0)
# define CAN_HL_TX_STAT_STARTINDEX(channel)                ((CanTransmitHandle)0)
# define CAN_HL_TX_DYN_ROM_STARTINDEX(channel)             (kCanNumberOfTxStatObjects)
# define CAN_HL_TX_DYN_RAM_STARTINDEX(channel)             ((CanTransmitHandle)0)
/* # define CAN_HL_RX_STARTINDEX(channel)                     ((CanReceiveHandle)0) */
/* index to access the ID tables - Basic index only for linear search 
   for hash search this is the start index of the ??? */
# define CAN_HL_RX_BASIC_STARTINDEX(channel)               ((CanReceiveHandle)0)
# if defined( C_SEARCH_HASH ) 
#  define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)          ((CanReceiveHandle)0)
# else
#  define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)          (kCanNumberOfRxBasicCANObjects)
# endif
# define CAN_HL_INIT_OBJ_STARTINDEX(channel)               ((vuint8)0)
# define CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel)         ((CanObjectHandle)0)
# define CAN_HL_HW_TX_STARTINDEX(canHwChannel)             (kCanHwTxStartIndex)
# define CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)        (kCanHwRxFullStartIndex)
# define CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel)       (kCanHwRxBasicStartIndex)
# define CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel)         (kCanHwUnusedStartIndex)

# define CAN_HL_TX_STOPINDEX(channel)                      (kCanNumberOfTxObjects)
# define CAN_HL_TX_STAT_STOPINDEX(channel)                 (kCanNumberOfTxStatObjects)
# define CAN_HL_TX_DYN_ROM_STOPINDEX(channel)              (kCanNumberOfTxObjects)
# define CAN_HL_TX_DYN_RAM_STOPINDEX(channel)              (kCanNumberOfTxDynObjects)
/* # define CAN_HL_RX_STOPINDEX(channel)                      (kCanNumberOfRxObjects) */
# define CAN_HL_RX_BASIC_STOPINDEX(channel)                (kCanNumberOfRxBasicCANObjects)
# if defined( C_SEARCH_HASH ) 
#  define CAN_HL_RX_FULL_STOPINDEX(canHwChannel)           (kCanNumberOfRxFullCANObjects)
# else
#  define CAN_HL_RX_FULL_STOPINDEX(canHwChannel)           (kCanNumberOfRxBasicCANObjects+kCanNumberOfRxFullCANObjects)
# endif
# define CAN_HL_INIT_OBJ_STOPINDEX(channel)                (kCanNumberOfInitObjects)
# define CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel)          (kCanNumberOfUsedTxCANObjects)
# define CAN_HL_HW_TX_STOPINDEX(canHwChannel)              (kCanHwTxStartIndex     +kCanNumberOfUsedTxCANObjects)
# define CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel)         (kCanHwRxFullStartIndex +kCanNumberOfRxFullCANObjects)
# define CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel)        (kCanHwRxBasicStartIndex+kCanNumberOfUsedRxBasicCANObjects)
# define CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel)          (kCanHwUnusedStartIndex +kCanNumberOfUnusedObjects)

#else
#  define canHwChannel                                     channel   /*brackets are not allowed here due to compiler error with Renesas HEW compiler for SH2*/
#  define CAN_HL_HW_CHANNEL_STARTINDEX(channel)            (channel)
#  define CAN_HL_HW_CHANNEL_STOPINDEX(channel)             (channel)

#  define CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel)       (CanHwMsgTransmitIndex[(canHwChannel)])
#  define CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)          (CanHwTxNormalIndex[(canHwChannel)])
/* Offset which has to be added to change the hardware Tx handle into a logical handle, which is unique over all channels */
/*        Tx-Hardware-Handle - CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) */
#  define CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)         (CanTxOffsetHwToLog[(canHwChannel)])

# define CAN_HL_TX_STARTINDEX(channel)                     (CanTxStartIndex[(channel)])
# define CAN_HL_TX_STAT_STARTINDEX(channel)                (CanTxStartIndex[(channel)])
# define CAN_HL_TX_DYN_ROM_STARTINDEX(channel)             (CanTxDynRomStartIndex[(channel)])
# define CAN_HL_TX_DYN_RAM_STARTINDEX(channel)             (CanTxDynRamStartIndex[(channel)])
/* # define CAN_HL_RX_STARTINDEX(channel)                     (CanRxStartIndex[(channel)]) */
/* index to access the ID tables - Basic index only for linear search */
# define CAN_HL_RX_BASIC_STARTINDEX(channel)               (CanRxBasicStartIndex[(channel)])      
# define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)           (CanRxFullStartIndex[(canHwChannel)])
# define CAN_HL_INIT_OBJ_STARTINDEX(channel)               (CanInitObjectStartIndex[(channel)])      
# define CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel)         (CanLogHwTxStartIndex[(canHwChannel)])
#  define CAN_HL_HW_TX_STARTINDEX(canHwChannel)             (CanHwTxStartIndex[(canHwChannel)])   
#  define CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)        (CanHwRxFullStartIndex[(canHwChannel)])
#  define CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel)       (CanHwRxBasicStartIndex[(canHwChannel)]) 
#  define CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel)         (CanHwUnusedStartIndex[(canHwChannel)])
                                                           
# define CAN_HL_TX_STOPINDEX(channel)                      (CanTxStartIndex[(channel) + 1]) 
# define CAN_HL_TX_STAT_STOPINDEX(channel)                 (CanTxDynRomStartIndex[(channel)])
# define CAN_HL_TX_DYN_ROM_STOPINDEX(channel)              (CanTxStartIndex[(channel) + 1]) 
# define CAN_HL_TX_DYN_RAM_STOPINDEX(channel)              (CanTxDynRamStartIndex[(channel) + 1])
/* # define CAN_HL_RX_STOPINDEX(channel)                      (CanRxStartIndex[(channel) + 1]) */
/* index to access the ID tables - Basic index only for linear search */
# define CAN_HL_RX_BASIC_STOPINDEX(channel)                (CanRxFullStartIndex[CAN_HL_HW_CHANNEL_STARTINDEX(channel)])
# define CAN_HL_INIT_OBJ_STOPINDEX(channel)                (CanInitObjectStartIndex[(channel) + 1])
# define CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel)          (CanLogHwTxStartIndex[(canHwChannel) +1])      
#  define CAN_HL_HW_TX_STOPINDEX(canHwChannel)              (CanHwTxStopIndex[(canHwChannel)])
#  define CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel)         (CanHwRxFullStopIndex[(canHwChannel)])
#  define CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel)         (CanHwRxBasicStopIndex[(canHwChannel)])
#  define CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel)         (CanHwUnusedStopIndex[(canHwChannel)])

#endif


#if defined( C_SINGLE_RECEIVE_CHANNEL )

# define CANRANGE0ACCMASK(i)                               C_RANGE0_ACC_MASK
# define CANRANGE0ACCCODE(i)                               C_RANGE0_ACC_CODE
# define CANRANGE1ACCMASK(i)                               C_RANGE1_ACC_MASK
# define CANRANGE1ACCCODE(i)                               C_RANGE1_ACC_CODE
# define CANRANGE2ACCMASK(i)                               C_RANGE2_ACC_MASK
# define CANRANGE2ACCCODE(i)                               C_RANGE2_ACC_CODE
# define CANRANGE3ACCMASK(i)                               C_RANGE3_ACC_MASK
# define CANRANGE3ACCCODE(i)                               C_RANGE3_ACC_CODE

# define APPL_CAN_MSG_RECEIVED( i )                        (APPL_CAN_MSGRECEIVED( i ))

# define APPLCANRANGE0PRECOPY( i )                         (ApplCanRange0Precopy( i ))   
# define APPLCANRANGE1PRECOPY( i )                         (ApplCanRange1Precopy( i ))   
# define APPLCANRANGE2PRECOPY( i )                         (ApplCanRange2Precopy( i ))   
# define APPLCANRANGE3PRECOPY( i )                         (ApplCanRange3Precopy( i ))   

# define APPL_CAN_BUSOFF( i )                              (ApplCanBusOff())
# define APPL_CAN_WAKEUP( i )                              (ApplCanWakeUp())

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
#  define APPLCANCANCELNOTIFICATION( i, j )                (APPL_CAN_CANCELNOTIFICATION( j ))
# else
#  define APPLCANCANCELNOTIFICATION( i, j )
# endif
# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#  define APPLCANMSGCANCELNOTIFICATION( i )                (APPL_CAN_MSGCANCELNOTIFICATION())
# else
#  define APPLCANMSGCANCELNOTIFICATION( i )
# endif

# define CAN_RX_INDEX_TBL(channel,id)                      (CanRxIndexTbl[id])

#else

# define CANRANGE0ACCMASK(i)                               (CanChannelObject[i].RangeMask[0])
# define CANRANGE0ACCCODE(i)                               (CanChannelObject[i].RangeCode[0])
# define CANRANGE1ACCMASK(i)                               (CanChannelObject[i].RangeMask[1])
# define CANRANGE1ACCCODE(i)                               (CanChannelObject[i].RangeCode[1])
# define CANRANGE2ACCMASK(i)                               (CanChannelObject[i].RangeMask[2])
# define CANRANGE2ACCCODE(i)                               (CanChannelObject[i].RangeCode[2])
# define CANRANGE3ACCMASK(i)                               (CanChannelObject[i].RangeMask[3])
# define CANRANGE3ACCCODE(i)                               (CanChannelObject[i].RangeCode[3])

/* generated id type of the range */
# define CANRANGE0IDTYPE(i)                                (CanChannelObject[i].RangeIdType[0])
# define CANRANGE1IDTYPE(i)                                (CanChannelObject[i].RangeIdType[1])
# define CANRANGE2IDTYPE(i)                                (CanChannelObject[i].RangeIdType[2])
# define CANRANGE3IDTYPE(i)                                (CanChannelObject[i].RangeIdType[3])

# define APPL_CAN_MSG_RECEIVED( i )                        (CanChannelObject[(i)->Channel].ApplCanMsgReceivedFct(i))

# define APPLCANRANGE0PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[0](i))
# define APPLCANRANGE1PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[1](i))   
# define APPLCANRANGE2PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[2](i))   
# define APPLCANRANGE3PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[3](i))   

# define APPL_CAN_BUSOFF( i )                              (CanChannelObject[i].ApplCanBusOffFct(i))
# define APPL_CAN_WAKEUP( i )                              (CanChannelObject[i].ApplCanWakeUpFct(i))

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
#  define APPLCANCANCELNOTIFICATION( i, j )                (CanChannelObject[i].ApplCanCancelNotificationFct( j ))
# else
#  define APPLCANCANCELNOTIFICATION( i, j )
# endif

# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#  define APPLCANMSGCANCELNOTIFICATION( i )                (CanChannelObject[i].ApplCanMsgTransmitCancelNotifyFct( i ))
# else
#  define APPLCANMSGCANCELNOTIFICATION( i )
# endif

# define CAN_RX_INDEX_TBL(channel,id)                      (CanRxIndexTbl[channel][id])

#endif


#if defined ( C_ENABLE_CAN_CAN_INTERRUPT_CONTROL )
# define CAN_CAN_INTERRUPT_DISABLE(i)                        (CanCanInterruptDisable(i))
# define CAN_CAN_INTERRUPT_RESTORE(i)                        (CanCanInterruptRestore(i))
#else
# define CAN_CAN_INTERRUPT_DISABLE(i)
# define CAN_CAN_INTERRUPT_RESTORE(i)
#endif


#if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* define first index to array CanHwObjIndivPolling[][] */
#   define CAN_HWOBJINDIVPOLLING_INDEX0                    (0)
#endif


/* mask for range enable status */
#define kCanRange0                                         ((vuint16)1)
#define kCanRange1                                         ((vuint16)2)
#define kCanRange2                                         ((vuint16)4)
#define kCanRange3                                         ((vuint16)8)


/* Assertions ----------------------------------------------------------------*/
/*lint -function(exit,ApplCanFatalError)*/

/*lint -emacro( (506), assertUser) */
#if defined( C_ENABLE_USER_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertUser(p,c,e)                                if (!(p))   {ApplCanFatalError(e);}                    /* PRQA S 3412 */
# else
#  define assertUser(p,c,e)                                if (!(p))   {ApplCanFatalError((c),(e));}              /* PRQA S 3412 */
# endif
#else
# define assertUser(p,c,e)
#endif

/*lint -emacro( (506), assertGen) */
#if defined( C_ENABLE_GEN_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertGen(p,c,e)                                 if (!(p))   {ApplCanFatalError(e);}                    /* PRQA S 3412 */
# else
#  define assertGen(p,c,e)                                 if (!(p))   {ApplCanFatalError((c),(e));}              /* PRQA S 3412 */
# endif
#else
# define assertGen(p,c,e)
#endif

/*lint -emacro( (506), assertHardware) */
#if defined( C_ENABLE_HARDWARE_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertHardware(p,c,e)                            if (!(p))   {ApplCanFatalError(e);}                    /* PRQA S 3412 */
# else
#  define assertHardware(p,c,e)                            if (!(p))   {ApplCanFatalError((c),(e));}              /* PRQA S 3412 */
# endif
#else
# define assertHardware(p,c,e)
#endif

/*lint -emacro( (506), assertInternal) */
#if defined( C_ENABLE_INTERNAL_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertInternal(p,c,e)                            if (!(p))   {ApplCanFatalError(e);}                   /* PRQA S 3412 */
# else
#  define assertInternal(p,c,e)                            if (!(p))   {ApplCanFatalError((c),(e));}             /* PRQA S 3412 */
# endif
#else
# define assertInternal(p,c,e)
#endif

#if defined( C_ENABLE_TRANSMIT_QUEUE )
#if defined( C_CPUTYPE_32BIT )
# define kCanTxQueueShift     5
#endif



/* mask used to get the flag index from the handle */
# define kCanTxQueueMask      (((vuint8)1 << kCanTxQueueShift) - (vuint8)1)


#if defined( C_SINGLE_RECEIVE_CHANNEL )
# define CAN_HL_TXQUEUE_PADBITS(channel)             ((CanTransmitHandle)0)
# define CAN_HL_TXQUEUE_STARTINDEX(channel)          ((CanSignedTxHandle)0)
# define CAN_HL_TXQUEUE_STOPINDEX(channel)           ((CanSignedTxHandle)kCanTxQueueSize)
#else
# define CAN_HL_TXQUEUE_PADBITS(channel)             ((CanTransmitHandle)CanTxQueuePadBits[(channel)])
# define CAN_HL_TXQUEUE_STARTINDEX(channel)          (CanTxQueueStartIndex[(channel)])
# define CAN_HL_TXQUEUE_STOPINDEX(channel)           (CanTxQueueStartIndex[(channel) + (CanChannelHandle)1])
#endif


#endif
#define CAN_NODE_NUM CanNodeNum[canHwChannel]
/* list command is in progress wait until !busy */
#if defined( C_ENABLE_HW_LOOP_TIMER )
# define CanLL_WaitForListReady \
  CanLL_ApplCanTimerStart(kCanLoopListInit); /* lint !e722 */\
  do \
  {  \
    if (CanLL_ApplCanTimerLoop(kCanLoopListInit)==0) break; /* lint !e722 */\
  } while ( (CANLL_CELL_PTR->Modul.PanCmd & C_PAN_BUSY)==C_PAN_BUSY );\
  CanLL_ApplCanTimerEnd(kCanLoopListInit)
#else /* C_ENABLE_HW_LOOP_TIMER */
# define CanLL_WaitForListReady \
  while ((CANLL_CELL_PTR->Modul.PanCmd & C_PAN_BUSY)==C_PAN_BUSY) \
  {;}
#endif /* C_ENABLE_HW_LOOP_TIMER */

#define CanLL_SetListCmd(pcell, para2, para1, cmd) \
         (pcell)->Modul.PanPara = (((vuint16)(para2)) << 8) | ((vuint16)(para1)); pcell->Modul.PanCmd = cmd;

#if defined(C_ENABLE_CAN_INTERRUPT_CONTROL)
# if defined ( V_CPU_TRICORE )
#  define CanLL_CanInterruptDisable(canHwChannel, localInterruptOldFlagPtr)        \
            *(localInterruptOldFlagPtr) = CANLL_CELL_PTR->Ctrl.Src[CANLL_SRC_OFFSET-CanSrcNum[canHwChannel]]; \
            CANLL_CELL_PTR->Ctrl.Src[CANLL_SRC_OFFSET-CanSrcNum[canHwChannel]] &= ~(C_ICR_IE_MASK)
#  define CanLL_CanInterruptRestore(canHwChannel, localInterruptOldFlag) \
            CANLL_CELL_PTR->Ctrl.Src[CANLL_SRC_OFFSET-CanSrcNum[canHwChannel]] |= ((localInterruptOldFlag) & C_ICR_IE_MASK)
# endif
#else
# define CanLL_CanInterruptDisable(canHwChannel, localInterruptOldFlagPtr) (*(localInterruptOldFlagPtr) = 0x00) /* avoid warning */
# define CanLL_CanInterruptRestore(canHwChannel, localInterruptOldFlag)
#endif


# define CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) \
         ((CANLL_CELL_PTR->Obj[txObjHandle].MoCtrL & C_MOCTR_NEWDAT) == (vuint16)0x0000)
#if defined( C_ENABLE_SLEEP_WAKEUP ) && defined( V_CPU_TRICORE ) 
# define CanLL_HwIsSleep( CAN_HW_CHANNEL_CANPARA_ONLY ) \
        (canllSleepNotify[canHwChannel]==CANLL_TRUE)
#else
# define CanLL_HwIsSleep( CAN_HW_CHANNEL_CANPARA_ONLY)  (kCanFalse != kCanFalse)  /* avoid misra warning */
#endif


#define CanLL_HwIsStop( CAN_HW_CHANNEL_CANPARA_ONLY ) \
        (canllHalt[canHwChannel]==CANLL_TRUE)

#define CanLL_HwIsBusOff( CAN_HW_CHANNEL_CANPARA_ONLY )  \
        (((CANLL_CELL_PTR->Node[CAN_NODE_NUM].NSR & C_NSR_BOFF)==C_NSR_BOFF) || \
         ((!CanLL_HwIsStop(canHwChannel)) && ((CANLL_CELL_PTR->Node[CAN_NODE_NUM].NCR & C_NCR_INIT)==C_NCR_INIT))) 

#if defined( C_ENABLE_EXTENDED_STATUS )
# define CanLL_HwIsPassive( CAN_HW_CHANNEL_CANPARA_ONLY )  \
         ((CANLL_CELL_PTR->Node[CAN_NODE_NUM].NECnt_Tec > 127) || (CANLL_CELL_PTR->Node[CAN_NODE_NUM].NECnt_Rec > 127))

# define CanLL_HwIsWarning( CAN_HW_CHANNEL_CANPARA_ONLY ) \
         ((CANLL_CELL_PTR->Node[CAN_NODE_NUM].NSR & C_NSR_EWRN)==C_NSR_EWRN)
#endif /* C_ENABLE_EXTENDED_STATUS */

#if defined(C_ENABLE_HWBUFFER)
/* temporary buffer for Rx data in one of last CAN-cell mailboxes */
# define TmpRxMailbox      CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM]
# define pTmpRxMailboxMsg  ((CanChipMsgPtr)    &CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM])
# define pTmpRxMailboxData ((CanChipDataPtr)   &CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM].MoDR[0])
#else
/* temporary buffer for Rx data in RAM */
# define TmpRxMailbox      tmpRxMsgMailbox[canHwChannel]
# define pTmpRxMailboxMsg  ((CanChipMsgPtr)    &tmpRxMsgMailbox[canHwChannel])
# define pTmpRxMailboxData ((CanChipDataPtr)   &tmpRxMsgMailbox[canHwChannel].MoDR[0])
#endif

#if defined(C_MULTIPLE_RECEIVE_CHANNEL)
# define CanLL_ApplCanTimerStart(loop)  ApplCanTimerStart(channel, loop)
# define CanLL_ApplCanTimerEnd(loop)    ApplCanTimerEnd(channel, loop)
# define CanLL_ApplCanTimerLoop(loop)   ApplCanTimerLoop(channel, loop)
#else
# define CanLL_ApplCanTimerStart(loop)  ApplCanTimerStart(loop)
# define CanLL_ApplCanTimerEnd(loop)    ApplCanTimerEnd(loop)
# define CanLL_ApplCanTimerLoop(loop)   ApplCanTimerLoop(loop)
#endif

/***************************************************************************/
/* Defines / data types / structs / unions                                 */
/***************************************************************************/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
#endif
/* Define CAN Chip hardware; segment must be located in locator file    */
/* register layout of the can chip                                      */
/* Structure describing CAN receive buffer. */
typedef volatile struct tCanMsgObj
{
  vuint8  MoFCR_Mode;         /* Fkt. Ctr.: MODE       +0x00 */
  vuint8  MoFCR_Gate;         /* Fkt. Ctr.: Gateway        1 */
  vuint8  MoFCR_IE;           /* Fkt. Ctr.: Interrupts     2 */
  vuint8  MoFCR_DLC;          /* Fkt. Ctr.: DLC            3 */
  vuint32 MoFGPR;             /* Fifo pointer          +0x04 */
  vuint16 MoIPR;              /* Interrupt pointer     +0x08 */
  vuint16 reserved;           /* message counters      +0x0a */
  vuint16 MoAMRLo;            /* Mask                  +0x0C */
  vuint16 MoAMRHi;            /* Mask                      e */
# if defined( C_CPUTYPE_32BIT )
  vuint32 MoDR[2];            /* 2x 32 bit data register     */
# else
  vuint16 MoDR[4];            /* 4x 16 bit data register     */
# endif
  vuint16 MoARLo;             /* arbitration           +0x18 */
  vuint16 MoARHi;             /* arbitration               a */
  vuint16 MoCtrL;             /* control               +0x1c */
  vuint16 MoCtrH;             /* control               +0x1e */
} tCanMsgObjStruct;  /* CAN_MSG_OBJ_PTR */
V_DEF_P2SFR_CAN_TYPE(volatile, tCanMsgObjStruct) CanMsgObjStructPtr;

typedef volatile struct tCanNode
{
  vuint16 NCR;                /* Control               +0x00 */
  vuint16 reserved1;
  vuint16 NSR;                /* Status                +0x04 */
  vuint16 reserved2;
  vuint16 NIPR;               /* Interrupt pointer     +0x08 */
  vuint16 reserved3;
  vuint16 NPCR;               /* port control          +0x0c */
  vuint16 reserved4;
  vuint16 NBTR;               /* Bit timing            +0x10 */
  vuint16 reserved5;          /*  */
  vuint8  NECnt_Rec;          /* Rx Error Counter      +0x14 */
  vuint8  NECnt_Tec;          /* Tx Error Counter          5 */
  vuint8  NECnt_EWRNLVL;      /* ErrorWarningLevel         6 */
  vuint8  reserved6;          /*  */
  vuint16 NFCRL;              /* frame counter         +0x18 (unused) */
  vuint16 NFCRH;              /* frame counter high word */
  vuint8  reserved7[0xE4];    /* up to next Node */
} tCanNodeStruct;  /* CAN_NODE_PTR */
V_DEF_P2SFR_CAN_TYPE(V_NONE, tCanNodeStruct) CanNodeStructPtr;

typedef volatile struct tCanModul
{
  vuint32 List[CAN_MAX_LIST];   /* Obj Lists            +0x100 */
  vuint32 MSPND[CAN_MAX_MSPND]; /* Interrupt Pending    +0x120/+0x140 */
  vuint32 MSID[CAN_MAX_MSID];   /* Msg. idx. Pend.      +0x140/+0x180 */
  vuint32 MSIMask;              /* MSG index Mask       +0x1c0 */
  vuint16 PanCmd;               /* Panel Control Low    +0x1c4 */
  vuint16 PanPara;              /* Panel Control High   +0x1c6 */
  vuint16 MCR;                  /* Module Control       +0x1c8 */
  vuint16 reserved4;
  vuint32 MITR;                 /* Modul Int. Trig.     +0x1cc (unused) */
  vuint8  reserved5[0x30];      /* up to Node */
} tCanModulStruct;  /* CAN_MODUL_CTR_PTR */

typedef volatile struct tCanController
{
  vuint16 CLC;                /* Clock control         +0x00 (only TriCore) */
  vuint8  reserved1[6];       /*  */
  vuint32 ID;                 /* Cell id               +0x08 (ununsed / not all derivatives) */
  vuint32 FDR;                /* Clock Dividor         +0x0c */
  vuint8  reserved2[0xB0];    /*  */
  vuint32 Src[16];            /* Interrupt Service     +0xc0 (amount depend on derivatives) */
} tCanControllerStruct;  /* CAN_CONTROLLER_PTR */

typedef volatile struct tCanCell
{                                                   /* BaseAddress + ... */
  tCanControllerStruct Ctrl;                        /* 0     - 0x100 */
  tCanModulStruct      Modul;                       /* 0x100 - 0x1D0 */
  tCanNodeStruct       Node[CAN_MAX_NODE];          /* 0x200 - x(derivative) / include reserved */
  tCanMsgObjStruct     Obj[CAN_MAX_NUM_MSGOBJ];     /* x - y (derivative) */
} tCanCellStruct; 
V_DEF_P2SFR_CAN_TYPE(V_NONE, tCanCellStruct) tCanCellStructPtr;


/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* ROM CATEGORY 1 START*/
/* lookup table for setting the flags in the queue */
V_MEMROM0 static V_MEMROM1 tCanQueueElementType V_MEMROM2 CanShiftLookUp[1 << kCanTxQueueShift] = 
{
#if defined( C_CPUTYPE_32BIT ) 
  (tCanQueueElementType)0x00000001, (tCanQueueElementType)0x00000002, (tCanQueueElementType)0x00000004, (tCanQueueElementType)0x00000008, 
  (tCanQueueElementType)0x00000010, (tCanQueueElementType)0x00000020, (tCanQueueElementType)0x00000040, (tCanQueueElementType)0x00000080
#endif

#if defined( C_CPUTYPE_32BIT ) 
 ,(tCanQueueElementType)0x00000100, (tCanQueueElementType)0x00000200, (tCanQueueElementType)0x00000400, (tCanQueueElementType)0x00000800, 
  (tCanQueueElementType)0x00001000, (tCanQueueElementType)0x00002000, (tCanQueueElementType)0x00004000, (tCanQueueElementType)0x00008000
#endif

#if defined( C_CPUTYPE_32BIT )
 ,(tCanQueueElementType)0x00010000, (tCanQueueElementType)0x00020000, (tCanQueueElementType)0x00040000, (tCanQueueElementType)0x00080000, 
  (tCanQueueElementType)0x00100000, (tCanQueueElementType)0x00200000, (tCanQueueElementType)0x00400000, (tCanQueueElementType)0x00800000,
  (tCanQueueElementType)0x01000000, (tCanQueueElementType)0x02000000, (tCanQueueElementType)0x04000000, (tCanQueueElementType)0x08000000, 
  (tCanQueueElementType)0x10000000, (tCanQueueElementType)0x20000000, (tCanQueueElementType)0x40000000, (tCanQueueElementType)0x80000000
#endif
};

/* returns the highest pending flag from the lower nibble */
V_MEMROM0 static V_MEMROM1 vsint8 V_MEMROM2 CanGetHighestFlagFromNibble[16] =        /* PRQA S 3218 */ /* Misra rule 8.7: only accessed in one function. - depends on configuration */
{    
  (vsint8)-1,                /* (vsint8)0xFF - changed due to misra ; cast due to R32C */
  0x00,
  0x01, 0x01,
  0x02, 0x02, 0x02, 0x02,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03
};
/* ROM CATEGORY 1 END*/
#endif
/* Global constants with CAN driver main and subversion */
/* ROM CATEGORY 4 START */
V_DEF_CONST(V_NONE, vuint8, CONST) kCanMainVersion   = (vuint8)(( DRVCAN_TRICOREMULTICANHLL_VERSION & 0xFF00 ) >> 8 );  /* lint !e572 !e778 */
V_DEF_CONST(V_NONE, vuint8, CONST) kCanSubVersion    = (vuint8)(( DRVCAN_TRICOREMULTICANHLL_VERSION ) & 0x00FF );
V_DEF_CONST(V_NONE, vuint8, CONST) kCanBugFixVersion = (vuint8)(  DRVCAN_TRICOREMULTICANHLL_RELEASE_VERSION );
/* ROM CATEGORY 4 END */

#if defined( C_ENABLE_CAN_RAM_CHECK )
/* ROM CATEGORY 4 START */
V_DEF_CONST(STATIC, vuint8, CONST) CanMemCheckValues8bit[3] = 
{
  0xAA, 0x55, 0x00
};
V_DEF_CONST(STATIC, vuint16, CONST) CanMemCheckValues16bit[3] = 
{
  0xAAAA, 0x5555, 0x0000
};
# if defined(C_CPUTYPE_32BIT)
V_DEF_CONST(STATIC, vuint32, CONST) CanMemCheckValues32bit[3] = 
{
  0xAAAAAAAAUL, 0x55555555UL, 0x00000000UL
};
/* ROM CATEGORY 4 END */
# endif
#endif

/***************************************************************************/
/* CAN-Hardware Data Definitions                                            */
/***************************************************************************/


/***************************************************************************/
/* external declarations                                                    */
/***************************************************************************/

#if !defined( CANDRV_SET_CODE_TEST_POINT )
# define CANDRV_SET_CODE_TEST_POINT(x)
#else
extern vuint8 tscCTKTestPointState[CTK_MAX_TEST_POINT];       /* PRQA S 3447 */
#endif

/***************************************************************************/
/* global data definitions                                                 */
/***************************************************************************/

/* RAM CATEGORY 1 START*/
volatile CanReceiveHandle canRxHandle[kCanNumberOfChannels];        /* PRQA S 1514 */
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 3 START*/
#if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )   && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
CanTransmitHandle          confirmHandle[kCanNumberOfChannels];
#endif
/* RAM CATEGORY 3 END*/

/* RAM CATEGORY 1 START*/
#if defined( C_ENABLE_CONFIRMATION_FLAG )
/* Msg(4:0750) A union type has been used. MISRA Rules 110 - no change */
V_MEMRAM0 volatile V_MEMRAM1_NEAR union CanConfirmationBits V_MEMRAM2_NEAR CanConfirmationFlags;       /* PRQA S 0759 */
#endif

#if defined( C_ENABLE_INDICATION_FLAG )
/* Msg(4:0750) A union type has been used. MISRA Rules 110 - no change */
V_MEMRAM0 volatile V_MEMRAM1_NEAR union CanIndicationBits   V_MEMRAM2_NEAR CanIndicationFlags;         /* PRQA S 0759 */
#endif
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 1 START*/
#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/* ##RI1.4 - 3.31: Dynamic Receive DLC */
volatile vuint8 canVariableRxDataLen[kCanNumberOfRxObjects];
#endif
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 1 START*/
CanChipDataPtr canRDSRxPtr[kCanNumberOfChannels];                  /* PRQA S 1514 */
/* RAM CATEGORY 1 END*/
/* RAM CATEGORY 1 START*/
CanChipDataPtr canRDSTxPtr[kCanNumberOfUsedTxCANObjects];          /* PRQA S 1514 */
/* RAM CATEGORY 1 END*/


/***************************************************************************/
/* local data definitions                                                  */
/***************************************************************************/

/* support for CAN driver features : */
/* RAM CATEGORY 1 START*/
static volatile CanTransmitHandle canHandleCurTxObj[kCanNumberOfUsedTxCANObjects];
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 2 START*/
#if defined( C_ENABLE_ECU_SWITCH_PASS )
static vuint8 canPassive[kCanNumberOfChannels];
#endif
/* RAM CATEGORY 2 END*/

/* RAM CATEGORY 2 START*/
#if defined( C_ENABLE_CAN_RAM_CHECK )
static vuint8 canComStatus[kCanNumberOfChannels]; /* stores the decision of the App after the last CAN RAM check */
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )
static volatile vuint8 canTxDynObjReservedFlag[kCanNumberOfTxDynObjects];

# if defined( C_ENABLE_DYN_TX_ID )
static tCanTxId0 canDynTxId0[kCanNumberOfTxDynObjects];
#  if (kCanNumberOfUsedCanTxIdTables > 1)
static tCanTxId1 canDynTxId1[kCanNumberOfTxDynObjects];
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 2)
static tCanTxId2 canDynTxId2[kCanNumberOfTxDynObjects];
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 3)
static tCanTxId3 canDynTxId3[kCanNumberOfTxDynObjects];
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 4)
static tCanTxId4 canDynTxId4[kCanNumberOfTxDynObjects];
#  endif
#  if defined( C_ENABLE_MIXED_ID )
#   if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#   else
static tCanIdType                 canDynTxIdType[kCanNumberOfTxDynObjects];
#   endif
#  endif
# endif

# if defined( C_ENABLE_DYN_TX_DLC )
static vuint8                   canDynTxDLC[kCanNumberOfTxDynObjects];
# endif
# if defined( C_ENABLE_DYN_TX_DATAPTR )
static vuint8*                  canDynTxDataPtr[kCanNumberOfTxDynObjects];
# endif
# if defined( C_ENABLE_CONFIRMATION_FCT )
# endif 
#endif /* C_ENABLED_DYN_TX_OBJECTS */


#if defined( C_ENABLE_TX_MASK_EXT_ID )
static tCanTxId0 canTxMask0[kCanNumberOfChannels];
# if (kCanNumberOfUsedCanTxIdTables > 1)
static tCanTxId1 canTxMask1[kCanNumberOfChannels];
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
static tCanTxId2 canTxMask2[kCanNumberOfChannels];
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
static tCanTxId3 canTxMask3[kCanNumberOfChannels];
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
static tCanTxId4 canTxMask4[kCanNumberOfChannels];
# endif
#endif

#if defined( C_ENABLE_VARIABLE_DLC )
static vuint8 canTxDLC_RAM[kCanNumberOfTxObjects];
#endif

#if defined( C_HL_ENABLE_COPROCESSOR_SUPPORT )
#else
static volatile vuint8 canStatus[kCanNumberOfChannels];

# if defined( C_ENABLE_PART_OFFLINE )
static vuint8 canTxPartOffline[kCanNumberOfChannels];
# endif
#endif
/* RAM CATEGORY 2 END*/

/* RAM CATEGORY 1 START*/
static vsintx          canCanInterruptCounter[kCanNumberOfChannels];
#if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )
static tCanLLCanIntOld canCanInterruptOldStatus[kCanNumberOfHwChannels];
#endif
/* RAM CATEGORY 1 END*/

#if defined( C_HL_ENABLE_LAST_INIT_OBJ )
/* RAM CATEGORY 4 START*/
static CanInitHandle lastInitObject[kCanNumberOfChannels];
/* RAM CATEGORY 4 END*/
#endif
#if defined( C_ENABLE_TX_POLLING )          || \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING )       
/* RAM CATEGORY 2 START*/
static vuint8 canPollingTaskActive[kCanNumberOfChannels];
/* RAM CATEGORY 2 END*/
#endif

/* RAM CATEGORY 1 START*/
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )  || \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
static tCanRxInfoStruct        canRxInfoStruct[kCanNumberOfChannels];
#endif
#if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
static tCanTxConfInfoStruct    txInfoStructConf[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
static volatile vuint8 canMsgCondRecState[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_RX_QUEUE )
static tCanRxQueue canRxQueue;         /* the rx queue (buffer and queue variables) */
#endif
/* RAM CATEGORY 1 END*/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* RAM CATEGORY 1 START*/
static volatile tCanQueueElementType canTxQueueFlags[kCanTxQueueSize];
/* RAM CATEGORY 1 END*/
#endif
#if defined( C_ENABLE_CAN_RAM_CHECK )
#endif

/* RAM CATEGORY 1 START */
V_DEF_VAR(STATIC, vuint8, VAR_NOINIT) canllHalt[kCanNumberOfChannels];
V_DEF_VAR(STATIC, vuint8, VAR_NOINIT) canllBusOffNotified[kCanNumberOfChannels];
# if !defined(C_ENABLE_HWBUFFER)
/* temporary buffer for Rx data in RAM */
V_DEF_VAR(STATIC, tCanMsgObjStruct, VAR_NOINIT) tmpRxMsgMailbox[kCanNumberOfChannels];
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP ) &&  defined( V_CPU_TRICORE )
V_DEF_VAR(STATIC, vuint8, VAR_NOINIT) canllSleepNotify[kCanNumberOfChannels];
# endif
/* RAM CATEGORY 1 END */

/***************************************************************************/
/*  local prototypes                                                       */
/***************************************************************************/
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )  || \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
# if defined( C_ENABLE_RX_QUEUE )
/* CODE CATEGORY 1 START*/
static vuint8 CanHL_ReceivedRxHandleQueue(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 1 END*/
# endif
/* CODE CATEGORY 1 START*/
# if ( kCanNumberOfRxObjects > 0 )
#  if defined( C_ENABLE_RX_QUEUE )
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_FIRST tCanRxInfoStruct *pCanRxInfoStruct );
#  else
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_ONLY );
#  endif
# endif /* ( kCanNumberOfRxObjects > 0 )*/ 
/* CODE CATEGORY 1 END*/
# if defined( C_ENABLE_INDICATION_FLAG ) || \
     defined( C_ENABLE_INDICATION_FCT )
/* CODE CATEGORY 1 START*/
static void CanHL_IndRxHandle( CanReceiveHandle rxHandle );
/* CODE CATEGORY 1 END*/
# endif
#endif
#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 1 START*/
static void CanBasicCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);  
/* CODE CATEGORY 1 END*/
#endif
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
/* CODE CATEGORY 1 START*/
static void CanFullCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
/* CODE CATEGORY 1 END*/
#endif

/* CODE CATEGORY 1 START*/
static void CanHL_TxConfirmation(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);
/* CODE CATEGORY 1 END*/
# if defined( C_ENABLE_CAN_TRANSMIT )
/* CODE CATEGORY 1 START*/
static vuint8 CanCopyDataAndStartTransmission(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle, CanTransmitHandle txHandle)  C_API_3;   /*lint !e14 !e31*/
/* CODE CATEGORY 1 END*/
# endif   /* C_ENABLE_CAN_TRANSMIT */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* CODE CATEGORY 4 START*/
static void CanDelQueuedObj( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 4 END*/
# if defined( C_ENABLE_TX_POLLING ) 
/* CODE CATEGORY 2 START*/
static void CanHl_RestartTxQueue( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/
# endif
#endif 

/* CODE CATEGORY 2 START*/
static void CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/* CODE CATEGORY 1 START*/
static void CanSetVariableRxDatalen ( CanReceiveHandle rxHandle, vuint8 dataLen );
/* CODE CATEGORY 1 END*/
#endif


#if defined( C_ENABLE_CAN_RAM_CHECK )
/* CODE CATEGORY 4 START */
V_DEF_FUNC(STATIC, vuint8, CODE) CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle);
/* CODE CATEGORY 4 END */
#endif
# if defined( C_LL_INTERRUPT_ISR_ACTIVE ) /* ! pure polling configuration */
/* CODE CATEGORY 1 START */
V_DEF_FUNC(STATIC, void, CODE) CanInterrupt( CAN_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END */
# endif

/***************************************************************************/
/*  Error Check                                                            */
/***************************************************************************/

/***************** error check for Organi process **************************/







/***************** error check for not supported feature  **************************/




#if defined( C_ENABLE_COMMON_CAN )
# error "Common CAN is not supported with this CAN driver implementation"
#endif

#if defined( C_ENABLE_MULTI_ECU_CONFIG )
# error "Multiple Configuration is not supported with this CAN driver implementation"
#endif

#if defined( ERROR_C_ENABLE_CAN_RAM_CHECK_EXTENDED )
# error "Extended RAM Check is not supported with this CAN driver implementation"
#endif

#if (VSTDLIB__COREHLL_VERSION  <  0x0213 )
# error "Incompatible version of VStdLib. Core Version 2.13.00 or higher is necessary."
#endif


#if (kCanNumberOfHwChannels > 255)
#  error "Too many CAN channels. This driver can only handle up to 255 channels"
#endif

#if defined( C_SEARCH_HASH )
# if !defined( kHashSearchListCountEx )
#  error "kHashSearchListCountEx not defined"
# endif
# if !defined( kHashSearchMaxStepsEx )
#  error "kHashSearchMaxStepsEx not defined"
# endif
# if !defined( kHashSearchListCount )
#  error "kHashSearchListCount not defined"
# endif
# if !defined( kHashSearchMaxSteps )
#  error "kHashSearchMaxSteps not defined"
# endif
# if ( (kHashSearchMaxStepsEx < 1) ||(kHashSearchMaxStepsEx > 32768) )
#  error "kHashSearchMaxStepsEx has ilegal value"
# endif
# if ( kHashSearchListCountEx > 32768 )
#  error "Hash table for extended ID is too large"
# endif
# if ( (kHashSearchMaxSteps < 1) ||(kHashSearchMaxSteps > 32768) )
#  error "kHashSearchMaxStepsEx has ilegal value"
# endif
# if ( kHashSearchListCount > 32768 )
#  error "Hash table for standard ID is too large"
# endif
# if !defined( C_ENABLE_EXTENDED_ID) && (kHashSearchListCountEx > 0)
#  error "kHashSearchListCountEx has to be 0 in this configuration"
# endif
# if defined( C_ENABLE_EXTENDED_ID) && !defined( C_ENABLE_MIXED_ID) &&(kHashSearchListCount > 0)
#  error "kHashSearchListCount has to be 0 in this configuration"
# endif
#endif





#if defined( C_ENABLE_RX_QUEUE )
# if !defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
#  error "RX-Queue requires C_HL_ENABLE_RX_INFO_STRUCT_PTR"
# endif
#endif




#if defined ( CAN_POLLING_IRQ_DISABLE ) || defined ( CAN_POLLING_IRQ_RESTORE )
# error "Feature has changed - use C_DISABLE_CAN_CAN_INTERRUPT_CONTROL to remove the CAN interrupt disabling"
#endif

#if !defined(V_ACTIVE_IDENTITY_LOG)
#  define V_ACTIVE_IDENTITY_LOG 0
#endif
#if defined(C_ENABLE_WORKAROUND_TC21) && defined(C_ENABLE_HWBUFFER)
# error "Workaround TC.21 and HW-buffer enabled:  This is not allowed, see documentation"
#endif
#if kCanNumberOfPhysChannels > CAN_MAX_NUM_NODE
# error "Too much hardware channels used (derivative does not support this amount) !"
#endif
#if (kCanNumberOfChannels > CAN_MAX_NUM_NODE)
# error "Too much nodes selected (see maximum amount of nodes available for this derivative) !"
#endif

#if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#else
#error "Unknown Compiler switch selected"
#endif
#if defined( V_CPU_TRICORE ) 
#else
#error "Unknown CPU switch selected"
#endif

#if !defined(kCanPhysToLogChannelIndex_0) && !defined(kCanPhysToLogChannelIndex_1) && \
    !defined(kCanPhysToLogChannelIndex_2) && !defined(kCanPhysToLogChannelIndex_3)
# error "no physical node generated"
#endif

#if !defined(kCanBaseAdr)
# error "kCanBaseAdr is not defined!"
#endif



/***************************************************************************/
/*  Functions                                                              */
/***************************************************************************/


# if defined( C_ENABLE_MEMCOPY_SUPPORT )
/* **************************************************************************
| NAME:             CanCopyFromCan
| CALLED BY:        Application
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: void *             dst        | pointer to destionation buffer
|                   CanChipDataPtr     src        | pointer to CAN buffer
|                   vuint8             len        | number of bytes to copy
|
| RETURN VALUES:    -
|
| DESCRIPTION:      copy data from CAN receive buffer to RAM.
|
| ATTENTION:        
************************************************************************** */
/* Msg(4:3673) The object addressed by the pointer "src" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
/* CODE CATEGORY 1 START */
V_DEF_FUNC_API(V_NONE, void, CODE) CanCopyFromCan(V_DEF_P2VAR_PARA(V_NONE, void, AUTOMATIC, APPL_VAR) dst, CanChipDataPtr src, vuint8 len)    /* PRQA S 3673 */      /* suppress message about const pointer */
{
  vuint8 canllidx;
  for(canllidx=0; canllidx<len; canllidx++)
  {
    ((V_DEF_P2VAR_PARA(V_NONE, vuint8, AUTOMATIC, APPL_VAR))dst)[canllidx] = src[canllidx];
  }
}
/* CODE CATEGORY 1 END */

/* **************************************************************************
| NAME:             CanCopyToCan
| CALLED BY:        Application
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: void *             src        | pointer to source buffer
|                   CanChipDataPtr     dst        | pointer to CAN buffer
|                   vuint8             len        | number of bytes to copy
|
| RETURN VALUES:    -
|
| DESCRIPTION:      copy data from CAN receive buffer to RAM.
|
| ATTENTION:        
************************************************************************** */
/* Msg(4:3673) The object addressed by the pointer "src" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
/* CODE CATEGORY 1 START */
V_DEF_FUNC_API(V_NONE, void, CODE) CanCopyToCan(CanChipDataPtr dst, V_DEF_P2VAR_PARA(V_NONE, void, AUTOMATIC, APPL_VAR) src, vuint8 len)       /* PRQA S 3673 */     /* suppress message about const pointer */
{
  vuint8 canllidx;
  for(canllidx=0; canllidx<len; canllidx++)
  {
    dst[canllidx] = ((V_DEF_P2VAR_PARA(V_NONE, vuint8, AUTOMATIC, APPL_VAR))src)[canllidx];
  }
}
/* CODE CATEGORY 1 END */
# endif

# if defined ( C_ENABLE_DRIVER_STATUS )
/* **********************************************************************
| NAME:             CanGetDriverStatus
| CALLED BY:        Application
| PRECONDITIONS:    none  
| INPUT PARAMETER:  CanChannelHandle   channel    | 
| RETURN VALUE:     kCanDriverBusoff:     CAN Driver is in state Busoff, 
|                                         Init mode is not reached jet.
|                   kCanDriverBusoffInit: Initmode is reached, CanResetBusoffEnd
|                                         can be processed without delay on entry. 
|                   kCanDriverNormal:     Initmode is already left. CanResetBusoffEnd
|                                         can be processed without delay on entry 
|                                         (if not already performed). 
|
| DESCRIPTION:      The API function provide the current state of the 
|                   bus off recovery. This allows to call CanResetBusOffEnd
|                   more early to be able to receive messages as soon 
|                   as possible after Busoff.
*********************************************************************** */
/* CODE CATEGORY 4 START */
V_DEF_FUNC_API(V_NONE, vuint8, CODE) CanGetDriverStatus(CAN_CHANNEL_CANTYPE_ONLY)
{
/*  not needed -> no split in reset busoff start / end necessary
  vuint8 retval;
if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), channel, kErrorDisabledChannel);
endif
  retval = kCanDriverNormal;
if (CANLL_CELL_PTR->Node[canllNode ].NCR & C_NCR_INIT) == C_NCR_INIT)
{
  retval = kCanDriverBusoffInit;
}
*/
}
/* CODE CATEGORY 4 END */
# endif /* C_ENABLE_DRIVER_STATUS         */



#if defined( C_ENABLE_CAN_RAM_CHECK )
/* **************************************************************************
| NAME:             CanLL_IsMailboxCorrupt
| CALLED BY:        CanCheckMemory()
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: CanChannelHandle  channel        | current CAN channel
|                   CanObjectHandle   hwObjHandle    | Handle to hardware object
|
| RETURN VALUES:    kCanTrue:  Mailbox is corrupt
|                   kCanFalse: Mailbox is not corrupt  
|
| DESCRIPTION:      check the current mailbox at index hwObjHandle
************************************************************************** */
/* CODE CATEGORY 4 START */
V_DEF_FUNC(STATIC, vuint8, CODE) CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle)
{
  vuint8 canllidx;
  vuint8 ret_val;
  V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;
  pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[hwObjHandle];
  ret_val = kCanFalse;
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  canHwChannel = canHwChannel; /* dummy avoid lint warning */
# endif
  /* at this point the mailbox is not initialized therefore the contents are not saved! */
  {
    for(canllidx = 0; canllidx < 3; canllidx++)
    {
       /* write Id, Dlc, data registers with CanMemCheckValues..bit[canllidx] and verify the content */ 
      pCanCellMailbox->MoFCR_DLC  = (CanMemCheckValues8bit[canllidx]  & C_MOFCR_DLC);
      pCanCellMailbox->MoARHi     = CanMemCheckValues16bit[canllidx];
      pCanCellMailbox->MoARLo     = CanMemCheckValues16bit[canllidx];
      pCanCellMailbox->MoDR[0]    = CanMemCheckValues32bit[canllidx];
      pCanCellMailbox->MoDR[1]    = CanMemCheckValues32bit[canllidx];
      if( (pCanCellMailbox->MoFCR_DLC != (CanMemCheckValues8bit[canllidx]  & C_MOFCR_DLC) ) ||
          (pCanCellMailbox->MoARHi    != CanMemCheckValues16bit[canllidx]) ||
          (pCanCellMailbox->MoARLo    != CanMemCheckValues16bit[canllidx]) ||
          (pCanCellMailbox->MoDR[0]   != CanMemCheckValues32bit[canllidx]) ||
          (pCanCellMailbox->MoDR[1]   != CanMemCheckValues32bit[canllidx]) )
      {
        ret_val = kCanTrue; /* the mailbox is corrupt */
        /* no break allowed by QAC */
      }
    }
  }
  return(ret_val);
}
/* CODE CATEGORY 4 END */
#endif

#if defined( C_LL_INTERRUPT_ISR_ACTIVE ) /* ! pure polling configuration */
/* **************************************************************************
| NAME:             CanInterrupt( canHwChannel )
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: canHwChannel
| RETURN VALUES:    none
| DESCRIPTION:      Interrupt service functions according to the CAN controller
|                   interrupt stucture
|                   - check for the interrupt reason ( interrupt source )
|                   - work appropriate interrupt:
|                     + status/error interrupt (busoff, wakeup, error warning)
|                     + basic can receive
|                     + full can receive
|                     + can transmit
|
|                   If an Rx-Interrupt occurs while the CAN controller is in Sleep mode, 
|                   a wakeup has to be generated. 
|
|                   If an Tx-Interrupt occurs while the CAN controller is in Sleep mode, 
|                   an assertion has to be called and the interrupt has to be ignored.
|
|                   The name of BrsTimeStrt...() and BrsTimeStop...() can be addapted to 
|                   realy used name of the interrupt functions.
|
************************************************************************** */
/* CODE CATEGORY 1 START */
V_DEF_FUNC(STATIC, void, CODE) CanInterrupt( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
  CanObjectHandle canllHwobj;
  vuint8  canllPndRegIdx;
  vuint8  canllPndReg;
  vuint32 canllPndRegBit;

# if defined( C_ENABLE_SLEEP_WAKEUP )
  if( !CanLL_HwIsSleep(CAN_CHANNEL_CANPARA_ONLY) )
  { /* do not access registers in sleep mode */
# endif

# if defined( C_ENABLE_NESTED_INTERRUPTS ) && defined( V_CPU_TRICORE)
  VStdEnableInt();  /* nested on: enable Interrupts with higher priority (XC2000 higher prio ISR is always on) */
# endif

# if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerStart(kCanLoopIrqReq);
  for (canllPndRegIdx=0; (CanLL_ApplCanTimerLoop(kCanLoopIrqReq)!=0x00) && (canllPndRegIdx<CAN_MAX_PND_REG); )
# else  /* C_ENABLE_HW_LOOP_TIMER */
  for (canllPndRegIdx=0; canllPndRegIdx<CAN_MAX_PND_REG; )
# endif /* C_ENABLE_HW_LOOP_TIMER */
  { /* any pending in the pending-regs */
# if (defined( C_ENABLE_NESTED_INTERRUPTS ) && defined( V_CPU_TRICORE)) 
    CanNestedGlobalInterruptDisable();
# endif
    CANLL_CELL_PTR->Modul.MSIMask = CanInterruptPendingMask[canHwChannel].GlobalMask[canllPndRegIdx];/* lint !e662 */
    canllPndReg = (vuint8) (CANLL_CELL_PTR->Modul.MSID[canllPndRegIdx] & (vuint32)0x000000FF);
# if (defined( C_ENABLE_NESTED_INTERRUPTS ) && defined( V_CPU_TRICORE)) 
    CanNestedGlobalInterruptRestore();
# endif
    if (canllPndReg != (vuint8)32) 
    {
      /* else found pending interrupt source */
      canllHwobj =  ((CanObjectHandle)canllPndReg + ((CanObjectHandle)canllPndRegIdx<<5)); /* calculate hwobject-index */
      canllPndRegBit = ((0x00000001UL)<<canllPndReg);                      /* calculate pending bit for mask */
# if defined( C_ENABLE_INTERNAL_CHECK ) && defined(C_ENABLE_INDIVIDUAL_POLLING)
      if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][canllHwobj] != (vuint8)0x00) {
        canllHwobj = (CanObjectHandle)CAN_MAX_NUM_MSGOBJ; /* illegal interrupt occure (handled below) */
      }
# endif
      /***************** Tx-Interrupts ***********************************************/
# if !defined( C_ENABLE_TX_POLLING) || defined( C_ENABLE_INDIVIDUAL_POLLING )
      if ( (CanInterruptPendingMask[canHwChannel].TxMask[canllPndRegIdx] & canllPndRegBit) == canllPndRegBit )/* lint !e662 */
      {
        CANLL_CELL_PTR->Modul.MSPND[canllPndRegIdx] = ~canllPndRegBit; /* delete pending interrupt */
        CanHL_TxConfirmation(CAN_CHANNEL_CANPARA_FIRST canllHwobj);
      }
      else
# endif
      {
      /***************** RxFull-Interrupts ***********************************************/
# if defined ( C_ENABLE_RX_FULLCAN_OBJECTS ) && (!defined (C_ENABLE_RX_FULLCAN_POLLING) || defined( C_ENABLE_INDIVIDUAL_POLLING ))
        if ( (CanInterruptPendingMask[canHwChannel].RxFullMask[canllPndRegIdx] & canllPndRegBit) == canllPndRegBit )/* lint !e662 */
        {
          CanFullCanMsgReceived( CAN_CHANNEL_CANPARA_FIRST canllHwobj );
        }
        else
# endif
        {
        /***************** RxBasic-Interrupts ***********************************************/
# if defined ( C_ENABLE_RX_BASICCAN_OBJECTS ) && (!defined (C_ENABLE_RX_BASICCAN_POLLING) || defined( C_ENABLE_INDIVIDUAL_POLLING ))
          if ( (CanInterruptPendingMask[canHwChannel].RxBasicMask[canllPndRegIdx] & canllPndRegBit) == canllPndRegBit )/* lint !e662 */
          {
            CanBasicCanMsgReceived ( CAN_CHANNEL_CANPARA_FIRST canllHwobj );
          }
          else
# endif
          {
            CANLL_CELL_PTR->Modul.MSPND[canllPndRegIdx] = ~canllPndRegBit; /* delete pending interrupt */
            assertInternal (CANLL_FALSE, channel, kErrorUnknownInterrupt)   /* lint !e774 !e506 */
          }
        }
      }
      canllPndRegIdx = 0; /* one pending is found .. restart with first object */
#if defined( V_CPU_TRICORE )
      /* clear pending interrupt .. no further occure after execution */
      CANLL_CELL_PTR->Ctrl.Src[CANLL_SRC_OFFSET-CanSrcNum[canHwChannel]] |= C_ICR_CLR_IPND;
#endif
    } else {
      canllPndRegIdx++;
    }
  }
# if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerEnd(kCanLoopIrqReq);
# endif
# if !defined( C_ENABLE_ERROR_POLLING)
  if ( (CANLL_CELL_PTR->Node[CAN_NODE_NUM].NSR & C_NSR_ALERT) == C_NSR_ALERT )
  { /* warning or busoff occure */
    /***************** Status-Interrupts *******************************************/
    CANLL_CELL_PTR->Node[CAN_NODE_NUM].NSR &= ~C_NSR_ALERT;
    CanHL_ErrorHandling(CAN_CHANNEL_CANPARA_ONLY);
  }
# endif
# if defined( C_ENABLE_SLEEP_WAKEUP )
  } /* do not access registers in sleep mode */
# endif
} /* end CanInterrupt */ /* lint !e550 */
/* CODE CATEGORY 1 END */
#endif /* !(full polling) */


# if defined(C_ENABLE_SUPPORT_REMOTE_FRAME)
#  if defined(C_ENABLE_WORKAROUND_TC21) || defined(C_ENABLE_HWBUFFER)
#   error "Workaround TC.21 or HW-buffer enabled but C_ENABLE_SUPPORT_REMOTE_FRAME does not run together with that feature"
#  endif
/****************************************************************************
| NAME:             Can_TransmitRemote
| CALLED BY:        HLL: Application
| PRECONDITIONS:    Using CanTransmit() with same "txHandle" 
| INPUT PARAMETERS: CanChannelHandle  channel    : CAN channel to send Remote Frame
|                   vuint32 msgId                : CAN ID for Remote or (bit31 set means extended ID)
| RETURN VALUES:    vuint8 kCanTxFailed or       : transmit failed (blocked object)
|                          kCanTxOk              : transmit was successful
| DESCRIPTION:      Use special mailbox, write ID and send Remote Frame.
****************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC_API(V_NONE, vuint8, CODE) Can_TransmitRemote(CAN_CHANNEL_CANTYPE_FIRST vuint32 msgId) C_API_3
{
  vuint8 retval;
  CanTransmitHandle rxObjHandle;
  CanDeclareGlobalInterruptOldStatus
  
  rxObjHandle = (CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM;
  V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;
  pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[rxObjHandle];
  CanNestedGlobalInterruptDisable();
  
  if ( (pCanCellMailbox->MoCtrL & C_MOCTR_TXREQ) == (vuint16)0x0000 )
  { /* free object ? can not be evaluated with 'CanLL_TxIsHWObjFree' for remote frames */
    /* Write ID */
#  if defined( C_ENABLE_EXTENDED_ID )
#   if defined( C_ENABLE_MIXED_ID )
    if ((msgId & 0x80000000UL) == 0x80000000UL)
    {
      pCanCellMailbox->MoARHi  = MK_EXTID0(msgId);
      pCanCellMailbox->MoARLo  = MK_EXTID1(msgId);
    }
    else
    {
      pCanCellMailbox->MoARHi  = MK_STDID0((vuint16)msgId);
    }
#   else /* ext_id only */
    pCanCellMailbox->MoARHi  = MK_EXTID0(msgId);
    pCanCellMailbox->MoARLo  = MK_EXTID1(msgId);
#   endif
#  else /* std_id only */
    pCanCellMailbox->MoARHi  = MK_STDID0((vuint16)msgId);
#  endif
    /* Set as Remote(Request) and start Tx */
    pCanCellMailbox->MoCtrL = C_MOCTR_RTSEL; /* delete sel bit */
    pCanCellMailbox->MoCtrH = C_MOCTR_TXEN0 | C_MOCTR_TXEN1 | C_MOCTR_TXREQ | C_MOCTR_NEWDAT;
    retval = kCanTxOk;
  }
  else
  {
    retval = kCanTxFailed;
  }
  CanNestedGlobalInterruptRestore();
  return retval;


}
/* CODE CATEGORY 2 END */
# endif /*  defined(C_ENABLE_SUPPORT_REMOTE_FRAME) */



#if defined( C_LL_INTERRUPT_ISR_ACTIVE )
/****************************************************************************
| NAME:             CanIsr_0
| CALLED BY:        HLL, Asr: Interrupt
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Interrupt service functions according to the CAN controller
|                   interrupt stucture
|
|  Attention:  <Name> has to be repleased with the name of the ISR. 
|              Naming conventions: with Name = "", "Rx", "Tx", "RxTx", "Wakeup", "Status" 
|  The name of the ISR will always have a channel index at the end. Even in single channel
|  systems.
************************************************************************** */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_0 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_0Cat)
#    if (osdIsrCanIsr_0Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_0 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_0Cat)
#    if (osdIsrCanIsr_0Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_0), CODE) CanIsr_0(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_0), CODE) CanIsr_0(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_0);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_0) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_1 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_1Cat)
#    if (osdIsrCanIsr_1Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_1 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_1Cat)
#    if (osdIsrCanIsr_1Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_1), CODE) CanIsr_1(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_1), CODE) CanIsr_1(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_1);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_1) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_2 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_2Cat)
#    if (osdIsrCanIsr_2Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_2 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_2Cat)
#    if (osdIsrCanIsr_2Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_2), CODE) CanIsr_2(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_2), CODE) CanIsr_2(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_2);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_2) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_3 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_3Cat)
#    if (osdIsrCanIsr_3Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_3 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_3Cat)
#    if (osdIsrCanIsr_3Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_3), CODE) CanIsr_3(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_3), CODE) CanIsr_3(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_3);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_3) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_4 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_4Cat)
#    if (osdIsrCanIsr_4Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_4 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_4Cat)
#    if (osdIsrCanIsr_4Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_4), CODE) CanIsr_4(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_4), CODE) CanIsr_4(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_4);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_4) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_5 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_5Cat)
#    if (osdIsrCanIsr_5Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_5 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_5Cat)
#    if (osdIsrCanIsr_5Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_5), CODE) CanIsr_5(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_5), CODE) CanIsr_5(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_5);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_5) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_6 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_6Cat)
#    if (osdIsrCanIsr_6Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_6 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_6Cat)
#    if (osdIsrCanIsr_6Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_6), CODE) CanIsr_6(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_6), CODE) CanIsr_6(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_6);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_6) */
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
# if defined( kCanPhysToLogChannelIndex_7 )
#  if defined( C_ENABLE_OSEK_OS ) && \
      defined( C_ENABLE_OSEK_OS_INTCAT2 )
#   if defined (osdIsrCanIsr_7Cat)
#    if (osdIsrCanIsr_7Cat != 2)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
ISR( CanIsr_7 )
#  else
#   if defined( C_ENABLE_OSEK_OS ) && \
     defined (osdIsrCanIsr_7Cat)
#    if (osdIsrCanIsr_7Cat != 1)
#     error "inconsistent configuration of Osek-OS interrupt categorie between CANgen/GENy and OIL-configurator (CanIsr)"
#    endif
#   endif
#   if defined( C_COMP_TASKING_TRICORE_MULTICAN )
#    if defined( COMPILER_VERSION_1 )
V_DEF_FUNC(V_NONE, void _interrupt(kCanISRPrio_7), CODE) CanIsr_7(void)
#    else
V_DEF_FUNC(V_NONE, void __interrupt(kCanISRPrio_7), CODE) CanIsr_7(void)
#    endif
#   endif
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_ENABLE_MULTICHANNEL_API)
  /* call Interrupthandling with logical channel */
  CanInterrupt(kCanPhysToLogChannelIndex_7);
#  else
  CanInterrupt( );
#  endif
} /* END OF CanISR */
# endif /* (kCanPhysToLogChannelIndex_7) */
/* CODE CATEGORY 1 END */
#endif /* interrupt system */

/****************************************************************************
| NAME:             CanInit
| CALLED BY:        CanInitPowerOn(), Network management
| PRECONDITIONS:    none
| INPUT PARAMETERS: Handle to initstructure
| RETURN VALUES:    none
| DESCRIPTION:      initialization of chip-hardware
|                   initialization of receive and transmit message objects
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInit( CAN_CHANNEL_CANTYPE_FIRST CanInitHandle initObject )     /* PRQA S 1505 */
{
#if defined( C_ENABLE_CAN_RAM_CHECK )
  vuint8                 canRamCheckStatus;
#endif
  CanObjectHandle        hwObjHandle;
#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) || \
    defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
  CanTransmitHandle      txHandle;
#endif
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
  CanReceiveHandle       rxHandle;
#endif
  CanObjectHandle        logTxObjHandle;

  vuint8 canllidx;
  vuint8 canllNode; /* do not use here CanChannelHandle - compiler abstraction for automatic not allowed */
  #if defined (C_ENABLE_RX_BASICCAN_OBJECTS)
  vuint8 canllmaskidx;
  #endif
  V_DEF_P2SFR_CAN(V_NONE, tCanNodeStruct, AUTOMATIC)   pCanCellNode;
  V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;

#if defined( C_HL_ENABLE_LAST_INIT_OBJ )  
  lastInitObject[channel] = initObject;
#endif

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/* Msg(4:3759) Implicit conversion: int to unsigned short. MISRA Rule 43 - no change in RI 1.4 */
  initObject  += CAN_HL_INIT_OBJ_STARTINDEX(channel);
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
#endif
  assertUser(initObject < kCanNumberOfInitObjects, channel, kErrorInitObjectHdlTooLarge);    /* PRQA S 3109 */
  assertUser(initObject < CAN_HL_INIT_OBJ_STOPINDEX(channel), channel, kErrorInitObjectHdlTooLarge);     /* PRQA S 3109 */

#if defined( C_ENABLE_CAN_RAM_CHECK )
  canRamCheckStatus = kCanRamCheckOk;
#endif

  {

    canllNode = CAN_NODE_NUM;
    pCanCellNode = (CanNodeStructPtr) &CANLL_CELL_PTR->Node[canllNode];
    canllBusOffNotified[canHwChannel] = CANLL_TRUE; /* lint !e661 suppress busoff detection while Init-flag is set */
    /* RDS generated as hwchannel access */
    canRDSRxPtr[channel] = pTmpRxMailboxData; /* lint !e661 !e662 */
    #if !defined(C_ENABLE_MULTIPLE_DRIVERS) && defined( V_CPU_TRICORE )
    VStdUnlockInit();
    CANLL_CELL_PTR->Ctrl.CLC &= ~C_CLC_DISABLE;
    VStdLockInit();
    # if defined( C_ENABLE_SLEEP_WAKEUP )
    canllSleepNotify[canHwChannel] = CANLL_FALSE; /* mark channel as wakeup */
    # endif
    #endif
    
    #if defined( C_ENABLE_ERROR_POLLING )
    pCanCellNode->NCR = ( C_NCR_CCE | C_NCR_INIT );   /* node deactivate, enable change */
    #else
    pCanCellNode->NCR = ( C_NCR_CCE | C_NCR_INIT | C_NCR_ErrIE );    /* +enable Error interrupts */
    #endif
    pCanCellNode->NBTR = CanInitBT[initObject];
    pCanCellNode->NSR  = (vuint16)0x00; /* delete status */
    pCanCellNode->NECnt_Rec = (vuint8)0x00;
    pCanCellNode->NECnt_Tec = (vuint8)0x00;
    pCanCellNode->NECnt_EWRNLVL = (vuint8)96;
    /* all 4 interrupts of node x occure in 1 CAN_ISR x */
    pCanCellNode->NIPR = (vuint16)(CanSrcNum[canHwChannel] & 0x0F) | ((vuint16)(CanSrcNum[canHwChannel] & 0x0F)<<4) | 
                        ((vuint16)(CanSrcNum[canHwChannel] & 0x0F)<<8) | ((vuint16)(CanSrcNum[canHwChannel] & 0x0F)<<12);
    /* set the rx-source for this node 0x000 - 0x111 possible for other P6-pinning */
    pCanCellNode->NPCR = (vuint16)CanInitPortSel[canHwChannel];
    
    /* Init Tx-Objects -------------------------------------------------------- */
    /* init saved Tx handles: */                   
    /* in case of CommonCAN, transmission is always on the frist HW channel of a CommonCAN channel */
    {
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
      assertGen( (vsintx)CAN_HL_HW_TX_STOPINDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel) 
                 == CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel), channel, kErrorHwToLogTxObjCalculation);          /* PRQA S 3109 */
      assertGen( (vsintx)CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel) 
                 == CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel), channel, kErrorHwToLogTxObjCalculation);         /* PRQA S 3109 */
      assertGen( CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) <= CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel), channel, kErrorHwToLogTxObjCalculation);  /* PRQA S 3109 */
#endif
  
      for (hwObjHandle=CAN_HL_HW_TX_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_TX_STOPINDEX(canHwChannel); hwObjHandle++ )     /*lint !e661*/
      {
        logTxObjHandle = (CanObjectHandle)((vsintx)hwObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));     /*lint !e661*/

#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) || \
    defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
        if((canStatus[channel] & kCanHwIsInit) == kCanHwIsInit)                    /*lint !e661*/
        {
          /* inform application, if a pending transmission is canceled */
          txHandle = canHandleCurTxObj[logTxObjHandle];

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
          if( txHandle < kCanNumberOfTxObjects )
          {
            APPLCANCANCELNOTIFICATION(channel, txHandle);
          }
# endif
# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
          if( txHandle == kCanBufferMsgTransmit)
          { 
            APPLCANMSGCANCELNOTIFICATION(channel);
          } 
# endif
        }
#endif

        canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                   /* MsgObj is free */

#if defined( C_ENABLE_CAN_RAM_CHECK )
        if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
        {
# if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
          ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
# endif
          canRamCheckStatus = kCanRamCheckFailed;
        }
#endif

        pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[hwObjHandle];
        
        pCanCellMailbox->MoCtrL = C_MOCTR_ALL; /* reset mailbox (clear MSGVAL before alloc to list) */
        if ( (canStatus[channel] & kCanHwIsInit) == 0 ) /* only in power on init */
        {
          CanLL_WaitForListReady; /* wait for list is not busy .. last command executed (< 40 Fsys-clk) */
          /* insert msg-object (idx) in list (node+1) */
          CanLL_SetListCmd(CANLL_CELL_PTR, (canllNode+1), hwObjHandle, C_PAN_ALLOC_TO_LIST)
        }
        pCanCellMailbox->MoIPR = (vuint16)(CanSrcNum[canHwChannel] & 0x0F) | ((vuint16)(CanSrcNum[canHwChannel] & 0x0F)<<4) |
                                ((vuint16)(hwObjHandle & 0xFF)<<8); /* interrupt Service source */
        {
          pCanCellMailbox->MoCtrH   = C_MOCTR_MSGVAL | C_MOCTR_TXEN1 | C_MOCTR_DIR; /* configure as Tx but no activation*/
        }
        pCanCellMailbox->MoAMRHi    = C_MOAMRHI_OPEN; /* mask open */
        pCanCellMailbox->MoAMRLo    = C_MOAMRLO_OPEN; /* mask open */
        pCanCellMailbox->MoARHi     = C_OBJ_PRIO_16;  /* set prio */
        pCanCellMailbox->MoFCR_Mode = C_MOFCR_MODE_NORMAL; /* configure mode */
        pCanCellMailbox->MoFGPR     = ((vuint32)hwObjHandle<<24) | ((vuint32)hwObjHandle<<16) | ((vuint32)hwObjHandle<<8) |
                                       (vuint32)hwObjHandle;
        pCanCellMailbox->MoFCR_IE   = 0x00; /* configure no interrupt */
        #if defined (C_ENABLE_INDIVIDUAL_POLLING) || (!defined (C_ENABLE_INDIVIDUAL_POLLING) && !defined(C_ENABLE_TX_POLLING))
        # if defined ( C_ENABLE_INDIVIDUAL_POLLING )
        if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][hwObjHandle] == (vuint8)0x00)
        # endif
        {
          pCanCellMailbox->MoFCR_IE   = C_MOFCR_IE_TX; /* configure Tx interrupt */
        }
        #endif
      }

    }

    /* init unused msg objects ------------------------------------------------ */
    for (hwObjHandle=CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel); hwObjHandle++ )  /*lint !e661 !e681*/
    {
      pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[hwObjHandle];
      
      CANLL_CELL_PTR->Obj[hwObjHandle].MoCtrL = C_MOCTR_ALL; /* reset mailbox (clear MSGVAL before alloc to list) */
      if ( (canStatus[channel] & kCanHwIsInit) == 0 ) /* only in power on init */
      {
        CanLL_WaitForListReady; /* wait for list is not busy .. last command executed (< 40 Fsys-clk) */
        /* insert msg-object (idx) in list (node+1) */
        CanLL_SetListCmd(CANLL_CELL_PTR, (canllNode+1), hwObjHandle, C_PAN_ALLOC_TO_LIST)
      }
      pCanCellMailbox->MoFCR_Mode = C_MOFCR_MODE_NORMAL; /* configure mode */
      pCanCellMailbox->MoFGPR     = ((vuint32)hwObjHandle<<24) | ((vuint32)hwObjHandle<<16) | ((vuint32)hwObjHandle<<8) |
                                     (vuint32)hwObjHandle;
      pCanCellMailbox->MoFCR_IE   = 0x00; /* configure interrupts */
    }


#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
    /* init full can receive msg objects: ------------------------------------- */
    for (hwObjHandle=CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel); hwObjHandle++ )
    {
      /* brackets to avoid lint info 834 */
      rxHandle = (hwObjHandle-CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel))+CAN_HL_RX_FULL_STARTINDEX(canHwChannel);

#if defined( C_ENABLE_CAN_RAM_CHECK )
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
# if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
# endif
        canRamCheckStatus = kCanRamCheckFailed;
      }
#endif

      pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[hwObjHandle];
      
      pCanCellMailbox->MoCtrL = C_MOCTR_ALL; /* reset mailbox (clear MSGVAL before alloc to list) */
      if ( (canStatus[channel] & kCanHwIsInit) == 0 ) /* only in power on init */
      {
        CanLL_WaitForListReady; /* wait for list is not busy .. last command executed (< 40 Fsys-clk) */
        /* insert msg-object (idx) in list (node+1) */
        CanLL_SetListCmd(CANLL_CELL_PTR, (canllNode+1), hwObjHandle, C_PAN_ALLOC_TO_LIST)
      }
      pCanCellMailbox->MoIPR   = (vuint16)(CanSrcNum[canHwChannel] & 0x0F) | ((vuint16)(CanSrcNum[canHwChannel] & 0x0F)<<4) |
                                 ((vuint16)(hwObjHandle & 0xFF)<<8); /* interrupt Service source */
      {
        pCanCellMailbox->MoCtrH  = C_MOCTR_MSGVAL | C_MOCTR_RXEN; /* configure as Rx */
      }
      # if defined(C_ENABLE_RX_MASK_EXT_ID) && defined( C_ENABLE_EXTENDED_ID )
      if ((CanGetRxIdType(rxHandle)) == kCanIdTypeExt)
      {
        pCanCellMailbox->MoAMRHi = C_EID_MASK_16 | MK_EXTID0(C_MASK_EXT_ID);
        pCanCellMailbox->MoAMRLo = MK_EXTID1(C_MASK_EXT_ID);
      }
      else
      # endif
      {
        pCanCellMailbox->MoAMRHi = C_EID_MASK_16 | C_MOAMRHI_CLOSE;
        pCanCellMailbox->MoAMRLo = C_MOAMRLO_CLOSE;
      }
      pCanCellMailbox->MoARHi  = (CanGetRxId0(rxHandle)); /* id-type in idRaw */
      # if defined( C_ENABLE_EXTENDED_ID )
      pCanCellMailbox->MoARLo  = (CanGetRxId1(rxHandle));
      #endif
      pCanCellMailbox->MoFCR_Mode = C_MOFCR_MODE_NORMAL; /* configure mode */
      pCanCellMailbox->MoFGPR     = ((vuint32)hwObjHandle<<24) | ((vuint32)hwObjHandle<<16) | ((vuint32)hwObjHandle<<8) | 
                                     (vuint32)hwObjHandle;
      pCanCellMailbox->MoFCR_IE = 0x00; /* configure interrupts */
      
      # if defined (C_ENABLE_INDIVIDUAL_POLLING) || (!defined (C_ENABLE_INDIVIDUAL_POLLING) && !defined(C_ENABLE_RX_FULLCAN_POLLING))
      #  if defined ( C_ENABLE_INDIVIDUAL_POLLING )
      if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][hwObjHandle] == (vuint8)0x00)
      #  endif
      {
        pCanCellMailbox->MoFCR_IE = C_MOFCR_IE_RX; /* (C_MOFCR_IE_OVR not usefull) configure interrupts */
      }
      # endif
    }
#endif

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
    /* init basic can receive msg object: ------------------------------------- */
    for (hwObjHandle=CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); hwObjHandle++ )
    {
#if defined( C_ENABLE_CAN_RAM_CHECK )
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
# if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
# endif
        canRamCheckStatus = kCanRamCheckFailed;
      }
#endif

      /* the LL driver has to know which ID tpyes have to be received by which object */
      pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[hwObjHandle];
      
      pCanCellMailbox->MoCtrL = C_MOCTR_ALL; /* reset mailbox (clear MSGVAL before alloc to list) */
      if ( (canStatus[channel] & kCanHwIsInit) == 0 ) /* only in power on init */
      {
        CanLL_WaitForListReady; /* wait for list is not busy .. last command executed (< 40 Fsys-clk) */
        /* insert msg-object (idx) in list (node+1) */
        CanLL_SetListCmd(CANLL_CELL_PTR, (canllNode+1), hwObjHandle, C_PAN_ALLOC_TO_LIST)
      }
      pCanCellMailbox->MoIPR = (vuint16)(CanSrcNum[canHwChannel] & 0x0F) | ((vuint16)(CanSrcNum[canHwChannel] & 0x0F)<<4) | 
                              ((vuint16)(hwObjHandle & 0xFF)<<8); /* interrupt Service source */
      canllmaskidx = (vuint8) (((vuint8)(hwObjHandle-CAN_HL_HW_RX_BASIC_STARTINDEX(channel))>>1) + (initObject*kCanNumberOfMaxBasicCAN));
      # if defined ( C_ENABLE_MULTIPLE_BASICCAN ) && \
           defined ( C_ENABLE_GEN_CHECK )
      assertGen(((vuint8)(hwObjHandle-CAN_HL_HW_RX_BASIC_STARTINDEX(channel))>>1) <= CanNumberOfBasicCAN[channel], channel, kErrorBasicCANMask);
      # endif
      pCanCellMailbox->MoFCR_IE = 0x00; /* configure no interrupts */
      
      # if defined (C_ENABLE_INDIVIDUAL_POLLING) || (!defined (C_ENABLE_INDIVIDUAL_POLLING) && !defined(C_ENABLE_RX_BASICCAN_POLLING))
      #  if defined ( C_ENABLE_INDIVIDUAL_POLLING )
      if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][hwObjHandle] == (vuint8)0x00)
      #  endif
      {
        pCanCellMailbox->MoFCR_IE = C_MOFCR_IE_RX; /* (C_MOFCR_IE_OVR not usefull) configure interrupts */
      }
      # endif
      /* Problem with Fifo: while Base Object MsgVal is reset (read data), All Fifos are deactivated -> no reception */
      /* so do not use msgval with fifo */
      if ( ((vuint16)(hwObjHandle - CAN_HL_HW_RX_BASIC_STARTINDEX(channel)) & (vuint16)0x0001) == (vuint16)0x0000 ) /* not the Shadow buffer */
      {
        pCanCellMailbox->MoFCR_Mode = C_MOFCR_MODE_RXBASE;
        pCanCellMailbox->MoFGPR = ((vuint32)hwObjHandle<<24) | ((vuint32)hwObjHandle<<16) | (((vuint32)hwObjHandle+(vuint32)1)<<8) | 
                                   (vuint32)hwObjHandle;
      # if defined( C_ENABLE_EXTENDED_ID )
        pCanCellMailbox->MoARLo  = CanInitCodeL[canllmaskidx];
        pCanCellMailbox->MoAMRLo = CanInitMaskL[canllmaskidx];
      #  if defined( C_ENABLE_MIXED_ID )
        pCanCellMailbox->MoARHi  = C_OBJ_PRIO_16 | CanInitCodeH[canllmaskidx];
        pCanCellMailbox->MoAMRHi = CanInitMaskH[canllmaskidx];
      #  else /* ext_id only */
        pCanCellMailbox->MoARHi  = C_OBJ_PRIO_16 | C_EID_MASK_16 | CanInitCodeH[canllmaskidx];
        pCanCellMailbox->MoAMRHi = C_EID_MASK_16 | CanInitMaskH[canllmaskidx];
      #  endif
      # else /* std_id only */
        pCanCellMailbox->MoAMRHi = C_EID_MASK_16 | CanInitMaskH[canllmaskidx];
        pCanCellMailbox->MoARHi  = C_OBJ_PRIO_16 | CanInitCodeH[canllmaskidx];
      # endif
        {
          pCanCellMailbox->MoCtrH = C_MOCTR_MSGVAL | C_MOCTR_RXEN;
        }
      }
      else
      { /* fifo */
        pCanCellMailbox->MoFCR_Mode = C_MOFCR_MODE_NORMAL;
        pCanCellMailbox->MoFGPR     = ((vuint32)hwObjHandle<<24) | ((vuint32)hwObjHandle<<16) | ((vuint32)hwObjHandle<<8) |
                                       (vuint32)hwObjHandle;
        pCanCellMailbox->MoAMRHi    = C_MOAMRHI_CLOSE;
        pCanCellMailbox->MoAMRLo    = C_MOAMRLO_CLOSE;
        pCanCellMailbox->MoARHi     = C_OBJ_PRIO_16;
        pCanCellMailbox->MoARLo     = 0x0000;
        {
          pCanCellMailbox->MoCtrH     = C_MOCTR_MSGVAL;
        }
      }
    }
#endif
                     
    CanLL_WaitForListReady; /* last finished (< 40 Fsys-clk) */
    #if defined(C_ENABLE_WORKAROUND_TC21) || (defined(C_ENABLE_SUPPORT_REMOTE_FRAME))
    canllidx = (vuint8)((CAN_MAX_NUM_MSGOBJ-1)-canllNode);
    pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[canllidx];
    
    if ( (canStatus[channel] & kCanHwIsInit) == 0 ) /* only in power on init */
    {
      CanLL_SetListCmd(CANLL_CELL_PTR, (canllNode+1), canllidx, C_PAN_ALLOC_TO_LIST)
      CanLL_WaitForListReady; /* last finished (< 40 Fsys-clk) */
    }
    pCanCellMailbox->MoCtrL     = C_MOCTR_ALL; /* reset mailbox */
    pCanCellMailbox->MoFCR_Mode = C_MOFCR_MODE_NORMAL; /* configure mode */
    pCanCellMailbox->MoFGPR     = ((vuint32)canllidx<<24) | ((vuint32)canllidx<<16) | ((vuint32)canllidx<<8) | (vuint32)canllidx;
    pCanCellMailbox->MoFCR_DLC  = 0x00;
    #endif
    
    #if defined(C_ENABLE_WORKAROUND_TC21)
    pCanCellMailbox->MoCtrH     = C_MOCTR_MSGVAL | C_MOCTR_TXEN1 | C_MOCTR_DIR; /* configure as Tx */
    pCanCellMailbox->MoFCR_IE   = C_MOFCR_IE_STT;      /* configure no interrupt & single tx try */
    # if defined( C_ENABLE_EXTENDED_ID )
    pCanCellMailbox->MoARHi     = C_MOARHI_PRIO_DUMMY | C_EID_MASK_16 | kCanUnusedIDHi;
    # else 
    pCanCellMailbox->MoARHi     = C_MOARHI_PRIO_DUMMY | kCanUnusedIDHi;
    # endif
    pCanCellMailbox->MoARLo     = kCanUnusedIDLo;
    #endif
    
    #if defined(C_ENABLE_SUPPORT_REMOTE_FRAME)
    pCanCellMailbox->MoCtrH     = C_MOCTR_MSGVAL;           /* configure as Rx but do not activate it */
    pCanCellMailbox->MoFCR_IE   = C_MOFCR_MODE_NORMAL;      /* configure no interrupt (normal tx or rx) */
    pCanCellMailbox->MoARHi     = C_OBJ_PRIO_16 | 0;        /* set ID to 0 */
    pCanCellMailbox->MoARLo     = 0;
    #endif
    
    /* set interrupt prio and enable interrupt */
    #if defined(C_ENABLE_CAN_INTERRUPT_CONTROL)
    if ( (canStatus[channel] & kCanHwIsInit) == 0 ) /* only in power on init */
    {
      assertInternal (canPhysPrio[CAN_NODE_NUM] != 0, channel, kErrorUnknownISRPriority)   /* lint !e774 !e506 */
    # if defined( V_CPU_TRICORE )
    #  if defined( C_LL_INTERRUPT_ISR_ACTIVE ) /* ! pure polling configuration */
      CANLL_CELL_PTR->Ctrl.Src[CANLL_SRC_OFFSET-CanSrcNum[canHwChannel]] = C_ICR_CR_INIT | (vuint32)canPhysPrio[CAN_NODE_NUM];
    #  else
      CANLL_CELL_PTR->Ctrl.Src[CANLL_SRC_OFFSET-CanSrcNum[canHwChannel]] = C_ICR_CLR_IPND | (vuint32)canPhysPrio[CAN_NODE_NUM];
    #  endif
    # endif
    }
    #endif /* C_ENABLE_CAN_INTERRUPT_CONTROL */
    /* delete all pending messages for this channel */
    for (canllidx=0; canllidx < CAN_MAX_PND_REG; canllidx++)
    {
      CANLL_CELL_PTR->Modul.MSPND[canllidx] = ~CanInterruptPendingMask[canHwChannel].GlobalMask[canllidx];
    }
    /* Problem while busoff: only Canll_Start should start CAN if halted... so no Init-flag should be reset before */
    canllHalt[canHwChannel] = CANLL_FALSE;
    {
      pCanCellNode->NCR &= ~(C_NCR_CCE | C_NCR_INIT);   /* activate node */
    }
    canllBusOffNotified[canHwChannel] = CANLL_FALSE;  /* enable busoff detection after Init-flag is reset */
  } /* end of loop over all hw channels */

#if defined( C_ENABLE_TX_OBSERVE )
  ApplCanInit( CAN_CHANNEL_CANPARA_FIRST CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel), CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel) );
#endif
#if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
  APPL_CAN_MSGTRANSMITINIT( CAN_CHANNEL_CANPARA_ONLY );
#endif

#if defined( C_ENABLE_CAN_RAM_CHECK )
  if( canRamCheckStatus == kCanRamCheckFailed)
  {
    /* let the application decide if communication is disabled */
    if (ApplCanMemCheckFailed(CAN_CHANNEL_CANPARA_ONLY) == kCanDisableCommunication)
    {
      canComStatus[channel] = kCanDisableCommunication;
    }  
  }
#endif


} /* END OF CanInit */
/* CODE CATEGORY 4 END*/


/****************************************************************************
| NAME:             CanInitPowerOn
| CALLED BY:        Application
| PRECONDITIONS:    This function must be called by the application before
|                   any other CAN driver function 
|                   Interrupts must be disabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initialization of the CAN chip
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInitPowerOn( void )
{ 

#if defined( C_ENABLE_VARIABLE_DLC )        || \
      defined( C_ENABLE_DYN_TX_OBJECTS )      || \
      defined( C_ENABLE_INDICATION_FLAG )     || \
      defined( C_ENABLE_CONFIRMATION_FLAG )
  CanTransmitHandle txHandle;
#endif
#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  CanReceiveHandle rxHandle;
#endif
  CAN_CHANNEL_CANTYPE_LOCAL


  VStdInitPowerOn();

#if defined( C_ENABLE_VARIABLE_DLC )
  for (txHandle = 0; txHandle < kCanNumberOfTxObjects; txHandle++)
  {
    assertGen(XT_TX_DLC(CanGetTxDlc(txHandle))<(vuint8)9, kCanAllChannels, kErrorTxROMDLCTooLarge);      /* PRQA S 3109 */
    canTxDLC_RAM[txHandle] = CanGetTxDlc(txHandle);
  }
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )
  /*  Reset dynamic transmission object management -------------------------- */
  for (txHandle = 0; txHandle < kCanNumberOfTxDynObjects; txHandle++)
  {
    /*  Reset management information  */
    canTxDynObjReservedFlag[txHandle] = 0;
  }
#endif /* C_ENABLE_DYN_TX_OBJECTS */

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  /*  Initialize the array with received dlc -------------------------- */
  for (rxHandle = 0; rxHandle < kCanNumberOfRxObjects; rxHandle++) {
    canVariableRxDataLen[rxHandle] = CanGetRxDataLen(rxHandle);
  }
#endif

#if defined( C_ENABLE_INDICATION_FLAG )
/* txHandle as loop variable is ok here, because the number of indication bytes is not as high as the number of Rx Messages */
  for (txHandle = 0; txHandle < kCanNumberOfIndBytes; txHandle++) {
    CanIndicationFlags._c[txHandle] = 0;
  }
#endif

#if defined( C_ENABLE_CONFIRMATION_FLAG )
  for (txHandle = 0; txHandle < kCanNumberOfConfBytes; txHandle++) {
    CanConfirmationFlags._c[txHandle] = 0;
  }
#endif

#if defined( C_ENABLE_RX_QUEUE )
  CanDeleteRxQueue();
#endif

#if defined( C_ENABLE_TRANSMIT_QUEUE )
#endif

  {
  #if !defined(C_ENABLE_MULTIPLE_DRIVERS)
  # if defined( V_CPU_TRICORE )
    volatile vuint16 canll_temp_read;
  # endif
  #endif
    vuint8 canll_ch;
  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    channel = 0;
  #endif
  #if !defined(C_ENABLE_MULTIPLE_DRIVERS)
  # if defined( V_CPU_TRICORE )
    VStdUnlockInit();
    CANLL_CELL_PTR->Ctrl.CLC = C_CLC_START; /* start CAN-clock / disable sleep */
    canll_temp_read = CANLL_CELL_PTR->Ctrl.CLC; /* lint !e552  needed before write to fdr -> workaround */
    CANLL_CELL_PTR->Ctrl.FDR = CanClockDividor; /* set dividor for baudrate-clock */
    VStdLockInit();
    canll_temp_read = canll_temp_read; /* dummy statement will be optimized */
  # endif
    /* Workaround for high frequency clock to ensure FDR write */
    CanLL_WaitForListReady; /* reset list finished (< 50 Fsys-clk) */
    CANLL_CELL_PTR->Modul.MCR = (vuint16)0x0000; /* use 1:1 pending mapping */
  #endif
  #if defined(C_ENABLE_MULTIPLE_DRIVERS)
    for (canll_ch=0; canll_ch < kCanNumberOfChannels; canll_ch++) 
  #else
    for (canll_ch=0; canll_ch < CAN_MAX_NUM_NODE; canll_ch++) 
  #endif
    {   /* Reset List needs Init-mode for all nodes */
      CANLL_CELL_PTR->Node[canll_ch].NCR = ( C_NCR_CCE | C_NCR_INIT ); /* deactivate all channel and activate in CanInit() again */
    }
  #if !defined(C_ENABLE_MULTIPLE_DRIVERS)
    CanLL_WaitForListReady; /* wait for list is not busy .. last command executed */
    CanLL_SetListCmd(CANLL_CELL_PTR, 0, 0, C_PAN_RESET_LIST)
    CanLL_WaitForListReady; /* reset list finished (< 50 Fsys-clk) */
  #endif
  }

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  for (channel=0; channel<kCanNumberOfChannels; channel++)
#endif
  {

#if defined( C_ENABLE_CAN_RAM_CHECK )
    canComStatus[channel] = kCanEnableCommunication;
#endif
   
    canStatus[channel]              = kCanStatusInit;

    #if defined(C_ENABLE_SLEEP_WAKEUP) && defined( V_CPU_TRICORE )
    canllSleepNotify[canHwChannel] = CANLL_FALSE; /* lint !e661 */
    #endif

#if defined( C_ENABLE_CAN_TX_CONF_FCT )
    txInfoStructConf[channel].Channel = channel;
#endif
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )  || \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
    canRxInfoStruct[channel].Channel  = channel;
#endif
    canCanInterruptCounter[channel] = 0; 

#if defined( C_ENABLE_TX_POLLING )          || \
      defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
      defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
      defined( C_ENABLE_ERROR_POLLING )       
    canPollingTaskActive[channel] = 0;
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )   && \
    defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  /*  Reset dynamic transmission object management -------------------------- */
    confirmHandle[channel] = kCanBufferFree;
#endif

#if defined( C_ENABLE_TX_MASK_EXT_ID )
    canTxMask0[channel] = 0;
# if (kCanNumberOfUsedCanTxIdTables > 1)
    canTxMask1[channel] = 0;
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
    canTxMask2[channel] = 0;
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
    canTxMask3[channel] = 0;
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
    canTxMask4[channel] = 0;
# endif
#endif

#if defined( C_ENABLE_ECU_SWITCH_PASS )
    canPassive[channel]             = 0;
#endif

#if defined( C_ENABLE_PART_OFFLINE )
    canTxPartOffline[channel]       = kCanTxPartInit;
#endif
#if defined( C_ENABLE_COND_RECEIVE_FCT )
    canMsgCondRecState[channel]     = kCanTrue;
#endif

    canRxHandle[channel] = kCanRxHandleNotUsed;

    {
#if defined( C_ENABLE_TRANSMIT_QUEUE )
  /* clear all Tx queue flags */
      CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
#endif

      CanInit( CAN_CHANNEL_CANPARA_FIRST 0 );

      /* canStatus is only set to init and online, if CanInit() is called for this channel. */
      canStatus[channel]              |= (kCanHwIsInit | kCanTxOn);
    }

  }

} /* END OF CanInitPowerOn */
/* CODE CATEGORY 4 END*/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/************************************************************************
* NAME:               CanDelQueuedObj
* CALLED BY:          
* PRECONDITIONS:      
* PARAMETER:          notify: if set to 1 for every deleted obj the appl is notified
* RETURN VALUE:       -
* DESCRIPTION:        Resets the bits with are set to 0 in mask
*                     Clearing the Transmit-queue
*************************************************************************/
/* CODE CATEGORY 4 START*/
static void CanDelQueuedObj( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3
{ 
  CanSignedTxHandle     queueElementIdx;
  #if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  CanSignedTxHandle     elementBitIdx;
  CanTransmitHandle     txHandle;
  tCanQueueElementType  elem;
  #endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);    /* PRQA S 3109 */
# endif

  #  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  if((canStatus[channel] & kCanHwIsInit) == kCanHwIsInit)
  {
    CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);        /* avoid interruption by CanHL_TxConfirmation */
    for(queueElementIdx = CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx < CAN_HL_TXQUEUE_STOPINDEX(channel); queueElementIdx++)
    { 
      elem = canTxQueueFlags[queueElementIdx];
      if(elem != (tCanQueueElementType)0) /* is there any flag set in the queue element? */
      {
        /* iterate through all flags and notify application for every scheduled transmission */
        for(elementBitIdx = ((CanSignedTxHandle)1 << kCanTxQueueShift) - (CanSignedTxHandle)1; elementBitIdx >= (CanSignedTxHandle)0; elementBitIdx--)
        { 
          if( ( elem & CanShiftLookUp[elementBitIdx] ) != (tCanQueueElementType)0)
          { 
            txHandle = (CanTransmitHandle)((((CanTransmitHandle)queueElementIdx << kCanTxQueueShift) + (CanTransmitHandle)elementBitIdx) - CAN_HL_TXQUEUE_PADBITS(channel));  /* ESCAN00039235 */
            APPLCANCANCELNOTIFICATION(channel, txHandle);
          } 
        } 
        canTxQueueFlags[queueElementIdx] = (tCanQueueElementType)0;
      }
    } 
    CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
  } 
  else
  #  endif
  {
    for(queueElementIdx = CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx < CAN_HL_TXQUEUE_STOPINDEX(channel); queueElementIdx++)
    { 
      canTxQueueFlags[queueElementIdx] = (tCanQueueElementType)0;
    } 
  }

# if defined( C_HL_ENABLE_COPROCESSOR_SUPPORT )
# endif
}
/* CODE CATEGORY 4 END*/
#endif

#if defined( C_ENABLE_CAN_TRANSMIT )
# if defined( C_ENABLE_CAN_CANCEL_TRANSMIT )
/* CODE CATEGORY 3 START*/
/****************************************************************************
| NAME:             CanCancelTransmit
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: Tx-Msg-Handle
| RETURN VALUES:    none
| DESCRIPTION:      delete on Msg-Object
****************************************************************************/
C_API_1 void C_API_2 CanCancelTransmit( CanTransmitHandle txHandle )
{
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL 
  CanObjectHandle        logTxObjHandle;
  /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle queueElementIdx; /* index for accessing the tx queue */
  CanSignedTxHandle elementBitIdx;  /* bit index within the tx queue element */
  CanTransmitHandle queueBitPos;  /* physical bitposition of the handle */
# endif

  if (txHandle < kCanNumberOfTxObjects)         /* legal txHandle ? */
  {
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    channel = CanGetChannelOfTxObj(txHandle);
# endif

# if defined( C_ENABLE_MULTI_ECU_PHYS )
    assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), channel , kErrorDisabledTxMessage);    /* PRQA S 3109 */
# endif

    CanNestedGlobalInterruptDisable();
# if defined( C_ENABLE_TRANSMIT_QUEUE )
    #if defined( C_ENABLE_INTERNAL_CHECK ) &&\
        defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if (sizeof(queueBitPos) == 1)
    {
      assertInternal( (((vuint16)kCanNumberOfTxObjects + (vuint16)CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 256u), kCanAllChannels, kErrorTxQueueTooManyHandle) /* PRQA S 3109 */ /*lint !e572 !e506*/
    }
    else
    {
      assertInternal( (((vuint32)kCanNumberOfTxObjects + (vuint32)CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 65536u), kCanAllChannels, kErrorTxQueueTooManyHandle) /* PRQA S 3109 */ /*lint !e572 !e506*/
    }
    #endif
    queueBitPos  = txHandle + CAN_HL_TXQUEUE_PADBITS(channel);
    queueElementIdx = (CanSignedTxHandle)(queueBitPos >> kCanTxQueueShift); /* get the queue element where to set the flag */
    elementBitIdx  = (CanSignedTxHandle)(queueBitPos & kCanTxQueueMask);   /* get the flag index wihtin the queue element */
    if( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
    {
      canTxQueueFlags[queueElementIdx] &= ~CanShiftLookUp[elementBitIdx]; /* clear flag from the queue */
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }
# endif

# if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
    logTxObjHandle = (CanObjectHandle)((vsintx)CanGetTxHwObj(txHandle) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
# else
    logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
# endif/* C_ENABLE_TX_FULLCAN_OBJECTS */
    if (canHandleCurTxObj[logTxObjHandle] == txHandle)
    {
      canHandleCurTxObj[logTxObjHandle] = kCanBufferCancel;

      /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }

    CanNestedGlobalInterruptRestore();
  } /* if (txHandle < kCanNumberOfTxObjects) */
}
/* CODE CATEGORY 3 END*/
# endif /* defined( C_ENABLE_CAN_CANCEL_TRANSMIT ) */

#endif /* if defined( C_ENABLE_CAN_TRANSMIT ) */


#if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
/****************************************************************************
| NAME:             CanCancelMsgTransmit
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    none
| DESCRIPTION:      delete on Msg-Object
****************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanCancelMsgTransmit( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
  CanObjectHandle  logTxObjHandle;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif


  logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  CanNestedGlobalInterruptDisable();
  if (canHandleCurTxObj[logTxObjHandle] == kCanBufferMsgTransmit)
  {
    canHandleCurTxObj[logTxObjHandle] = kCanBufferCancel;

    /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
    APPLCANMSGCANCELNOTIFICATION(channel);
  }
  CanNestedGlobalInterruptRestore();
}
/* CODE CATEGORY 3 END*/
#endif


#if defined( C_ENABLE_CAN_TRANSMIT )
# if defined( C_ENABLE_VARIABLE_DLC )
/* CODE CATEGORY 2 START*/
/****************************************************************************
| NAME:             CanTransmitVarDLC
| CALLED BY:        Netmanagement, application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle to Tx message, DLC of Tx message
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
C_API_1 vuint8 C_API_2 CanTransmitVarDLC(CanTransmitHandle txHandle, vuint8 dlc) C_API_3
{
  assertUser(dlc<(vuint8)9, kCanAllChannels, kErrorTxDlcTooLarge);         /* PRQA S 3109 */
  assertUser(txHandle<kCanNumberOfTxObjects, kCanAllChannels , kErrorTxHdlTooLarge);     /* PRQA S 3109 */

  canTxDLC_RAM[ txHandle ] = (canTxDLC_RAM[ txHandle ] & CanLL_DlcMask) | MK_TX_DLC(dlc);

  return CanTransmit( txHandle );
} /* END OF CanTransmitVarDLC */
/* CODE CATEGORY 2 END*/
# endif   /* C_ENABLE_VARIABLE_DLC */

/****************************************************************************
| NAME:             CanTransmit
| CALLED BY:        application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle of the transmit object to be send
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
/* CODE CATEGORY 2 START*/

C_API_1 vuint8 C_API_2 CanTransmit(CanTransmitHandle txHandle) C_API_3   /*lint !e14 !e31*/
{
  CanDeclareGlobalInterruptOldStatus

# if !defined( C_ENABLE_TX_POLLING )          ||\
     !defined( C_ENABLE_TRANSMIT_QUEUE )      ||\
     defined( C_ENABLE_TX_FULLCAN_OBJECTS )   ||\
     defined( C_ENABLE_INDIVIDUAL_POLLING )
  CanObjectHandle      txObjHandle;
  CanObjectHandle      logTxObjHandle;
  vuint8             rc;
# endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */
  CAN_CHANNEL_CANTYPE_LOCAL

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle queueElementIdx; /* index for accessing the tx queue */
  CanSignedTxHandle elementBitIdx;  /* bit index within the tx queue element */
  CanTransmitHandle queueBitPos;  /* physical bitposition of the handle */
# endif
  /* V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox; */



  assertUser(txHandle<kCanNumberOfTxObjects, kCanAllChannels , kErrorTxHdlTooLarge);      /* PRQA S 3109 */

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);          /*lint !e661*/
# endif

# if defined( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), channel , kErrorDisabledTxMessage);    /* PRQA S 3109 */
# endif

  /* test offline ---------------------------------------------------------- */
  if ( (canStatus[channel] & kCanTxOn) != kCanTxOn )           /* transmit path switched off */
  {
    return kCanTxFailed;
  }

# if defined( C_ENABLE_PART_OFFLINE )
  if ( (canTxPartOffline[channel] & CanTxSendMask[txHandle]) != (vuint8)0)   /*lint !e661*/ /* CAN off ? */
  {
    return (kCanTxPartOffline);
  }
# endif

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    return(kCanCommunicationDisabled);
  }
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
  assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);    /* PRQA S 3109 */
# endif
  assertUser(!CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanStop);      /* PRQA S 3109 */

  /* passive mode ---------------------------------------------------------- */
# if defined( C_ENABLE_ECU_SWITCH_PASS )
  if ( canPassive[channel] != (vuint8)0)                             /*  set passive ? */
  {
#  if defined( C_ENABLE_CAN_TX_CONF_FCT ) || \
      defined( C_ENABLE_CONFIRMATION_FCT )
    CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);      /* avoid CAN Rx interruption */
#  endif

#  if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
    txInfoStructConf[channel].Handle  = txHandle;
    APPL_CAN_TX_CONFIRMATION(&txInfoStructConf[channel]);
#  endif

#  if defined( C_ENABLE_CONFIRMATION_FLAG )       /* set transmit ready flag  */
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptDisable();
#   endif
    CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] |= CanGetConfirmationMask(txHandle);      /*lint !e661*/
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptRestore();
#   endif
#  endif

#  if defined( C_ENABLE_CONFIRMATION_FCT )
    {
      if ( CanGetApplConfirmationPtr(txHandle) != V_NULL )
      {
         (CanGetApplConfirmationPtr(txHandle))(txHandle);   /* call completion routine  */
      }
    }
#  endif /* C_ENABLE_CONFIRMATION_FCT */

#  if defined( C_ENABLE_CAN_TX_CONF_FCT ) || \
      defined( C_ENABLE_CONFIRMATION_FCT )
    CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
#  endif

    return kCanTxOk;
  }
# endif


   /* can transmit enabled ================================================== */

   /* ----------------------------------------------------------------------- */
   /* ---  transmit queue with one objects ---------------------------------- */
   /* ---  transmit using fullcan objects ----------------------------------- */
   /* ----------------------------------------------------------------------- */

#  if !defined( C_ENABLE_TX_POLLING )          ||\
      !defined( C_ENABLE_TRANSMIT_QUEUE )      ||\
      defined( C_ENABLE_TX_FULLCAN_OBJECTS )   ||\
      defined( C_ENABLE_INDIVIDUAL_POLLING )

#   if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  txObjHandle = CanGetTxHwObj(txHandle);    /*lint !e661*/
#   else
  txObjHandle = CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel);                                          /* msg in object 0 */
#   endif

  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
#  endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */

  CanNestedGlobalInterruptDisable();
 
  /* test offline after interrupt disable ---------------------------------- */
  if ( (canStatus[channel] & kCanTxOn) != kCanTxOn )                /* transmit path switched off */
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

# if defined( C_ENABLE_TRANSMIT_QUEUE )
#  if defined( C_ENABLE_TX_FULLCAN_OBJECTS )  ||\
      !defined( C_ENABLE_TX_POLLING )         ||\
      defined( C_ENABLE_INDIVIDUAL_POLLING )
  if (
#   if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
       (txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel))                                   /*disabled - lint !e774 */
#   endif
#   if defined( C_ENABLE_TX_FULLCAN_OBJECTS )  &&\
       ( !defined( C_ENABLE_TX_POLLING )         ||\
          defined( C_ENABLE_INDIVIDUAL_POLLING ) )
     &&
#   endif

#   if defined( C_ENABLE_TX_POLLING )
#    if defined( C_ENABLE_INDIVIDUAL_POLLING )
       ( ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] != (vuint8)0 )       /* Object is used in polling mode! */
                         || (canHandleCurTxObj[logTxObjHandle] != kCanBufferFree) )    /* MsgObj used?  */
#    else
        /* write always to queue; transmission is started out of TxTask */
#    endif
#   else
       ( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree)    /* MsgObj used?  */
#   endif
     )
#  endif   /*  ( C_ENABLE_TX_FULLCAN_OBJECTS )  || !( C_ENABLE_TX_POLLING ) || ( C_ENABLE_INDIVIDUAL_POLLING ) */

    {
      /* tx object 0 used -> set msg in queue: -----------------------------*/
      queueBitPos  = txHandle + CAN_HL_TXQUEUE_PADBITS(channel);
      queueElementIdx = (CanSignedTxHandle)(queueBitPos >> kCanTxQueueShift); /* get the queue element where to set the flag */
      elementBitIdx  = (CanSignedTxHandle)(queueBitPos & kCanTxQueueMask);   /* get the flag index wihtin the queue element */
      canTxQueueFlags[queueElementIdx] |= CanShiftLookUp[elementBitIdx];
      CanNestedGlobalInterruptRestore();
      return kCanTxOk;                          
  }
# endif  /* C_ENABLE_TRANSMIT_QUEUE */

# if !defined( C_ENABLE_TX_POLLING )          ||\
     !defined( C_ENABLE_TRANSMIT_QUEUE )      ||\
     defined( C_ENABLE_TX_FULLCAN_OBJECTS )   ||\
     defined( C_ENABLE_INDIVIDUAL_POLLING )

#  if defined( C_ENABLE_TRANSMIT_QUEUE )    && \
      ( defined( C_ENABLE_TX_FULLCAN_OBJECTS )  ||\
        !defined( C_ENABLE_TX_POLLING )         ||\
        defined( C_ENABLE_INDIVIDUAL_POLLING )  )
  else
#  endif
  {
  /* check for transmit message object free ---------------------------------*/
    if (( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree)    /* MsgObj used?  */
       || ( !CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) )

      /* hareware-txObject is not free --------------------------------------*/
       )  /* end of if question */

    {  /* object used */
      /* tx object n used, quit with error */
      CanNestedGlobalInterruptRestore();
      return kCanTxFailed;
    }
  }

  /* Obj, pMsgObject points to is free, transmit msg object: ----------------*/
  canHandleCurTxObj[logTxObjHandle] = txHandle;/* Save hdl of msgObj to be transmitted*/
  CanNestedGlobalInterruptRestore();

  rc = CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle, txHandle);

#  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  if ( rc == kCanTxNotify)
  {
    rc = kCanTxFailed;      /* ignore notification if calls of CanCopy.. is performed within CanTransmit */
  }
#  endif


  return(rc);

# else   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */
# endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */

} /* END OF CanTransmit */
/* CODE CATEGORY 2 END*/


/****************************************************************************
| NAME:             CanCopyDataAndStartTransmission
| CALLED BY:        CanTransmit, CanHl_RestartTxQueue and CanHL_TxConfirmation
| PRECONDITIONS:    - Can driver must be initialized
|                   - canTxCurHandle[logTxObjHandle] must be set
|                   - the hardwareObject (txObjHandle) must be free
| INPUT PARAMETERS: txHandle: Handle of the transmit object to be send
|                   txObjHandle:  Nr of the HardwareObjects to use
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
/* PRQA S 2001 ++ */    /* suppress misra message about usage of goto */
/* CODE CATEGORY 1 START*/
static vuint8 CanCopyDataAndStartTransmission( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle, CanTransmitHandle txHandle) C_API_3   /*lint !e14 !e31*/
{
   CanDeclareGlobalInterruptOldStatus
   vuint8             rc;
   CanObjectHandle      logTxObjHandle;
#  if defined( C_ENABLE_COPY_TX_DATA )  
   TxDataPtr   CanMemCopySrcPtr;
#  endif
# if defined( C_ENABLE_DYN_TX_OBJECTS )
   CanTransmitHandle    dynTxObj;
# endif /* C_ENABLE_DYN_TX_OBJECTS */
# if defined( C_ENABLE_PRETRANSMIT_FCT )
   CanTxInfoStruct      txStruct;
# endif

   V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
   assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);      /* PRQA S 3109 */
# endif
   assertInternal(txHandle < kCanNumberOfTxObjects, kCanAllChannels , kErrorInternalTxHdlTooLarge);        /* PRQA S 3109 */

# if defined( C_ENABLE_DYN_TX_OBJECTS )
#  if ( kCanNumberOfTxStatObjects == 0)
   dynTxObj = txHandle;               /* only dynamic messages are used */
#  else   /* ( kCanNumberOfTxStatObjects == 0) */
   /* dynamic and static messages are in the system */
   if (txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
   {
     dynTxObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);    /* PRQA S 3382,0291 */
   }
   else
   {
     dynTxObj = kCanTxHandleNotUsed;
   }
#  endif /* ( kCanNumberOfTxStatObjects == 0) */
# endif /* C_ENABLE_DYN_TX_OBJECTS */

   assertInternal(txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel), channel, kErrorTxObjHandleWrong);      /* PRQA S 3109 */
   logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

   assertHardware( CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ), channel, kErrorTxBufferBusy);          /* PRQA S 3109 */

   pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[txObjHandle];
   pCanCellMailbox->MoCtrL = C_MOCTR_TXEN0; /* delete enable flag */

   /* set id and dlc  -------------------------------------------------------- */
   {
# if defined( C_ENABLE_DYN_TX_DLC ) || \
      defined( C_ENABLE_DYN_TX_ID )
     if (dynTxObj < kCanNumberOfTxDynObjects)
     {           /* set dynamic part of dynamic objects ----------------------*/
#  if defined( C_ENABLE_DYN_TX_ID )
        pCanCellMailbox->MoARHi  = canDynTxId0[dynTxObj];
        # if defined( C_ENABLE_EXTENDED_ID )
        pCanCellMailbox->MoARLo  = canDynTxId1[dynTxObj];
        # endif
#  endif
 
#  if defined( C_ENABLE_DYN_TX_DLC )
        pCanCellMailbox->MoFCR_DLC = (canDynTxDLC[dynTxObj]) & C_MOFCR_DLC;
#  endif
     }
     else
     {          /* set part of static objects assocciated the dynamic --------*/
#  if defined( C_ENABLE_DYN_TX_ID )
#   if defined( C_ENABLE_TX_MASK_EXT_ID )
#    if defined( C_ENABLE_MIXED_ID )
        if (CanGetTxIdType(txHandle)==kCanIdTypeStd)
        {
          pCanCellMailbox->MoARHi  = CanGetTxId0(txHandle);
          # if defined( C_ENABLE_EXTENDED_ID )
          pCanCellMailbox->MoARLo  = CanGetTxId1(txHandle);
          # endif
        }
        else
#    endif
        {
          /* mask extened ID */
          pCanCellMailbox->MoARHi  = (CanGetTxId0(txHandle)|canTxMask0[channel]);
          # if defined( C_ENABLE_EXTENDED_ID )
          pCanCellMailbox->MoARLo  = (CanGetTxId1(txHandle)|canTxMask1[channel]);
          # endif
        }
#   else
        pCanCellMailbox->MoARHi  = CanGetTxId0(txHandle);
        # if defined( C_ENABLE_EXTENDED_ID )
        pCanCellMailbox->MoARLo  = CanGetTxId1(txHandle);
        # endif
#   endif
#  endif

#  if defined( C_ENABLE_DYN_TX_DLC )
#   if defined( C_ENABLE_VARIABLE_DLC )
        /* init DLC, RAM */
        pCanCellMailbox->MoFCR_DLC = (canTxDLC_RAM[txHandle]) & C_MOFCR_DLC;
#   else
        /* init DLC, ROM */
        pCanCellMailbox->MoFCR_DLC = CanGetTxDlc(txHandle) & C_MOFCR_DLC;
#   endif
#  endif
     }
# endif
     /* set static part commen for static and dynamic objects ----------------*/
# if defined( C_ENABLE_DYN_TX_ID )
# else
#  if defined( C_ENABLE_TX_MASK_EXT_ID )
#   if defined( C_ENABLE_MIXED_ID )
     if (CanGetTxIdType(txHandle)==kCanIdTypeStd)
     {
       pCanCellMailbox->MoARHi  = CanGetTxId0(txHandle);
       # if defined( C_ENABLE_EXTENDED_ID )
       pCanCellMailbox->MoARLo  = CanGetTxId1(txHandle);
       # endif
     }
     else
#   endif
     {
       /* mask extened ID */
       pCanCellMailbox->MoARHi  = (CanGetTxId0(txHandle)|canTxMask0[channel]);
       # if defined( C_ENABLE_EXTENDED_ID )
       pCanCellMailbox->MoARLo  = (CanGetTxId1(txHandle)|canTxMask1[channel]);
       # endif
     }
#  else
     pCanCellMailbox->MoARHi  = CanGetTxId0(txHandle);
     # if defined( C_ENABLE_EXTENDED_ID )
     pCanCellMailbox->MoARLo  = CanGetTxId1(txHandle);
     # endif
#  endif
# endif
# if defined( C_ENABLE_DYN_TX_DLC )
# else
#  if defined( C_ENABLE_VARIABLE_DLC )
     /* init DLC, RAM */
     pCanCellMailbox->MoFCR_DLC = (canTxDLC_RAM[txHandle]) & C_MOFCR_DLC;
#  else
     /* init DLC, ROM */
     pCanCellMailbox->MoFCR_DLC = CanGetTxDlc(txHandle) & C_MOFCR_DLC;
#  endif
# endif

# if defined( C_ENABLE_MIXED_ID )
#  if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#  else
#   if defined( C_ENABLE_DYN_TX_DLC ) || \
       defined( C_ENABLE_DYN_TX_ID )
     if (dynTxObj < kCanNumberOfTxDynObjects)
     {                      /* set dynamic part of dynamic objects */
#    if defined( C_ENABLE_DYN_TX_ID )
       if (canDynTxIdType[dynTxObj] == C_EID_MASK_16) /* lint !e506 !e774 */
       {
         pCanCellMailbox->MoARHi |= C_EID_MASK_16;
       }
       else
       {
         pCanCellMailbox->MoARHi &= C_EID_NOT_MASK_16;
       }
#    else
       if (CanGetTxIdType(txHandle) == C_EID_MASK_16) /* lint !e506 !e774 */
       {
         pCanCellMailbox->MoARHi |= C_EID_MASK_16;
       }
       else
       {
         pCanCellMailbox->MoARHi &= C_EID_NOT_MASK_16;
       }
#    endif
     }
#   else
#   endif
#  endif
# endif

   }


 /* call pretransmit function ----------------------------------------------- */
# if defined( C_ENABLE_PRETRANSMIT_FCT )

   /* pointer needed for other modules */
   (txStruct.pChipData) = (CanChipDataPtr) (&(pCanCellMailbox->MoDR[0]));
   canRDSTxPtr[logTxObjHandle] = txStruct.pChipData;
   txStruct.Handle      = txHandle;

   {  
    /* Is there a PreTransmit function ? ------------------------------------- */ 
    if ( CanGetApplPreTransmitPtr(txHandle) != V_NULL )    /* if PreTransmit exists */ 
    { 
      if ( (CanGetApplPreTransmitPtr(txHandle)) (txStruct) == kCanNoCopyData)  
      { 
      
        
        /* Do not copy the data - already done by the PreTransmit-function */  
        /* --- start transmission --- */  
        goto startTransmission; 
      } 
    } 
   }  
# endif /* C_ENABLE_PRETRANSMIT_FCT */

 /* copy data --------------------------------------------------------------- */
# if defined( C_ENABLE_COPY_TX_DATA )  
#  if defined( C_ENABLE_DYN_TX_DATAPTR )  
   if (dynTxObj < kCanNumberOfTxDynObjects)  
   {  
      CanMemCopySrcPtr = canDynTxDataPtr[dynTxObj];  
   }  
   else  
#  endif  
   {  
     CanMemCopySrcPtr = CanGetTxDataPtr(txHandle);  
   }  
 /* copy via index in MsgObj data field, copy always 8 bytes -----------*/  
   if ( CanMemCopySrcPtr != V_NULL )   /* copy if buffer exists */
   {
#  if C_SECURITY_LEVEL > 10
     CanNestedGlobalInterruptDisable();  
#  endif

     { /* - do not copy data with 32bit access because of failure with GNU compiler
          - do not use 32bit copy because of possible 8bit allignment for application data */
       vuint8           canllidx;
       CanChipDataPtr   canll_dst;
       canll_dst = (CanChipDataPtr) (&(pCanCellMailbox->MoDR[0]));
       for (canllidx = 0; canllidx < 8; canllidx++)
       {
         canll_dst[canllidx] = CanMemCopySrcPtr[canllidx];
       }
     }

#  if C_SECURITY_LEVEL > 10
     CanNestedGlobalInterruptRestore();  
#  endif
   }
# endif /* ( C_ENABLE_COPY_TX_DATA ) */  

   CANDRV_SET_CODE_TEST_POINT(0x10A);

# if defined( C_ENABLE_PRETRANSMIT_FCT )
/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
startTransmission:
# endif

   /* test offline and handle and start transmission ------------------------ */
   CanNestedGlobalInterruptDisable();  
   /* If CanTransmit was interrupted by a re-initialization or CanOffline */  
   /* no transmitrequest of this action should be started      */  
   if ((canHandleCurTxObj[logTxObjHandle] == txHandle) && ((canStatus[channel] & kCanTxOn) == kCanTxOn))
   {  
     #if defined(C_ENABLE_WORKAROUND_TC21)
     CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM].MoCtrL = C_MOCTR_RTSEL; /* delete sel bit */
     CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM].MoCtrH = C_MOCTR_TXEN0 | C_MOCTR_TXEN1 | 
                                                                       C_MOCTR_TXREQ | C_MOCTR_NEWDAT;
     #endif
     pCanCellMailbox->MoCtrL = C_MOCTR_RTSEL; /* delete sel bit */
     pCanCellMailbox->MoCtrH = C_MOCTR_TXEN0 | C_MOCTR_TXEN1 | C_MOCTR_TXREQ | C_MOCTR_NEWDAT;

# if defined( C_ENABLE_TX_OBSERVE )
     ApplCanTxObjStart( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
# endif
     rc = kCanTxOk;                                    
   }  
   else  
   {  
# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
     if (canHandleCurTxObj[logTxObjHandle] == txHandle)
     {
       /* only CanOffline was called on higher level */
       rc = kCanTxNotify;
     }
     else
# endif
     {
       rc = kCanTxFailed;   
     }
     assertInternal((canHandleCurTxObj[logTxObjHandle] == txHandle) || (canHandleCurTxObj[logTxObjHandle] == kCanBufferFree),
                                                                                       channel, kErrorTxHandleWrong);  /* PRQA S 3109 */
     canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* release TxHandle (CanOffline) */
   }  

   CanNestedGlobalInterruptRestore();

   
   return (rc);   

} /* END OF CanCopyDataAndStartTransmission */
/* CODE CATEGORY 1 END*/
/* PRQA S 2001 ++ */    /* suppress misra message about usage of goto */
#endif /* if defined( C_ENABLE_CAN_TRANSMIT ) */


#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING ) 
/****************************************************************************
| NAME:             CanTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling error bus off
|                   - polling Tx objects
|                   - polling Rx objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTask(void)
{
  CAN_CHANNEL_CANTYPE_LOCAL

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  for (channel = 0; channel < kCanNumberOfChannels; channel++)
# endif
  {
    {
# if defined( C_ENABLE_ERROR_POLLING )
      CanErrorTask(CAN_CHANNEL_CANPARA_ONLY);
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
# endif

# if defined( C_ENABLE_TX_POLLING ) 
      CanTxTask(CAN_CHANNEL_CANPARA_ONLY);
# endif

# if defined( C_ENABLE_RX_FULLCAN_POLLING ) && \
     defined( C_ENABLE_RX_FULLCAN_OBJECTS )
      CanRxFullCANTask(CAN_CHANNEL_CANPARA_ONLY);
# endif

# if defined( C_ENABLE_RX_BASICCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_BASICCAN_POLLING )
      CanRxBasicCANTask(CAN_CHANNEL_CANPARA_ONLY);
# endif
    }
  }
}
/* CODE CATEGORY 2 END*/
#endif

#if defined( C_ENABLE_ERROR_POLLING )
/****************************************************************************
| NAME:             CanErrorTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling error status
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanErrorTask( CAN_CHANNEL_CANTYPE_ONLY )
{

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);     /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

    {
# if defined( C_ENABLE_SLEEP_WAKEUP )
      if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
      {
        CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
        CanHL_ErrorHandling(CAN_HW_CHANNEL_CANPARA_ONLY);
        CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
      }
    }

    canPollingTaskActive[channel] = 0;
  }
}
/* CODE CATEGORY 2 END*/
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )
#endif /* C_ENABLE_SLEEP_WAKEUP */

#if defined( C_ENABLE_TX_POLLING ) 
/****************************************************************************
| NAME:             CanTxTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling Tx objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxTask( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3
{
  CanObjectHandle      txObjHandle;



# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);    /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;


# if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
    {
      /*--  polling Tx objects ----------------------------------------*/

# if defined( C_ENABLE_TX_POLLING )
      /* check for global confirmation Pending and may be reset global pending confirmation */
      {
        for ( txObjHandle = CAN_HL_HW_TX_STARTINDEX(canHwChannel); txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel) ; txObjHandle++ )
        {
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
          if ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] != (vuint8)0 )
#  endif
          {
            /* check for dedicated confirmation pending */
            if( (CANLL_CELL_PTR->Obj[txObjHandle].MoCtrL & C_MOCTR_TXPND)==C_MOCTR_TXPND ) /* lint !e737 */
            {
              CANDRV_SET_CODE_TEST_POINT(0x110);
              CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
              /* do tx confirmation */
              CanHL_TxConfirmation(CAN_HW_CHANNEL_CANPARA_FIRST txObjHandle );
              CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
            }
          } /* if individual polling & object has to be polled */
        }
      }
# endif /*( C_ENABLE_TX_POLLING ) */


# if defined( C_ENABLE_TRANSMIT_QUEUE )
      CanHl_RestartTxQueue( CAN_CHANNEL_CANPARA_ONLY );
# endif /*  C_ENABLE_TRANSMIT_QUEUE */

    } /* if ( CanLL_HwIsSleep... ) */

    canPollingTaskActive[channel] = 0;
  }


} /* END OF CanTxTask */
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_TX_POLLING */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_TX_POLLING ) 
/****************************************************************************
| NAME:             CanHl_RestartTxQueue
| CALLED BY:        CanTxTask, via CanLL (TxMsgDestroyed)
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      start transmission if queue entry exists and HW is free
****************************************************************************/
/* CODE CATEGORY 2 START*/
static void CanHl_RestartTxQueue( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanTransmitHandle    txHandle;
#  if  defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  vuint8             rc;
#  endif
  CanDeclareGlobalInterruptOldStatus

  CanSignedTxHandle         queueElementIdx;    /* use as signed due to loop */
  CanSignedTxHandle         elementBitIdx;
  tCanQueueElementType             elem;

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);      /* PRQA S 3109 */
#  endif


  if (canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] == kCanBufferFree)
  {
    for(queueElementIdx = CAN_HL_TXQUEUE_STOPINDEX(channel) - (CanSignedTxHandle)1; 
                             queueElementIdx >= CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx--)
    {
      elem = canTxQueueFlags[queueElementIdx];
      if(elem != (tCanQueueElementType)0) /* is there any flag set in the queue element? */
      {

        /* Transmit Queued Objects */
        /* start the bitsearch */
        #if defined( C_CPUTYPE_32BIT )
        if((elem & (tCanQueueElementType)0xFFFF0000) != (tCanQueueElementType)0)
        {
          if((elem & (tCanQueueElementType)0xFF000000) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0xF0000000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 28] + 28;
            }
            else /*0x0F000000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 24] + 24;
            }
          }
          else
          {
            if((elem & (tCanQueueElementType)0x00F00000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 20] + 20;
            }
            else /* 0x000F0000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 16] + 16;
            }
          }
        }
        else
        #endif
        {
        #if defined( C_CPUTYPE_32BIT ) 
          if((elem & (tCanQueueElementType)0x0000FF00) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0x0000F000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 12] + 12;
            }
            else /*0x00000F00*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 8] + 8;
            }
          }
          else
        #endif
          {
            if((elem & (tCanQueueElementType)0x000000F0) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 4] + 4;
            }
            else /* 0x0000000F*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem];
            }
          }
        }
        txHandle = (CanTransmitHandle)((((CanTransmitHandle)queueElementIdx << kCanTxQueueShift) + (CanTransmitHandle)elementBitIdx) - CAN_HL_TXQUEUE_PADBITS(channel));
        {

            CanNestedGlobalInterruptDisable();
            if (canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] == kCanBufferFree)
            {
              if ( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
              {
                canTxQueueFlags[queueElementIdx] &= ~CanShiftLookUp[elementBitIdx]; /* clear flag from the queue */
                /* Save hdl of msgObj to be transmitted*/
                canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] = txHandle;
                CanNestedGlobalInterruptRestore();  
#  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
                rc = CanCopyDataAndStartTransmission(CAN_CHANNEL_CANPARA_FIRST CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel), txHandle);
                if ( rc == kCanTxNotify)
                {
                  APPLCANCANCELNOTIFICATION(channel, txHandle);
                }
#  else
                (void)CanCopyDataAndStartTransmission(CAN_CHANNEL_CANPARA_FIRST CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel), txHandle);
#  endif
                return;
              }

            }
            CanNestedGlobalInterruptRestore();
            return;
        }
      }
    }
  }
} /* End of CanHl_RestartTxQueue */
/* CODE CATEGORY 2 END*/
# endif
#endif /*  C_ENABLE_TRANSMIT_QUEUE */

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  
/****************************************************************************
| NAME:             CanRxFullCANTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling Rx FullCAN objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxFullCANTask(CAN_CHANNEL_CANTYPE_ONLY) C_API_3
{

  CanObjectHandle     rxObjHandle;        /* keyword register removed 2005-06-29 Ht */


# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);    /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

    {
# if defined( C_ENABLE_SLEEP_WAKEUP )
      if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
      {
        /*--  polling fullcan Rx objects ----------------------------------------*/

        /* check for global fullCan Rx indication pending and may be reset global */
        /* indication pending */
        { /* no global basic flag - only basic or n-fullcan availabel */
          for (rxObjHandle=CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel); rxObjHandle<CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel); rxObjHandle++ )
          {
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
            if ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][rxObjHandle] != (vuint8)0 )
# endif
            {
              /* check for dedicated indication pending */
              if( (CANLL_CELL_PTR->Obj[rxObjHandle].MoCtrL & C_MOCTR_RXPND)==C_MOCTR_RXPND )
              {
                CANDRV_SET_CODE_TEST_POINT(0x109);
                CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
                CanFullCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
                CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
              }
            }  /* if individual polling & object has to be polled */
          } /* for ( txObjHandle ) */
        } /* if (global pending) */
      } /* if ( CanLL_HwIsSleep ... )  */
    }  /* for (all associated HW channel) */
    canPollingTaskActive[channel] = 0;
  }

} /* END OF CanRxTask */
/* CODE CATEGORY 2 END*/
#endif /*  C_ENABLE_RX_FULLCAN_OBJECTS && C_ENABLE_RX_FULLCAN_POLLING */

#if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanRxBasicCANTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling Rx BasicCAN objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANTask(CAN_CHANNEL_CANTYPE_ONLY) C_API_3
{

  CanObjectHandle     rxObjHandle;          /* keyword register removed 2005-06-29 Ht */


# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);   /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;


# if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
    {

      { /* no global basic flag - only basic or n-fullcan availabel */
        for (rxObjHandle=CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); rxObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); rxObjHandle++ )
        {
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
          if ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][rxObjHandle] != (vuint8)0 )
# endif
          {
            /* check for dedicated indication pending */
            if( (CANLL_CELL_PTR->Obj[rxObjHandle].MoCtrL & C_MOCTR_RXPND)==C_MOCTR_RXPND )
            { /* basic Rx is pending */
              CANDRV_SET_CODE_TEST_POINT(0x108);

              CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
              CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
              CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
            }
          } /* if individual polling & object has to be polled */
        }
      }
    } /* if ( CanLL_HwIsSleep... )  */

    canPollingTaskActive[channel] = 0;
  }

} /* END OF CanRxTask */
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_RX_BASICCAN_POLLING && C_ENABLE_RX_BASICCAN_OBJECTS */

/****************************************************************************
| NAME:             CanHL_ErrorHandling
| CALLED BY:        CanISR(), CanErrorTask()
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - error interrupt (busoff, error warning,...)
****************************************************************************/
/* CODE CATEGORY 2 START*/
static void CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);      /* PRQA S 3109 */
#endif




  /* check for status register (bus error)--*/
  if( (canllBusOffNotified[canHwChannel]==CANLL_FALSE) && \
      ((CANLL_CELL_PTR->Node[CAN_NODE_NUM].NCR & C_NCR_INIT)==C_NCR_INIT) && \
      (canllHalt[canHwChannel]==CANLL_FALSE)  )
  { /* don't use busoff-flag here -> after resync deleted */
    canllBusOffNotified[canHwChannel] = CANLL_TRUE; /* lint !e661 */
    /*==BUS OFF ERROR=========================*/
    APPL_CAN_BUSOFF( CAN_CHANNEL_CANPARA_ONLY );            /* call application specific function */
  }


} /* END OF CanHL_ErrorHandling */
/* CODE CATEGORY 2 END*/


#if defined( C_ENABLE_INDIVIDUAL_POLLING )
# if defined( C_ENABLE_TX_POLLING )
/****************************************************************************
| NAME:             CanTxObjTask()
| CALLED BY:        
| PRECONDITIONS:    
| INPUT PARAMETERS: CAN_HW_CHANNEL_CANTYPE_FIRST 
|                   CanObjectHandle txObjHandle
| RETURN VALUES:    none
| DESCRIPTION:      Polling special Object
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle)      /* PRQA S 1330 */
{



#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorChannelHdlTooLarge);  /* PRQA S 3109 */
#  endif
  assertUser(txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel), channel, kErrorHwHdlTooLarge);     /* PRQA S 3109 */
  assertUser((CanSignedTxHandle)txObjHandle >= (CanSignedTxHandle)CAN_HL_HW_TX_STARTINDEX(canHwChannel), channel, kErrorHwHdlTooSmall);   /* PRQA S 3109 */ /*lint !e568 */
  assertUser(CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] != (vuint8)0, channel, kErrorHwObjNotInPolling);    /* PRQA S 3109 */
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);     /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

#  if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
#  endif
    {

      /* check for dedicated confirmation pending */
      if( (CANLL_CELL_PTR->Obj[txObjHandle].MoCtrL & C_MOCTR_TXPND)==C_MOCTR_TXPND ) /* lint !e737 */
      {
        CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
        /* do tx confirmation */
        CanHL_TxConfirmation(CAN_HW_CHANNEL_CANPARA_FIRST txObjHandle );
        CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
      }

#  if defined( C_ENABLE_TRANSMIT_QUEUE )
      if ( txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(channel) )
      {
        CanHl_RestartTxQueue( CAN_CHANNEL_CANPARA_ONLY );
      }
#  endif /*  C_ENABLE_TRANSMIT_QUEUE */
    }

    canPollingTaskActive[channel] = 0;
  }
} /*CanTxObjTask*/
/* CODE CATEGORY 2 END*/
# endif    /* defined( C_ENABLE_INDIVIDUAL_POLLING ) && defined( C_ENABLE_TX_POLLING ) */

# if defined( C_ENABLE_RX_FULLCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  
/****************************************************************************
| NAME:             CanRxFullCANObjTask()
| CALLED BY:        
| PRECONDITIONS:    
| INPUT PARAMETERS: CAN_HW_CHANNEL_CANTYPE_FIRST 
|                   CanObjectHandle rxObjHandle
| RETURN VALUES:    none
| DESCRIPTION:      Polling special Object
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxFullCANObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle)      /* PRQA S 1330 */
{



#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
#  endif
  assertUser(rxObjHandle < CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel), channel, kErrorHwHdlTooLarge);   /* PRQA S 3109 */
  assertUser((CanSignedRxHandle)rxObjHandle >= (CanSignedRxHandle)CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel), channel, kErrorHwHdlTooSmall);  /* PRQA S 3109 */ /*lint !e568 */
  assertUser(CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][rxObjHandle] != (vuint8)0, channel, kErrorHwObjNotInPolling);   /* PRQA S 3109 */
  assertUser((canPollingTaskActive[channel] == 0), channel, kErrorPollingTaskRecursion);   /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

#  if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
#  endif
    {
      /* check for dedicated indication pending */
      if( (CANLL_CELL_PTR->Obj[rxObjHandle].MoCtrL & C_MOCTR_RXPND)==C_MOCTR_RXPND )
      {
        CANDRV_SET_CODE_TEST_POINT(0x109);
        CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
        CanFullCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
        CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
      }
    }
    canPollingTaskActive[channel] = 0;
  }
} /*CanRxFullCANObjTask*/
/* CODE CATEGORY 2 END*/
# endif

# if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanRxBasicCANObjTask()
| CALLED BY:        
| PRECONDITIONS:    
| INPUT PARAMETERS: CAN_HW_CHANNEL_CANTYPE_FIRST 
|                   CanObjectHandle rxObjHandle
| RETURN VALUES:    none
| DESCRIPTION:      Polling special Object
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle)        /* PRQA S 1330 */
{
#  if kCanNumberOfHwObjPerBasicCan > 1
  CanObjectHandle i;
#  endif



#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
#  endif
  assertUser(rxObjHandle < CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel), channel, kErrorHwHdlTooLarge);  /* PRQA S 3109 */
  assertUser((CanSignedRxHandle)rxObjHandle >= (CanSignedRxHandle)CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel), channel, kErrorHwHdlTooSmall); /* PRQA S 3109 */ /*lint !e568 */
  assertUser(CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][rxObjHandle] != (vuint8)0, channel, kErrorHwObjNotInPolling);   /* PRQA S 3109 */
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);  /* PRQA S 3109 */

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

#  if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY) )
#  endif
    {

#  if kCanNumberOfHwObjPerBasicCan > 1
      /* loop over all HW objects assigned to one BasicCAN  */
      for (i = 0; i < kCanNumberOfHwObjPerBasicCan; i++)
      {
#  endif
        /* check for dedicated indication pending */
        if( (CANLL_CELL_PTR->Obj[rxObjHandle].MoCtrL & C_MOCTR_RXPND)==C_MOCTR_RXPND )
        { /* basic Rx is pending */
          CANDRV_SET_CODE_TEST_POINT(0x109);
          CAN_CAN_INTERRUPT_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
          CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
          CAN_CAN_INTERRUPT_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
        }
#  if kCanNumberOfHwObjPerBasicCan > 1
        rxObjHandle++;
      }
#  endif
    }
    canPollingTaskActive[channel] = 0;
  }
} /*CanRxBasicCANObjTask*/
/* CODE CATEGORY 2 END*/
# endif
#endif /*C_ENABLE_INDIVIDUAL_POLLING*/

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanBasicCanMsgReceived
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: internal can chip number
| RETURN VALUES:    none
| DESCRIPTION:      - basic can receive
****************************************************************************/
/* PRQA S 2001 ++ */    /* suppress misra message about usage of goto */
/* CODE CATEGORY 1 START*/
static void CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle)
{
# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
# endif  
  
# if ( !defined( C_SEARCH_HASH ) )  ||\
     defined( C_ENABLE_RANGE_0 ) || \
     defined( C_ENABLE_RANGE_1 ) || \
     defined( C_ENABLE_RANGE_2 ) || \
     defined( C_ENABLE_RANGE_3 )
  tCanRxId0 idRaw0;
#  if (kCanNumberOfUsedCanRxIdTables > 1)
  tCanRxId1 idRaw1;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 2)
  tCanRxId2 idRaw2;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 3)
  tCanRxId3 idRaw3;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 4)
  tCanRxId4 idRaw4;
#  endif
# endif

# if defined( C_SEARCH_HASH )
#  if (kCanNumberOfRxBasicCANObjects > 0)
#   if (kHashSearchListCountEx > 0)
  vuint32          idExt;
#    if (kHashSearchListCountEx > 1)
  vuint32          winternExt;        /* prehashvalue         */
#    endif
#   endif
#   if (kHashSearchListCount > 0)
  vuint16          idStd;
#    if (kHashSearchListCount > 1)
  vuint16          winternStd;        /* prehashvalue         */
#    endif
#   endif
#   if (((kHashSearchListCountEx > 1) && (kHashSearchMaxStepsEx > 1)) ||\
        ((kHashSearchListCount > 1)   && (kHashSearchMaxSteps > 1))) 
  vuint16          i_increment;    /* delta for next step  */
  vsint16          count;
#   endif
#  endif  /* kCanNumberOfRxBasicCANObjects > 0 */
# endif

  #if defined( C_ENABLE_OVERRUN )
  vuint8 copy_cnt;
  #endif
  vuint8 canPndReg;
  vuint32 canPndBit;
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  vuint8 canHwLoopRetVal;
  #endif
  V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;

# if defined( C_ENABLE_GENERIC_PRECOPY ) || \
    defined( C_ENABLE_PRECOPY_FCT )     || \
    defined( C_ENABLE_COPY_RX_DATA )    || \
    defined( C_ENABLE_INDICATION_FLAG ) || \
    defined( C_ENABLE_INDICATION_FCT )  || \
    defined( C_ENABLE_DLC_CHECK )       || \
    defined( C_ENABLE_NOT_MATCHED_FCT )
#  if (kCanNumberOfRxBasicCANObjects > 0)

  CanReceiveHandle         rxHandle;

  rxHandle = kCanRxHandleNotUsed;

#  endif /* kCanNumberOfRxBasicCANObjects > 0 */
# endif

  CANDRV_SET_CODE_TEST_POINT(0x100);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);  /* PRQA S 3109 */
# endif

  pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[rxObjHandle];
  #if defined( C_ENABLE_OVERRUN )
  copy_cnt  = 0;
  #endif
  canPndReg = (vuint8)(rxObjHandle >> 5);
  canPndBit = (vuint32)1 << (rxObjHandle & 0x1F);
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerStart(kCanLoopRx);
  #endif
  do
  { /* repeat until consistent data , msgval should not be used here */
    pCanCellMailbox->MoCtrL = (C_MOCTR_RXPND | C_MOCTR_NEWDAT); /* delete pending flag */
    CANLL_CELL_PTR->Modul.MSPND[canPndReg] = ~canPndBit;        /* delete pending interrupt */
  #if defined( C_ENABLE_OVERRUN )
    copy_cnt++;
  #endif
    TmpRxMailbox.MoARHi    = pCanCellMailbox->MoARHi;
    TmpRxMailbox.MoARLo    = pCanCellMailbox->MoARLo;
    TmpRxMailbox.MoFCR_DLC = pCanCellMailbox->MoFCR_DLC;
    TmpRxMailbox.MoDR[0]   = pCanCellMailbox->MoDR[0];
    TmpRxMailbox.MoDR[1]   = pCanCellMailbox->MoDR[1];
  #if defined( C_ENABLE_HW_LOOP_TIMER )
    canHwLoopRetVal = CanLL_ApplCanTimerLoop(kCanLoopRx);
  } while( ((pCanCellMailbox->MoCtrL & (C_MOCTR_NEWDAT | C_MOCTR_RXUPD)) != (vuint16)0x0000) && (canHwLoopRetVal != 0) );
  #else
  } while((pCanCellMailbox->MoCtrL & (C_MOCTR_NEWDAT | C_MOCTR_RXUPD)) != (vuint16)0x0000);
  #endif
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerEnd(kCanLoopRx);
  #endif
  #if defined( C_ENABLE_OVERRUN )
  if (copy_cnt > 1) 
  {
    pCanCellMailbox->MoCtrH = C_MOCTR_MSGLOST; /* set overrun bit */
  }
  #endif
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  if(canHwLoopRetVal == 0)
  { /* do not proceed when data is inconsistent (no notification) */
    goto finishBasicCan;
  }
  #endif

# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
  (pCanRxInfoStruct->pChipMsgObj) = pTmpRxMailboxMsg;
  (pCanRxInfoStruct->pChipData)   = pTmpRxMailboxData;
  canRDSRxPtr[channel] = pCanRxInfoStruct->pChipData;
# else
  (canRxInfoStruct[channel].pChipMsgObj) = pTmpRxMailboxMsg;
  (canRxInfoStruct[channel].pChipData)   = pTmpRxMailboxData;
  canRDSRxPtr[channel] = canRxInfoStruct[channel].pChipData;
# endif
  CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)      = kCanRxHandleNotUsed;  

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    goto finishBasicCan; /* ignore reception */
  }
# endif

#  if defined( C_ENABLE_OVERRUN )
  if( (pCanCellMailbox->MoCtrL & C_MOCTR_MSGLOST)==C_MOCTR_MSGLOST )
  {
    pCanCellMailbox->MoCtrL = C_MOCTR_MSGLOST; /* delete overrun */
    ApplCanOverrun( CAN_CHANNEL_CANPARA_ONLY );
  }
#  endif

  /**************************** reject remote frames  ****************************************/


  /**************************** reject messages with unallowed ID type ****************************************/


# if defined( C_ENABLE_COND_RECEIVE_FCT )
  /**************************** conditional message receive function  ****************************************/
  if(canMsgCondRecState[channel]==kCanTrue)
  {
    ApplCanMsgCondReceived( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
# endif

# if defined( C_ENABLE_RECEIVE_FCT )
  /**************************** call ApplCanMsgReceived() ****************************************************/
  if (APPL_CAN_MSG_RECEIVED( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
  {
    goto finishBasicCan;
  }
# endif
  

# if ( !defined( C_SEARCH_HASH ) ) || \
     defined( C_ENABLE_RANGE_0 ) || \
     defined( C_ENABLE_RANGE_1 ) || \
     defined( C_ENABLE_RANGE_2 ) || \
     defined( C_ENABLE_RANGE_3 )
  /**************************** calculate idRaw for filtering ************************************************/
#  if defined( C_ENABLE_EXTENDED_ID )
#   if defined( C_ENABLE_MIXED_ID )
  if (CanRxActualIdType(CAN_HL_P_RX_INFO_STRUCT(channel)) == kCanIdTypeExt)
#   endif
  {
#   if defined( C_ENABLE_RX_MASK_EXT_ID )
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID0(C_MASK_EXT_ID);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID1(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID2(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID3(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID4(C_MASK_EXT_ID);
#    endif
#   else
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID0(0x1FFFFFFF);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID1(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID2(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID3(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID4(0x1FFFFFFF);
#    endif
#   endif /*  C_ENABLE_RX_MASK_EXT_ID */
  }
#   if defined( C_ENABLE_MIXED_ID )
  else
  {
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID0(0x7FF);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID1(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID2(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID3(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID4(0x7FF);
#    endif
  }
#   endif
#  else /* C_ENABLE_EXTENDED_ID */
  {
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID0(0x7FF);
#   if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID1(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID2(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID3(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID4(0x7FF);
#   endif
  }
#  endif /* C_ENABLE_EXTENDED_ID */
# endif /* ( !defined( C_SEARCH_HASH ) && ...  defined( C_ENABLE_RANGE_3 )*/

  /******************* Range filtering ********************************************************************/

  {
#  if defined( C_ENABLE_RANGE_0 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange0) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE0IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (C_RANGE0_IDTYPE == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
#    else  /* C_RANGE_0_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange0[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange0;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE0PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_0 */

#  if defined( C_ENABLE_RANGE_1 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange1) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE1IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (C_RANGE1_IDTYPE == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
#    else  /* C_RANGE_1_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange1[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange1;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE1PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_1 */

#  if defined( C_ENABLE_RANGE_2 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange2) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE2IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (C_RANGE2_IDTYPE == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
#    else  /* C_RANGE_2_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange2[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange2;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE2PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_2 */

#  if defined( C_ENABLE_RANGE_3 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange3) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE3IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (C_RANGE3_IDTYPE == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
#    else  /* C_RANGE_3_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange3[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange3;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE3PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_3 */

  }


# if defined( C_ENABLE_GENERIC_PRECOPY ) || \
    defined( C_ENABLE_PRECOPY_FCT )     || \
    defined( C_ENABLE_COPY_RX_DATA )    || \
    defined( C_ENABLE_INDICATION_FLAG ) || \
    defined( C_ENABLE_INDICATION_FCT )  || \
    defined( C_ENABLE_DLC_CHECK )       || \
    defined( C_ENABLE_NOT_MATCHED_FCT )
# if (kCanNumberOfRxBasicCANObjects > 0)

   /* search the received id in ROM table: */


#  if defined( C_SEARCH_LINEAR )
  /* ************* Linear search ******************************************** */
  for (rxHandle = CAN_HL_RX_BASIC_STARTINDEX(channel); rxHandle < CAN_HL_RX_BASIC_STOPINDEX(channel) ;rxHandle++)
  {
    if( idRaw0 == CanGetRxId0(rxHandle) )
    {
#   if (kCanNumberOfUsedCanRxIdTables > 1)
      if( idRaw1 == CanGetRxId1(rxHandle) )
#   endif
      {
#   if (kCanNumberOfUsedCanRxIdTables > 2)
        if( idRaw2 == CanGetRxId2(rxHandle) )
#   endif
        {
#   if (kCanNumberOfUsedCanRxIdTables > 3)
          if( idRaw3 == CanGetRxId3(rxHandle) )
#   endif
          {
#   if (kCanNumberOfUsedCanRxIdTables > 4)
            if( idRaw4 == CanGetRxId4(rxHandle) )
#   endif
            {
#   if defined( C_ENABLE_MIXED_ID ) 
#    if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#    else
              /* verify ID type, if not already done with the ID raw */
              if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CanGetRxIdType(rxHandle))
#    endif
#   endif
              {
                break;    /*exit loop with index rxHandle */
              }
            }
          }
        }
      }
    }
  }
#  endif

#  if defined( C_SEARCH_HASH )
  /* ************* Hash search ********************************************* */
    
#   if defined( C_ENABLE_EXTENDED_ID ) 
  /* one or more Extended ID listed */
#    if defined( C_ENABLE_MIXED_ID )
  if((CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) )) == kCanIdTypeExt)
#    endif
#    if (kHashSearchListCountEx > 0)
  {
  /* calculate the logical ID */
#     if defined( C_ENABLE_RX_MASK_EXT_ID )
    idExt          = (CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) ) &  C_MASK_EXT_ID ) | \
                                                                               ((vuint32)channel << 29);                
#     else
    idExt          = CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) )| ((vuint32)channel << 29);
#     endif

#     if (kHashSearchListCountEx == 1)
    rxHandle       = (CanReceiveHandle)0;
#     else
    winternExt     = idExt + kHashSearchRandomNumberEx;                    
    rxHandle       = (CanReceiveHandle)(winternExt % kHashSearchListCountEx);
#     endif /* (kHashSearchListCountEx == 1) */

#     if ((kHashSearchListCountEx == 1) || (kHashSearchMaxStepsEx == 1))
    if (idExt != CanRxHashIdEx[rxHandle])   
    { 
#      if defined( C_ENABLE_NOT_MATCHED_FCT )
      ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#      endif
      goto finishBasicCan;
    }
#     else /* (kHashSearchListCountEx == 1) || (kHashSearchMaxStepsEx == 1) */

    i_increment = (vuint16)((winternExt % (kHashSearchListCountEx - 1)) + (vuint8)1);          /* ST10-CCAN Keil compiler complains without cast */
    count       = (vsint16)kHashSearchMaxStepsEx - 1;               
  
    while(idExt != CanRxHashIdEx[rxHandle])   
    {
      if(count == (vsint16)0)  
      {
#      if defined( C_ENABLE_NOT_MATCHED_FCT )
        ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#      endif
        goto finishBasicCan;
      }
      count--;
      rxHandle += i_increment;
      if( rxHandle >= (vuint16)kHashSearchListCountEx ) 
      {
        rxHandle -= kHashSearchListCountEx;
      }
    }
#     endif  /* (kHashSearchListCountEx == 1) || (kHashSearchMaxStepsEx == 1) */
    rxHandle = CanRxMsgIndirection[rxHandle+kHashSearchListCount];
  }
#    else /* (kHashSearchListCountEx > 0) */
  {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
    goto finishBasicCan;
  }
#    endif /* (kHashSearchListCountEx > 0) */

#    if defined( C_ENABLE_MIXED_ID )
  else   /* if((CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) )) == kCanIdTypeStd)  */
#    endif
#   endif /* If defined( C_ENABLE_EXTENDED_ID ) */

#   if defined( C_ENABLE_MIXED_ID ) || !defined( C_ENABLE_EXTENDED_ID ) 
#    if (kHashSearchListCount > 0)
  {
    idStd          = ((vuint16)CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) ) | ((vuint16)channel << 11));    /* calculate the logical ID */

#     if (kHashSearchListCount == 1)
    rxHandle       = (CanReceiveHandle)0;
#     else
    winternStd     = idStd + kHashSearchRandomNumber;
    rxHandle       = (CanReceiveHandle)(winternStd % kHashSearchListCount);
#     endif /* (kHashSearchListCount == 1) */

#     if ((kHashSearchListCount == 1)  || (kHashSearchMaxSteps == 1))
    if (idStd != CanRxHashId[rxHandle])
    {
#      if defined( C_ENABLE_NOT_MATCHED_FCT )
      ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#      endif
      goto finishBasicCan;
    }
#     else /* ((kHashSearchListCount == 1)  || (kHashSearchMaxSteps == 1)) */

    i_increment = (vuint16)((winternStd % (kHashSearchListCount - 1)) + (vuint8)1);
    count       = (vsint16)kHashSearchMaxSteps-1;

    /* type of CanRxHashId table depends on the used type of Id */
    while ( idStd != CanRxHashId[rxHandle])
    {
      if (count == (vsint16)0)
      {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
        ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
        goto finishBasicCan;
      }
      count--; 
      rxHandle += i_increment;
      if ( rxHandle >= kHashSearchListCount )
      {
        rxHandle -= kHashSearchListCount;
      }
    }
#     endif /* ((kHashSearchListCount == 1)  || (kHashSearchMaxSteps == 1)) */
    rxHandle = CanRxMsgIndirection[rxHandle];
  }
#    else /* (kHashSearchListCount > 0) */
  {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
    goto finishBasicCan;
  }
#    endif /* (kHashSearchListCount > 0) */
#   endif /* defined( C_ENABLE_MIXED_ID ) || !defined( C_ENABLE_EXTENDED_ID ) */ 
#  endif /* defined( C_SEARCH_HASH ) */



  /**************************** handle filtered message ****************************************************/
#  if defined( C_SEARCH_HASH )
  assertInternal((rxHandle < kCanNumberOfRxObjects), kCanAllChannels , kErrorRxHandleWrong);  /* PRQA S 3109 */ /* legal rxHandle ? */
#  else
  if ( rxHandle < CAN_HL_RX_BASIC_STOPINDEX(channel)) 
#  endif
  {
    /* ID found in table */
#  if defined( C_SEARCH_HASH ) 
#  else
#   if defined( C_ENABLE_RX_MSG_INDIRECTION )
    rxHandle = CanRxMsgIndirection[rxHandle];       /* indirection for special sort-algoritms */
#   endif
#  endif

    CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel) = rxHandle;  

#  if defined( C_ENABLE_RX_QUEUE )
    if (CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY ) == kCanHlContinueRx)
#  else
    if (CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_ONLY ) == kCanHlContinueRx)
#  endif
    { 
#  if defined( C_ENABLE_INDICATION_FLAG ) || \
          defined( C_ENABLE_INDICATION_FCT )

      CanHL_IndRxHandle( rxHandle );
      

      return;
#  endif
    }
  }
#  if defined( C_ENABLE_NOT_MATCHED_FCT )
#   if defined( C_SEARCH_HASH )
#   else
  else
  {
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
#   endif
#  endif
# else  /* kCanNumberOfRxBasicCANObjects > 0 */
#  if defined( C_ENABLE_NOT_MATCHED_FCT )
  ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#  endif
# endif /* kCanNumberOfRxBasicCANObjects > 0 */

# endif

  goto finishBasicCan;     /* to avoid compiler warning */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishBasicCan:

  /* make receive buffer free*/


  return;    /* to avoid compiler warnings about label without code */

} /* end of BasicCan */
/* CODE CATEGORY 1 END*/
/* PRQA S 2001 -- */    /* suppress misra message about multiple return and usage of goto */
#endif

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
/****************************************************************************
| NAME:             CanFullCanMsgReceived
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: internal can chip number
| RETURN VALUES:    none
| DESCRIPTION:      - full can receive
****************************************************************************/
/* PRQA S 2001 ++ */    /* suppress misra message about usage of goto */
/* CODE CATEGORY 1 START*/
static void CanFullCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle )
{
  CanReceiveHandle   rxHandle; 

# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
# endif  

  #if defined( C_ENABLE_FULLCAN_OVERRUN )
  vuint8 copy_cnt;
  #endif
  vuint8 canPndReg;
  vuint32 canPndBit;
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  vuint8 canHwLoopRetVal;
  #endif
  V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;

  CANDRV_SET_CODE_TEST_POINT(0x101);
   
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);   /* PRQA S 3109 */
# endif



  pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[rxObjHandle];
  #if defined( C_ENABLE_FULLCAN_OVERRUN )
  copy_cnt  = 0;
  #endif
  canPndReg = (vuint8)(rxObjHandle >> 5);
  canPndBit = (vuint32)1 << (rxObjHandle & 0x1F);
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerStart(kCanLoopRx);
  #endif
  do
  { /* repeat until consistent data , msgval should not be used here */
    pCanCellMailbox->MoCtrL = (C_MOCTR_RXPND | C_MOCTR_NEWDAT); /* delete pending flag */
    CANLL_CELL_PTR->Modul.MSPND[canPndReg] = ~canPndBit;        /* delete pending interrupt */
  #if defined( C_ENABLE_FULLCAN_OVERRUN )
    copy_cnt++;
  #endif
    TmpRxMailbox.MoARHi    = pCanCellMailbox->MoARHi;
    TmpRxMailbox.MoARLo    = pCanCellMailbox->MoARLo;
    TmpRxMailbox.MoFCR_DLC = pCanCellMailbox->MoFCR_DLC;
    TmpRxMailbox.MoDR[0]   = pCanCellMailbox->MoDR[0];
    TmpRxMailbox.MoDR[1]   = pCanCellMailbox->MoDR[1];
  #if defined( C_ENABLE_HW_LOOP_TIMER )
    canHwLoopRetVal = CanLL_ApplCanTimerLoop(kCanLoopRx);
  } while( ((pCanCellMailbox->MoCtrL & (C_MOCTR_NEWDAT | C_MOCTR_RXUPD)) != (vuint16)0x0000) && (canHwLoopRetVal != 0) );
  #else
  } while((pCanCellMailbox->MoCtrL & (C_MOCTR_NEWDAT | C_MOCTR_RXUPD)) != (vuint16)0x0000);
  #endif
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerEnd(kCanLoopRx);
  #endif
  #if defined( C_ENABLE_FULLCAN_OVERRUN )
  if (copy_cnt > 1) 
  {
    pCanCellMailbox->MoCtrH = C_MOCTR_MSGLOST; /* set overrun bit */
  }
  #endif
  #if defined( C_ENABLE_HW_LOOP_TIMER )
  if(canHwLoopRetVal == 0)
  { /* do not proceed when data is inconsistent (no notification) */
    goto finishRxFullCan;
  }
  #endif

# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  /* pointer needed for other modules */
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
  (pCanRxInfoStruct->pChipMsgObj) = pTmpRxMailboxMsg;
  (pCanRxInfoStruct->pChipData) = pTmpRxMailboxData;
  canRDSRxPtr[channel] = pCanRxInfoStruct->pChipData;
# else
  (canRxInfoStruct[channel].pChipMsgObj) = pTmpRxMailboxMsg;
  (canRxInfoStruct[channel].pChipData) = pTmpRxMailboxData;
  canRDSRxPtr[channel] = canRxInfoStruct[channel].pChipData;
# endif

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    goto finishRxFullCan; /* ignore reception */
  }
# endif

#  if defined( C_ENABLE_FULLCAN_OVERRUN )

  if( (CANLL_CELL_PTR->Obj[rxObjHandle].MoCtrL & C_MOCTR_MSGLOST)==C_MOCTR_MSGLOST )
  {
    CANLL_CELL_PTR->Obj[rxObjHandle].MoCtrL = C_MOCTR_MSGLOST; /* delete overrun */
     ApplCanFullCanOverrun( CAN_CHANNEL_CANPARA_ONLY );
  }
#  endif



# if defined( C_ENABLE_COND_RECEIVE_FCT )
  /**************************** conditional message receive function  ****************************************/
  if(canMsgCondRecState[channel]==kCanTrue)
  {
    ApplCanMsgCondReceived( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
# endif

# if defined( C_ENABLE_RECEIVE_FCT )
  /**************************** call ApplCanMsgReceived() ****************************************************/
  if (APPL_CAN_MSG_RECEIVED( CAN_HL_P_RX_INFO_STRUCT(channel) )==kCanNoCopyData)
  {
     goto finishRxFullCan;
  }
# endif

  /* calculate the message handle to access the generated data for the received message */

  /* brackets to avoid lint info 834 */
  rxHandle = (rxObjHandle - CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel))
# if defined( C_SEARCH_HASH )
                         + kHashSearchListCount
                         + kHashSearchListCountEx
# endif
                         + CAN_HL_RX_FULL_STARTINDEX(canHwChannel);

# if defined( C_HL_ENABLE_ADJUST_RXHANDLE )
  /* not used */
# endif

# if defined( C_ENABLE_RX_MSG_INDIRECTION ) || \
     defined( C_SEARCH_HASH ) 
  rxHandle = CanRxMsgIndirection[rxHandle];
# endif

  assertInternal((rxHandle < kCanNumberOfRxObjects), kCanAllChannels , kErrorRxHandleWrong);  /* PRQA S 3109 */ /* legal rxHandle ? */

  CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel) = rxHandle;  

# if defined( C_ENABLE_RX_QUEUE )
  if (CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY) == kCanHlContinueRx)
# else
  if (CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_ONLY ) == kCanHlContinueRx)
# endif
  { 
# if defined( C_ENABLE_INDICATION_FLAG ) || \
        defined( C_ENABLE_INDICATION_FCT )

    CanHL_IndRxHandle( rxHandle );


    return;
# endif
  }

  goto finishRxFullCan;     /* to avoid compiler warning */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishRxFullCan:

  /* make receive buffer free*/


  return;    /* to avoid compiler warnings about label without code */
}
/* CODE CATEGORY 1 END*/
/* PRQA S 2001 -- */    /* suppress misra message about usage of goto */

#endif

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )  || \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
# if ( kCanNumberOfRxObjects > 0 )
/****************************************************************************
| NAME:             CanHL_ReceivedRxHandle
| CALLED BY:        CanBasicCanMsgReceived, CanFullCanMsgReceived
| PRECONDITIONS:
| INPUT PARAMETERS: Handle of received Message to access generated data
| RETURN VALUES:    none
| DESCRIPTION:      DLC-check, Precopy and copy of Data for received message
****************************************************************************/
/* CODE CATEGORY 1 START*/
#  if defined( C_ENABLE_RX_QUEUE )
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_FIRST tCanRxInfoStruct *pCanRxInfoStruct )
{
#  else
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_ONLY )
{
#  endif
#  if !defined( C_ENABLE_RX_QUEUE ) &&\
    defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
#  endif

#  if defined( C_ENABLE_COPY_RX_DATA )
#   if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#   endif
#  endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);  /* PRQA S 3109 */
# endif

#  if !defined( C_ENABLE_RX_QUEUE ) &&\
    defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
#  endif

#  if defined( C_ENABLE_MULTI_ECU_PHYS )
  if ( (CanRxIdentityAssignment[CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)] & V_ACTIVE_IDENTITY_MSK) == (tVIdentityMsk)0 )
  {
    /* message is not a receive message in the active indentity */
    CANDRV_SET_CODE_TEST_POINT(0x10B);
    return  kCanHlFinishRx;
  }
#  endif


#  if defined( C_ENABLE_DLC_CHECK )  
#   if defined( C_ENABLE_DLC_CHECK_MIN_DATALEN )
  if ( (CanGetRxMinDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))) > CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel) ) )
#   else
  if ( (CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))) > CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel) ) )
#   endif
  {
    /* ##RI1.4 - 2.7: Callbackfunction-DLC-Check */
#   if defined( C_ENABLE_DLC_FAILED_FCT )
    ApplCanMsgDlcFailed( CAN_HL_P_RX_INFO_STRUCT(channel) ); 
#   endif  /*C_ENABLE_DLC_FAILED_FCT */
    return  kCanHlFinishRx;
  }
#  endif

#  if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  CanSetVariableRxDatalen (CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel), CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel)));
#  endif

#  if defined( C_ENABLE_GENERIC_PRECOPY )
  if ( APPL_CAN_GENERIC_PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanCopyData)
  {
    return kCanHlFinishRx;  
  }
#  endif

#  if defined( C_ENABLE_PRECOPY_FCT )
  if ( CanGetApplPrecopyPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)) != V_NULL )    /*precopy routine */
  {
    /* canRxHandle in indexed drivers only for consistancy check in higher layer modules */
    canRxHandle[channel] = CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel);
    
    if ( CanGetApplPrecopyPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))( CAN_HL_P_RX_INFO_STRUCT(channel) )==kCanNoCopyData )
    {  /* precopy routine returns kCanNoCopyData:   */
      return  kCanHlFinishRx;
    }                      /* do not copy data check next irpt */
  }
#  endif

#  if defined( C_ENABLE_COPY_RX_DATA )
  /* no precopy or precopy returns kCanCopyData : copy data -- */
  /* copy via index -------------------------------------------*/
  if ( CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)) != V_NULL )      /* copy if buffer exists */
  {
    /* copy data ---------------------------------------------*/
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptDisable();
#   endif
    CANDRV_SET_CODE_TEST_POINT(0x107);
#   if defined( C_ENABLE_COPY_RX_DATA_WITH_DLC )
    if ( CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel)) < CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)) )
    {
#    if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )    
      {
        vuint8 canllidx;
        for(canllidx=0; canllidx<CanRxActualDLC(CAN_HL_P_RX_INFO_STRUCT(channel)); canllidx++)
        {
          CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))[canllidx] = pCanRxInfoStruct->pChipData[canllidx];
        }
      }
#    else
      {
        vuint8 canllidx;
        for(canllidx=0; canllidx<CanRxActualDLC(CAN_HL_P_RX_INFO_STRUCT(channel)); canllidx++)
        {
          CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))[canllidx] = canRxInfoStruct[channel].pChipData[canllidx];
        }
      }
#    endif 
    }
    else
#   endif   /* C_ENABLE_COPY_RX_DATA_WITH_DLC */
    {
#   if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )    
      {
        vuint8 canllidx;
        for(canllidx=0; canllidx<CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)); canllidx++)
        {
          CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))[canllidx] = pCanRxInfoStruct->pChipData[canllidx];
        }
      }
#   else
      {
        vuint8 canllidx;
        for(canllidx=0; canllidx<CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)); canllidx++)
        {
          CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))[canllidx] = canRxInfoStruct[channel].pChipData[canllidx];
        }
      }
#   endif 
    }
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptRestore();
#   endif
  }
#  endif /* ( C_ENABLE_COPY_RX_DATA ) */

  CANDRV_SET_CODE_TEST_POINT(0x105);
  return kCanHlContinueRx;
} /* end of CanReceivceRxHandle() */
/* CODE CATEGORY 1 END*/

#  if defined( C_ENABLE_INDICATION_FLAG ) || \
     defined( C_ENABLE_INDICATION_FCT )
/****************************************************************************
| NAME:             CanHL_IndRxHandle
| CALLED BY:        CanBasicCanMsgReceived, CanFullCanMsgReceived
| PRECONDITIONS:
| INPUT PARAMETERS: Handle of received Message to access generated data
| RETURN VALUES:    none
| DESCRIPTION:      DLC-check, Precopy and copy of Data for received message
****************************************************************************/
/* CODE CATEGORY 1 START*/
static void CanHL_IndRxHandle( CanReceiveHandle rxHandle )
{
#   if defined( C_ENABLE_INDICATION_FLAG )
#    if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#    endif
#   endif

#   if defined( C_ENABLE_INDICATION_FLAG )
#    if C_SECURITY_LEVEL > 20
  CanNestedGlobalInterruptDisable();
#    endif
  CanIndicationFlags._c[CanGetIndicationOffset(rxHandle)] |= CanGetIndicationMask(rxHandle);
#    if C_SECURITY_LEVEL > 20
  CanNestedGlobalInterruptRestore();
#    endif
#   endif

#   if defined( C_ENABLE_INDICATION_FCT )
  if ( CanGetApplIndicationPtr(rxHandle) != V_NULL )
  {
    CanGetApplIndicationPtr(rxHandle)(rxHandle);  /* call IndicationRoutine */
  }
#   endif
}
/* CODE CATEGORY 1 END*/
#  endif /* C_ENABLE_INDICATION_FLAG || C_ENABLE_INDICATION_FCT  */
# endif /* ( kCanNumberOfRxObjects > 0 ) */
#endif


/****************************************************************************
| NAME:             CanHL_TxConfirmation
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: - internal can chip number
|                   - interrupt ID
| RETURN VALUES:    none
| DESCRIPTION:      - full can transmit
****************************************************************************/
/* PRQA S 2001 ++ */    /* suppress misra message about usage of goto */
/* CODE CATEGORY 1 START*/
static void CanHL_TxConfirmation( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle)
{
  CanObjectHandle       logTxObjHandle;
  CanTransmitHandle     txHandle;

 
#if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle         queueElementIdx;    /* use as signed due to loop */
  CanSignedTxHandle         elementBitIdx;
  tCanQueueElementType             elem;
  CanDeclareGlobalInterruptOldStatus
#else
# if defined( C_ENABLE_CONFIRMATION_FLAG )
#  if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#  endif
# endif
#endif


#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  vuint8 rc;
#endif



# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);  /* PRQA S 3109 */
# endif



  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  /* set message object as free (invalid), Tx Pending reset, disable object */
  CANLL_CELL_PTR->Obj[txObjHandle].MoCtrL = C_MOCTR_TXPND | C_MOCTR_TXEN0; /* delete pending and enable */

  txHandle = canHandleCurTxObj[logTxObjHandle];           /* get saved handle */

  /* check associated transmit handle */
  if (txHandle == kCanBufferFree)
  {
    assertInternal (0, channel, kErrorTxHandleWrong);          /* PRQA S 3109 */ /*lint !e506 !e774*/
    goto finishCanHL_TxConfirmation;
  }  

#if defined( C_ENABLE_TX_OBSERVE ) || \
    defined( C_ENABLE_CAN_TX_CONF_FCT )
  {
# if defined( C_ENABLE_TX_OBSERVE )
    {
      ApplCanTxObjConfirmed( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
    }
# endif

# if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
    txInfoStructConf[channel].Handle  = txHandle;
    APPL_CAN_TX_CONFIRMATION(&txInfoStructConf[channel]);
# endif
  }
#endif /* defined( C_ENABLE_TX_OBSERVE ) || defined( C_ENABLE_CAN_TX_CONF_FCT ) */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_TX_FULLCAN_OBJECTS )  || \
     defined( C_ENABLE_MSG_TRANSMIT )
  if (txObjHandle != CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel))
  {
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                 /* free msg object of FullCAN or LowLevel Tx obj. */
  }
# endif
#else
  canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                   /* free msg object if queue is not used */
#endif

  if (txHandle != kCanBufferCancel)
  {
#if defined( C_ENABLE_MSG_TRANSMIT )
    if (txObjHandle == CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel))
    {
# if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
      APPL_CAN_MSGTRANSMITCONF( CAN_CHANNEL_CANPARA_ONLY );
# endif

      goto finishCanHL_TxConfirmation;
    }
#endif

#if defined( C_ENABLE_MULTI_ECU_PHYS )
    assertInternal(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), 
                                                                channel , kErrorInternalDisabledTxMessage);  /* PRQA S 3109 */
#endif

#if defined( C_ENABLE_CONFIRMATION_FLAG )       /* set transmit ready flag  */
# if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptDisable();
# endif
    CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] |= CanGetConfirmationMask(txHandle);
# if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptRestore();
# endif
#endif

#if defined( C_ENABLE_CONFIRMATION_FCT )
    {
      if ( CanGetApplConfirmationPtr(txHandle) != V_NULL )
      {
        (CanGetApplConfirmationPtr(txHandle))(txHandle);   /* call completion routine  */
      }
    }
#endif /* C_ENABLE_CONFIRMATION_FCT */

  } /* end if kCanBufferCancel */
  
#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_TX_FULLCAN_OBJECTS ) ||\
     defined( C_ENABLE_MSG_TRANSMIT )
  if (txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)) 
# endif
  {
    CanNestedGlobalInterruptDisable();                /* ESCAN00008914 */

    for(queueElementIdx = CAN_HL_TXQUEUE_STOPINDEX(channel) - (CanSignedTxHandle)1; 
                             queueElementIdx >= CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx--)
    {
      elem = canTxQueueFlags[queueElementIdx];
      if(elem != (tCanQueueElementType)0) /* is there any flag set in the queue element? */
      {

        CanNestedGlobalInterruptRestore();

        /* start the bitsearch */
        #if defined( C_CPUTYPE_32BIT )
        if((elem & (tCanQueueElementType)0xFFFF0000) != (tCanQueueElementType)0)
        {
          if((elem & (tCanQueueElementType)0xFF000000) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0xF0000000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 28] + 28;
            }
            else /*0x0F000000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 24] + 24;
            }
          }
          else
          {
            if((elem & (tCanQueueElementType)0x00F00000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 20] + 20;
            }
            else /* 0x000F0000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 16] + 16;
            }
          }
        }
        else
        #endif
        {
        #if defined( C_CPUTYPE_32BIT ) 
          if((elem & (tCanQueueElementType)0x0000FF00) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0x0000F000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 12] + 12;
            }
            else /*0x00000F00*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 8] + 8;
            }
          }
          else
        #endif
          {
            if((elem & (tCanQueueElementType)0x000000F0) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 4] + 4;
            }
            else /* 0x0000000F*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem];
            }
          }
        }
        txHandle = (CanTransmitHandle)((((CanTransmitHandle)queueElementIdx << kCanTxQueueShift) + (CanTransmitHandle)elementBitIdx) - CAN_HL_TXQUEUE_PADBITS(channel));
        {

            /* compute the logical message handle */
            CanNestedGlobalInterruptDisable();
 
            if ( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
            {
              canTxQueueFlags[queueElementIdx] &= ~CanShiftLookUp[elementBitIdx]; /* clear flag from the queue */

              CanNestedGlobalInterruptRestore();  
              canHandleCurTxObj[logTxObjHandle] = txHandle;/* Save hdl of msgObj to be transmitted*/
# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
              rc = CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle,txHandle); 
              if ( rc == kCanTxNotify)
              {
                APPLCANCANCELNOTIFICATION(channel, txHandle);
              }
# else       /* C_ENABLE_CAN_CANCEL_NOTIFICATION */
              (void)CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle,txHandle); 
# endif /* C_ENABLE_CAN_CANCEL_NOTIFICATION */

              goto finishCanHL_TxConfirmation;
            }
            /* meanwhile, the queue is empty. E.g. due to CanOffline on higher level */
            canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* free msg object if queue is empty */
            CanNestedGlobalInterruptRestore();  
            goto finishCanHL_TxConfirmation;
            
        }
        /* no entry found in Queue */
# if defined( CANHL_TX_QUEUE_BIT )
# else
        CanNestedGlobalInterruptDisable();                /*lint !e527 ESCAN00008914 */
                                                 /* unreachable in case of Bit-Queue */
# endif                                                 
      }
    }

    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* free msg object if queue is empty */
    CanNestedGlobalInterruptRestore();                 /* ESCAN00008914 */
  }
#endif
  /* check for next msg object in queue and transmit it */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishCanHL_TxConfirmation:


  return;

} /* END OF CanTxInterrupt */
/* CODE CATEGORY 1 END*/
/* PRQA S 2001 -- */    /* suppress misra message about usage of goto */


#if defined( C_ENABLE_ECU_SWITCH_PASS )
/************************************************************************
* NAME:               CanSetActive
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Set the CAN driver into active mode
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetActive( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
#endif

  canPassive[channel] = 0;
} /* END OF CanSetActive */
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetPassive
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Set the can driver into passive mode
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPassive( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);      /* PRQA S 3109 */
#endif

  canPassive[channel] = 1;
 
# if defined( C_ENABLE_TRANSMIT_QUEUE )
  /* clear all Tx queue flags: */
  CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY ); 
# endif
 

} /* END OF CanSetPassive */
/* CODE CATEGORY 4 END*/
#endif /* IF defined( C_ENABLE_ECU_SWITCH_PASS ) */


#if defined( C_ENABLE_OFFLINE )
/************************************************************************
* NAME:               CanOnline( CanChannelHandle channel )
* CALLED BY:          netmanagement
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Switch on transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOnline(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
#endif

  CanNestedGlobalInterruptDisable();

  canStatus[channel] |= kCanTxOn;

# if defined( C_ENABLE_ONLINE_OFFLINE_CALLBACK_FCT )
  APPL_CAN_ONLINE(CAN_CHANNEL_CANPARA_ONLY);
# endif
  CanNestedGlobalInterruptRestore();

}
/* CODE CATEGORY 4 END*/


/************************************************************************
* NAME:               CanOffline( CanChannelHandle channel )
* CALLED BY:          netmanagement
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Switch off transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOffline(CAN_CHANNEL_CANTYPE_ONLY) C_API_3
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
#endif

  CanNestedGlobalInterruptDisable();

  canStatus[channel] &= kCanTxNotOn;

# if defined( C_ENABLE_ONLINE_OFFLINE_CALLBACK_FCT )
  APPL_CAN_OFFLINE(CAN_CHANNEL_CANPARA_ONLY);
# endif
  CanNestedGlobalInterruptRestore();

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
# endif

}
/* CODE CATEGORY 4 END*/
#endif /* if defined( C_ENABLE_OFFLINE ) */


#if defined( C_ENABLE_PART_OFFLINE )
/************************************************************************
* NAME:               CanSetPartOffline
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          (channel), sendGroup
* RETURN VALUE:       none
* DESCRIPTION:        Switch partial off transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOffline(CAN_CHANNEL_CANTYPE_FIRST vuint8 sendGroup)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
#endif

  CanNestedGlobalInterruptDisable();
  canTxPartOffline[channel] |= sendGroup;   /* set offlinestate and Save for use of CanOnline/CanOffline */
  CanNestedGlobalInterruptRestore();
}
/* CODE CATEGORY 4 END*/


/************************************************************************
* NAME:               CanSetPartOnline
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          (channel), invSendGroup
* RETURN VALUE:       none
* DESCRIPTION:        Switch partial on transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOnline(CAN_CHANNEL_CANTYPE_FIRST vuint8 invSendGroup)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
#endif

  CanNestedGlobalInterruptDisable();
  canTxPartOffline[channel] &= invSendGroup;
  CanNestedGlobalInterruptRestore();
}
/* CODE CATEGORY 4 END*/


/************************************************************************
* NAME:               CanGetPartMode
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       canTxPartOffline
* DESCRIPTION:        return status of partoffline-Mode
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanGetPartMode(CAN_CHANNEL_CANTYPE_ONLY)
{
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
#endif

  return canTxPartOffline[channel];
}
/* CODE CATEGORY 4 END*/
#endif


/****************************************************************************
| NAME:             CanGetStatus
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    Bit coded status (more than one status can be set):
|                   kCanTxOn
|                   kCanHwIsStop        
|                   kCanHwIsInit        
|                   kCanHwIsInconsistent
|                   kCanHwIsWarning     
|                   kCanHwIsPassive     
|                   kCanHwIsBusOff      
|                   kCanHwIsSleep       
| DESCRIPTION:      returns the status of the transmit path and the CAN hardware.
|                   Only one of the statusbits Sleep,Busoff,Passiv,Warning is set.
|                   Sleep has the highest priority, error warning the lowerst. 
****************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetStatus( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3
{
#if defined( C_ENABLE_EXTENDED_STATUS )
  /* not used */
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
#endif

#if defined( C_ENABLE_EXTENDED_STATUS )
  /* not used */

# if defined( C_ENABLE_SLEEP_WAKEUP )
  /***************************** verify Sleep mode *************************************/
  if ( CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY)   )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsSleep ) ); }

# endif

  /***************************** verify Stop mode *************************************/
  if ( CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY)    )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsStop ) ); }

  /***************************** verify Busoff *************************************/
  if ( CanLL_HwIsBusOff(CAN_HW_CHANNEL_CANPARA_ONLY)  )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsBusOff ) ); }

  /***************************** verify Error Passiv *******************************/
  {
    if ( CanLL_HwIsPassive(CAN_HW_CHANNEL_CANPARA_ONLY) )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsPassive ) ); }
  }

  /***************************** verify Error Warning ******************************/
  {
    if ( CanLL_HwIsWarning(CAN_HW_CHANNEL_CANPARA_ONLY) )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsWarning ) ); }
  }
#endif
  return ( VUINT8_CAST (canStatus[channel] & kCanTxOn) );

} /* END OF CanGetStatus */
/* CODE CATEGORY 3 END*/


/****************************************************************************
| NAME:             CanSleep
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanOk, if CanSleep was successfull
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      disable CAN 
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanSleep(CAN_CHANNEL_CANTYPE_ONLY)
{
#if defined( C_ENABLE_SLEEP_WAKEUP )
  vuint8         canReturnCode;
#endif

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
#endif
  assertUser((canCanInterruptCounter[channel] == (vsintx)0), channel, kErrorDisabledCanInt);    /* PRQA S 3109 */

#if defined( C_ENABLE_COND_RECEIVE_FCT )
  /* this has to be done, even if SLEEP_WAKEUP is not enabled */
  canMsgCondRecState[channel] = kCanTrue;
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )

  assertUser((canStatus[channel] & kCanTxOn) != kCanTxOn, channel, kErrorCanOnline);   /* PRQA S 3109 */

  {
    assertUser(!CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanStop);     /* PRQA S 3109 */
    {
    # if defined( V_CPU_TRICORE )
      vuint8 canllLocalSleepNotify;
      vuint8 canllidx;
    #  if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
      vuint8 canll_ch;
    #  endif
      CanDeclareGlobalInterruptOldStatus
      canReturnCode = kCanOk;
      canllSleepNotify[canHwChannel] = CANLL_TRUE; /* mark channel as sleep */
      CANLL_CELL_PTR->Node[CAN_NODE_NUM].NCR |= ( C_NCR_INIT );   /* node deactivate, enable change */
      /* delete all pending object interrupts */
      for(canllidx=0;canllidx<CAN_MAX_PND_REG;canllidx++)
      {
        CANLL_CELL_PTR->Modul.MSPND[canllidx] = ~CanInterruptPendingMask[canHwChannel].GlobalMask[canllidx];
      }
      canllLocalSleepNotify = CANLL_TRUE;
    #  if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
      for (canll_ch = 0; canll_ch < kCanNumberOfChannels; canll_ch++)
      {
        {
          if (canllSleepNotify[canll_ch] == CANLL_FALSE) 
          {
            canllLocalSleepNotify = CANLL_FALSE;
          }
        }
      }
    #  else
      canllLocalSleepNotify = canllSleepNotify[channel];
    #  endif
    #  if !defined(C_ENABLE_MULTIPLE_DRIVERS)
      if (((CANLL_CELL_PTR->Ctrl.CLC & C_CLC_IS_DISABLE) != C_CLC_IS_DISABLE) 
    #   if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
          && (canllLocalSleepNotify == CANLL_TRUE))
    #   else
          )
    #   endif
      { /* sleep only if all channels set to sleep */
        CanNestedGlobalInterruptDisable();
        VStdUnlockInit();
        CANLL_CELL_PTR->Ctrl.CLC |= C_CLC_DISABLE;
        VStdLockInit();
        CanNestedGlobalInterruptRestore();
      }
    #  endif
    # else /* V_CPU_TRICORE */
      canReturnCode = kCanNotSupported;
    # endif
    }
  }
  return canReturnCode;
#else
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && \
     defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;
# endif
  return kCanNotSupported;
#endif
} /* END OF CanSleep */
/* CODE CATEGORY 4 END*/

/****************************************************************************
| NAME:             CanWakeUp
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanOk, if CanWakeUp was successfull
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      enable CAN 
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanWakeUp( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_ENABLE_SLEEP_WAKEUP )
  vuint8         canReturnCode;


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
#endif
  assertUser((canCanInterruptCounter[channel] == (vsintx)0), channel, kErrorDisabledCanInt);    /* PRQA S 3109 */

  {
    {
    # if defined( V_CPU_TRICORE )
      CanDeclareGlobalInterruptOldStatus
      canReturnCode = kCanOk;
    #  if !defined(C_ENABLE_MULTIPLE_DRIVERS)
      CanNestedGlobalInterruptDisable();
      VStdUnlockInit();
      CANLL_CELL_PTR->Ctrl.CLC &= ~C_CLC_DISABLE;
      VStdLockInit();
      CanNestedGlobalInterruptRestore();
      if((CANLL_CELL_PTR->Ctrl.CLC & C_CLC_IS_DISABLE)==C_CLC_IS_DISABLE)
      { /* do not use any CAN registers (rw) until wakeup */
        canReturnCode = kCanFailed;
      }
    #  endif
      canllSleepNotify[canHwChannel] = CANLL_FALSE; /* mark channel as wakeup */
      CANLL_CELL_PTR->Node[CAN_NODE_NUM].NCR &= ~( C_NCR_INIT );   /* node activate */
    # else /* V_CPU_TRICORE */
      canReturnCode = kCanNotSupported;
    # endif
    }
  }
  return canReturnCode;
#else
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && \
     defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;
# endif
  return kCanNotSupported;
#endif /* C_ENABLE_SLEEP_WAKEUP */
} /* END OF CanWakeUp */
/* CODE CATEGORY 4 END*/


#if defined( C_ENABLE_STOP )
/****************************************************************************
| NAME:             CanStop
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    kCanOk, if success
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      stop CAN-controller
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStop( CAN_CHANNEL_CANTYPE_ONLY )
{
  vuint8         canReturnCode;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
# endif
  assertUser((canStatus[channel] & kCanTxOn) != kCanTxOn, channel, kErrorCanOnline);   /* PRQA S 3109 */

  {
# if defined( C_ENABLE_SLEEP_WAKEUP )
    assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);   /* PRQA S 3109 */
# endif
    canllHalt[canHwChannel] = CANLL_TRUE; 
    CANLL_CELL_PTR->Node[CAN_NODE_NUM].NCR |= C_NCR_INIT;
    canReturnCode = kCanOk;
  }
  return canReturnCode;
}
/* CODE CATEGORY 4 END*/

/****************************************************************************
| NAME:             CanStart
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    kCanOk, if success
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      restart CAN-controller
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStart( CAN_CHANNEL_CANTYPE_ONLY )
{
  vuint8         canReturnCode;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
# endif

  {
    CANLL_CELL_PTR->Node[CAN_NODE_NUM].NCR &= ~(C_NCR_CCE | C_NCR_INIT);   /* activate node */
    canllHalt[canHwChannel] = CANLL_FALSE;
    canllBusOffNotified[canHwChannel] = CANLL_FALSE;  /* enable busoff detection after Init-flag is reset */
    canReturnCode = kCanOk;
  }
  return canReturnCode;
}
/* CODE CATEGORY 4 END*/
#endif /* if defined( C_ENABLE_STOP ) */

#if defined (C_ENABLE_CAN_CAN_INTERRUPT_CONTROL)
/****************************************************************************
| NAME:             CanCanInterruptDisable
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      disables CAN interrupts and stores old interrupt status
****************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptDisable( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3
{
# if defined (C_ENABLE_OSEK_CAN_INTCTRL)

  {
    OsCanCanInterruptDisable(CAN_HW_CHANNEL_CANPARA_ONLY);
  }
# else  /* defined (C_ENABLE_OSEK_CAN_INTCTRL) */

  CanDeclareGlobalInterruptOldStatus
#  if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )
#  endif

  /* local variable must reside on stack or registerbank, switched */
  /* in interrupt level                                            */
  /* disable global interrupt                                      */
#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
#  endif
  assertUser(canCanInterruptCounter[channel]<(vsint8)0x7f, kCanAllChannels, kErrorIntDisableTooOften);    /* PRQA S 3109 */

  CanNestedGlobalInterruptDisable();
  if (canCanInterruptCounter[channel] == (vsintx)0)  /* if 0 then save old interrupt status */
  {
#  if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )
    {
#   if defined( C_ENABLE_SLEEP_WAKEUP )
      assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);     /* PRQA S 3109 */
#   endif

      CanLL_CanInterruptDisable(canHwChannel, &canCanInterruptOldStatus[canHwChannel]);
    }
#  endif
#  if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
    ApplCanAddCanInterruptDisable(channel);
#  endif
  }
  canCanInterruptCounter[channel]++;               /* common for all platforms */

  CanNestedGlobalInterruptRestore();
# endif  /* C_ENABLE_OSEK_CAN_INTCTRL */
} /* END OF CanCanInterruptDisable */
/* CODE CATEGORY 1 END*/

/****************************************************************************
| NAME:             CanCanInterruptRestore
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      restores the old interrupt status of the CAN interrupt if 
|                   canCanInterruptCounter[channel] is zero
****************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptRestore( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3
{
# if defined (C_ENABLE_OSEK_CAN_INTCTRL)

  {
    OsCanCanInterruptRestore(CAN_HW_CHANNEL_CANPARA_ONLY);
  }
# else  /* defined (C_ENABLE_OSEK_CAN_INTCTRL) */

  CanDeclareGlobalInterruptOldStatus
#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
#  endif
  assertUser(canCanInterruptCounter[channel]>(vsintx)0, kCanAllChannels, kErrorIntRestoreTooOften);   /* PRQA S 3109 */

  CanNestedGlobalInterruptDisable();
  /* restore CAN interrupt */
  canCanInterruptCounter[channel]--;

  if (canCanInterruptCounter[channel] == (vsintx)0)         /* restore interrupt if canCanInterruptCounter=0 */
  {
#  if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )    
    {
#   if defined( C_ENABLE_SLEEP_WAKEUP )
      assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);   /* PRQA S 3109 */
#   endif

      CanLL_CanInterruptRestore(canHwChannel, canCanInterruptOldStatus[canHwChannel]);
    }
#  endif

#  if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
    ApplCanAddCanInterruptRestore(channel);
#  endif
  }
  CanNestedGlobalInterruptRestore();
# endif  /* defined (C_ENABLE_OSEK_CAN_INTCTRL) */
} /* END OF CanCanInterruptRestore */
/* CODE CATEGORY 1 END*/
#endif /* defined (C_ENABLE_CAN_CAN_INTERRUPT_CONTROL) */

#if defined( C_ENABLE_MSG_TRANSMIT )
/************************************************************************
* NAME:               CanMsgTransmit
* CALLED BY:          CanReceivedFunction
* PRECONDITIONS:      Called in Receive Interrupt
* PARAMETER:          Pointer to message buffer data block; This can either be
*                     a RAM structure data block or the receive buffer in the
*                     CAN chip
* RETURN VALUE:       The return value says that a transmit request was successful
*                     or not
* DESCRIPTION:        Transmit functions for gateway issues (with dynamic
*                     messages). If the transmit buffer is not free, the message
*                     is inserted in the FIFO ring buffer.
*************************************************************************/
/* Msg(4:3673) The object addressed by the pointer "txMsgStruct" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
/* CODE CATEGORY 2 START*/
# if defined ( V_ENABLE_USED_GLOBAL_VAR )
/* txMsgStruct is located in far memory */
C_API_1 vuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST V_MEMRAM1_FAR tCanMsgTransmitStruct V_MEMRAM2_FAR V_MEMRAM3_FAR *txMsgStruct )       /* PRQA S 3673 */      /* suppress message about const pointer */
{
# else
C_API_1 vuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST tCanMsgTransmitStruct *txMsgStruct )       /* PRQA S 3673 */      /* suppress message about const pointer */
{
# endif
  CanDeclareGlobalInterruptOldStatus
  vuint8                 rc;
  CanObjectHandle          txObjHandle;
  CanObjectHandle          logTxObjHandle;

  V_DEF_P2SFR_CAN(volatile, tCanMsgObjStruct, AUTOMATIC) pCanCellMailbox;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
# endif


  CanNestedGlobalInterruptDisable();

  /* --- test on CAN transmit switch --- */
  if ( (canStatus[channel] & kCanTxOn) != kCanTxOn )                /* transmit path switched off */
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    CanNestedGlobalInterruptRestore();
    return(kCanCommunicationDisabled);
  }
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
  assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);    /* PRQA S 3109 */
# endif
  assertUser(!CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanStop);      /* PRQA S 3109 */
  
  /* --- check on passive state --- */
# if defined( C_ENABLE_ECU_SWITCH_PASS )
  if ( canPassive[channel] != (vuint8)0)                             /*  set passive ? */
  {
    CanNestedGlobalInterruptRestore();
#  if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
    APPL_CAN_MSGTRANSMITCONF( CAN_CHANNEL_CANPARA_ONLY );
#  endif
    return (kCanTxOk);
  }
# endif /* C_ENABLE_ECU_SWITCH_PASS */

  /* calculate index for canhandleCurTxObj (logical object handle) */
  logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  /* check for transmit message object free ---------------------------------*/
  /* MsgObj used?  */
  if (( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree ))    
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

  /* Obj, pMsgObject points to is free, transmit msg object: ----------------*/
  /* Save hdl of msgObj to be transmitted*/
  canHandleCurTxObj[logTxObjHandle] = kCanBufferMsgTransmit;
  CanNestedGlobalInterruptRestore();



  txObjHandle = CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel);
  assertHardware( CanLL_TxIsHWObjFree(canHwChannel, txObjHandle ), channel, kErrorTxBufferBusy);

  pCanCellMailbox = (CanMsgObjStructPtr) &CANLL_CELL_PTR->Obj[txObjHandle];
  pCanCellMailbox->MoCtrL = C_MOCTR_TXEN0; /* delete enable flag */

  CanNestedGlobalInterruptDisable();
  /* Copy all data into transmit object */


  /* If CanTransmit was interrupted by a re-initialization or CanOffline */  
  /* no transmitrequest of this action should be started      */  
  if ((canHandleCurTxObj[logTxObjHandle] == kCanBufferMsgTransmit) && 
                                   ( (canStatus[channel] & kCanTxOn) == kCanTxOn ) )
  {  
     { /* - do not copy data with 32bit access because of failure with GNU compiler
          - do not use 32bit copy because of possible 8bit allignment for application data */
       vuint8 canllidx;
       CanChipDataPtr src, dst;
       src = txMsgStruct->DataFld;                           /* lint !e740 */
       dst = (CanChipDataPtr)(&(pCanCellMailbox->MoDR[0]));  /* lint !e740 */
       for(canllidx=0; canllidx < 8; canllidx++)
       {
         dst[canllidx] = src[canllidx];
       }
       pCanCellMailbox->MoFCR_DLC = txMsgStruct->DlcRaw & C_MOFCR_DLC;
       pCanCellMailbox->MoARHi    = C_OBJ_PRIO_16 | txMsgStruct->IdRaw0;
       pCanCellMailbox->MoARLo    = txMsgStruct->IdRaw1;
     }

     #if defined(C_ENABLE_WORKAROUND_TC21)
     CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM].MoCtrL = C_MOCTR_RTSEL; /* delete sel bit */
     CANLL_CELL_PTR->Obj[(CAN_MAX_NUM_MSGOBJ-1)-CAN_NODE_NUM].MoCtrH = C_MOCTR_TXEN0 | C_MOCTR_TXEN1 | 
                                                                       C_MOCTR_TXREQ | C_MOCTR_NEWDAT;
     #endif
     pCanCellMailbox->MoCtrL = C_MOCTR_RTSEL; /* delete sel bit */
     pCanCellMailbox->MoCtrH = C_MOCTR_TXEN0 | C_MOCTR_TXEN1 | C_MOCTR_TXREQ | C_MOCTR_NEWDAT;

# if defined( C_ENABLE_TX_OBSERVE )
     ApplCanTxObjStart( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
# endif
   
     rc = kCanTxOk;                                    
  }  
  else  
  {  
    /* release TxHandle (CanOffline) */
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  
    rc = kCanTxFailed;   
  }  

  CanNestedGlobalInterruptRestore();


  return rc;
} /*end of CanMsgTransmit() */
/* CODE CATEGORY 2 END*/
#endif


#if defined( C_ENABLE_DYN_TX_OBJECTS )
/************************************************************************
* NAME:           CanGetDynTxObj
* PARAMETER:      txHandle - Handle of the dynamic object to reserve
* RETURN VALUE:   kCanNoTxDynObjAvailable (0xFF) - 
*                   object not available
*                 0..F0 - 
*                   Handle to dynamic transmission object
* DESCRIPTION:    Function reserves and return a handle to a dynamic 
*                   transmission object
*
*                 To use dynamic transmission, an application must get
*                 a dynamic object from CAN-driver. 
*                 Before transmission, application must set all attributes 
*                 (id, dlc, data, confirmation function/flag, pretransmission
*                 etc. - as configurated). 
*                 Application can use a dynamic object for one or many
*                 transmissions (as it likes) - but finally, it must
*                 release the dynamic object by calling CanReleaseDynTxObj.
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanGetDynTxObj(CanTransmitHandle txHandle ) C_API_3
{
  CanTransmitHandle nTxDynObj;
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);    /* PRQA S 3109 */

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);  /* PRQA S 3109 */
# if ( kCanNumberOfTxStatObjects > 0) || defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /* PRQA S 3109 */ /*lint !e568 */
# endif

  nTxDynObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel); /* PRQA S 3382,0291 */

  CanNestedGlobalInterruptDisable();
  if ( canTxDynObjReservedFlag[nTxDynObj] != (vuint8)0)
  {
    CanNestedGlobalInterruptRestore();
    return kCanNoTxDynObjAvailable;
  }
  /*  Mark dynamic object as used  */
  canTxDynObjReservedFlag[nTxDynObj] = 1;

# if defined( C_ENABLE_CONFIRMATION_FLAG )
  CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] &= 
                            (vuint8)(~CanGetConfirmationMask(txHandle));
# endif
  CanNestedGlobalInterruptRestore();

  /* Initialize dynamic object */
# if defined( C_ENABLE_DYN_TX_DATAPTR )  
  canDynTxDataPtr[nTxDynObj] = V_NULL;  
# endif  
  

  return (txHandle);
}
/* CODE CATEGORY 3 END*/

/************************************************************************
* NAME:           CanReleaseDynTxObj
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
* RETURN VALUE:   --
* DESCRIPTION:    Function releases dynamic transmission object
*                   which was reserved before (calling CanGetDynTxObj)
*                 
*                 After a transmission of one or more messages is finished,
*                 application must free the reserved resource, formally the
*                 dynamic transmission object calling this function.
*
*                 As the number of dynamic transmission object is limited,
*                 application should not keep unused dynamic transmission
*                 objects for a longer time.
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanReleaseDynTxObj(CanTransmitHandle txHandle) C_API_3
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle queueElementIdx; /* index for accessing the tx queue */
  CanSignedTxHandle elementBitIdx;  /* bit index within the tx queue element */
  CanTransmitHandle queueBitPos;  /* physical bitposition of the handle */
# endif
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);    /* PRQA S 3109 */

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);  /* PRQA S 3109 */
# if ( kCanNumberOfTxStatObjects > 0) || defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /* PRQA S 3109 */ /*lint !e568 */
# endif

  dynTxObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);  /* PRQA S 3382,0291 */

  assertInternal((canTxDynObjReservedFlag[dynTxObj] != (vuint8)0), channel, kErrorReleasedUnusedDynObj);  /* PRQA S 3109 */

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  #if defined( C_ENABLE_INTERNAL_CHECK ) &&\
      defined( C_MULTIPLE_RECEIVE_CHANNEL )
  if (sizeof(queueBitPos) == 1)
  {
    assertInternal( (((vuint16)kCanNumberOfTxObjects + (vuint16)CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 256u), kCanAllChannels, kErrorTxQueueTooManyHandle) /* PRQA S 3109 */ /*lint !e572 !e506*/
  }
  else
  {
    assertInternal( (((vuint32)kCanNumberOfTxObjects + (vuint32)CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 65536u), kCanAllChannels, kErrorTxQueueTooManyHandle) /* PRQA S 3109 */ /*lint !e572 !e506*/
  }
  #endif
  queueBitPos  = txHandle + CAN_HL_TXQUEUE_PADBITS(channel);
  queueElementIdx = (CanSignedTxHandle)(queueBitPos >> kCanTxQueueShift); /* get the queue element where to set the flag */
  elementBitIdx  = (CanSignedTxHandle)(queueBitPos & kCanTxQueueMask);   /* get the flag index wihtin the queue element */
  if( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
  {
  }
  else
# endif
  {
    if (
# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
         (confirmHandle[channel] == txHandle) ||       /* confirmation active ? */
# endif
         (canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] != txHandle) )
    {
      /*  Mark dynamic object as not used  */
      canTxDynObjReservedFlag[dynTxObj] = 0;
      return(kCanDynReleased);
    }
  }
  return(kCanDynNotReleased);
}
/* CODE CATEGORY 3 END*/
#endif /* C_ENABLE_DYN_TX_OBJECTS */


#if defined( C_ENABLE_DYN_TX_ID ) 
# if !defined( C_ENABLE_EXTENDED_ID ) ||\
     defined( C_ENABLE_MIXED_ID )
/************************************************************************
* NAME:           CanDynTxObjSetId
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 id -
*                   Id (standard-format) to register with dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Function registers submitted id (standard format)
*                 with dynamic object referenced by handle.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetId(CanTransmitHandle txHandle, vuint16 id) C_API_3
{
  CanTransmitHandle dynTxObj;

  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);  /* PRQA S 3109 */

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
#  endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);  /* PRQA S 3109 */
# if ( kCanNumberOfTxStatObjects > 0) || defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /* PRQA S 3109 */ /*lint !e568 */
# endif
  assertUser(id <= (vuint16)0x7FF, channel, kErrorWrongId);                                              /* PRQA S 3109 */

  dynTxObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel); /* PRQA S 3382,0291 */

#  if defined( C_ENABLE_MIXED_ID )
#   if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#   else
  canDynTxIdType[dynTxObj]  = kCanIdTypeStd; 
#   endif
#  endif

  canDynTxId0[dynTxObj] = MK_STDID0(id);
#  if (kCanNumberOfUsedCanTxIdTables > 1)
  canDynTxId1[dynTxObj] = MK_STDID1(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 2)
  canDynTxId2[dynTxObj] = MK_STDID2(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 3)
  canDynTxId3[dynTxObj] = MK_STDID3(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 4)
  canDynTxId4[dynTxObj] = MK_STDID4(id);
#  endif
}
/* CODE CATEGORY 2 END*/
# endif /* !defined( C_ENABLE_EXTENDED_ID ) || defined( C_ENABLE_MIXED_ID ) */
#endif /* C_ENABLE_DYN_TX_ID */

#if defined( C_ENABLE_DYN_TX_ID ) && \
    defined( C_ENABLE_EXTENDED_ID )
/************************************************************************
* NAME:           CanDynTxObjSetExtId
* PARAMETER:      hTxObj -  Handle of dynamic transmission object
*                 idExtHi - Id low word (extended-format) to register with
*                                                         dynamic object
*                 idExtLo - Id high word (extended-format) 
* RETURN VALUE:   --
* DESCRIPTION:    Function registers submitted id (standard format)
*                 with dynamic object referenced by handle.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetExtId(CanTransmitHandle txHandle, vuint16 idExtHi, vuint16 idExtLo) C_API_3
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);   /* PRQA S 3109 */

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);  /* PRQA S 3109 */
# if ( kCanNumberOfTxStatObjects > 0) || defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /* PRQA S 3109 */ /*lint !e568 */
# endif
  assertUser(idExtHi <= (vuint16)0x1FFF, channel, kErrorWrongId);                                        /* PRQA S 3109 */

  dynTxObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel); /* PRQA S 3382,0291 */

# if defined( C_ENABLE_MIXED_ID )
#  if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#  else
  canDynTxIdType[dynTxObj] = kCanIdTypeExt; 
#  endif
# endif

  canDynTxId0[dynTxObj]      = MK_EXTID0( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canDynTxId1[dynTxObj]      = MK_EXTID1( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canDynTxId2[dynTxObj]      = MK_EXTID2( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canDynTxId3[dynTxObj]      = MK_EXTID3( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canDynTxId4[dynTxObj]      = MK_EXTID4( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
}
/* CODE CATEGORY 2 END*/
#endif


#if defined( C_ENABLE_DYN_TX_DLC )
/************************************************************************
* NAME:           CanDynTxObjSetDlc
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 dlc -
*                   data length code to register with dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Function registers data length code with 
*                 dynamic object referenced by submitted handle.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDlc(CanTransmitHandle txHandle, vuint8 dlc) C_API_3
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);     /* PRQA S 3109 */

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif
  
  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);  /* PRQA S 3109 */
# if ( kCanNumberOfTxStatObjects > 0) || defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /* PRQA S 3109 */ /*lint !e568 */
# endif
  assertUser(dlc<(vuint8)9, channel, kErrorTxDlcTooLarge);                                               /* PRQA S 3109 */
  
  dynTxObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel); /* PRQA S 3382,0291 */
  
# if defined( C_ENABLE_EXTENDED_ID )
  canDynTxDLC[dynTxObj] = MK_TX_DLC_EXT(dlc);
# else
  canDynTxDLC[dynTxObj] = MK_TX_DLC(dlc);
# endif
}
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_DYN_TX_DLC */


#if defined( C_ENABLE_DYN_TX_DATAPTR )
/************************************************************************
* NAME:           CanDynTxObjSetData
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 pData -
*                   data reference to be stored in data buffer of dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Functions stores reference to data registered with
*                 dynamic object.
*                 
*                 The number of byte copied is (always) 8. The number of 
*                 relevant (and consequently evaluated) byte is to be 
*                 taken from function CanDynObjGetDLC.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDataPtr(CanTransmitHandle txHandle, void* pData) C_API_3
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);    /* PRQA S 3109 */

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);  /* PRQA S 3109 */
# if ( kCanNumberOfTxStatObjects > 0) || defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /* PRQA S 3109 */ /*lint !e568 */
# endif

  dynTxObj = (txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel)) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);  /* PRQA S 3382,0291 */

  canDynTxDataPtr[dynTxObj] = pData;
}
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_DYN_TX_DATAPTR */




#if defined( C_ENABLE_TX_MASK_EXT_ID )
/************************************************************************
* NAME:               CanSetTxIdExtHi
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{  
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
# endif
  assertUser(mask <= (vuint8)0x1F, channel, kErrorWrongMask);                                /* PRQA S 3109 */

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0x00FFFFFFUL)) | MK_EXTID0((vuint32)mask<<24);
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0x00FFFFFFUL)) | MK_EXTID1((vuint32)mask<<24);
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0x00FFFFFFUL)) | MK_EXTID2((vuint32)mask<<24);
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0x00FFFFFFUL)) | MK_EXTID3((vuint32)mask<<24);
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0x00FFFFFFUL)) | MK_EXTID4((vuint32)mask<<24);
# endif
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetTxIdExtMidHi
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
# endif

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFF00FFFFUL)) | MK_EXTID0((vuint32)mask<<16);   /*lint !e572*/
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFF00FFFFUL)) | MK_EXTID1((vuint32)mask<<16);   /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFF00FFFFUL)) | MK_EXTID2((vuint32)mask<<16);   /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFF00FFFFUL)) | MK_EXTID3((vuint32)mask<<16);   /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFF00FFFFUL)) | MK_EXTID4((vuint32)mask<<16);   /*lint !e572*/
# endif
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetTxIdExtMidLo
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFFFF00FFUL)) | MK_EXTID0((vuint32)mask<<8);    /*lint !e572*/
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFFFF00FFUL)) | MK_EXTID1((vuint32)mask<<8);    /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFFFF00FFUL)) | MK_EXTID2((vuint32)mask<<8);    /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFFFF00FFUL)) | MK_EXTID3((vuint32)mask<<8);    /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFFFF00FFUL)) | MK_EXTID4((vuint32)mask<<8);    /*lint !e572*/
# endif
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetTxIdExtLo
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFFFFFF00UL)) | MK_EXTID0((vuint32)mask);     /*lint !e572*/
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFFFFFF00UL)) | MK_EXTID1((vuint32)mask);     /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFFFFFF00UL)) | MK_EXTID2((vuint32)mask);     /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFFFFFF00UL)) | MK_EXTID3((vuint32)mask);     /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFFFFFF00UL)) | MK_EXTID4((vuint32)mask);    /*lint !e572*/
# endif
}
/* CODE CATEGORY 4 END*/
#endif

#if defined( C_ENABLE_TX_OBSERVE )
/************************************************************************
* NAME:               CanTxGetActHandle
* CALLED BY:          
* PRECONDITIONS:      
* PARAMETER:          logical hardware object handle
* RETURN VALUE:       handle of the message in the assigned mailbox
* DESCRIPTION:        get transmit handle of the message, which is currently
*                     in the mailbox txHwObject.
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanTxGetActHandle( CanObjectHandle logicalTxHdl ) C_API_3
{
  assertUser(logicalTxHdl < kCanNumberOfUsedTxCANObjects, kCanAllChannels, kErrorTxHwHdlTooLarge);     /* PRQA S 3109 */

  return (canHandleCurTxObj[logicalTxHdl]);       /*lint !e661*/
}
/* CODE CATEGORY 3 END*/
#endif

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/************************************************************************
* NAME:               CanSetVariableRxDatalen
* CALLED BY:          
* PRECONDITIONS:      
* PARAMETER:          rxHandle: Handle of receive Message for which the datalen has
*                               to be changed
*                     dataLen:  new number of bytes, which have to be copied to the 
*                               message buffer.
* RETURN VALUE:       -
* DESCRIPTION:        change the dataLen of a receive message to copy a 
*                     smaller number of bytes than defined in the database.
*                     the dataLen can only be decreased. If the parameter
*                     dataLen is bigger than the statically defined value
*                     the statically defined value will be set.
*************************************************************************/
/* CODE CATEGORY 1 START*/
static void CanSetVariableRxDatalen (CanReceiveHandle rxHandle, vuint8 dataLen)
{
  assertInternal(rxHandle < kCanNumberOfRxObjects, kCanAllChannels , kErrorRxHandleWrong);  /* PRQA S 3109 */ /* legal rxHandle ? */
  /* assertion for dataLen not necessary due to runtime check */

  if (dataLen < CanGetRxDataLen(rxHandle))
  {
    canVariableRxDataLen[rxHandle]=dataLen;
  }
  else
  {
    canVariableRxDataLen[rxHandle] = CanGetRxDataLen(rxHandle);
  }
}
/* CODE CATEGORY 1 END*/
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
/************************************************************************
* NAME:               CanSetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:      
* PARAMETER:          -.
* RETURN VALUE:       -
* DESCRIPTION:        The service function CanSetMsgReceivedCondition()
*                     enables the calling of ApplCanMsgCondReceived()
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanSetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);     /* PRQA S 3109 */
# endif

  canMsgCondRecState[channel] = kCanTrue;
}
/* CODE CATEGORY 3 END*/

/************************************************************************
* NAME:               CanResetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:      
* PARAMETER:          -
* RETURN VALUE:       -
* DESCRIPTION:        The service function CanResetMsgReceivedCondition()
*                     disables the calling of ApplCanMsgCondReceived()
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanResetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);    /* PRQA S 3109 */
# endif

  canMsgCondRecState[channel] = kCanFalse;
}
/* CODE CATEGORY 3 END*/

/************************************************************************
* NAME:               CanGetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:      
* PARAMETER:          -
* RETURN VALUE:       status of Conditional receive function:
*                     kCanTrue : Condition is set -> ApplCanMsgCondReceived 
*                                will be called
*                     kCanFalse: Condition is not set -> ApplCanMsgCondReceived
*                                will not be called
* DESCRIPTION:        The service function CanGetMsgReceivedCondition() 
*                     returns the status of the condition for calling
*                     ApplCanMsgCondReceived()
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);   /* PRQA S 3109 */
# endif

  return( canMsgCondRecState[channel] );
}
/* CODE CATEGORY 3 END*/
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/************************************************************************
* NAME:           ApplCanChannelDummy
* PARAMETER:      channel
*                 current receive channel
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 ApplCanChannelDummy( CanChannelHandle channel )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;     /* to avoid lint warnings */
# endif
}
/* CODE CATEGORY 3 END*/
#endif   /* C_MULTIPLE_RECEIVE_CHANNEL */


#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) 
/************************************************************************
* NAME:           ApplCanRxStructPtrDummy
* PARAMETER:      rxStruct
*                 pointer of CanRxInfoStruct
* RETURN VALUE:   kCanCopyData 
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 vuint8 C_API_2 ApplCanRxStructPtrDummy( CanRxInfoStructPtr rxStruct )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  rxStruct = rxStruct;     /* to avoid lint warnings */
# endif
  return kCanCopyData;
}
/* CODE CATEGORY 1 END*/

/************************************************************************
* NAME:           ApplCanTxHandleDummy
* PARAMETER:      txHandle
*                 transmit handle
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 ApplCanTxHandleDummy( CanTransmitHandle txHandle )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  txHandle = txHandle;     /* to avoid lint warnings */
# endif
}
/* CODE CATEGORY 1 END*/
#endif   /* C_MULTIPLE_RECEIVE_CHANNEL || C_HL_ENABLE_DUMMY_FCT_CALL */


#if defined( C_ENABLE_RX_QUEUE )
/************************************************************************
* NAME:               CanHL_ReceivedRxHandleQueue
* CALLED BY:          CanBasicCanMsgReceived, CanFullCanMsgReceived
* Preconditions:      none
* PARAMETER:          none
* RETURN VALUE:       none
* DESCRIPTION:        Writes receive data into queue or starts further
*                     processing for this message
*************************************************************************/
/* CODE CATEGORY 1 START*/
static vuint8 CanHL_ReceivedRxHandleQueue(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus
  tCanRxInfoStruct    *pCanRxInfoStruct;

  V_DEF_P2VAR(V_NONE, tCanMsgTransmitStruct, AUTOMATIC, VAR_NOINIT) pSrc;
  V_DEF_P2VAR(V_NONE, tCanMsgTransmitStruct, AUTOMATIC, VAR_NOINIT) pDst;
  vuint8 canllidx;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);  /* PRQA S 3109 */
# endif

  /* Rx Queue is supported with C_HL_ENABLE_RX_INFO_STRUCT_PTR only! */
  pCanRxInfoStruct =  &canRxInfoStruct[channel];

  /* if C_ENABLE_APPLCANPRERXQUEUE is not set, a macro ApplCanPreRxQueue has to be provided by the tool */
  /* in case of ranges, ApplCanPreRxQueue has to return kCanCopyData! */
# if defined( C_ENABLE_APPLCANPRERXQUEUE )
  if(ApplCanPreRxQueue(CAN_HL_P_RX_INFO_STRUCT(channel)) == kCanCopyData)
# endif
  {
    /* Disable the interrupts because nested interrupts can take place -
      CAN interrupts of all channels have to be disabled here*/
    CanNestedGlobalInterruptDisable();
    if(canRxQueue.canRxQueueCount < kCanRxQueueSize)   /* Queue full ? */
    {
      if (canRxQueue.canRxQueueWriteIndex == (kCanRxQueueSize - 1) )
      {
        canRxQueue.canRxQueueWriteIndex = 0;
      }
      else
      {
        canRxQueue.canRxQueueWriteIndex++;
      }
      canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].Channel = channel;
      canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].Handle  = pCanRxInfoStruct->Handle;

      pDst = &canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].CanChipMsgObj;
      pSrc = (V_DEF_P2VAR_PARA(V_NONE, tCanMsgTransmitStruct, AUTOMATIC, VAR_NOINIT)) pCanRxInfoStruct->pChipMsgObj;
      for (canllidx=0; canllidx<8; canllidx++) {
        pDst->DataFld[canllidx] = pSrc->DataFld[canllidx];
      }
      pDst->DlcRaw = pSrc->DlcRaw;
      pDst->IdRaw0 = pSrc->IdRaw0;
      pDst->IdRaw1 = pSrc->IdRaw1;

      canRxQueue.canRxQueueCount++;
    }
# if defined( C_ENABLE_RXQUEUE_OVERRUN_NOTIFY )
    else
    {
      ApplCanRxQueueOverrun();
    }
# endif
    CanNestedGlobalInterruptRestore();
  } 
# if defined( C_ENABLE_APPLCANPRERXQUEUE )
  else
  {
    /* Call the application call-back functions and set flags */
#  if defined( C_ENABLE_RX_QUEUE_RANGE )
    if (pCanRxInfoStruct->Handle < kCanNumberOfRxObjects )
#  endif
    { 
      return CanHL_ReceivedRxHandle(CAN_CHANNEL_CANPARA_FIRST pCanRxInfoStruct);
    }
  }
# endif
  return kCanHlFinishRx;
}
/* CODE CATEGORY 1 END*/

/************************************************************************
* NAME:               CanHandleRxMsg
* CALLED BY:          Application
* Preconditions:      none
* PARAMETER:          none
* RETURN VALUE:       none
* DESCRIPTION:        Calls PreCopy and/or Indication, if existent and
*                     set the indication flag
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanHandleRxMsg(void)
{
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL
  tCanRxInfoStruct        localCanRxInfoStruct;

  while ( canRxQueue.canRxQueueCount != (vuintx)0 )
  {

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    channel = canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].Channel;
# endif


    CAN_CAN_INTERRUPT_DISABLE( CAN_CHANNEL_CANPARA_ONLY );

    /* Call the application call-back functions and set flags */
    localCanRxInfoStruct.Handle      = canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].Handle;
    localCanRxInfoStruct.pChipData   = (CanChipDataPtr)&(canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].CanChipMsgObj.DataFld[0]);
    canRDSRxPtr[channel] = localCanRxInfoStruct.pChipData;
    localCanRxInfoStruct.pChipMsgObj = (CanChipMsgPtr) &(canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].CanChipMsgObj);
    localCanRxInfoStruct.Channel     = channel;
    /* unused */

# if defined( C_ENABLE_RX_QUEUE_RANGE )
    switch ( localCanRxInfoStruct.Handle)
    {
#  if defined( C_ENABLE_RANGE_0 )
      case kCanRxHandleRange0: (void)APPLCANRANGE0PRECOPY( &localCanRxInfoStruct ); break;
#  endif
#  if defined( C_ENABLE_RANGE_1 )
      case kCanRxHandleRange1: (void)APPLCANRANGE1PRECOPY( &localCanRxInfoStruct ); break;
#  endif
#  if defined( C_ENABLE_RANGE_2 )
      case kCanRxHandleRange2: (void)APPLCANRANGE2PRECOPY( &localCanRxInfoStruct ); break;
#  endif
#  if defined( C_ENABLE_RANGE_3 )
      case kCanRxHandleRange3: (void)APPLCANRANGE3PRECOPY( &localCanRxInfoStruct ); break;
#  endif
      default:
#  if defined( C_ENABLE_INDICATION_FLAG ) || \
      defined( C_ENABLE_INDICATION_FCT )
             if( CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct ) == kCanHlContinueRx )
             {
               CanHL_IndRxHandle(localCanRxInfoStruct.Handle);
             }
#  else
             (void) CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct );
#  endif
             break;
   }
# else
#  if defined( C_ENABLE_INDICATION_FLAG ) || \
      defined( C_ENABLE_INDICATION_FCT )
    if( CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct ) == kCanHlContinueRx )
    {
      CanHL_IndRxHandle(localCanRxInfoStruct.Handle);
    }
#  else
    (void) CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct );
#  endif
# endif
    
    CAN_CAN_INTERRUPT_RESTORE( CAN_CHANNEL_CANPARA_ONLY );

    CanNestedGlobalInterruptDisable();
    if (canRxQueue.canRxQueueReadIndex == (kCanRxQueueSize - 1) )
    {
      canRxQueue.canRxQueueReadIndex = 0;
    }
    else
    {
      canRxQueue.canRxQueueReadIndex++;
    }
    canRxQueue.canRxQueueCount--;
    CanNestedGlobalInterruptRestore();
  }
  return;
} /* end of CanHandleRxMsg() */
/* CODE CATEGORY 2 END*/

/************************************************************************
* NAME:               CanDeleteRxQueue
* CALLED BY:          Application, CAN driver
* Preconditions:      none
* PARAMETER:          none
* RETURN VALUE:       none
* DESCRIPTION:        delete receive queue
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanDeleteRxQueue(void)
{
  CanDeclareGlobalInterruptOldStatus
  
  CanNestedGlobalInterruptDisable();
  canRxQueue.canRxQueueWriteIndex  = (vuintx)0xFFFFFFFF;
  canRxQueue.canRxQueueReadIndex   = 0;
  canRxQueue.canRxQueueCount       = 0;  
  CanNestedGlobalInterruptRestore();
} /* end of CanDeleteRxQueue() */
/* CODE CATEGORY 4 END*/

#endif /* C_ENABLE_RX_QUEUE */

#if defined(C_ENABLE_BASE_ADDRESS_UPDATE)
/************************************************************************
* NAME:           CanBaseAddressRequest
* CALLED BY:      Application
* Preconditions:  none
* PARAMETER:      channel: the CAN channel for which the address is requested
* RETURN VALUE:   ---
* DESCRIPTION:    The application calls this function in order to tell the 
*                 CAN driver to request the computation of the virtual 
*                 address of the CAN controller.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanBaseAddressRequest(CAN_CHANNEL_CANTYPE_ONLY)
{

  {
    /* unused */
    e.g. canNodePtrTemp[canHwChannel] = (tCanMSCAN*)ApplCanGetBaseAddress(<physical adress>, <size of CAN controller>);
  }
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:           CanBaseAddressActivate
* CALLED BY:      Application
* Preconditions:  Interrupts have to be disabled
* PARAMETER:      channel: the CAN channel for which the address is requested
* RETURN VALUE:   ---
* DESCRIPTION:    The application calls this function in order to tell the 
*                 CAN driver to activate the virtual address of the CAN 
*                 controller. The adress has to be requested with 
*                 CanBaseAddressRequest() before.
*                 Call is only allowed on Task level and must not interrupt
*                 any CAN driver service functions.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanBaseAddressActivate(CAN_CHANNEL_CANTYPE_ONLY)
{

  {
    /* unused */
    e.g. canNodePtr[canHwChannel]        = canNodePtrTemp[canHwChannel];
  }
}
/* CODE CATEGORY 4 END*/
#endif  /* defined(VGEN_ENABLE_MDWRAP) || defined(VGEN_ENABLE_QWRAP) || defined(C_ENABLE_BASE_ADDRESS_UPDATE) */



/* End of channel */
/* STOPSINGLE_OF_MULTIPLE */
/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/************   Organi, Version 3.9.0 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.9.0 Vector-Informatik GmbH  ************/
