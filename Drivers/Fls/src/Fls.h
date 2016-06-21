/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fls.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:21:55MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fls.h  $
 *  Revision 1.2 2010/11/22 08:21:55MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:16:39MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Fls/AA000/src/project.pj
 *
 * ***************************************************************************/
 

#ifndef FLS_H_
#define FLS_H_

#include "team-s.h"
#include "Std_Types.h"
#include "MemIf_Types.h"

/** @defgroup group_fls Fls
 *  @ingroup group_fls */
/*@{*/

#define FLS_MODULE_ID         (92)
#define FLS_VENDOR_ID         (TEAMS_VENDOR_ID)
#define FLS_AR_MAJOR_VERSION  (2)
#define FLS_AR_MINOR_VERSION  (2)
#define FLS_AR_PATCH_VERSION  (3)
#define FLS_SW_MAJOR_VERSION  (1)
#define FLS_SW_MINOR_VERSION  (0)
#define FLS_SW_PATCH_VERSION  (0)



/* Driver APIs Service ID */
#define FLS_INIT_API_ID             	((uint8)(0))
#define FLS_ERASE_API_ID             	((uint8)(1))
#define FLS_WRITE_API_ID             	((uint8)(2))
#define FLS_CANCEL_API_ID             	((uint8)(3))
#define FLS_GETSTATUS_API_ID           	((uint8)(4))
#define FLS_GETJOBRESULT_API_ID        	((uint8)(5))
#define FLS_MAINFUNCTION_API_ID			((uint8)(6))
#define FLS_READ_API_ID             	((uint8)(7))
#define FLS_COMPARE_API_ID             	((uint8)(8))
#define FLS_SETMODE_API_ID             	((uint8)(9))
#define FLS_GETVERSIONINFO_API_ID		((uint8)(0x10))

/* Development Error Codes */
#define FLS_E_PARAM_CONFIG				((uint8)(1))	/**< A wrong parameter to function Fls_Init() was passed. */
#define FLS_E_PARAM_ADDRESS				((uint8)(2))	/**< A wrong address was passed to a service. */
#define FLS_E_PARAM_LENGTH				((uint8)(3))	/**< A wrong length was passed to a service. */
#define FLS_E_PARAM_DATA				((uint8)(4))	/**< A wrong data parameter was passed to a service. */
#define FLS_E_UNINIT					((uint8)(5))	/**< A service was called without a prior Fls_Init(). */
#define FLS_E_BUSY 						((uint8)(6))	/**< A service was called while driver is still busy. */
#define FLS_E_VERIFY_ERASE_FAILED		((uint8)(7))	/**< The erase verification (blank check) failed. */
#define FLS_E_VERIFY_WRITE_FAILED		((uint8)(8))	/**< The write verification (compare) failed. */





