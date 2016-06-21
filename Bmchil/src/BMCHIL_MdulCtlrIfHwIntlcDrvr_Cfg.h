/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BMCHIL_MdulCtlrIfHwIntlcDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   13.04.2010
 *  LAST REVISION:      $Date: 2011/02/14 13:01:16MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BMCHIL_MdulCtlrIfHwIntlcDrvr_Cfg.h  $
 *  Revision 1.8 2011/02/14 13:01:16MEZ Cotor Vlad RGB (COTORV) 
 *  - Implement OSEK changes due to XCP update - after review
 *  Revision 1.7 2011/02/02 07:27:49MEZ Cotor Vlad RGB (COTORV)
 *  - BMCHIL SW update with interface for HV interlock PWM output
 *  Revision 1.6 2011/01/31 14:20:11MEZ Cotor Vlad RGB (COTORV)
 *  - Change output pin for BCU watchdog out signal from P1.03 to P1.05
 *  - Also change pin configuration of P1.05 to PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT.
 *  Revision 1.5 2010/11/25 09:36:47MEZ Cotor Vlad RGB (COTORV)
 *  Update BMCHIL: New trigger concept for MCs and current senso
 *  Revision 1.4 2010/11/22 07:24:46MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.3 2010/09/23 13:38:16MESZ Cotor Vlad RGB (COTORV)
 *  Correct offset of MC power supply current measurement. Insert configurable offset.
 *  Revision 1.2 2010/09/08 10:15:58MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for complex device driver BMCHIL
 *  - correction of BMCHIL_GetWdgHwSts function.
 *  Revision 1.1 2010/08/25 16:41:07MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bmchil/src/project.pj
 *  Revision 1.2 2010/07/19 09:57:54MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for controller interface and hardware interlock complex device driver due to specification update
 *  Revision 1.1 2010/05/20 13:59:52MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bmchil/src/project.pj
 *
 * ***************************************************************************/

#ifndef BMCHIL_MDULCTLRIFHWINTLCDRVR_CFG_H_
#define BMCHIL_MDULCTLRIFHWINTLCDRVR_CFG_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */

#include "OsTaskRuntime.h"

/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BMCHIL_MDULCTLRIFHWINTLCDRVR_CFG_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_bmchil_config BMCHIL Configuration
 *  @ingroup group_bmchil
 *
 * This module contains the configuration of the BMCHIL - Basic software for module controller interface and hardware interlock complex device driver.
 */
/*@{*/
/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */

/* ADC configuration */
#define BMCHIL_UAnDigCnvrMeasd_PwrSply            BMCHIL_AdcResult1[10]    /**< AN 26 */
#define BMCHIL_IAnDigCnvrMeasd_PwrSply            BMCHIL_AdcResult1[12]    /**< AN 28 */


/*====================================================================*/
/**
 * ADC measurement recurrence for module controller power supply voltage and current. \n
 * hex. limits:    0H : FFH \n
 * phys. limits:     0 : 255 \n
 * unit        : [ms] */
#define BMCHIL_CfgMeasPerdMdulPwrSply           OS_TASK_10MS

/*====================================================================*/
/**
 * Factor between ADC voltage and measured module controller power supply voltage.\n
 * hex. limits:    0H : FFFFH               \n
 * phys. limits:    0 : 63.99902344         \n
 * unit        : [-] */
#define BMCHIL_CfgFacUMdulPwrSply               0x0800u   /**< Decimal value: 2 */

/*====================================================================*/
/**
 * Conversion factor between ADC voltage and measured module controller power supply current. \n
 * hex. limits:    0H : FFFFH \n
 * phys. limits:    0 : 63.99902344 \n
 * unit        : [A/V] */
#define BMCHIL_CfgFacCnvnIMdulPwrSply           0x0D55u   /**< Decimal value: 3.333007813 */

/*====================================================================*/
/**
 * Flag to enable PWM measurement. \n
 * hex. limits:    0H : 1H \n
 * phys. limits:    0 : 1 \n
 * unit        : [-] */
