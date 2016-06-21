/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgM.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:29:41MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgM.h  $
 *  Revision 1.5 2011/03/22 09:29:41MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.4 2011/02/02 09:18:24MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/11/22 08:58:28MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:52:34MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:26:36MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/ServiceLayer/WdgM/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef WDGM_H
#define WDGM_H

#include "Std_Types.h"
#include "team-s.h"
#include "WdgM_Cfg.h"
#include "WdgIf.h" /**< TODO to be removed later as it gives lot of error in other drivers if removed!!!*/
#include "Gpt.h"  /**< TODO to be removed later as it gives lot of error in other drivers if removed!!!*/

/** @defgroup group_wdgm WdgM
 *  @ingroup group_wdgm */
/*@{*/

#define WDGM_MODULE_ID         (13)
#define WDGM_VENDOR_ID         (TEAMS_VENDOR_ID)
#define WDGM_AR_MAJOR_VERSION  (1)
#define WDGM_AR_MINOR_VERSION  (2)
#define WDGM_AR_PATCH_VERSION  (2)
#define WDGM_SW_MAJOR_VERSION  (1)
#define WDGM_SW_MINOR_VERSION  (0)
#define WDGM_SW_PATCH_VERSION  (0)



/* Driver APIs Service ID.
 * Short name in some macro length due to limit is # of character more then 32.
 */

#define WDGM_INIT_API_ID                    ((uint8)0x00)
#define WDGM_GETVERSIONINFO_API_ID          ((uint8)0x02)
#define WDGM_SETMODE_API_ID                 ((uint8)0x03)
#define WDGM_GETMODE_API_ID                 ((uint8)0x0B)
#define WDGM_UPDATEALIVECOUNTER_API_ID      ((uint8)0x04)
#define WDGM_ACTALIVESUPERVISION_API_ID     ((uint8)0x05)
#define WDGM_DEACTALIVESUPVISON_API_ID      ((uint8)0x06)
#define WDGM_GETALIVESUPVISSTAT_API_ID	    ((uint8)0x0C)
#define WDGM_GETGLOBALSTATUS_API_ID			((uint8)0x0D)
#define WDGM_CBKGPTNOTIFICATION_API_ID		((uint8)0x0A)
#define WDGM_MAINFUNCALIVESUPER_API_ID      ((uint8)0x08)
#define WDGM_MAINFUNCTRIGGER_API_ID         ((uint8)0x09)



/* Development Error Codes */
#define WDGM_E_NO_INIT                  ((uint8) 0x10) /**< A service was called without a prior wdgm_Init(). */
#define WDGM_E_PARAM_CONFIG             ((uint8) 0x11) /**< A wrong parameter to function wdgm_Init() was passed. */
#define WDGM_E_PARAM_MODE               ((uint8) 0x12) /**< Request mode is out of Range. */
#define WDGM_E_PARAM_SEID               ((uint8) 0x13) /**< Supervised Entity ID out of range. */
#define WDGM_E_NULL_POINTER     	    ((uint8) 0x14) /**< Service called with NULL pointer. */
#define WDGM_E_DISABLE_NOT_ALLOWED      ((uint8) 0x15) /**< WDG disable not allowed but requested. */
#define WDGM_E_DEACTIVATE_NOT_ALLOWED   ((uint8) 0x16) /**< Disabling of a particular SE not allowed. */
#define WDGM_E_ALIVE_SUPERVISION        ((uint8) 0x17) /**< AS failed. */

/**
 * Everything is OK.
 * The monitoring of this Supervised Entity has not
 * shown any failures. All underlying  Supervised Entities are
 * running within time Limit specified. This will be the default
 * state after initialization of WDGM or when WDGM is working fine.
 */
#define WDGM_ALIVE_OK           ((WdgM_AliveSupervisionStatusType)0x00)

