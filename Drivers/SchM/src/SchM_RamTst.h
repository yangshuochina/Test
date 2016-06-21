/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_RamTst.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:31:55MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_RamTst.h  $
 *  Revision 1.2 2010/07/12 14:31:55MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef SCHM_RAMTST_H_
#define SCHM_RAMTST_H_

#include "Std_Types.h"
#include "SchM.h"

inline void SchM_Enter_RamTst(uint8 instance, uint8 exclusiveArea)
{
	SchM_Enter(instance, exclusiveArea);
}

inline void SchM_Exit_RamTst(uint8 instance, uint8 exclusiveArea)
{
	SchM_Exit(instance, exclusiveArea);
}

#endif /* SCHM_RAMTST_H_ */


