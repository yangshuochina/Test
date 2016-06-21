/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Mcu_Clk.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/09/22 18:52:54MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Mcu_Clk.c  $
 *  Revision 1.3 2010/09/22 18:52:54MESZ Cotor Vlad RGB (COTORV) 
 *  Correct SPI clock initialization during wake up.
 *  Revision 1.2 2010/07/12 09:48:08MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:18:05MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Mcu/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Mcu.h"
#include "Mcu_Priv.h"
#include "Dem.h"


#define MCU_START_SEC_CODE
#include "MemMap.h"


Std_ReturnType Mcu_InitClock(Mcu_ClockType num_of_clock_set)
{
	const Mcu_ClockConfigType *cfg;
	uint32 timeout;
	uint8 ts_error = 0;
	const uint8 *k2_ramp;

	MCU_ASSERT_RETERR(mcu_cfg != NULL_PTR, MCU_INITCLOCK_API_ID, MCU_E_UNINIT);
	MCU_ASSERT_RETERR(num_of_clock_set < mcu_cfg->Mcu_NumberOfClockSettings, MCU_INITCLOCK_API_ID, MCU_E_PARAM_CLOCK);

	cfg = &mcu_cfg->Mcu_ClockConfig[num_of_clock_set];


	/*
	 * wait for a stable oscillator
	 */
	if(cfg->oscval > 0) {
		ts_endinit_clear();
		SCU_OSCCON.B.OSCVAL = cfg->oscval - 1;
	    ts_endinit_set();
	}
	timeout = 0;
	while(SCU_OSCCON.B.PLLLV == 0 || SCU_OSCCON.B.PLLHV == 0) {
		timeout++;
		if(timeout > MCU_TIMEOUT_OSC) {
			ts_error = MCU_E_NO_OSC;
			break;
		}
	}

	if(ts_error == 0) {
	    /*
	     * enter prescaler mode
	     */
	    /* wait until K1 is ready for an update */
	    timeout = 0;
	    while(SCU_PLLSTAT.B.K1RDY == 0) {
	    	timeout++;
	    	if(timeout > MCU_TIMEOUT_K1RDY) {
	    		ts_error = MCU_E_TIMEOUT_K1RDY;
	    		break;
	    	}
	    }

	    if(ts_error == 0) {
	    	/* enter the prescaler mode with the correct divider */
	    	ts_endinit_clear();
	    	SCU_PLLCON1.B.K1DIV = cfg->k1 - 1;
	    	SCU_PLLCON0.B.VCOBYP = 1;
	        ts_endinit_set();

	    	/* wait until it is accepted by the PLL module */
	    	timeout = 0;
	    	while(SCU_PLLSTAT.B.VCOBYST == 0) {
	    		timeout++;

	    		if(timeout > MCU_TIMEOUT_VCOBYST) {
		    		ts_error = MCU_E_TIMEOUT_VCOBYST;
		    		break;
	    		}
	    	}

		    /*
		     * setup system timing
		     */
		    ts_endinit_clear();
			SCU_CCUCON0.U = ((uint32)cfg->pcpdiv << 24u) |
					        ((uint32)cfg->lmbdiv << 8u) |
					        (uint32)(cfg->fpidiv);
			SCU_CCUCON1.U = (cfg->mcdsdiv);
		    ts_endinit_set();
	    }


    	/*
    	 * enter normal mode
    	 */
	    if(ts_error == 0 && cfg->clock_mode == MCU_CLOCK_MODE_NORMAL) {
	    	/* disable NMI trap generation for VCO lock */
	    	ts_endinit_clear();
	    	SCU_TRAPDIS.B.SYSVCOLCKT = 1;

	    	/* setup P, N, K2 */
	    	SCU_PLLCON1.B.K2DIV = cfg->k2 - 1;
	    	SCU_PLLCON0.B.PDIV = cfg->p - 1;
	    	SCU_PLLCON0.B.NDIV = cfg->n - 1;

	    	/* connect VCO to oscilator */
	    	SCU_PLLCON0.B.OSCDISCDIS = 1;
    		SCU_PLLCON0.B.CLRFINDIS = 1;

	    	/* wait for VCO lock */
	    	SCU_PLLCON0.B.RESLD = 1;
	        ts_endinit_set();

	    	timeout = 0;
	    	while(!SCU_PLLSTAT.B.VCOLOCK) {
	    		timeout++;

	    		if(timeout > MCU_TIMEOUT_VCOLOCK) {
	    			ts_error = MCU_E_TIMEOUT_VCOLOCK;
	    			break;
	    		}
	    	}

	    	if(ts_error == 0) {
	    		/* switch from prescaler mode to normal mode */
	    		ts_endinit_clear();
	    		SCU_PLLCON0.B.VCOBYP = 0;
	    		SCU_PLLCON0.B.CLRFINDIS = 1;

	    		/* switch back to free running mode if PLL looses it's lock */
	    		SCU_PLLCON0.B.OSCDISCDIS = 0;

	    		/* clear NMI status flag for NMI trap */
	    		SCU_TRAPCLR.B.SYSVCOLCKT = 1;
	    	    ts_endinit_set();
	    	}
	    }
	}


	/*
	 * Ramp up of K2
	 */
	if((ts_error == 0) && (cfg->k2_ramp != NULL_PTR)) {
		/* perform the ramp up of the clock */
		k2_ramp = cfg->k2_ramp;

		while(*k2_ramp != 0) {
	    	ts_endinit_clear();
			SCU_PLLCON1.B.K2DIV = *k2_ramp - 1;
    	    ts_endinit_set();
	    	timeout = 0;
	    	while(timeout < cfg->k2_ramp_delay) {
	    		timeout++;
	    		__nop();
	    	}
	    	k2_ramp++;
		}
	}

	if(ts_error != 0) {
		if(ts_error == MCU_E_TIMEOUT_VCOBYST || ts_error == MCU_E_TIMEOUT_VCOLOCK) {
			/* clock errors shall be reported to DEM */
			Dem_ReportErrorStatus(MCU_E_CLOCK_FAILURE, DEM_EVENT_STATUS_FAILED);
		}
		MCU_ASSERT_RETERR(0, MCU_INITCLOCK_API_ID, ts_error);
		return E_NOT_OK;
	}
	return E_OK;
}


