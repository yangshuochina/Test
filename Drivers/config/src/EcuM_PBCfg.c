/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM_PBCfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:00:15MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM_PBCfg.c  $
 *  Revision 1.2 2010/07/12 14:00:15MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "EcuM_Cfg.h"
#include "EcuM_Generated_Types.h"

#include "Adc.h"
#include "Fls.h"
#include "Gpt.h"
#include "Mcu.h"
#include "Port.h"
#include "Pwm.h"
#include "Spi.h"
#include "Wdg.h"

#include "os.h"

#ifdef ECUM_WDGM_INCLUDED
#include "WdgM.h"
#endif


/*------------------- Adc ------------------*/
#define ADC_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Adc_ConfigType adc_config;

#define ADC_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Fls ------------------*/
#define FLS_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Fls_ConfigType fls_config;

#define FLS_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Gpt ------------------*/
#define GPT_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Gpt_ConfigType gpt_config;

#define GPT_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Mcu ------------------*/
#define MCU_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Mcu_ConfigType mcu_config;

#define MCU_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Port -----------------*/
#define PORT_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Port_ConfigType port_config;

#define PORT_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Pwm -----------------*/
#define PWM_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Pwm_ConfigType pwm_config;

#define PWM_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Spi-----------------*/
#define SPI_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Spi_ConfigType spi_config;

#define SPI_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/*------------------- Wdg-----------------*/
#define WDG_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Wdg_ConfigType wdg_config;

#define WDG_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

#ifdef ECUM_WDGM_INCLUDED
/*------------------- WdgM-----------------*/
#define WDGM_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const WdgM_ConfigType wdgm_config;

#define WDGM_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
#endif

/* ----------------- CONFIG DATA 32 BIT ----------------*/
#define ECUM_START_SEC_CONFIG_DATA_32BIT
#include "memmap.h"

const EcuM_ConfigType ecum_cfg = {
		ECUM_CONFIGCONSISTENCY_HASH,
		OSDEFAULTAPPMODE,
		0,
		ECUM_RUN_SELF_REQUEST_PERIOD_TICKS,
		{
				{ /* driver initialization one */
						&adc_config,
						&fls_config,
						&gpt_config,
						&mcu_config,
						&port_config,
						&pwm_config,
						&spi_config,
						&wdg_config,
#ifdef ECUM_WDGM_INCLUDED
						&wdgm_config,
#endif
				},
				{ /* driver initialization two */
						0
				},
				{ /* driver initialization three */
						0
				}
		},
#ifdef ECUM_WDGM_INCLUDED
	/* Configuration of WdgM - Watchdog Manager. */
		{
				0, /* This parameter references the supervised entity ID to be used for the #WdgM. */
				0, /* This parameter references the WdgM mode to be set when entering #EcuM_StartupTwo. */
				0, /* This parameter references the WdgM mode to be set when entering #ECUM_STATE_RUN. */
				0, /* This parameter references the WdgM mode to be set when entering #ECUM_STATE_WAKEUP. */
				0, /* This parameter references the WdgM mode to be set when entering #ECUM_STATE_APP_POST_RUN. */
				0, /* This parameter references the WdgM mode to be set when entering #ECUM_STATE_SHUTDOWN. */

		}
#endif

#ifdef ECUM_NVMRAM_MGR_INCLUDED
	/* Configuration of NvRam Manager Handling. */
		{
			ECUM_NVRAM_WRITEALL_TIMEOUT_TICKS,
			ECUM_NVRAM_READALL_TIMEOUT_TICKS,
		}
#endif
};

#define ECUM_STOP_SEC_CONFIG_DATA_32BIT
#include "memmap.h"
