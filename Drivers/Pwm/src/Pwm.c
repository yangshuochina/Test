/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Pwm.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:43MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Pwm.c  $
 *  Revision 1.2 2011/02/02 09:17:43MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.1 2010/05/19 14:20:06MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Pwm/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Pwm.h"
#include "Pwm_Priv.h"


#if defined(PWM_DEV_ERROR_DETECT) && (PWM_DEV_ERROR_DETECT == STD_ON)
#  define PWM_RESET_CONCURRENCY(ch) pwm_update[ch] = 0
#else
#  define PWM_RESET_CONCURRENCY(ch)
#endif

#define PWM_START_SEC_VAR_32BIT
#include "MemMap.h"

const Pwm_ConfigType *pwm_cfg;

#if defined(PWM_DEV_ERROR_DETECT) && (PWM_DEV_ERROR_DETECT == STD_ON)
uint32 pwm_update[PWM_MAX_CHANNELS];
#endif

#define PWM_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define PWM_START_SEC_CODE
#include "MemMap.h"


/*
 * cancel an update in progress - if none is pending nothing is performed
 */
static void Pwm_CancelUpdate(uint8 unit, uint8 ltc)
{
	if(LTCCTR(unit, ltc).B1.CUD) {
		/* update in progress */
		/* terminate it */
		/* write the bits CUD = 0, CUDCLR = 1 */
		ts_bfld((volatile uint32 *)&LTCCTR(unit, ltc).U, LTCCTR_CUDCLR | LTCCTR_CUD, LTCCTR_CUDCLR);
	}
}


/*
 * convert the passed dutycycle to ticks
 *
 * 0%   -> 0xFFFF   -> timer reset and dutycycle event is
 *                     occuring at the same time, since dutycycle
 *                     is the cell with the higher number the
 *                     dutycycle event wins.
 * 100% -> period+1 -> dutycycle event does never occur, this
 *                     means that the pin is controlled by the
 *                     period event only
 */
static inline uint16 Pwm_CalcDutyCycle(uint16 per, uint16 dutycycle)
{
	return (uint16)((((uint32)per * dutycycle) / 0x8000) - 1);
}


void Pwm_Init(const Pwm_ConfigType *cfg)
{
	const Pwm_ConfigChannelType *cfg_ch;
	int i;
	uint8 unit;
	uint8 ltc;
	uint32 l;
	uint32 h;
	uint16 dc;

	PWM_ASSERT_RETVOID(pwm_cfg == NULL_PTR, PWM_INIT_API_ID, PWM_E_ALREADY_INITIALIZED);
	PWM_ASSERT_RETVOID(cfg != NULL_PTR, PWM_INIT_API_ID, PWM_E_PARAM_CONFIG);
	PWM_ASSERT_RETVOID((cfg->channels != NULL_PTR) || (cfg->num_of_channels == 0), PWM_INIT_API_ID, PWM_E_PARAM_CONFIG);

	/* timer cells are operating at f_GPTA / 2^DFALTC = default is f_GPTA */
	cfg_ch = cfg->channels;
	for(i=0; i<cfg->num_of_channels; i++) {
		unit = cfg_ch->unit;
		ltc = cfg_ch->first_ltc;

		l = cfg_ch->low_active;
		h = !l;

		dc = Pwm_CalcDutyCycle(cfg_ch->period, cfg_ch->dutycycle);

		LTCXR(unit, ltc).U = cfg_ch->period - 3;
		LTCXR(unit, ltc+1).U = cfg_ch->period - 2;
		LTCXR(unit, ltc+3).U = cfg_ch->period - 2;
		LTCXR(unit, ltc+2).U = dc;
		LTCXR(unit, ltc+4).U = dc;

		/* period if SI = low, set pin to h state */
		LTCCTR(unit, ltc+1).U = LTCCTR_MOD_COM  |  /* compare mode */
		                        LTCCTR_GBYP     |  /* global bypass */
		                        LTCCTR_OCM(4)   |  /* out is forced h at internal event */
			                    LTCCTR_CEN      |  /* cell enable */
		                        LTCCTR_SOL;        /* enabled on SI = low */

		/* duty if SI = low, set pin to l state */
		LTCCTR(unit, ltc+2).U = LTCCTR_MOD_COM  |  /* compare mode */
		                        LTCCTR_GBYP     |  /* global bypass */
		                        LTCCTR_OCM(6 | l)   |  /* out is forced l at internal event or copy M1I/M0I */
			                    LTCCTR_CEN      |  /* cell enable */
		                        LTCCTR_SOL;        /* enabled on SI = low */

		/* period if SI = high, set pin to h state */
		LTCCTR(unit, ltc+3).U = LTCCTR_MOD_COM  |  /* compare mode */
		                        LTCCTR_GBYP     |  /* global bypass */
		                        LTCCTR_OCM(4)   |  /* out is forced h at internal event or copy M1I/M0I */
			                    LTCCTR_CEN      |  /* cell enable */
		                        LTCCTR_SOH;        /* enabled on SI = high */

		/* duty if SI = high, set pin to l state */
		LTCCTR(unit, ltc+4).U = LTCCTR_MOD_COM  |  /* compare mode */
		                        LTCCTR_GBYP     |  /* global bypass */
		                        LTCCTR_OCM(6 | l)   |  /* out is forced l at internal event or copy M1I/M0I */
			                    LTCCTR_CEN      |  /* cell enable */
		                        LTCCTR_SOH;        /* enabled on SI = high */

		/* reset timer for the above cells */
		LTCCTR(unit, ltc).U =   LTCCTR_MOD_RTIM |  /* reset timer */
		                        LTCCTR_GBYP     |  /* global bypass */
		                        LTCCTR_OCM(2 | h)   |  /* out is forced h at internal event */
		                        LTCCTR_CEN      |  /* cell enable */
		                        LTCCTR_RED;        /* inc. on rising edge */

		cfg_ch++;
	}

	pwm_cfg = cfg;
}