/** @page fls Fls
 *
 * @section fls_intro Introduction
 * The Fls function implements the Fls functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_FlashDriver.pdf">AUTOSAR_SWS_FlashDriver</a>.
 *
 *
 * @section fls_init Initialization of Fls
 * Before any service to the Fls can be used the initialization function Fls_Init() with a pointer
 * proper configuration set (#Fls_ConfigType) must be called.
 *
 *
 * @section fls_autosar AutoSAR Conformity
 * Fls implements all services defined by AutoSAR.
 *
 * @section fls_tc1767 TC1767
 * The TC1767 contains 2 Flash memories -- a program flash and a data flash.
 * These flashes are divided into sectors of different sizes. The sectors are
 * again divided into pages. The flash technology allows only to erase complete
 * sectors and to write complete pages.
 *
 * @subsection fls_sector Sectors
 * The following shows the sectors of the data flash.
 * <table>
 *   <tr><th>Start Address</th><th>Size</th></tr>
 *   <tr><td>0xAFE0 0000</td><td>32 KB</td></tr>
 *   <tr><td>0xAFE1 0000</td><td>32 KB</td></tr>
 * </table>
 *
 * The following shows the sectors of the program flash.
 * <table>
 *   <tr><th>Start Address</th><th>Size</th></tr>
 *   <tr><td>0xA000 0000</td><td>16 KB</td></tr>
 *   <tr><td>0xA000 4000</td><td>16 KB</td></tr>
 *   <tr><td>0xA000 8000</td><td>16 KB</td></tr>
 *   <tr><td>0xA000 C000</td><td>16 KB</td></tr>
 *   <tr><td>0xA001 0000</td><td>16 KB</td></tr>
 *   <tr><td>0xA001 4000</td><td>16 KB</td></tr>
 *   <tr><td>0xA001 8000</td><td>16 KB</td></tr>
 *   <tr><td>0xA001 C000</td><td>16 KB</td></tr>
 *   <tr><td>0xA002 0000</td><td>128 KB</td></tr>
 *   <tr><td>0xA004 0000</td><td>256 KB</td></tr>
 *   <tr><td>0xA008 0000</td><td>256 KB</td></tr>
 *   <tr><td>0xA00C 0000</td><td>256 KB</td></tr>
 *   <tr><td>0xA010 0000</td><td>256 KB</td></tr>
 *   <tr><td>0xA014 0000</td><td>256 KB</td></tr>
 *   <tr><td>0xA018 0000</td><td>256 KB</td></tr>
 *   <tr><td>0xA01C 0000</td><td>256 KB</td></tr>
 * </table>
 *
 * @subsection fls_page Page Size
 * The data flash has a page size of 128 bytes. The program flash has a
 * page size of 256 bytes.
 *
 * @section fls_usage Usage
 * @subsection fls_usage_addr Addressing
 * The flash address is always built out of the base address of the configuration
 * Fls_ConfigType::base_address and the passed address to the Erase/Write etc. function.
 * The passed address is always rounded down towards an word (32 bit) address to achieve
 * a proper alignment.
 *
 * The address in RAM that is passed to the functions Fls_Compare(), Fls_Write()
 * and Fls_Read() needs to be aligned to a word boundary too.
 *
 * @subsection fls_usage_length Length
 * The passed length to the functions are used as a minimum length only. In
 * case of Fls_Erase() this length is always rounded up the to end of the
 * sector. In case of Fls_Write() it is rounded up the the end of a page.
 * In case of Fls_Compare() and Fls_Read() it is rounded up to fit to a word boundary.
 *
 * @section fls_cfg Configuration
 * @subsection fls_cfg_step_by_step Step by Step
 * @li Configure #Fls_ConfigType.
 * @li Call the function Fls_Init() with the pointer to the configured structure #Fls_ConfigType
 *     in the startup phase.
 * @li Call the function Fls_MainFunction() in the background task or periodically.
 *
 * @subsection fls_cfg_by_example Example
 * Configuration of #Fls_ConfigType:
 * @code
 * const Fls_ConfigType cfg = {
 *   (uint32 *)0xAFE00000,  Fls_ConfigType::base_address: base address is the data flash
 *   100,                   Fls_ConfigType::proc_time_slow: processing time of Fls_MainFunction() while in slow mode
 *   10000,                 Fls_ConfigType::proc_time_fast: processing time of Fls_MainFunction() while in fast mode
 *   NULL_PTR,              Fls_ConfigType::ram_image_addr: data flash can be processed out of program flash
 *   NULL_PTR,              Fls_ConfigType::job_end_notification: job end notification is not required
 *   NULL_PTR,              Fls_ConfigType::job_error_notification: no error notification
 * };
 * @endcode
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Fls_Init(&cfg);
 *   ...
 * }
 *
 * void background_task(void)
 * {
 *   ...
 *   Fls_MainFunction();
 *   ...
 * }
 * @endcode
 *
 */

/** Address information.
 *
 * This type is used as address offset from the configured flash base address
 * to access a certain flash memory area.
 */
typedef uint32 Fls_AddressType;

/** Length information.
 *
 * This type specifies the number of bytes to read/write/erase/compare.
 */
typedef uint32 Fls_LengthType;



/** This is the initial Fls configuration structure.
 *
 * This configuration is passed to the function Fls_Init().
 */
typedef struct {
	uint32 *base_address;		/**< Base address of the flash memory where the erase, write, ... operations shall be performed on. */
	uint32 proc_time_slow;		/**< Number of STM ticks that the Fls_MainFunction() is allowed to take in slow mode. */
	uint32 proc_time_fast;		/**< Number of STM ticks that the Fls_MainFunction() is allowed to take in fast mode. */
	uint32 *ram_image_addr;		/**< Base address where to copy the RAM image; NULL_PTR if no RAM image shall be made. */
	void (*job_end_notification)(void);		/**< The function that gets called upon completion of a job. */
	void (*job_error_notification)(void);	/**< The function that gets called in case of an error. */
} Fls_ConfigType;



