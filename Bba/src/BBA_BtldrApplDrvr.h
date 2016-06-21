/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BBA_BtldrApplDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   30.08.2011
 *  LAST REVISION:      $Date: 2012/06/26 17:20:33CEST $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: BBA_BtldrApplDrvr.h  $
 *  Revision 1.6 2012/06/26 17:20:33CEST Cotor Vlad RGB (COTORV) 
 *  - Update doxygen documentation
 *  Revision 1.5 2012/06/04 08:22:52CEST Koller Andreas RGB (KOLLERA) 
 *  - BSW changes of application S19 file for MC reprogramming
 *  Revision 1.4 2011/12/13 19:28:33MEZ Cotor Vlad RGB (COTORV) 
 *  - Update due to changes pragma
 *  Revision 1.3 2011/12/09 16:33:24CET Cotor Vlad RGB (COTORV) 
 *  - Update due to parasoft sil tests
 *  Revision 1.2 2011/11/02 15:06:07CET Cotor Vlad RGB (COTORV) 
 *  Initial version
 *  Revision 1.2 2011/10/10 14:36:48CEST Engoulou Roger RGB (ENGOULOR) 
 *  - update due to comment correction
 *  Revision 1.1 2011/10/10 14:27:30MESZ Engoulou Roger RGB (ENGOULOR) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Cdd/BtLdr/src/project.pj
 *  Revision 1.1 2011/08/30 13:35:52CEST Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/Btldr/AA000/appl/src/project.pj
 *
 * ***************************************************************************/

#ifndef BBA_BTLDRAPPLDRVR_H_
#define BBA_BTLDRAPPLDRVR_H_

/* ========== Includes ===================================================== */

/** @defgroup group_bba BBA
 *  @ingroup group_bba
 *
 *  Basic software for bootloader application complex device driver */
/*@{*/


/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */
#define BSW_START_BTLDR_CMD_8BIT
#include "MemMap.h"

extern uint8 BBA_BtldrCmd;

#define BSW_STOP_BTLDR_CMD_8BIT
#include "MemMap.h"

/* ========== Global macros ================================================ */

/* ========== Global function prototypes =================================== */

/* ========== End of Global variables (End of header function) ============= */
/*@}*/

#endif    /* BBA_BTLDRAPPLDRVR_H_ */
