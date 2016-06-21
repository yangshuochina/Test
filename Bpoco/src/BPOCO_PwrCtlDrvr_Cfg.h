/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: BPOCO_PwrCtlDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.03.2010
 *  LAST REVISION:      $Date: 2011/02/28 09:19:17MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BPOCO_PwrCtlDrvr_Cfg.h  $
 *  Revision 1.7 2011/02/28 09:19:17MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.6 2011/02/14 13:01:25MEZ Cotor Vlad RGB (COTORV) 
 *  - Implement OSEK changes due to XCP update - after review
 *  Revision 1.5 2010/11/23 10:42:56MEZ Cotor Vlad RGB (COTORV)
 *  Enable use of third fused power input measurement (AN18/AN15). Configure this function to enable measurement of all 3 fused power inputs.
 *  Revision 1.4 2010/11/22 09:09:39MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.3 2010/08/24 10:18:38MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver
 *  Update due to calculation correction, resolution adjustment
 *  Revision 1.2 2010/07/19 10:16:46MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver due to specification update
 *  Revision 1.1 2010/05/20 13:58:08MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bpoco/src/project.pj
 *
 * ***************************************************************************/

#ifndef BPOCO_PWRCTLDRVR_CFG_H_
#define BPOCO_PWRCTLDRVR_CFG_H_

/* ========== Includes ================================================ */
#include "Std_Types.h"     /*!< Contains the basic data types. */

#include "OsTaskRuntime.h"


/* ========== Declare Alternate Control _EXTERNAL_ definition ========= */
#ifdef  MASTER_BPOCO_PWRCTLDRVR_CFG_C_  /*! Read from associated modul ?    */
   #define  EXTERNAL_           /*! yes: associated   */
#else
   #define  EXTERNAL_  extern   /*! no: extern modul  */
#endif

/** @defgroup group_bpoco_config BPOCO Configuration
 *  @ingroup group_bpoco
 *
 * This module contains the configuration of the BCTC - Basic software for xCU power control complex device driver.
 */
/*@{*/
/* ========== Enumerations, Typedefinitions =========================== */


/* ========== Global variables  ======================================= */



/* ========== Global macros =========================================== */

/* === Port configuration === */
/* Power supply enable line */
#define DIO_PIN_BPOCO_SetPwrSplyEna             DIO_CHANNEL_ID(DIO_PORT_6, 3)    /**< PIN_159 - P6.03 */
/* Watchdog line */
#define DIO_PIN_BPOCO_SetWdgSig                 DIO_CHANNEL_ID(DIO_PORT_5, 15)   /**< PIN_009 - P5.15 */
/* Fused power relay activation pin */
#define DIO_PIN_BPOCO_SetRlyPwrEna              DIO_CHANNEL_ID(DIO_PORT_0, 15)   /**< PIN_176 - P0.15 */

/* ADC configuration */
#define BPOCO_UAnDigCnvrMeasd_UBat              BPOCO_AdcResult0[12]             /**< AN 12 */

#define BPOCO_WAKE_1                            /* 1st Wake Line */
#ifdef BPOCO_WAKE_1
#define BPOCO_UAnDigCnvrMeasd_UWk1              BPOCO_AdcResult0[10]             /**< AN 10 */
#endif

#define BPOCO_WAKE_2                            /* 2nd Wake Line */
#ifdef BPOCO_WAKE_2
#define BPOCO_UAnDigCnvrMeasd_UWk2              BPOCO_AdcResult0[11]             /**< AN 11 */
#endif

#define BPOCO_UAnDigCnvrMeasd_UGndDig           BPOCO_AdcResult0[8]              /**< AN 08 */

#define BPOCO_UAnDigCnvrMeasd_UGndPwr           BPOCO_AdcResult0[9]              /**< AN 09 */

#define BPOCO_UAnDigCnvrMeasd_UASI06            BPOCO_AdcResult0[6]              /**< AN 09 */


