/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BELHPN_EltlHzrdProtnDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   28.10.2010
 *  LAST REVISION:      $Date: 2011/02/14 13:25:49MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BELHPN_EltlHzrdProtnDrvr_Cfg.h  $
 *  Revision 1.3 2011/02/14 13:25:49MEZ Cotor Vlad RGB (COTORV) 
 *  - Update BELHPN_EltlHzrdProtnDrvr_Cfg.h due to update of config documentation
 *  Revision 1.2 2010/11/15 13:51:08MEZ Cotor Vlad RGB (COTORV)
 *  Implement and integrate complex device driver for electrical hazard protection
 *
 * ***************************************************************************/

#ifndef BELHPN_ELTLHZRDPROTNDRVR_CFG_H_
#define BELHPN_ELTLHZRDPROTNDRVR_CFG_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */


/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BELHPN_ELTLHZRDPROTNDRVR_CFG_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_belhpn_config BELHPN Configuration
 *  @ingroup group_belhpn
 *
 * This module contains the configuration of the BELHPN - Basic software for electrical hazard protection complex device driver.
 */
/*@{*/
/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */

/* === Port configuration === */
/* Positive High Voltage */
#define DIO_PIN_BCTC_SetChnIsln_Pos_HV          DIO_CHANNEL_ID(DIO_PORT_0, 3)    /* PIN_148 - P0.03 - Positive High Voltage */
/* Negative High Voltage */
#define DIO_PIN_BCTC_SetChnIsln_Neg_HV          DIO_CHANNEL_ID(DIO_PORT_0, 4)    /* PIN_166 - P0.04 - Negative High Voltage */



#define Chip_AS8510_0_address_j0_cmd            0x80u
#define Chip_AS8510_0_address_j1_cmd            0x80u
#define Chip_AS8510_0_address_j2_cmd            0x80u

#define Chip_AS8510_0_address_j3_cmd            0x15u
#define Chip_AS8510_0_address_j4_cmd            0x13u

#define Chip_AS8510_0_address_j5_cmd            0x17u

/*====================================================================*/
/**
 * Microcontroller measurement recurrence of AS8510 for electrical hazard protection. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 31.875 \n
 * unit: [ms] */
#define BELHPN_CfgMeasPerd                      4u


/*====================================================================*/
/**
 * Hardware internal measurement recurrence of AS8510 for electrical hazard protection - directly configured as SPI data word (see datasheet). \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [-] */
#define BELHPN_CfgMeasPerdHW                    0x4548u        /**< Decimal value: 17736 */


/*====================================================================*/
/**
 * Programmable gain of the voltage measurement – initial value (allowed values are 1, 5, 25, 40 and 100). \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit : [-] */
#define BELHPN_CfgGainIni                       1u


/*====================================================================*/
/**
 * Resolution of measured ADC voltage by the AS8510 without gain. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 655.35e-3 \n
 * unit: [-] */
#define BELHPN_CfgUReslBas                      0x3670u        /**< Decimal value: 1.3936e-1 */


/* ========== Global function prototypes =================================== */



/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BELHPN_ELTLHZRDPROTNDRVR_CFG_H_ */
