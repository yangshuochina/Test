/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/21 14:13:09MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM.h  $
 *  Revision 1.4 2011/02/21 14:13:09MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.3 2010/11/22 08:31:56MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:27:06MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef NVM_H_
#define NVM_H_

#include "team-s.h"
#include "Std_Types.h"
#include "NvM_Cfg.h"

/** @defgroup group_nvm NvM 
 *  @ingroup group_nvm */
/*@{*/

#define NVM_MODULE_ID         (20)
#define NVM_VENDOR_ID         (TEAMS_VENDOR_ID)
#define NVM_AR_MAJOR_VERSION  (2)
#define NVM_AR_MINOR_VERSION  (2)
#define NVM_AR_PATCH_VERSION  (2)
#define NVM_SW_MAJOR_VERSION  (2)
#define NVM_SW_MINOR_VERSION  (0)
#define NVM_SW_PATCH_VERSION  (0)

/* Driver APIs Service ID */
#define NVM_INIT_API_ID					((uint8)(0))
#define NVM_SETDATAINDEX_API_ID			((uint8)(1))
#define NVM_GETDATAINDEX_API_ID			((uint8)(2))
#define NVM_SETBLOCKPROTECTION_API_ID	((uint8)(3))
#define NVM_GETERRORSTATUS_API_ID		((uint8)(4))
#define NVM_SETRAMBLOCKSTATUS_API_ID	((uint8)(5))
#define NVM_READBLOCK_API_ID			((uint8)(6))
#define NVM_WRITEBLOCK_API_ID			((uint8)(7))
#define NVM_RESTOREBLOCKDEFAULTS_API_ID	((uint8)(8))
#define NVM_ERASENVBLOCK_API_ID			((uint8)(9))
#define NVM_CANCELWRITEALL_API_ID		((uint8)(10))
#define NVM_INVALIDATENVBLOCK_API_ID	((uint8)(11))
#define NVM_READALL_API_ID				((uint8)(12))
#define NVM_WRITEALL_API_ID				((uint8)(13))
#define NVM_MAINFUNCTION_API_ID			((uint8)(14))
#define NVM_GETVERSIONINFO_API_ID		((uint8)(15))

/* Development Error Codes */
#define NVM_E_PARAM_BLOCK_ID			((uint8)(0xA))	/**< API requests called with wrong parameter */
#define NVM_E_PARAM_BLOCK_TYPE			((uint8)(0xB))	/**< API requests called with wrong parameter */
#define NVM_E_PARAM_BLOCK_DATA_IDX		((uint8)(0xC))	/**< API requests called with wrong parameter */
#define NVM_E_PARAM_ADDRESS				((uint8)(0xD))	/**< API requests called with wrong parameter */
#define NVM_E_PARAM_DATA				((uint8)(0xE))	/**< API requests called with wrong parameter */
#define NVM_E_NOT_INITIALIZED			((uint8)(0x14))	/**< NVRAM manager is still not initialized */
#define NVM_E_BLOCK_PENDING				((uint8)(0x15))	/**< API read/write/control request failed because a block with the same ID is already listed or currently in progress */
#define NVM_E_LIST_OVERFLOW				((uint8)(0x16))	/**< NVRAM manager job queue overflow occurred */
#define NVM_E_NV_WRITE_PROTECTED		((uint8)(0x17))	/**< A write attempt to a write protected NVRAM block was requested. */
#define NVM_E_BLOCK_CONFIG				((uint8)(0x18))	/**< The service is not possible with this block configuration. */
#define NVM_E_ERROR_CODE				((uint8)(0x40))	/**< A called API returned E_NOT_OK. */
#define NVM_E_PARAM_CONFIG				((uint8)(0x41))	/**< A problem with the configuration was detected. */

