/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:25:35MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM.h  $
 *  Revision 1.5 2011/03/22 09:25:35MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.4 2010/11/24 13:08:19MEZ Opel Martin RGB (OPELM) 
 *  - doxygen stuff
 *  Revision 1.3 2010/11/22 07:58:26MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:20:45MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef ECUM_H_
#define ECUM_H_

#include "team-s.h"
#include "Std_Types.h"
#include "ComStack_Types.h"
#include "OSEK_Types.h"
#include "NvM.h"

/** @defgroup group_ecum_dev_errors EcuM Development Error Codes
 * @ingroup group_ecum
 */
/*@{*/

#define ECUM_MODULE_ID         (10)
#define ECUM_VENDOR_ID         (TEAMS_VENDOR_ID)
#define ECUM_AR_MAJOR_VERSION  (2)
#define ECUM_AR_MINOR_VERSION  (2)
#define ECUM_AR_PATCH_VERSION  (2)
#define ECUM_SW_MAJOR_VERSION  (1)
#define ECUM_SW_MINOR_VERSION  (0)
#define ECUM_SW_PATCH_VERSION  (0)

#define ECUM_E_NOT_INITED						((uint8)(0x10))	/**< A service was called without a prior EcuM_Init(). */
#define ECUM_E_SERVICE_DISABLED					((uint8)(0x11))	/**< A service was called which was disabled by configuration. */
#define ECUM_E_NULL_POINTER 					((uint8)(0x12))	/**< A null pointer was passes as an argument. */
#define ECUM_E_INVALID_PAR   					((uint8)(0x13))	/**< A parameter was invalid. */
#define ECUM_E_MULTIPLE_RUN_REQUESTS			((uint8)(0x14))	/**< RUN was requested multiple times by the same id. */
#define ECUM_E_MISMATCHED_RUN_RELEASE			((uint8)(0x15))	/**< RUN was released though it was not requested. */
#define ECUM_E_STATE_PAR_OUT_OF_RANGE			((uint8)(0x16))	/**< A state passed to a service was out of range. */
#define ECUM_E_UNKOWN_WAKEUP_SOURCE				((uint8)(0x17))	/**< An unknown wakeup source was passed as parameter to an API. */
#define ECUM_E_NVMRAM_MGR_WRITEALL_TIMEOUT      ((uint8)(0x20)) /**< A time out during writing the NVRAM data occurred. */
#define ECUM_E_NVMRAM_MGR_READALL_TIMEOUT       ((uint8)(0x21)) /**< A time out during reading the NVRAM data in GO OFF I occurred. */
/*@}*/