/**
 * Some failure in one or many SEs, but can be "healed".
 * The monitoring of the Supervised Entity status /Global Status has failed but
 * can still be "healed". I.e., if the Supervised Entity(SE) returns to a
 * normal behavior, its monitoring state will also return to #WDGM_ALIVE_OK.
 * so as the global status of the WDGM. Furthermore, the number of times
 * that the monitoring has failed has not yet exceeded a configurable limit.
 * When this limit has been exceeded the state will change to #WDGM_ALIVE_EXPIRED.
 * This state is applicable to WDGM Supervised Entity status and WDGM Global status.
 */
#define WDGM_ALIVE_FAILED       ((WdgM_AliveSupervisionStatusType)0x01)

/**
 * No recovery possible for SE(s).
 * The monitoring of the Supervised Entity status /Global status of WDGM
 * has failed permanently(EXPIRED). This state cannot be left and go back to
 * #WDGM_ALIVE_OK at this point of time. But the Watchdog Driver has not yet been
 * instructed to stop triggering by the WDGM. If the WdgMFailedSupervisionRefCycleTol
 * is configured as 0, then the state will directly go from #WDGM_ALIVE_OK
 * to #WDGM_ALIVE_EXPIRED. This state is applicable to WDGM Supervised Entity status
 * and WDGM Global status.
 */
#define WDGM_ALIVE_EXPIRED      ((WdgM_AliveSupervisionStatusType)0x02)

/**
 * WDG reset Inevitable.
 * Monitoring has failed, the allowed limit of failures has been exceeded,
 * and the Watchdog Driver has been instructed to stop triggering by WDGM.
 * A Watchdog reset is about to happen. User may get the DEM error if configured.
 * Else, direct reset!.  This state is applicable ONLY to WDGM Global status.
 * If #WDGM_IMMEDIATE_RESET is activated, it will reset the system immediately!.
 */
#define WDGM_ALIVE_STOPPED      ((WdgM_AliveSupervisionStatusType)0x03)

/**
 * SE is disabled.
 * The monitoring of this Supervised Entity is temporarily disabled. It can be
 * activated by the interface #WdgM_UpdateAliveCounter .
 * This state is applicable ONLY to WDGM Supervised Entity Status.
 *
 */
#define WDGM_ALIVE_DEACTIVATED  ((WdgM_AliveSupervisionStatusType)0x04)

/**
 * SEId de activation is NOT allowed.
 * If any SE is configured with this option, a call to
 * #WdgM_DeactivateAliveSupervision will lead to
 * #WDGM_E_DEACTIVATE_NOT_ALLOWED error.
 * So these SEs can never be de activated.
 *
 */
#define WDGM_DEACTIVATION_ACCESS_DISABLED  ((uint8)0U)

/**
 * SEId de activation IS allowed.
 * If any SE is configured with this option, a call to
 * #WdgM_DeactivateAliveSupervision will de activate them, IF
 * the status of SE is #WDGM_ALIVE_OK. Else it will NOT
 * de activate the SE and no error will be raised. If the
 * SE is de activated by SW, it can also be
 * activated by the interface #WdgM_ActivateAliveSupervision.
 *
 */
#define WDGM_DEACTIVATION_ACCESS_ENABLED   ((uint8)1U)

