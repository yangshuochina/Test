/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: RamTst.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:17:53MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: RamTst.h  $
 *  Revision 1.4 2011/02/02 09:17:53MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/11/22 08:35:17MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:29:43MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/


#ifndef RAMTST_H_
#define RAMTST_H_

#include "team-s.h"
#include "Std_Types.h"
#include "RamTst_Cfg.h"

/** @defgroup group_ram RamTst
 *  @ingroup group_ramtst */
/*@{*/

#define RAMTST_MODULE_ID          (93U)
#define RAMTST_VENDOR_ID          (TEAMS_VENDOR_ID)
#define RAMTST_AR_MAJOR_VERSION   (1U)
#define RAMTST_AR_MINOR_VERSION   (1U)
#define RAMTST_AR_PATCH_VERSION   (1U)
#define RAMTST_SW_MAJOR_VERSION   (1U)
#define RAMTST_SW_MINOR_VERSION   (0U)
#define RAMTST_SW_PATCH_VERSION   (3U)

/* Driver APIs Service ID */
#define RAMTST_INIT_API_ID                          ((uint8)(0x0))
#define RAMTST_MAINFUNCTION_API_ID                  ((uint8)(0x1))
#define RAMTST_STOP_API_ID                          ((uint8)(0x2))
#define RAMTST_ALLOW_API_ID                         ((uint8)(0x3))
#define RAMTST_GETEXECUTIONSTATUS_API_ID            ((uint8)(0x4))
#define RAMTST_GETTESTRESULT_API_ID                 ((uint8)(0x5))
#define RAMTST_GETTESTRESULTPERBLOCK_API_ID         ((uint8)(0x6))
#define RAMTST_GETTESTALGORITHM_API_ID              ((uint8)(0x7))
#define RAMTST_CHANGENUMBEROFTESTEDCELLS_API_ID     ((uint8)(0x8))
#define RAMTST_CHANGEMAXNUMBEROFTESTEDCELLS_API_ID  ((uint8)(0x8)) /*both has same ID*/
#define RAMTST_GETNUMBEROFTESTEDCELLS_API_ID        ((uint8)(0x9))
#define RAMTST_GETVERSIONINFO_API_ID                ((uint8)(0xA))
#define RAMTST_CHANGETESTALGORITHM_API_ID           ((uint8)(0xB))
#define RAMTST_DEINIT_API_ID                        ((uint8)(0xC))
#define RAMTST_SUSPEND_API_ID                       ((uint8)(0xD))
#define RAMTST_RESUME_API_ID                        ((uint8)(0xE))


/* Development Error Codes */
#define RAMTST_E_STATUS_FAILURE         ((uint8)(0x01)) /**< Failure within RAM test execution status */
#define RAMTST_E_OUT_OF_RANGE           ((uint8)(0x02)) /**< parameter out of specified range */
#define RAMTST_E_UNINIT                 ((uint8)(0x03)) /**< Service used with module Initialization */