/** @defgroup group_ecum EcuM */
/*@{*/
/* Driver APIs Service ID */
#define ECUM_INIT_API_ID             	         ((uint8)0x00)
#define ECUM_GETVERSIONINFO_API_ID		         ((uint8)0x09)
#define ECUM_STARTUP_TWO_API_ID                  ((uint8)0x1a)
#define ECUM_SHUTDOWN_API_ID			         ((uint8)0x02)
#define ECUM_GET_STATE_API_ID                    ((uint8)0x07)
#define ECUM_REQUEST_RUN_API_ID                  ((uint8)0x03)
#define ECUM_RELEASE_RUN_API_ID                  ((uint8)0x04)
#define ECUM_COMM_RELEASE_RUN_API_ID             ((uint8)0x10)
#define ECUM_COMM_REQUEST_RUN_API_ID             ((uint8)0x0e)
#define ECUM_COMM_HAS_REQUESTED_RUN_API_ID       ((uint8)0x1b)
#define ECUM_REQUEST_POST_RUN_API_ID             ((uint8)0x0a)
#define ECUM_RELEASE_POST_RUN_API_ID             ((uint8)0x0b)
#define ECUM_KILLALL_RUN_REQUESTS_API_ID         ((uint8)0x05)
#define ECUM_SELECT_SHUTDOWN_TARGET_API_ID       ((uint8)0x06)
#define ECUM_GET_SHUTDOWN_TARGET_API_ID          ((uint8)0x09)
#define ECUM_GET_LAST_SHUTDOWN_TARGET_API_ID     ((uint8)0x08)
#define ECUM_GET_PENDING_WAKEUP_EVENTS_API_ID    ((uint8)0x0D)
#define ECUM_CLEAR_WAKEUP_EVENT_API_ID           ((uint8)0x16)
#define ECUM_GET_VALIDATED_WAKEUP_EVENTS_API_ID  ((uint8)0x15)
#define ECUM_GET_EXPIRED_WAKEUP_EVENTS_API_ID    ((uint8)0x19)
#define ECUM_GET_STATUS_OF_WAKEUP_SOURCE_API_ID  ((uint8)0x17)
#define ECUM_SET_APPLICATION_MODE_API_ID         ((uint8)0x0f)
#define ECUM_GET_APPLICATION_MODE_API_ID         ((uint8)0x11)
#define ECUM_SELECT_BOOT_TARGET_API_ID           ((uint8)0x12)
#define ECUM_GET_BOOT_TARGET_API_ID              ((uint8)0x13)
#define ECUM_MAIN_FUNCTION_API_ID                ((uint8)0x18)
#define ECUM_CB_NFYNVM_JOB_END_API_ID            ((uint8)0x65)
#define ECUM_SET_WAKEUP_EVENT_API_ID             ((uint8)0x0c)
#define ECUM_VALIDATE_WAKEUP_EVENT_API_ID        ((uint8)0x14)


/** @page ecum EcuM
 *
 * @section ecum_intro Introduction
 * The EcuM service implements the EcuM serviceality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_ECU_StateManager.pdf">AUTOSAR_SWS_ECU_StateManager</a>.
 *
 *
 * @section ecum_init Initialization of EcuM
 * Before any service of the EcuM can be used the initialization service EcuM_Init().
 *
 * @section ecum_autosar AutoSAR Conformity
 * EcuM implements all services defined by AutoSAR beside the following exceptions:
 * @li The WAKEUP features and states are not supported. The frame and interfaces are added with an empty implementation.
 * @li The SLEEP features and states not supported. The frame and interfaces are added with an empty implementation.
 * @li The consistency checking of the configuration data 'EcuMConfigConsistencyHash' is not implemented.
 * @li Additional development error codes #ECUM_E_NVMRAM_MGR_WRITEALL_TIMEOUT and #ECUM_E_NVMRAM_MGR_READALL_TIMEOUT are implemented.
 *
 * @section ecum_cfg Configuration
 * @subsection ecum_cfg_step_by_step Step by Step
 * @li Configure the \ref ecum_pct_config.
 * @li Configure the \ref ecum_pbt_config.
 * @li Call the service EcuM_Init() in the startup phase of the ECU. The ECU State Manager assumes, that before EcuM_Init is called a minimal
 *      initialization of the microcontroller has taken place, so that a stack is set up and code can be executed.
 *
 *
 * @subsection ecum_cfg_by_example Post Build Time Configuration Example
 *
 * \include EcuM_PBCfg.c
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   EcuM_Init();
 *   ...
 * }
 * @endcode
 *
 */

