/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BMCHIL_MdulCtlrIfHwIntlcDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   13.04.2010
 *  LAST REVISION:      $Date: 2011/01/31 16:14:58MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BMCHIL_MdulCtlrIfHwIntlcDrvr.h  $
 *  Revision 1.8 2011/01/31 16:14:58MEZ Cotor Vlad RGB (COTORV) 
 *  - BMCHIL SW update with interface for HV interlock PWM output
 *  Revision 1.7 2010/12/01 15:39:47MEZ Cotor Vlad RGB (COTORV) 
 *  Implement and integrate complex device driver for electrical hazard protection
 *  Revision 1.6 2010/11/25 09:35:37MEZ Cotor Vlad RGB (COTORV) 
 *  Update BMCHIL: New trigger concept for MCs and current senso
 *  Revision 1.5 2010/11/22 07:22:53MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.4 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.3 2010/09/20 10:14:26MESZ Cotor Vlad RGB (COTORV) 
 *  LOG files clean up through SW correction
 *  Revision 1.2 2010/08/27 14:26:26MESZ Cotor Vlad RGB (COTORV) 
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.1 2010/08/25 16:41:04MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bmchil/src/project.pj
 *  Revision 1.3 2010/07/30 15:37:51MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for module controller interface and hardware interlock complex device driver due to BMCHIL scheduler and Dio_ReadChannel
 *  Revision 1.2 2010/07/19 10:53:34MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for controller interface and hardware interlock complex device driver due to specification update
 *  Revision 1.1 2010/05/20 13:59:51MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bmchil/src/project.pj
 *
 * ***************************************************************************/

#ifndef BMCHIL_MDULCTLRIFHWINTLCDRVR_H_
#define BMCHIL_MDULCTLRIFHWINTLCDRVR_H_

/* ========== Includes ================================================ */
#include "Adc.h"

#include "BMCHIL_MdulCtlrIfHwIntlcDrvr_Cfg.h"

/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BMCHIL_MDULCTLRIFHWINTLCDRVR_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_bmchil BMCHIL
 *  @ingroup group_bmchil
 *
 *  Basic software for module controller interface and hardware interlock complex device driver */
/*@{*/

/** @page bmchil BMCHIL
 *
 * @section bmchil_intro Introduction
 *  @li This driver handles the electrical interface between the BCU and the module controllers (MC),
 *  which are controlling the individual high voltage battery modules. Further, this driver generates
 *  and reads all signals for the MC- and vehicle-interlock-mechanism.
 *  @li In particular, this includes enabling and disabling of the MC-power-supply as well as measuring
 *  of the MC-power-supply voltage and current, generating the digital signals for the MC-fault-line and
 *  the ID-request-line and reading the digital signals for MC-fault and the ID-request.
 *  In addition, digital signals to detect a hardware interlock on the vehicle side are read.
 */
/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */
#define BMCHIL_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   extern Adc_ValueGroupType BMCHIL_AdcResult1[16];
#define BMCHIL_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */
#define BMCHIL_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void BMCHIL_Ini(void);
 *
 * @details This function is called at RESET from operating system's StartUpHook and initialize all needed values.
 * If the initialization values are not specified, the variables will be initialized with 0.
 *
 * @param none
 *
 * @return void
 */
extern void BMCHIL_Ini(void);
/*****************************************************************************/
#define BMCHIL_STOP_SEC_CODE_INIT
#include "MemMap.h"

#define BMCHIL_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BMCHIL_Scheduler_1ms(void)
 *
 * @details The 1ms scheduler function will be called from operating system in 1ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BMCHIL_Scheduler_1ms(void);
/*****************************************************************************/
#define BMCHIL_STOP_SEC_CODE_1MS
#include "MemMap.h"

