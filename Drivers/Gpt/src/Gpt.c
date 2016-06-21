/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Gpt.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:29:13MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Gpt.c  $
 *  Revision 1.3 2010/11/22 08:29:13MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:46:41MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:17:21MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Gpt/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Gpt.h"
#include "Gpt_Priv.h"
#include "SchM_Gpt.h"

#define GTC_CELL(channel) (gpt_cfg->gpt_channel_config_set[channel].gtc_cell)

/* GTC control register active mode MOD=compare + timer, OSM = 1, REN = 1, GES = 1, clear EOA*/
#define GTC_ACTIVE_MODE(channel) ((0x2u | gpt_cfg->gpt_channel_config_set[channel].gtc_timer)| (1u << 2) | (1u << 3) | (1u << 4))

typedef struct {
	Gpt_ValueType timeout; /**< The timeout of the channel. */
	Gpt_ValueType event;   /**< The timer value at the timeout event of the channel. */
}ChannelDataType;


#define GPT_START_SEC_VAR_32BIT
#include "MemMap.h"
/**
 * Reference to active configuration.
 */
static const Gpt_ConfigType* gpt_cfg;

/**
 * Local channel data.
 */
static ChannelDataType gpt_channel_data[GPT_MAX_TIMER_CHANNELS];

/**
 * Bitfield containing the started channels.
 */
static BITFIELD32(gpt_channels_started);

/**
 * Bitfield containing the enabled notifications.
 */
static BITFIELD32(gpt_channel_notify);

/**
 * Bitfield containing the occured timeouts.
 */
static BITFIELD32(gpt_channel_timeout_event);

#define GPT_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define GPT_START_SEC_CODE
#include "MemMap.h"

static Gpt_ValueType getTimeRemaining(Gpt_ChannelType channel) {
	uint8 gtc_cell;
	uint8 gtc_timer;
	Gpt_ValueType timer_value;
	Gpt_ValueType result;
	Gpt_ValueType event;

	gtc_cell = GTC_CELL(channel);
	gtc_timer = gpt_cfg->gpt_channel_config_set[channel].gtc_timer;

	SchM_Enter_Gpt(GPT_MODULE_ID, 0);

	/* read current timer value */
	if(gtc_timer == 0) {
		/* GT0 */
		timer_value = GPTA0_GTTIM0.U;
	} else {
		/* GT1 */
		timer_value = GPTA0_GTTIM1.U;
	}

	/* read current event time */
	event = gpt_channel_data[channel].event;

	SchM_Exit_Gpt(GPT_MODULE_ID, 0);

	if(gpt_cfg->gpt_channel_config_set[channel].gtc_mode == GPT_MODE_ONE_SHOT) {
		/* GPT_MODE_ONE_SHOT */
		/* a pending interrupt request indicates, that time has elapsed */
		if((ts_getReqGTC(gtc_cell) > 0) || (BITFIELD32_GETBIT(gpt_channel_timeout_event, channel) >0)) {
			/* return zero */
			return 0;
		}
	} else {
		/* GPT_MODE_CONTINUOUS */
		if((sint32)((event << 8) - (timer_value << 8)) < 0) {
			/* event occurred already, but not yet serviced - use next event time */
			event = event + gpt_channel_data[channel].timeout;
		}
	}

	result = (event - timer_value) & 0xFFFFFF;

	return result;
}

/**
 * @ingroup group_gpt
 * This routine is the interrupt handler routine that is called at each Gpt event.
 * @param gtc_cell The number of the GTC cell that raised the interrupt, where GPTA0_GTC0 is 0...GPTA0_GTC31 is 31.
 * @param param The logical number of the channel, where 0 corresponds to the first entry in the Gpt Channel Configuration
 * \ref Gpt_ConfigChannelType.
 */
