/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fee.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:25:50MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fee.h  $
 *  Revision 1.4 2011/03/22 09:25:50MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.3 2010/11/22 08:20:51MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:22:44MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef FEE_H_
#define FEE_H_

#include "team-s.h"
#include "Fls.h"
#include "Fee_Cfg.h"
#include "Std_Types.h"

#define FEE_MODULE_ID         (21)
#define FEE_VENDOR_ID         (TEAMS_VENDOR_ID)
#define FEE_AR_MAJOR_VERSION  (2)
#define FEE_AR_MINOR_VERSION  (2)
#define FEE_AR_PATCH_VERSION  (2)
#define FEE_SW_MAJOR_VERSION  (1)
#define FEE_SW_MINOR_VERSION  (0)
#define FEE_SW_PATCH_VERSION  (0)


/** @defgroup group_fee Fee
 *  @ingroup group_fee */
/*@{*/

/* Driver APIs Service ID */
#define FEE_INIT_API_ID             	((uint8)(0))
#define FEE_SETMODE_API_ID             	((uint8)(1))
#define FEE_READ_API_ID             	((uint8)(2))
#define FEE_WRITE_API_ID             	((uint8)(3))
#define FEE_CANCEL_API_ID             	((uint8)(4))
#define FEE_GETSTATUS_API_ID           	((uint8)(5))
#define FEE_GETJOBRESULT_API_ID        	((uint8)(6))
#define FEE_INVALIDATEBLOCK_API_ID      ((uint8)(7))
#define FEE_GETVERSIONINFO_API_ID		((uint8)(8))
#define FEE_ERASEIMMEDIATEBLOCK_API_ID	((uint8)(9))
#define FEE_JOBENDNOTIFICATION_API_ID   ((uint8)(0x10))
#define FEE_JOBERRORNOTIFICATION_API_ID ((uint8)(0x11))
#define FEE_MAINFUNCTION_API_ID         ((uint8)(0x12))

/* Development Error Codes */
#define FEE_E_INVALID_BLOCK_NO			((uint8)(0x02))	/**< API service called with invalid block number. */
#define FEE_E_FLSREAD			        ((uint8)(0x10))	/**< Internal error; Fls_Read has failed. */
#define FEE_E_FLSWRITE			        ((uint8)(0x11))	/**< Internal error; Fls_Write has failed. */
#define FEE_E_FLSERASE			        ((uint8)(0x12))	/**< Internal error; Fls_Erase has failed. */


/** @page fee Fee
 *
 * @section fee_intro Introduction
 * The Fee function implements the Fee functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_FEE_Driver.pdf">AUTOSAR_SWS_FEE_Driver</a>.
 *
 *
 * @section fee_init Initialization of Fee
 * Before any service to the Fee can be used the initialization function Fee_Init() must be called.
 * Since all services of Fee are processed in the mainloop the function Fee_MainFunction() must be
 * called periodically.
 *
 * It needs to be ensured that the initialization of the Fls driver is performed before the
 * initialization of Fee is requested.
 *
 *
 * @section fee_autosar AutoSAR Conformity
 * Fee implements all services defined by AutoSAR.
 *
 *
 * @section fee_memory_layout Memory Layout
 * The following diagrams show the memory layout of a single segment in the data flash.
 * The knowledge of the layout is useful to understand the different configuration
 * options that are available in the Fee driver.
 *
 * @image html  external_images/fee_memory_layout.png "Memory Layout"
 * @image latex external_images/fee_memory_layout.png "Memory Layout" width=14cm
 *
 * The page "serial" has the following format:
 * @image html  external_images/fee_memory_layout_serial.png "Memory Layout Serial Page"
 * @image latex external_images/fee_memory_layout_serial.png "Memory Layout Serial Page" width=14cm
 *
 * The page "Management" has the following format:
 * @image html  external_images/fee_memory_layout_mgmt.png "Memory Layout Management Page"
 * @image latex external_images/fee_memory_layout_mgmt.png "Memory Layout Management Page" height=22cm
 *
 *
 * @section fee_nomenclature Nomenclature
 * @subsection fee_nomen_generation Generation
 * A generation holds the latest information that is written by the provided interface.
 * To support a larger amount of write cycles a segment of the flash memory holds
 * several generations. To achieve the desired number of write cycles the number of
 * generations must be selected that way that the number of erase cycles of the
 * data flash is not exceeded. The selection of the number of generations is always
 * a trade of between lifetime and amount of data that can be stored.
 *
 * @subsection fee_nomen_block Block
 * A block contains the data that is written with the provided interface. A block can
 * is always aligned to a page boundary. This also means that a block is always a multiple
 * of 128 bytes. However not all data of the block must be written with valid data.
 *
 *
 * @section fee_cfg Configuration
 * @subsection fee_cfg_step_by_step Step by Step
 * @li Configure @ref fls to use support the access to data flash only
 * @li Configure #Fee_BlockConfiguration.
 * @li Configure #FEE_NUM_OF_IMMEDIATE_BLOCKS
 * @li Configure #FEE_NUM_OF_MGMT_BLOCKS
 * @li Configure #FEE_NUM_OF_GENERATIONS
 *
 *
 * @subsection fee_cfg_by_example Example
 * Configuration of #Fee_BlockConfiguration:
 * @code
 * const Fee_BlockConfiguration fee_block_configuration[] = {
 *   {   0,         Fee_BlockConfiguration::block_number
 *     128,         Fee_BlockConfiguration::block_size
 *       0          Fee_BlockConfiguration::immediate_data
 *   },
 *   {   1,         Fee_BlockConfiguration::block_number
 *     128,         Fee_BlockConfiguration::block_size
 *       0          Fee_BlockConfiguration::immediate_data
 *   },
 *   {   2,         Fee_BlockConfiguration::block_number
 *     250,         Fee_BlockConfiguration::block_size
 *       0          Fee_BlockConfiguration::immediate_data
 *   },
 *   {   4,         Fee_BlockConfiguration::block_number
 *     300,         Fee_BlockConfiguration::block_size
 *       1          Fee_BlockConfiguration::immediate_data
 *   },
 * };
 * @endcode
 *
 * Configuration of #FEE_NUM_OF_IMMEDIATE_BLOCKS:
 * @code
 * #define FEE_NUM_OF_IMMEDIATE_BLOCKS	4		Number of blocks that require an immediate write access.
 * @endcode
 * This configuration data holds the number of blocks that need to be written without a change
 * of the segment. In the consequence this means that the management blocks for this amount
 * is reserved for the immediate blocks.\
 *
 * Configuration of #FEE_NUM_OF_MGMT_BLOCKS:
 * @code
 * #define FEE_NUM_OF_MGMT_BLOCKS		50		Number of mgmt. blocks; each write occupies one mgmt. block.
 * @endcode
 * See the memory layout (@ref fee_memory_layout) for a description of this configuration value.\n
 *
 * Configuration of #FEE_NUM_OF_GENERATIONS:
 * @code
 * #define FEE_NUM_OF_GENERATIONS      4		Number of generations a segment shall be able to hold.
 * @endcode
 * See the memory layout (@ref fee_memory_layout) for a description of this configuration value.
 *
 */

