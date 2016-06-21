/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Gpt_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:00:16MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Gpt_Cfg.h  $
 *  Revision 1.3 2010/11/22 09:00:16MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:43:08MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:29:01MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#ifndef GPT_CFG_H_
#define GPT_CFG_H_

#include "team-s.h"
#include "Std_Types.h"

#define GPT_INSTANCE_ID				0



/** @defgroup group_gpt_pct_config Gpt Module Pre Compile Time Configuration
 *  @ingroup group_gpt_config
 *
 */
/*@{*/
/*@}*/

/**
 * Enable or disable development error detection.
 * Selection STD_ON, STD_OFF.
 */
#define GPT_DEV_ERROR_DETECT (STD_ON)

/** @defgroup group_gpt_pct_config_module Gpt General Purpose Timer Module Global Configuration
 *  @ingroup group_gpt_pct_config
 *
 */

/*@{*/

/**
 * This define sets the maximum timer channels that are handled by the GPT module.
 * The value range is from 0..31. The value shall not be less than the configuration
 * #Gpt_ConfigType::num_of_channels.
 */
#define GPT_MAX_TIMER_CHANNELS (5)
/*@}*/

/** @defgroup group_gpt_pct_config_api Gpt General Purpose Timer Module API Configuration
 *  @ingroup group_gpt_pct_config
 *
 */

/*@{*/

/**
 * Adds / removes the service #Gpt_DeInit from the code.
 *
 * Selection STD_ON, STD_OFF.
 *
 */
#define GPT_DEINIT_API (STD_ON)

/**
 * Adds / removes the services #Gpt_EnableNotification and #Gpt_DisableNotification from the code.
 *
 * Selection STD_ON, STD_OFF.
 */
#define GPT_ENABLE_DISABLE_NOTIFICATION_API (STD_ON)

/**
 * Adds / removes the service #Gpt_GetTimeElapsed from the code.
 *
 * Selection STD_ON, STD_OFF.
 */
#define GPT_TIME_ELAPSED_API (STD_ON)

/**
 * Adds / removes the service #Gpt_GetTimeRemaining from the code.
 *
 * Selection STD_ON, STD_OFF.
 */
#define GPT_TIME_REMAINING_API (STD_ON)

/**
 * Adds / removes the service #Gpt_GetVersionInfo from the code.
 *
 * Selection STD_ON, STD_OFF.
 */
#define GPT_VERSION_INFO_API (STD_ON)

/**
 * Adds / removes the services  the services #Gpt_SetMode, #Gpt_EnableWakeup, #Gpt_DisableWakeup
 * and #Gpt_Cbk_CheckWakeup from the code.

 *
 * Selection STD_ON, STD_OFF.
 *
 * @warning Wakeup is not supported by the implementation. Selection should be always STD_OFF.
 *
 */
#define GPT_WAKEUP_FUNCTIONALITY_API (STD_OFF)

/*@}*/




/** @defgroup group_gpt_pct_config_wakeup Gpt General Purpose Timer Module Wakeup Source Report Pre Compile Time Configuration
 *  @ingroup group_gpt_pct_config
 *
 */
/*@{*/

/**
 * This configuration parameter enables or disables the wakeup source reporting by the Gpt.
 *
 * Possible selection is #STD_ON or #STD_OFF.
 *
 * @warning Wakeup is not supported by the implementation. Selection should be always STD_OFF.
 */
#define GPT_REPORT_WAKEUP_SOURCE (STD_OFF)


/*@}*/

/* --- nothing to configure here ---*/
#if defined(GPT_DEV_ERROR_DETECT) && (GPT_DEV_ERROR_DETECT == STD_ON)
#define GPT_DETECT_DEVELOPMENT_ERRORS
#endif
#if defined(GPT_DEINIT_API) && (GPT_DEINIT_API == STD_ON)
#define GPT_INCLUDE_DEINIT_API
#endif
#if defined(GPT_ENABLE_DISABLE_NOTIFICATION_API) && (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
#define GPT_INCLUDE_ENABLE_DISABLE_NOTIFICATION_API
#endif
#if defined(GPT_TIME_ELAPSED_API) && (GPT_TIME_ELAPSED_API == STD_ON)
#define GPT_INCLUDE_IME_ELAPSED_API
#endif
#if defined(GPT_TIME_REMAINING_API) && (GPT_TIME_REMAINING_API == STD_ON)
#define GPT_INCLUDE_TIME_REMAINING_API
#endif
#if defined(GPT_VERSION_INFO_API) && (GPT_VERSION_INFO_API == STD_ON)
#define GPT_INCLUDE_VERSION_INFO_API
#endif
#if defined(GPT_WAKEUP_FUNCTIONALITY_API) && (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON)
#define GPT_INCLUDE_WAKEUP_FUNCTIONALITY_API
#endif
#if defined(GPT_REPORT_WAKEUP_SOURCE) && (GPT_REPORT_WAKEUP_SOURCE == STD_ON)
#include "EcuM.h"
#define GPT_INCLUDE_REPORT_WAKEUP_SOURCE
#endif


#endif /* GPT_CFG_H_ */