#if defined(PWM_DEINIT_API) && (PWM_DEINIT_API == STD_ON)

void Pwm_DeInit(void)
{
	Pwm_ChannelType i;
	uint8 unit;
	uint8 ltc;

	PWM_ASSERT_RETVOID(pwm_cfg != NULL_PTR, PWM_DEINIT_API_ID, PWM_E_UNINIT);

	for(i=0; i<pwm_cfg->num_of_channels; i++) {
		Pwm_SetOutputToIdle(i);

		unit = pwm_cfg->channels[i].unit;
		ltc = pwm_cfg->channels[i].first_ltc;

		LTCCTR(unit, ltc).U = 0;
		LTCCTR(unit, ltc+1).U = 0;
		LTCCTR(unit, ltc+2).U = 0;
		LTCCTR(unit, ltc+3).U = 0;
		LTCCTR(unit, ltc+4).U = 0;

		LTCXR(unit, ltc).U =   0;
		LTCXR(unit, ltc+1).U = 0;
		LTCXR(unit, ltc+3).U = 0;
		LTCXR(unit, ltc+2).U = 0;
		LTCXR(unit, ltc+4).U = 0;
	}

	pwm_cfg = NULL_PTR;
}

#endif


#if defined(PWM_SETDUTYCYCLE_API) && (PWM_SETDUTYCYCLE_API == STD_ON)

void Pwm_SetDutyCycle(Pwm_ChannelType channel_id, uint16 dutycycle)
{
	uint8 unit;
	uint8 ltc;
	uint8 ltc_per_shadow;
	uint8 ltc_per_active;
	uint16 per;
	uint16 duty;
	uint8 update_pending;

	PWM_ASSERT_RETVOID(pwm_cfg != NULL_PTR, PWM_SETDUTYCYCLE_API_ID, PWM_E_UNINIT);
	PWM_ASSERT_RETVOID(channel_id < pwm_cfg->num_of_channels, PWM_SETDUTYCYCLE_API_ID, PWM_E_PARAM_CHANNEL);
	PWM_ASSERT_RETVOID((unsigned int)__swap((unsigned int *)&pwm_update[channel_id], 1) == 0, PWM_SETDUTYCYCLE_API_ID, PWM_E_CONCURRENCY);

	unit = pwm_cfg->channels[channel_id].unit;
	ltc = pwm_cfg->channels[channel_id].first_ltc;

	if(LTCCTR(unit, ltc).B1.CUD) {
		/* there is currently a update pending */
		update_pending = 1;
		Pwm_CancelUpdate(unit, ltc);
	}
	else {
		update_pending = 0;
	}

	if(LTCCTR(unit, ltc+4).B2.SLL == 0) {
		/* SI is low */
		ltc_per_shadow = ltc + 3;
		ltc_per_active = ltc + 1;
	}
	else {
		/* SI is high */
		ltc_per_shadow = ltc + 1;
		ltc_per_active = ltc + 3;
	}

	if(update_pending) {
		/* if a PWM update is pending the period of the currently pending update must be used */
		per = LTCXR(unit, ltc_per_shadow).U + 2;
	}
	else {
		per = LTCXR(unit, ltc_per_active).U + 2;
	}

	duty = Pwm_CalcDutyCycle(per, dutycycle);

	LTCXR(unit, ltc_per_shadow).U = per-2;
	LTCXR(unit, ltc_per_shadow+1).U = duty;

	/* trigger the coherent update - switch over to the shadow at the next period */
	__putbit(1, (int *)&LTCCTR(unit, ltc).U, LTCCTR_CUD_BITNO);

	PWM_RESET_CONCURRENCY(channel_id);
}

#endif


