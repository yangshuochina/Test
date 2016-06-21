/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BAUXM_AuxMeasDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   23.04.2010
 *  LAST REVISION:      $Date: 2011/02/14 13:00:51MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BAUXM_AuxMeasDrvr_Cfg.h  $
 *  Revision 1.3 2011/02/14 13:00:51MEZ Cotor Vlad RGB (COTORV) 
 *  - Implement OSEK changes due to XCP update - after review
 *  Revision 1.2 2010/11/22 07:07:31MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/08/25 16:41:06MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bauxm/src/project.pj
 *  Revision 1.2 2010/07/19 09:56:07MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for auxiliary measurements complex device driver due to specification update
 *  Revision 1.1 2010/05/20 14:04:14MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bauxm/src/project.pj
 *
 * ***************************************************************************/

#ifndef BAUXM_AUXMEASDRVR_CFG_H_
#define BAUXM_AUXMEASDRVR_CFG_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */

#include "OsTaskRuntime.h"


/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BAUXM_AUXMEASDRVR_CFG_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/** @defgroup group_bauxm_config BAUXM Configuration
 *  @ingroup group_bauxm
 *
 * This module contains the configuration of the BAUXM - Basic software for auxiliary measurements complex device driver.
 */
/*@{*/


/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */

/* ADC configuration */
#define AuxMeas_CH_0          /**< 1st analog input */
#ifdef AuxMeas_CH_0
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_0        BAUXM_AdcResult0[0]      /**< AN 0  */
#endif

#define AuxMeas_CH_1          /**< 2nd analog input */
#ifdef AuxMeas_CH_1
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_1        BAUXM_AdcResult0[1]      /**< AN 1  */
#endif

#define AuxMeas_CH_2          /**< 3rd analog input */
#ifdef AuxMeas_CH_2
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_2        BAUXM_AdcResult0[2]      /**< AN 2  */
#endif

#define AuxMeas_CH_3          /**< 4th analog input */
#ifdef AuxMeas_CH_3
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_3        BAUXM_AdcResult0[3]      /**< AN 3  */
#endif

#define AuxMeas_CH_4          /**< 5th analog input */
#ifdef AuxMeas_CH_4
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_4        BAUXM_AdcResult0[4]      /**< AN 4  */
#endif

#define AuxMeas_CH_5          /**< 6th analog input */
#ifdef AuxMeas_CH_5
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_5        BAUXM_AdcResult0[5]      /**< AN 5  */
#endif

#define AuxMeas_CH_6          /**< 7th analog input */
#ifdef AuxMeas_CH_6
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_6        BAUXM_AdcResult0[6]      /**< AN 6  */
#endif

#define AuxMeas_CH_7          /**< 8th analog input */
#ifdef AuxMeas_CH_7
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_7        BAUXM_AdcResult0[7]      /**< AN 7  */
#endif

#define AuxMeas_CH_8          /**< 9th analog input */
#ifdef AuxMeas_CH_8
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_8        BAUXM_AdcResult1[11]     /**< AN 27 */
#endif

#define AuxMeas_CH_9          /**< 10th analog input */
#ifdef AuxMeas_CH_9
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_9        BAUXM_AdcResult1[13]     /**< AN 29 */
#endif

#define AuxMeas_CH_10         /**< 11th analog input */
#ifdef AuxMeas_CH_10
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_10       BAUXM_AdcResult1[14]     /**< AN 30 */
#endif

#define AuxMeas_CH_11         /**< 12th analog input */
#ifdef AuxMeas_CH_11
#define BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_11       BAUXM_AdcResult1[15]     /**< AN 31 */
#endif


/*====================================================================*/
/**
 * ADC measurement recurrence.\n
 * hex. limits:   00H : FFH                 \n
 * phys. limits:     0 : 255                 \n
 * unit        : [-] */
#define BAUXM_CfgMeasPerd           OS_TASK_10MS


/*====================================================================*/
/**
 * Number of ADC channels.\n
 * hex. limits:   00H : FFH                 \n
 * phys. limits:     0 : 255                 \n
 * unit        : [-] */
#define BAUXM_CfgNoChnAdc           12


/* ========== Global function prototypes =================================== */



/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BAUXM_AUXMEASDRVR_CFG_H_ */
