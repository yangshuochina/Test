/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BSW_DrvrAux.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   13.04.2010
 *  LAST REVISION:      $Date: 2010/12/08 15:43:51MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BSW_DrvrAux.h  $
 *  Revision 1.9 2010/12/08 15:43:51MEZ Cotor Vlad RGB (COTORV) 
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.8 2010/12/01 16:32:20MEZ Cotor Vlad RGB (COTORV) 
 *  Implement and integrate complex device driver for electrical hazard protection
 *  Revision 1.7 2010/11/22 07:53:13MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.6 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.5 2010/10/14 18:06:19MESZ Cotor Vlad RGB (COTORV) 
 *  Update complex device driver for real time clock due to changed SPI configuration.
 *  Revision 1.4 2010/09/24 18:06:40MESZ Cotor Vlad RGB (COTORV) 
 *  Reorganize the SPI Configuration switching to macros
 *  Revision 1.3 2010/09/20 10:12:55MESZ Cotor Vlad RGB (COTORV)
 *  LOG files clean up through SW correction
 *  Revision 1.2 2010/08/27 14:21:15MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.1 2010/08/25 16:44:43MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Common/src/project.pj
 *  Revision 1.3 2010/08/25 14:41:15MESZ Cotor Vlad RGB (COTORV)
 *  Implement and integrate complex device driver for cooling fan control
 *  Revision 1.2 2010/07/14 09:15:48MESZ Cotor Vlad RGB (COTORV)
 *  Update auxiliary SW functions and structure, include structure and clean the log files
 *  Revision 1.1 2010/05/20 13:55:33MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Common/src/project.pj
 *
 * ***************************************************************************/

#ifndef BSW_DRVRAUX_H_
#define BSW_DRVRAUX_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */

#include "Dio.h"
#include "Spi.h"

#include "BSW_DrvrAux_Cfg.h"

#include "Prj_MainCfg.h"

/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BSW_DRVRAUX_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_bsw_drvaux BSW_DrvAux
 *  @ingroup group_bsw_drvaux
 *
 *  Driver auxiliary functions */
/*@{*/

/** @page bsw_drvaux BSW_DrvAux
 *
 * @section bsw_drvaux_intro Introduction
 *  @li This module contains auxiliary functions use from other complex device drivers.
 *  @li Content:
 *    @li Calculation of a simple input PWM signal
 *    @li Calculation of PT1 filter
 *    @li Conversion function for electrical error status of a side driver
 */
/* ========== Enumerations, Typedefinitions ================================ */
typedef struct InPwm_
{
   uint16   InDuty;
   uint16   InPerd;
   uint16   counter_time;
   uint16   counter_high;
   boolean  status_pin_old;
   uint16   PwmInPin;
} Struct_InPwm;

typedef struct PT1Fil_
{
   uint16   factor;
   uint16   v_ADC_meas;
   uint16   v_ADC_out_old;
   uint16   v_ADC_out;
} Struct_PT1Fil;

/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */
#define BSWAUX_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn InPwmClcd(Struct_InPwm *pwm_data)
 *
 * @details This function calculates a simple PWM period and duty cycle of an input signal.
 *
 * @param Struct_InPwm *pwm_dat - Structure which contains the values for calculation of input PWM for a signal. \n
 *
 * Structure elements: \n
 *
 * uint16 InDuty - Input PWM duty cycle. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 99.9998565 \n
 * unit: [%] \n
 *
 * uint16 InPerd - Input PWM period. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 655.35 \n
 * unit: [ms] \n
 *
 * uint16 counter_time - total counter used as divisor for calculation of the PWM period. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [-] \n
 *
 * uint16 counter_high - Pin's high level counter. It's incremented when the pin's level is high
 * and is used as dividend for calculation of the PWM period. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [-] \n
 *
 * boolean status_pin_old - status of the pin's old level (0 - low, 1 - high). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * uint16 PwmInPin - Input PWM pin. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void InPwmClcd(Struct_InPwm *pwm_data);
/*****************************************************************************/
/**
 * @fn uint8 returnCfgGain(uint8 CfgGain)
 *
 * @details This function returns the converted gain settings of PGA control register of AS8515.
 *
 * @param uint8 CfgGain - Programmable gain of the voltage measurement (0: Gain = 5; 1: Gain = 25; 2: Gain = 40; 3: Gain = 100). \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n *
 *
 * @return uint8 temp_CfgGain - converted gain \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 */
extern uint8 returnCfgGain(uint8 CfgGain);
/*****************************************************************************/
#define BSWAUX_STOP_SEC_CODE_1MS
#include "MemMap.h"


#define BSWAUX_START_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/
/**
 * @fn void ClcdPT1Fil(Struct_PT1Fil *PT1Fil_data)
 *
 * @details This function calculate PT1 Filter
 *
 * @param Struct_PT1Fil *PT1Fil_data - Structure which contains the values for the calculation of the PT1 filter. \n
 *
 * Structure elements: \n
 *
 * factor - PT1-filter-constant \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 0.99940875 \n
 * unit: [-] \n
 *
 * v_ADC_meas - ADC measured voltage \n
 * hex. limits: 0000H : 0FFFH \n
 * phys. limits: 0 : 4095 \n
 * unit: [V]
 *
 * v_ADC_out_old - old filtered voltage \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 4.99966515 \n
 * unit: [V]
 *
 * v_ADC_out - filtered voltage \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 4.99966515 \n
 * unit: [V]
 *
 * @return void
 */
extern void ClcdPT1Fil(Struct_PT1Fil *PT1Fil_data);
/*****************************************************************************/
/**
 * @fn uint8 returnPrmErrSym(uint8 CfgPrmErrSym)
 *
 * @details This function returns the converted electrical error status of a side driver used to control a contactor.
 *
 * @param uint8 CfgPrmErrSym - Error status (0: no error; 1: short circuit to ground; 2: open load; 3: over load / over temperature). \n
 * hex. limits: 0H : 3H \n
 * phys. limits: 0 : 3 \n
 * unit: [-] \n
 *
 * @return void
 */
extern uint8 returnPrmErrSym(uint8 CfgPrmErrSym);
/*****************************************************************************/
#define BSWAUX_STOP_SEC_CODE_10MS
#include "MemMap.h"



/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BSW_DRVRAUX_H_ */
