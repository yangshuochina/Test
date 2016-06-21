/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM_Cbk.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:58:34MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM_Cbk.h  $
 *  Revision 1.3 2010/11/22 07:58:34MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:21:20MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef ECUM_CBK_H_
#define ECUM_CBK_H_

#include "EcuM_Generated_Types.h"

#define ECUM_START_SEC_CODE
#include "MemMap.h"

/** @defgroup group_ecum_callout EcuM Callout Definitions
 *  @ingroup group_ecum
 *
 */

/*@{*/

/** @page ecum_callout EcuM Callout/Callback
 * @ingroup group_ecum_callout
 *
 * @section ecum_callout_intro Introduction
 *
 * Callouts are pieces of code that have to be added to the ECU State Manager during
 * ECU integration. The content of most callouts is hand-written code, for some callouts
 * the ECU State Manager configuration tool shall generate a default implementation
 * that is manually edited by the integrator. Conceptually, these callouts belong to the
 * ECU Firmware.
 * @see Callout Definition in <a href="../external/autosar/AUTOSAR_SWS_ECU_StateManager.pdf">AUTOSAR_SWS_ECU_StateManager</a>.
 *
 */


/**
 * In unrecoverable error situations, the ECU State Manager will call the error hook. It
 * is up the system integrator to react accordingly (reset, halt, restart, safe state etc. )
 *
 * @param reason Reason for calling the error hook.
 * @warning A call to this function never returns.
 *
 */
extern void EcuM_ErrorHook(Std_ReturnType reason);

/**
 * This callout provides driver initialization and other hardware-related startup
 * activities for loading the post-build configuration data.
 *
 * @warning: Here only pre-compile and link-time configurable modules may be used.
 *
 */
extern void EcuM_AL_DriverInitZero(void);

/**
 * This callout should evaluate some condition, like port pin or NVRAM value, to
 * determine which post-build configuration shall be used in the remainder of the
 * startup process. It shall load this configuration data into a piece of memory that is
 * accessible by all BSW modules and shall return a pointer to the EcuM post-build
 * configuration as a base for all BSW module post-build configurations.
 *
 * @return Pointer to the EcuM post-build configuration which contains pointers to all other BSW module post-build configurations.
 *
 */
extern EcuM_ConfigType* EcuM_DeterminePbConfiguration(void);

/**
 * This callout shall provide driver initialization and other hardware-related startup
 * activities in case of a power on reset.
 *
 * @param configPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW module post-build configurations.
 *
 */
extern void EcuM_AL_DriverInitOne( const EcuM_ConfigType* configPtr);

/**
 * This callout shall provide driver initialization of drivers which need OS and do not
 * need to wait for the NvM_ReadAll job to finish.
 *
 * @param configPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW module post-build configurations.
 *
 */
extern void EcuM_AL_DriverInitTwo(const EcuM_ConfigType* configPtr);

/**
 * This callout shall provide driver initialization of drivers which need OS and need to
 * wait for the NvM_ReadAll job to finish.
 *
 * @param configPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW module post-build configurations.
 *
 */
extern void EcuM_AL_DriverInitThree(const EcuM_ConfigType* configPtr);

/**
 * Invocation of EcuM_OnRTEStartup: Just before calling RTE_Start
 *
 */
extern void EcuM_OnRTEStartup(void);

/**
 * On entry of RUN state is very similar to adjust after startup. This call allows the
 * system designer to notify that RUN state has been reached.
 *
 */
extern void EcuM_OnEnterRun(void);

/**
 * This call allows the system designer to notify that the APP RUN state is about to be left.
 *
 */
extern void EcuM_OnExitRun(void);

/**
 * This call allows the system designer to notify that the APP POST RUN state is about to be left.
 *
 */
extern void EcuM_OnExitPostRun(void);

/**
 * This call allows the system designer to notify that the PREP SHUTDOWN state is about to be entered.
 *
 */
extern void EcuM_OnPrepShutdown(void);

/**
 * This call allows the system designer to notify that the GO SLEEP state is about to be entered.
 *
 */
extern void EcuM_OnGoSleep(void);