#define ECUM_SUBSTATE_MASK           ((EcuM_StateType)0x0f) /**< Mask to decode the sub state of the ecu state.*/
#define ECUM_STATE_STARTUP           ((EcuM_StateType)0x10) /**< ecu state startup. */
#define ECUM_STATE_STARTUP_ONE       ((EcuM_StateType)0x11) /**< ecu state startup sub state one.*/
#define ECUM_STATE_STARTUP_TWO       ((EcuM_StateType)0x12) /**< ecu state startup sub state two.*/
#define ECUM_STATE_WAKEUP            ((EcuM_StateType)0x20) /**< ecu state wakeup.*/
#define ECUM_STATE_WAKEUP_ONE        ((EcuM_StateType)0x21) /**< ecu state wakeup sub state one.*/
#define ECUM_STATE_WAKEUP_VALIDATION ((EcuM_StateType)0x22) /**< ecu state wakeup sub state validation.*/
#define ECUM_STATE_WAKEUP_REACTION   ((EcuM_StateType)0x23) /**< ecu state wakeup sub state reaction.*/
#define ECUM_STATE_WAKEUP_WAKESLEEP  ((EcuM_StateType)0x25) /**< ecu state wakeup sub state wake sleep.*/
#define ECUM_STATE_WAKEUP_TWO        ((EcuM_StateType)0x24) /**< ecu state wakeup sub state two.*/
#define ECUM_STATE_WAKEUP_TTII       ((EcuM_StateType)0x26) /**< ecu state wakeup sub state TTII.*/
#define ECUM_STATE_RUN               ((EcuM_StateType)0x30) /**< ecu state run.*/
#define ECUM_STATE_APP_RUN           ((EcuM_StateType)0x32) /**< ecu state run sub state application.*/
#define ECUM_STATE_APP_POST_RUN      ((EcuM_StateType)0x33) /**< ecu state run sub state application post run.*/
#define ECUM_STATE_SHUTDOWN          ((EcuM_StateType)0x40) /**< ecu state shutdown.*/
#define ECUM_STATE_PREP_SHUTDOWN     ((EcuM_StateType)0x44) /**< ecu state shutdown sub state prepare shutdown.*/
#define ECUM_STATE_GO_SLEEP          ((EcuM_StateType)0x49) /**< ecu state shutdown sub state go sleep.*/
#define ECUM_STATE_GO_OFF_ONE        ((EcuM_StateType)0x4d) /**< ecu state shutdown sub state go off one.*/
#define ECUM_STATE_GO_OFF_TWO        ((EcuM_StateType)0x4e) /**< ecu state shutdown sub state go off two.*/
#define ECUM_STATE_SLEEP             ((EcuM_StateType)0x50) /**< ecu state sleep.*/
#define ECUM_STATE_OFF               ((EcuM_StateType)0x80) /**< ecu state off.*/
#define ECUM_STATE_RESET             ((EcuM_StateType)0x90) /**< ecu state reset.*/



/** Data type for the ecu state manager types.
 *  @li states are encoded in the hi-nibble,
 *  @li sub-state in the lo-nibble.
 *  @li the sub-state can be determined by ANDing the state value with \ref ECUM_SUBSTATE_MASK.
 *
 * @image latex external_images/ecum_statetype.pdf "EcuM StateType"
 */
/* @dot
 *   digraph EcuM_StateType {
 *     node [shape=plaintext fontsize="10.0"];
 *     EcuM_StateType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="type" BGCOLOR="grey" colspan="8">EcuM_StateType</td></tr>
 *                         <tr><td PORT="port" colspan="4">state</td>
 *                             <td PORT="pin" colspan="4">sub state</td>
 *                         </tr>
 *                         <tr><td PORT="MSB" >MSB</td>
 *                             <td PORT="b6" >6</td>
 *                             <td PORT="b5" >5</td>
 *                             <td PORT="b4" >4</td>
 *                             <td PORT="b3" >3</td>
 *                             <td PORT="b2" >2</td>
 *                             <td PORT="b1" >1</td>
 *                             <td PORT="LSB" >LSB</td>
 *                          </tr>
 *       </table>>];
 *
 *   }
 * @enddot
 */
typedef uint8 EcuM_StateType;

