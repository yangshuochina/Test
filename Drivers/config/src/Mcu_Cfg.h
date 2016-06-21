/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Mcu_Cfg.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2011/02/24 13:45:51MEZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Mcu_Cfg.h  $
 * Revision 1.4 2011/02/24 13:45:51MEZ Cotor Vlad RGB (COTORV) 
 * - Develop shutdown function for xCU power control
 * Revision 1.3 2010/11/22 09:00:23MEZ Cotor Vlad RGB (COTORV) 
 * Update BSW software modules with doxygen documentation comments
 * Revision 1.2 2010/07/12 09:43:24MESZ Cotor Vlad RGB (COTORV) 
 * Integration of EcuM, Fee, NvM, RamTst
 * Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 * Upfate of ARE9018.lsl file due to EcuM
 * Revision 1.1 2010/05/20 11:29:02MESZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 * Revision 1.1 2010/03/08 14:06:23MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/config/src/project.pj
 * *****************************************************************************/

#ifndef MCU_CFG_H_
#define MCU_CFG_H_

#include "Std_Types.h"

#define MCU_DEV_ERROR_DETECT        STD_ON

#define MCU_INSTANCE_ID				0

/* bits for FDR setting */
#define DM_DIS     (0u)
#define DM_NORMAL  (0x1u)
#define DM_FRACT   (0x2u)
#define DIS_CLK    (BITPOS(31))
#define DM(dm)     ((uint32)(dm) << 14u)
#define STEP(step) ((uint32)(step))

/* bits for CLC setting */
#define RMC(rmc)   ((rmc) << 8)

/** @defgroup group_mcu_cfg Mcu Configuration Parameters
 *  @ingroup group_mcu */
/*@{*/
#define MCU_TIMEOUT_OSC     5000    /**< timeout until oscillator is stable. */
#define MCU_TIMEOUT_K1RDY   1000    /**< timeout until K1RDY is appearing. */
#define MCU_TIMEOUT_VCOBYST 10000   /**< timeout until VCOBYST is appearing. */
#define MCU_TIMEOUT_VCOLOCK 100000  /**< timeout until VCOLOCK is appearing. */

#define MCU_SCU_FDR_VAL    (STEP(0x3FF) | DM(DM_NORMAL))	/**< The fractional divider for SCU. */
#define MCU_SSC0_FDR_VAL   (STEP(0x3FF) | DM(DM_NORMAL))	/**< The fractional divider for SSC0. */
#define MCU_SSC1_FDR_VAL   (STEP(0x3FF) | DM(DM_NORMAL))	/**< The fractional divider for SSC1. */
#define MCU_GPTA0_FDR_VAL  (STEP(0x3FF) | DM(DM_NORMAL))	/**< The fractional divider for GPTA0. */

#define MCU_STM_CLC_VAL    (RMC(2)| 0x4 | 0x10 )   /**< The divider for STM. */
#define MCU_ASC0_CLC_VAL   (RMC(1))                /**< The divider for ASC0. */
#define MCU_GPTA0_CLC_VAL  (BITPOS(2) | BITPOS(4)) /**< The CLC for GPTA0. */
#define MCU_ADC0_CLC_VAL   (0)                     /**< The CLC for ADC0. */


/* configuration of GPTA0_CKBCTR: see Figure 20-37 of TC1767 User Manual */
#define MCU_GPTA0_CKBCTR_DFA02         (4u)     /**< CLK2: f_GPTA / 2^MCU_GPTA0_CKBCTR_DFA02 or 14=PLL, 15=DCM3. */
#define MCU_GPTA0_CKBCTR_DFA03         (5u)     /**< CLK3: 0=DCM2, 1=PLL, 2=uncomp.PLL 3=not usable. */
#define MCU_GPTA0_CKBCTR_DFA04         (5u)     /**< CLK4: f_GPTA / 2^MCU_GPTA0_CKBCTR_DFA04 or 15=DCM1. */
#define MCU_GPTA0_CKBCTR_DFA06         (7u)     /**< CLK6: f_GPTA / 2^MCU_GPTA0_CKBCTR_DFA06 or 15=FPC1. */
#define MCU_GPTA0_CKBCTR_DFA07         (7u)     /**< CLK7: f_GPTA / 2^MCU_GPTA0_CKBCTR_DFA07 or 15=FPC4. */
#define MCU_GPTA0_CKBCTR_DFALTC        (0u)     /**< prescaler clock for LTC: f_GPTA / 2^MCU_GPTA0_CKBCTR_DFALTC. */

