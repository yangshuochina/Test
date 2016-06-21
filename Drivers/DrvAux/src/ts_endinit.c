/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: ts_endinit.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/20 11:21:23MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: ts_endinit.c  $
 *  Revision 1.2 2010/05/20 11:21:23MESZ Cotor Vlad RGB (COTORV) 
 *  Member moved from ts_endinit.c in project /PSL/Bsw/Misc/DrvAux/src/project.pj to ts_endinit.c in project /PSL/Bsw/Misc/DrvAux/AA000/src/project.pj.
 *  Revision 1.1 2010/05/20 10:46:32CEST Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/Misc/DrvAux/src/project.pj
 *
 * ***************************************************************************/

#include "team-s.h"

static uint32 ts_wdt0con_sav;


void ts_wdtpassword(void)
{
	uint32 con0;
	uint32 con1;

	con0 = WDT_CON0.U;
	con1 = WDT_CON1.U;

	/* clr LCK[0], HPW0[2,3] */
	con0 &= ~(BITPOS(1) | BITPOS(2) | BITPOS(3));
	/* set HPW1[4-7] and copy con1 IR[2], DR[3] */
	con0 |= 0xF0 | (con1 & (BITPOS(2) | BITPOS(3)));

	WDT_CON0.U = con0;
}


void ts_endinit_clear(void)
{
	uint32 con0;

	ts_wdt0con_sav = WDT_CON0.U;

	/* password access */
	ts_wdtpassword();

	/* clear endinit */
	con0 = WDT_CON0.U;
	con0 &= ~(BITPOS(0) | BITPOS(2) | BITPOS(3));
	con0 &= 0xFFFF;
	con0 |= 0xF2;

	WDT_CON0.U = con0;
}


void ts_endinit_set(void)
{
	uint32 con0;

	/* password access */
	ts_wdtpassword();

	/* set endinit */
	con0 = ts_wdt0con_sav;
	con0 &= ~(BITPOS(2) | BITPOS(3));
	con0 |= 0xF3;
	WDT_CON0.U = con0;
}