/** @page Ramtst RamTst
 *
 * @section RamTst_intro Introduction
 * The RamTst function implements the RamTst functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_RAM_Test.pdf">AUTOSAR_SWS_RAM_Test</a>.
 *
 *
 * @section ramtst_init Initialization of RamTst
 * Before any service of the RamTst driver can be used, the initialization function RamTst_Init()
 * must to be called which uses the configuration set (#RamTst_ConfigType) to initialize the driver.
 *
 *
 * @section ramtst_autosar AutoSAR Conformity
 * @li RamTst implements all API services defined by AutoSAR. In addition it implements the FullTest which was
 * 	   not part of the API list is also implemented which can be used for Initial(ForeGround) testing of ram.
 * @li The ForeGround test is implemented independent of Scheduler test function(RamTst_MainFunction).
 * @li The ForeGround test or RamTst_FullTest is not protected against any interrupts as the intension
 *     of providing this interface is to test the ram completely before Interrupt activitation. If
 *     interrupt protection is necessary, user shall call this function under interrupt disabled.
 * @li The interface "RamTst_ChangeMaxNumberOfTestedCells" is not implemented due to below reasons.
 * 		- 1. It is explained in the SWS that Max or Min configured by the integrator to a ideal value and expected
 * 			be an macro.
 * 		- 2. The API ID is same as "RamTst_ChangeNumberOfTestedCells".
 *		- 3. It is expected that this "number of cells to be tested" is usually static value, so maximum value
 *		   can be easily found and configured statically.
 * @li TranspGalpat & AbrahamTest Algorithm is not implemented due to non available info.
 *
 *
 *
 * @section ramtest_operation Operation
 * @subsection ramtst_algorithm RAMTST Algorithm
 * The Ram regions are tested using the configured default algorithm. The default algorithm
 * can be changed later using the provided API. There are six different algorithm requested
 * by the Autosar Specification, but only four of them are available in this implementation.
 * For each algorithm, the parameters has to be configured.
 *
 * @subsection march_test March Test
 * March test is nothing but march from low address to high address and march
 * back from hight to low address with a PATTERN and complement of the
 * PATTERN respectively. The test has 4 steps:
 *
 * - Step 1: <B> INITIALIZE: </B>
 *          @li Write a TESTPATTERN in all memory locations in the test area.
 * - Step 2: <B> MARCHING ONE: </B> Repeat the following steps starting from \n
 *           the lowest address until the highest address is reached.
 *          @li Read the address and check if the content of the memory \n
 *             is  TESTPATTERN (If not, store the address of faulty location).
 *          @li Invert the content at address to write COMPL_TESTPATTERN.
 *          @li Read the memory location to confirm that the content at \n
 *              address has been inverted successfully. (If not, store the\n
 *              address of faulty location).
 * - Step 3: <B> MARCHING ZEROS: </B> Repeat the following steps starting from the \n
 *          highest address until the lowest address is reached.
 *          @li Read the address and check if the content of the memory is \n
 *              COMPL_TESTPATTERN  (If not, store the address of faulty location).
 *          @li Invert the content at address to write TESTPATTERN.
 *          @li Read the memory location to confirm that the content at address \n
 *              has been inverted successfully. (If not, store the address of faulty location).
 * -  Step 4: <B> REPEAT: </B> Repeat the above two passes over the test area with new pattern
 *
 *
 * @subsection Galpat Galpat Test
 * GALPAT is known as Galloping patterns.
 * The galloping patterns test is a strong test for most faults.
 * it is a complete test to detect all SAFs, TFs and CFs.
 * In Galloping algorithm (GALPAT), there are two types of memory cells:
 * base cell and local cells which are the remaining cells of the test
 * area. The concept is that when the value in the base cell is changed,
 * all other memory cells will be read to verify if they
 * have been affected by the write operation to the base cell,
 * and the base cell will be read after each of the read operations
 * to each other local cell.
 * - Step 1: <B> INITIALIZE: </B> Write a TESTPATTERN in all memory locations in the test area.
 * - Step 2: <B> PERFORM TEST: </B> Repeat the following steps starting from the
 *              lowest address until the highest address is reached, the lowest
 *              address is the test cell at start.
 *              -# Phase 1 :
 *              @li Invert  bit 1 of the contents of the test cell (from lsb to msb).
 *              @li Test that untested addresses in area contain test pattern.
 *              @li Test that test address contains correctly inverted result.
 *              @li Test that previously tested addresses contain inverted test pattern.
 *              @li Between each test, go back and test the test cell.
 *              @li Repeat until all 8 bits at address have been inverted.
 *              -# Phase 2: Now the next cell in the address area becomes \n
 *              the test cell then repeat phase 1 again.
 * - Step 3: <B> REPEAT: </B> Repeat the above pass over the test area with new pattern.
 *
 * @subsection CheckerboardTest Checker Board Test
 * The checker board test has only 2 steps.
 * - Step 1: <B> INITIALIZE: </B> Write a 0x55555555 and 0xAAAAAAAA in first and second word alternatively.
 * - Step 2: <B> PERFORM TEST: </B> Read 0x55555555 and 0xAAAAAAAA in first and second word alternatively.
 * 				Any mis-match will be treated as ram test error.
 *
 * This test normally finds data bus problems, a bad device, chip select failures,
 * or an incorrectly inserted pin will usually exhibit a simple read/write error or
 * adjacent data bus short. Writing a pattern of 55s and AAs tests the ability of the
 * devices to hold data, but it doesn't insure that the RAMs are being addressed
 * correctly. Examples of failures that could pass this simple test are: a post-buffer
 * address short, an open address line (say, from a pin not being inserted properly),
 * or chip select failures causing multiple addressing. It's important to run a second
 * routine that isolates these not-uncommon problems.
 *
 * @subsection WalkpathTest Walkpath Test
 * - Step 1: <B> INITIALIZE: </B> Initialize a test area with a pattern.
 * - Step 2: <B> PERFORM TEST: </B> Starting at lowest address and proceeding to top address below steps.
 *     @li Invert the contents at the test address.
 *     @li Check all addresses except test address contain test pattern.
 *     @li Check content at test address is inverted.
 *     @li Invert content at test address back to test pattern.
 * - Step 3: <B> REPEAT </B> invert patterns & Re-initialize the test are with inverted test pattern and do STEP 2.
 *
 * @subsection RamTstConcept RamTest Concept:
 * The RamTst_Init() only initialize the driver, tests the ram region which will be used by driver
 * and keeps the driver ready for ram testing.
 * The Ram Regions will be tested for each call of RamTst Scheduler function - RamTst_MainFunction,
 * called "BackGround Test" or at one stretch for the complete configured range using "RamTst_FullTest"
 * called "ForeGround Test".
 * In BackGround test for each call defined number of ram cells will be tested till the entire range is tested.
 * If the end of block is reached, next block will be selected from next call of main function.
 * The algorithm shall be changed using the given interface, but that specific algorithm and its
 * attributes(RamTst_AlgParamsType) must be configured.
 * The ForeGround test will be performed based on the selected ram test algorithm and its configured attributes.
 * The ram blocks configured for this algorithms will be tested. It records only the overall Ram test
 * status and not the individual blocks.
 *
 * There is separate memory section defined for the RamTst Purpose. Here the RamTst driver
 * variables and backup area to store the ram region under test are alloted(For NonDestructive test).
 *
 * If the ram test algorithm finds any error in the ram cell, then the corresponding block result
 * and overall ram test result will be updated and the error hook function will be called if configured.
 * In addition the test status will be turned as "Paused"(Suspended). The remaining locations will not be tested
 * with in this call and the ram test shall continue only it is "Resumed" again using the
 * RamTst_Resume() interface.
 *
 * If there is no error and the test completes successfully then test completed hook function
 * will be called if configured.
 *
 * All the regions must be unique, implies no overlapping in configuration. So it is advisable to
 * use the labels which always provides unique start and end address. The start address must be less
 * than the end address.
 *
 * For the stack and CSA region, only the unused portion of the region will be tested. It is assumed
 * that Stack always grow from Highest address to Lower address.
 *
 * @section RamTst_cfg Configuration
 * @subsection RamTst_cfg_step_by_step Step by Step
 * @li Configure #RamTst_BlockParamsType
 * @li Configure #RamTst_AlgParamsType
 * @li Configure #RamTst_ConfigType.
 * @li Call the function RamTst_Init() with the pointer to the configured structure #RamTst_ConfigType
 *     in the startup phase.
 *
 *
 * @subsection ramtst_cfg_by_example Example
 * Configuration of #RamTst_BlockParamsType:
 * This is the memory block configuration, each block(entry) provides start and end address of an block.
 * @code
 * const RamTst_BlockParamsType RamTstBlockCfg0[] = {
 *  {(uint32*)0xD0004000,  (uint32*)0xD0005000, 0},
 *  {(uint32*)0xD0006000,  (uint32*)0xD0008000, 1},
 *  {(uint32*)0xD0005000,  (uint32*)0xD0005800, 2}
 * };
 * const RamTst_BlockParamsType RamTstBlockCfg1[] = {
 *  {(uint32*)0xD0008000,  (uint32*)0xD0008800, 0},
 *  {(uint32*)0xD0000000,  (uint32*)0xD0002000, 1},
 * };
 * @endcode
 *
 *
 * Configuration of #RamTst_AlgParamsType:
 * This is the algorithm configuration, here it provides the attributes for each algorithm with the memory
 * blocks belongs to it.
 * @code
 * const RamTst_AlgParamsType RamTstAlgParamCfg[] = {
 *  {
 *      &RamTstBlockCfg0[0], 	Pointer to the Block configuration structure which has to be tested by this algorithm(RamTst_AlgParamsType::BlockParamCfg)
 *      RAMTST_MARCH_TEST,  	Algorithm for which parameters are to be defined(RamTst_AlgParamsType::RamTstAlgId)
 *      RAMTST_ALGORITHM_NONDESTRUCTIVE, Ram Algorithm test type(RamTst_AlgParamsType::algo_type)
 *      0x400,					Maximum number of ram test cells(in mutliple of 4bytes) shall be tested in one call(RamTst_AlgParamsType::RamTstNumberofTestedCells)
 *      0,						Extended number of ram cells that can be tested in one call at maximum(RamTst_AlgParamsType::RamTstExtNumberOfTestedCells)
 *      0x400,					Maximum limit for the "RamTstNumberofTestedCells"(RamTst_AlgParamsType::RamTstMaxNumberOfTestedCells)
 *      0x100,					Minimum limit for the "RamTstNumberofTestedCells"(RamTst_AlgParamsType::RamTstMinNumberOfTestedCells)
 *      3,						Number of blocks in this algorithm configuration(RamTst_AlgParamsType::RamTstNumberOfBlocks)
 *      2,						This is the block where the stack region comes under(RamTst_AlgParamsType::StackBlockID)
 *      0,				    	This is the CSA region configured for testing(RamTst_AlgParamsType::CSABlockID)
 *  },
 *  {
 *      &RamTstBlockCfg1[0],	Pointer to the Block configuration structure which has to be tested by this algorithm(RamTst_AlgParamsType::BlockParamCfg)
 *      RAMTST_GALPAT_TEST, 	Algorithm for which parameters are to be defined(RamTst_AlgParamsType::RamTstAlgId)
 *      RAMTST_ALGORITHM_DESTRUCTIVE, Ram Algorithm test type(RamTst_AlgParamsType::algo_type)
 *      0x100,					Maximum number of ram test cells(in multiple of 4bytes) shall be tested in one call(RamTst_AlgParamsType::RamTstNumberofTestedCells)
 *      0,						Extended number of ram cells that can be tested in one call at maximum(RamTst_AlgParamsType::RamTstExtNumberOfTestedCells)
 *      0x400,					Maximum limit for the "RamTstNumberofTestedCells"(RamTst_AlgParamsType::RamTstMaxNumberOfTestedCells)
 *      0x0,					Minimum limit for the "RamTstNumberofTestedCells"(RamTst_AlgParamsType::RamTstMinNumberOfTestedCells)
 *      2,						Number of blocks in this algorithm configuration(RamTst_AlgParamsType::RamTstNumberOfBlocks)
 *      0xFF,					There is no stack region configured for testing(RamTst_AlgParamsType::StackBlockID)
 *      0xFF,					There is no CSA region configured for testing(RamTst_AlgParamsType::CSABlockID)
 *  }
 * };
 * @endcode
 *
 * Configuration of RamTst_ConfigType:
 * This is the main ram test configuration through which all the necessary elements are accessed from
 * algorithm and block configuration structure.
 * @code
 * const RamTst_ConfigType RamTstConfig[] = {
 *  &RamTstAlgParamCfg[0],			Pointer to the algorithm configuration structure(RamTst_ConfigType::AlgParamCfg)
 *  RamTstComplete,					Ram test completion notification function(RamTst_ConfigType::RamTstCompleteNotify)
 *  RamTstError,					Ram test failure notification function(RamTst_ConfigType::RamTstErrorNotify)
 *  NULL,							Watchdog service function(RamTst_ConfigType::RamTstWdgHook)
 *  RAMTST_MARCH_TEST,				Default algorithm to be used after init(RamTst_ConfigType::RamTstDefaultAlgParamsId)
 *  sizeof(RamTstAlgParamCfg)/sizeof(RamTstAlgParamCfg[0]), Number of Algorithm configuration(RamTst_ConfigType::RamTstNumberofAlgs)
 * };
 * @endcode
 *
 * Usage of the above:
 * The RamTst_Init only initialize the driver and the ram test must be done via the RamTst_MainFunction().
 * It is advisable to perform one complete ram test(using RamTst_FullTest) after Init before go to scheduler.
 *
 * @code
 * void init(void)
 * {
 *   ...
 *   RamTst_Init();
 *   RamTst_FullTest();
 *   ...
 * }
 * @endcode
 *
 */

