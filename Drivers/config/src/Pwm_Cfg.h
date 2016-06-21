/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Pwm_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:00:53MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Pwm_Cfg.h  $
 *  Revision 1.2 2010/11/22 09:00:53MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/20 11:29:04MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#ifndef PWM_CFG_H_
#define PWM_CFG_H_

#include "Std_Types.h"


#define PWM_DEV_ERROR_DETECT        STD_ON
#define PWM_INSTANCE_ID				0


/** @defgroup group_pwm_cfg Pwm Configuration Parameters
 *  @ingroup group_pwm */
/*@{*/

#define PWM_MAX_CHANNELS				32		/**< Maximum number of Pwm channels. */

#define PWM_DEINIT_API					STD_ON   /**< Enable/disable of the service Pwm_DeInit(). */
#define PWM_SETDUTYCYCLE_API			STD_ON   /**< Enable/disable of the service Pwm_SetDutyCycle(). */
#define PWM_SETPERIODANDDUTYCYCLE_API	STD_ON   /**< Enable/disable of the service Pwm_SetPeriodAndDuty(). */
#define PWM_SETOUTPUTSTATETOIDLE_API	STD_ON   /**< Enable/disable of the service Pwm_SetOutputToIdle(). */
#define PWM_GETOUTPUTSTATE_API			STD_ON   /**< Enable/disable of the service Pwm_GetOutputState(). */
#define PWM_DISABLENOTIFICATION_API		STD_ON   /**< Enable/disable of the service Pwm_DisableNotification(). */
#define PWM_NOTIFICATIONSUPPORTED		STD_ON   /**< Enable/disable the notification support; Pwm_EnableNotification(), Pwm_DisableNotification(). */

/*@}*/

#endif /* PWM_CFG_H_ */