static void Gpt_Notify(uint32 gtc_cell, uint8 param) {
	Gpt_ChannelType channel;
	Gpt_ValueType compare_value;

	channel = param;

	/* service interrupt only if enabled - controller issue */
	if(GTCCTR(gtc_cell).B2.REN == 1) {
		BITFIELD32_SETBIT(gpt_channel_timeout_event, param);

		/* this is required for returning 0 at Gpt_GetTimeRemaining and */
		/* Gpt_GetTimeElapsed service routines                          */

		if(gpt_cfg->gpt_channel_config_set[channel].gtc_mode == GPT_MODE_CONTINOUS) {
			compare_value = (gpt_channel_data[channel].event + gpt_channel_data[channel].timeout) & 0xFFFFFF;
			GTCXR(gtc_cell).U = compare_value;
			GTCCTR(gtc_cell).U = GTC_ACTIVE_MODE(channel);
			gpt_channel_data[channel].event = compare_value;
		}

		/* call notification if enabled */
		if(BITFIELD32_GETBIT(gpt_channel_notify, param) > 0) {
			gpt_cfg->gpt_channel_config_set[channel].notification();
		}
	}
}


void Gpt_Init(const Gpt_ConfigType *cfg) {

	uint8 i;
	const Gpt_ConfigChannelType *cfg_ch;


	/* check for a valid configuration */
	GPT_ASSERT_RETVOID(cfg != NULL_PTR, GPT_INIT_API_ID, GPT_E_PARAM_CONFIG);
	GPT_ASSERT_RETVOID(gpt_cfg == NULL_PTR, GPT_INIT_API_ID, GPT_E_ALREADY_INITIALIZED);

	/* store reference to configuration */
	gpt_cfg = cfg;

	/* setup all channels */
	cfg_ch = cfg->gpt_channel_config_set;
	for(i=0; i<cfg->num_of_channels; i++) {
		/* register interrupt handler*/
		ts_registerGTC(cfg_ch[i].gtc_cell, i, Gpt_Notify);
	}
}

#ifdef GPT_INCLUDE_DEINIT_API
void Gpt_DeInit(void) {
	const Gpt_ConfigChannelType *cfg_ch;
	int i;

	GPT_ASSERT_RETVOID(gpt_cfg != NULL_PTR, GPT_DEINIT_API_ID, GPT_E_UNINIT);
	GPT_ASSERT_RETVOID(BITFIELD32_IS_CLEAR(gpt_channels_started), GPT_DEINIT_API_ID, GPT_E_BUSY);

	cfg_ch = gpt_cfg->gpt_channel_config_set;
	for(i=0; i<gpt_cfg->num_of_channels; i++) {
		GTCCTR(cfg_ch[i].gtc_cell).U = 0;
		GTCXR(cfg_ch[i].gtc_cell).U = 0;
		/* clear a pending interrupt request */
		ts_clrReqGTC(cfg_ch[i].gtc_cell);
	}
	gpt_cfg = NULL_PTR;
	gpt_channels_started = 0;
	gpt_channel_notify = 0;
	gpt_channel_timeout_event = 0;
}
#endif

#ifdef GPT_INCLUDE_IME_ELAPSED_API
Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType channel) {
	Gpt_ValueType result;

	GPT_ASSERT_RETVAL(gpt_cfg != NULL_PTR, GPT_GET_TIME_ELAPSED_API_ID, GPT_E_UNINIT, 0);
	GPT_ASSERT_RETVAL(channel < gpt_cfg->num_of_channels, GPT_GET_TIME_ELAPSED_API_ID, GPT_E_PARAM_CHANNEL, 0);
	GPT_ASSERT_RETVAL(BITFIELD32_GETBIT(gpt_channels_started, channel) == 1, GPT_GET_TIME_ELAPSED_API_ID, GPT_E_NOT_STARTED, 0);

	if(BITFIELD32_GETBIT(gpt_channels_started, channel) == 0) {
		return 0;
	}

	result = getTimeRemaining(channel);

	if(result == 0) {
		return 0;
	}

	result = gpt_channel_data[channel].timeout - result;

	return result;
}
#endif

#ifdef GPT_INCLUDE_TIME_REMAINING_API
Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType channel) {
	Gpt_ValueType result;

	GPT_ASSERT_RETVAL(gpt_cfg != NULL_PTR, GPT_GET_TIME_REMAINING_API_ID, GPT_E_UNINIT, 0);
	GPT_ASSERT_RETVAL(channel < gpt_cfg->num_of_channels, GPT_GET_TIME_REMAINING_API_ID, GPT_E_PARAM_CHANNEL, 0);
	GPT_ASSERT_RETVAL(BITFIELD32_GETBIT(gpt_channels_started, channel) == 1, GPT_GET_TIME_REMAINING_API_ID, GPT_E_NOT_STARTED, 0);

	if(BITFIELD32_GETBIT(gpt_channels_started, channel) == 0) {
		return 0;
	}

	result = getTimeRemaining(channel);

	return result;
}
#endif

