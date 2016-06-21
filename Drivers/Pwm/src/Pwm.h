/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Pwm.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:28:42MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Pwm.h  $
 *  Revision 1.3 2011/03/22 09:28:42MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.2 2010/11/22 08:34:28MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:20:08MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Pwm/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "team-s.h"
#include "Std_Types.h"

/** @defgroup group_pwm Pwm
 *  @ingroup group_pwm */
/*@{*/

#define PWM_MODULE_ID         (121)
#define PWM_VENDOR_ID         (TEAMS_VENDOR_ID)
#define PWM_AR_MAJOR_VERSION  (2)
#define PWM_AR_MINOR_VERSION  (2)
#define PWM_AR_PATCH_VERSION  (2)
#define PWM_SW_MAJOR_VERSION  (1)
#define PWM_SW_MINOR_VERSION  (0)
#define PWM_SW_PATCH_VERSION  (0)

/* Driver APIs Service ID */
#define PWM_INIT_API_ID             	((uint8)(0))
#define PWM_DEINIT_API_ID             	((uint8)(1))
#define PWM_SETDUTYCYCLE_API_ID        	((uint8)(2))
#define PWM_SETPERIODANDDUTY_API_ID    	((uint8)(3))
#define PWM_SETOUTPUTTOIDLE_API_ID		((uint8)(4))
#define PWM_GETOUTPUTSTATE_API_ID      	((uint8)(5))
#define PWM_DISABLENOTIFICATION_API_ID	((uint8)(6))
#define PWM_ENABLENOTIFICATION_API_ID	((uint8)(7))
#define PWM_GETVERSIONINFO_API_ID		((uint8)(8))

/* Development Error Codes */
#define PWM_E_PARAM_CONFIG				((uint8)(0x10))	/**< A wrong parameter to function Pwm_Init() was passed. */
#define PWM_E_UNINIT					((uint8)(0x11))	/**< A service was called without a prior Pwm_Init(). */
#define PWM_E_PARAM_CHANNEL				((uint8)(0x12))	/**< A service was used with an invalid channel Identifier. */
#define PWM_E_PERIOD_UNCHANGEABLE 		((uint8)(0x13))	/**< The service Pwm_SetPeriodAndDuty() was used on a PWM channel configured with a fixed period. */
#define PWM_E_ALREADY_INITIALIZED 		((uint8)(0x14))	/**< The service Pwm_Init() was service called while the PWM driver was not already initialized. */
#define PWM_E_CONCURRENCY				((uint8)(0x20))	/**< A simultaneous update of the dutycycle was detected. */