#define FLS_START_SEC_CODE
#include "MemMap.h"



/** This service initializes the Fls driver.
 *
 * The function Fls_Init() initializes the Fls module. This service need to be called
 * prior to any other services.
 * It is possible to call the Fls_Init() with a configuration set when the
 * Fls functionality is in the state #MEMIF_UNINIT or in the state #MEMIF_IDLE
 * (see Fls_GetStatus()).
 *
 * @note After the execution of the function Fls_Init(), the configuration data are accessible
 * and can be used by the Fls module functions.
 */
extern void Fls_Init(const Fls_ConfigType *cfg);


/** This service erases one or more flash sectors.
 *
 * This service erases one or more complete flash sectors.
 * The FLS module will execute the job of the function Fls_Erase()
 * asynchronously within the FLS modules main function.
 * The job of the function Fls_Erase() will erase a flash memory block
 * starting from FlsBaseAddress + address of size length.
 * The length will be rounded up to the next full sector boundary
 * since only complete flash sectors can be erased.
 *
 * The status of the erase operation can
 * be retrieved by Fls_GetJobResult().
 *
 * @param   address    The target address in flash memory. This address offset
 *                     will be added to the flash memory base address.
 *                     [0..FLS_SIZE[
 * @param   length     The number of bytes to erase. [1 .. FLS_SIZE - address]
 * @return  #E_OK:     erase command has been accepted \n
 *          #E_NOT_OK: erase command has not been accepted
 */
extern Std_ReturnType Fls_Erase(Fls_AddressType address, Fls_LengthType length);


/** This service writes to one or more complete flash pages.
 *
 * This service writes one or more complete flash pages to the flash device.
 * The write job of the function Fls_Write() is performed asynchronously
 * within the FLS modules main function.
 * The job of the function Fls_Write() programs a flash memory block
 * with data provided via src_addr starting from FlsBaseAddress +
 * dst_addr of size length.
 *
 * The service Fls_Write() always writes a complete page.
 *
 * The status of the write operation can
 * be retrieved by Fls_GetJobResult().
 *
 * @param   dst_addr   The target address in flash memory. This address offset
 *                     will be added to the flash memory base address.
 *                     [0..FLS_SIZE[
 * @param   length     The number of bytes to write. [1 .. FLS_SIZE - address]
 * @param   src_addr   The pointer to source data buffer containing the data
 *                     that shall be written.
 * @return  #E_OK:     The write command has been accepted \n
 *          #E_NOT_OK: The write command has not been accepted
 *
 */
extern Std_ReturnType Fls_Write(Fls_AddressType dst_addr, const uint8* src_addr, Fls_LengthType length);


/** This service cancels an ongoing job.
 *
 * The function Fls_Cancel() cancels an ongoing flash read, write, erase
 * or compare job.
 * After the usage of this service a new job can be started -- the
 * module state changes to #MEMIF_IDLE.
 *
 * If a error notification is configured, the function Fls_Cancel() calls the
 * error notification function to inform the caller about the cancellation of
 * a job.
 *
 * The FLS modules states and data of the affected flash memory cells are
 * undefined when canceling an ongoing job with the function Fls_Cancel().
 */
extern void Fls_Cancel(void);


/** This service returns the state of the driver.
 *
 * The service Fls_GetStatus() returns the FLS module state synchronously.
 *
 * @return  #MEMIF_UNINIT: The Fls driver is not yet initialized. \n
 *          #MEMIF_IDLE:   The Fls driver is currently idle. \n
 *          #MEMIF_BUSY:   The Fls driver is currently busy.
 */
extern MemIf_StatusType Fls_GetStatus(void);


/** This service returns the result of the last job.
 *
 * The erase, write, read and compare functions share the same job
 * result, i.e. only the result of the last job can be queried.
 * The FLS module overwrites the job result with #MEMIF_JOB_PENDING
 * if the FLS module has accepted a new job.
 *
 * @return 	#MEMIF_JOB_OK:        the last job completed successfully or no job has been started\n
 *          #MEMIF_JOB_FAILED:    the last job failed \n
 *          #MEMIF_JOB_PENDING:   the last job is ongoing \n
 *          #MEMIF_JOB_CANCELLED: the last job has been canceled \n
 *          #MEMIF_BLOCK_INCONSISTENT: the compare job found a difference in the comparison
 */
