/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* STARTSINGLE_OF_MULTIPLE */

#ifndef  CAN_HL_H
# define  CAN_HL_H


/* ***************************************************************************
| Project Name: DrvCan_TricoreMulticanHll
|    CBD: File Name: CAN_DEF.H
|    Asr: File Name: Can.h
|
|  Description: Application-Interface of the CAN-Driver
|               Declaration of functions, variables, and constants
|
|     Compiler: see module file
|
|-----------------------------------------------------------------------------
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
| --------     ---------------------     ------------------------------------
| Bir          Holger Birke              Vector Informatik GmbH
| Ces          Senol Cendere             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------ -- --------------------------------------------------
| 2003-09-08 1.00.00 Bir  - first implementation, Review modify
| 2004-01-24 1.01.00 Bir  - add sleepmode
|                         - MultiBasics
|                         - Workaround CAN_TC.21
| 2004-01-24 1.02.00 Bir  - Individual polling + Functions
|                         - lint
|                         - No Lock of HwObj with MsgVal
|                         - Use Hw-Fifo
|                         - Buffer Rx Basic+Full for consistency, adapt RDSmakros
|                         - 2nd review, FDRreg Initworkaround
| 2004-04-17 1.03.00 Bir  - lint
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
|                         - ESCAN00009610: Sleep, Wakeup not set
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
| 2006-03-08 2.00.00 Bir  - adapt to new RI1.5
|                         - new HL adaptions
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
|                    Bir  - ESCAN00039248: Error Frames during initialization
| 2007-09-25 2.02.00 Ces  - ESCAN00022662: support compiler Tasking V8.6R3 for XC2000
|                         - updated HLL component to V2.05.00
|                         - support configuration tool GENy for XC2000
| 2007-10-29 2.03.00 Bir  - ESCAN00023032: Add src table
|                         - None Postbuild tables moved from Can_PBcfg.h to Can_Lcfg.h (pointer removed)
| 2007-12-04 2.03.01 Bir  - ESCAN00023531: no change here
| 2007-12-10 2.04.00 Bir  - ESCAN00023626: Add new derivative 1797 (changed addressoffsets)
|                         - ESCAN00023538: Generate table canPhysPrio and canPhysGroup
| 2008-02-04 2.04.01 Bir  - ESCAN00024354: Update HL-ASR 2.22.01 (no change here)
| 2008-02-14 2.05.00 Bir  - ESCAN00024530: MEMORY_CAN for XC2000 near model to far
|                         - Update HL-ASR 3.0 (no change here)
| 2008-02-14 2.06.00 Bir  - review reworks
|                         - ESCAN00025994: Postbuild for table "CanInterruptPendingMask" (linktime before)
| 2008-07-04 2.07.00 Bir  - ESCAN00028128, ESCAN00028130: CAN DRV might execute the RX nofication handling 2 times for 1 message
|                         - ESCAN00029952: support 1767
|                         - ESCAN00029987: compiler warning unused variable "copy_cnt"
| 2008-09-23 2.08.00 Bir  - ESCAN00030164: support XC2361A (add also some other xc2000 derivatives and generic platforms)
|                           ESCAN00029997: missing hardware loop check
| 2008-12-04 2.08.01 Bir  - ESCAN00030164: check generated mailbox and node amount for generic platforms
| 2008-12-12 2.08.02 Bir  - ESCAN00031987: write on illlegal RAM address
| 2009-01-15 2.09.00 Bir  - ESCAN00032396: Add derivative 1768
|                         - ESCAN00032482: Add extended RAM check feature
|                         - ESCAN00033087: AutoSar only: Software Trap due to access NULL-pointer
| 2009-03-02 2.10.00 Bir  - ESCAN00033517: Add XC2000 platform for ASR + derivative XC2336B, XC2363B
| 2009-04-20 2.11.00 Bir  - ESCAN00034643: Add compiler and memory abstraction + update to Core ASR 3.08
|                         - ESCAN00036146: Runtime optimization
| 2009-07-22 2.11.01 Bir  - Remove compiler abstraction for automatic variable (not release for effected platform)
|                         - ESCAN00036616: activate Clock inside CanInit() to avoid problems when init is called in sleep mode
| 2009-09-04 2.11.02 Bir  - ESCAN00038205: FullCAN Rx with standard ID will be masked like extended ID
| 2010-01-19 2.12.00 Bir  - ESCAN00040216: ASR: Support Highend feature (Indiv.Polling, RxQueue, Multiple BasicCAN)
| 2010-03-03 2.13.00 Bir  - ESCAN00041312: Support Remote Frames
| 2010-03-29 2.14.00 Bir  - ESCAN00041897: support new derivative XC2267M
|                         - ESCAN00041898: Add Group priority extension bit GPX
|                         - ESCAN00041932: Can_TransmitRemote() will not send extended ID
|************************************************************************** */
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
| --------     ---------------------     ------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
| Pl           Georg Pfluegel            Vector Informatik GmbH
| Vg           Frank Voorburg            Vector CANtech, Inc.
| An           Ahmad Nasser              Vector CANtech, Inc.
| Ml           Patrick Markl             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------ -----------------------------------------------------
| ---------  ---  ------ -- --------------------------------------------------
| 24-Jan-01  0.02      Ht     - derived from C16x V3.3
| 18-Apr-01  1.00      Pl     - adaption for ARM7 TDMI
| 26-Apr-01  1.01      Ht     - adaption to LI 1.2
|                             - changed from code doupling to indexed
| 19-Jun-01            Ht     - define of CanSetActive/Passive changed
| 31-Oct-01  1.02      Ht     - support hash search
|                      Vg     - adaption for PowerPC
| 07-Nov-01  1.03      Ht     - remove some comments
| 12-Nov-01  1.04      Fz     - no changes
| 02-Jan-02  1.05      Ht     - ESCAN00002009: support of polling mode improved
| 12-Feb-02  1.06      Pl     - ESCAN00002279: - now it is possible to use only the error-task without the tx-task
|                                              - support of the makro  REENTRANT
|                                              - support of the makro C_HL_ENABLE_RX_INFO_STRUCT_PTR
|                                              - For better performance for the T89C51C there is a switch-case
|                                                instruction for direct call of the PreTransmitfunction
|                                              - insert Prototyp for CanErrorTask
| 18-Mai-02  1.07       Ht    - ESCAN....    : support Hash search with FullCAN controller
|                             - ESCAN00002707: Reception could went wrong if IL and Hash Search
|                             - ESCAN00002893: adaption to LI 1.3
| 29-Mai-02  1.08       Ht    - ESCAN00003028: Transmission could fail in Polling mode
|                             - ESCAN00003082: call Can_LL_TxEnd() in CanMsgTransmit()
|                             - ESCAN00003083: Hash search with extended ID
|                             - ESCAN00003084: Support C_COMP_METROWERKS_PPC
|                             - ESCAN00002164: Temporary vaiable "i" not defined in function CanBasicCanMsgReceived
|                             - ESCAN00003085: support C_HL_ENABLE_IDTYPE_IN_ID
| 25-Jun     1.08.01    Vg    - Declared localInterruptOldFlag in CanRxTask()
|                             - Corrected call to CanWakeUp for multichannel
| 11-Jul-02  1.08.02    Ht    - ESCAN00003203: Hash Search routine does not work will with extended IDs
|                             - ESCAN00003205: Support of ranges could went wrong on some platforms
| 08-Aug-02  1.08.03    Ht    - ESCAN00003447: Transmission without databuffer and pretransmit-function 
| 08-Aug-02  1.08.04    An      no changes
| 09-Sep-02  1.09       Ht    - ESCAN00003837: code optimication with KernelBuilder 
| 2002-12-12 1.10       Ht    -                support Dummy functions for indirect function call 
| 2003-02-04 1.11       Ht    -                optimization for polling mode
|                       Fz    - ESCAN00004600: Flags changed for V850 aFCAN only
| 2003-03-19 1.12       Ht    - ESCAN00005153: Wrong number of Indication bytes for V850 aFCAN only
|                             - ESCAN00005152: optimization of CanInit() in case of Direct Tx Objects
|                             - ESCAN00005143: CompilerWarning about function prototype 
|                                              CanHL_ReceivedRxHandle() and CanHL_IndRxHandle
|                             - ESCAN00005130: Wrong result of Heash Search on second or higher channel               
| 2003-05-12 1.13       Ht    - ESCAN00005624: support CantxMsgDestroyed for multichannel system
|                             - ESCAN00005209: Support confirmation and indication flags for EasyCAN4
|                             - ESCAN00004721: Change data type of Handle in CanRxInfoStruct
| 2003-06-18 1.20       Ht    - ESCAN00005908: support features of RI1.4
|                             - ESCAN: Support FJ16LX-Workaround for multichannel system
|                             - ESCAN00005671: Dynamic ID in extended ID system could fail
|                             - ESCAN00005863: Notification about cancelation failes in case of CanTxMsgDestroyed
| 2003-06-30 1.21       Ht   - ESCAN00006117: Common Confirmation Function: Write access to wrong memory location
|                            - ESCAN00006008: CanCanInterruptDisable in case of polling
|                            - ESCAN00006118: Optimization for Mixed ID and ID type in Own Register or ID type in ID Register
|                            - ESCAN00006063: Undesirable hardware dependency for 
|                                             CAN_HL (CanLL_RxBasicTxIdReceived)
| 2003-09-10 1.22       Ht   - ESCAN00006853: Support V_MEMROM0
|                            - ESCAN00006854: suppress some Compiler warnings
|                            - ESCAN00006856: support CanTask if only Wakeup in polling mode
|                            - ESCAN00006857: variable newDLC not defined in case of Variable DataLen
| 2003-10-14 1.23       Ht   - ESCAN00006858: support BrsTime for internal runtime measurement
|                            - ESCAN00006860: support Conditional Msg Receive
|                            - ESCAN00006865: support "Cancel in HW" with CanTask
|                            - ESCAN00006866: support Direct Tx Objects
|                            - ESCAN00007109: support new memory qualifier for const data and pointer to const
| 2004-01-05 1.24       Ml   - ESCAN00007206: no changes
|                       Ml   - ESCAN00007254: several changes
| 2004-02-06 1.25       Ml   - ESCAN00007281: no changes
|                       Ml   - removed compiler warnings
| 2004-02-21 1.26       Ml   - ESCAN00007670: CAN RAM check
|                       Ml   - ESCAN00007671: no changes
|                       Ml   - ESCAN00007764: no changes
|                       Ml   - ESCAN00007681: no changes
|                       Ml   - ESCAN00007272: no changes
|                       Ml   - ESCAN00008064: encapsulated CanRxHashId in case of array dimension == 0
| 2004-04-16 1.27       Ml   - ESCAN00008204: Optimized CanRxActualId for different ID modes
|                       Ml   - ESCAN00008160: encapsulated functions declared by tool in case of multichannel
|                       Ml   - ESCAN00008266: changed name of parameter of function CanTxGetActHandle
|                       Fz   - ESCAN00008272: Compiler error due to missing array canPollingTaskActive
| 2004-05-10 1.28       Fz   - ESCAN00008328: Compiler error if cancel in hardware is active
|                            - ESCAN00008363: Hole closed when TX in interrupt and cancel in HW is used
|                            - ESCAN00008365: Switch C_ENABLE_APPLCANPREWAKEUP_FCT added
|                            - ESCAN00008391: Wrong parameter macro used in call of 
|                                             CanLL_WakeUpHandling
| 2004-05-24 1.29       Ht   - ESCAN00008441: Interrupt not restored in case of internal error if TX Polling is used
| 2004-09-21 1.30       Ht   - ESCAN00008824: check of reference implementation version added
|                            - ESCAN00008825: No call of ApplCanMsgCancelNotification during CanInit()
|                            - ESCAN00008826: Support asssertions for "Conditional Message Received"  
|                       Ml   - ESCAN00008752: Added function qualifier macros
|                       Ht   - ESCAN00008823: compiler error due to array size 0
|                            - ESCAN00008977: label without instructions
|                            - ESCAN00009485: Message via Normal Tx Object will not be sent  
|                            - ESCAN00009497: support of CommonCAN and RX queue added
| 2004-09-28 1.31       Ht   - ESCAN00009703: unresolved functions CAN_POLLING_IRQ_DISABLE/RESTORE()
| 2004-11-25 1.32       Ht   - move fix for ESCAN00007671 to CAN-LL of DrvCan_MpcToucanHll
|                            - ESCAN00010350: Dynamic Tx messages are send always with Std. ID
|                            - ESCAN00010388: ApplCanMsgConfirmed will only be called if realy transmitted
|                       Ml   - ESCAN00009931: The HardwareLoopCheck should have a channelparameter in multichannel systems.
|                            - ESCAN00010093: lint warning: function type inconsistent "CanCheckMemory"
|                        Ht  - ESCAN00010811: remove Misra and compiler warnings
|                            - ESCAN00010812: support Multi ECU
|                            - ESCAN00010526: CAN interrupts will be disabled accidently
|                            - ESCAN00010584: ECU may crash or behave strange with Rx queue active
| 2005-01-20 1.33       Ht   - ESCAN00010877: ApplCanMsgTransmitConf() is called erronemous
| 2005-03-03 1.34       Ht   - ESCAN00011139: Improvement/Correction of C_ENABLE_MULTI_ECU_CONFIG
|                            - ESCAN00011511: avoid PC-Lint warnings
|                            - ESCAN00011512: copy DLC in case of variable Rx Datalen
|                            - ESCAN00010847: warning due to missing brakets in can_par.c at CanChannelObject
| 2005-05-23 1.35       Ht   - ESCAN00012445: compiler error "V_MEMROMO undefined"in case of multi ECU
|                            - ESCAN00012350: Compiler Error "Illegal token channel"
| 2005-07-06 1.36       Ht   - ESCAN00012153: Compile Error: missing declaration of variable i
|                            - ESCAN00012460: Confirmation of LowLevel message will run into assertion (C_ENABLE_MULTI_ECU_PHYS enabled)
|                            - support Testpoints for CanTestKit
| 2005-07-14 1.37       Ht   - ESCAN00012892: compile error due to missing logTxObjHandle
|                            - ESCAN00012998: Compile Error: missing declaration of txHandle in CanInit()
|                            - support Testpoints for CanTestKit for FullCAN controller
| 2005-10-05 1.38       Ht   - ESCAN00013597: Linker error: Undefined symbol 'CanHL_IndRxHandle'
| 2005-11-10 1.39.00    Ht   - ESCAN00014331: Compile error due to missing 'else' in function CanTransmit
| 2005-04-26 2.00.00    Ht   - ESCAN00016698: support RI1.5
|                            - ESCAN00014770: Cosmic compiler reports truncating assignement
|                            - ESCAN00016191: Compiler warning about unused variable in CanTxTask
| 2007-01-23 2.01.00    Ht   - ESCAN00017279: Usage of SingleGlobalInterruptDisable lead to assertion in OSEK
|                            - ESCAN00017148: Compile error in higher layer due to missing declaration of CanTxMsgHandleToChannel
| 2007-03-14 2.02.00   Ht    - ESCAN00019825: error directives added and misra changes
|                            - ESCAN00019827: adaption to never version of VStdLib.
|                            - ESCAN00019619: V_CALLBACK_1 and V_CALLBACK_2 not defined
|                            - ESCAN00019953: Handling of FullCAN reception in interrupt instead of polling or vice versa.
|                            - ESCAN00019958: CanDynTxObjSetId() or CanDynTxObjSetExtId() will run into assertion
| 2007-03-26 2.03.00  Ht     - ESCAN00019988: Compile warnings in can_drv.c
|                            - ESCAN00018831: polling mode: function prototype without function implemenation (CanRxFullCANTask + CanRxBasicCANTask)
| 2007-04-20 2.04.00  dH     - ESCAN00020299: user assertion fired irregularly due to unknown parameter (in case of CommonCAN)
| 2007-05-02 2.05.00  Ht     - ESCAN00021069: Handling of canStatus improved, usage of preprocessor defines unified
|                            - ESCAN00021070: support relocation of HW objects in case of Multiple configuration
|                            - ESCAN00021166: Compiler Warnings: canHwChannel & canReturnCode not used in CanGetStatus
|                            - ESCAN00021223: CanCanInterruptDisabled called during Sleepmode in CanWakeupTask
|                            - ESCAN00022048: Parameter of ApplCancorruptMailbox is hardware channel instead of logical channel - Error directive added
| 2007-11-12 2.06.00  Ht     - ESCAN00023188: support CAN Controller specific polling sequence for BasicCAN objects
|                            - ESCAN00023208: Compile issue about undefined variable kCanTxQueuePadBits in the CAN driver in Bit based Tx queue
| 2008-10-20 2.07.00  Ht     - ESCAN00023010: support disabled mailboxes in case of extended RAM check
|                            - ESCAN00025706: provide C_SUPPORTS_MULTI_ECU_PHYS
|                            - ESCAN00026120: compiler warnings found on DrvCan_V85xAfcanHll RI 1.5     ##Ht: reviewed 2008-09-03
|                            - ESCAN00026322: ApplCanMsgNotMatched not called in special configuration
|                            - ESCAN00026413: Add possibility to reject remote frames received by FullCAN message objects
|                            - ESCAN00028758: CAN HL must support QNX
|                            - ESCAN00029788: CommonCAN for Driver which support only one Tx object improved (CanInit()).
|                            - ESCAN00029889: Compiler warning about uninitialized variable canHwChannel in CanCanInterruptDisable/Restore()
|                            - ESCAN00029891: Compiler warning: variable "rxHandle" was set but never used
|                            - ESCAN00029929: Support Extended ID Masking for Tx Fullcan messages 
|                            - ESCAN00030371: Improvements (assertions, misra)
|                            - ESCAN00027931: Wrong check of "queueBitPos" size
| 2009-04-08 2.08.00  Ht     - ESCAN00034492: no automatic remove of CanCanInterruptDisable/Restore
|                            - ESCAN00031816: CANRANGExIDTYPE can be removed and direct expression used
|                            - ESCAN00032027: CanMsgTransmit shall support tCanMsgTransmitStruct pointer accesses to far RAM
|                            - ESCAN00034488: Postfix for unsigned const in perprocessor directives are not supported by all Compiler (ARM-Compiler 1.2)
| 2009-06-04 2.08.01  Ht     - ESCAN00035426: Compiler warning about truncation in CanGetStatus removed
| 2009-10-21 2.09.00  Ht     - ESCAN00036258: Compiler warning about "CanHL_ReceivedRxHandle" was declared but never referenced
|                            - ESCAN00038642: Support reentrant functions for compiler with own keyword
|                            - ESCAN00038645: support new switch C_ENABLE_UPDATE_BASE_ADDRESS
| 2010-02-01 2.10.00  Ht     - ESCAN00036260: Support configuartion without static Tx messages and only one channel (remove compiler warning)
|                            - ESCAN00040478: Handle update of virtual CanBaseAdress in accordance to QNX documentation
|                            - ESCAN00039235: Compiler Warning: Narrowing or Signed-to-Unsigned type conversion
|
|    ************    Version and change information of      **********        
|    ************    high level part only                   **********        
|
|    Please find the version number of the whole module in the previous 
|    File header.
|
|****************************************************************************/


