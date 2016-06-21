/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM_Cbk.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:32:03MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM_Cbk.h  $
 *  Revision 1.3 2010/11/22 08:32:03MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:25:41MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef NVM_CBK_H_
#define NVM_CBK_H_

#include "Std_Types.h"


/** @defgroup group_nvm_cbk NvM Callback Functions 
 *  @ingroup group_nvm */
/*@{*/

/** Function to be used by the underlying memory abstraction to signal end of job without error.
 *
 * The callback function NvM_JobEndNotification is used by the
 * underlying memory abstraction to signal end of job without error.
 */
extern void NvM_JobEndNotification(void);


/** Function to be used by the underlying memory abstraction to signal end of job with error.
 *
 * The callback function NvM_JobErrorNotification is to be used by the
 * underlying memory abstraction to signal end of job with error.
 */
extern void NvM_JobErrorNotification(void);


/*@}*/

#endif /* NVM_CBK_H_ */