/** @page wdgm WdgM
 *
 * @section wdgm_intro Introduction
 * This driver manages the underlying all watchdogs.
 *
 * In a conventional watchdog system. the watchdog is triggered at
 * regular interval. Failing to trigger will cause system reset.
 * The problem with conventional system is tight coupling of the software
 * component and watchdog timer. Also in some micros were no pre-warning
 * system is possible, watchdog reset will be a sudden surprise.
 * Also if more then few component timing has to be closely
 * monitored for watchdog triggering, conventional watchdog
 * doesn't give clean solution. These problems
 * were solved by Watchdog Manger Module of AutoSAR.
 * This WdgM module implements the Watchdog Manager functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_WatchdogManager.pdf">AUTOSAR_SWS_SWS_WatchdogManager</a>. \n
 *
 *
 * The main functionality of WDGM is to monitor the alive indications of all
 * Supervised Entity. The monitoring is done at every Alive Supervision Cycle. At every
 * Supervision Reference Cycle (number of pre defined Alive Supervision Cycle),
 * alive indications are checked against min and max range. If the deviation
 * is too much or too low, based on Failed Supervision Reference Cycle the
 * state of individual Supervised Entity changes. The Global Supervision Status
 *  is the status that summarizes the Local Monitoring Status of all Supervised Entities.
 * By this way all software components can be easily decoupled as specified in
 * below picture. Pre warning is possible if 1 or more then one SE is not
 * respecting the time line. Also it is possible to find out which one. With is
 * solution it is also possible to run the legasy system. Pl refer the AutoSAR
 * spec for more details. Following picture shows how the SE, WDGM & WDG will
 * interact with each other. Current solution doesn't support RTE. Refer the
 * limitation section of this document for more details.
 *
 * @image html  external_images/wdgm_overview.png "WDGM Overview"
 * @image latex external_images/wdgm_overview.png "WDGM Overview"
 *
 *
 * @subsection wdgm_nomen Nomenclature
 *
 * @subsubsection SEId Supervised Entity Identifier
 * Supervised Entity Identifier is any software component which will use
 * the watchdog manager service i.e A software entity which is
 * included in the monitoring of the Watchdog Manager.
 * Each Supervised Entity (SE) has unique identifier(SEId).
 * Refer the above picture for more details.
 *
 *
 * @subsection Alive_indication Alive Indication or Alive Counter(AC)
 * An indication provided by a Supervised Entity to signal
 * its aliveness to the Watchdog Manager. The indication is achieved by
 * WDGM interface WdgM_UpdateAliveCounter. Refer the above
 * picture for more details.
 *
 * @subsection se_status Supervised Entity Status
 * Supervision status is the status of each SE.
 * The Supervision Status state is maintained for each SE by WDGM.
 * These status is used by the WDGM to define the WDGM Global status.
 * The state could be any of the following.
 * @li #WDGM_ALIVE_OK.
 * @li #WDGM_ALIVE_FAILED.
 * @li #WDGM_ALIVE_EXPIRED.
 * @li #WDGM_ALIVE_DEACTIVATED.
 *
 * The transition between the sates are explained in below picture.
 * @image html  external_images/se_states.png "Supervised Entity-Status State Machine"
 * @image latex external_images/se_states.png "Supervised Entity-Status State Machine" width=14cm
 *
 *
 * @subsection g_status WDGM Global Monitoring Status
 * Status that summarizes the Local Monitoring Status of
 * all Supervised Entities.
 * The state could be any of the following.
 * @li #WDGM_ALIVE_OK.
 * @li #WDGM_ALIVE_FAILED.
 * @li #WDGM_ALIVE_EXPIRED.
 * @li #WDGM_ALIVE_STOPPED.
 *
 * The transition between the sates are explained in below picture.
 * @image html  external_images/global_states.png "Global Supervision-Status State Machine"
 * @image latex external_images/global_states.png "Global Supervision-Status State Machine" width=14cm
 *
 * @subsection alivesup Alive Supervision
 * Alive Supervision is the algorithm which checks if a Supervised Entity
 * executed sufficiently often and not too often (including
 * tolerances). This could be configured as GPT callback function or
 * Scheduler callback function. Refer the structure WdgMModeType
 * for more details.
 *
 * @subsection sup_cycle Supervision Cycle
 * The time period of Watchdog Manager, where the
 * cyclic Alive Supervision algorithm (Alive Supervision)
 * is performed.
 *
 * @subsection sup_ref_cycle Supervision Reference Cycle
 * The amount of Supervision Cycles to be used as
 * reference by the alive-supervision algorithm to
 * perform the check of counted Alive Indications
 * (individually for each Supervised Entity) against the range.
 *
 * @subsection wdg_trig_ref_cyc Watchdog Trigger Reference Cycle
 * Underlying Watchdog needs a regular trigger by the watchdog manager. The
 * Trigger reference cycle specifies how often the internal/external watchdog
 * has to be triggered. It is a good idea to keep the reference trigger as multiple
 * of supervision cycle time/call back time in case of GPT.
 *
 * @subsection failed_sup_ref_cycle Failed Supervision Reference Cycle
 * A Supervision Reference Cycle that ends with a
 * detected deviation (including tolerances) between the
 * Alive Counter and the expected amount of Alive
 * Indications. If it is 0, the state will switch to
 * #WDGM_ALIVE_EXPIRED where no recovery will be possible.
 *
 * @subsection init_mode Initial Mode
 * In Watchdog Manager a Mode is defined by a set of
 * configuration options. The set of Supervised Entities to be
 * supervised may vary from mode to mode. The config structure
 * type WdgMModeType has all details about the mode. It also
 * defines what should be the initial mode after WDGM initialization.
 *
 *
 * @section WdgM_autosar AutoSAR Conformity
 * The following limitations apply: \n
 * @li Refer the Section4 of the specification for limitations from AutoSAR Spec.
 * @li Current implementation doesn't implement any functionality
 *     related to RTE function calls/interactions.
 * @li Masking of production error code is not respected due to
 *     contradicting requirements WDGM129 Vs WDGM015 & WDGM022.
 * @li If the SeId passed is within range, then it should belong to running mode SeIds.
 * @li Cycle timing for #WdgM_MainFunction_AliveSupervison and #WdgM_MainFunction_Trigger are project
 *     specific and not a configurable item.
 * @li WdgMMinMargin & WdgMMaxMargin is not an offset from WdgMExpectedAliveIndications instead they are the
 *     absolute value to improve the driver efficiency. So WdgMExpectedAliveIndications is unused in the SW.
 *     This change doesn't have impact on functional requirement.
 * @li WDGMACTIVATIONACTIVATED is defined as WdgMActivationActivatedStatus for easy understanding.
 *     This change doesn't have impact on functional requirement.
 * @li Parallel usage of multiple watchdogs is not tested as the current HW doesn't have more then 1 WDG.
 *
 *
 * @section wdgm_cfg Configuration
 * @subsection wdgm_cfg_step_by_step Step by Step
 * @li step 1. Configure the internal and external watchdog (#Wdg_Int_ConfigType & #Wdg_ConfigType) of WDG Driver.
 * @li step 2. Configure GPT channel for WDGM call back  (#Gpt_ConfigChannelType).
 * @li step 3. Configure GPT channel & timeout reference inside WDGM (#WdgMActivationGptType).
 * @li step 4. Configure WDG Channel reference inside WDGM (#WdgMTriggerType).
 * @li step 5. Configure Supervision Entity(s) (#WdgMAliveSupervisionType).
 * @li step 6. Configure WDGM Mode(s) (#WdgMModeType).
 * @li step 7. Configure WDGM root structure (#WdgM_ConfigType) and point to mode(s).
 * @li step 8. Call the GPT initialization.
 * @li step 9. Call the WDG initialization.
 * @li step 10. Call the function WdgM_Init() with the pointer to the configured structure #WdgM_ConfigType
 *     in the startup phase.
 * @li step 11.Call Main function(s)in case of scheduler usage.
 * @li step 12.In case of GPT, it will be taken care by GPT call back.
 * @li step 13.Update alive counters based the min max values between supervision cycles to avoid resets.
 *
 *
 * Following picture shows the initialization sequence as explained above.
 * @image html  external_images/wdgm_init_sequence.png "WDGM Initialization sequence Diagram"
 * @image latex external_images/wdgm_init_sequence.png "WDGM Initialization sequence Diagram" width=14cm
 *
 * @subsection wdgm_cfg_note Note the following during configuration
 * @li Don't forget to initialize WDG,GPT before using WDGM
 * @li In case of GPT based usage,WDGM_ALIVE_SUPERVISION switch will decide
 *     either you need supervision/Not But it is compile time switch.
 * @li In case of Scheduled based service, WdgM_MainFunction_AliveSupervision should be
 *     called before WdgM_MainFunction_Trigger to do the alive supervision (if enabled).
 * @li "WdgM_Cbk_GptNotification" has to be configured in GPT if call back function is used for WDGM.
 *
 * @subsection wdgm_cfg_by_example Example
 *
 * The example shows the configuration of two SEs.
 * WDG is set to run is slow mode. GPT callback is used.
 *
 *
 * Example configuration of WDG Driver:
 * @code
 *
 *
 * static const WdgMTriggerType WdgMTrigger_s = {
 *		0,                              // Watchdog ID to be used in WdgIf interfaces.
 *		75,              	            // trig reg cycle set to 75
 *		WDGIF_SLOW_MODE                 // Mode of the WatchDog as SLOW.
 * };
 *
 *
 * static const WdgMActivationGptType WdgMActivationGpt = {
 * 		WDGM_GPT_CH_ID,       	        // GPT Channel Used by WDDGM.
 *		500                   	        // GPT Channel Timeout Period.
 * };
 *
 *
 * static const WdgMAliveSupervisionType WdgM_AS_startup2[] = {
 *	 {
 *	  0,                                //SE's ID
 *	  WDGM_SUPERVISION_ENABLED,         // This SEId's supervision is enabled
 *    WDGM_DEACTIVATION_ACCESS_ENABLED, // No deactivation is possible for this SE
 *    5,                                // 5 failed SC is possible to recover
 *    55,                               // max 55 alive indications are OK
 *    45,                               // min 45 alive indications are OK
 *    5                                 // after 5 cycles, alive indications will be checked
 *   },
 *
 *	 {
 *	  1,                                //SE's ID
 *	  WDGM_SUPERVISION_ENABLED,         // This SEId's supervision is enabled
 *    WDGM_DEACTIVATION_ACCESS_DISBLED, // Deactivation is possible for this SE
 *    2,                                // 2 failed SC is possible to recover
 *    75,                               // max 75 alive indications are OK
 *    68,                               // min 68 alive indications are OK
 *    7                                 // after 7 cycles, alive indications will be checked
 *    },
 *
 * };
 *
 *
 * static const WdgMModeType WdgMMode[] = {
 *		                                 // Mode:0
 *		{10, 	                         // WDG Trigger Tolerance after expiring is 10 SCs
 *		 1,                              // 1 watchdog is configured
 *		 WDGM_ACTIVATION_TYPE_GPT,       // Mode activation is GPT
 *		 2,                              //2 SEs configured in this mode
 *		 &WdgM_AS_startup2[0],           //Pointer to  Alive-Supervision
 * 		 &WdgMActivationGpt,             //Pointer to GPT Configuration
 *		 &WdgMTrigger_s                  //Pointer to Watchdog Configuration
 *		},
 *
 *};
 *
 *
 * const WdgM_ConfigType wdgm_config  = {
 *		0,  			                //Init Mode
 *		WdgMMode    	                //Mode pointer
 *		};
 *
 * @endcode
 *
 *
 * Usage of the above:
 * @code
 *
 * void init(void)
 * {
 *   ...
 *   Gpt_Init(&gpt_config);
 *   Wdg_Init(&wdg_config);
 *   WdgM_Init(&wdgm_config);
 *   ...
 *   //some other steps
 *   ...
 *   //change the mode if necessary
 *   ...
 * }
 * @endcode
 * @subsection example_discription Example Description
 *
 * With the above configuration,
 * @li Alive indication for SEID 0 should be given at least 10 times between each AS.
 * @li Alive indication for SEID 1 should be given at least 5 times between each AS.
 * @li If AS is done every 10ms,
 *   - The AI update for SEID0 can be done every 1ms.
 *   - The AI update for SEID1 can be done every 2ms.
 * @li At the end of 5 AS cycle, if SEID0 is >45 or <35 error is detected for SEID0.
 * @li At the end of 5 AS cycle, if SEID0 is >38 or <30 error is detected for SEID1.
 * @li If the error persist for 3 such cycle in case of SEID0 or 5 cycle in case of SEID1
 * then global mode changes and no recovery possible.
 * @li But it will wait for 10 AS cycle(tolerance level) and system reset will occur.
 * @li User can check the global status to do house keeping work in case of error.
 * @li Also user can enable/disable based on which SEID he want to monitor.
 * @li Note: WDG trigger cycle and GPT timeout has to  be decided based on AS cycle time+ tolerance level.
 *
 *
 * @code
 * void foo_1ms(void){
 * ...
 * ...
 *  //Somewhere in code at every 1ms intervals
 *  WdgM_UpdateAliveCounter(0);
 *  ...
 *  ...
 * }
 *
 * void foo_2ms(void){
 *  ...
 *  ...
 *  //Somewhere in code at every 2ms intervals
 *  WdgM_UpdateAliveCounter(1);
 *  ...
 *  ...
 *  }
 *
 * @endcode
 *
 */




