/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/28 10:18:40MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM.c  $
 *  Revision 1.4 2011/02/28 10:18:40MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.3 2011/02/02 09:17:17MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 14:20:26MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "Std_Types.h"
#include "Dem.h"
#include "EcuM.h"
#include "EcuM_Cbk.h"
#include "EcuM_Cfg.h"
#include "EcuM_Generated_Types.h"
#include "Mcu.h"
#include "OSEK_Types.h"
#include "Os.h"
#include "SchM.h"
#include "Gpt.h"
#include "ComM_EcuM.h"

#ifdef ECUM_NVMRAM_MGR_INCLUDED
#include "Eep.h"
#include "Ea.h"
#include "Fee.h"
#include "Fls.h"
#include "NvM.h"
#include "NvM_Scheduler.h"
#endif

#ifdef ECUM_WDGM_INCLUDED
#include "WdgM.h"
#endif

#ifdef ECUM_DEM_INCLUDED
#include "Dem.h"
#endif

#ifdef ECUM_PERFORM_SHUTDOWN_COMM
#include "ComM.h"
#endif

/*----------------------- VAR 8 BIT ---------------------*/
#define ECUM_START_SEC_VAR_8BIT
#include "MemMap.h"

/**
 * The state of the ECU manager.
 */
static EcuM_StateType ecum_state;

#define ECUM_STOP_SEC_VAR_8BIT
#include "MemMap.h"

/*----------------------- VAR BOOLEAN ---------------------*/
#define ECUM_START_SEC_VAR_BOOLEAN
#include "MemMap.h"

/**
 * This flag indicates that all run request must be killed.
 * #EcuM_KillAllRUNRequests
 */
boolean ecum_kill_all_run_requests;

#ifdef ECUM_NVMRAM_MGR_INCLUDED

/**
 * This flag indicates, that a nvm job has been finished.
 */
boolean ecum_nvm_job_finished;

#endif

#define ECUM_STOP_SEC_VAR_BOOLEAN
#include "MemMap.h"

/*----------------------- VAR 16 BIT ---------------------*/
#define ECUM_START_SEC_VAR_16BIT
#include "MemMap.h"

/**
 * The high byte contains the selected shutdown target, #ECUM_STATE_SLEEP, #ECUM_STATE_RESET, #ECUM_STATE_OFF.
 * The low byte contains the selected mode of the shutdown target.
 * The mode is only valid, if the target #ecum_selected_shutdown_target equals \ref ECUM_STATE_SLEEP.
 */
static uint16 ecum_selected_shutdown_target;

#define ECUM_STOP_SEC_VAR_16BIT
#include "MemMap.h"

/*----------------------- VAR 32 BIT ---------------------*/
#define ECUM_START_SEC_VAR_32BIT
#include "MemMap.h"

/**
 * Pointer to the active EcuM configuration set.
 */
EcuM_ConfigType *ecum_pb_cfg;

/**
 * The unvalidated wake-up source.
 */
EcuM_WakeupSourceType ecum_wakup_source_unvalidated;

/**
 * A #BITFIELD32 containing the users requesting the RUN state to be hold.
 * For the configuration of the users see also #ecum_pct_config_user.
 */
static BITFIELD32(ecum_users_request_run);

/**
 * A #BITFIELD32 containing the users requesting the POSTRUN state to be hold.
 * For the configuration of the users see also #ecum_pct_config_user.
 */
static BITFIELD32(ecum_users_request_post_run);

/**
 * A #BITARRAY containing the list of communication channels requestin RUN state.
 */
static BITARRAY(ecum_comm_channels_request_run, 256);

#define ECUM_STOP_SEC_VAR_32BIT
#include "MemMap.h"

/*----------------------- VAR 16 BIT NOINIT ---------------*/
#define ECUM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"

/**
 * The high byte contains the last selected shutdown target, #ECUM_STATE_SLEEP, #ECUM_STATE_RESET, #ECUM_STATE_OFF.
 * The state is stored in non initialized ram area to be available during next startup.
 * The low byte contains the selected mode of the shutdown target.
 * The mode is only valid, if the target #ecum_selected_shutdown_target equals \ref ECUM_STATE_SLEEP.
 */
static uint16 ecum_last_selected_shutdown_target;

