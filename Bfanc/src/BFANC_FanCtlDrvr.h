/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BFANC_FanCtlDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   24.08.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:19:46MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BFANC_FanCtlDrvr.h  $
 *  Revision 1.2 2010/11/22 07:19:46MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/08/25 14:45:51MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bfanc/src/project.pj
 *
 * ***************************************************************************/

#ifndef BFANC_FANCTLDRVR_H_
#define BFANC_FANCTLDRVR_H_

/* ========== Includes ===================================================== */
#include "BFANC_FanCtlDrvr_Cfg.h"


/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BFANC_FANCTLDRVR_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/** @defgroup group_bfanc BFANC
 *  @ingroup group_bfanc
 *
 *  Basic software for fan control complex device driver */
/*@{*/

/** @page bfanc BFANC
 *
 * @section bfanc_intro Introduction
 * This driver shall handle:
 *  @li the control of the power stages controlling the cooling fan of the battery pack
 *  @li the electrical diagnosis of cooling fan of the battery pack outputs
 *
 */

/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */
#define BFANC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BFANC_SetFanEna(boolean PrmFlagAcvt)
 *
 * @details This interface activates / deactivates the cooling fan of the battery pack.
 *
 * @param boolean PrmFlagAcvt - Activation state of the cooling fan. \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BFANC_SetFanEna(boolean PrmFlagAcvt);
/******************************************************************************/
/**
 * @fn uint8 BFANC_GetErrEltl(void)
 *
 * @details This interface gets the electrical error status of the low side driver used to control the battery cooling fan.
 *
 * @param none
 *
 * @return uint8 PrmErrSym - Error status (0: no error; 1: short circuit to ground; 2: open load; 3: over load / over temperature). \n
 * hex. limits: 0H : 3H \n
 * phys. limits: 0 : 3 \n
 * unit: [-] \n
 */
extern uint8 BFANC_GetErrEltl(void);
/*****************************************************************************/
#define BFANC_STOP_SEC_CODE_10MS
#include "MemMap.h"

/* ========== End of Global variables (End of header function) ============= */

/* parasoft unsuppress item MISRA2004-8_5 */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BFANC_FANCTLDRVR_H_ */