/** This enumeration is the definition for the RamTest Algorithms.
 *
 */
typedef enum
{
    RAMTST_ALGORITHM_UNDEFINED = 0u,	/**< No Specific algorithm.*/
    #if defined (RAMTST_CHECKERBOARD_TEST_SELECTED) && (RAMTST_CHECKERBOARD_TEST_SELECTED == STD_ON)
        RAMTST_CHECKERBOARD_TEST,		/**< Check Board algorithm is selected. */
    #endif
    #if defined (RAMTST_MARCH_TEST_SELECTED) && (RAMTST_MARCH_TEST_SELECTED == STD_ON)
        RAMTST_MARCH_TEST,				/**< March test algorithm is selected. */
    #endif
    #if defined (RAMTST_WALK_PATH_TEST_SELECTED) &&  (RAMTST_WALK_PATH_TEST_SELECTED == STD_ON)
        RAMTST_WALK_PATH_TEST,			/**< Walk path algorithm is selected. */
    #endif
    #if  defined (RAMTST_GALPAT_TEST_SELECTED) && (RAMTST_GALPAT_TEST_SELECTED == STD_ON)
        RAMTST_GALPAT_TEST,				/**< GalPat test algorithm is selected. */
    #endif
    #if  defined (RAMTST_TRANSP_GALPAT_TEST_SELECTED) && (RAMTST_TRANSP_GALPAT_TEST_SELECTED == STD_ON)
        RAMTST_TRANSP_GALPAT_TEST,		/**< Transparent GalPat test algorithm is selected. */
    #endif
    #if  defined (RAMTST_ABRAHAM_TEST_SELECTED) && (RAMTST_ABRAHAM_TEST_SELECTED == STD_ON)
        RAMTST_ABRAHAM_TEST				/**< Abraham test algorithm is selected. */
    #endif
}RamTst_AlgorithmType;