#define ECUM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"

/*----------------------- VAR 32 BIT NOINIT ---------------*/
#define ECUM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"

static AppModeType ecum_selected_application_mode;

#define ECUM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"

/*----------------------- CODE ---------------------*/
#define ECUM_START_SEC_CODE
#include "MemMap.h"

#include "NvM.h"
#include "Ea.h"
#include "Eep.h"

void EcuM_Init(void)
{

	ecum_state = ECUM_STATE_STARTUP_ONE;

	/* startup block 0 */

	/* initialize BSW modules that do not use post build configuration */
	EcuM_AL_DriverInitZero();

	ecum_pb_cfg = EcuM_DeterminePbConfiguration();

#ifdef ECUM_DETECT_DEVELOPMENT_ERRORS
	ECUM_ASSERT_RETVOID(ecum_pb_cfg != NULL_PTR, ECUM_INIT_API_ID, ECUM_E_NULL_POINTER);ECUM_ASSERT_RETVOID(sizeof(EcuM_StateType) == 1, ECUM_INIT_API_ID, ECUM_E_STATE_PAR_OUT_OF_RANGE);
#endif

	/* check consistency of configuration data */
	if(ecum_pb_cfg->config_consistency_hash != ECUM_CONFIGCONSISTENCY_HASH) {

#ifdef ECUM_DETECT_DEVELOPMENT_ERRORS
		ECUM_ASSERT_RETVOID(0, ECUM_INIT_API_ID, ECUM_E_CONFIGURATION_DATA_INCONSISTENT);
#endif
		/* this call never returns */
		EcuM_ErrorHook(ECUM_E_CONFIGURATION_DATA_INCONSISTENT);
	}

	/* startup block 1 */
	EcuM_AL_DriverInitOne(ecum_pb_cfg);

	/* map reset reason to wake-up source                    */
	/* select the shutdown target                            */
	/* select the application mode if startup was unintended */
	/* TODO: The mapping of the reset reason to the wakup-up source in EcuM_Init is not implemented. */
	switch(Mcu_GetResetReason()) {
		case MCU_POWER_ON_RESET:
			/* TODO: In EcuM_Init after MCU_POWER_ON_RESET no application mode shall be set, so value undefined or what after first startup. */
			ecum_wakup_source_unvalidated = ECUM_WKSOURCE_POWER;
         EcuM_SelectApplicationMode(ecum_pb_cfg->default_application_mode);
			break;
		case MCU_WATCHDOG_RESET:
			ecum_wakup_source_unvalidated = ECUM_WKSOURCE_INTERNAL_WDG;
			EcuM_SelectApplicationMode(ecum_pb_cfg->default_application_mode);
			break;
		case MCU_SW_RESET:
			ecum_wakup_source_unvalidated = ECUM_WKSOURCE_RESET;
			break;
		case MCU_RESET_UNDEFINED:
		default:
			ecum_wakup_source_unvalidated = ECUM_WKSOURCE_POWER;
			EcuM_SelectApplicationMode(ecum_pb_cfg->default_application_mode);
			break;

	}

	/* Start operating system with the selected application mode */
	StartOS(ecum_selected_application_mode);
}
void EcuM_StartupTwo(void)
{
#ifdef ECUM_NVMRAM_MGR_INCLUDED
	Gpt_ValueType nvm_readall_remaining_time;
#endif

	ecum_state = ECUM_STATE_STARTUP_TWO;

	SchM_Init();

#ifdef ECUM_WDGM_INCLUDED
	WdgM_SetMode(ecum_pb_cfg->ecum_wdgm_cfg.startup_mode);
#endif

#ifdef ECUM_NVMRAM_MGR_INCLUDED
	ecum_nvm_job_finished = 0;
#endif

	EcuM_AL_DriverInitTwo(ecum_pb_cfg);

#ifdef ECUM_NVMRAM_MGR_INCLUDED
	/* schedule timer for NVM reading timeout */
	Gpt_StartTimer(ecum_pb_cfg->gpt_timer_channel,
			ecum_pb_cfg->ecum_nvmram_mgr_cfg.nvram_read_all_timeout);
#endif

	EcuM_OnRTEStartup();

	/* TODO: Rte_Start(); */

#ifdef ECUM_NVMRAM_MGR_INCLUDED

	NvM_ReadAll_for_App();

	Gpt_StopTimer(ecum_pb_cfg->gpt_timer_channel);

	EcuM_AL_DriverInitThree(ecum_pb_cfg);

#endif


	/* TODO: RTE_Switch_CurrentMode(RTE_MODE_EcuM_Mode_RUN);*/

	/* check the wake-up sources set to determine how to continue */
	switch(ecum_wakup_source_unvalidated) {
		case ECUM_WKSOURCE_INTERNAL_RESET:
		case ECUM_WKSOURCE_EXTERNAL_WDG:
		case ECUM_WKSOURCE_INTERNAL_WDG:
		case ECUM_WKSOURCE_POWER:
		case ECUM_WKSOURCE_RESET:
			/* These wake-up sources do not require validation under no circumstances. */
			ecum_state = ECUM_STATE_RUN;
			break;
		default:
			ecum_state = ECUM_STATE_WAKEUP_VALIDATION;
			break;
	}
}