#define BMCHIL_CfgMeasPwmEna                    1u

/*====================================================================*/
/**
 * Number of measured PWM signals. \n
 * hex. limits:    0H : FFH \n
 * phys. limits:    0 : 255 \n
 * unit        : [-] */
#define BMCHIL_CfgNoMeasPwm                     3u

/*====================================================================*/
/**
 * Flag to enable measurement of digital inputs. \n
 * hex. limits:    0H : 1H \n
 * phys. limits:    0 : 1 \n
 * unit        : [-] */
#define BMCHIL_CfgMeasDigInpEna                 1u

/*====================================================================*/
/**
 * Number of measured digital input signals. \n
 * hex. limits:    0H : FFH \n
 * phys. limits:    0 : 255 \n
 * unit        : [-] */
#define BMCHIL_CfgNoMeasDigInp                  4u

/*====================================================================*/
/**
 * ADC voltage offset for module controller power supply current measurement. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits:    0 : 4.99966515 \n
 * unit        : [V] */
#define BMCHIL_CfgUOfsIMdul                     0x4CCEu   /**< Decimal value: 1.50001398 V */
/*====================================================================*/
/* === Port configuration === */

/* Digital channels */
#define BMCHIL_Dig_CH_0
#ifdef BMCHIL_Dig_CH_0
#define BMCHIL_DIO_PIN_Dig_CH_0                 DIO_CHANNEL_ID(DIO_PORT_0, 8)    /**< PIN_149 - P0.08 - CHANNEL 0 */
#endif

#define BMCHIL_Dig_CH_1
#ifdef BMCHIL_Dig_CH_1
#define BMCHIL_DIO_PIN_Dig_CH_1                 DIO_CHANNEL_ID(DIO_PORT_0, 9)    /**< PIN_150 - P0.09 - CHANNEL 1 */
#endif

#define BMCHIL_Dig_CH_2
#ifdef BMCHIL_Dig_CH_2
#define BMCHIL_DIO_PIN_Dig_CH_2                 DIO_CHANNEL_ID(DIO_PORT_0,10)    /**< PIN_151 - P0.10 - CHANNEL 2 */
#endif

#define BMCHIL_Dig_CH_3
#ifdef BMCHIL_Dig_CH_3
#define BMCHIL_DIO_PIN_Dig_CH_3                 DIO_CHANNEL_ID(DIO_PORT_0,11)    /**< PIN_152 - P0.11 - CHANNEL 3 */
#endif

//#define BMCHIL_Dig_CH_4
#ifdef BMCHIL_Dig_CH_4
#define BMCHIL_DIO_PIN_Dig_CH_4                 DIO_CHANNEL_ID(DIO_PORT_0, 0)    /**< PIN_145 - P0.00 - CHANNEL 4 */
#endif

//#define BMCHIL_Dig_CH_5
#ifdef BMCHIL_Dig_CH_5
#define BMCHIL_DIO_PIN_Dig_CH_5                 DIO_CHANNEL_ID(DIO_PORT_1, 1)    /**< PIN_119 - P1.01 - CHANNEL 5 */
#endif

//#define BMCHIL_Dig_CH_6
#ifdef BMCHIL_Dig_CH_6
#define BMCHIL_DIO_PIN_Dig_CH_6                 DIO_CHANNEL_ID(DIO_PORT_1, 2)    /**< PIN_093 - P1.02 - CHANNEL 6 */
#endif


/* Input pwm channels */
#define BMCHIL_Pwm_CH_0
#ifdef BMCHIL_Pwm_CH_0
#define BMCHIL_DIO_PIN_Pwm_CH_0                 DIO_CHANNEL_ID(DIO_PORT_0, 0)    /**< PIN_145 - P0.00 - CHANNEL 0 */
#define BMCHIL_DIO_PIN_HwIntlc_0                DIO_CHANNEL_ID(DIO_PORT_5, 7)    /**< PIN_008 - P5.07 - Hardware Interlock 0 */
#endif

