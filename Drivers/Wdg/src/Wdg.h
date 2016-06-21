/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Wdg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:18:08MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Wdg.h  $
 *  Revision 1.5 2011/02/02 09:18:08MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.4 2010/11/22 08:55:35MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.3 2010/11/22 08:47:49MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:52:01MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:23:37MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Wdg/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef WDG_H_
#define WDG_H_

#include "team-s.h"
#include "Std_Types.h"
#include "WdgIf.h"
#include "Wdg_Cfg.h"

/** @defgroup group_wdg Wdg
 *  @ingroup group_wdg */
/*@{*/

#define WDG_MODULE_ID         (102)
#define WDG_VENDOR_ID         (TEAMS_VENDOR_ID)

#define WDG_AR_MAJOR_VERSION  (2)
#define WDG_AR_MINOR_VERSION  (2)
#define WDG_AR_PATCH_VERSION  (2)

#define WDG_SW_MAJOR_VERSION  (1)
#define WDG_SW_MINOR_VERSION  (0)
#define WDG_SW_PATCH_VERSION  (0)

#define WDG_VENDOR_API_INFIX  (0)

/* Driver APIs Service ID */
#define WDG_INIT_API_ID             	((uint8)(0x00))
#define WDG_SETMODE_API_ID             	((uint8)(0x01))
#define WDG_TRIGGER_API_ID             	((uint8)(0x02))
#define WDG_GETVERSIONINFO_API_ID		((uint8)(0x04))

/* Development Error Codes */
#define WDG_E_DRIVER_STATE				((uint8)(0x10))	/**< API used in wrong context. */
#define WDG_E_PARAM_CONFIG				((uint8)(0x11))	/**< A wrong parameter to function Wdg_Init() was passed. */
#define WDG_E_PARAM_MODE  				((uint8)(0x12))	/**< Interface Wdg_SetMode()is called in wrong sequence. */
#define WDG_E_NOT_INITIALIZED           ((uint8)(0x13)) /**< Driver Not initialized. */
#define WDG_E_NULL_POINTER              ((uint8)(0x14)) /**< NULL pointer is passed to the function. */

/** @page wdg Wdg
 *
 * @section Wdg_intro Introduction
 * The wdg function implements the Wdg functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_WatchdogDriver">AUTOSAR_SWS_WatchdogDriver</a>.
 *
 *
 * @section Wdg_init Initialization of Wdg
 * Before any service to the Wdg can be used the initialization function Wdg_Init() with a pointer
 * proper configuration set (#Wdg_ConfigType) must be called.
 *
 *
 * @section Wdg_autosar AutoSAR Conformance
 * External Wdg services are not implemented in this release. So configure NULL to HW \n
 * config structure pointer during configuration.
 * SEId should be sequential
 *
 *
 * @section Wdg_cfg Configuration
 * @subsection Wdg_cfg_step_by_step Step by Step
 * @li Configure #Wdg_Int_ConfigType for internal WDG timer.
 * @li Configure #Wdg_ConfigType for the complete WDG Driver.
 * @li #STD_ON or #STD_OFF the switches #WDG_DISABLE_ALLOWED and #WDG_DEV_ERROR_DETECT based on project need.
 * @li Call the function Wdg_Init() with the pointer to the configured structure #Wdg_ConfigType
 *     in the startup phase.
 * @li Now Wdg_SetMode() and Wdg_Trigger() are available for the project to use.
 * @li Based on your timeout value, use WDG Trigger interface to re-trigger the WDG.
 *
 * @subsection wdg_cfg_by_example Example
 * Configuration of #Wdg_ConfigType. Below example shows the configuration of
 * Watchdog timer in SLOW mode and Password as 0xA0. The time out for the
 * internal watchdog timer is configured as 0xA000 ticks. Refer #Wdg_ConfigType for
 * details about the config element details.
 *
 * @code
 *
 * const Wdg_Int_ConfigType int_wdg_config = {
 *	WDGIF_SLOW_MODE,         Wdg_Int_ConfigType::init_mode: Initial WDG mode configuration is SLOW.
 *	0xA0,                    Wdg_Int_ConfigType::pw: Initial WDG password is set to 0xA0.
 *	0xA000,                  Wdg_Int_ConfigType::wdg_timeout:WDG timer time out value is 0xA000 ticks.
 *  };
 *
 * @endcode
 *
 *
 * Configuration of #Wdg_ConfigType. Below example shows the configuration of
 * Watchdog timer with internal watchdog ONLY.
 *
 * @code
 *
 * const Wdg_ConfigType wdg_config = {
 *	&int_wdg_config,        Wdg_ConfigType::int_wdg:Internal watchdog configuration pointer.
 *	NULL,                   Wdg_ConfigType::int_wdg:External watchdog configuration pointer is set to NULL as it is not implemented.
 * };
 *
 * @endcode
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Wdg_Init(&wdg_config);
 *   ...
 * }
 * @endcode
 *
 */

