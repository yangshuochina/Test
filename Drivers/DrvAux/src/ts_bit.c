/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: ts_bit.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/20 11:21:23MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: ts_bit.c  $
 *  Revision 1.2 2010/05/20 11:21:23MESZ Cotor Vlad RGB (COTORV) 
 *  Member moved from ts_bit.c in project /PSL/Bsw/Misc/DrvAux/src/project.pj to ts_bit.c in project /PSL/Bsw/Misc/DrvAux/AA000/src/project.pj.
 *  Revision 1.1 2010/05/20 10:46:31CEST Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/Misc/DrvAux/src/project.pj
 *
 * ***************************************************************************/

#include "team-s.h"


int ts_getFirstBit(const int *name, int start, int num_of_bits)
{
	int idx;
	int offset;
	int word;
	int mask;
	int bitno;

	if(start >= num_of_bits) return -1;

	offset = start & 31;
	idx = start / 32;
	word = name[idx];

	/* first shot - check the first word */
	mask = 0xFFFFFFFFU >> offset;
	word &= mask;
	bitno = __clz(word);

	if(bitno == 32) {
		/* no set bit found - continue with the remaining words */
		num_of_bits = (num_of_bits - 1) / 32;
		while(idx < num_of_bits) {
			idx++;
			word = name[idx];
			bitno = __clz(word);
			if(bitno != 32) {
				return bitno + idx * 32;
			}
		}
		return -1;
	}
	else {
		/* return the first set bit */
		return bitno + idx * 32;
	}
}
