/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: MemIf.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:31:00MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: MemIf.h  $
 *  Revision 1.3 2010/11/22 08:31:00MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:48:27MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:07:13MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/EcuAbstractionLayer/MemIf/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef MEMIF_H_
#define MEMIF_H_

#include "team-s.h"
#include "Std_Types.h"
#include "MemIf_Cfg.h"

#pragma nomisrac 15.4
#pragma nomisrac 16.7
#pragma nomisrac 16.8
#pragma nomisrac 15.3


/** @defgroup group_memif MemIf
 *  @ingroup group_memif */
/*@{*/

/* Driver APIs Service ID */
#define MEMIF_INIT_API_ID                ((uint8)(0))
#define MEMIF_SETMODE_API_ID             ((uint8)(1))
#define MEMIF_READ_API_ID                ((uint8)(2))
#define MEMIF_WRITE_API_ID               ((uint8)(3))
#define MEMIF_CANCEL_API_ID              ((uint8)(4))
#define MEMIF_GETSTATUS_API_ID           ((uint8)(5))
#define MEMIF_GETJOBRESULT_API_ID        ((uint8)(6))
#define MEMIF_INVALIDATEBLOCK_API_ID     ((uint8)(7))
#define MEMIF_GETVERSIONINFO_API_ID      ((uint8)(8))
#define MEMIF_ERASEIMMEDIATEBLOCK_API_ID ((uint8)(9))

/* Development Error Codes */
#define MEMIF_E_PARAM_DEVICE			((uint8)(1))	/**< API service called with wrong device index parameter. */


/** @page memif MemIf
 *
 * @section memif_intro Introduction
 * The MemIf function implements the MemIf functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_Mem_AbstractionInterface.pdf">AUTOSAR_SWS_Mem_AbstractionInterface</a>.
 *
 *
 * @section memif_autosar AutoSAR Conformity
 * MemIf implements all services defined by AutoSAR.
 *
 *
 * @section memif_cfg Configuration
 * @subsection memif_cfg_step_by_step Step by Step
 * @li Configure the subsystems to use by setting MEMIF_USE_FEE, MEMIF_USE_EA.
 *
 *
 * @subsection memif_cfg_by_example Example
 * If the FEE (EEPROM Emulation) shall be used as the storage system then
 * the define MEMIF_USE_FEE should be set in MemIf_Cfg.h.
 *
 * If the EA (external EEPROM) shall be used as the storage system then
 * the define MEMIF_USE_EA should be set in MemIf_Cfg.h.
 *
 * Configuration in MemIf_Cfg.h:
 * @code
 * #define MEMIF_USE_FEE
 * @endcode
 *
 */

#define MEMIF_FEE_ID 		((uint8)(0))	/**< Index for FEE. */
#define MEMIF_EA_ID			((uint8)(1))	/**< Index for EA. */
#define MEMIF_BROADCAST_ID  ((uint8)(255))	/**< Index to access all configured subsystems. */


/** This service sets the drivers operation mode.
 *
 * This function switches the operation mode of all configured
 * drivers to either #MEMIF_MODE_SLOW or #MEMIF_MODE_FAST.
 *
 * @param mode  #MEMIF_MODE_SLOW: slow access \n
 *              #MEMIF_MODE_FAST: fast access
 */
inline void MemIf_SetMode(MemIf_ModeType mode) {
#ifdef MEMIF_USE_FEE
	 Fee_SetMode(mode);
#endif
#ifdef MEMIF_USE_EA

#endif
}


/** This service reads from the drivers.
 *
 * This service reads for the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index        This parameter selects the respective driver (EA or FEE).
 * @param block_number     Block number that shall be read.
 * @param block_offset     Block offset of the block_number.
 * @param data_buffer_ptr  Address to which the data shall be read.
 * @param length           Number of bytes that shall be read.
 */
inline Std_ReturnType MemIf_Read(uint8 dev_index, uint16 block_number, uint16 block_offset, uint8* data_buffer_ptr, uint16 length) {
	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			return Fee_Read(block_number, block_offset, data_buffer_ptr, length);
#endif
#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:
			return Ea_Read(block_number, block_offset, data_buffer_ptr, length);
#endif
#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_READ_API_ID, MEMIF_E_PARAM_DEVICE);
			return E_NOT_OK;
#endif
	}

	return E_NOT_OK;
}