/***************************************************************************/
/* Version                  (abcd: Main version ab Sub Version cd )        */
/***************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan_TricoreMulticanHll CQComponent : Implementation */
#define DRVCAN_TRICOREMULTICANHLL_VERSION 0x0214
#define DRVCAN_TRICOREMULTICANHLL_RELEASE_VERSION 0x00


/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan__coreHll CQComponent : Implementation */
# define DRVCAN__COREHLL_VERSION                 0x0210
# define DRVCAN__COREHLL_RELEASE_VERSION         0x00

#define DRVCAN__HLLTXQUEUEBIT_VERSION 0x0106
#define DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION 0x02

/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/

# include "v_def.h"

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/

/***************************************************************************/
/* Default switches                                                        */
/* Automatic define settings, depending on user configuration in can_cfg.h */
/***************************************************************************/
# if defined( C_ENABLE_MULTICHANNEL_API )
#  define C_MULTIPLE_RECEIVE_CHANNEL
#  define MULTIPLE_RECEIVE_CHANNEL
# else
#  define C_SINGLE_RECEIVE_CHANNEL
# endif

# if ( kCanNumberOfTxObjects > 0 )
#  if !(defined( C_ENABLE_CAN_TRANSMIT ) || defined( C_DISABLE_CAN_TRANSMIT ))
#   define C_ENABLE_CAN_TRANSMIT
#  endif
# endif

# if !(defined( C_ENABLE_OFFLINE ) || defined( C_DISABLE_OFFLINE ))
#  define C_ENABLE_OFFLINE
# endif

# if !(defined( C_ENABLE_STOP ) || defined( C_DISABLE_STOP ))
#  define C_ENABLE_STOP
# endif

# if !(defined( C_ENABLE_CAN_CAN_INTERRUPT_CONTROL ) || defined( C_DISABLE_CAN_CAN_INTERRUPT_CONTROL ))
#  define C_ENABLE_CAN_CAN_INTERRUPT_CONTROL
# endif

# if !(defined( C_ENABLE_CAN_CANCEL_TRANSMIT ) || defined( C_DISABLE_CAN_CANCEL_TRANSMIT ))
#  if defined( C_ENABLE_CAN_TRANSMIT )
#   define C_ENABLE_CAN_CANCEL_TRANSMIT
#  endif
# endif

# if !defined( kCanNumberOfHwChannels )
/* compatibility for older tool versions */
#  define kCanNumberOfHwChannels                 kCanNumberOfChannels
# endif

#define C_HL_DISABLE_OVERRUN_IN_STATUS

#define C_HL_DISABLE_HW_RANGES_FILTER

#define C_HL_ENABLE_IDTYPE_IN_ID


#if defined (C_HL_ENABLE_IDTYPE_IN_ID)
/* Specify the ID tables which contain the ID type */
# define C_LL_RX_IDTYPE_TABLE      CanRxId0
# define C_LL_TX_IDTYPE_TABLE      CanTxId0
#endif


#define C_HL_DISABLE_DUMMY_FCT_CALL


#define C_HL_DISABLE_TX_MSG_DESTROYED


#define C_HL_DISABLE_CANCEL_IN_HW_TASK


#define C_HL_DISABLE_HW_EXIT_TRANSMIT 

#define C_HL_ENABLE_LAST_INIT_OBJ     

#define C_HL_ENABLE_CAN_IRQ_DISABLE   

#define C_HL_DISABLE_ADJUST_RXHANDLE  


#define C_HL_DISABLE_REJECT_UNWANTED_IDTYPE

#define C_HL_DISABLE_REJECT_REMOTE_FRAME

#define C_HL_DISABLE_REJECT_REMOTE_FRAME_FULLCAN

#define C_HL_DISABLE_COPROCESSOR_SUPPORT


#define C_DISABLE_DRIVER_STATUS

#if defined( C_COMP_TASKING_TRICORE_MULTICAN ) 
# if !defined( V_CPU_TRICORE )
#  define V_CPU_TRICORE
# endif
#endif



/* default defines for advanced Interrupt control */
#if defined( V_CPU_TRICORE ) 
# if !defined(C_ENABLE_NESTED_INTERRUPTS) && !defined(C_DISABLE_NESTED_INTERRUPTS)
#  define C_ENABLE_NESTED_INTERRUPTS
# endif
# if !defined(C_ENABLE_CAN_INTERRUPT_CONTROL) && !defined(C_DISABLE_CAN_INTERRUPT_CONTROL)
#  define C_ENABLE_CAN_INTERRUPT_CONTROL
# endif
#endif

# if  !( (defined( C_ENABLE_RX_BASICCAN_POLLING ) || !defined( C_ENABLE_RX_BASICCAN_OBJECTS )) && \
        (defined( C_ENABLE_RX_FULLCAN_POLLING )  || !defined( C_ENABLE_RX_FULLCAN_OBJECTS ))  && \
         defined( C_ENABLE_TX_POLLING ) && !defined( C_ENABLE_INDIVIDUAL_POLLING ) && \
         defined( C_ENABLE_ERROR_POLLING )  ) /* ! pure polling configuration */
#  define C_LL_INTERRUPT_ISR_ACTIVE
# endif


#if !defined(C_ENABLE_SUPPORT_REMOTE_FRAME) && !defined(C_DISABLE_SUPPORT_REMOTE_FRAME)
# define C_DISABLE_SUPPORT_REMOTE_FRAME
#endif


# if !defined( C_CALLBACK_1 )
#  define C_CALLBACK_1
# endif

# if !defined( C_CALLBACK_2 )
#  define C_CALLBACK_2
# endif

# if !defined( C_API_1 )
#  define C_API_1
# endif

# if !defined( C_API_2 )
#  define C_API_2
# endif

# if !defined( C_API_3 )
#  define C_API_3                                REENTRANT
# endif

# if !defined( REENTRANT )
#  define REENTRANT
# endif

# if defined( C_ENABLE_TX_POLLING )
#  if !(defined( C_ENABLE_ERROR_POLLING )  || defined( C_DISABLE_ERROR_POLLING ))
#   define C_ENABLE_ERROR_POLLING 
#  endif
# endif

# if !(defined( C_ENABLE_TASK_RECURSION_CHECK ) || defined( C_DISABLE_TASK_RECURSION_CHECK ))
#  define C_ENABLE_TASK_RECURSION_CHECK
# endif