typedef uint8 WdgM_ModeType ;  /**< Denotes the WdgM Mode. */

#if defined(WDGM_NUMBER_OF_SUPERVISED_ENTITIES) && ( WDGM_NUMBER_OF_SUPERVISED_ENTITIES > 255u )
typedef uint16 WdgM_SupervisedEntityIdType; /**< Supervision Entity ID Type as uint16. */
#else
typedef uint8 WdgM_SupervisedEntityIdType;  /**< Supervision Entity ID Type as uint8. */
#endif
typedef uint8 WdgM_AliveSupervisionStatusType; /**< Alive Supervision Status. */

# define WDGM_ACTIVATION_TYPE_GPT        (0x00U) /***< GPT based Activation. */
# define WDGM_ACTIVATION_TYPE_SCHEDULER  (0x01U) /***< Scheduler based Activation. */

/** SEId supervision Status.
 * This type is used for the specification of the activation of
 * SE as either the supervision is enabled or not.
 *
 */

typedef enum {
    WDGM_SUPERVISION_ENABLED, /**< Supervision Enabled. */
    WDGM_SUPERVISION_DISABLED /**< Supervision Disabled. */
} WdgM_ActivationStatusType;

/** Type for the configuration of the Supervision Entity.
 *
 * The channel configuration holds all properties linked to a SE. These properties
 * shell be configurable for each SE. So it shell be made as array of structures.
 */