/** This enumeration is the definition for the ram test execution status.
 *
 */
typedef enum
{
        RAMTST_EXECUTION_STATE_UNDEFINED, 	/**< RamTst driver is in undefined state(before Init).*/
        RAMTST_EXECUTION_UNINIT,			/**< RamTst driver is in Uninitialized state(after DeInit).*/
        RAMTST_EXECUTION_INIT,				/**< RamTst driver is initialized(after Init). */
        RAMTST_EXECUTION_RUNNING,			/**< RamTst is going-on, ram regions are under test.*/
        RAMTST_EXECUTION_STOPPED,			/**< RamTst execution is stopped. */
        RAMTST_EXECUTION_SUSPENDED, 		/**< RamTst execution is suspended or paused. */
        RAMTST_EXECUTION_ALLOWED			/**< RamTst execution is allowed(resumed).*/
}RamTst_ExecutionStatusType;


/** This enumeration is the definition for the ram test result.
 *
 */
typedef enum
{
        RAMTST_RESULT_NOT_TESTED,			/**< RamTest is not yet performed. */
        RAMTST_RESULT_OK,					/**< RamTest is performed and region is found OK.*/
        RAMTST_RESULT_NOT_OK,				/**< RamTest is performed and region is found with defect.*/
        RAMTST_RESULT_UNDEFINED,			/**< RamTest is not one of above three defined state.*/
}RamTst_TestResultType;


