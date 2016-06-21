/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BSW_DrvrAux_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   24.09.2010
 *  LAST REVISION:      $Date: 2010/12/13 08:37:16MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BSW_DrvrAux_Cfg.h  $
 *  Revision 1.3 2010/12/13 08:37:16MEZ Cotor Vlad RGB (COTORV) 
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.2 2010/11/22 07:53:24MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/09/24 18:15:56MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Common/src/project.pj
 *
 * ***************************************************************************/

#ifndef BSW_DRVRAUX_CFG_H_
#define BSW_DRVRAUX_CFG_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */


/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BSW_DRVRAUX_CFG_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif
/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_bsw_drvaux_config BSW_DrvAux Configuration
 *  @ingroup group_bsw_drvaux
 *
 * This module contains the configuration of the BSW_DrvAux - complex device driver auxiliary functions.
 */
/*@{*/
/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */



/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BSW_DRVRAUX_CFG_H_ */