typedef struct {
		WdgM_SupervisedEntityIdType WdgMSupervisedEntityId;      /**< Supervision Entity Id. */
		WdgM_ActivationStatusType WdgMActivationActivatedStatus; /**< Denotes this SEId is supervision enabled/disabled. */
		uint8 WdgMDeactivationAccessEnabled;                     /**< Supervision de activation possible / not. */
		uint16 WdgMFailedSupervisionRefCycleTol;                 /**< Acceptable amount of failed AS for this SE. */
		uint16 WdgMMaxMargin; 					                 /* Allowed number of additional executions of the SE. */
		uint16 WdgMMinMargin; 					                 /**< Allowed number of missing executions of the SE. */
		uint16 WdgMSupervisionReferenceCycle;                    /**< SCs used by AS mechanism to perform check up. */
} WdgMAliveSupervisionType;

/** Type for the configuration of the Watchdog.
 *
 * The channel configuration holds all properties linked to a Watchdog.
 * These properties shell be configurable for each Watchdog.
 * So it shell be made as array of structures if more watchdogs to be used.
 *
 * Based on the WDGM Mode the underlying WDG mode shell be configured.
 */
typedef struct {
		uint8 			DeviceIndex;               /**< Watchdog ID to be used in WdgIf interfaces. */
		uint16			WdgMTriggerReferenceCycle; /**< How often the WatchDog to be triggered. */
		WdgIf_ModeType	WdgMWatchdogMode;          /**< Mode of the WatchDog. */
}WdgMTriggerType;

