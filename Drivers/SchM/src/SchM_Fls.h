/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_Fls.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:36:25MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_Fls.h  $
 *  Revision 1.3 2010/11/22 08:36:25MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:31:11MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef SCHM_FLS_H_
#define SCHM_FLS_H_

#include "team-s.h"
#include "Std_Types.h"
#include "SchM.h"
#include "Fls.h"

/** @defgroup group_schm_fls_if SchM Fls Module Interfaces
 *  @ingroup group_schm
 *
 * This module contains the interfaces of the SchM Fls.
 */
/*@{*/

/**
 * Triggers the activation of the main function execution \ref Fls_MainFunction.
 *
 *
 * @param activationPoint Parameter for identifying the unique execution point.
 * @return #SCHM_E_OK: No error occurred.
 *         #SCHM_E_LIMIT: To many task activation,
 *         #SCHM_E_STATE: ActivationPoint is already in use.
 * @return
 */
inline SchM_ReturnType SchM_ActMainFunction_Fls(uint8 activationPoint) {
	return SchM_ActMainFunction(FLS_MODULE_ID, activationPoint);
}

/**
 * Cancels the activation of the main function execution \ref Fls_MainFunction.
 *
 * @param Parameter referring to the corresponding ActivationPoint to be canceled,
 * @return  #SCHM_E_OK: No error occurred.
 *          #SCHM_E_NOFUNC : Currently, there is no activation of a main processing function for the given
 *          ActivationPoint pending.
 */
inline SchM_ReturnType SchM_CancelMainFunction_Fls(uint8 activationPoint) {
	return SchM_CancelMainFunction(FLS_MODULE_ID, activationPoint);
}

/*@}*/

#endif /* SCHM_FLS_H_ */
