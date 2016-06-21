/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Dem.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/05/20 11:32:24MESZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Dem.h  $
 * Revision 1.1 2010/05/20 11:32:24MESZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/stubs/src/project.pj
 * Revision 1.1 2010/03/08 14:06:41MEZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/stubs/src/project.pj
 * *****************************************************************************/

#ifndef DEM_H_
#define DEM_H_

#include "Std_Types.h"
#include "Dem_IntErrId.h"


#define DEM_EVENT_STATUS_PASSED     0
#define DEM_EVENT_STATUS_FAILED     1
#define DEM_EVENT_STATUS_PREPASSED  2
#define DEM_EVENT_STATUS_PREFAILED  3


inline void Dem_ReportErrorStatus(Dem_EventIdType id, uint8 status)
{

}

inline void Dem_Shutdown(void) {

}

#endif /* DEM_H_ */