extern MemIf_JobResultType Fls_GetJobResult(void);


/** This service reads from flash memory.
 *
 * The FLS module executes the read job of the function Fls_Read()
 * asynchronously within the FLS modules main function.
 *
 * The read job of the function Fls_Read() copies a continuous flash
 * memory block starting from FlsBaseAddress + src_addr of size length
 * to the buffer pointed to by dst_addr.
 *
 * @param src_addr   The source address in flash memory. This address
 *                   offset will be added to the flash memory base address.
 *                   [0..FLS_SIZE - 1]
 * @param length     The number of bytes to read [1..FLS_SIZE - SourceAddress]
 * @param dst_addr   A pointer to target data buffer.
 *
 * @return  #E_OK:     The read command has been accepted \n
 *          #E_NOT_OK: The read command has not been accepted
 */
extern Std_ReturnType Fls_Read(Fls_AddressType src_addr, uint8* dst_addr, Fls_LengthType length);


/** This service compares the contents of an area in flash memory with that of an application data buffer.
 *
 * The job of the function Fls_Compare() compares a continuous flash
 * memory block starting from FlsBaseAddress + src_addr of size length
 * with the buffer pointed to by dst_addr.
 *
 * The function Fls_Compare() initiates a compare job and sets the status
 * to MEMIF_BUSY, set the job result to MEMIF_JOB_PENDING and return
 * with E_OK.
 *
 * The FLS module executes the job of the function Fls_Compare()
 * asynchronously within the FLS modules main function.
 *
 * If the comparison shows a difference the result of the job that can
 * be retrieved by Fls_GetJobResult() will be returned as #MEMIF_BLOCK_INCONSISTENT.
 *
 * @param src_addr   The source address in flash memory. This address
 *                   offset will be added to the flash memory base address.
 *                   [0..FLS_SIZE - 1]
 * @param dst_addr   A pointer to target data buffer.
 * @param length     The number of bytes to compare [1..FLS_SIZE - SourceAddress]
 *
 * @return  #E_OK:     The compare command has been accepted \n
 *          #E_NOT_OK: The compare command has not been accepted
 *
 */
extern Std_ReturnType Fls_Compare(Fls_AddressType src_addr, const uint8* dst_addr, Fls_LengthType length);


/** This service sets the flash drivers operation mode.
 *
 * The function Fls_SetMode() sets the FLS modules operation mode to
 * the given mode parameter.
 *
 * If the passed parameter is MEMIF_MODE_SLOW then the parameter
 * Fls_ConfigType::proc_time_slow determines the minimum processing time
 * taken in Fls_MainFunction().
 *
 * If the passed parameter is MEMIF_MODE_FAST then the parameter
 * Fls_ConfigType::proc_time_fast determines the minimum processing time
 * taken in Fls_MainFunction().
 *
 * The processing time is passed as ticks of STM timer. The STM
 * get configured by @ref mcu with the configuration parameter
 * #MCU_STM_CLC_VAL.
 *
 * @param mode  #MEMIF_MODE_SLOW: slow read access \n
 *              #MEMIF_MODE_FAST: fast read access
 */
extern void Fls_SetMode(MemIf_ModeType mode);

/** This service performs the processing of the jobs.
 *
 * This service needs to be called periodically once a job
 * needs to be performed. The function may also be called cyclically
 * if no job is currently pending.
 *
 * This function performs the processing of the flash read, write,
 * erase and compare jobs.
 */
extern void Fls_MainFunction(void);


/** The function Fls_GetVersionInfo returns the version information of the Fls module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Fls_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter FlsVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Fls_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = FLS_MODULE_ID;
	versioninfo->sw_major_version = FLS_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = FLS_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = FLS_SW_PATCH_VERSION;
	versioninfo->vendorID = FLS_VENDOR_ID;
}

/*@}*/

#define FLS_STOP_SEC_CODE
#include "MemMap.h"


#endif /* FLS_H_ */