void Mcu_DistributePllClock(void)
{
	MCU_ASSERT_RETVOID(Mcu_GetPllStatus() == MCU_PLL_LOCKED, MCU_DISTRIBUTEPLLCLOCK_API_ID, MCU_E_PLL_NOT_LOCKED);
    ts_endinit_clear();

#ifdef MCU_SCU_FDR_VAL
	SCU_FDR.U = MCU_SCU_FDR_VAL;
#endif
#ifdef MCU_SSC0_FDR_VAL
	SSC0_CLC.B.DISR = 0;
	while(SSC0_CLC.B.DISS != 0);
	SSC0_FDR.U = MCU_SSC0_FDR_VAL;
#endif
#ifdef MCU_SSC1_FDR_VAL
	SSC1_CLC.B.DISR = 0;
   while(SSC1_CLC.B.DISS != 0);
	SSC1_FDR.U = MCU_SSC1_FDR_VAL;
#endif
#ifdef MCU_CAN_FDR_VAL
	CAN_FDR.U = MCU_CAN_FDR_VAL;
#endif
#ifdef MCU_GPTA0_CLC_VAL
	GPTA0_CLC.U = MCU_GPTA0_CLC_VAL;
#endif
#ifdef MCU_GPTA0_FDR_VAL
	GPTA0_CLC.B.DISR = 0;
   while(GPTA0_CLC.B.DISS != 0);
	GPTA0_FDR.U = MCU_GPTA0_FDR_VAL;
#endif
#ifdef MCU_FADC_FDR_VAL
	FADC_FDR.U =  MCU_FADC_FDR_VAL;
#endif

#ifdef MCU_STM_CLC_VAL
	STM_CLC.U = MCU_STM_CLC_VAL;
#endif
#ifdef MCU_ASC0_CLC_VAL
	ASC0_CLC.U = MCU_ASC0_CLC_VAL;
#endif
#ifdef MCU_ADC0_CLC_VAL
    ADC0_CLC.U = MCU_ADC0_CLC_VAL;
    DMA_CLC.U = 0;
#endif
	ts_endinit_set();

#ifdef MCU_GPTA0_CKBCTR
	GPTA0_CKBCTR.U = MCU_GPTA0_CKBCTR;
#endif

#ifdef MCU_GPTA0_GTCTR0
	GPTA0_GTCTR0.U = MCU_GPTA0_GTCTR0;
#endif

#ifdef MCU_GPTA0_GTCTR1
	GPTA0_GTCTR1.U = MCU_GPTA0_GTCTR1;
#endif

#ifdef MCU_GPTA0_EDCTR
	GPTA0_EDCTR.U = MCU_GPTA0_EDCTR;
#endif
}


Mcu_PllStatusType Mcu_GetPllStatus(void)
{
	if(SCU_PLLSTAT.B.VCOLOCK) {
		return MCU_PLL_LOCKED;
	}
	if(SCU_PLLSTAT.B.VCOBYST) {
		return MCU_PLL_STATUS_UNDEFINED;
	}
	return MCU_PLL_UNLOCKED;
}

#define MCU_STOP_SEC_CODE
#include "MemMap.h"
