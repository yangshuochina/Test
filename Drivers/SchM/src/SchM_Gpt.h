/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_Gpt.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:36:30MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_Gpt.h  $
 *  Revision 1.3 2010/11/22 08:36:30MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:31:31MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

/** @ingroup group_schm_gpt */

#ifndef SCHM_GPT_H_
#define SCHM_GPT_H_

#include "Std_Types.h"
#include "SchM.h"

inline void SchM_Enter_Gpt(uint8 instance, uint8 exclusiveArea)
{
	SchM_Enter(instance, exclusiveArea);
}

inline void SchM_Exit_Gpt(uint8 instance, uint8 exclusiveArea)
{
	SchM_Exit(instance, exclusiveArea);
}


#endif /* SCHM_GPT_H_ */
