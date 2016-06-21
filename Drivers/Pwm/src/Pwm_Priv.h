/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Pwm_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:20:10MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Pwm_Priv.h  $
 *  Revision 1.1 2010/05/19 14:20:10MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Pwm/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef PWM_PRIV_H_
#define PWM_PRIV_H_

#include "team-s.h"
#include "Pwm_Cfg.h"

/*
 * bitdefinitions for LTC
 */
#define LTCCTR_GBYP			BITPOS(16)
#define LTCCTR_OIA_BITNO	(14)
#define LTCCTR_OIA			BITPOS(LTCCTR_OIA_BITNO)
#define LTCCTR_OCM(v)		((v) << 11)
#define LTCCTR_CEN			BITPOS(10)
#define LTCCTR_CUD_BITNO	(9)			/* timer mode only */
#define LTCCTR_CUD			BITPOS(LTCCTR_CUD_BITNO)	/* timer mode only */
#define LTCCTR_SLL			BITPOS(9)	/* compare mode only */
#define LTCCTR_ILM			BITPOS(8)
#define LTCCTR_CUDCLR_BITNO	(7)
#define LTCCTR_CUDCLR		BITPOS(LTCCTR_CUDCLR_BITNO)	/* timer mode only */
#define LTCCTR_EOA			BITPOS(7)	/* compare mode only */
#define LTCCTR_SLO			BITPOS(6)	/* timer mode only */
#define LTCCTR_BYP			BITPOS(6)	/* compare mode only */
#define LTCCTR_FED			BITPOS(5)	/* timer mode only */
#define LTCCTR_SOH			BITPOS(5)	/* compare mode only */
#define LTCCTR_RED			BITPOS(4)	/* timer mode only */
#define LTCCTR_SOL			BITPOS(4)	/* compare mode only */
#define LTCCTR_REN_BITNO	(3)
#define LTCCTR_REN			BITPOS(LTCCTR_REN_BITNO)
#define LTCCTR_OSM			BITPOS(2)
#define LTCCTR_MOD_CAP		(0u)
#define LTCCTR_MOD_COM		(1u)
#define LTCCTR_MOD_TIM		(2u)
#define LTCCTR_MOD_RTIM		(3u)




#if defined(PWM_DEV_ERROR_DETECT) && (PWM_DEV_ERROR_DETECT == STD_ON)
#  define PWM_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, PWM_MODULE_ID, PWM_INSTANCE_ID, api_id, error_id)
#  define PWM_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, PWM_MODULE_ID, PWM_INSTANCE_ID, api_id, error_id)
#  define PWM_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, PWM_MODULE_ID, PWM_INSTANCE_ID, api_id, error_id)
#else
#  define PWM_ASSERT_RETERR(cond, api_id, error_id)
#  define PWM_ASSERT_RETVOID(cond, api_id, error_id)
#  define PWM_ASSERT(cond, api_id, error_id)
#endif

#define REG(reg, unit)  ((reg##_type *) ((volatile uint8 *)(&(reg)) + (0x1000 * (unit))))
#define LTCXR(unit, number)    (*(GPTA0_LTCXR00_type  *)((volatile uint8 *)&GPTA0_LTCXR00  + (unit)*0x1000 + (number) * 8))
#define LTCCTR(unit, number)   (*(GPTA0_LTCCTR00_type *)((volatile uint8 *)&GPTA0_LTCCTR00 + (unit)*0x1000 + (number) * 8))


#define PWM_START_SEC_VAR_32BIT
#include "MemMap.h"

extern const Pwm_ConfigType *pwm_cfg;

#define PWM_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#endif /* PWM_PRIV_H_ */