/** This enumeration is the definition for the ram algorithm test type.
 *
 */
typedef enum
{
        RAMTST_ALGORITHM_NONDESTRUCTIVE,	/**< RamTest is performed in non-destructive way. */
        RAMTST_ALGORITHM_DESTRUCTIVE,		/**< RamTest is performed in destructive way. */
}RamTst_AlgoTestType;

/** This is the type for the "number of cells to be tested" which is used in algorithm parameter configuration
 *  and in the interface. In each call of RamTst_MainFunction, this many number of cells will be tested. */
typedef uint16 RamTst_NumberOfTestedCellsType;


/** This is the type for the "total number of blocks" configured for an algorithm. */
typedef uint8 RamTst_NumberOfBlocksType;

/** This is the configuration structure for the ram test address block.
 *
 * This configuration structure contains the Ram block start and end address, where the
 * end address is exclusive of the test range.
 */
typedef struct
{
    uint32 *RamTstStartAddress;                 /**< Block's Start address - Inclusive of ram test.*/
    uint32 *RamTstEndAddress;                   /**< Block's End address   - Inclusive of ram test.*/
    RamTst_NumberOfBlocksType RamTstBlockId;    /**< Address Block IDs.*/
}RamTst_BlockParamsType;


/** This is the configuration structure for each algorithm specific parameters.
 *
 * This configuration structure contains the algorithm specific configuration like
 * number of ram cells to be tested, the maximum and minimum to be tested. Furthur
 * which are the address blocks will be covered by that algorithm.
 */
