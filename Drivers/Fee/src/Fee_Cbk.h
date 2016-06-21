/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fee_Cbk.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:21:03MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fee_Cbk.h  $
 *  Revision 1.3 2010/11/22 08:21:03MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:23:05MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef FEE_CBK_H_
#define FEE_CBK_H_

#include "Std_Types.h"


/** @defgroup group_fee_cfg_callback Fee Callback functions
 *  @ingroup group_fee */
/*@{*/

/** Service to report the FEE module the successful end of an asynchronous operation.
 *
 * This callback is called when a job has been successfully completed.
 */
extern void Fee_JobEndNotification(void);


/** Service to report the FEE module the failure of an asynchronous operation.
 *
 * This callback is called when a job has completed with an error.
 */
extern void Fee_JobErrorNotification(void);

/*@}*/

#endif /* FEE_CBK_H_ */

