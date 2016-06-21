/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgM_PBcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:16:54MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgM_PBcfg.c  $
 *  Revision 1.3 2011/02/02 09:16:54MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 14:17:14MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "WdgM.h"

#define WDGM_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

static const WdgMAliveSupervisionType WdgM_AS_startup2[] = {
		/* SEId,Supervision enable/disable, deacttivation allowed/not, failed as, max SE ind, min SE ind, ref SE Cyc*/
		{0U,	WDGM_SUPERVISION_DISABLED, WDGM_DEACTIVATION_ACCESS_ENABLED, 5U, 55U, 45U, 5U },
		{1U,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_DISABLED, 5U, 120U, 95U, 10U },
		{2U,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_DISABLED, 5U, 155U, 135U, 14U },
		{3U,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_DISABLED, 5U, 400U, 200U, 27U },
};

static const WdgMAliveSupervisionType WdgM_AS_run[] = {
		/* SEId,Supervision enable/disable, deacttivation allowed/not, failed as, max SE ind, min SE ind, ref SE Cyc*/
		{4,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_DISABLED, 5, 76, 66, 7 },
		{5,	WDGM_SUPERVISION_DISABLED, WDGM_DEACTIVATION_ACCESS_ENABLED, 2, 145, 125, 13 },
		{6,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_DISABLED, 4, 280, 260, 27 }
};

static const WdgMAliveSupervisionType WdgM_AS_wakeup[] = {
		/* SEId,Supervision enable/disable, deacttivation allowed/not, failed as, max SE ind, min SE ind, ref SE Cyc*/
		{7,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_DISABLED, 15, 220, 195, 20 },
};

static const WdgMAliveSupervisionType WdgM_AS_postrun[] = {
		/* SEId,Supervision enable/disable, deacttivation allowed/not, failed as, max SE ind, min SE ind, ref SE Cyc*/
		{8,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_ENABLED, 15, 270, 225, 25 },
		{0,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_ENABLED, 17, 390, 325, 35 },
};

static const WdgMAliveSupervisionType WdgM_AS_shutdown[] = {
		/* SEId,Supervision enable/disable, deacttivation allowed/not, failed as, max SE ind, min SE ind, ref SE Cyc*/
		{1,	WDGM_SUPERVISION_ENABLED, WDGM_DEACTIVATION_ACCESS_ENABLED, 15, 270, 185, 22 },
};

static const WdgMTriggerType WdgMTrigger_s = {
		0,                      			/**< Watchdog ID to be used in WdgIf interfaces. */
		75,              					/**< How often the WatchDog to be triggered(trig reg cycle). */
		WDGIF_SLOW_MODE         			/**< Mode of the WatchDog. */
};

static const WdgMTriggerType WdgMTrigger_f = {
		0,                      			/**< Watchdog ID to be used in WdgIf interfaces. */
		5,              					/**< How often the WatchDog to be triggered(trig reg cycle). */
		WDGIF_FAST_MODE         			/**< Mode of the WatchDog. */
};

#ifdef WDGM_ENABLE_MTRIGGER_O
static const WdgMTriggerType WdgMTrigger_o = {
		0,                      			/**< Watchdog ID to be used in WdgIf interfaces. */
		30,              					/**< How often the WatchDog to be triggered(trig reg cycle). */
		WDGIF_OFF_MODE         				/**< Mode of the WatchDog. */
};
#endif

static const WdgMActivationGptType WdgMActivationGpt = {
		WDGM_GPT_CH_ID,       				/**< GPT Channel Used by WDDGM.  */
		500                   				/**< GPT Channel Timeout Period. */
};

static const WdgMModeType WdgMMode[] = {
		/* Mode:WDGM_STARTUP2_MODE */
		{
				10, 								/**< WDG Trigger Tolerance after expiring. */
				1,         						 	/**< Total number of watchdogs. */
				WDGM_ACTIVATION_TYPE_GPT,        	/**< Mode activation is GPT/SCHM. */
				sizeof(WdgM_AS_startup2)/sizeof(WdgMAliveSupervisionType),  /**< Total Number of SEs in this mode. */
				&WdgM_AS_startup2[0],      			/**< Pointer to  configuration parameters of Alive-Supervision. */
				&WdgMActivationGpt,         		/**< Pointer to GPT Configuration for WDGM. */
				&WdgMTrigger_f            			/**< Pointer to Watchdog Configuration for WDGM. */
		},
		/* Mode:WDGM_STATE_RUN_MODE */
		{
				10, 								/**< WDG Trigger Tolerance after expiring. */
				1,          						/**< Total number of watchdogs. */
				WDGM_ACTIVATION_TYPE_SCHEDULER,     /**< Mode activation is GPT/SCHM. */
				sizeof(WdgM_AS_run)/sizeof(WdgMAliveSupervisionType),  /**< Total Number of SEs in this mode. */
				&WdgM_AS_run[0],       				/**< Pointer to  configuration parameters of Alive-Supervision. */
				&WdgMActivationGpt,          		/**< Pointer to GPT Configuration for WDGM. */
				&WdgMTrigger_s               		/**< Pointer to Watchdog Configuration for WDGM. */
		},
		/* Mode:WDGM_STATE_WAKEUP_MODE */
		{
				10, 								/**< WDG Trigger Tolerance after expiring. */
				1,          						/**< Total number of watchdogs. */
				WDGM_ACTIVATION_TYPE_GPT,     		/**< Mode activation is GPT/SCHM. */
				sizeof(WdgM_AS_wakeup)/sizeof(WdgMAliveSupervisionType), /**< Total Number of SEs in this mode. */
				&WdgM_AS_wakeup[0],   		      	/**< Pointer to  configuration parameters of Alive-Supervision. */
				&WdgMActivationGpt,          		/**< Pointer to GPT Configuration for WDGM. */
				&WdgMTrigger_s               		/**< Pointer to Watchdog Configuration for WDGM. */
		},
		/* Mode:WDGM_STATE_APP_POST_RUN_MODE */
		{
				10, 								/**< WDG Trigger Tolerance after expiring. */
				1,          						/**< Total number of watchdogs. */
				WDGM_ACTIVATION_TYPE_SCHEDULER,     /**< Mode activation is GPT/SCHM. */
				sizeof(WdgM_AS_postrun)/sizeof(WdgMAliveSupervisionType), /**< Total Number of SEs in this mode. */
				&WdgM_AS_postrun[0],       			/**< Pointer to  configuration parameters of Alive-Supervision. */
				&WdgMActivationGpt,          		/**< Pointer to GPT Configuration for WDGM. */
				&WdgMTrigger_s               		/**< Pointer to Watchdog Configuration for WDGM. */
		},
		/* Mode:WDGM_STATE_SHUTDOWN_MODE */
		{
				10, 								/**< WDG Trigger Tolerance after expiring. */
				1,          						/**< Total number of watchdogs. */
				WDGM_ACTIVATION_TYPE_GPT,    		 /**< Mode activation is GPT/SCHM. */
				sizeof(WdgM_AS_shutdown)/sizeof(WdgMAliveSupervisionType), /**< Total Number of SEs in this mode. */
				&WdgM_AS_shutdown[0],       		/**< Pointer to  configuration parameters of Alive-Supervision. */
				&WdgMActivationGpt,          		/**< Pointer to GPT Configuration for WDGM. */
				&WdgMTrigger_s               		/**< Pointer to Watchdog Configuration for WDGM. */
		},
};

const WdgM_ConfigType wdgm_config  = {
		WDGM_STARTUP2_MODE,  				/**< Init Mode .*/
		WdgMMode    						/**< Mode pointer. */
		};


#define WDGM_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