# if defined( C_ENABLE_MULTI_ECU_PHYS )
#  define C_SUPPORTS_MULTI_ECU_PHYS
# endif

/***************************************************************************/
/* return values                                                           */
/***************************************************************************/

/* return values for precopy-routines */
# define kCanNoCopyData                          ((vuint8)0x00)
# define kCanCopyData                            ((vuint8)0x01)

/* Bitmask of return value of CanGetStatus() */
# define kCanTxOff                               ((vuint8)0x00)
# define kCanStatusInit                          ((vuint8)0x00)
# define kCanTxOn                                ((vuint8)0x01)
# define kCanTxNotOn                             ((vuint8)0xFE)              /* internal use only */
# define kCanHwIsStop                            ((vuint8)0x02)
# define kCanHwIsInit                            ((vuint8)0x04)
# define kCanHwIsInconsistent                    ((vuint8)0x08)              /* used of for common CAN */
# define kCanHwIsWarning                         ((vuint8)0x10)
# define kCanHwIsPassive                         ((vuint8)0x20)
# define kCanHwIsBusOff                          ((vuint8)0x40)
# define kCanHwIsSleep                           ((vuint8)0x80)

/* CanTransmit return values -----------------------------------------------*/
# define kCanTxFailed                            ((vuint8)0x00)  /* Tx path switched off or no sending possible */
# define kCanTxOk                                ((vuint8)0x01)  /* msg transmitted or in queue                 */
# define kCanTxPartOffline                       ((vuint8)0x02)  /* Tx path switched part off or           */
# define kCanCommunicationDisabled               ((vuint8)0x03)  /* if application has disabled com after memory check */

# if defined( C_DRV_INTERNAL )
#  define kCanTxNotify                           ((vuint8)0x05)  /* internal returncode only - not used for the API */
# endif

/* CanGetDynTxObj return values -----------------------------------------------*/
# define kCanNoTxDynObjAvailable                 ((CanTransmitHandle)0xFFFFFFFFU)

/* CanReleaseDynTxObj return values -------------------------------------------*/
# define kCanDynReleased                         ((vuint8)0x00)
# define kCanDynNotReleased                      ((vuint8)0x01)

/* generel return values */
# define kCanFailed                              ((vuint8)0x00)
# define kCanOk                                  ((vuint8)0x01)
# define kCanNotSupported                        ((vuint8)0x02)

# define kCanFalse                               ((vuint8)0x00)
# define kCanTrue                                ((vuint8)0x01)

/* CanTxGetActHandle return value - status of transmit objects */
# define kCanBufferMsgTransmit                   ((CanTransmitHandle)0xFFFFFFFCU)   /* mark a transmit object as used by CanMsgTransmit */

/* results of CAN RAM check */
# define kCanRamCheckFailed                      ((vuint8)0x00)
# define kCanRamCheckOk                          ((vuint8)0x01)
# define kCanDisableCommunication                ((vuint8)0x00)
# define kCanEnableCommunication                 ((vuint8)0x01)


/* return values of CanRxActualIdType */
#define kCanIdTypeStd                           (0x0000U)
#define kCanIdTypeExt                           (0x2000U)
#define kCanIdTypeExtMask                       ((tCanIdType)0xDFFF)

/***************************************************************************/
/* parameter values                                                        */
/***************************************************************************/
/* parameters for partial offline */
# define kCanTxPartInit                          ((vuint8)0x00)

/* paramater for assertions */
# define kCanAllChannels                         ((CanChannelHandle)0xFFU)
/* used for generated tables */
# define kCanChannelNotUsed                      ((CanChannelHandle)0xFFU)

/* canRxHandle does not contain a certain message handle or entry in table is not used */
# define kCanRxHandleNotUsed                     ((CanReceiveHandle)  0xFFFFFFFFU)
# define kCanTxHandleNotUsed                     ((CanTransmitHandle) 0xFFFFFFFFU)

# if defined( C_DRV_INTERNAL )
#  define kCanRxHandleRange0                     ((CanReceiveHandle)  0xFFFFFFF0U)
#  define kCanRxHandleRange1                     ((CanReceiveHandle)  0xFFFFFFF1U)
#  define kCanRxHandleRange2                     ((CanReceiveHandle)  0xFFFFFFF2U)
#  define kCanRxHandleRange3                     ((CanReceiveHandle)  0xFFFFFFF3U)

/* entries for generated table CanHwMsgTransmitIndex[], CanHwTxNormalIndex[] - unused in case of CommonCan */
/* table entry does not contain a HW object assignment */
#  define kCanObjectHandleNotUsed                ((CanObjectHandle)0xFFU)
# endif

/* parameter for ApplCanTimerStart(),-End(), -Loop()*/
/* index has to start with 0, continues numbers have to be used. Gabs are not 
   allowed! */
#define kCanLoopListInit        ((vuint8)0x00)
#define kCanLoopIrqReq          ((vuint8)0x01)
#define kCanLoopRx              ((vuint8)0x02)

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

# if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#  define CAN_CHANNEL_CANTYPE_ONLY               void                                 /* PRQA S 3460 */
#  define CAN_CHANNEL_CANTYPE_FIRST
#  define CAN_CHANNEL_CANTYPE_LOCAL         
#  define CAN_CHANNEL_CANPARA_ONLY
#  define CAN_CHANNEL_CANPARA_FIRST
#  define CAN_HW_CHANNEL_CANTYPE_ONLY            void                                 /* PRQA S 3460 */
#  define CAN_HW_CHANNEL_CANTYPE_FIRST
#  define CAN_HW_CHANNEL_CANTYPE_LOCAL
#  define CAN_HW_CHANNEL_CANPARA_ONLY
#  define CAN_HW_CHANNEL_CANPARA_FIRST
# else
#  define CAN_CHANNEL_CANTYPE_ONLY               CanChannelHandle channel
#  define CAN_CHANNEL_CANTYPE_FIRST              CanChannelHandle channel,
#  define CAN_CHANNEL_CANTYPE_LOCAL              CanChannelHandle channel;             /* PRQA S 3412 */
#  define CAN_CHANNEL_CANPARA_ONLY               channel
#  define CAN_CHANNEL_CANPARA_FIRST              channel,
#  define CAN_HW_CHANNEL_CANTYPE_ONLY            CanChannelHandle canHwChannel
#  define CAN_HW_CHANNEL_CANTYPE_FIRST           CanChannelHandle canHwChannel,
#  define CAN_HW_CHANNEL_CANTYPE_LOCAL           CanChannelHandle canHwChannel;        /* PRQA S 3412 */
#  define CAN_HW_CHANNEL_CANPARA_ONLY            canHwChannel
#  define CAN_HW_CHANNEL_CANPARA_FIRST           canHwChannel,
# endif

# if defined( C_DRV_INTERNAL )
/* macros to fill struct elements RangeMask and RangeCode of type tCanChannelObject - used by generation tool only */
#  if (kCanNumberOfUsedCanRxIdTables == 1)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 2)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 3)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i),MK_RX_RANGE_MASK_IDSTD2(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i),MK_RX_RANGE_CODE_IDSTD2(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 4)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i),MK_RX_RANGE_MASK_IDSTD2(i),MK_RX_RANGE_MASK_IDSTD3(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i),MK_RX_RANGE_CODE_IDSTD2(i),MK_RX_RANGE_CODE_IDSTD3(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 5)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i),MK_RX_RANGE_MASK_IDSTD2(i),MK_RX_RANGE_MASK_IDSTD3(i),MK_RX_RANGE_MASK_IDSTD4(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i),MK_RX_RANGE_CODE_IDSTD2(i),MK_RX_RANGE_CODE_IDSTD3(i),MK_RX_RANGE_CODE_IDSTD4(i)}
#  endif

#  if (kCanNumberOfUsedCanRxIdTables == 1)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 2)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 3)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i),MK_RX_RANGE_MASK_IDEXT2(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i),MK_RX_RANGE_CODE_IDEXT2(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 4)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i),MK_RX_RANGE_MASK_IDEXT2(i),MK_RX_RANGE_MASK_IDEXT3(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i),MK_RX_RANGE_CODE_IDEXT2(i),MK_RX_RANGE_CODE_IDEXT3(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 5)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i),MK_RX_RANGE_MASK_IDEXT2(i),MK_RX_RANGE_MASK_IDEXT3(i),MK_RX_RANGE_MASK_IDEXT4(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i),MK_RX_RANGE_CODE_IDEXT2(i),MK_RX_RANGE_CODE_IDEXT3(i),MK_RX_RANGE_CODE_IDEXT4(i)}
#  endif
# endif

/* calculate size of cond/ind-flags */
# define kCanNumberOfConfBytes                   ((kCanNumberOfConfFlags + 7)/8)
# define kCanNumberOfConfWords                   ((kCanNumberOfConfFlags + 15)/16)
# define kCanNumberOfConfDWords                  ((kCanNumberOfConfFlags + 31)/32)
# define kCanNumberOfIndBytes                    ((kCanNumberOfIndFlags + 7)/8)
# define kCanNumberOfIndWords                    ((kCanNumberOfIndFlags + 15)/16)
# define kCanNumberOfIndDWords                   ((kCanNumberOfIndFlags + 31)/32)

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */

# if defined( C_ENABLE_EXTENDED_ID )
#  if defined( C_ENABLE_MIXED_ID )
#   define CanRxActualId(rxStruct)               ((CanRxActualIdType(rxStruct) == kCanIdTypeExt) ?      \
                                                 CanRxActualExtId(rxStruct) : ((vuint32)CanRxActualStdId(rxStruct)))      /* returns vuint32 */
#  else
#   define CanRxActualId(rxStruct)               (CanRxActualExtId(rxStruct))             /* returns vuint32 */
#  endif
# else
#  define CanRxActualId(rxStruct)                (CanRxActualStdId(rxStruct))             /* returns vuint16 */
# endif

/* Macros for CAN Status */
# define CanHwIsOk(state)                        (((state) & (kCanHwIsWarning |     \
                                                    kCanHwIsPassive |     \
                                                    kCanHwIsBusOff) )    ? 0 : 1)
# define CanHwIsWarning(state)                   (((state) & kCanHwIsWarning)      ? 1 : 0)
# define CanHwIsPassive(state)                   (((state) & kCanHwIsPassive)      ? 1 : 0)
# define CanHwIsBusOff(state)                    (((state) & kCanHwIsBusOff)       ? 1 : 0)
# define CanHwIsWakeup(state)                    (((state) & kCanHwIsSleep)        ? 0 : 1)
# define CanHwIsSleep(state)                     (((state) & kCanHwIsSleep)        ? 1 : 0)
# define CanHwIsStop(state)                      (((state) & kCanHwIsStop)         ? 1 : 0)
# define CanHwIsStart(state)                     (((state) & kCanHwIsStop)         ? 0 : 1)
# define CanIsOnline(state)                      (((state) & kCanTxOn)             ? 1 : 0)
# define CanIsOffline(state)                     (((state) & kCanTxOn)             ? 0 : 1)
# define CanHwIsInconsistent(state)              (((state) & kCanHwIsInconsistent) ? 1 : 0)



#  if defined( C_DRV_INTERNAL )
#   define CanGetTxId0(i)                        (CanTxId0[i])
#   define CanGetTxId1(i)                        (CanTxId1[i])
#   define CanGetTxId2(i)                        (CanTxId2[i])
#   define CanGetTxId3(i)                        (CanTxId3[i])
#   define CanGetTxId4(i)                        (CanTxId4[i])
#   if defined( C_ENABLE_EXTENDED_ID )
#    if defined( C_ENABLE_MIXED_ID )
#     if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#      define CanGetTxIdType(i)                  (C_LL_TX_IDTYPE_TABLE[i] & kCanIdTypeExt)
#     else
#       define CanGetTxIdType(i)                 (CanTxIdType[i] & kCanIdTypeExt)
#     endif
#    else
#     define CanGetTxIdType(i)                   (kCanIdTypeExt)
#    endif
#   else
#    define CanGetTxIdType(i)                    (kCanIdTypeStd)
#   endif
#  endif /* C_DRV_INTERNAL */

#  define CanGetTxDlc(i)                         (CanTxDLC[i])
#  define CanGetTxDataPtr(i)                     (CanTxDataPtr[i])
#  define CanGetConfirmationOffset(i)            (CanConfirmationOffset[i])
#  define CanGetConfirmationMask(i)              (CanConfirmationMask[i])
#   define CanGetTxHwObj(i)                      (CanTxHwObj[i])
#  define CanGetTxSendMask(i)                    (CanTxSendMask[i])
#  define CanGetApplPreTransmitPtr(i)            (CanTxApplPreTransmitPtr[i])
#  define CanGetApplConfirmationPtr(i)           (CanTxApplConfirmationPtr[i])
#  define CanGetChannelOfTxObj(i)                (CanTxMsgHandleToChannel[i])