/** @page nvm NvM
 *
 * @section nvm_intro Introduction
 * The NvM function implements the NvM functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_NVRAMManager.pdf">AUTOSAR_SWS_NVRAMManager</a>.
 *
 *
 * @section nvm_init Initialization of NvM
 * Before any service to the NvM can be used the initialization function NvM_Init() must be called.
 * The function uses the configuration data of the constant nvm_cfg.
 *
 *
 * @section nvm_autosar AutoSAR Conformity
 * NvM implements all services defined by AutoSAR configuration class 1.
 *
 * The following restrictions apply:
 * - No implementation of write protection.
 * - Only support of block type NVM_BLOCK_NATIVE.
 *
 *
 * @section nvm_cfg Configuration
 * @subsection nvm_cfg_step_by_step Step by Step
 * @li Configure #NvM_ConfigType.
 * @li Configure #NvM_ConfigBlockType.
 * @li Provide reset safe memory for field NvM_ConfigBlockType::status
 * @li Provide reset safe memory for field NvM_ConfigBlockType::ram_image
 * @li Configure @ref fee with the corresponding data.
 * @li Call the function NvM_Init() in the startup phase.
 * @li Call the function NvM_MainFunction() periodically to let the NvM process its jobs.
 *
 *
 * @subsection nvm_cfg_by_example Example
 * Configuration of #NvM_ConfigType:
 * @code
 * const NvM_ConfigType nvm_cfg = {
 *   nvm_cfg_blocks,  NvM_ConfigType::blocks Pointer to the block configuration.
 *   4,               NvM_ConfigType::num_of_blocks 4 block; 2 for administration 2 for user data.
 *   100,             NvM_ConfigType::crc_calc_cnt calculate 100 bytes of CRC at each call of NvM_MainFunction().
 *   1                NvM_ConfigType::config_id ID for the configuration of the blocks.
 * };
 * @endcode
 *
 * Configuration of #NvM_ConfigType:
 * @code
 * const NvM_ConfigBlockType nvm_cfg_blocks[] = {
 *   { block 0 - not used by NvM
 *     0,               NvM_ConfigBlockType::enable_readall: not loaded
 *     NVM_CRC_NONE,    NvM_ConfigBlockType::crc_type: no CRC
 *     MEMIF_FEE_ID,    NvM_ConfigBlockType::dev_id: use Fee
 *     0,               NvM_ConfigBlockType::block_number: block number 0 of Fee
 *     0,               NvM_ConfigBlockType::block_size: block size in bytes
 *     NULL_PTR,        NvM_ConfigBlockType::ram_image: location of copy in RAM
 *     NULL_PTR,        NvM_ConfigBlockType::rom_image: location of default data
 *     NULL_PTR,        NvM_ConfigBlockType::status: storage for variables for this block
 *     NULL_PTR,        NvM_ConfigBlockType::init_block: function that sets up the default data
 *     NULL_PTR         NvM_ConfigBlockType::single_block_callback: function that gets called when block is processed
 *   },
 *   { block 1 - used by NvM administration data
 *     1,               NvM_ConfigBlockType::enable_readall: not loaded
 *     NVM_CRC16,       NvM_ConfigBlockType::crc_type: use CRC16
 *     MEMIF_FEE_ID,    NvM_ConfigBlockType::dev_id: use Fee
 *     0,               NvM_ConfigBlockType::block_number: block number 0 of Fee
 *     sizeof(NvM_Block1), NvM_ConfigBlockType::block_size: block size in bytes
 *     NULL_PTR,        NvM_ConfigBlockType::ram_image: location of copy in RAM
 *     NULL_PTR,        NvM_ConfigBlockType::rom_image: location of default data -- none will be created automatically
 *     &nvm_status[0],  NvM_ConfigBlockType::status: storage for variables for this block
 *     NULL_PTR,        NvM_ConfigBlockType::init_block: function that sets up the default data
 *     NULL_PTR         NvM_ConfigBlockType::single_block_callback: function that gets called when block is processed
 *   },
 *   { block 2 - used by user data
 *     1,               NvM_ConfigBlockType::enable_readall: not loaded
 *     NVM_CRC16,       NvM_ConfigBlockType::crc_type: use CRC16
 *     MEMIF_FEE_ID,    NvM_ConfigBlockType::dev_id: use Fee
 *     1,               NvM_ConfigBlockType::block_number: block number 0 of Fee
 *     sizeof(block2_data), NvM_ConfigBlockType::block_size: block size in bytes
 *     &block2_data,    NvM_ConfigBlockType::ram_image: location of copy in RAM
 *     &block2_def,     NvM_ConfigBlockType::rom_image: location of default data
 *     &nvm_status[1],  NvM_ConfigBlockType::status: storage for variables for this block
 *     NULL_PTR,        NvM_ConfigBlockType::init_block: function that sets up the default data
 *     NULL_PTR         NvM_ConfigBlockType::single_block_callback: function that gets called when block is processed
 *   },
 *   { block 3 - used by user data
 *     1,               NvM_ConfigBlockType::enable_readall: not loaded
 *     NVM_CRC32,       NvM_ConfigBlockType::crc_type: use CRC16
 *     MEMIF_FEE_ID,    NvM_ConfigBlockType::dev_id: use Fee
 *     4,               NvM_ConfigBlockType::block_number: block number 4 of Fee
 *     sizeof(block3_data), NvM_ConfigBlockType::block_size: block size in bytes
 *     &block3_data,    NvM_ConfigBlockType::ram_image: location of copy in RAM
 *     NULL_PTR,        NvM_ConfigBlockType::rom_image: location of default data -- none
 *     &nvm_status[2],  NvM_ConfigBlockType::status: storage for variables for this block
 *     NULL_PTR,        NvM_ConfigBlockType::init_block: function that sets up the default data
 *     NULL_PTR         NvM_ConfigBlockType::single_block_callback: function that gets called when block is processed
 *   }
 * };
 * @endcode
 *
 * Providing reset safe data storage:
 * @code
 * #define NVM_START_SEC_VAR_NOINIT_32BIT
 * #include "MemMap.h"
 * NvM_BlockStatus nvm_status[3];
 * @endcode
 *
 * Providing storage for user data and its defaults:
 * @code
 * typedef struct {
 *   ...
 * } Block2;
 *
 * typedef struct {
 *   ...
 * } Block3;
 *
 * #define NVM_START_SEC_VAR_NOINIT_32BIT
 * #include "MemMap.h"
 * Block2 block2_data = {
 *   ...
 * };
 *
 * Block3 block3_data = {
 *   ...
 * };
 *
 * #define NVM_START_SEC_CONFIG_DATA_32BIT
 * #include "MemMap.h"
 * const Block2 block2_def = {
 *   ...
 * };
 * @endcode
 *
 *
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   NvM_Init();
 *   ...
 * }
 * @endcode
 *
 */

