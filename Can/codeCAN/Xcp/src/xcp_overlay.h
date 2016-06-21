/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: xcp_overlay.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   08.10.2010
 *  LAST REVISION:      $Date: 2010/10/08 16:02:52MESZ $
 *                      $Author: Opel Martin RGB (OPELM) $
 *
 * ***************************************************************************
 *
 *  $Log: xcp_overlay.h  $
 *  Revision 1.1 2010/10/08 16:02:52MESZ Opel Martin RGB (OPELM) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Can/codeCAN/Xcp/src/project.pj
 *
 * ***************************************************************************/

#ifndef XCP_OVERLAY_H_
#define XCP_OVERLAY_H_

/* ========== Includes ===================================================== */
#include "Std_Types.h"     /* Contains the basic data types. */



/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */
#define XCP_START_SEC_CODE_INIT
#include "MemMap.h"
extern void RAM_OverlayRamReset(void);
#define XCP_STOP_SEC_CODE_INIT
#include "MemMap.h"



#define XCP_START_SEC_CODE
#include "MemMap.h"
extern void RAM_OverlayRamEnable(boolean enable);
#define XCP_STOP_SEC_CODE
#include "MemMap.h"


#endif    /* XCP_OVERLAY_H_ */
