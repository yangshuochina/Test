/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Pwm_PBcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/01 14:40:40MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Pwm_PBcfg.c  $
 *  Revision 1.3 2011/02/01 14:40:40MEZ Cotor Vlad RGB (COTORV) 
 *  - BMCHIL SW update with interface for HV interlock PWM output
 *  Revision 1.2 2010/11/25 09:37:51MEZ Cotor Vlad RGB (COTORV)
 *  Update BMCHIL: New trigger concept for MCs and current senso
 *  Revision 1.1 2010/05/20 11:29:05MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#include "Pwm.h"
#include "Prj_MainCfg.h"


#define PWM_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

static const Pwm_ConfigChannelType pwm_config_channels[] = {
		{
		      /* Channel #0 */
		      /* H-Bridge #1 PWM P5.1 OUT41 */
		      NULL_PTR,         /* notification. */
		      PWM_FIXED_PERIOD, /* Pwm_ChannelClassType class. */
		      PwmTicks,         /* Pwm_PeriodType period; 10 Hz*/
				0,                /* Pwm_PeriodType phase_shift; */
				0x0000,           /* uint16 dutycycle; */
				0,                /* uint8 low_active; */
				0,                /* uint8 unit; GPTA0 */
				8,                /* uint8 first_ltc; 8, 9, 10, 11, 12 */
		},
		{
		      /* Channel #1 */
		      /* H-Bridge #2 PWM P5.4 OUT44 */
				NULL_PTR,         /* notification. */
				PWM_FIXED_PERIOD, /* Pwm_ChannelClassType class. */
				PwmTicks,         /* Pwm_PeriodType period; 10 Hz */
				0,                /* Pwm_PeriodType phase_shift; */
				0x0000,           /* uint16 dutycycle; */
				0,                /* uint8 low_active; */
				0,                /* uint8 unit; GPTA0 */
				40,               /* uint8 first_ltc; 40, 41, 42, 43, 44 */
		},
		{
            /* Channel #2 */
		      /* High voltage interlock PWM output PWM P1.3 OUT19 */
            NULL_PTR,            /* notification. */
            PWM_VARIABLE_PERIOD, /* Pwm_ChannelClassType class. */
            PwmTicks,            /* Pwm_PeriodType period; 10 Hz */
            0,                   /* Pwm_PeriodType phase_shift; */
            0x0000,              /* uint16 dutycycle; */
            1,                   /* uint8 low_active; */
            0,                   /* uint8 unit; GPTA0 */
            15,                  /* uint8 first_ltc; 15, 16, 17, 18, 19 */
		},
} ;


const Pwm_ConfigType pwm_config = {
		pwm_config_channels,
		3
};


#define PWM_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
