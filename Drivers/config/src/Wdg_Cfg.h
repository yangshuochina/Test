/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Wdg_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:01:19MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Wdg_Cfg.h  $
 *  Revision 1.2 2010/11/22 09:01:19MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/20 11:29:06MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/


#ifndef WDG_CFG_H_
#define WDG_CFG_H_

#include "Std_Types.h"

/** @defgroup group_wdg_cfg Wdg Configuration Parameters 
 *  @ingroup group_wdg */
/*@{*/
#define WDG_INSTANCE_ID				0

#define WDG_DEV_ERROR_DETECT         STD_ON       /**< Enable or Disable development error detection. */
#define WDG_DISABLE_ALLOWED          STD_OFF      /**< Allow WDG Timer disable/not. */
/*@}*/

#endif /* WDG_CFG_H_ */