void EcuM_Shutdown(void)
{

	EcuM_StateType shutdownTarget;
	uint8 sleepMode;

	/* Deinitialization Sequence III (GO OFF II) */
	ecum_state = ECUM_STATE_GO_OFF_TWO;

	EcuM_OnGoOffTwo();

	EcuM_GetShutdownTarget(&shutdownTarget, &sleepMode);

	ecum_last_selected_shutdown_target = shutdownTarget;

	if(shutdownTarget == ECUM_STATE_RESET) {
		Mcu_PerformReset();
	}
	else {
		/* ECUM_STATE_OFF */
		EcuM_AL_SwitchOff();
	}
}

Std_ReturnType EcuM_GetState(EcuM_StateType* state)
{
#ifdef ECUM_DETECT_DEVELOPMENT_ERRORS
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_GET_STATE_API_ID, ECUM_E_NOT_INITED);
#endif
	*state = ecum_state;
	return E_OK;
}

Std_ReturnType EcuM_RequestRUN(EcuM_UserType user)
{
#ifdef ECUM_DETECT_DEVELOPMENT_ERRORS
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_REQUEST_RUN_API_ID, ECUM_E_NOT_INITED);
	ECUM_ASSERT_RETERR((1u << user) & ECUM_ALL_USER_ID_MASK,
			ECUM_REQUEST_RUN_API_ID, ECUM_E_INVALID_PAR);
	ECUM_ASSERT_RETERR(BITFIELD32_GETBIT(ecum_users_request_run, 1 << user) == 0,
			ECUM_REQUEST_RUN_API_ID, ECUM_E_MULTIPLE_RUN_REQUESTS);
#endif

	if(ecum_kill_all_run_requests) {
		return E_NOT_OK;
	}

	BITFIELD32_SETBIT(ecum_users_request_run, user);
	return E_OK;
}

Std_ReturnType EcuM_ReleaseRUN(EcuM_UserType user)
{
#ifdef ECUM_DETECT_DEVELOPMENT_ERRORS
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_RELEASE_RUN_API_ID, ECUM_E_NOT_INITED);ECUM_ASSERT_RETERR(user & ECUM_ALL_USER_ID_MASK,
			ECUM_RELEASE_RUN_API_ID, ECUM_E_INVALID_PAR);ECUM_ASSERT_RETERR(BITFIELD32_GETBIT(ecum_users_request_run, 1 << user) > 0,
			ECUM_RELEASE_RUN_API_ID, ECUM_E_MISMATCHED_RUN_RELEASE);
#endif
	BITFIELD32_CLRBIT(ecum_users_request_run, user);
	return E_OK;
}

Std_ReturnType EcuM_ComM_RequestRUN(NetworkHandleType channel)
{
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_COMM_REQUEST_RUN_API_ID, ECUM_E_NOT_INITED);
	ECUM_ASSERT_RETERR(BITARRAY_GETBIT(ecum_comm_channels_request_run, channel) == 0,
			ECUM_COMM_REQUEST_RUN_API_ID, ECUM_E_MULTIPLE_RUN_REQUESTS);

	if(ecum_kill_all_run_requests) {
		return E_NOT_OK;
	}

	BITARRAY_SETBIT(ecum_comm_channels_request_run, channel);
	return E_OK;
}