/**
 * This call allows the system designer to notify that the GO OFF I state is about to
 * be entered.
 *
 */
extern void EcuM_OnGoOffOne(void);

/**
 * This call allows the system designer to notify that the GO OFF II state is about to be entered.
 *
 */
extern void EcuM_OnGoOffTwo(void);

/**
 * The ECU State Manager needs to derive the wakeup sources to be
 * enabled from configuration information.
 *
 */
extern void EcuM_EnableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This function shall generate a Ram Hash.
 *
 * The invocation of EcuM_GenerateRamHash is done just before putting the ECU physically to sleep.
 *
 * @see \ref EcuM_CheckRamHash
 *
 */
extern void EcuM_GenerateRamHash(void);

/**
 * This callout shall take the code for shutting off the power supply of the ECU. If the
 * ECU cannot unpower itself, a reset may be an adequate reaction.
 *
 */
extern void EcuM_AL_SwitchOff(void);

/**
 * This callout is intended to provide a RAM integrity test. The goal of this test is to
 * ensure that after a long SLEEP duration, RAM contents is still consistent. The
 * check does not need to be exhaustive since this would consume quite some
 * processing time during wakeups. A well designed check will execute quickly and
 * detect RAM integrity defects with a sufficient probability.
 * This specification does not make any assumption about the algorithm chosen for a particular ECU.
 * The areas of RAM which will be checked have to be chosen carefully. It depends
 * on the check algorithm itself and the task structure. Stack contents of the task
 * executing the RAM check e.g. very likely cannot be checked. It is good practice to
 * have the hash generation and checking in the same task and that this task is not
 * preemptible and that there is only little activity between hash generation and hash check.
 * The RAM check itself is provided by the system designer.
 *
 * @return 0: RAM integrity test failed else: RAM integrity test passed
 *
 */
extern uint8 EcuM_CheckRamHash(void);

/**
 * The callout shall set up the given wakeup source(s) so that they are not able to wakeup the ECU.
 *
 * @param wakeupSource The wakeupsource to be disabled.
 */
extern void EcuM_DisableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This callout shall provide driver initialization and other hardware-related startup
 * activities in the wakeup case.
 *
 */
extern void EcuM_AL_DriverRestart(void);

/**
 * The callout shall start the given wakeup source(s) so that they are ready to perform wakeup validation.
 *
 */
extern void EcuM_StartWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This callout is called by the EcuM to validate a wakeup source. If a valid wakeup
 * has been detected, it shall be reported to EcuM via
 * \ref EcuM_ValidateWakeupEvent.
 *
 * @param wakeupSource The wakeup source to validate.
 *
 */
extern void EcuM_CheckValidation(EcuM_WakeupSourceType wakeupSource);

/**
 * The callout shall stop the given wakeup source(s) after unsuccessful wakeup
 * validation.
 *
 * @param wakeupSource The wakeup source(s) to stop validation.
 */
extern void EcuM_StopWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**
 * This callout gives the system designer the chance to intercept the automatic boot
 * behavior and to override the wakeup reaction computed from wakeup source.
 *
 * @param wact The wakeup reaction computed by ECU State Manager.
 *
 * @return The desired wakeup reaction.
 *
 */
extern EcuM_WakeupReactionType EcuM_OnWakeupReaction(EcuM_WakeupReactionType wact);

/**
 * This callout is invoked periodically in all reduced clock sleep modes.
 *It is explicitly allowed to poll wakeup sources from this callout and to call wakeup
 * notification functions to indicate the end of the sleep state to the ECU State Manager.
 *
 */
extern void EcuM_SleepActivity(void);

/**
 * This callout is called by the EcuM to poll a wakeup source. It shall also be called
 * by the ISR of a wakeup source to set up the PLL and check other wakeup sources
 * that may be connected to the same interrupt.
 *
 * @param wakeupSource The wakeup source to be checked.
 */
extern void EcuM_CheckWakeup(EcuM_WakeupSourceType wakeupSource);

/*@}*/

#define ECUM_STOP_SEC_CODE
#include "MemMap.h"


#endif

