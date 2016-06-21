/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BBA_BtldrApplDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   30.08.2011
 *  LAST REVISION:      $Date: 2012/06/04 08:22:53CEST $
 *                      $Author: Koller Andreas RGB (KOLLERA) $
 *
 * *************************************************************************** 
 *
 *  $Log: BBA_BtldrApplDrvr.c  $
 *  Revision 1.6 2012/06/04 08:22:53CEST Koller Andreas RGB (KOLLERA) 
 *  - BSW changes of application S19 file for MC reprogramming
 *  Revision 1.5 2012/03/21 08:57:58MEZ Koller Andreas RGB (KOLLERA) 
 *  - checksum-addresses changed
 *  Revision 1.4 2011/12/13 19:28:32MEZ Cotor Vlad RGB (COTORV) 
 *  - Update due to changes pragma
 *  Revision 1.3 2011/12/09 16:33:24CET Cotor Vlad RGB (COTORV) 
 *  - Update due to parasoft sil tests
 *  Revision 1.2 2011/11/02 15:05:57CET Cotor Vlad RGB (COTORV) 
 *  Initial version
 *  Revision 1.1 2011/10/10 14:27:29CEST Engoulou Roger RGB (ENGOULOR) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Cdd/BtLdr/src/project.pj
 *  Revision 1.1 2011/08/30 13:35:50CEST Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/Btldr/AA000/appl/src/project.pj
 *
 * ***************************************************************************/

/* ========== Includes ===================================================== */
#include "Std_Types.h"
#include "BBA_BtldrApplDrvr.h"           /* module header file     */
#include "Mcu.h"

/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */



/* ========== Local Variables ============================================== */
#define BSW_START_BTLDR_CMD_8BIT
#include "MemMap.h"
uint8 BBA_BtldrCmd;
#define BSW_STOP_BTLDR_CMD_8BIT
#include "MemMap.h"
/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */


/* ========== Global Functions ============================================= */


/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif 