/** Type for the configuration of the GPT channel.
 *
 * The GPT channel is used to with cycle time to callback WDGM interfaces.
 * Configuration in GPT driver need to be taken care for the specified channel.
 *
 */
typedef struct {
		Gpt_ChannelType	   WdgMGptChannelRef; /**< GPT Channel Used by WDDGM.  */
		Gpt_ValueType	   WdgMGptCycle;      /**< GPT Channel Timeout Period. */
} WdgMActivationGptType;

/** Type for the configuration of the WDGM Mode.
 *
 * The mode configuration holds all properties linked to a WDGM Mode.
 * These properties shell be applicable for all SEs configured under this mode.
 * When more then one mode to be used for WDGM, it shell be made as array of
 * structures.
 *
 */

typedef struct {
		uint8 						WdgMExpiredSupervisionCycleTol; /**< WDG Trigger Tolerance after expiring. */
		uint8 						WdgMNumberOfWatchdogs;          /**< Total number of watchdogs. */
		uint8 						WdgMActivationType;             /**< Mode activation is GPT/SCHM. */
		uint16 						WdgMNumberOfEntities;           /**< Total Number of SEs in this mode. */
		const WdgMAliveSupervisionType	*WdgMAliveSupervisionPtr;       /**< Pointer to  configuration parameters of Alive-Supervision. */
		const WdgMActivationGptType		*WdgMActivationGptPtr;          /**< Pointer to GPT Configuration for WDGM. */
		const WdgMTriggerType           *WdgMTriggerPtr;                /**< Pointer to Watchdog Configuration for WDGM. */
} WdgMModeType;