/**
 * Type definition to define wakeup sources.
 *
 * The bitfield provides one bit for each wakeup source.
 * In WAKEUP state, all bits cleared indicates that no wakeup source is known.
 * In STARTUP state, all bits cleared indicates that no reason for restart or reset is
 * known. In this case, \ref ECUM_WKSOURCE_RESET shall be assumed.
 *
 * @see \ref ECUM_WKSOURCE_INTERNAL_RESET
 * @see \ref ECUM_WKSOURCE_EXTERNAL_WDG
 * @see \ref ECUM_WKSOURCE_INTERNAL_WDG
 * @see \ref ECUM_WKSOURCE_POWER
 * @see \ref ECUM_WKSOURCE_ALL_SOURCES
 * @see \ref ECUM_WKSOURCE_RESET
 *
 */
typedef uint32 EcuM_WakeupSourceType;

/**
 *  Definition to indicate an internal reset of uC (bit 2).
 *  The internal reset typically only resets the uC
 *  core but not peripherals or memory controllers. The exact behavior is hardware
 *  specific.This source may also indicate an not handled exception.
 *
 *  @see EcuM_WakeupSourceType
 *
 */
#define ECUM_WKSOURCE_INTERNAL_RESET ((EcuM_WakeupSourceType)0x00000004u)

/**
 *  Definition to indicate an reset by the external watchdog (bit 4).
 *
 *  @see EcuM_WakeupSourceType
 */
#define ECUM_WKSOURCE_EXTERNAL_WDG ((EcuM_WakeupSourceType)0x00000010u)

/**
 *  Definition to indicate an reset by the internal watchdog (bit 3).
 *
 *  @see EcuM_WakeupSourceType
 */
#define ECUM_WKSOURCE_INTERNAL_WDG ((EcuM_WakeupSourceType)0x00000008u)

/**
 *  Definition to indicate power cycle (bit 0).
 *
 *  @see EcuM_WakeupSourceType
 */
#define ECUM_WKSOURCE_POWER ((EcuM_WakeupSourceType)0x00000001u)

/**
 * @todo Description of ECUM_WKSOURCE_ALL_SOURCES missing.
 */
#define ECUM_WKSOURCE_ALL_SOURCES (ECUM_WKSOURCE_INTERNAL_RESET | \
		                           ECUM_WKSOURCE_EXTERNAL_WDG   | \
		                           ECUM_WKSOURCE_INTERNAL_WDG   | \
		                           ECUM_WKSOURCE_POWER          | \
		                           ECUM_WKSOURCE_RESET           )

/**
 *  Definition to indicate a hardware reset (bit 1).
 *
 *  If hardware cannot distinguish between a power cycle and a reset reason, then this
 *  is the default wakeup source.
 *
 *  @see EcuM_WakeupSourceType
 */
#define ECUM_WKSOURCE_RESET ((EcuM_WakeupSourceType)0x00000002)

/*
 * wakeup status definitions
 */
#define ECUM_WKSTATUS_NONE       ((EcuM_WakeupStatusType) 0) /**< No pending wakeup event was detected. */
#define ECUM_WKSTATUS_PENDING    ((EcuM_WakeupStatusType) 1) /**< The wakeup event was detected but not yet validated. */
#define ECUM_WKSTATUS_VALIDATED  ((EcuM_WakeupStatusType) 2) /**< The wakeup event is valid. */
#define ECUM_WKSTATUS_EXPIRED    ((EcuM_WakeupStatusType) 3) /**< The wakeup event has not been validated and has expired therefore. */


/*
 * wakeup reaction definitions
 */
#define ECUM_WKACT_RUN      ((EcuM_WakeupReactionType)0) /**< Initialization into RUN state. */
#define ECUM_WKACT_TTII     ((EcuM_WakeupReactionType)2) /**< Execute time triggered increased in operation protocol and shutdown. */
#define ECUM_WKACT_SHUTDOWN ((EcuM_WakeupReactionType)3) /**< Immediate shutdown. */

/*
 * boot target types
 */
#define ECUM_BOOT_TARGET_APP        ((EcuM_BootTargetType)0) /**< The ECU will boot into the application. */
#define ECUM_BOOT_TARGET_BOOTLOADER ((EcuM_BootTargetType)1) /**<  The ECU will boot into the bootloader. */