#define NVM_CRC_NONE		((uint8)0)	/**< Use no CRC calculation to ensure block integrity */
#define NVM_CRC16			((uint8)1)	/**< Use CRC16 calculation to ensure block integrity */
#define NVM_CRC32			((uint8)2)	/**< Use CRC32 calculation to ensure block integrity */

#define NVM_BLOCK_DATASET	((uint8)0)	/**< NVRAM block is configured to be of dataset type. */
#define NVM_BLOCK_NATIVE	((uint8)1)	/**< NVRAM block is configured to be of native type. */
#define NVM_BLOCK_REDUNDANT	((uint8)2)	/**< NVRAM block is configured to be of redundant type. */


/* data for block 1 */
typedef struct {
	uint16 id;
	uint32 crc_of_block[NVM_MAX_NUM_OF_BLOCKS];
} NvM_Block1;

/* requests for blocks */
typedef enum {
	NVM_REQ_IDLE,				/* block is idle */
	NVM_REQ_VERIFY_CRC,			/* CRC is verification is needed */
	NVM_REQ_VERIFY_CRC_WAIT,	/* CRC verification is in progress */
	NVM_REQ_LOAD_EE,			/* image from EE is currently loaded */
	NVM_REQ_START_CALC_CRC,		/* start the calculation of a CRC */
	NVM_REQ_CALC_CRC,			/* CRC is calculated */
	NVM_REQ_VERIFY_ID,			/* ID of block 1 shall be read */
	NVM_REQ_WRITE,				/* start the writing of the block */
	NVM_REQ_WRITE_EE,			/* perform the writing */
	NVM_REQ_CALC_CRC_AND_COMPARE_WITH_EE,	/* calculate CRC and compare it against the CRC stored in block 1 */
	NVM_REQ_RECOVER_BLOCK,		/* recover a block from it's defaults */
	NVM_REQ_VERIFY_ID_WAIT,		/* wait for verification of ID */
	NVM_REQ_LOAD,				/* start the reading of the block */
} NvM_ReqType;


