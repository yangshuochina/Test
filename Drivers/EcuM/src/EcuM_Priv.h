/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:22MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM_Priv.h  $
 *  Revision 1.4 2011/02/02 09:17:22MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/11/22 07:58:54MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:22:10MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef ECUM_PRIV_H_
#define ECUM_PRIV_H_

#include "team-s.h"
#include "Std_Types.h"
#include "EcuM_Cfg.h"
#include "WdgM.h"

#ifdef ECUM_DETECT_DEVELOPMENT_ERRORS
#  define ECUM_ASSERT_RETERR(cond, api_id, error_id)     TEAMS_ASSERT_RETERR(cond, ECUM_MODULE_ID, ECUM_INSTANCE_ID, api_id, error_id)
#  define ECUM_ASSERT_RETVOID(cond, api_id, error_id)    TEAMS_ASSERT_RETVOID(cond, ECUM_MODULE_ID, ECUM_INSTANCE_ID, api_id, error_id)
#  define ECUM_ASSERT(cond, api_id, error_id)            TEAMS_ASSERT(cond, ECUM_MODULE_ID, ECUM_INSTANCE_ID, api_id, error_id)
#  define ECUM_ASSERT_REPORTONLY(cond, api_id, error_id) TEAMS_ASSERT_REPORTONLY(cond, ECUM_MODULE_ID, ECUM_INSTANCE_ID, api_id, error_id)
#else
#  define ECUM_ASSERT_RETERR(cond, api_id, error_id)
#  define ECUM_ASSERT_RETVOID(cond, api_id, error_id)
#  define ECUM_ASSERT(cond, api_id, error_id)
#  define ECUM_ASSERT_REPORTONLY(cond, api_id, error_id)
#endif

/** @defgroup group_ecum_pbt_config_wdgm EcuM Watchdog Manager Post Build Configuration
 *  @ingroup group_ecum_pbt_config
 *
 */
/*@{*/

/**
 * This type definition holds the configuration container EcuWdgM for the Watchdog Manager.
 */
typedef struct {
	WdgM_SupervisedEntityIdType supervised_entity_ref; /**< This parameter references the supervised entity
	 ID to be used for the #WdgM. */
	WdgM_ModeType startup_mode;  /**< This parameter references the WdgM mode to be set when entering #EcuM_StartupTwo. */
	WdgM_ModeType run_mode;      /**< This parameter references the WdgM mode to be set when entering #ECUM_STATE_RUN. */
	WdgM_ModeType wakeup_mode;   /**< This parameter references the WdgM mode to be set when entering #ECUM_STATE_WAKEUP. */
	WdgM_ModeType postrun_mode;  /**< This parameter references the WdgM mode to be set when entering #ECUM_STATE_APP_POST_RUN. */
	WdgM_ModeType shutdown_mode; /**< This parameter references the WdgM mode to be set when entering #ECUM_STATE_SHUTDOWN. */
} EcuM_WdgMConfigType;
/*@}*/

/** @defgroup group_ecum_pbt_config_nvram_mgr EcuM NvRam Manager Post Build Configuration
 *  @ingroup group_ecum_pbt_config
 *
 */
/*@{*/

/**
 * This type definition holds the configuration for the handling of the EcuM NvRam Manager.
 */
typedef struct {
	Gpt_ValueType nvram_write_all_timeout;  /**< Period in seconds scaled as ticks for which the Ecu State Manager will wait until
	it considers a WriteAllJob of the NvRam Manager as failed. */
	Gpt_ValueType nvram_read_all_timeout;  /**< Period in seconds scaled as ticks for which the Ecu State Manager will wait until
	it considers a ReadAllJob of the NvRam Manager as failed. */
} EcuM_NvMRamMgrConfigType;
/*@}*/

/**
 *  @ingroup group_ecum_config
 *
 */
/*@{*/
typedef uint32 EcuM_ConfigConsistencyHashType;
/*@}*/

#endif /* ECUM_PRIV_H_ */
