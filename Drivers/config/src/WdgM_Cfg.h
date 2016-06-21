/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgM_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:01:26MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgM_Cfg.h  $
 *  Revision 1.3 2010/11/22 09:01:26MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:44:35MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:29:07MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

/** @ingroup group_wdgm */

#ifndef WDGM_CFG_H_
#define WDGM_CFG_H_

#include "Std_Types.h"

/** @defgroup group_wdgm_cfg WdgM Configuration Parameters 
 *  @ingroup group_wdgm */
/*@{*/

#define WDGM_INSTANCE_ID				0

/** Enable or Disable Alive Supervision.
 * This macro is used as part of the #WdgMAliveSupervisionType configuration
 * structure. If is is #STD_OFF then alive supervision will no longer be done the
 * configured SEId.
 */
#define WDGM_ALIVE_SUPERVISION   				STD_ON

/**Enable or disable development error detection.
 * When this is #STD_ON all development errors (ex: parameter check)
 * were done by the driver.
 */
#define WDGM_DEV_ERROR_DETECT        			STD_ON

/**Immediate Reset after alive supervision failure.
 * During the Alive supervision cycle, if the Alive indication is
 * beyond the allowed limit and if the Global status is
 * #WDGM_ALIVE_STOPPED then this macro defines either immediate reset is
 * applicable/not.
 */
#define WDGM_IMMEDIATE_RESET 					STD_ON

/** WdgM OFF is allowed/not.
 *  This macro defined either the underlying WDG can be
 *  switched off or not. While selecting this option please
 *  synchronize all watchdog configuration too.
 */
#define WDGM_OFF_MODE_ENABLED 					STD_OFF

#define WDGM_NUMBER_OF_MODES 					(5)      /**< Total WdgM modes used. 0 to 255.*/

/** Total SEs to be used in WdgM
 * This will be sum of all unique SEs in all modes.
 * It can be 0 to 65535.
 */
#define WDGM_NUMBER_OF_SUPERVISED_ENTITIES      (9)

/** Total watchdogs used by WdgM
 * It can be 0 to 255.
 */
#define WDGM_NUMBER_OF_WATCHDOGS_INSTANCES      (1)


#define WDGM_STARTUP2_MODE                      ((WdgM_ModeType)0)   /**< Refer #EcuM_StartupTwo for more details about this mode.*/
#define WDGM_STATE_RUN_MODE                     ((WdgM_ModeType)1)   /**< Refer #ECUM_STATE_RUN for more details about this mode.*/
#define WDGM_STATE_WAKEUP_MODE                  ((WdgM_ModeType)2)   /**< Refer #ECUM_STATE_WAKEUP for more details about this mode.*/
#define WDGM_STATE_APP_POST_RUN_MODE            ((WdgM_ModeType)3)   /**< Refer #ECUM_STATE_APP_POST_RUN for more details about this mode.*/
#define WDGM_STATE_SHUTDOWN_MODE                ((WdgM_ModeType)4)   /**< Refer #ECUM_STATE_SHUTDOWN for more details about this mode.*/


/*@}*/

#define WDGM_GPT_CH_ID   (1) /**< This denotes the channel position in the structure #Gpt_ConfigChannelType */

#endif /* WDGM_CFG_H_ */

