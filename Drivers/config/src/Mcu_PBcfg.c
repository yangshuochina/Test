/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Mcu_PBcfg.c $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/05/20 11:29:02MESZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Mcu_PBcfg.c  $
 * Revision 1.1 2010/05/20 11:29:02MESZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 * Revision 1.1 2010/03/08 14:06:23MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/config/src/project.pj
 * *****************************************************************************/

#include "Mcu.h"

#define MCU_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

#define USE_8MHZ
#ifdef USE_8MHZ
const uint8 mcu_clock_ramp_cfg[] = {
        53, 40, 32, 26, 22, 19, 17, 15, 13, 12, 11, 10, 9, 8, 0
};

const Mcu_ClockConfigType mcu_clock_cfg = {
        mcu_clock_ramp_cfg,        /* Mcu_ClockConfigType::k2_ramp No K2 ramp shall be used */
        100,                    /* Mcu_ClockConfigType::k2_ramp_delay no ramp delay */
        MCU_CLOCK_MODE_NORMAL,  /* Mcu_ClockConfigType::clock_mode shall be normal mode */
        3,                      /* Mcu_ClockConfigType::oscval */
        1,                      /* Mcu_ClockConfigType::k1 */
        80,                     /* Mcu_ClockConfigType::k2 */
        1,                      /* Mcu_ClockConfigType::p */
        80,                     /* Mcu_ClockConfigType::n */
        0,                      /* Mcu_ClockConfigType::pcpdiv: divide by 1 */
        0,                      /* Mcu_ClockConfigType::lmbdiv: divide by 1 */
        1,                      /* Mcu_ClockConfigType::fpidiv: divide by 2 */
        1                       /* Mcu_ClockConfigType::mcdsdiv: divide by 2 */
};
#else
const Mcu_ClockConfigType mcu_clock_cfg = {
		NULL_PTR,				/* Mcu_ClockConfigType::k2_ramp No K2 ramp shall be used */
		0,						/* Mcu_ClockConfigType::k2_ramp_delay no ramp delay */
		MCU_CLOCK_MODE_NORMAL,  /* Mcu_ClockConfigType::clock_mode shall be normal mode */
		8,                      /* Mcu_ClockConfigType::oscval */
		1,                      /* Mcu_ClockConfigType::k1 */
		6,                      /* Mcu_ClockConfigType::k2 */
		2,                      /* Mcu_ClockConfigType::p */
		80,                     /* Mcu_ClockConfigType::n */
		0,                      /* Mcu_ClockConfigType::pcpdiv: divide by 1 */
		0,                      /* Mcu_ClockConfigType::lmbdiv: divide by 1 */
		1,                      /* Mcu_ClockConfigType::fpidiv: divide by 2 */
		1                       /* Mcu_ClockConfigType::mcdsdiv: divide by 2 */
};
#endif


const Mcu_RamConfigType mcu_ram_cfg[] = {
		{
				(uint32*)0xAFE80000u,         /* Mcu_RamConfigType::start_addr */
				NULL_PTR,                     /* Mcu_RamConfigType::start_image */
				0x1000                        /* Mcu_RamConfigType::size */
		},
		{
				(uint32*)0xF0050000u,         /* Mcu_RamConfigType::start_addr */
				(const uint32 *)0x80000000u,  /* Mcu_RamConfigType::start_image */
				0x2000                        /* Mcu_RamConfigType::size */
		},
};


const Mcu_ConfigType mcu_config = {
		0,              /* Mcu_ConfigType::Mcu_NumberOfMcuModes: no mode configuration */
		1,              /* Mcu_ConfigType::Mcu_NumberOfClockSettings: 1 clock configuration */
		0,              /* Mcu_ConfigType::Mcu_NumberOfRamSections: 2 RAM sections */
		NULL_PTR,       /* no mode configuration: Mcu_ModeConfigType */
		&mcu_clock_cfg, /* pointer to clock configuration: Mcu_ClockConfigType */
		NULL_PTR        /* pointer to RAM configuration: Mcu_RamConfigType */
};


#define MCU_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
