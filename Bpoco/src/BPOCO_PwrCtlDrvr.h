/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: BPOCO_PwrCtlDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.03.2010
 *  LAST REVISION:      $Date: 2011/02/28 09:28:30MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BPOCO_PwrCtlDrvr.h  $
 *  Revision 1.8 2011/02/28 09:28:30MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.7 2010/11/22 09:08:59MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.6 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.5 2010/08/24 10:18:14MESZ Cotor Vlad RGB (COTORV) 
 *  Update BSW for xCU power control complex device driver
 *  Update due to calculation correction, resolution adjustment
 *  Revision 1.4 2010/07/30 14:38:47MESZ Cotor Vlad RGB (COTORV) 
 *  Update BSW for xCU power control complex device driver due to introduction of BPOCO scheduler
 *  Revision 1.3 2010/07/19 10:04:11MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver due to specification update
 *  Revision 1.2 2010/07/12 09:40:56MESZ Cotor Vlad RGB (COTORV)
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 13:58:08MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bpoco/src/project.pj
 *
 * ***************************************************************************/

#ifndef BPOCO_PWRCTLDRVR_H_
#define BPOCO_PWRCTLDRVR_H_

/* ========== Includes ================================================ */
#include "Adc.h"

#include "BPOCO_PwrCtlDrvr_Cfg.h"       /* module header configuration file */

/* ========== Declare Alternate Control _EXTERNAL_ definition ========= */
#ifdef  MASTER_BPOCO_PWRCTLDRVR_C_  /*! Read from associated modul ?    */
   #define  EXTERNAL_           /*! yes: associated   */
#else
   #define  EXTERNAL_  extern   /*! no: extern modul  */
#endif

/** @defgroup group_bpoco BPOCO
 *  @ingroup group_bpoco
 *
 *  Basic software for xCU power control complex device driver */
/*@{*/

/** @page bpoco BPOCO
 *
 * @section bpoco_intro Introduction
 *  @li This driver shall handle all measurements and control outputs related to xCU power supply and watchdog control.
 *  @li This includes the measurement of the voltage and current of the fused power inputs and outputs, the measurement
 *  of the vehicle (12V) battery voltage, wake up lines and grounds. Also, the enable and watchdog inputs of the power
 *  supply ASIC as well as the fused-power-relay switch is controlled.
 */
/* ========== Enumerations, Typedefinitions =========================== */



/* ========== Global variables  ======================================= */
#define BPOCO_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   extern Adc_ValueGroupType BPOCO_AdcResult0[16];
   extern Adc_ValueGroupType BPOCO_AdcResult1[16];
#define BPOCO_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

#define BPOCO_START_SEC_VAR_8BIT
#include "MemMap.h"
   extern uint8 WdgDirectionFlag;
   extern uint8 SetWdgTiShtdnWasCalled;
#define BPOCO_STOP_SEC_VAR_8BIT
#include "MemMap.h"
/* ========== Global macros =========================================== */



/* ========== Global function prototypes  ============================= */
#define BPOCO_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void BPOCO_RstInit(void);
 *
 * @details This function is called at RESET from operating system's StartUpHook and initialize all needed values.
 * If the initialization values are not specified, the variables will be initialized with 0.
 *
 * @param none
 *
 * @return void
 */
extern void BPOCO_Ini(void);
/*****************************************************************************/
#define BPOCO_STOP_SEC_CODE_INIT
#include "MemMap.h"


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BPOCO_Scheduler_10ms(void)
 *
 * @details The 10ms scheduler function will be called from operating system in 10ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BPOCO_Scheduler_10ms(void);
/*****************************************************************************/
/**
 * @fn void BPOCO_SetPwrSplyEna(boolean PrmFlagEna)
 *
 * @details This interface sets the power supply enable pin to the state PrmFlagEna.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BPOCO_SetPwrSplyEna(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn void BPOCO_SetWdgSig(boolean PrmFlagEna)
 *
 * @details This interface sets the watchdog signal to the state PrmFlagEna.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BPOCO_SetWdgSig(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn void BPOCO_SetRlyPwrEna(boolean PrmFlagEna)
 *
 * @details This interface sets the fused power relay control pin to the state PrmFlagEna.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BPOCO_SetRlyPwrEna(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn void BPOCO_SetFacFilUAdcBat(uint16 PrmFacFil)
 *
 * @details This interface sets the PT1-filter-constant for the battery and ground voltage measurement.
 *
 * @param uint16 PrmFacFil - PT1-filter-constant. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 0.99940875 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BPOCO_SetFacFilUAdcBat(uint16 PrmFacFil);
/*****************************************************************************/
/**
 * @fn void BPOCO_SetFacFilUAdcPwr(uint16 PrmFacFil)
 *
 * @details This interface sets the PT1-filter-constant for the fused power in-/output voltage and current measurement.
 *
 * @param uint16 PrmFacFil - PT1-filter-constant. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 0.99940875 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BPOCO_SetFacFilUAdcPwr(uint16 PrmFacFil);
extern void BPOCO_SetFacFilUAdcAsi06(uint16 PrmFacFil_Asi);
/*****************************************************************************/
/**
 * @fn uint16 BPOCO_GetUBat(void)
 *
 * @details This interface gets the vehicle battery voltage measurement value.
 *
 * @param none
 *
 * @return uint16 PrmU - Vehicle battery voltage. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 39.9999426 \n
 * unit: [V] \n
 */