#define MCU_GPTA0_CKBCTR  ((MCU_GPTA0_CKBCTR_DFA02 << 0u)  | \
                           (MCU_GPTA0_CKBCTR_DFA04 << 4u)  | \
                           (MCU_GPTA0_CKBCTR_DFA06 << 8u)  | \
                           (MCU_GPTA0_CKBCTR_DFA07 << 12u) | \
                           (MCU_GPTA0_CKBCTR_DFA03 << 16u) | \
                           (MCU_GPTA0_CKBCTR_DFALTC << 18u))

/* configuration of GPTA0_EDCTR */
#define MCU_GPTA0_EDCTR_L2EN        (1)      /**< Enable/disable LTCA2 timer clock. */
#define MCU_GPTA0_EDCTR_G0EN        (1)      /**< Enable/disable f_GPTA0 timer clock. */
#define MCU_GPTA0_EDCTR_GT00RUN     (1)      /**< Start/stop GT0 timer. */
#define MCU_GPTA0_EDCTR_GT01RUN     (1)      /**< Start/stop GT1 timer. */

#define MCU_GPTA0_EDCTR    ((MCU_GPTA0_EDCTR_L2EN    << 10u) | \
                            (MCU_GPTA0_EDCTR_G0EN    << 8u)  | \
                            (MCU_GPTA0_EDCTR_GT00RUN << 1u)  | \
                            (MCU_GPTA0_EDCTR_GT01RUN << 0u) )

/* configuration of GPTA0_GTCTR0 */
#define MCU_GPTA0_GTCTR0_REN     (0)   /**< Enable/disable Interrupt. */
#define MCU_GPTA0_GTCTR0_MUX     (2)   /**< Timer clock selection. */
#define MCU_GPTA0_GTCTR0_SCO     (14)  /**< TGE Flag Source Selection (0=10th bit, 1=11th bit, ...). */

#define MCU_GPTA0_GTCTR0   ((MCU_GPTA0_GTCTR0_REN << 7u)  | \
                            (MCU_GPTA0_GTCTR0_MUX << 4u)  | \
                            (MCU_GPTA0_GTCTR0_SCO << 0u) )


/* configuration of GPTA0_GTCTR1 */
#define MCU_GPTA0_GTCTR1_REN     (0u)  /**< Enable/disable Interrupt. */
#define MCU_GPTA0_GTCTR1_MUX     (0u)  /**< Timer clock selection. */
#define MCU_GPTA0_GTCTR1_SCO     (14u) /**< TGE Flag Source Selection (0=10th bit, 1=11th bit, ...). */

#define MCU_GPTA0_GTCTR1  ((MCU_GPTA0_GTCTR1_REN << 7u)  | \
                           (MCU_GPTA0_GTCTR1_MUX << 4u)  | \
                           (MCU_GPTA0_GTCTR1_SCO << 0u) )

/*@}*/


#define CPU_FREQ (80000000)  /**< CPU frequency in Hz. */

/* the following macro does not correctly use the GPTA0 FDR value. It assumes
a divider of 2 */
#define CLK_BUS_FREQ(bus)     (CPU_FREQ / 2 / (1 << (bus)))
#define CLK_TICKS(freq, bus)  (CLK_BUS_FREQ(bus) / (freq))

/* on a change of the timer bus MCU_GPTA0_GTCTR0_MUX the macro below must be adapted */
#define GPTA0_GT0_TICKS(period)   CLK_TICKS((1./period), MCU_GPTA0_CKBCTR_DFA02)


#endif /* MCU_CFG_H_ */