/** Type for the configuration of WDGM main structure.
 * It has the initial mode in which WDGM should run after
 * initialization and pointer to all related config structures.
 */
typedef struct {
		WdgM_ModeType 	WdgMInitialMode;     /**< The mode that the WDGM after initialized.*/
		const WdgMModeType	*WdgMModePtr;    /**< Pointer to the mode Containers of WDGM. */
}  WdgM_ConfigType;


#define WDGM_START_SEC_CODE
#include "MemMap.h"

/** This Interface Initializes the Watchdog Manager.
 * After execution of this function, monitoring is activated according to the
 * list of Supervised Entities defined in the initial mode in configuration structure.
 *
 * @note After the execution of the function WdgM_Init(),
 * the configuration data are accessible and can be used by
 * the WDGM module functions.
 *
 * @param ConfigPtr   Pointer to the configuration.
 */
extern void WdgM_Init(const WdgM_ConfigType *ConfigPtr);

/** This interface Sets the current mode of Watchdog Manager to the passed one.
 * Status of individual SEs are unchanged during the Mode Change.
 *
 * @param  Mode    Mode which needs to be set.
 *
 * @return  #E_OK: Successfully changed to the new mode. \n
 *          #E_NOT_OK: Changing to the new mode failed.
 */
extern Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode);

/** This interface Returns the current running mode of the Watchdog Manager.
 *
 * @param  Mode    Current Mode of WdgM.
 * @return  #E_OK: Current mode successfully returned. \n
 *          #E_NOT_OK: Returning current mode failed.
 *
 * @note    WdgM_GetMode service returns the currently active mode
 *  of the Watchdog Manager. If the WdgM_SetMode service is active while
 *  this service is called, WdgM_GetMode will return the previously active
 *  mode as long as the new mode has not been completely activated!.
 *
 */
extern Std_ReturnType WdgM_GetMode(WdgM_ModeType* Mode);

/** Callback function of WdgM in case of GPT usage.
 *  This interface needs to be a call back function of GPT in order to service WDGM.
 *  During the powerup/ shutdown, when the alive supervision is not required/configured,
 *  this interface will only service the WDGs.
 *
 */
