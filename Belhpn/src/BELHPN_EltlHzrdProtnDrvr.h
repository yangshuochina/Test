/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BELHPN_EltlHzrdProtnDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   28.10.2010
 *  LAST REVISION:      $Date: 2010/12/01 09:04:27MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BELHPN_EltlHzrdProtnDrvr.h  $
 *  Revision 1.2 2010/12/01 09:04:27MEZ Cotor Vlad RGB (COTORV) 
 *  Implement and integrate complex device driver for electrical hazard protection
 *
 * ***************************************************************************/

#ifndef BELHPN_ELTLHZRDPROTNDRVR_H_
#define BELHPN_ELTLHZRDPROTNDRVR_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */

#include "Dio.h"
#include "Dio_Cfg.h"
#include "Spi.h"
#include "Adc.h"

#include "BSW_DrvrAux.h"
#include "BELHPN_EltlHzrdProtnDrvr_Cfg.h"

#include "Prj_MainCfg.h"

/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BELHPN_ELTLHZRDPROTNDRVR_C_  /* Read from associated module ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: external module  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_belhpn BELHPN
 *  @ingroup group_belhpn
 *
 *  Basic software for electrical hazard protection complex device driver */
/*@{*/

/** @page belhpn BELHPN
 *
 * @section belhpn_intro Introduction
 *  @li This driver shall handle the voltage measurement and channel switching for isolation detection.
 *  @li The general task of this driver is to switch the measurement input of the AS8510 to the positive
 *  or negative high voltage. On request, the driver shall activate a parallel resistor to the measured
 *  isolation resistance. Voltage measurement and control of the AS8510 is also done by this driver.
 */

/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */
#define BELHPN_START_SEC_VAR_8BIT
#include "MemMap.h"
   extern uint8 AS8510_0_Gain;
   extern uint8 AS8510_0_Gain_tx;
   extern uint8 AS8510_0_Gain_rx;
   extern uint8 Chip_AS8510_0_address_j3_data;
   extern uint8 Chip_AS8510_0_address_j4_data;
   extern uint8 BELHPN_time_stamp;
#define BELHPN_STOP_SEC_VAR_8BIT
#include "MemMap.h"


#define BELHPN_START_SEC_VAR_16BIT
#include "MemMap.h"
   extern uint16 BELHPN_AnChnSelnReg_data;
   extern uint16 BELHPN_CurSrcSetReg_data;
   extern uint16 BELHPN_AS8510_UAdc;
#define BELHPN_STOP_SEC_VAR_16BIT
#include "MemMap.h"


/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */
#define BELHPN_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void BELHPNC_Ini(void)
 *
 * @details This function is called at RESET from operating system's StartUpHook and initialize all needed values.
 * If the initialization values are not specified, the variables will be initialized with 0.
 *
 * @param none
 *
 * @return void
 */
extern void BELHPN_Ini(void);
/*****************************************************************************/
#define BELHPN_STOP_SEC_CODE_INIT
#include "MemMap.h"


#define BELHPN_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BELHPN_Scheduler_1ms(void)
 *
 * @details The 1ms scheduler function will be called from operating system in 1ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BELHPN_Scheduler_1ms(void);
/*****************************************************************************/
#define BELHPN_STOP_SEC_CODE_1MS
#include "MemMap.h"



#define BELHPN_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BELHPN_SetChnIsln (uint8 PrmChn, boolean PrmROn)
 *
 * @details PrmChn selects if no voltage (PrmChn=0), HV+ (PrmChn=1) or HV- (PrmChn=2) is selected. PrmROn switch the measurement resistance on (PrmROn=1) and off (PrmROn=0).
 *
 * @param uint8 PrmChn - PrmChn selects if no voltage (PrmChn=0), HV+ (PrmChn=1) or HV- (PrmChn=2) is selected. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @param boolean PrmROn - PrmROn switches the measurement resistance on (PrmROn=1) and off (PrmROn=0). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BELHPN_SetChnIsln (uint8 PrmChn, boolean PrmROn);
/******************************************************************************/
/**
 * @fn sint16 BELHPN_GetUMeasIsln(void)
 *
 * @details This interface gets the measured voltage for isolation detection.
 *
 * @param none
 *
 * @return sint16 PrmU - This Parameter returns the measured voltage. \n
 * hex. limits: 8000H : 7FFFH \n
 * phys. limits: -655.36 : 655.34 \n
 * unit: [V] \n
 */
extern sint16 BELHPN_GetUMeasIsln(void);
/******************************************************************************/
/**
 * @fn void BELHPN_SetGainADC(uint8 PrmGain)
 *
 * @details This interface sets the programmable gain of the voltage measurement of the AS8510.
 *
 * @param uint8 PrmGain - Programmable gain of the voltage measurement (allowed values are 1, 5, 25, 40 and 100). \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BELHPN_SetGainADC(uint8 PrmGain);
/******************************************************************************/
#define BELHPN_STOP_SEC_CODE_10MS
#include "MemMap.h"


/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BELHPN_ELTLHZRDPROTNDRVR_H_ */