/** Configuration of the block structure.
 *
 * This typedef is used to configure the block structure of the data sets that
 * can be read and written by the Fee driver. It should be noted that not all
 * blocks need to be used/configured. This allows a later introduction of new
 * blocks.
 */
typedef struct {
	uint16 block_number;	/**< Block number. */
	uint16 block_size;		/**< Size of the block in bytes. */
	uint8  immediate_data;	/**< Set to 1 if this block shall be able to be written immediatelly; 0 otherwise */
} Fee_BlockConfiguration;



/** This service initializes the Fee driver.
 *
 * The function Fee_Init initializes the Fee module.
 * This function forces the Fee driver to trigger the reading of the
 * management structures in the data flash.
 *
 * It is required to call  Fee_MainFunction() since the reading is
 * done in the background and not immediately at the call of this
 * function.
 */
extern void Fee_Init(void);


/** This service sets the mode of the underlying flash driver.
 *
 * This service calls the Fls_SetMode() function of the underlying flash driver.
 *
 * @param mode  #MEMIF_MODE_SLOW: slow read access \n
 *              #MEMIF_MODE_FAST: fast read access
 */
inline void Fee_SetMode(MemIf_ModeType mode)
{
	Fls_SetMode(mode);
}


/** Service to initiate a read job.
 *
 * The function Fee_Read() takes the block start address and offset and calculates
 * the corresponding memory read address.
 *
 * Note: The address offset and length parameter can take any value within the given
 * types range. This allows reading of an arbitrary number of bytes from an arbitrary
 * start address inside a logical block.
 *
 * The function Fee_Read() copies the given / computed parameters to
 * module internal variables, initiate a read job, set the Fee module status to
 * #MEMIF_BUSY, set the job result to #MEMIF_JOB_PENDING and return with E_OK.
 *
 * The Fee module executes the job of the function Fee_Read() asynchronously within
 * the Fee modules main function Fee_MainFunction().
 *
 * @param   block_number  Number of the logical block, also denoting start address of
 *                        that block in flash memory.
 * @param   block_offset  Read address offset inside the block.
 * @param   length        Number of bytes to read.
 * @param   buf           Pointer to a RAM region to which the data shall be copied.
 * @return  #E_OK:        The job has been accepted \n
 *          #E_NOT_OK:    The job has not been accepted
 */
extern Std_ReturnType Fee_Read(uint16 block_number, uint16 block_offset, uint8 *buf, uint16 length);


