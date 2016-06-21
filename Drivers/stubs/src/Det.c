/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Det.c $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/08/20 14:18:01MESZ $
 *                    $Author: Opel Martin RGB (OPELM) $
 *
 * ==============================================================================
 * $Log: Det.c  $
 * Revision 1.2 2010/08/20 14:18:01MESZ Opel Martin RGB (OPELM) 
 * - introduced MemMap sections
 * Revision 1.1 2010/05/20 11:32:27MESZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/stubs/src/project.pj
 * Revision 1.1 2010/03/08 14:06:42MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/stubs/src/project.pj
 * *****************************************************************************/

#include <string.h>
#include "Det.h"


#define DET_MAX_ERROR 16

#define DET_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
Det_Error det_errors[DET_MAX_ERROR];
#define DET_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"


#define DET_START_SEC_VAR_8BIT
#include "MemMap.h"
uint8 det_last_error;
#define DET_STOP_SEC_VAR_8BIT
#include "MemMap.h"


#define DET_START_SEC_CODE
#include "MemMap.h"
void Det_Reset(void)
{
	memset(det_errors, 0, sizeof(det_errors));
	det_last_error = 0;
}


void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
	if(det_last_error >= DET_MAX_ERROR) return;

	det_errors[det_last_error].ModuleId = ModuleId;
	det_errors[det_last_error].InstanceId = InstanceId;
	det_errors[det_last_error].ApiId = ApiId;
	det_errors[det_last_error].ErrorId = ErrorId;

	det_last_error++;
}
#define DET_STOP_SEC_CODE
#include "MemMap.h"
