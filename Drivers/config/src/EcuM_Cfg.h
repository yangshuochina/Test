/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/22 15:04:00MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM_Cfg.h  $
 *  Revision 1.4 2011/02/22 15:04:00MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.3 2010/11/22 08:59:50MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 13:59:54MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

 #ifndef ECUM_CFG_H_
#define ECUM_CFG_H_

#include "team-s.h"
#include "Std_Types.h"
#include "Mcu_Cfg.h"

#define ECUM_INSTANCE_ID				0


/** @defgroup group_ecum_config EcuM Configuration
 *  @ingroup group_ecum
 *
 * This module contains the configuration of the EcuM - ECU State Manager.
 */
/*@{*/

/**
 * Enable or disable development error detection.
 * Selection STD_ON, STD_OFF.
 */
#define ECUM_DEV_ERROR_DETECT STD_ON

/* --- nothing to configure here ---*/
#if defined(ECUM_DEV_ERROR_DETECT) && (ECUM_DEV_ERROR_DETECT == STD_ON)
#define ECUM_DETECT_DEVELOPMENT_ERRORS
#endif
/*@}*/

/** @defgroup group_ecum_pct_config_wdgm EcuM Watchdog Manager Pre Compile Time Configuration
 *  @ingroup group_ecum_pct_config
 *
 */
/*@{*/

/**
 * This configuration parameter defines whether the watchdog manager
 * is supported by the EcuM.
 *
 * This feature is presented for development purpose to compile out the
 * watchdog manager in the early debugging phase.
 */
#define ECUM_INCLUDE_WDGM (STD_OFF)

/* --- nothing to configure here ---*/
#if defined(ECUM_INCLUDE_WDGM) && (ECUM_INCLUDE_WDGM == STD_ON)
#include "WdgM.h"
#define ECUM_WDGM_INCLUDED
#endif

/*@}*/

/** @defgroup group_ecum_pct_config_nvmram_mgr EcuM NVM RAM Manager Pre Compile Time Configuration
 *  @ingroup group_ecum_pct_config
 *
 */
/*@{*/

/**
 * This configuration parameter defines whether the NVM manager
 * is supported by the EcuM.
 *
 * This feature is presented to compile out the
 * NVM usage.
 */
#define ECUM_INCLUDE_NVMRAM_MGR (STD_ON)

/* --- nothing to configure here ---*/
#if defined(ECUM_INCLUDE_NVMRAM_MGR) && (ECUM_INCLUDE_NVMRAM_MGR == STD_ON)
#include "NvM.h"
#define ECUM_NVMRAM_MGR_INCLUDED
#endif

/*@}*/


/** @defgroup group_ecum_pct_config_dem EcuM Diagnostics Event Manager Pre Compile Time Configuration
 *  @ingroup group_ecum_pct_config
 *
 */

/*@{*/
/**
 * This configuration parameter defines whether the diagnostic event manager
 * is supported by the EcuM.
 *
 * Possible selection is #STD_ON or #STD_OFF.
 */
#define ECUM_INCLUDE_DEM (STD_ON)

/* --- nothing to configure here ---*/
#if defined(ECUM_INCLUDE_DEM) && (ECUM_INCLUDE_DEM == STD_ON)
#include "Dem.h"
#define ECUM_DEM_INCLUDED
#endif

/*@}*/

/** @defgroup group_ecum_pct_config_comm EcuM Communication Manager Pre Compile Time Configuration
 *  @ingroup group_ecum_pct_config
 *
 */
/*@{*/

/**
 * This configuration parameter defines whether the ComM shall be deinitialized during
 * shutdown state #ECUM_STATE_GO_OFF_ONE.
 *
 * Possible selection is #STD_ON or #STD_OFF.
 *
 */
#define ECUM_SHUTDOWN_COMM (STD_ON)

