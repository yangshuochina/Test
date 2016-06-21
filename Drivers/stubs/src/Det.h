/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Det.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/08/20 08:40:33MESZ $
 *                    $Author: Opel Martin RGB (OPELM) $
 *
 * ==============================================================================
 * $Log: Det.h  $
 * Revision 1.2 2010/08/20 08:40:33MESZ Opel Martin RGB (OPELM) 
 * - added missing MemMap sections
 * Revision 1.1 2010/05/20 11:32:29MESZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/stubs/src/project.pj
 * Revision 1.1 2010/03/08 14:06:42MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/stubs/src/project.pj
 * *****************************************************************************/

#ifndef DET_H_
#define DET_H_

#include "Std_Types.h"


typedef struct {
	uint16 ModuleId;
	uint8 InstanceId;
	uint8 ApiId;
	uint8 ErrorId;
} Det_Error;



#define DET_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
extern Det_Error det_errors[];
#define DET_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"


#define DET_START_SEC_VAR_8BIT
#include "MemMap.h"
extern uint8 det_last_error;
#define DET_STOP_SEC_VAR_8BIT
#include "MemMap.h"



#define DET_START_SEC_CODE
#include "MemMap.h"
extern void Det_Reset(void);
extern void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);
#define DET_STOP_SEC_CODE
#include "MemMap.h"


#endif
