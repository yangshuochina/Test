/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BFANC_FanCtlDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   24.08.2010
 *  LAST REVISION:      $Date: 2010/12/13 08:36:39MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BFANC_FanCtlDrvr.c  $
 *  Revision 1.4 2010/12/13 08:36:39MEZ Cotor Vlad RGB (COTORV) 
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.3 2010/11/22 07:19:33MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/08/25 16:18:48MESZ Cotor Vlad RGB (COTORV) 
 *  Update BSW complex device drivers names
 *  Revision 1.1 2010/08/25 14:45:50MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bfanc/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_BFANC_FANCTLDRVR_C_              /* Master define for this module    */


/* ========== Includes ===================================================== */

#include "Dio.h"

#include "BFANC_FanCtlDrvr.h"           /* module header file     */
#include "BCTC_CtctrCtlDrvr.h"
#include "BSW_DrvrAux.h"

/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */



/* ========== Local Variables ============================================== */



/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */



/* ========== Global Functions ============================================= */


#define BFANC_START_SEC_CODE_10MS
#include "MemMap.h"
void BFANC_SetFanEna(boolean PrmFlagAcvt)
{
   if (PrmFlagAcvt)
   { Dio_WriteChannel(DIO_PIN_BFANC_SetFanEna, STD_HIGH); }
   else
   { Dio_WriteChannel(DIO_PIN_BFANC_SetFanEna, STD_LOW);  }
}
#define BFANC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BFANC_SetFanEna ========================================== */


#define BFANC_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 BFANC_GetErrEltl(void)
{
   return returnPrmErrSym((TLE8104.rx_buffer_UL2 & MASK_FanCtl) >> SHIFT_FanCtl);
}
#define BFANC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BFANC_GetErrEltl ========================================= */


/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif

/* undefine master define for this module    */
#ifdef MASTER_BFANC_FANCTLDRVR_C_
#undef MASTER_BFANC_FANCTLDRVR_C_
#endif

