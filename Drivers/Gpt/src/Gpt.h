/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Gpt.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:26:41MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Gpt.h  $
 *  Revision 1.4 2011/03/22 09:26:41MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.3 2010/11/22 08:29:05MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:46:55MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:17:23MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Gpt/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef GPT_H_
#define GPT_H_

#include "team-s.h"
#include "Std_Types.h"
#include "Gpt_Cfg.h"

#ifdef GPT_INCLUDE_WAKEUP_FUNCTIONALITY_API
#include "Ecum.h"
#endif


/** @defgroup group_gpt Gpt 
 *  @ingroup group_gpt */
/*@{*/

#define GPT_MODULE_ID         (100)
#define GPT_VENDOR_ID         (TEAMS_VENDOR_ID)
#define GPT_AR_MAJOR_VERSION  (2)
#define GPT_AR_MINOR_VERSION  (2)
#define GPT_AR_PATCH_VERSION  (2)
#define GPT_SW_MAJOR_VERSION  (1)
#define GPT_SW_MINOR_VERSION  (0)
#define GPT_SW_PATCH_VERSION  (0)


/* Driver APIs Service ID */
#define GPT_INIT_API_ID             	((uint8)(0x1)) /**< #Gpt_Init API ID used by error detection. */
#define GPT_DEINIT_API_ID             	((uint8)(0x2)) /**< #Gpt_DeInit API ID used by error detection. */
#define GPT_GET_TIME_ELAPSED_API_ID     ((uint8)(0x3)) /**< #Gpt_GetTimeElapsed API ID used by error detection. */
#define GPT_GET_TIME_REMAINING_API_ID   ((uint8)(0x4)) /**< #Gpt_GetTimeRemaining API ID used by error detection. */
#define GPT_START_TIMER_API_ID   		((uint8)(0x5)) /**< #Gpt_StartTimer API ID used by error detection. */
#define GPT_STOP_TIMER_API_ID   		((uint8)(0x6)) /**< #Gpt_StopTimer API ID used by error detection. */
#define GPT_ENABLE_NOTIFICATION_API_ID  ((uint8)(0x7)) /**< #Gpt_EnableNotification API ID used by error detection. */
#define GPT_DISABLE_NOTIFICATION_API_ID ((uint8)(0x8)) /**< #Gpt_DisableNotification API ID used by error detection. */
#define GPT_SET_MODE_API_ID 			((uint8)(0x9)) /**< #Gpt_SetMode API ID used by error detection. */
#define GPT_DISABLE_WAKEUP_API_ID 	 	((uint8)(0xA)) /**< #Gpt_DisableWakeup API ID used by error detection. */
#define GPT_ENABLE_WAKEUP_API_ID 	 	((uint8)(0xB)) /**< #Gpt_EnableWakeup API ID used by error detection. */
#define GPT_CBK_CHECK_WAKEUP_API_ID 	((uint8)(0xC)) /**< #Gpt_Cbk_CheckWakeup API ID used by error detection. */

/* Development Error Codes */
#define GPT_E_PARAM_CONFIG				((uint8)(0x09))	/**< A wrong parameter to function #Gpt_Init was passed. */
#define GPT_E_UNINIT					((uint8)(0x0A))	/**< A service was called without a prior #Gpt_Init. */
#define GPT_E_BUSY						((uint8)(0x0B)) /**< Function #Gpt_StartTimer or deInit called while timer is already running. */
#define GPT_E_NOT_STARTED               ((uint8)(0x0C)) /**< Operational function #Gpt_GetTimeElapsed or #Gpt_GetTimeRemaining called prior call of #Gpt_StartTimer function or after
timer has been stopped. */
#define GPT_E_ALREADY_INITIALIZED       ((uint8)(0x0D)) /**< API #Gpt_Init service called while the GPT driver has already been initialized. */
#define GPT_E_PARAM_CHANNEL             ((uint8)(0x14)) /**< Operational function #Gpt_GetTimeElapsed, #Gpt_GetTimeRemaining, #Gpt_StartTimer, #Gpt_StopTimer
 #Gpt_EnableNotification, #Gpt_DisableNotification, #Gpt_EnableWakeup, #Gpt_DisableWakeup called with invalid channel ID.
 #Gpt_EnableWakeup, #Gpt_DisableWakeup called on a non-wakeup capable channel. */
#define GPT_E_PARAM_VALUE               ((uint8)(0x15)) /**< Function #Gpt_StartTimer called with invalid value. */
#define GPT_E_PARAM_MODE                ((uint8)(0x1F)) /**< #Gpt_SetMode called with invalid mode parameter. */




