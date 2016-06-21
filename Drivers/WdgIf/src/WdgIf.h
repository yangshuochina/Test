/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgIf.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:53:27MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgIf.h  $
 *  Revision 1.2 2010/11/22 08:53:27MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:13:57MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/EcuAbstractionLayer/WdgIf/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef WDGIF_H_
#define WDGIF_H_

#include "WdgIf_Types.h"
#include "Wdg.h"

/** @defgroup group_wdgif WdgIf
 *  @ingroup group_wdgif */
/*@{*/

#define WDGIF_MODULE_ID         (43)
#define WDGIF_VENDOR_ID         (TEAMS_VENDOR_ID)
#define WDGIF_AR_MAJOR_VERSION  (2)
#define WDGIF_AR_MINOR_VERSION  (2)
#define WDGIF_AR_PATCH_VERSION  (2)
#define WDGIF_SW_MAJOR_VERSION  (1)
#define WDGIF_SW_MINOR_VERSION  (0)
#define WDGIF_SW_PATCH_VERSION  (0)


/* Driver APIs Service ID */
#define WDGIF_SETMODE_API_ID	    ((uint8)(0x03))
#define WDGIF_TRIGGER_API_ID		((uint8)(0x03))
#define WDGIF_GETVERSIONINFO_API_ID	((uint8)(0x03))

/* Development Error Codes */
#define WDGIF_E_PARAM_DEVICE				((uint8)(0x01))	/**< No development error detection is available since only one device is implemented. */

/**< API to set the Mode.
 * Since only internal watchdog is implemented, DeviceIndex is not used.
 * Redirected to Wdg driver.
 *
 * DeviceIndex:   WDG device index.
 * WdgMode        #WDGIF_OFF_MODE:  This will switch off the Watchdog timer. Not Recommended for safety critical applications.
 *                #WDGIF_SLOW_MODE: This will make the watchdog timer to run slow. Normally this is used in startup or shutdown.
 *                #WDGIF_FAST_MODE: This will make the watchdog timer to run fast.
 *
 */

#define WdgIf_SetMode(DeviceIndex, WdgMode) Wdg_SetMode(WdgMode)

/**API to trigger the WDG driver.
 * Since only internal watchdog is implemented, DeviceIndex is not used.
 * Redirected to Wdg driver.
 *
 * DeviceIndex:   WDG device index.
 */
#define WdgIf_Trigger(DeviceIndex) Wdg_Trigger()


/** The function WdgIf_GetVersionInfo returns the version information of the WdgIf module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function WdgIf_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter WdgIfVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void WdgIf_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = WDGIF_MODULE_ID;
	versioninfo->sw_major_version = WDGIF_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = WDGIF_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = WDGIF_SW_PATCH_VERSION;
	versioninfo->vendorID = WDGIF_VENDOR_ID;

}
/*@}*/
#endif /* WDGIF_H_ */
