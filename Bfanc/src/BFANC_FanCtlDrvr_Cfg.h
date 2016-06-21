/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BFANC_FanCtlDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   24.08.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:20:07MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BFANC_FanCtlDrvr_Cfg.h  $
 *  Revision 1.2 2010/11/22 07:20:07MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/08/25 14:45:50MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bfanc/src/project.pj
 *
 * ***************************************************************************/

#ifndef BFANC_FANCTLDRVR_CFG_H_
#define BFANC_FANCTLDRVR_CFG_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */


/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BFANC_FANCTLDRVR_CFG_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_bfanc_config BFANC Configuration
 *  @ingroup group_bfanc
 *
 * This module contains the configuration of the BFANC - Basic software for fan control complex device driver.
 */
/*@{*/

/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */

/* Cooling fan of the battery pack */
#define DIO_PIN_BFANC_SetFanEna                 DIO_CHANNEL_ID(DIO_PORT_2, 3)    /**< PIN_077 - P2.03 */

#define BFANC_TLE8104_sequence                  2
#define BFANC_TLE8104_channel                   5
#define BFANC_TLE8104_data                      0x0000


/*====================================================================*/
/*
 * TLE8104E - SPI Control - Serial Output
 * +--------------+--------------+--------------+--------------+
 * |     CH4      |     CH3      |     CH2      |     CH1      |
 * +--------------+--------------+--------------+--------------+
 * | ch4_1  ch4_0 | ch3_1  ch3_0 | ch2_1  ch2_0 | ch1_1  ch1_0 |
 * +--------------+--------------+--------------+--------------+
 * | MASK  = 0xC0 | MASK  = 0x30 | MASK  = 0x0C | MASK  = 0x03 |
 * | SHIFT = 6    | SHIFT = 4    | SHIFT = 2    | SHIFT = 0    |
 * +--------------+--------------+--------------+--------------+
 */
/* for BFANC_GetErrEltl */
#define MASK_FanCtl                             0xC0     /**< 4th channel of UL2 */
#define SHIFT_FanCtl                            6

/* ========== Global function prototypes =================================== */



/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BFANC_FANCTLDRVR_CFG_H_ */
