/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: ts_wait.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/20 11:21:24MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: ts_wait.c  $
 *  Revision 1.2 2010/05/20 11:21:24MESZ Cotor Vlad RGB (COTORV) 
 *  Member moved from ts_wait.c in project /PSL/Bsw/Misc/DrvAux/src/project.pj to ts_wait.c in project /PSL/Bsw/Misc/DrvAux/AA000/src/project.pj.
 *  Revision 1.1 2010/05/20 10:46:37CEST Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/Misc/DrvAux/src/project.pj
 *
 * ***************************************************************************/

#include "team-s.h"


void ts_wait(uint32 ticks)
{
	uint32 end = STM_TIM0.U + ticks;

	while((sint32)(STM_TIM0.U - end) < 0);
}