/**
 * The type describes possible boot target types.
 */
typedef uint8 EcuM_BootTargetType;


/**
 * The type describes the possible outcomes of the \ref ECUM_STATE_WAKEUP_REACTION state.
 *
 */
typedef uint8 EcuM_WakeupReactionType;


/**
 * The type describes the possible outcomes of the \ref ECUM_STATE_WAKEUP_VALIDATION state. The
 * type is be applied to one wakeup source or a collection of wakeup sources.
 *
 */
typedef uint8 EcuM_WakeupStatusType;

/**
 * Type definition to define unique values for each user.
 */
typedef uint8 EcuM_UserType;


#define ECUM_START_SEC_CODE
#include "MemMap.h"

/** This service initializes the EcuM driver.
 *
 * The service EcuM_Init initializes the EcuM module.
 *
 * @note After the execution of the service EcuM_Init(), the configuration data are accessible
 * and can be used by the EcuM module services.
 */
extern void EcuM_Init(void);



/** The service EcuM_GetVersionInfo returns the version information of the EcuM module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the service EcuM_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter EcuMVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline service
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void EcuM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = ECUM_MODULE_ID;
	versioninfo->sw_major_version = ECUM_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = ECUM_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = ECUM_SW_PATCH_VERSION;
	versioninfo->vendorID = ECUM_VENDOR_ID;
}

/**
 * This service implements the STARTUP II state \ref ECUM_STATE_STARTUP_TWO.
 *
 * @note This service must be called from a task which is started directly as a
 * consequence of StartOS. I.e. either it must be called from an autostart task or it must
 * be called from a task which is explicitly started.
 *
 */
extern void EcuM_StartupTwo(void);

/**
 * This service takes over execution control and will carry out GO OFF II activities at shutdown.
 *
 */
extern void EcuM_Shutdown(void);

/**
 * This service returns a copy of the internal ecu state.
 * @param state The copy of the current ecu state.
 * @return E_OK: The out parameter was set successfully, E_NOT_OK: The out parameter was not set.
 *
 */
extern Std_ReturnType EcuM_GetState(EcuM_StateType* state);

/**
 * This service sets a request for the RUN state.
 *
 * @param user ID of the entity requesting the RUN state.
 * @return E_OK: The request was accepted by EcuM, E_NOT_OK: The request was not accepted by EcuM, a detailed error condition was sent to DET.
 *
 * @note Requests can be placed by every user made known to the state manager at configuration time.
 *
 */
extern Std_ReturnType EcuM_RequestRUN(EcuM_UserType user);

/**
 * This service releases a RUN request previously done with a call to EcuM_RequestRUN.
 *
 * @param user ID of the entity releasing the RUN state.
 * @return E_OK: The release request was accepted by EcuM, E_NOT_OK: The release request was not accepted by EcuM, a detailed error condition was sent to DET.
 *
 * @note The service is intended for implementing AUTOSAR ports.
 *
 */
extern Std_ReturnType EcuM_ReleaseRUN(EcuM_UserType user);

/**
 * This service set a request for the RUN state of an communication channel.
 *
 * @param channel ID of the communication channel requesting the RUN state.
 * @return E_OK: The request was accepted by EcuM, E_NOT_OK: The request was not accepted by EcuM, a detailed error condition was sent to DET.
 *
 * @note The behavior is identical to EcuM_RequestRUN except that the parameter is not a user but a communication channel.
 *
 */
extern Std_ReturnType EcuM_ComM_RequestRUN(NetworkHandleType channel);

/**
 * This service releases a request for the RUN state of an communication channel.
 *
 * @param channel ID of the communication channel releasing the RUN state.
 * @return E_OK: The release request was accepted by EcuM, E_NOT_OK: The release request was not accepted by EcuM, a detailed error condition was sent to DET.
 *
 * @note Releases a RUN request previously done with a call to \ref EcuM_ComM_RequestRUN.
 *
 */