/** @page pwm Pwm
 *
 * @section pwm_intro Introduction
 * The Pwm function implements the Pwm functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_PWM_Driver.pdf">AUTOSAR_SWS_PWM_Driver</a>.
 *
 *
 * @section pwm_init Initialization of Pwm
 * Before any service to the Pwm can be used the initialization function Pwm_Init() with a pointer
 * proper configuration set (#Pwm_ConfigType) must be called.
 *
 *
 * @section pwm_autosar AutoSAR Conformity
 * Pwm implements all services defined by AutoSAR.
 * The following features are not implemented:
 * @li phase synchronous PWM, due to the 5 cell PWM implementation
 *
 *
 * @section pwm_operation Operation
 * @subsection pwm_frequency PWM Frequency
 * The frequency of the PWM is determined by the clock that is provided to the
 * first cell of each PWM channel. The used clock can be configured with the
 * input multiplexer of GPTA. That multiplexer is configured by @ref port.
 *
 * The clock can be received by one of the clock busses. These busses are
 * configured by @ref port (e.g. #MCU_GPTA0_CKBCTR_DFA02).
 *
 * The final PWM frequency is:
 * @f[ f_{PWM} = \frac{f_{selected clk}}{PWM_{period}} @f]
 *
 * Where PWM<sub>period</sub> can be setup either via the configuration
 * Pwm_ConfigChannelType::period or via the interface Pwm_SetPeriodAndDuty().
 *
 *
 * @subsection pwm_waveform Waveforms
 * The PWM driver can generate either low or high active PWMs. The type
 * is selected with the configuration parameter Pwm_ConfigChannelType::low_active.
 * If this parameter is 0 then the generated waveform looks like shown below.
 * @image html  external_images/pwm_high_active.png "High Active PWM"
 * @image latex external_images/pwm_high_active.png "High Active PWM"
 *
 * If this parameter is 1 then the generated waveform looks like shown below.
 * @image html  external_images/pwm_low_active.png "Low Active PWM"
 * @image latex external_images/pwm_low_active.png "Low Active PWM"
 *
 *
 * @subsection pwm_update Update
 * The new period and dutycycle as passed via the interfaces Pwm_SetDutyCycle()
 * and Pwm_SetPeriodAndDuty() are reflected at the next PWM period in the output.
 * This means that if multiple updates during a PWM period are performed the
 * last values that where passed will be used for the next PWM period.
 *
 * In other words the dutycycle and period will always be used at the end of the
 * PWM period.
 *
 *
 * @subsection pwm_notfication Notification
 * The notification can be configured via the configuration parameter
 * Pwm_ConfigChannelType::notification. The notification can be enabled
 * either for rising, falling or both edges via the interface
 * Pwm_EnableNotification().
 *
 * It must be noted that if there is a 100% dutycycle used for a PWM
 * channel only the notification for the period event is generated not for
 * the dutycycle event. In case of a high active PWM this means that for
 * 100% dutycycle the falling edge event will be never generated.
 *
 *
 * @section pwm_cfg Configuration
 * @subsection pwm_cfg_step_by_step Step by Step
 * @li Configure #Pwm_ConfigChannelType.
 * @li Configure #Pwm_ConfigType.
 * @li Configure @ref port (clock for timer cell and output for 5th cell)
 * @li Configure interrupts (optional - only required if notification is needed)
 * @li Call the function Pwm_Init() with the pointer to the configured structure #Pwm_ConfigType
 *     in the startup phase.
 *
 *
 * @subsection pwm_cfg_by_example Example
 * @subsubsection pwm_cfg_by_example_1 Configuration of #Pwm_ConfigChannelType:
 * For each PWM 5 consecutive LTC cells are required. The first cell is operating as
 * reset timer while the remaining 4 cells are used as compares for the PWM.
 * The configuration of the PWM channels shall be in an array of the type #Pwm_ConfigChannelType.
 * The index to the array is the channel identifier that is used in the interfaces to control
 * the PWM.
 * @code
 * const Pwm_ConfigChannelType pwm_config_channel[] = {
 *   {                        channel 0
 *     NULL_PTR,              Pwm_ConfigChannelType::notification
 *     PWM_VARIABLE_PERIOD,   dutycycle and frequency can be changed (Pwm_ConfigChannelType::class)
 *     0x1000,                period in LTC ticks (Pwm_ConfigChannelType::period)
 *     0,                     not supported by this implementation (Pwm_ConfigChannelType::phase_shift)
 *     0x0,                   initial dutycycle (Pwm_ConfigChannelType::dutycycle)
 *     0,                     PWM is high active (Pwm_ConfigChannelType::low_active)
 *     0,                     use LTC on GPTA (Pwm_ConfigChannelType::unit)
 *     8,                     use LTC cells 8..12 (Pwm_ConfigChannelType::first_ltc)
 *   },
 *   {                        channel 1
 *     pwm_notify1,           Pwm_ConfigChannelType::notification
 *     PWM_VARIABLE_PERIOD,   dutycycle and frequency can be changed (Pwm_ConfigChannelType::class)
 *     0x2000,                period in LTC ticks (Pwm_ConfigChannelType::period)
 *     0,                     not supported by this implementation (Pwm_ConfigChannelType::phase_shift)
 *     0x0,                   initial dutycycle (Pwm_ConfigChannelType::dutycycle)
 *     1,                     PWM is low active (Pwm_ConfigChannelType::low_active)
 *     0,                     use LTC on GPTA (Pwm_ConfigChannelType::unit)
 *     15,                    use LTC cells 15..19 (Pwm_ConfigChannelType::first_ltc)
 *   },
 * };
 * @endcode
 *
 *
 * @subsubsection pwm_cfg_by_example_2 Configuration of #Pwm_ConfigType:
 * @code
 * const Pwm_ConfigType pwm_cfg = {
 *   pwm_config_channel      pointer to channel configuration (Pwm_ConfigType::channels)
 *   2,                      2 PWM channels (Pwm_ConfigType::num_of_channels)
 * };
 * @endcode
 *
 *
 * @subsubsection pwm_cfg_by_example_3 Configuration Port:
 * Please refer also to the documentation of @ref port.
 *
 * Pin configuration:\n
 * The LTC12 cell can be connected to IOG1, IOG5, OG2 or OG6. In our example we select IOG5.
 * The IOG5 is connected to the output pins OUT40..OUT47. We want to use OUT41. This output
 * is connected amongst others to P5.1. This pin needs to be switched to the alternate
 * function.
 *
 * @code
 * const Port_ConfigPinType port_config_pin[] = {
 * ...
 *  {
 *    PORT_5_1, PORT_PIN_LEVEL_LOW, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE
 *  },
 * ...
 * @endcode
 *
 * Now the output multiplexer for IOG5, OUT41 needs to be configured. So that LTC12 is connected
 * to OUT41.
 * @code
 * const Port_ConfigGptaMultiplexerArray port_config_gpta0_multiplexer_array = {
 *   ...
 *   // OMCRL5 IN[43:40]/OUT[43:40] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]
 *   OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG_LTC0_31, OMG_IN4) | OMCRX(0, OMG0, OMG_IN0),
 *   ...
 * @endcode
 *
 * Clock configuration:\n
 * Last but not least the LTC cell 8 needs to be connected to the clock system. In our
 * example we use CLK2 bus for this cell. The frequency of this CLK2 is adjusted by
 * #MCU_GPTA0_CKBCTR_DFA02.
 * @code
 * const Port_ConfigGptaMultiplexerArray port_config_gpta0_multiplexer_array = {
 *   ...
 *   // LIMCRL1 LTC[11:08] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0]
 *   LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 1, LIMG3, LIMG_IN2),
 *   ...
 * @endcode
 *
 *
 * @subsubsection pwm_cfg_by_example_5 Configuration of interrupt (optional):
 * The notification requires a configured interrupt service. On the TC1767
 * 4 LTC cells share a single interrupt node. The interrupt handler of
 * the respective SRC (interrupt node) must call the function ts_decodeLTC()
 * for the decoding of the correct interrupt handler on PWM. It must be
 * noted that the interrupt on SRC must be enabled by the startup, since
 * the SRC is a shared resource this cannot be done by the PWM function.
 *
 * The assignment SRC to the LTC number can be found in the TC1767 users manual
 * (Table 20-16).
 *
 * The following example shows the configuration of a PWM that is located on
 * the cells 8 to 12.
 * @code
 * void __interrupt(SRPN_GPTA0_SRC24) __enable_ Gpta0Src24(void)
 * {
 *   ts_decodeLTC(8);
 * }
 *
 * void __interrupt(SRPN_GPTA0_SRC25) __enable_ Gpta0Src25(void)
 * {
 *   ts_decodeLTC(12);
 * }
 * @endcode
 * @note Depending on the environment the interrupt might need to be
 *       configured in a different way e.g. in a OIL file.
 *
 *
 * @subsubsection pwm_cfg_by_example_usage Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Pwm_Init(&pwm_cfg);
 *   ...
 *   // set PWM channel 0 to 25% dutycycle with a PWM period of 0x5000
 *   Pwm_SetPeriodAndDuty(0, 0x5000, 0x2000);
 * }
 * @endcode
 *
 */