/** @defgroup group_gpt_config Gpt Configuration
 *  @ingroup group_gpt
 *
 * This module contains the configuration of the Gpt - General Purpose Timer Module.
 *
 * The Port module contains two configuration parts.
 * @li \ref gpt_pct_config
 * @li \ref gpt_pbt_config
 */
/*@{*/
/*@}*/

/** @defgroup group_gpt_pbt_config Gpt Post Build Time Configuration
 *  @ingroup group_gpt_config
 *
 * @section gpt_pbt_intro Introduction
 *
 *  The post build time configuration \ref Gpt_ConfigType is passed to the gpt module during runtime.
 *
 *  The post build time configuration consists of the following parts:
 *  @li The Gpt channel configuration \ref Gpt_ConfigChannelType.
 *  @li The Gpt module configuration \ref Gpt_ConfigType.
 *
 * @section gpt_pbt_description Configuration Description
 *
 * The gpt module post build configuration is performed in the configuration file <b>Gpt_PBCfg.c</b>.
 *
 * The gpt module post build configuration is performed in the following steps.
 * <ol>
 *  <li>The gpt channel configuration is performed, see \ref gpt_channel_configuration_example.
 *  For each channel that is required an entry is made.
 *  The structure \ref Gpt_ConfigChannelType is used for configuration.
 *  </li>
 *  <li>The gpt module configuration is performed, see \ref gpt_configuration_example. The structure \ref Gpt_ConfigType is used for configuration.
 *  @li The number of gpt channels in the gpt channel configuration is inserted.
 *  @li The reference to the gpt channel configuration is inserted.
 *  </li>
 *
 *  <li> The interrupt configuration of each gpt channel (GTC cell respectively) must be performed.
 *   @li Each gpt channel requires a configured interrupt service for the used GTC cell of the channel.
 *   @li On the TC1767 2 GTC cells share a single interrupt node.
 *   @li The interrupt handler of the respective SRC (interrupt node) must call the function ts_decodeGTC()
 *       for the decoding of the correct interrupt handler of the GPT channel.
 *   @li It must be noted that the interrupt on SRC must be enabled by the startup, since
 *       the SRC is a shared resource this cannot be done by the GPT function.
 *   @li The assignment SRC to the GTC number can be found in the TC1767 users manual (Table 20-16 GPTA v5 Service Request Groups).
 *   @li See the configuration example for a better understanding \ref gpt_interrrupt_configuration_example.
 *
 *   @note Depending on the environment the interrupt might need to be configured in a different way e.g. in a OIL file.
 *
 * </li>
 *
 *  <li> The service \ref Gpt_Init is called and the configuration is passed as parameter.</li>
 * </ol>
 *
 * The following figure shows the hierarchy of the configuration.
 *
 * @image latex external_images/gpt_configuration.pdf "Gpt Configuration" width=14cm
 *
 *
 * @section configuration_example Configuration Example
 *
 * The following example code shows a possible configuration.
 *
 * @subsection gpt_channel_configuration_example Gpt Channel Configuration Example
 *
 * The example shows the configuration of 2 channels.
 *
 * The first entry configures the gtc timer cell GPTA0_GTC0 as gpt channel.
 * @li The function gpt_notify0 function shall be called at an event of this channel, see also \ref Gpt_EnableNotification.
 * @li The mode of the channel is continuous.
 * @li The channel operates to the timer base GPTA0_GT0.
 * @li GTC cell 0 (GPTA0_GTC0) is used for event generation.
 *
 * The second entry configures the gtc timer cell GPTA0_GTC31 as gpt channel.
 * @li The function gpt_notify1 function shall be called at an event of this channel, see also \ref Gpt_EnableNotification.
 * @li The mode of the channel is one shot.
 * @li The channel operates to the timer base GPTA0_GT1.
 * @li GTC cell 31 (GPTA0_GTC31) is used for event generation.
 *
 * @code
 *
 * ...
 *
 * static void gpt_notify0(void)
 * {
 *	 user function of channel 0 event
 * }
 *
 * static void gpt_notify1(void)
 * {
 *	user function of channel 1 event
 * }
 * ...
 *
 * const Gpt_ConfigChannelType gpt_channel_config[] = {
 *		{
 *			gpt_notify0,
 *			GPT_MODE_CONTINOUS,
 *			GPT_GT0,
 *			0,
 *		},
 *		{
 *			gpt_notify1,
 *			GPT_MODE_ONE_SHOT,
 *			GPT_GT1,
 *			31,
 *		},
 * };
 *
 * @endcode
 *
 * @subsection gpt_configuration_example Gpt Module Configuration Example
 * @code
 *
 * Gpt_ConfigType gpt_config = {
 *		sizeof(gpt_channel_config) / sizeof(Gpt_ConfigChannelType),
 *		gpt_channel_config
 * };
 * @endcode
 *
 *
 * @subsection gpt_interrrupt_configuration_example Gpt Module Interrupt Configuration Example
 *
 * The following example shows the configuration of the Gpt channel 0 and 1 that are located on
 * the GTC cells 0(SRPN_GPTA0_SRC06) and 31(SRPN_GPTA0_SRC21).
 *
 * The value behind the definition SRPN_GPTA0_SRC06 defines the interrupt level of the GPTA0_GTC0 cell.

 * The value behind the definition SRPN_GPTA0_SRC21 defines the interrupt level of the GPTA0_GTC31 cell.
 *
 * @code
 * void __interrupt(SRPN_GPTA0_SRC06) __enable_ Gpta0Src06(void)
 * {
 *	ts_decodeGTC(0);
 * }
 *
 * void __interrupt(SRPN_GPTA0_SRC21) __enable_ Gpta0Src21(void)
 * {
 *	ts_decodeGTC(30);
 * }
 * @endcode
 *
 * @subsection gpt_configuraion_file_example Complete Configuration Example
 *
 * The complete configuration is as follows.
 * \include Gpt_PBcfg.c
 *
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Gpt_Init(&gpt_config);
 *   ...
 * }
 * @endcode
 */