Std_ReturnType EcuM_ComM_ReleaseRUN(NetworkHandleType channel)
{
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_COMM_RELEASE_RUN_API_ID, ECUM_E_NOT_INITED);
	ECUM_ASSERT_RETERR(BITARRAY_GETBIT(ecum_comm_channels_request_run, channel) > 0,
			ECUM_COMM_RELEASE_RUN_API_ID, ECUM_E_MISMATCHED_RUN_RELEASE);
	BITARRAY_CLRBIT(ecum_comm_channels_request_run, channel);
	return E_NOT_OK;
}

boolean EcuM_ComM_HasRequestedRUN(NetworkHandleType channel)
{
	unsigned int result;
	result = BITARRAY_GETBIT(ecum_comm_channels_request_run, channel);
	if(result != 0) {
		return TRUE;
	}
	return FALSE;
}

Std_ReturnType EcuM_RequestPOST_RUN(EcuM_UserType user)
{
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_REQUEST_POST_RUN_API_ID, ECUM_E_NOT_INITED);
	ECUM_ASSERT_RETERR(user & ECUM_ALL_USER_ID_MASK,
			ECUM_REQUEST_POST_RUN_API_ID, ECUM_E_INVALID_PAR);
	ECUM_ASSERT_RETERR(BITFIELD32_GETBIT(ecum_users_request_post_run, 1 << user) == 0,
			ECUM_REQUEST_POST_RUN_API_ID, ECUM_E_MULTIPLE_RUN_REQUESTS);
	if(ecum_kill_all_run_requests) {
		return E_NOT_OK;
	}

	BITFIELD32_SETBIT(ecum_users_request_post_run, user);
	return E_OK;
}

Std_ReturnType EcuM_ReleasePOST_RUN(EcuM_UserType user)
{
	ECUM_ASSERT_RETERR(ecum_pb_cfg != NULL_PTR,
			ECUM_RELEASE_POST_RUN_API_ID, ECUM_E_NOT_INITED);
	ECUM_ASSERT_RETERR(user & ECUM_ALL_USER_ID_MASK,
			ECUM_RELEASE_POST_RUN_API_ID, ECUM_E_INVALID_PAR);
	ECUM_ASSERT_RETERR(BITFIELD32_GETBIT(ecum_users_request_post_run, 1 << user),
			ECUM_RELEASE_POST_RUN_API_ID, ECUM_E_MISMATCHED_RUN_RELEASE);
	BITFIELD32_CLRBIT(ecum_users_request_post_run, user);
	return E_OK;
}

void EcuM_KillAllRUNRequests(void)
{

	ecum_kill_all_run_requests = TRUE;

	Dem_ReportErrorStatus(ECUM_E_ALL_RUN_REQUESTS_KILLED, DEM_EVENT_STATUS_FAILED);

	ecum_users_request_run = 0;
	ecum_users_request_post_run = 0;
	BITARRAY_CLR(ecum_comm_channels_request_run);
}

Std_ReturnType EcuM_SelectShutdownTarget(EcuM_StateType target, uint8 mode)
{
	ECUM_ASSERT_RETERR((target == ECUM_STATE_SLEEP) ||
			(target == ECUM_STATE_RESET) ||
			(target == ECUM_STATE_OFF),
			ECUM_SELECT_SHUTDOWN_TARGET_API_ID, ECUM_E_STATE_PAR_OUT_OF_RANGE);

	ecum_selected_shutdown_target = ((uint16)target) * 256 + mode;

	/* TODO: preload TTII divisor counter variable - required for AUTOSAR Ports. */

	return E_OK;
}

Std_ReturnType EcuM_GetShutdownTarget(EcuM_StateType* shutdownTarget, uint8* sleepMode)
{
	uint16 tmp;

	ECUM_ASSERT_RETERR(shutdownTarget != NULL_PTR,
			ECUM_GET_SHUTDOWN_TARGET_API_ID, ECUM_E_NULL_POINTER);

	tmp = ecum_selected_shutdown_target;
	*shutdownTarget = (EcuM_StateType)(tmp / 256);
	*sleepMode = (EcuM_StateType)(tmp & 0xFF);

	return E_OK;
}