/* block status variables */
typedef struct {
	uint32 ram_crc;				/* CRC of the block in RAM */
	uint8 *ram_image;			/* Address of the RAM image. */
	uint8 block_status;			/* set if the block is consistent */
	NvM_ReqType req;			/* holds the current request */
	NvM_RequestResultType result;	/* holds the latest job result. */

	uint32 ram_crc_work;		/* CRC of the block in RAM. Working copy. */
	uint8 *ptr;					/* Temporary pointer for job processing */
	uint16 cnt;					/* Temporary counter for job processing */
} NvM_BlockStatus;


/** Type for the definition of a single block.
 *
 * For the configuration the following must be considered:
 * - The field NvM_ConfigBlockType::block_number must be available on the lower level driver too (e.g. @ref fee).
 * - The field NvM_ConfigBlockType::block_size must match the size of the size of the lower level driver (e.g. @ref fee).
 * - If the field NvM_ConfigBlockType::enable_readall is set to 1 then the field NvM_ConfigBlockType::ram_image must be
 *   a pointer to a reset safe memory region of the size NvM_ConfigBlockType::block_size.
 * - The field NvM_ConfigBlockType::status must be a pointer to a reset safe memory region of the size NvM_BlockStatus.
 */
typedef struct {
	uint8 enable_readall;                 /**< This block shall be processed by NvM_ReadAll() [0=not processed, 1=processed]. */
	uint8 crc_type;                       /**< Type of the CRC calculation [NVM_CRC_NONE, NVM_CRC16, NVM_CRC32] */
	uint8 dev_id;                         /**< ID for the device to use [MEMIF_FEE_ID, MEMIF_EA_ID]. */
	uint16 block_number;                  /**< Block number of EA or FEE where this block is located. */
	uint16 block_size;                    /**< Size of this block in bytes [1..65535]. */
	uint8 *ram_image;                     /**< Address of the RAM image. */
	const uint8 *rom_image;               /**< Address of the ROM image that is used to load the default data (NULL_PTR if no image is existing). */
	NvM_BlockStatus *status;              /**< Pointer to reset safe RAM region to hold the status for the block. */
	void (*init_block)(void);             /**< This function is called if the block needs to be initialized with default values. */
	void (*single_block_callback)(void);  /**< This function is called on termination of a block request. */
} NvM_ConfigBlockType;


/** This is the initial NvM configuration structure.
 *
 * There must a a constant array with the name "nvm_cfg" that provides the NvM driver with
 * the configuration data.
 *
 * The following must apply:
 * - The configured NvM_ConfigType::block[0] cannot be used due to the AutoSAR specification.
 *   However it needs to be configured.
 * - The configured NvM_ConfigType::block[1] is used for administration data (configuration ID,
 *   and CRCs). The size depends on the number of blocks. The use of sizeof(NvM_Block1) is recommended.
 *   However the Fee driver needs to be configured accordingly.
 * - The field NvM_ConfigType::config_id is used to detect incompatible changes of the configuration.
 *   If the stored configuration ID of block 1 is not identical to the config_id in the configuration
 *   set then NVM_REQ_INTEGRITY_FAILED would be reported for block 1 as error status by Nvm_GetErrorStatus().
 * - The field NvM_ConfigType::crc_calc_cnt defines the number of bytes that are checked at each
 *   call of NvM_MainFunction().
 */
typedef struct {
	const NvM_ConfigBlockType *blocks;	/**< Pointer to the block configuration. */
	uint16 num_of_blocks;				/**< Number of blocks contained in the configuration. */
	uint16 crc_calc_cnt;				/**< Number of bytes to be checked in NvM_MainFunction(). */
	uint16 config_id;					/**< ID for the configuration of the blocks. */
} NvM_ConfigType;



/** This service initializes the NvM driver.
 *
 * The function NvM_Init() resets all internal variables, e.g. the queues,
 * request flags, state machines, to their initial values. After the usage
 * of this service all interfaces can be used.
 *
 * The function NvM_Init() does not modify the permanent RAM block contents,
 * as this is done by NvM_ReadAll().
 *
 * The function NvM_Init() does not initialize other modules (it is assumed
 * that the underlying layers are already initialized).
 */
extern void NvM_Init(void);


