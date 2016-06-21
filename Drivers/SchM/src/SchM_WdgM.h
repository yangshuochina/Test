/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_WdgM.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:50:58MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_WdgM.h  $
 *  Revision 1.2 2010/07/12 09:50:58MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:26:17MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/ServiceLayer/SchM/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef SCHM_WDGM_H_
#define SCHM_WDGM_H_

#include "team-s.h"
#include "Std_Types.h"
#include "SchM.h"

inline void SchM_Enter_WdgM(uint8 instance, uint8 exclusiveArea)
{
	SchM_Enter(instance, exclusiveArea);
}

inline void SchM_Exit_WdgM(uint8 instance, uint8 exclusiveArea)
{
	SchM_Exit(instance, exclusiveArea);
}

#endif /* SCHM_WDGM_H_ */