/** Numeric identifier of a PWM channel.
 *
 */
typedef uint8 Pwm_ChannelType;

/** Definition of the period of a PWM channel.
 *
 */
typedef uint16 Pwm_PeriodType;

/** Output state of a PWM channel.
 *
 */
typedef enum {
	PWM_LOW,		/**< The PWM channel is in a low state. */
	PWM_HIGH		/**< The PWM channel is in a high state. */
} Pwm_OutputStateType;


/** Definition of the type of edge notification of a PWM channel.
 *
 */
typedef enum {
	PWM_RISING_EDGE,	/**< Notification will be called when a rising edge occurs on the PWM output signal. */
	PWM_FALLING_EDGE,	/**< Notification will be called when a falling edge occurs on the PWM output signal. */
	PWM_BOTH_EDGES,		/**< Notification will be called when either a rising edge or falling edge occur on the PWM output signal. */
} Pwm_EdgeNotificationType;


/** Defines the class of a PWM channel
 */
typedef enum {
	PWM_VARIABLE_PERIOD,		/**< The PWM channel has a variable period; The duty cycle and the period can be changed. */
	PWM_FIXED_PERIOD,			/**< The PWM channel has a fixed period; Only the duty cycle can be changed. */
	PWM_FIXED_PERIOD_SHIFTED	/**< The PWM channel has a fixed shifted period. */
} Pwm_ChannelClassType;


