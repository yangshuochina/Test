/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Pwm_Notify.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:47MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Pwm_Notify.c  $
 *  Revision 1.2 2011/02/02 09:17:47MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.1 2010/05/19 14:20:09MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Pwm/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Pwm.h"
#include "Pwm_Priv.h"

#define PWM_START_SEC_CODE
#include "MemMap.h"

#if defined(PWM_NOTIFICATIONSUPPORTED) && (PWM_NOTIFICATIONSUPPORTED == STD_ON)


/* interrupt handling */
static void Pwm_Notify(uint32 ltc_no, uint8 param)
{
	Pwm_ChannelType channel_id = param;
	uint32 ltc;
	uint32 unit;

	ltc = ltc_no;
	if(ltc > 64) {
		unit = 1;
		ltc -= 64;
	}
	else {
		unit = 0;
	}

	if(LTCCTR(unit, ltc).B1.REN != 0) {
		/* call the notification if the interrupt is enabled */
		pwm_cfg->channels[channel_id].notification();
	}
}


/* enable the interrupt for the LTC cell. */
static void Pwm_EnableInt(Pwm_ChannelType channel_id, uint8 unit, uint8 ltc)
{
	if(LTCCTR(unit, ltc).B2.REN == 0) {
		/* clear a pending request */
		ts_clrReqLTC(ltc + unit*64);

		/* register interrupt handler and enable the interrupts */
		ts_registerLTC(ltc + unit*64, channel_id, Pwm_Notify);
		__putbit(1, (int *)&LTCCTR(unit, ltc).U, LTCCTR_REN_BITNO);
	}
}


void Pwm_EnableNotification(Pwm_ChannelType channel_id, Pwm_EdgeNotificationType notification)
{
	uint8 unit;
	uint8 ltc;
	uint8 i;
	uint8 ocm;

	PWM_ASSERT_RETVOID(pwm_cfg != NULL_PTR, PWM_ENABLENOTIFICATION_API_ID, PWM_E_UNINIT);
	PWM_ASSERT_RETVOID(channel_id < pwm_cfg->num_of_channels, PWM_ENABLENOTIFICATION_API_ID, PWM_E_PARAM_CHANNEL);
	PWM_ASSERT_RETVOID(pwm_cfg->channels[channel_id].notification != NULL_PTR, PWM_ENABLENOTIFICATION_API_ID, PWM_E_PARAM_CHANNEL);

	unit = pwm_cfg->channels[channel_id].unit;
	ltc = pwm_cfg->channels[channel_id].first_ltc;

	for(i=0; i<5; i++) {
		ocm = LTCCTR(unit, ltc+i).B2.OCM & 0x3u;

		if(ocm == 2) {
			/* LTC sets pin to low */

			if((notification == PWM_FALLING_EDGE) || (notification == PWM_BOTH_EDGES)) {
				/* enable it */
				Pwm_EnableInt(channel_id, unit, ltc + i);
			}
			else {
				/* disable it */
				__putbit(0, (int *)&LTCCTR(unit, ltc+i).U, LTCCTR_REN_BITNO);
			}
		}
		else if(ocm == 3) {
			/* LTC sets pin to high */
			if((notification == PWM_RISING_EDGE) || (notification == PWM_BOTH_EDGES)) {
				/* enable it */
				Pwm_EnableInt(channel_id, unit, ltc + i);
			}
			else {
				/* disable it */
				__putbit(0, (int *)&LTCCTR(unit, ltc+i).U, LTCCTR_REN_BITNO);
			}
		}
	}
}


#if defined(PWM_DISABLENOTIFICATION_API) && (PWM_DISABLENOTIFICATION_API == STD_ON)

void Pwm_DisableNotification(Pwm_ChannelType channel_id)
{
	uint8 unit;
	uint8 ltc;
	uint8 i;

	PWM_ASSERT_RETVOID(pwm_cfg != NULL_PTR, PWM_DISABLENOTIFICATION_API_ID, PWM_E_UNINIT);
	PWM_ASSERT_RETVOID(channel_id < pwm_cfg->num_of_channels, PWM_DISABLENOTIFICATION_API_ID, PWM_E_PARAM_CHANNEL);

	unit = pwm_cfg->channels[channel_id].unit;
	ltc = pwm_cfg->channels[channel_id].first_ltc;

	for(i=0; i<5; i++) {
		/* disable it */
		__putbit(0, (int *)&LTCCTR(unit, ltc+i).U, LTCCTR_REN_BITNO);
	}
}

#endif

#endif

#define PWM_STOP_SEC_CODE
#include "MemMap.h"
