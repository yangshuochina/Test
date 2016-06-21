/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM_Generated_Types.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:58:40MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM_Generated_Types.h  $
 *  Revision 1.3 2010/11/22 07:58:40MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:21:41MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef ECUM_GENERATED_TYPES_H_
#define ECUM_GENERATED_TYPES_H_

#include "team-s.h"
#include "Std_Types.h"
#include "EcuM_Cfg.h"
#include "EcuM_Priv.h"
#include "Adc.h"
#include "Fls.h"
#include "Gpt.h"
#include "Mcu.h"
#include "Port.h"
#include "Pwm.h"
#include "Spi.h"
#include "Wdg.h"

#ifdef ECUM_WDGM_INCLUDED
#include "WdgM.h"
#endif

#include "OSEK_types.h"

/** @defgroup group_ecum_pbt_config EcuM Post Build Time Configuration
 *  @ingroup group_ecum_config
 *
 */
/*@{*/

/** This is the initial EcuM configuration structure. */
typedef struct {
	EcuM_ConfigConsistencyHashType config_consistency_hash; /**< Hash value over all pre-compiled and link-time parameters of all BSW modules. */
	AppModeType default_application_mode;                   /**< The default application mode. */
	Gpt_ChannelType gpt_timer_channel;                      /**< The timer channel to measure periods. */
	Gpt_ValueType ecum_run_self_request_period;             /**< Duration given timer ticks, for which the  ECU State Manager will stay in RUN state
	 even if no one requests RUN. */
	struct {
		struct {
			const Adc_ConfigType *adc_cfg;      /**< The pointer to the adc configuration.  */
			const Fls_ConfigType *fls_cfg;      /**< The pointer to the fls configuration.  */
			const Gpt_ConfigType *gpt_cfg;      /**< The pointer to the gpt configuration.  */
			const Mcu_ConfigType *mcu_cfg;      /**< The pointer to the mcu configuration.  */
			const Port_ConfigType *port_cfg;    /**< The pointer to the port configuration. */
			const Pwm_ConfigType *pwm_cfg;      /**< The pointer to the pwm configuration.  */
			const Spi_ConfigType *spi_cfg;      /**< The pointer to the spi configuration.  */
			const Wdg_ConfigType *wdg_cfg;      /**< The pointer to the wdg configuration.  */
#ifdef ECUM_WDGM_INCLUDED
			const WdgM_ConfigType *wdgm_cfg;    /**< The pointer to the wdgm configuration.  */
#endif
		} init_one;                          /**< Initialization functions called from \ref EcuM_AL_DriverInitOne. */
		struct {
			int tmp;
		} init_two;                          /**< Initialization functions called from \ref EcuM_AL_DriverInitTwo. */
		struct {
			int tmp;
		} init_three;                        /**< Initialization functions called from \ref EcuM_AL_DriverInitThree. */
	} bsw_driver;                            /**< BSW driver configuration. */
#ifdef ECUM_WDGM_INCLUDED
	EcuM_WdgMConfigType ecum_wdgm_cfg;       /**< Configuration of WdgM - Watchdog Manager. */
#endif
#ifdef ECUM_NVMRAM_MGR_INCLUDED
	EcuM_NvMRamMgrConfigType ecum_nvmram_mgr_cfg; /**< EcuM configuration to handle NvM - NVRAM Manager. */
#endif

} EcuM_ConfigType;

/*@}*/

#endif /* ECUM_GENERATED_TYPES_H_ */