extern Std_ReturnType EcuM_ComM_ReleaseRUN(NetworkHandleType channel);

/**
 * This service returns, if a ComM channel has requested RUN state.
 *
 * @param channel ID of the communication channel being tested.
 * @return TRUE: The channel has requested RUN state, FALSE: The channel has not requested RUN state.
 *
 */
extern boolean EcuM_ComM_HasRequestedRUN(NetworkHandleType channel);


/**
 * This service places a request for the POST RUN state.
 *
 * @param user ID of the entity requesting the POST RUN state.
 * @return E_OK: The request was accepted by EcuM, E_NOT_OK: The request was not accepted by EcuM, a detailed error condition was sent to DET.
 *
 * @note Requests can be placed by every user made known to the state manager at configuration time.
 */
extern Std_ReturnType EcuM_RequestPOST_RUN(EcuM_UserType user);

/**
 * This service releases a POST RUN request previously done with a call to \ref EcuM_RequestPOST_RUN.
 *
 * @param user ID of the entity releasing the POST RUN state.
 * @return E_OK: The release request was accepted by EcuM, E_NOT_OK: The release request was not accepted by EcuM, a detailed error condition was sent to DET.
 */
extern Std_ReturnType EcuM_ReleasePOST_RUN(EcuM_UserType user);

/**
 * The service unconditionally clears all requests to RUN and POST RUN.
 *
 * @note The benefit of this service over an ECU reset is that the shutdown sequence is
 *       executed, which e.g. takes care of writing back NV memory contents.
 */
extern void EcuM_KillAllRUNRequests(void);

/**
 * This service selects the shutdown target.
 *
 * @param target The selected shutdown target.
 * @param mode   An index like value which can be dereferenced to a sleep mode.
 * @return E_OK: The new shutdown target was set, E_NOT_OK: The new shutdown target was not set.
 *
 * @note The parameter mode accepts only the following subset of the EcuM_StateType value \ref ECUM_STATE_SLEEP, \ref ECUM_STATE_RESET, \ref ECUM_STATE_OFF.
 * @note The mode parameter shall only be used if the target parameter equals \ref ECUM_STATE_SLEEP.
 *
 *
 */
extern Std_ReturnType EcuM_SelectShutdownTarget(EcuM_StateType target, uint8 mode);

/**
 * This service returns the selected shutdown target as set by \ref EcuM_SelectShutdownTarget.
 *
 * @param shutdownTarget One of these values is returned: \ref ECUM_STATE_SLEEP, \ref ECUM_STATE_RESET, \ref ECUM_STATE_OFF.
 * @param sleepMode  If the return parameter is ECUM_STATE_SLEEP, this out parameter tells which of the configured sleep modes was actually chosen.
 * @return E_OK: The service has succeeded, E_NOT_OK: The service has failed, e.g. due to NULL pointer being passed.
 *
 */
extern Std_ReturnType EcuM_GetShutdownTarget(EcuM_StateType* shutdownTarget, uint8* sleepMode);

/**
 * This service returns the shutdown target of the previous shutdown process.
 *
 * @param shutdownTarget  One of these values is returned: \ref ECUM_STATE_SLEEP, \ref ECUM_STATE_RESET, \ref ECUM_STATE_OFF.
 * @param sleepMode This parameter tells which of the configured sleep modes was actually chosen.
 * @return E_OK: The service has succeeded, E_NOT_OK: The service has failed, e.g. due to NULL pointer being passed.
 *
 */
extern Std_ReturnType EcuM_GetLastShutdownTarget(EcuM_StateType* shutdownTarget, uint8* sleepMode);

/**
 * This service returns the pending wakeup events.
 *
 * @return The wakeup events.
 */
extern EcuM_WakeupSourceType EcuM_GetPendingWakeupEvents(void);