/** This is the configuration structure for a Pwm channel.
 *
 * This configuration contains all properties of a Pwm channel.
 */
typedef struct {
	void (*notification)(void);		/**< This function get called for the notification; see Pwm_EnableNotification(). */
	Pwm_ChannelClassType class;		/**< This defines the properties of the Pwm channel. */
	Pwm_PeriodType period;			/**< This element is the period of the Pwm channel in ticks [2 .. 65535]. */
	Pwm_PeriodType phase_shift;		/**< This element is the phase shift that will be applied for phase shifted PWMs (not supported). */
	uint16 dutycycle;				/**< This element defines the initial dutycycle of the Pwm [0..0x8000]. */
	uint8 low_active;				/**< For low active PWMs the dutycycle defines the low phase, while for high active it defines the high phase. */
	uint8 unit;						/**< This is the unit to use 0=GPTA0, 1=LTCA2. */
	uint8 first_ltc;				/**< This defines the first LTC cell to use; a channel uses 5 LTC cells. */
} Pwm_ConfigChannelType;


/** This is the initial Pwm configuration structure.
 *
 * This configuration is passed to the function Pwm_Init().
 */
typedef struct {
	const Pwm_ConfigChannelType *channels;	/**< This element is a pointer to the channel configuration. */
	uint8 num_of_channels;					/**< This element defines the number of PWM channels. */
} Pwm_ConfigType;


#define PWM_START_SEC_CODE
#include "MemMap.h"



/** This service initializes the Pwm driver.
 *
 * The function Pwm_Init() starts all PWM channels with the configured
 * default values. If the duty cycle parameter equals
 * 0% or 100% then the PWM output signal is in the state according to
 * the configured polarity parameter (in an active or inactive state).
 *
 * If the duty cycle is between 0% and 100%: Then the PWM output signal
 * is modulated according to the parameters period, duty cycle and the
 * configured polarity.
 *
 * It is not possible to perform an reinitialization with
 * this function. If a different configuration is required the
 * function Pwm_DeInit() needs to be called first.
 *
 * After the usage of the service all services provided by Pwm can
 * be used.
 *
 * @note After the execution of the function Pwm_Init(), the configuration data are accessible
 * and can be used by the Pwm module functions.
 */
extern void Pwm_Init(const Pwm_ConfigType *cfg);


/** This service terminates the operation of the Pwm driver.
 *
 *  The service Pwm_DeInit() sets the state of all configured PWM output
 *  signals to the idle state.
 *
 *
 */
extern void Pwm_DeInit(void);


/** This service sets the duty cycle of the PWM channel.
 *
 * The service Pwm_SetDutyCycle() updates the duty cycle at the
 * end of the period.
 *
 * The passed dutycycle shall be between 0x0 (which is 0%) and
 * 0x8000 (which is 100%).
 *
 * @warning The services Pwm_SetDutyCycle(), Pwm_SetPeriodAndDuty() and
 *          Pwm_SetOutputToIdle() are only reentrant for different channel
 *          identifiers.
 *
 * @param channel_id  Channel identifier for the PWM channel where the dutycycle shall be changed.
 * @param dutycycle   New dutycycle for the PWM channel [0x0 .. 0x8000].
 */