/* @dot
 *   digraph Gpt_Configuration {
 *     node [shape=plaintext fontsize="8.0"];
 *     rankdir=LR;
 *     Gpt_ConfigType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="type" BGCOLOR="grey">Gpt_ConfigType</td></tr>
 *                         <tr><td PORT="num_of_channels">uint8 num_of_channels</td></tr>
 *                         <tr><td PORT="channelCfg">const Gpt_ConfigChannelType *gpt_channel_config_set</td></tr>
 *       </table>>];
 *
 *     Gpt_ConfigChannelType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="grey">Gpt_ConfigChannelType</td></tr>
 *                         <tr><td PORT="notification">void (*notification)(void)</td></tr>
 *                         <tr><td PORT="gtc_mode">uint8 gtc_mode</td></tr>
 *                         <tr><td PORT="gtc_timer">uint8 gtc_timer</td></tr>
 *                         <tr><td PORT="gtc_cell">uint8 gtc_cell</td></tr>
 *       </table>>];
 *
 *     Gpt_ConfigType:channelCfg -> Gpt_ConfigChannelType:Type [label="array [1..num_of_channels]" fontsize="8.0"];
 *   }
 * @enddot
 */

/*@{*/
#define GPT_GT0  ((uint8)(0))  /**< GPTA Timer 0 definition (GT0), used to configure #Gpt_ConfigChannelType::gtc_timer. */
#define GPT_GT1  ((uint8)(1))  /**< GPTA Timer 1 definition (GT1), used to configure #Gpt_ConfigChannelType::gtc_timer. */

#define GPT_MODE_CONTINOUS ((uint8)(0)) /**< The timer channel operates in period mode,
used to configure #Gpt_ConfigChannelType::gtc_mode. */
#define GPT_MODE_ONE_SHOT ((uint8)(1)) /**< The timer channel operates in one shot mode,
used to configure #Gpt_ConfigChannelType::gtc_mode. */

typedef enum {
	GPT_MODE_NORMAL,  /**<  Normal operation mode of the GPT. */
	GPT_MODE_SLEEP    /**<  Operation for reduced power operation mode. In Wakeup mode only wakeup capable channels are available.@warning This
	mode is not supported. */

}Gpt_ModeType;
/*@}*/


/**
 * This type definition is used for reading current timer value/setting
 * of periodic timer values (in number of ticks) up to hours.
 */
typedef uint32 Gpt_ValueType;

/**
 * Type definition for a numeric ID of a GPT channel.
 */
typedef uint32 Gpt_ChannelType;


/**
 * @ingroup group_gpt_pbt_config
 *
 * This container contains the configuration set of a GPT channel.
 */
typedef struct {
	void (*notification)(void);	 /**< This function get called for the notification; see Gpt_EnableNotification(). */
	uint8 gtc_mode;				 /**< Selects the mode of the timer channel #GPT_MODE_CONTINOUS, #GPT_MODE_ONE_SHOT. */
	uint8 gtc_timer;			 /**< The timer used for the GTC cell as base timer #GPT_GT0 or #GPT_GT1. */
	uint8 gtc_cell;              /**< This member configures the GTC cell to use for the timer channel, where a value of 0 is GPTA0_GTC0...
	a value of 31 is GPTA0_GTC31 . */
} Gpt_ConfigChannelType;