#define BMCHIL_Pwm_CH_1
#ifdef BMCHIL_Pwm_CH_1
#define BMCHIL_DIO_PIN_Pwm_CH_1                 DIO_CHANNEL_ID(DIO_PORT_1, 1)    /**< PIN_119 - P1.01 - CHANNEL 1 */
#define BMCHIL_DIO_PIN_HwIntlc_1                DIO_CHANNEL_ID(DIO_PORT_5, 8)    /**< PIN_013 - P5.08 - Hardware Interlock 1 */
#endif

#define BMCHIL_Pwm_CH_2
#ifdef BMCHIL_Pwm_CH_2
#define BMCHIL_DIO_PIN_Pwm_CH_2                 DIO_CHANNEL_ID(DIO_PORT_1, 2)    /**< PIN_093 - P1.02 - CHANNEL 2 */
#define BMCHIL_DIO_PIN_HwIntlc_2                DIO_CHANNEL_ID(DIO_PORT_5, 9)    /**< PIN_014 - P5.09 - Hardware Interlock 2 */
#endif

//#define BMCHIL_Pwm_CH_3
#ifdef BMCHIL_Pwm_CH_3
#define BMCHIL_DIO_PIN_Pwm_CH_3                 DIO_CHANNEL_ID(DIO_PORT_0, 8)    /**< PIN_149 - P0.08 - CHANNEL 3 */
#endif

//#define BMCHIL_Pwm_CH_4
#ifdef BMCHIL_Pwm_CH_4
#define BMCHIL_DIO_PIN_Pwm_CH_4                 DIO_CHANNEL_ID(DIO_PORT_0, 9)    /**< PIN_150 - P0.09 - CHANNEL 4 */
#endif

//#define BMCHIL_Pwm_CH_5
#ifdef BMCHIL_Pwm_CH_5
#define BMCHIL_DIO_PIN_Pwm_CH_5                 DIO_CHANNEL_ID(DIO_PORT_0,10)    /**< PIN_151 - P0.10 - CHANNEL 5 */
#endif

//#define BMCHIL_Pwm_CH_6
#ifdef BMCHIL_Pwm_CH_6
#define BMCHIL_DIO_PIN_Pwm_CH_6                 DIO_CHANNEL_ID(DIO_PORT_0,11)    /**< PIN_152 - P0.11 - CHANNEL 6 */
#endif



/* Module controller power supply pin */
#define DIO_PIN_BMCHIL_SetMdulCtlrPwrSplyEna    DIO_CHANNEL_ID(DIO_PORT_0, 7)    /**< PIN_174 - P0.07 */
/* Module controller ID request line */
#define DIO_PIN_BMCHIL_SetMdulCtlrIdReqLine     DIO_CHANNEL_ID(DIO_PORT_0,12)    /**< PIN_168 - P0.12 */
/* Sensor diagnostic signal */
#define DIO_PIN_BMCHIL_SnsrDiagSig              DIO_CHANNEL_ID(DIO_PORT_0, 2)    /**< PIN_147 - P0.02 */
/* Module controller watchdog signal */
#define DIO_PIN_BMCHIL_SetWdgOutSig             DIO_CHANNEL_ID(DIO_PORT_1, 5)    /**< PIN_108 - P1.05 */
/* Current sensor trigger signal */
#define DIO_PIN_BMCHIL_SetCurSnsrTrig           DIO_CHANNEL_ID(DIO_PORT_1, 4)    /**< PIN_107 - P1.04 */
/* Hardware interlock watchdog signal */
#define BMCHIL_CfgPwmCh_SetPwmHiVIntlc          2                                /**< OUT19 - P01.03  - PWM Channel #2 - see Pwm_PBcfg.c */
/* Conversion factor for PWM duty and frequency */
#define BMCHIL_CfgPwmCnvnFac_SetPwmHiVIntlc     (PwmTicks / 10000)

/* ========== Global function prototypes =================================== */



/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BMCHIL_MDULCTLRIFHWINTLCDRVR_CFG_H_ */
