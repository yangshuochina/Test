/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Wdg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:23:36MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Wdg.c  $
 *  Revision 1.1 2010/05/19 14:23:36MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Wdg/AA000/src/project.pj
 *
 * ***************************************************************************/

/** @ingroup Wdg */

#include "Dem.h"
#include "Wdg.h"
#include "Wdg_Priv.h"
#include "Os.h"

/* Version checking */
#define WDG_SW_MAJOR_VERSION_IN_C  (1)
#define WDG_SW_MINOR_VERSION_IN_C  (0)
#define WDG_SW_PATCH_VERSION_IN_C  (0)

#define WDG_AR_MAJOR_VERSION_IN_C  (2)
#define WDG_AR_MINOR_VERSION_IN_C  (2)
#define WDG_AR_PATCH_VERSION_IN_C  (2)

#if ( WDG_SW_MAJOR_VERSION != WDG_SW_MAJOR_VERSION_IN_C )
    #error Incompatible WDG SW Major Version
#endif

#if ( WDG_SW_MINOR_VERSION != WDG_SW_MINOR_VERSION_IN_C )
    #error Incompatible WDG SW Minor Version
#endif

#if ( WDG_SW_PATCH_VERSION != WDG_SW_PATCH_VERSION_IN_C )
    #error Incompatible WDG SW Patch Version
#endif

#if ( WDG_AR_MAJOR_VERSION != WDG_AR_MAJOR_VERSION_IN_C )
    #error Incompatible WDG AR Major Version
#endif

#if ( WDG_AR_MINOR_VERSION != WDG_AR_MINOR_VERSION_IN_C )
    #error Incompatible WDG AR Minor Version
#endif

#if ( WDG_AR_PATCH_VERSION != WDG_AR_PATCH_VERSION_IN_C )
    #error Incompatible WDG AR Patch Version
#endif

#if defined(WDG_DEV_ERROR_DETECT) && (WDG_DEV_ERROR_DETECT == STD_ON)
	#define WDG_START_SEC_VAR_8BIT
	#include "MemMap.h"
	static volatile WdgIf_StatusType wdg_state = WDGIF_UNINIT;
	#define WDG_STOP_SEC_VAR_8BIT
	#include "MemMap.h"
#endif

#define WDG_START_SEC_CODE
#include "MemMap.h"

static inline void WDG_PasswordAccess(void)
{
	uint32 con0;
	uint32 con1;

	con0 = WDT_CON0.U;
	con1 = WDT_CON1.U;
	/* clr LCK[0], HPW0[2,3] */
	con0 &= (uint32)~(uint32)(BITPOS(1U) | BITPOS(2U) | BITPOS(3U));
	/* set HPW1[4-7] and copy con1 IR[2], DR[3] */
	con0 |= (uint32)((uint32)0xF0U | (con1 & (uint32)(BITPOS(2) | BITPOS(3))));
	WDT_CON0.U = con0;
}

/*
 * if the parameter in WDG_ModifyAccess is
 * 0xF2: clear end init bit
 * 0xF3: set end init bit
 */
static inline void WDG_ModifyAccess(uint8 end_init_bit)
{
	volatile uint32 tmp;
	/* keep the old PW and REL value*/
	WDT_CON0.U = ((WDT_CON0.U & 0xFFFFFF00U)|end_init_bit);
	/* dummy read as recommended by manual pg:4-75 V1.4_2007*/
	tmp = WDT_CON0.U;
}

inline void WDG_Update_CON1(WdgIf_ModeType tmp_mode)
{

	switch (tmp_mode)
	{
		case WDGIF_FAST_MODE:
			WDT_CON1.U = WDG_SETTINGS_FAST;
			break;

		case WDGIF_SLOW_MODE:
			WDT_CON1.U = WDG_SETTINGS_SLOW;
			break;

		case WDGIF_OFF_MODE:
			WDT_CON1.U = WDG_SETTINGS_OFF;
			break;

		default:
			/* this condition is covered in function entry */
			break;

	}

}