extern uint16 BPOCO_GetUBat(void);
/*****************************************************************************/
#ifdef BPOCO_WAKE_1
/**
 * @fn uint16 BPOCO_GetUWk1(void)
 *
 * @details This interface gets the voltage measurement value of the wake 1 input.
 *
 * @param none
 *
 * @return uint16 PrmU - Voltage. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 39.9999426 \n
 * unit: [V] \n
 */
extern uint16 BPOCO_GetUWk1(void);
#endif
/*****************************************************************************/
#ifdef BPOCO_WAKE_2
/**
 * @fn uint16 BPOCO_GetUWk2(void)
 *
 * @details This interface gets the voltage measurement value of the wake 2 input.
 *
 * @param none
 *
 * @return uint16 PrmU - Voltage. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 39.9999426 \n
 * unit: [V] \n
 */
extern uint16 BPOCO_GetUWk2(void);
#endif
/*****************************************************************************/
/**
 * @fn sint16 BPOCO_GetUGndDig(void)
 *
 * @details This interface gets the voltage measurement value of digital ground.
 *
 * @param none
 *
 * @return sint16 PrmU - Voltage. \n
 * hex. limits: 8000H : 7FFFH \n
 * phys. limits: -2.5 : 2.49979443 \n
 * unit: [V] \n
 */
extern sint16 BPOCO_GetUGndDig(void);
/*****************************************************************************/
/**
 * @fn sint16 BPOCO_GetUGndPwr(void)
 *
 * @details This interface gets the voltage measurement value of power ground.
 *
 * @param none
 *
 * @return sint16 PrmU - Voltage. \n
 * hex. limits: 8000H : 7FFFH \n
 * phys. limits: -2.5 : 2.49979443 \n
 * unit: [V] \n
 */
extern sint16 BPOCO_GetUGndPwr(void);
/*****************************************************************************/
/**
 * @fn uint16 BPOCO_GetUPwr(uint8 PrmIdx)
 *
 * @details This interface gets the fused power in-/output voltage measurement value of port PrmIdx.
 *
 * @param uint8 PrmIdx - Port index. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmU - Voltage. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 39.9999426 \n
 * unit: [V] \n
 */
extern uint16 BPOCO_GetUPwr(uint8 PrmIdx);
/*****************************************************************************/
/**
 * @fn sint16 BPOCO_GetIPwr(uint8 PrmIdx)
 *
 * @details This interface gets the fused power in-/output current measurement value of port PrmIdx.
 *
 * @param uint8 PrmIdx - Port index. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return void * @return sint16 PrmU - Voltage. \n
 * hex. limits: 8000H : 7FFFH \n
 * phys. limits: -40 : 39.9987793 \n
 * unit: [A] \n
 */
extern sint16 BPOCO_GetIPwr(uint8 PrmIdx);
/*****************************************************************************/
/**
 * @fn void BPOCO_ShtdnEcu(void)
 *
 * @details This interface initiates the xCU shutdown procedure.
 *
 * @param none
 *
 * @return void
 */
extern void BPOCO_ShtdnEcu(void);
/*****************************************************************************/
/**
 * @fn void BPOCO_SetWdgTiShtdn(uint16 PrmTiLo, uint16 PrmTiHi)
 *
 * @details This interface sets the low and high times of the watchdog signal during xCU shutdown procedure.
 *
 * @param uint16 PrmTiLo - Watchdog signal low time. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [ms] \n
 *
 * @param uint16 PrmTiHi - Watchdog signal high time. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [ms] \n
 *
 * @return void
 */
extern void BPOCO_SetWdgTiShtdn(uint16 PrmTiLo, uint16 PrmTiHi);
/*****************************************************************************/
/**
 * @fn void WgdTrig_GptNotification(void)
 *
 * @details This interface is the Gpt Notification for watchdog triggering in shutdown procedure.
 *
 * @param none
 *
 * @return void
 */
extern void WgdTrig_GptNotification(void);
/*****************************************************************************/
/**
 * @fn void ShtdnEcuWait_GptNotification(void)
 *
 * @details This interface is the Gpt Notification for wait time after shutdown procedure 
 * to execute a microcontroller reset.
 *
 * @param none
 *
 * @return void
 */
 extern void ShtdnEcuWait_GptNotification(void);
 /*****************************************************************************/
 /**
  * @fn uint16 BPOCO_GetUASI06(void)
  *
  * @details This interface gets the  defrost  rely voltage measurement value.
  *
  * @param none
  *
  * @return uint16 PrmU - ASI06 defrost rely input . \n
  * hex. limits: 0H : FFFFH \n
  * phys. limits: 0 : 39.9999426 \n
  * unit: [V] \n
  */
 extern uint16 BPOCO_GetUASI06(void);
 /*****************************************************************************/

#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"

/* ========== End of Global variables (End of header function) ======== */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BPOCO_PWRCTLDRVR_H_ */