/** Service to read the block dependent error/status information.
 *
 * The function NvM_GetErrorStatus() reads the block dependent
 * error/status information.
 *
 * @note Since this interface is a mixture between job result and block
 *       status information the result might be not useful in all
 *       situations e.g. when it reports #NVM_REQ_BLOCK_SKIPPED.
 *
 * @param block_id  			The block identifier.
 * @param[out] req_result_ptr   Pointer to where to store the request result.
 */
extern void NvM_GetErrorStatus(NvM_BlockIdType block_id, NvM_RequestResultType *req_result_ptr);


/** Service for setting the RAM block status of a NVRAM block.
 *
 * The function NvM_SetRamBlockStatus() does only work on NVRAM
 * blocks with a permanently configured RAM block and does have no effect to other
 * NVRAM blocks.
 *
 * If the “block_changed” parameter passed to the function
 * NvM_SetRamBlockStatus() is FALSE the corresponding RAM block is set to
 * unchanged.
 *
 * If the “block_changed” parameter passed to the function
 * NvM_SetRamBlockStatus() is TRUE, the corresponding permanent RAM block is
 * set to changed.
 *
 * A changed RAM block will be written to NV storage once the write is
 * requested by the controlling software (e.g. EcuM).
 *
 * @param block_id      The block identifier uniquely identifies one NVRAM block descriptor.
 *                      A NVRAM block descriptor contains all needed information about a
 *                      single NVRAM block.
 * @param block_changed TRUE:  Mark block as changed. \n
 *                      FALSE: Mark block as unchanged.
 */
extern void NvM_SetRamBlockStatus(NvM_BlockIdType block_id, boolean block_changed);


/** Service to cancel a running NvM_WriteAll() request.
 *
 * The function NvM_CancelWriteAll() cancels a running
 * NvM_WriteAll() request.
 */
extern void NvM_CancelWriteAll(void);


/** Initiates a multi block read request.
 *
 * The multi block service NvM_ReadAll() provides two distinct
 * functionalities.
 * -# Initialize the management data for all NVRAM blocks
 * -# Copy data to the permanent RAM blocks for those NVRAM blocks which are
 *    configured accordingly
 *
 * The function NvM_ReadAll() signals the request to the NvM module
 * and returns.
 *
 * The job of the function NvM_ReadAll() iterates over all user NVRAM
 * blocks, i.e. except for reserved Block Ids 0 (multi block request result) and 1 (NV
 * configuration ID), beginning with the lowest Block Id.
 *
 * The job of the function NvM_ReadAll() sets the job result to
 * NVM_REQ_BLOCK_SKIPPED for all NVRAM blocks which are not loaded
 * automatically during processing of the NvM_ReadAll() job since
 * the RAM block is detected as valid.
 *
 * The job of the function NvM_ReadAll() loads the ROM default data to
 * the corresponding RAM blocks and set the error/status field in the administrative
 * block to NVM_REQ_OK when processing the extended runtime preparation.
 */
extern void NvM_ReadAll(void);


/** Initiates a multi block write request.
 *
 * The job of the function NvM_WriteAll() synchronizes the contents of
 * the permanent RAM blocks to their corresponding NV blocks.
 * It processes all permanent RAM blocks except for block ID 0.
 *
 * The function NvM_WriteAll() signals the request to the NvM module
 * and returns.
 *
 * The job of the function NvM_WriteAll() sets the job result for each
 * NVRAM block which has not been written automatically by the job to
 * NVM_REQ_BLOCK_SKIPPED.
 */
extern void NvM_WriteAll(void);


extern Std_ReturnType NvM_ReadBlock(NvM_BlockIdType block_id, uint8 *dst_ptr);

extern Std_ReturnType NvM_WriteBlock(NvM_BlockIdType block_id, uint8 *src_ptr);

extern Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType block_id, uint8 *dst_ptr);


/** Service for performing the processing of the NvM jobs.
 *
 * The function NvM_MainFunction shall perform the processing of the
 * NvM module jobs.
 */
extern void NvM_MainFunction(void);


/** The function NvM_GetVersionInfo returns the version information of the NvM module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function NvM_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter NvMVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void NvM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = NVM_MODULE_ID;
	versioninfo->sw_major_version = NVM_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = NVM_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = NVM_SW_PATCH_VERSION;
	versioninfo->vendorID = NVM_VENDOR_ID;
}

/*@}*/

#endif /* NVM_H_ */