extern void Pwm_SetDutyCycle(Pwm_ChannelType channel_id, uint16 dutycycle);


/** This service sets the period and the duty cycle of a PWM channel.
 *
 * The service Pwm_SetPeriodAndDuty() updates the duty cycle and the period at the
 * end of the current PWM period.
 *
 * The passed dutycycle shall be between 0x0 (which is 0%) and
 * 0x8000 (which is 100%).
 *
 * The passed period are scaled in ticks.
 *
 * This service can be used only on channels that are configured with the
 * property #PWM_VARIABLE_PERIOD.

 * @warning The services Pwm_SetDutyCycle(), Pwm_SetPeriodAndDuty() and
 *          Pwm_SetOutputToIdle() are only reentrant for different channel
 *          identifiers.
 *
 * @param channel_id  Channel identifier for the PWM channel where the dutycycle shall be changed.
 * @param period      New period for the PWM channel [0x2 .. 0xFFFF].
 * @param dutycycle   New dutycycle for the PWM channel [0x0 .. 0x8000].
 */
extern void Pwm_SetPeriodAndDuty(Pwm_ChannelType channel_id, Pwm_PeriodType period, uint16 dutycycle);


/** This service sets the PWM output to the configured idle state immediately.
 *
 * The service Pwm_SetOutputToIdle() sets immediately the PWM output to the
 * configured inactive state (0% dutycycle). The idle state is defined in
 * the configuration of the Pwm channel.
 *
 * After the call of the service Pwm_SetOutputToIdle(), the Pwm
 * channel can be reactivated either using Pwm_SetPeriodAndDuty() to activate
 * the PWM channel with the new passed period or with Pwm_SetDutyCycle()
 * to activate the PWM channel with the period that was set before the
 * usage of Pwm_SetOutputToIdle().
 *
 * @warning The services Pwm_SetDutyCycle(), Pwm_SetPeriodAndDuty() and
 *          Pwm_SetOutputToIdle() are only reentrant for different channel
 *          identifiers.
 *
 * @param channel_id  Channel identifier of the Pwm channel.
 */
extern void Pwm_SetOutputToIdle(Pwm_ChannelType channel_id);


/** This service reads the internal state of the PWM output signal.
 *
 * This service returns the current state of the output pin. It is
 * independent from the configuration of Pwm_ConfigChannelType::low_active.
 *
 * @param channel_id  Channel identifier of the Pwm channel.
 * @return #PWM_HIGH: The PWM output state is high \n
 *         #PWM_LOW:  The PWM output state is low
 */
extern Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType channel_id);


/** This service disables the PWM signal edge notification.
 *
 * The enabled notifications of the Pwm channel are disabled after the
 * usage of this service.
 *
 * @param channel_id  Channel identifier of the Pwm channel.
 */
extern void Pwm_DisableNotification(Pwm_ChannelType channel_id);


/** This service enables the PWM signal edge notification.
 *
 * This service to enables the PWM signal edge notification according to
 * notification parameter.
 *
 * @param channel_id    Channel identifier of the Pwm channel.
 * @param notification  #PWM_RISING_EDGE: @copydoc #PWM_RISING_EDGE \n
 *                      #PWM_FALLING_EDGE: @copydoc #PWM_FALLING_EDGE\n
 *                      #PWM_BOTH_EDGES: @copydoc #PWM_BOTH_EDGES
 */
extern void Pwm_EnableNotification(Pwm_ChannelType channel_id, Pwm_EdgeNotificationType notification);



/** The function Pwm_GetVersionInfo returns the version information of the Pwm module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Pwm_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter PwmVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Pwm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = PWM_MODULE_ID;
	versioninfo->sw_major_version = PWM_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = PWM_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = PWM_SW_PATCH_VERSION;
	versioninfo->vendorID = PWM_VENDOR_ID;
}

/*@}*/

#define PWM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* PWM_H_ */
