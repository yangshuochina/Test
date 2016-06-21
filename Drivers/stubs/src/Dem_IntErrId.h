/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Dem_IntErrId.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/07/12 09:51:45MESZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Dem_IntErrId.h  $
 * Revision 1.2 2010/07/12 09:51:45MESZ Cotor Vlad RGB (COTORV) 
 * --- Added comments ---  COTORV [Jul 12, 2010 10:32:23 AM CEST]
 * Integration of EcuM, Fee, NvM, RamTst
 * Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 * Upfate of ARE9018.lsl file due to EcuM
 * Revision 1.1 2010/05/20 11:32:26MESZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/stubs/src/project.pj
 * Revision 1.1 2010/03/08 14:06:42MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/stubs/src/project.pj
 * *****************************************************************************/

#ifndef DEM_INTERRID_H_
#define DEM_INTERRID_H_

#include "team-s.h"

/**< @TODO Production Event Code ID to be defined */

typedef enum {
   /* MCU ERRORS*/
	MCU_E_CLOCK_FAILURE,

   /* WDG ERRORS*/
	WDG_E_MODE_SWITCH_FAILED,    /**< Mode switch failed. */
	WDG_E_DISABLE_REJECTED,      /**< WDG Reset disable request not accepted. Refer #WDG_DISABLE_ALLOWED .*/

   /* WDGM_ERRORS */
	WDG_E_ALIVE_SUPERVISION,     /**< WDGM Alive Supervision failed. Reset Inevitable!!!.*/
	WDGM_E_SET_MODE,             /**< WDGM Mode switch failed.*/

   /* FLS ERRORS */
   FLS_E_ERASE_FAILED,
   FLS_E_WRITE_FAILED,
   FLS_E_READ_FAILED,
   FLS_E_COMPARE_FAILED,
   FLS_E_UNEXPECTED_FLASH_ID,
	/* RAM ERRORS */
    RAMTST_E_RAM_FAILURE,

	/* NVM Errors */
	NVM_E_REQ_FAILED,
	NVM_E_INTEGRITY_FAILED,

	/* ECUM Errors */
    ECUM_E_RAM_CHECK_FAILED, 				/**< The RAM check during wakeup failed. */
    ECUM_E_ALL_RUN_REQUESTS_KILLED,			/**< The service #EcuM_KillAllRUNRequests was issued. */
    ECUM_E_CONFIGURATION_DATA_INCONSISTENT  /**< Configuration data is inconsistent. */

} Dem_EventIdType;

#endif /* DEM_INTERRID_H_ */