/** Service to initiate a write job.
 *
 * The function Fee_Write() takes the block start address and calculates
 * the corresponding memory write address. The block address offset is zero.
 *
 * The function Fee_Write() copies the given / computed parameters to
 * module internal variables, initiate a write job, set the Fee module status to
 * #MEMIF_BUSY, set the job result to #MEMIF_JOB_PENDING and return with #E_OK.
 *
 * The Fee module executes the write job of the function Fee_Write()
 * asynchronously within the FEE modules main function Fee_MainFunction().
 *
 * @param   block_number  Number of the logical block, also denoting start address of
 *                        that block in flash memory.
 * @param   buf           Pointer from which the data shall be copied.
 */
extern Std_ReturnType Fee_Write(uint16 block_number, const uint8 *buf);


/** Service to call the cancel function of the underlying flash driver.
 *
 * The function Fee_Cancel() calls the cancel function of the underlying
 * flash driver.
 *
 * The function Fee_Cancel() resets the Fee modules internal variables
 * to make the module ready for a new job request.
 *
 * Note: The function Fee_Cancel() and the cancel function of the underlying flash
 * driver are asynchronous w.r.t. an ongoing read, erase or write job in the flash
 * memory. The cancel functions shall only reset their modules internal variables
 * so that a new job can be accepted by the modules. They do not cancel an ongoing
 * job in the hardware and they do not wait for an ongoing job to be finished by
 * the hardware.
 */
extern void Fee_Cancel(void);


/** Service to call the GetStatus function of the underlying flash driver.
 *
 * If no internal operation is currently ongoing, the function Fee_GetStatus()
 * call the Fls_GetStatus() function of the underlying flash driver and pass its
 * return value back to the caller.
 *
 * The function Fee_GetStatus() returns #MEMIF_BUSY_INTERNAL, if an internal
 * operation is currently ongoing.
 *
 * @return  #MEMIF_UNINIT: The Fls driver is not yet initialized. \n
 *          #MEMIF_IDLE:   The Fls driver is currently idle. \n
 *          #MEMIF_BUSY:   The Fls driver is currently busy. \n
 *          #MEMIF_BUSY_INTERNAL: The Fee driver is currently busy.
 */
extern MemIf_StatusType Fee_GetStatus(void);


/** Service to call the Fls_GetJobResult() function of the underlying flash driver.
 *
 * The function Fee_GetJobResult() returns the status of the last initited
 * job.
 *
 * @return 	#MEMIF_JOB_OK:        the last job completed successfully or no job has been started\n
 *          #MEMIF_JOB_FAILED:    the last job failed \n
 *          #MEMIF_JOB_PENDING:   the last job is ongoing \n
 *          #MEMIF_JOB_CANCELLED: the last job has been canceled \n
 *          #MEMIF_BLOCK_INVALID: the requested block was invalidated \n
 *          #MEMIF_BLOCK_INCONSISTENT: the requested block is in an inconsistent state (write operation not completed)
 */
extern MemIf_JobResultType Fee_GetJobResult(void);


/** Service to invalidate a logical block.
 *
 * The function Fee_InvalidateBlock() takes the block number and
 * calculates the corresponding memory block address.
 *
 * The function Fee_InvalidateBlock() invalidates the requested
 * block number. If the respective block number is requested to be
 * read then the job status would return #MEMIF_BLOCK_INVALID.
 *
 * @param   block_number  Number of the logical block, also denoting start address of
 *                        that block in flash memory.
 * @return  #E_OK:        The job has been accepted \n
 *          #E_NOT_OK:    The job has not been accepted
 */
extern Std_ReturnType Fee_InvalidateBlock(uint16 block_number);


/** Service to erase a logical block.
 *
 * The function Fee_EraseImmediateBlock() takes the block number
 * and calculates the corresponding memory block address.
 *
 * The function Fee_EraseImmediateBlock() ensures that the Fee
 * module can write immediate data. Whether this involves physically
 * erasing a memory area and therefore calling the erase function of
 * the underlying driver depends on the implementation.
 *
 * Note: The function Fee_EraseImmediateBlock() shall only be called
 * by e.g. diagnostic or similar system service to pre-erase the area
 * for immediate data if necessary.
 *
 * @param   block_number  Number of the logical block, also denoting start address of
 *                        that block in flash memory.
 * @return  #E_OK:        The job has been accepted \n
 *          #E_NOT_OK:    The job has not been accepted
 */
extern Std_ReturnType Fee_EraseImmediateBlock(uint16 block_number);


/** The function Fee_GetVersionInfo returns the version information of the Fee module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Fee_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter FeeVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Fee_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = FEE_MODULE_ID;
	versioninfo->sw_major_version = FEE_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = FEE_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = FEE_SW_PATCH_VERSION;
	versioninfo->vendorID = FEE_VENDOR_ID;
}

/** Service to handle the requested read / write / erase jobs respectively
 * the internal management operations.
 *
 * The function Fee_MainFunction() asynchronously handles the requested
 * read / write / erase jobs respectively the internal management operations.
 */
extern void Fee_MainFunction(void);


/*@}*/

#endif /* FEE_H_ */