Std_ReturnType EcuM_GetLastShutdownTarget(EcuM_StateType* shutdownTarget, uint8* sleepMode)
{
	uint16 tmp;

	ECUM_ASSERT_RETERR(shutdownTarget != NULL_PTR,
			ECUM_GET_LAST_SHUTDOWN_TARGET_API_ID, ECUM_E_NULL_POINTER);

	tmp = ecum_last_selected_shutdown_target;
	*shutdownTarget = (EcuM_StateType)(tmp / 256);
	*sleepMode = (EcuM_StateType)(tmp & 0xFF);

	return E_OK;
}

EcuM_WakeupSourceType EcuM_GetPendingWakeupEvents(void)
{
	return 0;
}

void EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources)
{

}

EcuM_WakeupSourceType EcuM_GetValidatedWakeupEvents(void)
{
	return ECUM_WKSOURCE_POWER;
}

EcuM_WakeupSourceType EcuM_GetExpiredWakeupEvents(void)
{
	return ECUM_WKSOURCE_POWER;
}

EcuM_WakeupStatusType EcuM_GetStatusOfWakeupSource(EcuM_WakeupSourceType sources)
{
	return ECUM_WKSTATUS_NONE;
}

Std_ReturnType EcuM_SelectApplicationMode(AppModeType appMode)
{
	ecum_selected_application_mode = appMode;
	return E_NOT_OK;
}

Std_ReturnType EcuM_GetApplicationMode(AppModeType* appMode)
{
	*appMode = ecum_selected_application_mode;
	return E_OK;
}

Std_ReturnType EcuM_SelectBootTarget(EcuM_BootTargetType target)
{
	return E_NOT_OK;
}

Std_ReturnType EcuM_GetBootTarget(EcuM_BootTargetType * target)
{
	/* TODO: default implementation of EcuM_GetBootTarget */
	*target = ECUM_BOOT_TARGET_APP;
	return E_OK;
}