void Gpt_StartTimer(Gpt_ChannelType channel, Gpt_ValueType value) {
	Gpt_ValueType compare_value;
	Gpt_ValueType timer_value;
	uint8 gtc_cell;
	uint8 gtc_timer;

	GPT_ASSERT_RETVOID(gpt_cfg != NULL_PTR, GPT_START_TIMER_API_ID, GPT_E_UNINIT);
	GPT_ASSERT_RETVOID(BITFIELD32_GETBIT(gpt_channels_started, channel) == 0, GPT_START_TIMER_API_ID, GPT_E_BUSY);
	GPT_ASSERT_RETVOID(channel < gpt_cfg->num_of_channels, GPT_START_TIMER_API_ID, GPT_E_PARAM_CHANNEL);
	GPT_ASSERT_RETVOID(value <= 0x7FFFFF, GPT_START_TIMER_API_ID, GPT_E_PARAM_VALUE);

	gtc_cell = GTC_CELL(channel);
	gtc_timer = gpt_cfg->gpt_channel_config_set[channel].gtc_timer;


	if(gtc_timer == 0) {
		/* GT0 */
		timer_value = GPTA0_GTTIM0.U;
	} else {
		/* GT1 */
		timer_value = GPTA0_GTTIM1.U;
	}

	compare_value = (timer_value + value) & 0xFFFFFFu;

	GTCXR(gtc_cell).U = compare_value;

	BITFIELD32_SETBIT(gpt_channels_started, channel);
	BITFIELD32_CLRBIT(gpt_channel_timeout_event, channel);

	gpt_channel_data[channel].event = compare_value;
	gpt_channel_data[channel].timeout = value;

	/* set control register */
	GTCCTR(gtc_cell).U = GTC_ACTIVE_MODE(channel);
	GTCXR(gtc_cell).U = compare_value;
}

void Gpt_StopTimer(Gpt_ChannelType channel) {
	uint8 gtc_cell;

	GPT_ASSERT_RETVOID(gpt_cfg != NULL_PTR, GPT_STOP_TIMER_API_ID, GPT_E_UNINIT);
	GPT_ASSERT_RETVOID(channel < gpt_cfg->num_of_channels, GPT_STOP_TIMER_API_ID, GPT_E_PARAM_CHANNEL);

	gtc_cell = GTC_CELL(channel);

	GTCCTR(gtc_cell).U = 0;

	BITFIELD32_CLRBIT(gpt_channels_started, channel);

	/* clear a pending request */
	ts_clrReqGTC(gtc_cell);
}

#ifdef GPT_INCLUDE_ENABLE_DISABLE_NOTIFICATION_API
void Gpt_EnableNotification(Gpt_ChannelType channel) {
	GPT_ASSERT_RETVOID(gpt_cfg != NULL_PTR, GPT_ENABLE_NOTIFICATION_API_ID, GPT_E_UNINIT);
	GPT_ASSERT_RETVOID(channel < gpt_cfg->num_of_channels, GPT_ENABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL);

	BITFIELD32_SETBIT(gpt_channel_notify, channel);
}

void Gpt_DisableNotification(Gpt_ChannelType channel) {
	GPT_ASSERT_RETVOID(gpt_cfg != NULL_PTR, GPT_DISABLE_NOTIFICATION_API_ID, GPT_E_UNINIT);
	GPT_ASSERT_RETVOID(channel < gpt_cfg->num_of_channels, GPT_DISABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL);

	BITFIELD32_CLRBIT(gpt_channel_notify, channel);
}
#endif

#ifdef GPT_INCLUDE_WAKEUP_FUNCTIONALITY_API
void Gpt_SetMode(Gpt_ModeType mode) {

}

void Gpt_DisableWakeup(Gpt_ChannelType channel) {

}


void Gpt_EnableWakeup(Gpt_ChannelType channel) {

}

#ifdef GPT_REPORT_WAKEUP_SOURCE
void Gpt_Cbk_CheckWakeup(EcuM_WakeupSourceType wakeupSource) {

}
#endif

#endif

#define GPT_STOP_SEC_CODE
#include "MemMap.h"