typedef struct
{
	const RamTst_BlockParamsType *BlockParamCfg;                /**< Pointer to the block configuration structure.*/
	RamTst_AlgorithmType RamTstAlgId;                           /**< RamTest Algorithm for which attributes are configured.*/
    RamTst_AlgoTestType algo_type;                              /**< algorithm test type - destructive or non-destructive.*/
    RamTst_NumberOfTestedCellsType RamTstNumberofTestedCells;   /**< Number of ram cells to be tested in each call.*/
    uint16 RamTstExtNumberOfTestedCells;                        /**< Extended number of ram cells can be tested in each call.*/
    uint16 RamTstMaxNumberOfTestedCells;                        /**< Maximum number of ram cells to be tested in each call.*/
    uint16 RamTstMinNumberOffTestedCells;                       /**< Minimum number of ram cells to be tested in each call.*/
    uint8  RamTstNumberOfBlocks;                                /**< Number of address blocks configured for this algorithm.*/
    uint8  StackBlockID;										/**< Block ID(corresponding address range configured in the RamTst_BlockParamsType) refers the stack range. */
    uint8  CSABlockID;											/**< Block ID(corresponding address range configured in the RamTst_BlockParamsType) refers the CSA region. */
}RamTst_AlgParamsType;


/** This is the configurations structure for the ramtest driver.
 *
 * This configuration structure will be used inside the ram test driver functionality
 * to test the configured ram blocks.
 */
typedef struct
{
	const RamTst_AlgParamsType *AlgParamCfg;  		/**< Pointer to the algorithm parameter configuration.*/
    void (*RamTstCompleteNotify)(void);         /**< Ram test complete notification function.*/
    void (*RamTstErrorNotify)(void);            /**< Ram test error notification function.*/
    void (*RamTstWdgHook)(void);				/**< Hook to Watchdog service function. */
    RamTst_AlgorithmType RamTstDefaultAlgParamsId;     /**< default algorithm to be used after init.*/
	uint8 RamTstNumberofAlgs;           		/**< total number of algorithms configured.*/
}RamTst_ConfigType;


/** This service initializes the RamTst driver.
 *
 * The function RamTst_Init initializes the RamTst module. It doesnt start the ram test functionality.
 * The testing of the ram regions shall be done by calling the RamTst_MainFunction.
 * The Init functions tests the special ram region which is used for the ram test driver to store
 * the ram test driver variables and the back up area to store the ram region under test.
 *
 * @note After the execution of the function RamTst_Init(), the configuration data are accessible
 * and can be used by the RamTst module functions.
 */