#  if defined( C_DRV_INTERNAL )
#   define CanGetRxId0(i)                        (CanRxId0[i])
#   define CanGetRxId1(i)                        (CanRxId1[i])
#   define CanGetRxId2(i)                        (CanRxId2[i])
#   define CanGetRxId3(i)                        (CanRxId3[i])
#   define CanGetRxId4(i)                        (CanRxId4[i])
#   if defined( C_ENABLE_EXTENDED_ID )
#    if defined( C_ENABLE_MIXED_ID )
#     if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#      define CanGetRxIdType(i)                  (C_LL_RX_IDTYPE_TABLE[i] & kCanIdTypeExt)
#     else
#      define CanGetRxIdType(i)                  (CanRxIdType[i] & kCanIdTypeExt)
#     endif
#    else
#     define CanGetRxIdType(i)                   (kCanIdTypeExt)
#    endif
#   else
#    define CanGetRxIdType(i)                    (kCanIdTypeStd)
#   endif
#  endif /* C_DRV_INTERNAL */

#  define CanGetRxDataLen(i)                     (CanRxDataLen[i])
#  define CanGetRxMinDataLen(i)                  (CanRxMinDataLen[i])
#  define CanGetRxDataPtr(i)                     (CanRxDataPtr[i])
#  define CanGetIndicationOffset(i)              (CanIndicationOffset[i])
#  define CanGetIndicationMask(i)                (CanIndicationMask[i])
#  define CanGetApplPrecopyPtr(i)                (CanRxApplPrecopyPtr[i])
#  define CanGetApplIndicationPtr(i)             (CanRxApplIndicationPtr[i])

# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )  && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  /* set confirmation active for CanReleaseDynTxObj() */
#  if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#   define CanConfirmStart(txHandle)             {confirmHandle[0] = (txHandle);}                 /* PRQA S 3458 */
#   define CanConfirmEnd()                       {confirmHandle[0] = kCanTxHandleNotUsed;}        /* PRQA S 3458 */
#  else
#   define CanConfirmStart(channel, txHandle)    {confirmHandle[channel] = (txHandle);}           /* PRQA S 3458 */
#   define CanConfirmEnd(channel)                {confirmHandle[channel] = kCanTxHandleNotUsed;}  /* PRQA S 3458 */
#  endif
# endif

# if defined( C_ENABLE_PART_OFFLINE )
#  if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#   define CanPartOffline(sendGroup)             (CanSetPartOffline( sendGroup))
#   define CanPartOnline(sendGroup)              (CanSetPartOnline( (vuint8)~(vuint8)(sendGroup)) )
#  else
#   define CanPartOffline(channel, sendGroup)    (CanSetPartOffline((channel), (sendGroup)))
#   define CanPartOnline(channel,  sendGroup)    (CanSetPartOnline((channel), (vuint8)~(vuint8)(sendGroup)) )
#  endif
# else
#  if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#   define CanPartOffline(sendGroup)
#   define CanPartOnline(sendGroup)
#  else
#   define CanPartOffline(channel, sendGroup)
#   define CanPartOnline(channel, sendGroup)
#  endif
# endif

/* provide interface for interrupt disable and restore functions */
# define CanGlobalInterruptDisable               VStdSuspendAllInterrupts
# define CanGlobalInterruptRestore               VStdResumeAllInterrupts

# if ( C_SECURITY_LEVEL == 0 )
#  define CanDeclareGlobalInterruptOldStatus
#  define CanPutGlobalInterruptOldStatus(x)
#  define CanGetGlobalInterruptOldStatus(x)
#  define CanNestedGlobalInterruptDisable()
#  define CanNestedGlobalInterruptRestore()
# else  /* C_SECURITY_LEVEL == 0 */
#  define CanDeclareGlobalInterruptOldStatus     VStdDeclareGlobalInterruptOldStatus
#  define CanPutGlobalInterruptOldStatus(x)      VStdPutGlobalInterruptOldStatus(x)
#  define CanGetGlobalInterruptOldStatus(x)      VStdGetGlobalInterruptOldStatus(x)
#  if (VSTDLIB__COREHLL_VERSION  <  0x0216 )
#   define CanNestedGlobalInterruptDisable()      VStdNestedGlobalInterruptDisable()
#   define CanNestedGlobalInterruptRestore()      VStdNestedGlobalInterruptRestore()
#  else
#   define CanNestedGlobalInterruptDisable()      VStdGlobalInterruptDisable()
#   define CanNestedGlobalInterruptRestore()      VStdGlobalInterruptRestore()
#  endif
# endif /* C_SECURITY_LEVEL == 0 */


/* provide support for multiple callbacks */
#if !defined( APPL_CAN_GENERIC_PRECOPY )
# define APPL_CAN_GENERIC_PRECOPY                          ApplCanGenericPrecopy
#endif
#if !defined( APPL_CAN_TX_CONFIRMATION )
# define APPL_CAN_TX_CONFIRMATION                          ApplCanTxConfirmation
#endif
#if !defined( APPL_CAN_MSGTRANSMITCONF )
# define APPL_CAN_MSGTRANSMITCONF                          ApplCanMsgTransmitConf
#endif
#if !defined( APPL_CAN_MSGTRANSMITINIT )
# define APPL_CAN_MSGTRANSMITINIT                          ApplCanMsgTransmitInit
#endif
#if !defined( APPL_CAN_ONLINE )
# define APPL_CAN_ONLINE                                   ApplCanOnline
#endif
#if !defined( APPL_CAN_OFFLINE )
# define APPL_CAN_OFFLINE                                  ApplCanOffline
#endif
#if !defined( APPL_CAN_MSGRECEIVED )
# define APPL_CAN_MSGRECEIVED                              ApplCanMsgReceived
#endif
#if !defined( APPL_CAN_CANCELNOTIFICATION )
# define APPL_CAN_CANCELNOTIFICATION                       ApplCanCancelNotification
#endif
#if !defined( APPL_CAN_MSGCANCELNOTIFICATION )
# define APPL_CAN_MSGCANCELNOTIFICATION                    ApplCanMsgCancelNotification
#endif

#if defined( C_CPUTYPE_32BIT )
# define kCanTxQueueSize      ((kCanTxQueueBytes + 3) >> 2)
#endif


#define C_DLC_M           ((vuint8)0x0F)

/* Transmit priority is set in the upper 2 bits, EID/SID is set in the bit below prio */
#define C_EID_MASK_16     ((vuint16)kCanIdTypeExt)
#define C_EID_MASK_32     (((vuint32)kCanIdTypeExt) << 16)
#define C_EID_NOT_MASK_16 kCanIdTypeExtMask

#if defined( C_ENABLE_VARIABLE_DLC )
/* mask to keep some bits unchanged in the DLC for use with variable DLC */
/*  - bit remaining unchanged should be masked with 1 e.g. id type or direction (difference between MK_TX_DLC and MK_TX_DLC_EXT?) */
#define CanLL_DlcMask            0x00
#endif



/* macros to set DLC and ID in rx-,tx-structures ---------------------------*/
/* MK... macros are used by the generation tool to fill the rx and tx structures */
/* In case the IDE bit is located in the ID registers, this bit has to be handled as must match.
   In general, this is done by setting the IDE bit to 0 in the .._MASK_.. macros.
*/

#define MK_RX_RANGE_MASK_IDSTD0(id)   ( MK_STDID0(id) )
#define MK_RX_RANGE_MASK_IDSTD1(id)   ( 0x00 )
/* macros with postfix 2-4 not used */

#define MK_RX_RANGE_CODE_IDSTD0(id)   ( MK_STDID0(id) & C_PRIO_NOT_MASK_16 )
#define MK_RX_RANGE_CODE_IDSTD1(id)   ( 0x00 )
/* macros with postfix 2-4 not used */

#define MK_RX_RANGE_MASK_IDEXT0(id)   ( MK_EXTID0(id) & C_EID_NOT_MASK_16 )
#define MK_RX_RANGE_MASK_IDEXT1(id)   ( MK_EXTID1(id) )
/* macros with postfix 2-4 not used */

#define MK_RX_RANGE_CODE_IDEXT0(id)   ( MK_EXTID0(id) & C_PRIO_NOT_MASK_16 )
#define MK_RX_RANGE_CODE_IDEXT1(id)   ( MK_EXTID1(id) )
/* macros with postfix 2-4 not used */


/*lint -emacro( (778), MK_STDID0) */
#define MK_STDID0(id)            (C_OBJ_PRIO_16 | (vuint16)(((id) & (vuint16)0x07FF) << 2))
#define MK_STDID1(id)            ((vuint16)0x0000)
/* macros with postfix 2-4 not used */


/*lint -emacro( (572,778), MK_EXTID0) */
#define MK_EXTID0(id)            (C_OBJ_PRIO_16 | C_EID_MASK_16 | (vuint16)(((id)&(vuint32)0x1FFF0000) >> 16) )
#define MK_EXTID1(id)            ((vuint16) (id))
/* macros with postfix 2-4 not used */



#define MK_TX_DLC(dlc)       ((dlc) & C_DLC_M)
#define MK_TX_DLC_EXT(dlc)   (MK_TX_DLC(dlc))

#define XT_TX_DLC(dlc)       ((dlc) & C_DLC_M)


#if defined( C_SINGLE_RECEIVE_CHANNEL )
/* Bus-Off functions */
# define CanResetBusOffStart(a)  (CanInit(a))
# define CanResetBusOffEnd(a)

/* Bus-Sleep functions */
# define CanResetBusSleep(a)     (CanInit(a))

#else
/* Bus-Off functions */
# define CanResetBusOffStart(ch, a)  (CanInit((ch), (a)))
# define CanResetBusOffEnd(ch, a)
  
/* Bus-Sleep functions */
# define CanResetBusSleep(ch, a)     (CanInit((ch), (a)))
#endif


/* The macros CanRxActualIdRawx() have to provide the register context of the ID registers.
   It is not necessary to mask any bits. These macros are for internal use only and masking
   will be done in can_hls.c
*/
#if defined( C_ENABLE_EXTENDED_ID )
# define CanRxActualIdRaw0(rxStruct)   (*((CanChipMsgPtr16)((rxStruct)->pChipMsgObj+0x1A)))
# define CanRxActualIdRaw1(rxStruct)   (*((CanChipMsgPtr16)((rxStruct)->pChipMsgObj+0x18)))
/* macros with postfix 2-4 not used */
#else
# define CanRxActualIdRaw0(rxStruct)    (*((CanChipMsgPtr16)((rxStruct)->pChipMsgObj+0x1A)))
/* macros with postfix 1-4 not used */
#endif


/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */
#if defined( C_ENABLE_EXTENDED_ID )
# define CanRxActualIdExtHi(rxStruct)      (*((rxStruct)->pChipMsgObj+0x1B ) & (vuint8)0x1F )
# define CanRxActualIdExtMidHi(rxStruct)   (*((rxStruct)->pChipMsgObj+0x1A ) )
# define CanRxActualIdExtMidLo(rxStruct)   (*((rxStruct)->pChipMsgObj+0x19 ) )
# define CanRxActualIdExtLo(rxStruct)      (*((rxStruct)->pChipMsgObj+0x18 ) )
#endif

#define CanRxActualExtId(rxStruct)       (  (*((CanChipMsgPtr32)((rxStruct)->pChipMsgObj+0x18))      & (vuint32)0x1FFFFFFF))
#define CanRxActualStdId(rxStruct)       ((((*((CanChipMsgPtr16)((rxStruct)->pChipMsgObj+0x1A)))>>2) & (vuint16)0x07FF))

#define CanRxActualData(rxStruct, i)     ((rxStruct)->pChipData[i])
#define CanRxActualDLC(rxStruct)        (*((rxStruct)->pChipMsgObj+0x03) & C_DLC_M)
/*lint -emacro( (826), CanRxActualIdType) */
#define CanRxActualIdType(rxStruct)     ((*((CanChipMsgPtr16)((rxStruct)->pChipMsgObj+0x1A))) & C_EID_MASK_16)

#if defined (C_SINGLE_RECEIVE_CHANNEL )
# define CanRxActualErrorCounter()             (*((CanChipMsgPtr)(CanNodeAdr[0]+(vuint32)0x14)))
# define CanTxActualErrorCounter()             (*((CanChipMsgPtr)(CanNodeAdr[0]+(vuint32)0x15)))
#else
# define CanRxActualErrorCounter(ch)      (*((CanChipMsgPtr)(CanNodeAdr[(ch)]+(vuint32)0x14)))
# define CanTxActualErrorCounter(ch)      (*((CanChipMsgPtr)(CanNodeAdr[(ch)]+(vuint32)0x15)))
#endif