/** This is the configuration structure of Internal Watchdog */

typedef struct {
	WdgIf_ModeType init_mode;         /**< Initial WDG mode configuration. */
	uint8 pw;                         /**< Initial WDG password. */
	uint16 wdg_timeout;               /**< WDG timer time out value in ticks*/
} Wdg_Int_ConfigType;

/** This is the configuration structure of External Watchdog */

typedef struct {
	uint8 cs_pin;                     /**< Chip select for external WDG . */
	} Wdg_Ext_ConfigType;

/** This is the configuration structure of Watchdog driver */
typedef struct {
	const Wdg_Int_ConfigType* int_wdg;       /**< Internal watchdog configuration. */
	const Wdg_Ext_ConfigType* ext_wdg;       /**< External watchdog configuration. */
} Wdg_ConfigType;

#define WDG_START_SEC_CODE
#include "MemMap.h"


/** This service initializes the Wdg driver.
 *
 * The function Wdg_Init initializes the Wdg module.
 * @warning This service has to be called manually before Wdg Manager (@ref wdgm) module is used.\n
 *          This function should be called before any other Wdg driver function is called.
 *
 * @note After the execution of the function Wdg_Init(), the configuration data are accessible
 * and can be used by the Wdg module functions.
 *
 * @param cfg   Pointer to the configuration.
 *
 */
extern void Wdg_Init(const Wdg_ConfigType *cfg);

/** This service Set the Mode of the Wdg driver.
 *  The function Wdg_SetMode sets the mode in which the Watchdog timer should operate.
 *
 * @param Mode       #WDGIF_OFF_MODE:  This will switch off the Watchdog timer. Not Recommended for safety critical applications. \n
 *                   #WDGIF_SLOW_MODE: This will make the watchdog timer to run slow. Normally this is used in startup or shutdown. \n
 *                   #WDGIF_FAST_MODE: This will make the watchdog timer to run fast. \n
 *
 *  @return #E_OK:     WDG Timer mode was set as requested.\n
 *          #E_NOT_OK: WDG Timer mode was NOT set as requested.\n
 *
 */

extern Std_ReturnType Wdg_SetMode(WdgIf_ModeType Mode);

#define WDG_STOP_SEC_CODE
#include "MemMap.h"

#define WDG_START_SEC_CODE_TRIGGER
#include "MemMap.h"

/** This service Trigger the Watchdog timer.
 *  The function Wdg_Trigger triggers the Wdg so that it time out is prevented.
 *  The time out period depends on either the Wdg running in mode #WDGIF_SLOW_MODE or
 *  #WDGIF_FAST_MODE. Consider using the WdgIF interface instead of this. This interface
 *  can also be run from RAM. It is located at the specific address by locating the memory
 *  section defined by the memory hint WDG_START_SEC_CODE_TRIGGER. Use the built
 *  in function from tasking (memcpy) to copy this function to RAM area. Also make sure the
 *  Destination address is word aligned.\n
 *
 *  Normally this interface is called by the watchdog manager (@ref wdgm)
 *  during normal operation.
 *
 */
extern void Wdg_Trigger(void);


/** The function Wdg_GetVersionInfo returns the version information of the Wdg module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Wdg_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter WdgVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Wdg_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = WDG_MODULE_ID;
	versioninfo->sw_major_version = WDG_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = WDG_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = WDG_SW_PATCH_VERSION;
	versioninfo->vendorID = WDG_VENDOR_ID;
}


#define WDG_STOP_SEC_CODE_TRIGGER
#include "MemMap.h"

/*@}*/

#endif /* WDG_H_ */