#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BMCHIL_Scheduler_10ms(void)
 *
 * @details The 10ms scheduler function will be called from operating system in 10ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BMCHIL_Scheduler_10ms(void);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetMdulCtlrPwrSplyEna(boolean PrmFlagEna)
 *
 * @details This interface sets the module controller power supply enable pin to the state PrmFlagEna.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BMCHIL_SetMdulCtlrPwrSplyEna(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetFacFilPwrSply(uint16 PrmFacFil_UI)
 *
 * @details This interface sets the PT1-filter-constant for the module controller power supply voltage and current measurement.
 *
 * @param uint16 PrmFacFil_UI - PT1-filter-constant. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 0.99940875 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BMCHIL_SetFacFilPwrSply(uint16 PrmFacFil_UI);
/*****************************************************************************/
/**
 * @fn uint16 BMCHIL_GetUMdulCtlrPwrSply(void)
 *
 * @details This interface gets the module controller power supply voltage measurement value.
 *
 * @param none
 *
 * @return uint16 PrmU - Module controller power supply voltage.
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 39.9999426 \n
 * unit: [V] \n
 */
extern uint16 BMCHIL_GetUMdulCtlrPwrSply(void);
/*****************************************************************************/
/**
 * @fn uint16 BMCHIL_GetIMdulCtlrPwrSply(void)
 *
 * @details This interface gets the module controller power supply current measurement value.
 *
 * @param none
 *
 * @return uint16 PrmI - Module controller power supply current.
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 39.9999426 \n
 * unit: [A] \n
 */
extern uint16 BMCHIL_GetIMdulCtlrPwrSply(void);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetPwmHiVIntlc(uint16 PrmTiPerd, uint16 PrmDutyCyc)
 *
 * @details Detailed description
 *
 * @param uint16 PrmTiPerd - Period time of the PWM signal. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 655.35 \n
 * unit: [ms] \n
 *
 * @param uint16 PrmDutyCyc - Duty cycle of the PWM signal. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 99.9998565 \n
 * unit: [%] \n
 *
 * @return void
 */
extern void BMCHIL_SetPwmHiVIntlc(uint16 PrmTiPerd, uint16 PrmDutyCyc);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetWdgOutSig(boolean PrmFlagEna)
 *
 * @details This interface sets the module controller watchdog signal to high or low state.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BMCHIL_SetWdgOutSig(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetCurSnsrTrig(boolean PrmFlagEna)
 *
 * @details This interface sets the current sensor trigger signal to high or low state.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BMCHIL_SetCurSnsrTrig(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetMdulCtlrIdReq(boolean PrmFlagEna)
 *
 * @details This interface sets the module controller ID request line to the state PrmFlagEna.
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BMCHIL_SetMdulCtlrIdReq(boolean PrmFlagEna);
/*****************************************************************************/
/**
 * @fn uint8 BMCHIL_GetWdgHwSts(uint8 PrmIdx)
 *
 * @details This interface gets the watchdog status flag of the watchdog PrmIdx.
 *
 * @param uint8 PrmIdx - Watchdog index 0...BMCHIL_CfgNoWdgHw. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint8 PrmFlagSts - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 */
extern uint8 BMCHIL_GetWdgHwSts(uint8 PrmIdx);
/*****************************************************************************/
/**
 * @fn uint16 BMCHIL_GetPwmTiPerd(uint8 PrmIdx)
 *
 * @details This interface gets the period time of the PWM signal PrmIdx.
 *
 * @param uint8 PrmIdx - PWM index 0...BMCHIL_CfgNoMeasPwm. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmTiPerd - Period time of the PWM signal.
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 655.35 \n
 * unit: [ms] \n
 */
extern uint16 BMCHIL_GetPwmTiPerd(uint8 PrmIdx);
/*****************************************************************************/
/**
 * @fn uint16 BMCHIL_GetPwmDutyCyc(uint8 PrmIdx)
 *
 * @details This interface gets the duty cycle of the PWM signal PrmIdx.
 *
 * @param uint8 PrmIdx - PWM index 0...BMCHIL_CfgNoMeasPwm. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmDutyCyc - Duty cycle of the PWM signal. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 99.9998565 \n
 * unit: [%] \n
 */
extern uint16 BMCHIL_GetPwmDutyCyc(uint8 PrmIdx);
/*****************************************************************************/
/**
 * @fn uint8 BMCHIL_GetDigInpSts(uint8 PrmIdx)
 *
 * @details This interface gets the status of the digital input PrmIdx.
 *
 * @param uint8 PrmIdx - Digital input index 0...BMCHIL_CfgNoMeasDigInp. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint8 PrmFlagSts - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 */
extern uint8 BMCHIL_GetDigInpSts(uint8 PrmIdx);
/*****************************************************************************/
/**
 * @fn void BMCHIL_SetDigInpDiagSig(boolean PrmFlagEna)
 *
 * @details This interface sets the diagnostic signal of the digital input conditioning chip to the state PrmFlagEna. \n
 *
 * @param boolean PrmFlagEna - State of the pin. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BMCHIL_SetDigInpDiagSig(boolean PrmFlagEna);
/*****************************************************************************/
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"


/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BMCHIL_MDULCTLRIFHWINTLCDRVR_H_ */