/** This service requests a write operation from the driver.
 *
 * This service requests a write operation for the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index        This parameter selects the respective driver (EA or FEE).
 * @param block_number     Block number that shall be written.
 * @param data_buffer_ptr  Address of the data that shall be written.
 */
inline Std_ReturnType MemIf_Write(uint8 dev_index, uint16 block_number, const uint8 *data_buffer_ptr) {
	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			return Fee_Write(block_number, data_buffer_ptr);
#endif
#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:
			return Ea_Write(block_number, data_buffer_ptr);
#endif
#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_WRITE_API_ID, MEMIF_E_PARAM_DEVICE);
			return E_NOT_OK;
#endif
	}

	return E_NOT_OK;
}


/** This service requests a cancel operation from the driver.
 *
 * This service requests a cancel operation for the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index        This parameter selects the respective driver (EA or FEE).
 */
inline void MemIf_Cancel(uint8 dev_index) {
	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			Fee_Cancel();
			return;
#endif
#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:

			return;
#endif
#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_CANCEL_API_ID, MEMIF_E_PARAM_DEVICE);
			return E_NOT_OK;
#endif
	}
}


/** This service requests a status from the driver.
 *
 * This service requests a status for the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index        This parameter selects the respective driver (EA or FEE or both).
 */
inline MemIf_StatusType MemIf_GetStatus(uint8 dev_index) {

	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			return Fee_GetStatus();
#endif

#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:
			return Ea_GetStatus();
#endif

#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA)
		case MEMIF_BROADCAST_ID:
		{
			MemIf_StatusType status;

			status = Fee_GetStatus();
			status |= Ea_GetStatus();
			return status;
		}
#endif

#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_GETSTATUS_API_ID, MEMIF_E_PARAM_DEVICE);
			return MEMIF_UNINIT;
#endif
	}

	return E_NOT_OK;
}



/** This service requests a the job result from the driver.
 *
 * This service requests a job result for the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index        This parameter selects the respective driver (EA or FEE).
 */
inline MemIf_JobResultType MemIf_GetJobResult(uint8 dev_index) {
	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			return Fee_GetJobResult();
#endif
#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:
			return Ea_GetJobResult();
#endif
#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_GETJOBRESULT_API_ID, MEMIF_E_PARAM_DEVICE);
			return E_NOT_OK;
#endif
	}

	return E_NOT_OK;
}


/** This service requests a invalidate operation from the driver.
 *
 * This service requests a invalidate operation of the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index     This parameter selects the respective driver (EA or FEE).
 * @param block_number  This is the block number that shall be invalidated.
 */
inline Std_ReturnType MemIf_InvalidateBlock(uint8 dev_index, uint16 block_number) {
	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			return Fee_InvalidateBlock(block_number);
#endif
#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:
			return E_OK;
#endif
#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_INVALIDATEBLOCK_API_ID, MEMIF_E_PARAM_DEVICE);
			return E_NOT_OK;
#endif
	}

	return E_NOT_OK;
}



/** This service requests a invalidate operation from the driver.
 *
 * This service requests a invalidate operation of the selected device driver. The selection is
 * performed with the parameter dev_index.
 *
 * The request is forwarded to the respective EA or FEE service.
 *
 * @param dev_index     This parameter selects the respective driver (EA or FEE).
 * @param block_number  This is the block number that shall be invalidated.
 */
inline Std_ReturnType MemIf_EraseImmediateBlock(uint8 dev_index, uint16 block_number) {
	switch(dev_index) {
#ifdef MEMIF_USE_FEE
		case MEMIF_FEE_ID:
			return Fee_EraseImmediateBlock(block_number);
#endif
#ifdef MEMIF_USE_EA
		case MEMIF_EA_ID:
			return E_OK;
#endif
#if defined(MEMIF_USE_FEE) && defined(MEMIF_USE_EA) && defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON)
		default:
			Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, MEMIF_ERASEIMMEDIATEBLOCK_API_ID, MEMIF_E_PARAM_DEVICE);
			return E_NOT_OK;
#endif
	}

	return E_NOT_OK;
}


/** The function MemIf_GetVersionInfo returns the version information of the MemIf module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function MemIf_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter MemIfVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void MemIf_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = MEMIF_MODULE_ID;
	versioninfo->sw_major_version = MEMIF_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = MEMIF_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = MEMIF_SW_PATCH_VERSION;
	versioninfo->vendorID = MEMIF_VENDOR_ID;
}

/*@}*/

#pragma nomisrac default

#endif /* MEMIF_H_ */