extern void RamTst_Init(void);


/** This service terminates the services of the RamTst driver.
 *
 * After this service is called, no other ram test service must be called without calling RamTst_init.
 *
 */
extern void RamTst_DeInit(void);


/** This service stops the performance of ram test via RamTst_MainFunction.
 *
 * All the internal ram test status are lost and the ram test can be started again only after
 * the call of Allow service.
 */
extern void RamTst_Stop(void);


/** This service Allows the performance of ram test for the next occurrence of RamTst_MainFunction.
 *
 *  After this call, the ram test starts from the beginning using the default ram test algorithm.
 */
extern void RamTst_Allow(void);


/** This service Suspends the performance of ram test from the next call of RamTst_MainFunction.
 *
 *  After the call, the ram test status are hold but no test performed till it is allowed.
 */
extern void RamTst_Suspend(void);


/** This service resumes the performance of ram test for the next occurrence of RamTst_MainFunction.
 *
 *  After the call, the ram test starts from the place where it was left earlier.
 */
extern void RamTst_Resume(void);


/** This service returns the ram test execution status.
 *
 * It returns the either one of the six status currently it is in.
 *
 */
extern RamTst_ExecutionStatusType RamTst_GetExecutionStatus(void);


/** This service return the overall ram test result.
 *
 * It returns the test result about the complete ram region.
 */
extern RamTst_TestResultType RamTst_GetTestResult(void);


/** This service returns each block's ram test result.
 *
 * It returns the test result for the passed address block ID.
 *
 * @param BlockID    The Block for which the test status must be returned.
 */
extern RamTst_TestResultType RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID);


/** This service returns the current test algorithm.
 *
 * It returns the test algorithm which is used to test the ram regions.
 */
extern RamTst_AlgorithmType RamTst_GetTestAlgorithm(void);


/** This service returns the number of cells to be tested in each call.
 *
 * It returns the number of ram cells to be tested for each call of RamTst_MainFunction().
 */
extern RamTst_NumberOfTestedCellsType RamTst_GetNumberOfTestedCells(void);


/** This service sets the new algorithm to be used for ram test.
 *
 * It changes the current used algorithm with passed one. So the ram test starts
 * freshly again from the beginning as per the configuration after the current region is tested.
 *
 * @param NewTestAlgorithm    New algorithm to be used.
 */
extern void RamTst_ChangeTestAlgorithm(RamTst_AlgorithmType NewTestAlgorithm);


/** This service changes the number of cells to be tested in each test call.
 *
 * It sets the new value for the number of cells to be tested.
 *
 * @param NewNumberOfTestedCells    Number of ram cells to be tested.
 */
extern void RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells);


/** This service changes the maximum number of cells to be tested in each test call.
 *
 * It sets the new value for the maximum number of cells to be tested.
 *
 * @param NewNumberOfTestedCells    Maximum Number of ram cells to be tested.
 */
extern void RamTst_ChangeMaxNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells);


/** This service tests the "NumberOfTestedCells" of ram region in each call.
 *
 * It tests the defined number of cells in each call untill all the ram regions are tested.
 * It restarts the test once again after the last region is tested.
 */
extern void RamTst_MainFunction(void);


/** This service tests the all the blocks configured for an algorithm.
 *
 * Usually this interface is called after RamTst_Init, but before control reaches scheduler to test
 * all the configured ram regions. The Autosar spec this test must realized via the same schduler function
 * RamTst_Function. But due to easier and flexible configuration, this is implemented with a special
 * Functionality.
 */

extern void RamTst_FullTest(void);


/** The function RamTst_GetVersionInfo returns the version information of the RamTst module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function RamTst_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter RamTstVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void RamTst_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    versioninfo->moduleID = RAMTST_MODULE_ID;
    versioninfo->sw_major_version = RAMTST_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = RAMTST_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = RAMTST_SW_PATCH_VERSION;
    versioninfo->vendorID = RAMTST_VENDOR_ID;
}

/*@}*/

#endif /* RAMTST_H_ */
