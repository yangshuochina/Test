/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:36:00MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM.h  $
 *  Revision 1.3 2010/11/22 08:36:00MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:49:28MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:26:16MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/ServiceLayer/SchM/AA000/src/project.pj
 *
 * ***************************************************************************/

/** @ingroup SchM */

#ifndef SCHM_H_
#define SCHM_H_

#include "team-s.h"
#include "Std_Types.h"
#include "Os.h"

/** @defgroup group_schm SchM */
/*@{*/

#define SCHM_MODULE_ID         (130)
#define SCHM_VENDOR_ID         (TEAMS_VENDOR_ID)
#define SCHM_AR_MAJOR_VERSION  (2)
#define SCHM_AR_MINOR_VERSION  (2)
#define SCHM_AR_PATCH_VERSION  (2)
#define SCHM_SW_MAJOR_VERSION  (1)
#define SCHM_SW_MINOR_VERSION  (0)
#define SCHM_SW_PATCH_VERSION  (0)

/* Driver APIs Service ID */
#define SCHM_INIT_API_ID             	 ((uint8)(0x1)) /**< #SchM_Init API ID used by error detection. */
#define SCHM_DEINIT_API_ID             	 ((uint8)(0x2)) /**< #SchM_DeInit API ID used by error detection. */
#define SCHM_ENTER_API_ID                ((uint8)(0x3)) /**< #SchM_Enter API ID used by error detection. */
#define SCHM_EXIT_API_ID                 ((uint8)(0x4)) /**< #SchM_Exit API ID used by error detection. */
#define SCHM_ACT_MAIN_FUNCTION_API_ID    ((uint8)(0x5)) /**< #SchM_ActMainFunction API ID used by error detection. */
#define SCHM_CANCEL_MAIN_FUNCTION_API_ID ((uint8)(0x6)) /**< #SchM_CancelMainFunction API ID used by error detection. */
#define SCHM_GETVERSIONINFO_API_ID		 ((uint8)(0x7)) /**< #SchM_GetVersionInfo API ID used by error detection. */

/* Development Error Codes */
#define SCHM_E_UNINIT					((uint8)(15))	/**< A service was called without a prior SchM_Init(). */


/** @page schm SchM
 *
 * @section schm_intro Introduction
 * The SchM function implements the SchM functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_BSW_Scheduler.pdf">AUTOSAR_SWS_BSW_Scheduler</a>.
 *
 */

/**
 * Return type of the SchM module functions.
 */
typedef uint8 SchM_ReturnType;

#define SCHM_E_NOFUNC ((SchM_ReturnType)0x05) /**< Requested service has not the desired effect. */
#define SCHM_E_STATE  ((SchM_ReturnType)0x07) /**< Requested service is already in use. */
#define SCHM_E_OK     ((SchM_ReturnType)0x00) /**< No error occurred. */
#define SCHM_E_LIMIT  ((SchM_ReturnType)0x04) /**< An internal limit has been exceeded. */

#define SCHM_START_SEC_CODE
#include "MemMap.h"

/** This service initializes the SchM module.
 *
 * The function SchM_Init allocates and initializes the resources used by the
 * SchM module.
 */
extern void SchM_Init(void);

/**
 * The function for de-initializes the SchM module.
 *
 */
extern void SchM_Deinit(void);


/**
 * This service enters a module local exclusive area.
 *
 * @param instance Parameter for identifying a unique instance of the calling BSW module.
 * @param exclusiveArea  Parameter for identify a unique internal resource of the BSW module.
 *
 */
inline void SchM_Enter(uint8 instance, uint8 exclusiveArea) {
	SuspendAllInterrupts();
}

/**
 * This service exits a module local exclusive area.
 *
 * @param instance Parameter for identifying a unique instance of the calling BSW module.
 * @param exclusiveArea  Parameter for identify a unique internal resource of the BSW module.
 *
 */
inline void SchM_Exit(uint8 instance, uint8 exclusiveArea) {
	ResumeAllInterrupts();
}

/**
 * This function triggers the activation of a corresponding main processing function.
 *
 * @param instance Parameter for identifying a unique instance of the calling BSW module.
 * @param activationPoint Parameter for identifying the unique execution point.
 * @return #SCHM_E_OK: No error occurred.
 *         #SCHM_E_LIMIT: To many task activation,
 *         #SCHM_E_STATE: ActivationPoint is already in use.
 */
extern SchM_ReturnType SchM_ActMainFunction(uint8 instance, uint8 activationPoint);

/**
 * This function requests the cancellation of the requested activation of a corresponding main processing function.
 *
 * @param instance Parameter for identifying a unique instance of the calling BSW module.
 * @param Parameter referring to the corresponding ActivationPoint to be canceled,
 * @return  #SCHM_E_OK: No error occurred.
 *          #SCHM_E_NOFUNC : Currently, there is no activation of a main processing function for the given
 *          ActivationPoint pending.
 */
extern SchM_ReturnType SchM_CancelMainFunction(uint8 instance, uint8 activationPoint);

/** The function SchM_GetVersionInfo returns the version information of the SchM module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function SchM_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter SchMVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void SchM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = SCHM_MODULE_ID;
	versioninfo->sw_major_version = SCHM_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = SCHM_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = SCHM_SW_PATCH_VERSION;
	versioninfo->vendorID = SCHM_VENDOR_ID;
}
/*@}*/

#define SCHM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* SCHM_H_ */