#if defined(PWM_SETPERIODANDDUTYCYCLE_API) && (PWM_SETPERIODANDDUTYCYCLE_API == STD_ON)

void Pwm_SetPeriodAndDuty(Pwm_ChannelType channel_id, Pwm_PeriodType period, uint16 dutycycle)
{
	uint8 unit;
	uint8 ltc;
	uint8 ltc_per_shadow;
	uint16 duty;

	PWM_ASSERT_RETVOID(pwm_cfg != NULL_PTR, PWM_SETPERIODANDDUTY_API_ID, PWM_E_UNINIT);
	PWM_ASSERT_RETVOID(channel_id < pwm_cfg->num_of_channels, PWM_SETPERIODANDDUTY_API_ID, PWM_E_PARAM_CHANNEL);
	PWM_ASSERT_RETVOID(pwm_cfg->channels[channel_id].class == PWM_VARIABLE_PERIOD, PWM_SETPERIODANDDUTY_API_ID, PWM_E_PERIOD_UNCHANGEABLE);
	PWM_ASSERT_RETVOID((unsigned int)__swap((unsigned int *)&pwm_update[channel_id], 1) == 0, PWM_SETPERIODANDDUTY_API_ID, PWM_E_CONCURRENCY);

	unit = pwm_cfg->channels[channel_id].unit;
	ltc = pwm_cfg->channels[channel_id].first_ltc;

	Pwm_CancelUpdate(unit, ltc);

	if(LTCCTR(unit, ltc+4).B2.SLL == 0) {
		/* SI is low */
		ltc_per_shadow = ltc + 3;
	}
	else {
		/* SI is high */
		ltc_per_shadow = ltc + 1;
	}

	duty = Pwm_CalcDutyCycle(period, dutycycle);

	/* a dutycycle of 0% requires 0xFFFF in the data register */
	/* the timer resets to 0xFFFF */
	LTCXR(unit, ltc_per_shadow).U = period - 2;
	LTCXR(unit, ltc_per_shadow+1).U = duty;

	/* trigger the coherent update - switch over to the shadow at the next period */
	__putbit(1, (int *)&LTCCTR(unit, ltc).U, LTCCTR_CUD_BITNO);

	PWM_RESET_CONCURRENCY(channel_id);
}

#endif


#if (defined(PWM_SETOUTPUTSTATETOIDLE_API) && (PWM_SETOUTPUTSTATETOIDLE_API == STD_ON)) || defined(PWM_DEINIT_API) && (PWM_DEINIT_API == STD_ON)

void Pwm_SetOutputToIdle(Pwm_ChannelType channel_id)
{
	uint8 unit;
	uint8 ltc;

	PWM_ASSERT_RETVOID(pwm_cfg != NULL_PTR, PWM_SETOUTPUTTOIDLE_API_ID, PWM_E_UNINIT);
	PWM_ASSERT_RETVOID(channel_id < pwm_cfg->num_of_channels, PWM_SETOUTPUTTOIDLE_API_ID, PWM_E_PARAM_CHANNEL);
	PWM_ASSERT_RETVOID((unsigned int)__swap((unsigned int *)&pwm_update[channel_id], 1) == 0, PWM_SETOUTPUTTOIDLE_API_ID, PWM_E_CONCURRENCY);

	unit = pwm_cfg->channels[channel_id].unit;
	ltc = pwm_cfg->channels[channel_id].first_ltc;

	Pwm_CancelUpdate(unit, ltc);

	LTCXR(unit, ltc+2).U = 0xFFFF;
	LTCXR(unit, ltc+4).U = 0xFFFF;

	/* set pin to idle state */
	__putbit(1, (int *)&LTCCTR(unit, ltc+2).U, LTCCTR_OIA_BITNO);
	__putbit(1, (int *)&LTCCTR(unit, ltc+4).U, LTCCTR_OIA_BITNO);

	PWM_RESET_CONCURRENCY(channel_id);
}

#endif


#if defined(PWM_GETOUTPUTSTATE_API) && (PWM_GETOUTPUTSTATE_API == STD_ON)

Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType channel_id)
{
	uint8 unit;
	uint8 ltc;

	PWM_ASSERT_RETERR(pwm_cfg != NULL_PTR, PWM_GETOUTPUTSTATE_API_ID, PWM_E_UNINIT);
	PWM_ASSERT_RETERR(channel_id < pwm_cfg->num_of_channels, PWM_GETOUTPUTSTATE_API_ID, PWM_E_PARAM_CHANNEL);

	unit = pwm_cfg->channels[channel_id].unit;
	ltc = pwm_cfg->channels[channel_id].first_ltc;

	if(LTCCTR(unit, ltc+4).B2.OUT == 0) {
		return PWM_LOW;
	}
	else {
		return PWM_HIGH;
	}
}

#endif

#define PWM_STOP_SEC_CODE
#include "MemMap.h"