/* Macros for CAN message access within PreTransmit() function */
#define CanTxWriteActId(txStruct, id)     { *((CanChipMsgPtr16)((txStruct).pChipData+0x0A)) = MK_STDID0((id));   }
#if defined( C_ENABLE_EXTENDED_ID )
# define CanTxWriteActExtId(txStruct, id) { *((CanChipMsgPtr32)((txStruct).pChipData+0x08)) = ((C_OBJ_PRIO_32 | C_EID_MASK_32 | ((id) & (vuint32)0x1FFFFFFF)));  }
#endif

#define CanTxWriteActDLC(txStruct, dlc)   { *((txStruct).pChipData-0x0D) = ((vuint8) (dlc));}


/* Macros to fill variables of the type tCanMsgTransmitStruct */
#define CanMsgTransmitSetStdId(pCanMsgTransmitStruct, id)               { (pCanMsgTransmitStruct)->IdRaw0 = MK_STDID0((id));  \
                                                                          (pCanMsgTransmitStruct)->IdRaw1 = MK_STDID1((id));  }
#define CanMsgTransmitSetExtId(pCanMsgTransmitStruct, id)               { (pCanMsgTransmitStruct)->IdRaw0 = MK_EXTID0((id));  \
                                                                          (pCanMsgTransmitStruct)->IdRaw1 = MK_EXTID1((id));  }
#define CanMsgTransmitSetDlc(pCanMsgTransmitStruct, dlc)                { (pCanMsgTransmitStruct)->DlcRaw = MK_TX_DLC((dlc)); }
#define CanMsgTransmitSetData(pCanMsgTransmitStruct, dataByte, msgData) { (pCanMsgTransmitStruct)->DataFld[dataByte] = (msgData); }

/* macros for buffer access */
#define StartRxReadSync()          (VStdSuspendAllInterrupts())
#define EndRxReadSync()            (VStdResumeAllInterrupts())

#define StartRxWriteSync()         (VStdSuspendAllInterrupts())
#define EndRxWriteSync()           (VStdResumeAllInterrupts())

#define StartTxReadSync()
#define EndTxReadSync()

#define StartTxWriteSync()         (VStdSuspendAllInterrupts())
#define EndTxWriteSync()           (VStdResumeAllInterrupts())

#define StartRxWriteCANSync() 
#define EndRxWriteCANSync()   

#define StartRxReadCANSync()  
#define EndRxReadCANSync()    

/* look for newdat-bit and read until no change */
#define StartRxFullCANReadSync(Obj)
#define EndRxFullCANReadSync(Obj)

#define StartRxFullCANWriteSync(obj)
#define EndRxFullCANWriteSync(obj)

/* macros for flag access */
#define CanStartFlagWriteSync()    (VStdSuspendAllInterrupts())
#define CanEndFlagWriteSync()      (VStdResumeAllInterrupts())



/***************************************************************************/
/* error codes                                                             */
/***************************************************************************/
/* error numbers for User Assertions 0x00-0x1f - hardware independed */
# define kErrorTxDlcTooLarge                     ((vuint8)0x01)
# define kErrorTxHdlTooLarge                     ((vuint8)0x02)
# define kErrorIntRestoreTooOften                ((vuint8)0x03)
# define kErrorIntDisableTooOften                ((vuint8)0x04)
# define kErrorChannelHdlTooLarge                ((vuint8)0x05)
# define kErrorInitObjectHdlTooLarge             ((vuint8)0x06)
# define kErrorTxHwHdlTooLarge                   ((vuint8)0x07)
# define kErrorHwObjNotInPolling                 ((vuint8)0x08)
# define kErrorHwHdlTooSmall                     ((vuint8)0x09)
# define kErrorHwHdlTooLarge                     ((vuint8)0x0A)

# define kErrorAccessedInvalidDynObj             ((vuint8)0x0B)
# define kErrorAccessedStatObjAsDyn              ((vuint8)0x0C)
# define kErrorDynObjReleased                    ((vuint8)0x0D)

# define kErrorPollingTaskRecursion              ((vuint8)0x0E)
# define kErrorDisabledChannel                   ((vuint8)0x0F)
# define kErrorDisabledTxMessage                 ((vuint8)0x10)
# define kErrorDisabledCanInt                    ((vuint8)0x11)

# define kErrorCanSleep                          ((vuint8)0x12)
# define kErrorCanOnline                         ((vuint8)0x13)
# define kErrorCanStop                           ((vuint8)0x14)
# define kErrorWrongMask                         ((vuint8)0x15)
# define kErrorWrongId                           ((vuint8)0x16)
/* error numbers for User Assertions 0x20-0x3f - hardware depended */


/* error numbers for Gentool Assertions 0x40-0x5f */
# define kErrorTxROMDLCTooLarge                  ((vuint8)0x40)
# define kErrorWrongHwTxObjHandle                ((vuint8)0x41)
# define kErrorHwToLogTxObjCalculation           ((vuint8)0x42)

/* error numbers for Gentool Assertions 0x60-0x7f - hardware depended */
#define kErrorBasicCANMask                   ((vuint8)0x60)

/* error numbers for Hardware Assertions 0x80-0x9f */
# define kErrorTxBufferBusy                      ((vuint8)0x80)

/* error numbers for Hardware Assertions 0xa0-0xbf - hardware depended */
#define kErrorUnknownInterrupt               ((vuint8)0xa0)
#define kErrorUnknownISRPriority             ((vuint8)0xa1)
#define kErrorUnknownISRGroup                ((vuint8)0xa2)

/* error numbers for Internal Assertions 0xc0-0xdf */
# define kErrorTxHandleWrong                     ((vuint8)0xC0)
# define kErrorInternalTxHdlTooLarge             ((vuint8)0xC1)
# define kErrorRxHandleWrong                     ((vuint8)0xC2)        
# define kErrorTxObjHandleWrong                  ((vuint8)0xC3)
# define kErrorReleasedUnusedDynObj              ((vuint8)0xC4)
# define kErrorTxQueueTooManyHandle              ((vuint8)0xC5)
# define kErrorInternalChannelHdlTooLarge        ((vuint8)0xC6)
# define kErrorInternalDisabledChannel           ((vuint8)0xC7)
# define kErrorInternalDisabledTxMessage         ((vuint8)0xC8)

/* error numbers for Internal Assertions 0xe0-0xff - hardware depended */


/***************************************************************************/
/* DummyFunction                                                           */
/***************************************************************************/
# if defined( C_DRV_INTERNAL )
#  define CAN_RECEIVE_FCT_DUMMY                  ApplCanRxStructPtrDummy
#  define CAN_RANGE_FCT_DUMMY                    ApplCanRxStructPtrDummy
#  define CAN_BUSOFF_FCT_DUMMY                   ApplCanChannelDummy
#   define CAN_INDICATION_FCT_DUMMY              V_NULL
#   define CAN_PRECOPY_FCT_DUMMY                 V_NULL
#   define CAN_CONFIRMATION_FCT_DUMMY            V_NULL
#   define CAN_PRETRANSMIT_FCT_DUMMY             V_NULL
# endif

# if defined( C_NO_COMPATIBILITY )
# else
/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/

#  define CAN_TX_DLC(i)                          (CanGetTxDlc(i))            
#  define CAN_TX_DATA_PTR(i)                     (CanGetTxDataPtr(i))         
#  define CAN_CONFIRMATION_OFFSET(i)             (CanGetConfirmationOffset(i))
#  define CAN_CONFIRMATION_MASK(i)               (CanGetConfirmationMask(i))

#  define CAN_RX_DATA_LEN(i)                     (CanGetRxDataLen(i))
#  define CAN_RX_DATA_PTR(i)                     (CanGetRxDataPtr(i))
#  define CAN_INDICATION_OFFSET(i)               (CanGetIndicationOffset(i))
#  define CAN_INDICATION_MASK(i)                 (CanGetIndicationMask(i))

#  define CanInterruptDisable()                  (VStdSuspendAllInterrupts())
#  define CanInterruptRestore()                  (VStdResumeAllInterrupts())

#  define ApplCanReceiveDummy0                   ApplCanRxStructPtrDummy
#  define ApplCanReceiveDummy1                   ApplCanChannelDummy

#  define CanGetTxDirectMsg                      (CanGetTxHwObj)

#  define canRxDlcRam                            (canVariableRxDataLen)

#  define CAN_START_INDEX_INIT_OBJ(i)            (CanInitObjectStartIndex[i])
#  define CAN_START_INDEX_TX_OBJ(i)              (CanTxStartIndex[i])
#  define CAN_START_INDEX_RX_OBJ(i)              (CanRxStartIndex[i])

#  if !defined( VGEN_GENY )
#  endif
/* CANGen use old define kCanNumberOfMailboxes */
#if defined(kCanNumberOfMailboxes) && !defined(kCanNumberOfHwObjIndivPolling)
#define kCanNumberOfHwObjIndivPolling kCanNumberOfMailboxes
#endif

/***************************************************************************/
/* end of compatibility defines                                            */
/***************************************************************************/
# endif /* C_NO_COMPATIBILITY */

/***************************************************************************/
/* Memory qualifier                                                        */
/***************************************************************************/
/* memory qualifier for the CAN controller registers */
# define MEMORY_CAN

/***************************************************************************/
/* data types                                                              */
/***************************************************************************/

/* declare data type for the queue */
#if defined( C_CPUTYPE_32BIT )
typedef vuint32 tCanQueueElementType;
#endif
/* This defines below used in ASR and generated Data / so its essential to put in Can.h over this token */
#if   defined (C_PROCESSOR_INFINEON_TC1796)
# define CAN_MSGOBJ_OFFSET        ((vuint16)0x0600)
# define CAN_MAX_NODE             (4)       /* 4 real + 0 reserved */
# define CAN_MAX_LIST             (8)       /* 8 real + 0 reserved */
# define CAN_MAX_MSPND            (8)       /* 8 real + 0 reserved */
# define CAN_MAX_MSID             (8+24)    /* 8 real + 24 reserved */
# define CAN_MAX_NUM_MSGOBJ       (128)
# define CAN_MAX_NUM_NODE         (4)
# define CAN_MAX_NUM_SRC          (16)
#elif defined (C_PROCESSOR_INFINEON_TC1797) 
# define CAN_MSGOBJ_OFFSET        ((vuint16)0x1000)
# define CAN_MAX_NODE             (4+10)    /* 4 real + 10 reserved */
# define CAN_MAX_LIST             (8+8)     /* 8 real + 8 reserved */
# define CAN_MAX_MSPND            (8+8)     /* 8 real + 8 reserved */
# define CAN_MAX_MSID             (8+8)     /* 8 real + 8 reserved */
# define CAN_MAX_NUM_MSGOBJ       (128)
# define CAN_MAX_NUM_NODE         (4)
# define CAN_MAX_NUM_SRC          (6)
#elif defined (C_PROCESSOR_INFINEON_TC1766)
# define CAN_MSGOBJ_OFFSET        ((vuint16)0x0400)
# define CAN_MAX_NODE             (2)       /* 2 real + 0  reserved */
# define CAN_MAX_LIST             (8)       /* 8 real + 0  reserved */
# define CAN_MAX_MSPND            (8)       /* 8 real + 0  reserved */
# define CAN_MAX_MSID             (8+24)    /* 8 real + 24 reserved */
# define CAN_MAX_NUM_MSGOBJ       (64)
# define CAN_MAX_NUM_NODE         (2)
# define CAN_MAX_NUM_SRC          (6)
#elif defined (C_PROCESSOR_INFINEON_TC1767)
# define CAN_MSGOBJ_OFFSET        ((vuint16)0x1000)
# define CAN_MAX_NODE             (2+12)    /* 2 real + 12 reserved */
# define CAN_MAX_LIST             (8+8)     /* 8 real + 8  reserved */
# define CAN_MAX_MSPND            (8+8)     /* 8 real + 8  reserved */
# define CAN_MAX_MSID             (8+8)     /* 8 real + 8  reserved */
# define CAN_MAX_NUM_MSGOBJ       (64)
# define CAN_MAX_NUM_NODE         (2)
# define CAN_MAX_NUM_SRC          (16)
#elif defined (C_PROCESSOR_INFINEON_TC1768) /* same as TC1387 */
# define CAN_MSGOBJ_OFFSET        ((vuint16)0x1000)
# define CAN_MAX_NODE             (3+11)    /* 2 real + 12 reserved */
# define CAN_MAX_LIST             (8+8)     /* 8 real + 8  reserved */
# define CAN_MAX_MSPND            (8+8)     /* 8 real + 8  reserved */
# define CAN_MAX_MSID             (8+8)     /* 8 real + 8  reserved */
# define CAN_MAX_NUM_MSGOBJ       (128)
# define CAN_MAX_NUM_NODE         (3)
# define CAN_MAX_NUM_SRC          (16)
#elif defined (C_PROCESSOR_INFINEON_TC1XXX)
# define CAN_MSGOBJ_OFFSET        ((vuint16)0x1000)
# define CAN_MAX_NODE             (8+6)     /* 8 real + 6 reserved */
# define CAN_MAX_LIST             (8+8)     /* 8 real + 8 reserved */
# define CAN_MAX_MSPND            (8+8)     /* 8 real + 8 reserved */
# define CAN_MAX_MSID             (8+8)     /* 8 real + 8 reserved */
# define CAN_MAX_NUM_SRC          (16)
# if defined(CAN_MAX_MAILBOX_AMOUNT)
#  define CAN_MAX_NUM_MSGOBJ       (CAN_MAX_MAILBOX_AMOUNT)
# else
#  define CAN_MAX_NUM_MSGOBJ       (256)
#  if defined(C_ENABLE_WORKAROUND_TC21) || defined(C_ENABLE_HWBUFFER)
#   error "C_ENABLE_WORKAROUND_TC21 nor C_ENABLE_HWBUFFER work for generic derivative (amount of mailboxes is not given) define (CAN_MAX_MAILBOX_AMOUNT) in user config file"
#  endif
# endif
# if defined(CAN_MAX_HW_NODE_AMOUNT)
#  define CAN_MAX_NUM_NODE         CAN_MAX_HW_NODE_AMOUNT
# else
#  define CAN_MAX_NUM_NODE         (8)
# endif
#else
# error "unknown derivative used"
#endif
/* check generate against expected node and mailbox amount */
#if defined(CAN_MAX_MAILBOX_AMOUNT)
# if (CAN_MAX_NUM_MSGOBJ != CAN_MAX_MAILBOX_AMOUNT)
#   error "Generated Max amount of mailboxes does not fit expected amount for this derivative"
# endif
#endif
#if defined(CAN_MAX_HW_NODE_AMOUNT)
# if (CAN_MAX_NUM_NODE != CAN_MAX_HW_NODE_AMOUNT)
#   error "Generated Max amount of Nodes (hardware channels) does not fit expected amount for this derivative"
# endif
#endif


