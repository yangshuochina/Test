/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_Fee.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:36:19MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_Fee.h  $
 *  Revision 1.3 2010/11/22 08:36:19MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:30:29MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef SCHM_FEE_H_
#define SCHM_FEE_H_

#include "team-s.h"
#include "Std_Types.h"
#include "SchM.h"
#include "Fee.h"

/** @defgroup group_schm_fee_if SchM Fee Module Interfaces
 *  @ingroup group_schm
 *
 * This module contains the interfaces of the SchM Fee.
 */
/*@{*/

/**
 * Triggers the activation of the main function execution \ref Fee_MainFunction.
 *
 *
 * @param activationPoint Parameter for identifying the unique execution point.
 * @return #SCHM_E_OK: No error occurred.
 *         #SCHM_E_LIMIT: To many task activation,
 *         #SCHM_E_STATE: ActivationPoint is already in use.
 * @return
 */
inline SchM_ReturnType SchM_ActMainFunction_Fee(uint8 activationPoint) {
	return SchM_ActMainFunction(FEE_MODULE_ID, activationPoint);
}

/**
 * Cancels the activation of the main function execution \ref Fee_MainFunction.
 *
 * @param Parameter referring to the corresponding ActivationPoint to be canceled,
 * @return  #SCHM_E_OK: No error occurred.
 *          #SCHM_E_NOFUNC : Currently, there is no activation of a main processing function for the given
 *          ActivationPoint pending.
 */
inline SchM_ReturnType SchM_CancelMainFunction_Fee(uint8 activationPoint) {
	return SchM_CancelMainFunction(FEE_MODULE_ID, activationPoint);
}

/*@}*/

#endif /* SCHM_FEE_H_ */