/**
 * This service clears wakeup events.
 *
 * Clears all pending events passed in the in parameters from the internal variable (NAND-operation).
 *
 * @param sources Events to be cleared.
 *
 */
extern void EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources);

/**
 * This service returns validated wakeup events.
 *
 * @return All validated wakup events.
 */
extern EcuM_WakeupSourceType EcuM_GetValidatedWakeupEvents(void);


/**
 * This service returns all expired wakeup events.
 *
 *
 * All wakeup events: Returns all events that have been set and for which validation has failed. Events which
 * do not need validation are not reported by this service.
 *
 * @return All expired wakup events.
 */
extern EcuM_WakeupSourceType EcuM_GetExpiredWakeupEvents(void);

/**
 * The service computes the sum status of the wakeup source.
 *
 * @return If EcuM_GetValidatedWakeupEvents returns not null then \ref ECUM_WKSTATUS_VALIDATED.
 *         If EcuM_GetPendingWakeupEvents returns not null then \ref ECUM_WKSTATUS_PENDING.
 *	       If EcuM_GetExpiredWakeupEvents returns not null then \ref ECUM_WKSTATUS_EXPIRED.
 *	       Else return \ref ECUM_WKSTATUS_NONE.
 *
 */
extern EcuM_WakeupStatusType EcuM_GetStatusOfWakeupSource(EcuM_WakeupSourceType sources);

/**
 * The service stores the application mode preferably in a non-initialized area of RAM.
 *
 * @note The service is intended for implementing AUTOSAR ports.
 *
 * @param appMode Application mode taken for next OS start.
 * @return E_OK: The new application mode was accepted by EcuM, E_NOT_OK: The new application mode was not accepted by EcuM.
 *
 */
extern Std_ReturnType EcuM_SelectApplicationMode(AppModeType appMode);

/**
 * This service returns the currently selected application mode, see also \ref EcuM_SelectApplicationMode.
 *
 * @param appMode The currently selected application mode.
 * @return  E_OK: The service always succeeds.
 *
 */
extern Std_ReturnType EcuM_GetApplicationMode(AppModeType* appMode);

/**
 * This service selects a boot target.
 *
 * @param target The selected boot target.
 * @return  E_OKThe new boot target was accepted by EcuM, E_NOT_OK: The new boot target was not accepted by EcuM
 *
 */
extern Std_ReturnType EcuM_SelectBootTarget(EcuM_BootTargetType target);

/**
 * This service returns the currently selected boot target.
 *
 * @param target The currently selected boot target.
 * @return  E_OK: The service always succeeds.
 */
extern Std_ReturnType EcuM_GetBootTarget(EcuM_BootTargetType * target);

/**
 * This service implements all activities of the ECU State Manager while the OS is up and running.
 *
 * @note : This service must be called on a periodic basis from an adequate BSW task (i.e. a task under control of the BSW scheduler).
 *
 */
extern void EcuM_MainFunction(void);

/**
 * The callback is used to notify about the end of NVRAM jobs initiated by EcuM.
 *
 * @note The callback must be callable from normal and interrupt execution contexts.
 *
 * @param serviceId Unique Service ID of NVRAM manager service.
 * @param jobResult Covers the job result of the previous processed multi block job.
 *
 */
extern void EcuM_CB_NfyNvMJobEnd(uint8 serviceId, NvM_RequestResultType jobResult);

/**
 * This service sets a wakeup event form a wakeup source.
 *
 * @param sources The source to be set.
 */
extern void EcuM_SetWakeupEvent(EcuM_WakeupSourceType sources);


/**
 * After wakeup, the ECU State Manager will stop the process during the WAKEUP
 * VALIDATION state to wait for validation of the wakeup event. The validation is
 * carried out with a call to this API service.
 *
 * @param sources Events to be validated
 *
 */
extern void EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType sources);



/*@}*/

#define ECUM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* ECUM_H_ */