extern void WdgM_Cbk_GptNotification(void);

/** This interface will Trigger all the configures WDGs.
 *  This interface will trigger the WDGs only if the
 *  activation source is Schedule Manager.
 *
 *  @note This interface shell be used when no WDGM functionality
 *  is required. In case GPT is used for triggering, then the interface
 *  WdgM_Cbk_GptNotification could be used for triggering without
 *  WDGM scheduling functionality.
 */

extern void WdgM_MainFunction_Trigger(void);

/** This interface will update the Alive Indicator(AI) of the passed SEId.
 *  In case if the SEId is deactivated before, it skips the update.
 *
 *  @param  SEId    Supervised Entity ID.
 *  @return  #E_OK: Alive counter successfully updated. \n
 *           #E_NOT_OK: Alive counter update failed.
 *  @note This interface will also check the SEId is in range and
 *  raise error if it is not. Normally this interface will be called
 *  at the regular interval in order to update the SE's Alive Indicators(AI).
 *
 */
extern Std_ReturnType WdgM_UpdateAliveCounter(WdgM_SupervisedEntityIdType SEId);

/** This interface will activate the Aliveness monitoring for the passed SEId.
 *
 *  @param  SEId    Supervised Entity ID.
 *  @return  #E_OK: Aliveness monitoring successfully activated. \n
 *           #E_NOT_OK: Aliveness monitoring not activated.
 *
 *  @note This interface will also check the SEId is in range and
 *  raise error if it is not. If it is already activated also this
 *  interface will return #E_OK.
 *
 */
extern Std_ReturnType WdgM_ActivateAliveSupervision(WdgM_SupervisedEntityIdType SEId);

/** This interface will de-activate the Aliveness monitoring for the passed SEId.
 *
 *  @param  SEId    Supervised Entity ID.
 *  @return  #E_OK: Aliveness monitoring successfully de-activated. \n
 *           #E_NOT_OK: Aliveness monitoring not de-activated.
 *
 *  @note This interface will also check the SEId is in range/de-activation allowed
 *  / SE is active and raise error if it is not. If it is already
 *  de-activated also this  interface will return #E_OK. In case de-activation
 *  is not allowed for the passed SE, DET error is raised.
 *
 */
extern Std_ReturnType WdgM_DeactivateAliveSupervision(WdgM_SupervisedEntityIdType SEId);

/** This interface will return Alive supervision status on the passed SEId.
 *
 *  @param  SEId    Supervised Entity ID.
 *  @param  Status  Pointer were the status to be returned.
 *  @return  #E_OK: Status successfully returned. \n
 *           #E_NOT_OK: Status not returned.
 *
 */
extern Std_ReturnType WdgM_GetAliveSupervisionStatus(WdgM_SupervisedEntityIdType SEId, WdgM_AliveSupervisionStatusType* Status);


/** This interface will Alive supervision status on the WDGM.
 *
 *  @param  Status  Pointer were the status to be updated.
 *  @return  #E_OK: Status successfully returned. \n
 *           #E_NOT_OK: Status not returned.
 *
 */
extern Std_ReturnType WdgM_GetGlobalStatus(WdgM_AliveSupervisionStatusType* Status);

/** This interface does the alive supervision of WDGM Module.
 *  When alive counters doesn't match within expected limit,
 *  this interface will stop servicing the WDGs which will cause the reset.
 *
 */
extern void WdgM_MainFunction_AliveSupervision(void);


/** The function WdgM_GetVersionInfo returns the version information of the WdgM module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function WdgM_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter WdgMVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void WdgM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = WDGM_MODULE_ID;
	versioninfo->sw_major_version = WDGM_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = WDGM_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = WDGM_SW_PATCH_VERSION;
	versioninfo->vendorID = WDGM_VENDOR_ID;
}

#define WDGM_STOP_SEC_CODE
#include "MemMap.h"

/*@}*/

#endif /* WDGM_H_ */