void Wdg_Init(const  Wdg_ConfigType *cfg)
{
	uint32 con0;
	/* is config PTR is NULL? */
	WDG_ASSERT_RETVOID(!(cfg == NULL_PTR), WDG_INIT_API_ID, WDG_E_PARAM_CONFIG);
	/* is config PTR for internal WDG is NULL? */
	WDG_ASSERT_RETVOID(!(cfg->int_wdg == NULL_PTR), WDG_INIT_API_ID, WDG_E_PARAM_CONFIG);
	/* check if the predefined Mode is not configured */
	WDG_ASSERT_RETVOID(!(cfg->int_wdg->init_mode > 2), WDG_INIT_API_ID, WDG_E_PARAM_CONFIG);
	/* no check on timeout is needed as it's size fits to available register size*/
	/* raise error if WDG Disable is not allowed but configured? raise production error*/
	if((WDG_DISABLE_ALLOWED == STD_OFF) && (cfg->int_wdg->init_mode == WDGIF_OFF_MODE))
	{
		Dem_ReportErrorStatus(WDG_E_DISABLE_REJECTED, DEM_EVENT_STATUS_FAILED);
		return;
	}
	con0 = (uint32)((uint32)((uint32)0x10000U - (uint32)cfg->int_wdg->wdg_timeout) << 16);
	con0 |= (uint32)(((uint32)cfg->int_wdg->pw) <<8);
	con0 |= (uint32)0xF3U;
	con0 &= 0xFFFFFFF3U;
	WDG_PasswordAccess();
	WDG_ModifyAccess(0xF2U);
	/* set the WDG Clock pre scaler*/
	WDG_Update_CON1(cfg->int_wdg->init_mode);
	WDG_PasswordAccess();
	WDT_CON0.U =  con0; /* modify access */
	WDG_UPDATE_STATUS(WDGIF_IDLE);
}

Std_ReturnType Wdg_SetMode(WdgIf_ModeType Mode)
{
	uint32 sr;
	Std_ReturnType tmp_status;
	/* check if the predefined Mode is passed */
	WDG_ASSERT_RETERR(!(Mode > 2), WDG_SETMODE_API_ID, WDG_E_PARAM_MODE);
	/* check if the WDG is initialized */
	WDG_ASSERT_RETERR(!(wdg_state == WDGIF_UNINIT), WDG_SETMODE_API_ID, WDG_E_NOT_INITIALIZED);
	/* check if the WDG is free */
	WDG_ASSERT_RETERR(!(wdg_state == WDGIF_BUSY), WDG_SETMODE_API_ID, WDG_E_DRIVER_STATE);

	if((WDG_DISABLE_ALLOWED == STD_OFF) && (Mode == WDGIF_OFF_MODE))
	{
		Dem_ReportErrorStatus(WDG_E_DISABLE_REJECTED, DEM_EVENT_STATUS_FAILED);
		return E_NOT_OK;
	}

	WDG_UPDATE_STATUS(WDGIF_BUSY);
	SuspendAllInterrupts();
	/* protect the sequence during WDG mode change */
	ts_endinit_clear();
	WDG_Update_CON1(Mode);
	ts_endinit_set();
	ResumeAllInterrupts();

	tmp_status = E_NOT_OK;
	sr = WDT_SR.U;
	/* check the status register either pre scale/disable bit is set/not? */
	switch (Mode)
	{
		case WDGIF_FAST_MODE: /* F of fpi/256 */
			sr &= BITPOS(2);
			if (sr == BITPOS(2))
			{
				tmp_status = E_OK;
			}
			break;
		case WDGIF_SLOW_MODE: /* F of fpi/16384 */
			sr &= BITPOS(2);
			if (sr == 0)
			{
				tmp_status = E_OK;
			}
			break;
		case WDGIF_OFF_MODE:
			sr &= BITPOS(3);
			if (sr == BITPOS(3))
			{
				tmp_status = E_OK;
			}
			break;
		default:
			/* this will not occur as it is
			 * addressed in Function entry itself
			 */
			break;
	}
	/* if mode change was not taken by the micro */
	if(tmp_status == E_NOT_OK)
	{
		Dem_ReportErrorStatus(WDG_E_MODE_SWITCH_FAILED, DEM_EVENT_STATUS_FAILED);
		return E_NOT_OK;
	}
	/* change the state irrespective of development error detection*/
	WDG_UPDATE_STATUS(WDGIF_IDLE);
	return tmp_status;
}
#define WDG_STOP_SEC_CODE
#include "MemMap.h"

#define WDG_START_SEC_CODE_TRIGGER
#include "MemMap.h"

void Wdg_Trigger(void)
{
	volatile uint32 icr;
	/* check if the WDG is initialized */
	WDG_ASSERT_RETVOID(!(wdg_state == WDGIF_UNINIT), WDG_TRIGGER_API_ID, WDG_E_NOT_INITIALIZED);
	/* check if the WDG is free */
	WDG_ASSERT_RETVOID(!(wdg_state == WDGIF_BUSY), WDG_TRIGGER_API_ID, WDG_E_DRIVER_STATE);
	WDG_UPDATE_STATUS(WDGIF_BUSY);
	icr = __mfcr(ICR);  /* save the interrupt control register */
	/* Os.h interface not used here since this needs to be self contained function */
	__disable();
	WDG_PasswordAccess();
	WDG_ModifyAccess(0xF3U);
	if (icr & 0x00000100U)
	{
		__enable();
	}
	WDG_UPDATE_STATUS(WDGIF_IDLE);
}

#define WDG_STOP_SEC_CODE_TRIGGER
#include "MemMap.h"