/**
 * @ingroup group_gpt_pbt_config
 *
 * This is the Gpt module configuration structure.
 */
typedef struct {
	uint8 num_of_channels;                               /**< The number of timer channels #gpt_channel_config_set contained. */
	const Gpt_ConfigChannelType *gpt_channel_config_set; /**< Array containing the timer channel configuration. */
} Gpt_ConfigType;

#define GPT_START_SEC_CODE
#include "MemMap.h"


/** This service initializes the Gpt driver.
 *
 * The function Gpt_Init initializes the Gpt module.
 *
 * @param cfg Pointer to a selected configuration structure.
 *  *
 * @note After the execution of the function Gpt_Init(), the configuration data are accessible
 * and can be used by the Gpt module functions.
 */
extern void Gpt_Init(const Gpt_ConfigType *cfg);


/** The function Gpt_GetVersionInfo returns the version information of the Gpt module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Gpt_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter GptVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Gpt_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = GPT_MODULE_ID;
	versioninfo->sw_major_version = GPT_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = GPT_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = GPT_SW_PATCH_VERSION;
	versioninfo->vendorID = GPT_VENDOR_ID;
}

/**
 * This service deinitializes all hardware timer channels.
 *
 */
extern void Gpt_DeInit(void);

/**
 * This time returns the time already elapsed of a channel.
 *
 * @param channel Numeric identifier of the GPT channel.
 * @return Elapsed timer value (in number of ticks).
 *
 */
extern Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType channel);

/**
 * This service gets the time remaining until the next timeout period will expire.
 * @param channel Numeric identifier of the GPT channel.
 * @return The remaining timer value (in number of ticks).
 *
 */
extern Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType channel);

/**
 * This service starts a timer channel.
 * @param channel Numeric identifier of the GPT channel.
 * @param value Timeout period (in number of ticks) after a notification shall occur.
 */

extern void Gpt_StartTimer(Gpt_ChannelType channel, Gpt_ValueType value);

/**
 * This service stops a timer channel.
 * @param channel Numeric identifier of the GPT channel.
 *
 */
extern void Gpt_StopTimer(Gpt_ChannelType channel);

/**
 * This service enables the notification of a channel.
 * @param channel Numeric identifier of the GPT channel.
 */
extern void Gpt_EnableNotification(Gpt_ChannelType channel);

/**
 * This service disbles the notification of a channel.
 * @param channel Numeric identifier of the GPT channel.
 */
extern void Gpt_DisableNotification(Gpt_ChannelType channel);

/**
 * This service sets the operation mode of the GPT.
 * @param mode  #GPT_MODE_NORMAL, #GPT_MODE_SLEEP:
 *
 * @note In mode #GPT_MODE_SLEEP the operation for reduced power operation mode is activated. In
 * Wakeup mode only wakeup capable channels are capable of generating interrupts.
 *
 * @warning #GPT_MODE_SLEEP is not supported.
 *
 */
extern void Gpt_SetMode(Gpt_ModeType mode);

/**
 * This service disables the wakeup interrupt invocation of a channel.
 * @param channel Numeric identifier of the GPT channel.
 *
 * @warning Wakeup and #GPT_MODE_SLEEP is not supported.
 */
extern void Gpt_DisableWakeup(Gpt_ChannelType channel);

/**
 * This service enables the wakeup interrupt invocation of a channel.
 * @param channel Numeric identifier of the GPT channel.
 *
 * @warning Wakeup and #GPT_MODE_SLEEP is not supported.
 */
extern void Gpt_EnableWakeup(Gpt_ChannelType channel);

/**
 * This service checks if a wakeup capable GPT channel is the source for a wakeup event and
 * calls the ECU state manager service EcuM_SetWakeupEvent in case of a valid
 * GPT channel wakeup event.
 *
 * @param wakeupSource  Information on wakeup source to be checked. The associated GPT channel can be determined from configuration data.
 *
 *
 */
#ifdef GPT_INCLUDE_WAKEUP_FUNCTIONALITY_API
extern void Gpt_Cbk_CheckWakeup(EcuM_WakeupSourceType wakeupSource);
#endif

/*@}*/

#define GPT_STOP_SEC_CODE
#include "MemMap.h"


#endif /* GPT_H_ */