void EcuM_MainFunction(void)
{
	int com_ch;

#ifdef ECUM_NVMRAM_MGR_INCLUDED
	Gpt_ValueType nvm_writeall_remaining_time;
#endif

	switch(ecum_state) {
		case ECUM_STATE_WAKEUP:
		case ECUM_STATE_WAKEUP_ONE:
			break;
		case ECUM_STATE_WAKEUP_VALIDATION:
			break;
		case ECUM_STATE_WAKEUP_REACTION:
			break;
		case ECUM_STATE_WAKEUP_TWO:
			break;
		case ECUM_STATE_WAKEUP_TTII:
			break;
		case ECUM_STATE_RUN:
			/* RUN state is entered */
			EcuM_OnEnterRun();

#ifdef ECUM_WDGM_INCLUDED
			WdgM_SetMode(ecum_pb_cfg->ecum_wdgm_cfg.run_mode);
#endif
			/* notify all communication channels that have requested run */
			com_ch = 0;
			do {
				com_ch = BITARRAY_GETFIRSTBIT(ecum_comm_channels_request_run, com_ch);
				if(com_ch >= 0) {
					ComM_EcuM_RunModeIndication(com_ch);
					com_ch++;
				}
			} while(com_ch >= 0);

			/* schedule timer for minimum duration to stay in RUN state */
			Gpt_StartTimer(ecum_pb_cfg->gpt_timer_channel,
					ecum_pb_cfg->ecum_run_self_request_period);

			/* proceed to RUN II state */
			ecum_state = ECUM_STATE_APP_RUN;
			break;
		case ECUM_STATE_APP_RUN:
			/* RUN II state */
			if(BITFIELD32_IS_CLEAR(ecum_users_request_run) &&
					(BITARRAY_GETFIRSTBIT(ecum_comm_channels_request_run, 0) < 0) &&
					(Gpt_GetTimeRemaining(ecum_pb_cfg->gpt_timer_channel) == 0)) {
				Gpt_StopTimer(ecum_pb_cfg->gpt_timer_channel);
				EcuM_OnExitRun();
#ifdef ECUM_WDGM_INCLUDED
				WdgM_SetMode(ecum_pb_cfg->ecum_wdgm_cfg.postrun_mode);
#endif
				/* TODO: Rte_Switch_currentMode(RTE_MODE_EcuM_Mode_POSTRUN)); */

				ecum_state = ECUM_STATE_APP_POST_RUN;
			}
			break;
		case ECUM_STATE_APP_POST_RUN:
			/* RUN III state */
			/* if a RUN request is received, return to RUN III state. */
			if((BITFIELD32_IS_CLEAR(ecum_users_request_run) == 0)
					|| (BITARRAY_GETFIRSTBIT(ecum_comm_channels_request_run, 0) >= 0)) {
				ecum_state = ECUM_STATE_APP_RUN;
			}
			else if(BITFIELD32_IS_CLEAR(ecum_users_request_post_run)) {
				/* the last post run request has been released - proceed to SHUTDOWN. */
				EcuM_OnExitPostRun();
				ecum_state = ECUM_STATE_PREP_SHUTDOWN;
			}
			break;
		case ECUM_STATE_SHUTDOWN:
		case ECUM_STATE_PREP_SHUTDOWN:
			/* Deinitialization Sequence I */

			/* clear any set wakeup event */
			EcuM_ClearWakeupEvent(ECUM_WKSOURCE_ALL_SOURCES);

			EcuM_OnPrepShutdown();

#ifdef ECUM_DEM_INCLUDED
			/* shutdown diagnostics event manager */
			Dem_Shutdown();
#endif
			/* get shutdown target - select further proceeding */
			{
				EcuM_StateType shutdownTarget;
				uint8 sleepMode;
				if(EcuM_GetShutdownTarget(&shutdownTarget, &sleepMode) == E_OK) {
					switch(shutdownTarget) {
						case ECUM_STATE_SLEEP:
							/* TODO: Rte_Switch_currentMode(RTE_EcuM_Mode_SLEEP); */
							/* proceed according to the selected sleep mode */
							ecum_state = ECUM_STATE_GO_SLEEP;
							break;
						case ECUM_STATE_RESET:
						case ECUM_STATE_OFF:
						default:
							/* TODO: Rte_Switch_currentMode(RTE_EcuM_Mode_SHUTDOWN); */
							ecum_state = ECUM_STATE_GO_OFF_ONE;
							break;
					}
				}
			}
			break;
		case ECUM_STATE_GO_SLEEP:
			break;
		case ECUM_STATE_GO_OFF_ONE:
			/* Deiniitalization Sequence IIb (GO OFF I) */
			EcuM_OnGoOffOne();

			/* TODO: Rte_Stop(); */

#ifdef ECUM_PERFORM_SHUTDOWN_COMM
			/* Shutdown communication manager ComM. */
			ComM_DeInit();
#endif

#ifdef ECUM_NVMRAM_MGR_INCLUDED

#endif

#ifdef ECUM_WDGM_INCLUDED
			/* Select watchdog mode for shutdown */
			WdgM_SetMode(ecum_pb_cfg->ecum_wdgm_cfg.shutdown_mode);
#endif
			{
				/* Check for pending wakeup events. */
				EcuM_WakeupSourceType pendingWakeupEvents;
				pendingWakeupEvents = EcuM_GetPendingWakeupEvents();
				if(pendingWakeupEvents > 0) {
					EcuM_SelectShutdownTarget(ECUM_STATE_RESET, 0);
				}
			}
			ecum_state = ECUM_STATE_GO_OFF_TWO;

			/* Shutdown the operating system. */
			/* The OS must call EcuM_Shutdown() in its shutdown hook */
			ShutdownOS(E_OK);
			/* no return to here from this point of execution */
			break;
		case ECUM_STATE_GO_OFF_TWO:
			break;
		case ECUM_STATE_SLEEP:

			/* TODO: store last shutdown target - ecum_last_selected_shutdown_target */

			break;
		case ECUM_STATE_OFF:
			break;
		case ECUM_STATE_RESET:
			break;
		default:
			break;
	}

}

void EcuM_CB_NfyNvMJobEnd(uint8 serviceId, NvM_RequestResultType jobResult)
{
#ifdef ECUM_NVMRAM_MGR_INCLUDED
	ecum_nvm_job_finished = 1;
#endif
}

void EcuM_SetWakeupEvent(EcuM_WakeupSourceType sources)
{

}

void EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType sources)
{

}

#define ECUM_STOP_SEC_CODE
#include "MemMap.h"