#define BPOCO_FUSE_1                            /* 1st FUSE is used */
#ifdef BPOCO_FUSE_1
#define BPOCO_UAnDigCnvrMeasd_UPwr1             BPOCO_AdcResult0[13]             /**< AN 13 */
#define BPOCO_UAnDigCnvrMeasd_IPwr1             BPOCO_AdcResult1[0]              /**< AN 16 */
#endif

#define BPOCO_FUSE_2                            /* 2nd FUSE is used */
#ifdef BPOCO_FUSE_2
#define BPOCO_UAnDigCnvrMeasd_UPwr2             BPOCO_AdcResult0[14]             /**< AN 14 */
#define BPOCO_UAnDigCnvrMeasd_IPwr2             BPOCO_AdcResult1[1]              /**< AN 17 */
#endif

#define BPOCO_FUSE_3                            /* 3rd FUSE is used */
#ifdef BPOCO_FUSE_3
#define BPOCO_UAnDigCnvrMeasd_UPwr3             BPOCO_AdcResult0[15]             /**< AN 15 */
#define BPOCO_UAnDigCnvrMeasd_IPwr3             BPOCO_AdcResult1[2]              /**< AN 18 */
#endif

/*====================================================================*/
/**
 * ADC measurement recurrence for battery and ground voltages. \n
 * hex. limits:  00H : FFH \n
 * phys. limits:   0 : 255 \n
 * unit: [ms] */
#define BPOCO_CfgMeasPerdBat     OS_TASK_10MS


/*====================================================================*/
/**
 * ADC measurement recurrence for fused power in- and outputs. \n
 * hex. limits:  00H : FFH \n
 * phys. limits:   0 : 255 \n
 * unit: [ms] */
#define BPOCO_CfgMeasPerdPwr        OS_TASK_10MS


/*====================================================================*/
/**
 * Number of used ports for fused power in- and outputs. \n
 * hex. limits:  00H : FFH \n
 * phys. limits:   0 : 255 \n
 * unit: [-] */
#define BPOCO_CfgNoPortPwr          3


/*====================================================================*/
/**
 * Factor between ADC voltage and measured battery voltage. \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 65.535 \n
 * unit: [-] */
#define BPOCO_CfgFacUBat            0x2C00      /**< Decimal value: 11 */


/*====================================================================*/
/**
 * Factor between ADC voltage and measured ground voltage. \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 65.535 \n
 * unit: [-] */
#define BPOCO_CfgFacUGnd            0x1000      /**< Decimal value: 4 */


/*====================================================================*/
/**
 * ADC voltage offset for ground voltage measurement. \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 4.99966515 \n
 * unit: [V] */
#define BPOCO_CfgUOfsGnd            0xC000      /**< Decimal value: 3.74980608 */


/*====================================================================*/
/**
 * Conversion factor between ADC voltage and measured current. \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 63.99902344 \n
 * unit: [A/V] */
#define BPOCO_CfgFacCnvnIPwr        0x1639      /**< Decimal value: 5.55564063 */

/*====================================================================*/
/**
 * ADC voltage offset for current measurement. \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 4.99966515 \n
 * unit: [V] */
#define BPOCO_CfgUOfsIPwr           0x8000      /**< Decimal value: 2.49987072 */

/*====================================================================*/
/**
 * Wait time after shutdown procedure to execute a microcontroller reset. \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 65535 \n
 * unit: [ms] */
#define BPOCO_CfgTiWaitShtdn        20u

/* ========== Global function prototypes  ============================= */

/*====================================================================*/
/**
 * Factor between ADC voltage and measured ASI06 \n
 * hex. limits:  0000H : FFFFH \n
 * phys. limits:     0 : 65.535 \n
 * unit: [-] */
#define BPOCO_CfgFacUAsi06           0x2C00      /**< Decimal value: 11 */



/* ========== End of Global variables (End of header function) ======== */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BPOCO_PWRCTLDRVR_CFG_H_ */