#define CAN_MAX_PND_REG      (CAN_MAX_NUM_MSGOBJ >> 5)
#if !defined (C_ENABLE_TX_PRIO_BYID) && !defined (C_DISABLE_TX_PRIO_BYID)
# define C_ENABLE_TX_PRIO_BYID
#endif
#if defined (C_ENABLE_TX_PRIO_BYID)
# define C_OBJ_PRIO_16     ((vuint16)0x8000U)
# define C_OBJ_PRIO_32     ((vuint32)0x80000000U)
#else  /* 0x80 dont work in first step TC.012 */
# define C_OBJ_PRIO_16     ((vuint16)0xC000U)
# define C_OBJ_PRIO_32     ((vuint32)0xC0000000U)
#endif /* C_TX_PRIO_BYID */

#define C_PRIO_MASK_16     ((vuint16)0xC000U)
#define C_PRIO_NOT_MASK_16 ((vuint16)0x3FFFU)

#define C_NODE_0  ((vuint8)0)
#define C_NODE_1  ((vuint8)1)
#define C_NODE_2  ((vuint8)2)
#define C_NODE_3  ((vuint8)3)
#define C_NODE_4  ((vuint8)4)

/* data types for driver function parameters */
/* the types of CanInitHandle, CanChannelHandle and CanObjectHandle depends
   on the compiler and controller - what generates effective code */
typedef vuint8            CanInitHandle;
typedef vuint8            CanChannelHandle;
/* CanObjectHandle has to be unsigned! */
typedef vuint16           CanObjectHandle;

typedef vuint16           CanTransmitHandle;
typedef vuint16           CanReceiveHandle;

/* define datatype of local signed variables for message handles */

typedef vsintx CanSignedTxHandle;

typedef vsintx CanSignedRxHandle;
 /* ptr to msgObj & data may point to RAM or CAN cell depend on Rx or Tx and feature C_ENABLE_HWBUFFER */
 /*  so the pointers have to be the same for all (far or nothing / far also works for RAM) */
V_DEF_P2SFR_CAN_TYPE(volatile, vuint8)  CanChipDataPtr;
V_DEF_P2SFR_CAN_TYPE(volatile, vuint8)  CanChipMsgPtr;
V_DEF_P2SFR_CAN_TYPE(volatile, vuint16) CanChipMsgPtr16;
V_DEF_P2SFR_CAN_TYPE(volatile, vuint32) CanChipMsgPtr32;

/* up to 5 tables can be defined for the transmission ID. The type and how 
   many tables are needed depends on the CPU bus type and on the ID registers 
   of the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCanRxId0 of type vuint16. The same CPU
   with 5 byte ID registers for extended ID needs in this case 3 tables and
   tCanRxId0 = vuint16, tCanRxId1 = vuint16 and tCanRxId2 = vuint16. */

typedef vuint16          tCanTxId0;
typedef vuint16 tCanTxId1;
/* macros with postfix 2-4 not used */

/* type of variable for ID type kCanIdTypeExt or kCanIdTypeStd will be assigned to 
   a variable of this type */
typedef vuint16          tCanIdType;

/* up to 3 tables can be defined for the reception ID. The type and how many
   tables are needed depends on the CPU bus type and on the ID registers of
   the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCanRxId0 of type vuint16. The same CPU
   with 5 byte ID registers for extended ID needs in this case 3 tables and 
   tCanRxId0 = vuint16, tCanRxId1 = vuint16 and tCanRxId2 = vuint16. */

typedef vuint16          tCanRxId0;
typedef vuint16          tCanRxId1;
/* macros with postfix 2-4 not used */

/* type of the variable, in which the old status of the interrupt flag or level is stored */
typedef vuint32 tCanHLIntOld;

/* this type could also be a struct if more than one variable is necessary */
typedef vuint32 tCanLLCanIntOld;

/* stucture of the CAN message object used by CanMsgTransmit() and RxQueue */
typedef struct
{
  vuint8      reserved1[3];
  vuint8      DlcRaw;
  vuint8      reserved2[12];
  vuint8      DataFld[8];
  tCanTxId1   IdRaw1;
  tCanTxId0   IdRaw0;
} tCanMsgTransmitStruct;

typedef volatile struct tCanIntPendMask
{ /* help to optimize runtime for interrupt-pending-search */
  vuint32 GlobalMask[CAN_MAX_PND_REG];
  vuint32 TxMask[CAN_MAX_PND_REG];
  vuint32 RxFullMask[CAN_MAX_PND_REG];
  vuint32 RxBasicMask[CAN_MAX_PND_REG];
} tCanIntPendMaskStruct;
/* This section (include in header Asr) */
#ifdef C_COMP_TASKING_TRICORE_MULTICAN
# if !defined(COMPILER_VERSION_1) && !defined(COMPILER_VERSION_2) && !defined(COMPILER_VERSION_3)
/* __VERSION__ define is not usable due to compiler versions 3.x expand it as string but 2.x as integer and 1.x does not support it */
#  define COMPILER_VERSION_2 /* set as default */
# endif
#endif

typedef struct 
{
  CanChipDataPtr     pChipData;
  CanTransmitHandle  Handle;
/* CPU-specific part */
} tCanTxInfoStruct;

/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
typedef struct 
{
  CanChannelHandle   Channel;
  CanTransmitHandle  Handle;
/* CPU-specific part */
} tCanTxConfInfoStruct;

typedef struct 
{
  CanChannelHandle  Channel;
  CanChipMsgPtr     pChipMsgObj;
  CanChipDataPtr    pChipData;
  CanReceiveHandle  Handle;
/* CPU-specific part */
} tCanRxInfoStruct;

typedef tCanRxInfoStruct          *CanRxInfoStructPtr;
typedef tCanTxInfoStruct          CanTxInfoStruct;
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
typedef tCanTxConfInfoStruct      *CanTxInfoStructPtr;

/* types of application functions called by driver ---------------------------*/
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplCanMsgRcvFct)    (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplRangeFct)        (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplPrecopyFct)      (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplIndicationFct)   (CanReceiveHandle rxObject);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplConfirmationFct) (CanTransmitHandle txObject);
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplPreTransmitFct)  (CanTxInfoStruct txStruct);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplChannelFct)      (CAN_CHANNEL_CANTYPE_ONLY);

typedef struct
{
  tCanRxId0           Id0;
# if (kCanNumberOfUsedCanRxIdTables > 1)
  tCanRxId1           Id1;
# endif
# if (kCanNumberOfUsedCanRxIdTables > 2)
  tCanRxId2           Id2;
# endif     
# if (kCanNumberOfUsedCanRxIdTables > 3)
  tCanRxId3           Id3;
# endif
# if (kCanNumberOfUsedCanRxIdTables > 4)
  tCanRxId4           Id4;
# endif
} tCanStructRxIdType;

# if defined( C_DRV_INTERNAL )
typedef struct
{
  ApplChannelFct      ApplCanWakeUpFct; 
  ApplCanMsgRcvFct    ApplCanMsgReceivedFct;
  ApplRangeFct        ApplCanRangeFct[4];
  ApplChannelFct      ApplCanBusOffFct;
  ApplConfirmationFct ApplCanCancelNotificationFct;
  ApplChannelFct      ApplCanMsgTransmitCancelNotifyFct;
  tCanStructRxIdType  RangeMask[4];
  tCanStructRxIdType  RangeCode[4];
  vuint16             RangeActiveFlag;
  tCanIdType          RangeIdType[4];
} tCanChannelObject;
# endif


/* datatypes for indication and confirmation flags */
# if defined( C_ENABLE_CONFIRMATION_FLAG )
union CanConfirmationBits                                 
{    /* PRQA S 0750 */
  vuint8         _c[kCanNumberOfConfBytes];
#  if defined( V_CPUTYPE_BITARRAY_32BIT )
#   if kCanNumberOfConfFlags > 16
  struct _c_bits32 w[kCanNumberOfConfDWords];
#   elif kCanNumberOfConfFlags > 8
  struct _c_bits16 w[kCanNumberOfConfWords];
#   else
  struct _c_bits8  w[1];
#   endif
#  else
#   if kCanNumberOfConfFlags > 8
  struct _c_bits16 w[kCanNumberOfConfWords];
#   else
  struct _c_bits8  w[1];
#   endif
#  endif
}; 
# endif


# if defined( C_ENABLE_INDICATION_FLAG )
union CanIndicationBits                                  
{    /* PRQA S 0750 */
  vuint8         _c[kCanNumberOfIndBytes];
#  if defined( V_CPUTYPE_BITARRAY_32BIT )
#   if kCanNumberOfIndFlags > 16
  struct _c_bits32 w[kCanNumberOfIndDWords];
#   elif kCanNumberOfIndFlags > 8
  struct _c_bits16 w[kCanNumberOfIndWords];
#   else
  struct _c_bits8  w[1];
#   endif
#  else
#   if kCanNumberOfIndFlags > 8
  struct _c_bits16 w[kCanNumberOfIndWords];
#   else
  struct _c_bits8 w[1];
#   endif
#  endif
};
# endif

# if defined( C_ENABLE_RX_QUEUE )
/* struct for receive-Queue. Should correspond to t_MsgObject */
typedef struct
{
  CanReceiveHandle      Handle;
  CanChannelHandle      Channel;
  tCanMsgTransmitStruct CanChipMsgObj;
}
tCanRxQueueObject;

/* the queue */
typedef struct 
{
  tCanRxQueueObject canRxQueueBuf[kCanRxQueueSize];  /* buffer for msg and handle */  
  volatile vuintx   canRxQueueWriteIndex;            /* index in canRxQueueBuf */
  volatile vuintx   canRxQueueReadIndex;             /* index in canRxQueueBuf */
  volatile vuintx   canRxQueueCount;                 /* count of messages in canRxQueueBuf  */   
}
tCanRxQueue;
# endif


/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/

/*  CAN driver version */
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCanMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCanSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCanBugFixVersion;
/* ROM CATEGORY 4 END*/



# if defined( C_ENABLE_PRECOPY_FCT )
/* RAM CATEGORY 1 START*/
extern volatile CanReceiveHandle canRxHandle[kCanNumberOfChannels];
/* RAM CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_CONFIRMATION_FLAG )
/* RAM CATEGORY 1 START*/
V_MEMRAM0 extern volatile V_MEMRAM1_NEAR union CanConfirmationBits V_MEMRAM2_NEAR CanConfirmationFlags;              /* PRQA S 0759 */
/* RAM CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_INDICATION_FLAG )
/* RAM CATEGORY 1 START*/
V_MEMRAM0 extern volatile V_MEMRAM1_NEAR union CanIndicationBits   V_MEMRAM2_NEAR CanIndicationFlags;                /* PRQA S 0759 */
/* RAM CATEGORY 1 END*/
# endif


# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )  && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
/* RAM CATEGORY 3 START*/
extern CanTransmitHandle          confirmHandle[kCanNumberOfChannels];
/* RAM CATEGORY 3 END*/
# endif

/* ##RI1.4 - 3.31: Dynamic Receive DLC */
# if defined( C_ENABLE_VARIABLE_RX_DATALEN )
#  if ( kCanNumberOfRxObjects > 0 )
/* RAM CATEGORY 1 START*/
extern volatile vuint8 canVariableRxDataLen[kCanNumberOfRxObjects];
/* RAM CATEGORY 1 END*/
#  endif
# endif

/* RAM CATEGORY 1 START*/
extern CanChipDataPtr canRDSRxPtr[kCanNumberOfChannels];
/* RAM CATEGORY 1 END*/
/* RAM CATEGORY 1 START*/
extern CanChipDataPtr canRDSTxPtr[kCanNumberOfUsedTxCANObjects];
/* RAM CATEGORY 1 END*/


/***************************************************************************/
/* Data structures filled with data by parameter generating tool           */
/***************************************************************************/

# if defined( C_DRV_INTERNAL )
/* structures for init and common objects -----------------------*/
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#  else
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 tCanChannelObject   V_MEMROM2 CanChannelObject[kCanNumberOfChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 vuint8              V_MEMROM2 CanInitObjectStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 4 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle   V_MEMROM2 CanTxDynRomStartIndex[kCanNumberOfChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle   V_MEMROM2 CanTxDynRamStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle    V_MEMROM2 CanRxBasicStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle    V_MEMROM2 CanRxFullStartIndex[kCanNumberOfHwChannels+1];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanLogHwTxStartIndex[kCanNumberOfHwChannels+1];
/* ROM CATEGORY 2 END*/

/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwTxStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 2 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxFullStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxBasicStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwUnusedStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 4 END*/
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwTxStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 2 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxFullStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxBasicStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwUnusedStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 4 END*/

/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwMsgTransmitIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwTxNormalIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 vsintx              V_MEMROM2 CanTxOffsetHwToLog[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/


#  endif  /* C_SINGLE_RECEIVE_CHANNEL */




#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* ROM CATEGORY 1 START*/
/* define first index to array CanHwObjIndivPolling[][] */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2     CanHwObjIndivPolling[1][kCanNumberOfHwObjIndivPolling];
/* ROM CATEGORY 1 END*/
#  endif

# endif /* C_DRV_INTERNAL */

# if defined( C_SINGLE_RECEIVE_CHANNEL )
# else
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle   V_MEMROM2 CanTxStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 1 END*/ 
/* ROM CATEGORY 2 START*/ 
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle    V_MEMROM2 CanRxStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 2 END*/
# endif

/* ROM generated */

# if ( kCanNumberOfTxObjects > 0 )
#   if defined( C_DRV_INTERNAL )
/* structures for transmit objects -----------------------*/
/* ROM CATEGORY 1 START*/ 
V_MEMROM0 extern V_MEMROM1 tCanTxId0 V_MEMROM2       CanTxId0[kCanNumberOfTxObjects];  
#    if (kCanNumberOfUsedCanTxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanTxId1 V_MEMROM2       CanTxId1[kCanNumberOfTxObjects];  
#    endif
#    if (kCanNumberOfUsedCanTxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanTxId2 V_MEMROM2       CanTxId2[kCanNumberOfTxObjects];  
#    endif
#    if (kCanNumberOfUsedCanTxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanTxId3 V_MEMROM2       CanTxId3[kCanNumberOfTxObjects];  
#    endif
#    if (kCanNumberOfUsedCanTxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanTxId4 V_MEMROM2       CanTxId4[kCanNumberOfTxObjects];  
#    endif

#    if defined( C_ENABLE_MIXED_ID )
#    endif
/* ROM CATEGORY 1 END*/
#   endif /* C_DRV_INTERNAL */

/* ROM CATEGORY 2 START*/
#   if defined( C_ENABLE_PART_OFFLINE )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanTxSendMask[kCanNumberOfTxObjects];
#   endif
/* ROM CATEGORY 2 END*/

/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanTxDLC[kCanNumberOfTxObjects];
V_MEMROM0 extern V_MEMROM1 TxDataPtr V_MEMROM2       CanTxDataPtr[kCanNumberOfTxObjects];

#   if defined( C_ENABLE_CONFIRMATION_FLAG )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanConfirmationOffset[kCanNumberOfTxObjects];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanConfirmationMask[kCanNumberOfTxObjects];
#   endif
#   if defined( C_ENABLE_CONFIRMATION_FCT )
V_MEMROM0 extern V_MEMROM1 ApplConfirmationFct V_MEMROM2 CanTxApplConfirmationPtr[kCanNumberOfTxObjects];
#   endif
#   if defined( C_ENABLE_PRETRANSMIT_FCT )
V_MEMROM0 extern V_MEMROM1 ApplPreTransmitFct  V_MEMROM2 CanTxApplPreTransmitPtr[kCanNumberOfTxObjects];
#   endif
/* ROM CATEGORY 1 END*/

#   if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanTxHwObj[kCanNumberOfTxObjects];
/* ROM CATEGORY 2 END*/
#   endif

#   if defined( C_SINGLE_RECEIVE_CHANNEL )
#   else
#    if ( kCanNumberOfTxObjects > 0 )
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanChannelHandle    V_MEMROM2 CanTxMsgHandleToChannel[kCanNumberOfTxObjects];
/* ROM CATEGORY 2 END*/
#    endif
#   endif /* C_SINGLE_RECEIVE_CHANNEL */ 

#   if defined( C_ENABLE_MULTI_ECU_PHYS )
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 tVIdentityMsk V_MEMROM2      CanTxIdentityAssignment[kCanNumberOfTxObjects];
/* ROM CATEGORY 4 END*/
#   endif


#  if defined( C_ENABLE_TRANSMIT_QUEUE )
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/* ROM CATEGORY 1 START*/
/* pad bits added before the current channel position*/
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 CanTxQueuePadBits[kCanNumberOfChannels];

/* start / stop indices for the element search*/
V_MEMROM0 extern V_MEMROM1 CanSignedTxHandle V_MEMROM2 CanTxQueueStartIndex[kCanNumberOfChannels + 1];
/* ROM CATEGORY 1 END*/
#endif
#  endif

# endif /* ( kCanNumberOfTxObjects > 0 ) */

/* structures for basic and full can receive objects -----------------------*/

# if ( kCanNumberOfRxObjects > 0 )
#  if defined( C_DRV_INTERNAL )
/* ROM CATEGORY 1 START*/
#   if defined( C_SEARCH_HASH ) 
#    if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
V_MEMROM0 extern V_MEMROM1 tCanRxId0 V_MEMROM2       CanRxId0[kCanNumberOfRxFullCANObjects];  
#     if (kCanNumberOfUsedCanRxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanRxId1 V_MEMROM2       CanRxId1[kCanNumberOfRxFullCANObjects];  
#     endif
#     if (kCanNumberOfUsedCanRxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanRxId2 V_MEMROM2       CanRxId2[kCanNumberOfRxFullCANObjects];  
#     endif
#     if (kCanNumberOfUsedCanRxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanRxId3 V_MEMROM2       CanRxId3[kCanNumberOfRxFullCANObjects];  
#     endif
#     if (kCanNumberOfUsedCanRxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanRxId4 V_MEMROM2       CanRxId4[kCanNumberOfRxFullCANObjects];  
#     endif

#     if defined( C_ENABLE_MIXED_ID )
#      if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#      else
V_MEMROM0 extern V_MEMROM1 tCanIdType V_MEMROM2      CanRxIdType[kCanNumberOfRxFullCANObjects];
#      endif
#     endif
#    endif /* C_ENABLE_RX_FULLCAN_OBJECTS */
#   else   /* C_SEARCH_HASH */
V_MEMROM0 extern V_MEMROM1 tCanRxId0 V_MEMROM2       CanRxId0[kCanNumberOfRxObjects];  
#    if (kCanNumberOfUsedCanRxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanRxId1 V_MEMROM2       CanRxId1[kCanNumberOfRxObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanRxId2 V_MEMROM2       CanRxId2[kCanNumberOfRxObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanRxId3 V_MEMROM2       CanRxId3[kCanNumberOfRxObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanRxId4 V_MEMROM2       CanRxId4[kCanNumberOfRxObjects];  
#    endif

#    if defined( C_ENABLE_MIXED_ID )
#     if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#     else
V_MEMROM0 extern V_MEMROM1 tCanIdType V_MEMROM2      CanRxIdType[kCanNumberOfRxObjects];
#     endif
#    endif
#   endif  /* C_SEARCH_HASH */


#   if defined( C_SEARCH_HASH )
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2     CanRxMsgIndirection[kHashSearchListCount + kHashSearchListCountEx + kCanNumberOfRxFullCANObjects];
#   else
#     if defined( C_ENABLE_RX_MSG_INDIRECTION )
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2     CanRxMsgIndirection[kCanNumberOfRxObjects];
#     endif
#   endif

#   if defined( C_SEARCH_HASH )
#    if defined( C_ENABLE_EXTENDED_ID )
#     if( kHashSearchListCountEx > 0)
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2              CanRxHashIdEx[kHashSearchListCountEx];
#     endif /* (kHashSearchListCountEx > 0) */
#     if defined( C_ENABLE_MIXED_ID )
#      if( kHashSearchListCount > 0)
V_MEMROM0 extern V_MEMROM1 vuint16 V_MEMROM2              CanRxHashId[kHashSearchListCount];
#      endif
#     endif
#    else
#     if( kHashSearchListCount > 0)
V_MEMROM0 extern V_MEMROM1 vuint16 V_MEMROM2              CanRxHashId[kHashSearchListCount];
#     endif 
#    endif
#   endif

#   if defined( C_ENABLE_MULTI_ECU_PHYS )
V_MEMROM0 extern V_MEMROM1 tVIdentityMsk V_MEMROM2           CanRxIdentityAssignment[kCanNumberOfRxObjects];
#   endif

#   if defined( C_ENABLE_RX_QUEUE_RANGE )
#    if defined( C_ENABLE_RANGE_0 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange0[kCanNumberOfChannels];
#    endif
#    if defined( C_ENABLE_RANGE_1 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange1[kCanNumberOfChannels];
#    endif
#    if defined( C_ENABLE_RANGE_2 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange2[kCanNumberOfChannels];
#    endif
#    if defined( C_ENABLE_RANGE_3 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange3[kCanNumberOfChannels];
#    endif
#   endif
/* ROM CATEGORY 1 END*/

/* ROM CATEGORY 4 START*/
#   if defined( C_ENABLE_MULTIPLE_BASICCAN ) && \
       defined( C_ENABLE_GEN_CHECK )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanNumberOfBasicCAN[kCanNumberOfChannels];
#   endif
/* ROM CATEGORY 4 END*/

#  endif /* C_DRV_INTERNAL */

/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanRxDataLen[kCanNumberOfRxObjects];
#   if defined( C_ENABLE_DLC_CHECK_MIN_DATALEN )
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanRxMinDataLen[kCanNumberOfRxObjects];
#   endif
V_MEMROM0 extern V_MEMROM1 RxDataPtr            V_MEMROM2 CanRxDataPtr[kCanNumberOfRxObjects];

#   if defined( C_ENABLE_PRECOPY_FCT )
V_MEMROM0 extern V_MEMROM1 ApplPrecopyFct       V_MEMROM2 CanRxApplPrecopyPtr[kCanNumberOfRxObjects];
#   endif
#   if defined( C_ENABLE_INDICATION_FLAG )
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanIndicationOffset[kCanNumberOfRxObjects];
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanIndicationMask[kCanNumberOfRxObjects];
#   endif
#   if defined( C_ENABLE_INDICATION_FCT )
V_MEMROM0 extern V_MEMROM1 ApplIndicationFct    V_MEMROM2 CanRxApplIndicationPtr[kCanNumberOfRxObjects];
#   endif
/* ROM CATEGORY 1 END*/
# endif /* ( kCanNumberOfRxObjects > 0 ) */


/* structure used on initialization CAN chip ------------------------------- */
/* ROM CATEGORY 1 START */
/* ROM CATEGORY 1 END */ 
/* ROM CATEGORY 4 START */
V_DEF_CONST(extern, vuint16, CONST_PBCFG) CanInitBT[];      /* bitTimingReg */
V_DEF_CONST(extern, vuint16, CONST_PBCFG) CanInitMaskH[];
V_DEF_CONST(extern, vuint16, CONST_PBCFG) CanInitCodeH[];
#if defined( C_ENABLE_EXTENDED_ID ) || defined( C_ENABLE_MIXED_ID )
V_DEF_CONST(extern, vuint16, CONST_PBCFG) CanInitMaskL[];
V_DEF_CONST(extern, vuint16, CONST_PBCFG) CanInitCodeL[];
#endif

V_DEF_CONST(extern, vuint32, CONST) CanNodeAdr[];
V_DEF_CONST(extern, vuint8, CONST)  CanNodeNum[];
V_DEF_CONST(extern, vuint8, CONST)  CanInitPortSel[];
V_DEF_CONST(extern, vuint8, CONST)  CanSrcNum[]; 

V_DEF_CONST(extern, tCanIntPendMaskStruct, CONST) CanInterruptPendingMask[kCanNumberOfChannels];
/* ROM CATEGORY 4 END */ 

/* ROM CATEGORY 3 START */
V_DEF_CONST(extern, vuint8, CONST) canPhysPrio[];
/* ROM CATEGORY 3 END */


/***************************************************************************/
/* Callback functions                                                      */
/***************************************************************************/

# if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanAddCanInterruptRestore(CanChannelHandle channel) C_API_3;
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanAddCanInterruptDisable(CanChannelHandle channel) C_API_3;
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_HARDWARE_CHECK ) || \
    defined( C_ENABLE_USER_CHECK )     || \
    defined( C_ENABLE_GEN_CHECK )      || \
    defined( C_ENABLE_INTERNAL_CHECK ) 
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanFatalError(CAN_CHANNEL_CANTYPE_FIRST vuint8 errorNumber) C_API_3;
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_SINGLE_RECEIVE_CHANNEL )
/* these two functions are declared by the tool in case of multichannel */
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanBusOff(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif
# if defined( C_ENABLE_APPLCANPREWAKEUP_FCT )
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanPreWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif
# if defined( C_ENABLE_OVERRUN )
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanOverrun ( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif
# if defined( C_ENABLE_FULLCAN_OVERRUN )
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanFullCanOverrun ( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

/* Range precopy/ generic precopy functions ********************************/
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  if defined( C_ENABLE_RANGE_0 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange0Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RANGE_1 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange1Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RANGE_2 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange2Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RANGE_3 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange3Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
# endif

# if defined( C_ENABLE_COND_RECEIVE_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgCondReceived  ( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_CAN_RAM_CHECK )
#  if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanCorruptMailbox(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle);
/* CODE CATEGORY 4 END*/
#  endif
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanMemCheckFailed(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  if defined( C_ENABLE_RECEIVE_FCT )
/* in case of multiple CAN channels this function is declared by the tool */
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 APPL_CAN_MSGRECEIVED(CanRxInfoStructPtr rxStruct);
/* CODE CATEGORY 1 END*/
#  endif
# endif

/* ##RI1.4 - 2.7: Callbackfunction-DLC-Check */
# if defined( C_ENABLE_DLC_FAILED_FCT )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgDlcFailed( CanRxInfoStructPtr rxStruct ); 
/* CODE CATEGORY 4 END*/ 
# endif  /*C_ENABLE_DLC_FAILED_FCT */

# if defined( C_ENABLE_GENERIC_PRECOPY )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 APPL_CAN_GENERIC_PRECOPY( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_NOT_MATCHED_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgNotMatched( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_TX_CONFIRMATION(CanTxInfoStructPtr txStruct);
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_TX_OBSERVE )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxObjStart( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObject );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxObjConfirmed( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObject );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanInit( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObjectFirstUsed, CanObjectHandle txHwObjectFirstUnused);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_ENABLE_HW_LOOP_TIMER )
/* CODE CATEGORY 2 START*/
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerStart(CAN_CHANNEL_CANTYPE_FIRST vuint8 source);
/* CODE CATEGORY 2 END*/
/* CODE CATEGORY 2 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanTimerLoop (CAN_CHANNEL_CANTYPE_FIRST vuint8 source);
/* CODE CATEGORY 2 END*/
/* CODE CATEGORY 2 START*/
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerEnd  (CAN_CHANNEL_CANTYPE_FIRST vuint8 source);
/* CODE CATEGORY 2 END*/
# endif

# if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_MSGTRANSMITCONF(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_MSGTRANSMITINIT(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_SINGLE_RECEIVE_CHANNEL )
/* in case of multichannel these two functions are declared by the tool */
#  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
/* CODE CATEGORY 3 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_CANCELNOTIFICATION( CanTransmitHandle txHandle ) C_API_3;
/* CODE CATEGORY 3 END*/
#  endif
#  if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
/* CODE CATEGORY 3 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_MSGCANCELNOTIFICATION( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
#  endif
# endif

# if defined( C_ENABLE_RX_QUEUE )
#  if defined( C_ENABLE_APPLCANPRERXQUEUE )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanPreRxQueue( CanRxInfoStructPtr pCanRxInfoStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RXQUEUE_OVERRUN_NOTIFY )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanRxQueueOverrun( void );
/* CODE CATEGORY 4 END*/
#  endif
# endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/* CODE CATEGORY 3 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanChannelDummy( CanChannelHandle channel );
/* CODE CATEGORY 3 END*/
# endif
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) 
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxHandleDummy( CanTransmitHandle txHandle );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRxStructPtrDummy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif
# if defined( C_ENABLE_ONLINE_OFFLINE_CALLBACK_FCT )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_OFFLINE( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_ONLINE( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

# if defined (C_ENABLE_CAN_CAN_INTERRUPT_CONTROL) &&\
     defined (C_ENABLE_OSEK_CAN_INTCTRL)
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 OsCanCanInterruptDisable( CAN_HW_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 OsCanCanInterruptRestore( CAN_HW_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 1 END*/
# endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
#endif

/***************************************************************************/
/* function prototypes                                                     */
/***************************************************************************/

/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInitPowerOn( void );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInit( CAN_CHANNEL_CANTYPE_FIRST CanInitHandle initObject );
/* CODE CATEGORY 4 END*/

# if defined( C_ENABLE_CAN_TRANSMIT )
/* CODE CATEGORY 2 START*/
C_API_1 vuint8 C_API_2 CanTransmit(CanTransmitHandle txHandle) C_API_3;
/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_VARIABLE_DLC )
/* CODE CATEGORY 2 START*/
C_API_1 vuint8 C_API_2 CanTransmitVarDLC( CanTransmitHandle txHandle, vuint8 dlc) C_API_3;
/* CODE CATEGORY 2 END*/
#  endif
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanCancelTransmit( CanTransmitHandle txHandle );
/* CODE CATEGORY 3 END*/
# endif

# if defined( C_ENABLE_MSG_TRANSMIT )
/* CODE CATEGORY 2 START*/

# if defined ( V_ENABLE_USED_GLOBAL_VAR )
/* txMsgStruct is located in far memory */
C_API_1 vuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST V_MEMRAM1_FAR tCanMsgTransmitStruct V_MEMRAM2_FAR V_MEMRAM3_FAR *txMsgStruct );
# else
C_API_1 vuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST tCanMsgTransmitStruct *txMsgStruct);
# endif

/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanCancelMsgTransmit( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
#  endif
# endif

# if defined( C_ENABLE_OFFLINE )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOnline( CAN_CHANNEL_CANTYPE_ONLY );           /* switch CanStatus to On */
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOffline( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_ENABLE_PART_OFFLINE )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOffline( CAN_CHANNEL_CANTYPE_FIRST vuint8 sendGroup);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOnline( CAN_CHANNEL_CANTYPE_FIRST vuint8 invSendGroup);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanGetPartMode( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetStatus( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 3 END*/

/* CanSleep functions */
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanSleep( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanWakeUp( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/

# if defined( C_ENABLE_ECU_SWITCH_PASS )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetActive(  CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPassive( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# else
#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
#   define    CanSetActive( channel )            /* remove calls to setActive/Passive*/
#   define    CanSetPassive( channel )
#  else
#   define    CanSetActive( )                    /* remove calls to setActive/Passive*/
#   define    CanSetPassive( )
#  endif
# endif

# if defined( C_ENABLE_STOP )
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStart( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStop( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

# if defined (C_ENABLE_CAN_CAN_INTERRUPT_CONTROL)
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptDisable( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptRestore( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING ) 
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTask(void);
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_TX_POLLING ) 
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxTask( CAN_CHANNEL_CANTYPE_ONLY ) C_API_3;
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_TX_POLLING ) && \
     defined( C_ENABLE_INDIVIDUAL_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_ERROR_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanErrorTask( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_SLEEP_WAKEUP )
# endif
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxFullCANTask(CAN_CHANNEL_CANTYPE_ONLY) C_API_3;
/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxFullCANObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
/* CODE CATEGORY 2 END*/
#  endif
# endif
#if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANTask(CAN_CHANNEL_CANTYPE_ONLY) C_API_3;
/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
/* CODE CATEGORY 2 END*/
#  endif
# endif


# if defined( C_ENABLE_DYN_TX_OBJECTS )
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanGetDynTxObj(CanTransmitHandle txHandle ) C_API_3;
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8          C_API_2 CanReleaseDynTxObj(CanTransmitHandle txHandle) C_API_3;
/* CODE CATEGORY 3 END*/

#  if defined( C_ENABLE_DYN_TX_ID )
#   if !defined( C_ENABLE_EXTENDED_ID ) ||\
     defined( C_ENABLE_MIXED_ID )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetId(CanTransmitHandle txHandle, vuint16 id) C_API_3;
/* CODE CATEGORY 2 END*/
#   endif
#   if defined( C_ENABLE_EXTENDED_ID )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetExtId(CanTransmitHandle txHandle, vuint16 idExtHi, vuint16 idExtLo) C_API_3;
/* CODE CATEGORY 2 END*/
#   endif
#  endif
#  if defined( C_ENABLE_DYN_TX_DLC )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDlc(CanTransmitHandle txHandle, vuint8 dlc) C_API_3;
/* CODE CATEGORY 2 END*/
#  endif
#  if defined( C_ENABLE_DYN_TX_DATAPTR )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDataPtr(CanTransmitHandle txHandle, void* pData) C_API_3;
/* CODE CATEGORY 2 END*/
#  endif


# endif /* defined( C_ENABLE_DYN_TX_OBJECTS ) */

# if defined( C_ENABLE_TX_MASK_EXT_ID )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_ENABLE_TX_OBSERVE )
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanTxGetActHandle( CanObjectHandle logicalTxHdl ) C_API_3;
/* CODE CATEGORY 3 END*/
# endif

# if defined( C_ENABLE_COND_RECEIVE_FCT )
/* CODE CATEGORY 3 START*/
C_API_1 void     C_API_2 CanSetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
C_API_1 void     C_API_2 CanResetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
# endif

# if defined( C_ENABLE_RX_QUEUE )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanDeleteRxQueue(void);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanHandleRxMsg(void);
/* CODE CATEGORY 2 END*/
# endif

/* Additional API for QNX */



#if defined( C_ENABLE_MEMCOPY_SUPPORT )
/* CODE CATEGORY 1 START */
V_DEF_FUNC_API(V_NONE, void, CODE) CanCopyToCan(CanChipDataPtr dst, V_DEF_P2VAR_PARA(V_NONE, void, AUTOMATIC, APPL_VAR) src, vuint8 len);
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
V_DEF_FUNC_API(V_NONE, void, CODE) CanCopyFromCan(V_DEF_P2VAR_PARA(V_NONE, void, AUTOMATIC, APPL_VAR) dst, CanChipDataPtr src, vuint8 len);
/* CODE CATEGORY 1 END */
#endif
#if defined( C_ENABLE_CAN_RAM_CHECK )
#endif /* C_ENABLE_CAN_RAM_CHECK */

# if defined(C_ENABLE_SUPPORT_REMOTE_FRAME)
/* CODE CATEGORY 2 START */
V_DEF_FUNC_API(V_NONE, vuint8, CODE) Can_TransmitRemote( CAN_CHANNEL_CANTYPE_FIRST vuint32 msgId ) C_API_3;
/* CODE CATEGORY 2 END */
# endif /* defined(C_ENABLE_SUPPORT_REMOTE_FRAME) */


/* Possibility to disable the prototypes of interrupt service routines in the driver header file */
#if !defined (C_DISABLE_ISR_PROTOTYPE)
# if defined( C_LL_INTERRUPT_ISR_ACTIVE ) /* ! pure polling configuration */
# endif
#endif


#endif /* CAN_DEF_H */

/* End of channel */



/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/* Kernbauer Version: 1.09 Konfiguration: TricoreMulticanHll Erzeugungsgangnummer: 17 */

/************   Organi, Version 3.9.0 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.9.0 Vector-Informatik GmbH  ************/