/* --- nothing to configure here ---*/
#if defined(ECUM_SHUTDOWN_COMM) && (ECUM_SHUTDOWN_COMM == STD_ON)
#define ECUM_PERFORM_SHUTDOWN_COMM
#endif
/*@}*/


/** @defgroup group_ecum_pct_config EcuM Pre Compile Time Configuration
 *  @ingroup group_ecum_config
 *
 */
/*@{*/

/**
 * The hash value generated across all-pre-compiled and link-time parameters of
 * all BSW modules. This hash value is compared against the field \ref EcuM_ConfigType::config_consistency_hash
 * and hence allows checking the consistency of the entire configuration.
 * TODO: Calculate hash value for ECUM_CONFIGCONSISTENCY_HASH check with configuration tool.
 */
#define ECUM_CONFIGCONSISTENCY_HASH ((EcuM_ConfigConsistencyHashType) 0)

#define ECUM_RUN_SELF_REQUEST_PERIOD (2.0) /**< Duration given in seconds (float), for which the ECU State Manager will stay in RUN state even, if no one requests RUN. */


#if defined(ECUM_RUN_SELF_REQUEST_PERIOD)
#define ECUM_RUN_SELF_REQUEST_PERIOD_TICKS ((Gpt_ValueType)(GPTA0_GT0_TICKS(ECUM_RUN_SELF_REQUEST_PERIOD)))

#else
#error ECUM_RUN_SELF_REQUEST_PERIOD is not defined!
#endif

#ifdef ECUM_NVMRAM_MGR_INCLUDED

#define ECUM_NVRAM_WRITEALL_TIMEOUT (0.5) /**< Duration given in seconds (float), for which the  ECU State Manager will wait until
                                               it considers a WriteAllJob of the NvRam Manager as failed. */

#if defined(ECUM_NVRAM_WRITEALL_TIMEOUT)
#define ECUM_NVRAM_WRITEALL_TIMEOUT_TICKS ((Gpt_ValueType)(GPTA0_GT0_TICKS(ECUM_NVRAM_WRITEALL_TIMEOUT)))
#else
#error ECUM_NVRAM_WRITEALL_TIMEOUT is not defined!
#endif

#define ECUM_NVRAM_READALL_TIMEOUT (0.5) /**< Duration given in seconds (float), for which the  ECU State Manager will wait until
                                              it considers a ReadAllJob of the NvRam Manager as failed. */

#if defined(ECUM_NVRAM_READALL_TIMEOUT)
#define ECUM_NVRAM_READALL_TIMEOUT_TICKS ((Gpt_ValueType)(GPTA0_GT0_TICKS(ECUM_NVRAM_READALL_TIMEOUT)))
#else
#error ECUM_NVRAM_READALL_TIMEOUT is not defined!
#endif

#endif

/*@}*/


/** @defgroup group_ecum_pct_config_user EcuM User Pre Compile Time Configuration
 *  @ingroup group_ecum_pct_config
 *
 * This configuration contains the user configuration of the EcuM - EcuMUserConfig{EcuM_User}.
 * A list of unique user id's can be defined here.
 * @warning The number of user id's must be in the range of 0..31. Thus a maximum of 32 users is possible.
 */
/*@{*/
#define ECUM_USER_ID_0   ((EcuM_UserType) 0)  /**< Example user with ID 0. */
#define ECUM_USER_ID_1   ((EcuM_UserType) 1)  /**< Example user with ID 1. */
#define ECUM_USER_ID_2   ((EcuM_UserType) 2)  /**< Example user with ID 2. */

/**
 * This mask contains all user id's as bitfield, where
 * a user with id 0 must set bit 0 in the mask,
 * a user with id 1 must set bit 1 in the mask and so on.
 */
#define ECUM_ALL_USER_ID_MASK ((1 << ECUM_USER_ID_0) | \
                               (1 << ECUM_USER_ID_1) | \
                               (1 << ECUM_USER_ID_2))

extern boolean ecum_nvm_job_finished;

/*@}*/

#endif /* ECUM_CFG_H_ */
