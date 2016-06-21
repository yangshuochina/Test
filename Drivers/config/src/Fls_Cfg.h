/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fls_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:00:04MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fls_Cfg.h  $
 *  Revision 1.2 2010/11/22 09:00:04MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/20 11:29:00MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#ifndef FLS_CFG_H_
#define FLS_CFG_H_

#include "Std_Types.h"
#include "MemIf_Types.h"


#define FLS_DEV_ERROR_DETECT        STD_ON

#define FLS_INSTANCE_ID				0



/** @defgroup group_fls_cfg Fls Configuration Parameters
 *  @ingroup group_fls*/
/*@{*/

#define FLS_CANCEL_API					STD_ON   /**< Enable/disable of the service Fls_Cancel(). */
#define FLS_GETSTATUS_API				STD_ON   /**< Enable/disable of the service Fls_GetStatus(). */
#define FLS_GETJOBRESULT_API			STD_ON   /**< Enable/disable of the service Fls_GetJobResult(). */
#define FLS_COMPARE_API					STD_ON   /**< Enable/disable of the service Fls_Compare(). */
#define FLS_SETMODE_API					STD_ON   /**< Enable/disable of the service Fls_SetMode(). */


/*@}*/

#endif /* FLS_CFG_H_ */

