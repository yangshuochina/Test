/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: RamTst.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:29:05MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: RamTst.c  $
 *  Revision 1.2 2010/07/12 14:29:05MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "RamTst.h"
#include "RamTst_Priv.h"
#include "Os.h"
#include "Dem.h"
#include "SchM_RamTst.h"

#define RAMTST_RSTSAFE_START_SEC_VAR_32BIT
#include "MemMap.h"

RamTst_DataStruct RamTst_Info;

#define RAMTST_RSTSAFE_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define RAMTST_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const RamTst_ConfigType RamTstConfig[];
extern const RamTstAlgorithmType ramtst_algo[];

#define RAMTST_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

#define RAMTST_MARCHTEST_PATTERN 0x00000000
#define RAMTST_GALPATEST_PATTERN 0x00000000
#define RAMTST_WALKPATH_PATTERN  0x00000000

#define RAMTST_START_SEC_CODE
#include "MemMap.h"

#if defined (RAMTST_MARCH_TEST_SELECTED) && (RAMTST_MARCH_TEST_SELECTED == STD_ON)

uint32* RamTst_Algo_MarchTest(uint32* low_add, uint32 length )
{
    volatile uint32* add;
    uint32 tst_pat,tst_pat_compl,cyc_cnt,size;

    size = length - 1;
    cyc_cnt=0;
    tst_pat = RAMTST_MARCHTEST_PATTERN;
    tst_pat_compl = ~tst_pat;
    do
    {
        cyc_cnt++;
        /* initialize the area to 0 */
        for(add = low_add; add <= (low_add+size); add++ )
        {
            *add = tst_pat;
        }
        /* STEP2: Marching 1*/
        for (add = low_add; add <= (low_add+size); add++)
        {
            if ((*add) == tst_pat)
            {
                *add =tst_pat_compl;
                if ((*add) !=tst_pat_compl)
                {
                    return (uint32*)add;
                }
            }
            else
            {
            	return (uint32*)add;
            }
        }

        tst_pat = tst_pat_compl;
        tst_pat_compl = ~tst_pat;
        for (add = (low_add+size); add >= low_add; add--)
        {
            if ((*add) == tst_pat)
            {
                *add =tst_pat_compl;
                if (*add !=tst_pat_compl)
                {
                    return (uint32*)add;
                }
            }
            else
            {
            	return (uint32*)add;
            }
        }
        if (cyc_cnt<2)
        {
            /* complement test patterns */
            tst_pat = ~(RAMTST_MARCHTEST_PATTERN);
            tst_pat_compl = ~tst_pat;
            /* do STEP1*/
        }
    }while(cyc_cnt<2);
    return NULL_PTR; /* test succeeded */
}

#endif


#if  defined (RAMTST_GALPAT_TEST_SELECTED) && (RAMTST_GALPAT_TEST_SELECTED == STD_ON)

uint32* RamTst_Algo_GalpatTest(uint32* low_add, uint32 length)
{
    volatile uint32* add;
    volatile uint32* add_t;

    uint32 tst_pat,tst_pat_compl,compare_val,size;
    uint8 i,cyc_cnt;

    size = length - 1;
    cyc_cnt=0;

    tst_pat = RAMTST_GALPATEST_PATTERN;
    tst_pat_compl = ~tst_pat;

    /* STEP1. Initialize all memory locations in the
     * test area with a TESTPATTERN.
     */
    for(add = low_add; add <= (low_add+size); add++ )
    {
        *add = tst_pat;
     }

    /* STEP2: Perform test.
     * Repeat the following steps starting from the lowest
     * address until the highest address is reached,
     * the lowest address is the test cell at start.
     */
    do
    {
        cyc_cnt++;
        for (add = low_add; add <= (low_add+size); add++)
        {
            for (i=0;i<=31;i++ )
            {
                /* complement one bit of the test cell bit
                 * one after other from lsb to msb.
                 */
                compare_val = (*add ^ (1<<i));
                *add = compare_val;
                /* check if any memory location of the test area
                 * is affected by the write operation to the testbyte
                 */
                for (add_t = (volatile uint32*)low_add; add_t <= (volatile uint32*)(low_add+size); add_t++)
                {
                    /* check non-test bytes (above/below test byte)*/
                    if (add_t < add)
                    {
                        if ((*add_t) != tst_pat_compl)
                        {
                            return (uint32*)add_t;
                        }
                    }
                    else if (add_t > add)
                    {
                        if ((*add_t) != tst_pat)
                        {
                            return (uint32*)add_t;
                        }
                    }
                    /* check test byte has the inverted pattern */
                    if (*add != compare_val)
                    {
                        return (uint32*)add;
                    }
                }/* check all locations of test area after bit change*/
            } /* do it for all bits of the byte */
        }/* do it for all bytes */

        /*
         * STEP 3: Repeat
         * Repeat the above pass over the test area with new pattern
         */
        if (cyc_cnt<2)
        {
            /* complement test patterns */
            tst_pat = ~(RAMTST_GALPATEST_PATTERN);
            tst_pat_compl = ~tst_pat;
            /* do STEP1*/
            for(add = low_add; add <= (low_add+size); add++ )
            {
                *add = tst_pat;
            }
        }

    } while (cyc_cnt<2);
    return NULL_PTR; /* test succeeded */
}

#endif


#if defined (RAMTST_CHECKERBOARD_TEST_SELECTED) && (RAMTST_CHECKERBOARD_TEST_SELECTED == STD_ON)

uint32* RamTst_Algo_CheckerboardTest(uint32* low_add, uint32 length )
{
	uint32 i,size;
	volatile uint32* add;
	const uint32 pat[]={0x55555555U, 0xAAAAAAAAU};

	size = length-1;
	add = low_add;
	for(i = 0; i <= size; i++)
	{
		*add = pat[i&0x1];
		add++;
	}
	add = low_add;

	for(i = 0; i <= size; i++)
	{
		if ((*add) != pat[i&0x1])
		{
			return (uint32*)add;
		}
		add++;
	}
	return NULL_PTR; /* test succeeded */
}

#endif


#if defined (RAMTST_WALK_PATH_TEST_SELECTED) &&  (RAMTST_WALK_PATH_TEST_SELECTED == STD_ON)

uint32* RamTst_Algo_WalkpathTest(uint32* low_add, uint32 length )
{
	uint32 tst_pat,tst_pat_compl,size;

	volatile uint32* add;
    volatile uint32* add_test;

    uint8 cyc_cnt;

    size = length - 1;
    cyc_cnt=0;

    tst_pat = RAMTST_WALKPATH_PATTERN;
    tst_pat_compl = ~tst_pat;

    do
    {
    	cyc_cnt++;
    	/* STEP1. Initialize all memory locations in the
    	 * test area with a TESTPATTERN.
    	 */
    	 for(add_test = low_add; add_test <= (low_add+size); add_test++ )
    	 {
    		 *add_test = tst_pat;
    	 }
    	 /* STEP2:
    	  * Starting at lowest address and proceeding to top address:
    	  * do the sub steps a,b&c
    	  */
    	 for(add_test = low_add; add_test <= (low_add+size); add_test++ )
    	 {
    		/* STEP2.a Invert the contents at the test address.*/
    		*add_test =  tst_pat_compl;
    		/* STEP2.b Check all addresses except test address contain test pattern.*/
    		for(add = low_add; add <= (low_add+size); add++ )
    		{
    			if(add == add_test)
    			{
    				/* skip reading test address*/
    			}
    			else
    			{
    				if ((*add) != tst_pat)
    				{
    					return (uint32*)add;
    				}
    			}
    		}
    		/* STEP2.c Check content at test address is inverted */
    		if (*add_test != tst_pat_compl)
    		{
				return (uint32*)add_test;
    		}
    		else
    		{
    			/* Invert content at test address back to test pattern. */
    			*add_test = tst_pat;
    		}
    	 }
    	 /* STEP3:
    	  *  Invert the test pattern and do STEP1 & STEP2
    	  */
    	 if (cyc_cnt<2)
    	 {
    		 /* complement test patterns */
    	     tst_pat = ~(RAMTST_WALKPATH_PATTERN);
    	     tst_pat_compl = ~tst_pat;
    	 }
    } while (cyc_cnt<2);
    return NULL_PTR; /* test succeeded */
}

#endif


#if  defined (RAMTST_TRANSP_GALPAT_TEST_SELECTED) && (RAMTST_TRANSP_GALPAT_TEST_SELECTED == STD_ON)

uint32* RamTst_Algo_TranspGalpatTest(uint32* low_add, uint32 size )
{
	/* Not yet implemented.  */
    return NULL_PTR; /* test succeeded */
}

#endif


#if  defined (RAMTST_ABRAHAM_TEST_SELECTED) && (RAMTST_ABRAHAM_TEST_SELECTED == STD_ON)

uint32* RamTst_Algo_AbrahamTest(uint32* low_add, uint32 size)
{
	/* Not yet implemented.  */
    return NULL_PTR; /* test succeeded */
}

#endif

inline void RamTst_StoreData(const uint32* start_address, uint32 copy_length)
{
    int i;
    for(i=0;i<copy_length;i++){
        RamTst_Info.backup_area[i] = *(start_address + i);
    }
}

inline void RamTst_ReStoreData(uint32* start_address, uint32 copy_length)
{
    int i;
    for(i=0;i<copy_length;i++){
        *(start_address + i) = RamTst_Info.backup_area[i];
    }
}

inline uint32 * RamTst_GetStackPointer(void)
{
	uint32 *stack_ptr;
	__asm("mov.aa %0,a10":"=a"(stack_ptr));
	return((uint32*)stack_ptr);
}

inline uint8 RamTst_GetCSA(void)
{
	uint32 pcx;
	pcx = (uint32)__mfcr(PCXI);
	return (uint8)(pcx & 0xFF);
}

void RamTst_Init(void)
{
    int i;
    uint32 * start_address;
    uint32 * error_address=NULL;
    const RamTst_AlgParamsType *algo_cfg;
    uint8 length, algo, algo_detected=1;


    /* First test the ram data structure and the back up area.*/
    start_address = (uint32 *)&RamTst_Info;
    length = (uint8)sizeof(RamTst_Info);

    for(i=0; ((i<RamTstConfig->RamTstNumberofAlgs) && algo_detected); i++)
     {
     	if(RamTstConfig->RamTstDefaultAlgParamsId == RamTstConfig->AlgParamCfg[i].RamTstAlgId)
     	{
     		RamTst_Info.algo_idx = (uint8)i /*  index to access the algo config structure for corresponding algorithm */;
     		algo_detected = 0; /* Exit the loop*/
     	}
     }

    algo = RamTst_Info.algo_idx;
    error_address = ramtst_algo[RamTstConfig->RamTstDefaultAlgParamsId].ramtst_algofunc(start_address, length);
    if(error_address != NULL)
    {
    	RamTst_Info.Result = RAMTST_RESULT_NOT_OK;
    	if(RamTstConfig->RamTstErrorNotify)
    	{
    		RamTstConfig->RamTstErrorNotify();
    	}
    }
    else
    {
    	RamTst_Info.Result = RAMTST_RESULT_NOT_TESTED;
    }

    for(i=0;i<RAMTST_NUMBER_OF_BLOCKS;i++)
    {
        RamTst_Info.RamTst_BlockResults[i] = RAMTST_RESULT_NOT_TESTED;
    }

    algo_cfg = (const RamTst_AlgParamsType *)&RamTstConfig->AlgParamCfg[algo];
    RamTst_Info.Algorithm = RamTstConfig->RamTstDefaultAlgParamsId;
   	RamTst_Info.RamTstNumberOfTestedCells = algo_cfg->RamTstNumberofTestedCells;
    RamTst_Info.test_address = algo_cfg->BlockParamCfg[0].RamTstStartAddress;
    RamTst_Info.block_id = 0;
    RamTst_Info.ExecutionState = RAMTST_EXECUTION_INIT;
}

void RamTst_DeInit(void)
{
    uint8 i;
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_DEINIT_API_ID, RAMTST_E_UNINIT);
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_UNINIT), RAMTST_DEINIT_API_ID, RAMTST_E_UNINIT);
    RamTst_Info.ExecutionState = RAMTST_EXECUTION_UNINIT;
    RamTst_Info.Result = RAMTST_RESULT_NOT_TESTED;
    for(i=0;i<RAMTST_NUMBER_OF_BLOCKS;i++)
    {
        RamTst_Info.RamTst_BlockResults[i] = RAMTST_RESULT_NOT_TESTED;
    }
}


#if defined(RAMTST_STOP_API_ID) && (RAMTST_STOP_API == STD_ON)

void RamTst_Stop(void)
{
	int i;
	uint8 algo_detected = 1;
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_STOP_API_ID, RAMTST_E_UNINIT);
    RAMTST_ASSERT_RETVOID(((RamTst_Info.ExecutionState == RAMTST_EXECUTION_RUNNING) || (RamTst_Info.ExecutionState == RAMTST_EXECUTION_SUSPENDED)), RAMTST_STOP_API_ID, RAMTST_E_STATUS_FAILURE);

    RamTst_Info.ExecutionState = RAMTST_EXECUTION_STOPPED;

    for(i=0; ((i<RamTstConfig->RamTstNumberofAlgs) && algo_detected); i++)
     {
     	if(RamTstConfig->RamTstDefaultAlgParamsId == RamTstConfig->AlgParamCfg[i].RamTstAlgId)
     	{
     		RamTst_Info.algo_idx = (uint8)i /* index to access the algo config structure for corresponding algorithm */;
     		algo_detected = 0; /* Exit the loop*/
     	}
     }

    RamTst_Info.Algorithm = RamTstConfig->RamTstDefaultAlgParamsId;
    RamTst_Info.block_id = 0;
    RamTst_Info.test_address = RamTstConfig->AlgParamCfg[RamTstConfig->RamTstDefaultAlgParamsId-1].BlockParamCfg[0].RamTstStartAddress;
}

#endif


#if defined (RAMTST_ALLOW_API_ID) && (RAMTST_ALLOW_API == STD_ON)

void RamTst_Allow(void)
{
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_ALLOW_API_ID, RAMTST_E_UNINIT);
    if((RamTst_Info.ExecutionState == RAMTST_EXECUTION_STOPPED) || (RamTst_Info.ExecutionState == RAMTST_EXECUTION_INIT))
    {
        RamTst_Info.ExecutionState = RAMTST_EXECUTION_ALLOWED;
        RamTst_Info.test_address = RamTstConfig->AlgParamCfg[RamTst_Info.algo_idx].BlockParamCfg[0].RamTstStartAddress;
        RamTst_Info.block_id = 0;
    }
}

#endif


#if defined(RAMTST_SUSPEND_API_ID) && (RAMTST_SUSPEND_API == STD_ON)

void RamTst_Suspend(void)
{
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_SUSPEND_API_ID, RAMTST_E_UNINIT);

    RamTst_Info.ExecutionState = RAMTST_EXECUTION_SUSPENDED;
}

#endif


#if defined(RAMTST_RESUME_API_ID) && (RAMTST_RESUME_API == STD_ON)

void RamTst_Resume(void)
{
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_RESUME_API_ID, RAMTST_E_UNINIT);
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STOPPED), RAMTST_RESUME_API_ID, RAMTST_E_STATUS_FAILURE);
    if(RamTst_Info.ExecutionState == RAMTST_EXECUTION_SUSPENDED)
    {
        RamTst_Info.ExecutionState = RAMTST_EXECUTION_ALLOWED; /* No seprate state maintained for RESUMED/ALLOWED*/
    }
}

#endif


#if defined(RAMTST_GETEXECUTIONSTATUS_API_ID) && (RAMTST_GETEXECUTIONSTATUS_API == STD_ON)

RamTst_ExecutionStatusType RamTst_GetExecutionStatus(void)
{
	int ts_error = 0;
	RAMTST_ASSERT((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_GETEXECUTIONSTATUS_API_ID, RAMTST_E_UNINIT);
	if(ts_error != 0)
	{
		return RAMTST_EXECUTION_STATE_UNDEFINED;
	}
    else
    {
    	return RamTst_Info.ExecutionState;
    }
}

#endif


#if defined(RAMTST_GETTESTRESULT_API_ID) && (RAMTST_GETTESTRESULT_API == STD_ON)

RamTst_TestResultType RamTst_GetTestResult(void)
{
	int i;
	int ts_error = 0;
	RamTst_TestResultType result;
    RAMTST_ASSERT((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_GETTESTRESULT_API_ID, RAMTST_E_UNINIT);
    if(ts_error != 0)
    {
    	return RAMTST_RESULT_UNDEFINED;
    }
    result = RamTst_Info.Result;
    RamTst_Info.Result = RAMTST_RESULT_NOT_TESTED;
    for(i=0;i<RAMTST_NUMBER_OF_BLOCKS;i++)
    {
        RamTst_Info.RamTst_BlockResults[i] = RAMTST_RESULT_NOT_TESTED;
    }
    return result;
}

#endif


#if defined(RAMTST_GETTESTRESULTPERBLOCK_API_ID) && (RAMTST_GETTESTRESULTPERBLOCK_API == STD_ON)

RamTst_TestResultType RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID)
{
	int ts_error = 0;
	RamTst_TestResultType result;

    RAMTST_ASSERT((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_GETTESTRESULTPERBLOCK_API_ID, RAMTST_E_UNINIT);
    RAMTST_ASSERT((BlockID <= RamTstConfig->AlgParamCfg[RamTst_Info.algo_idx].RamTstNumberOfBlocks), RAMTST_GETTESTRESULTPERBLOCK_API_ID, RAMTST_E_OUT_OF_RANGE);
    if(ts_error != 0)
    {
       	return RAMTST_RESULT_UNDEFINED;
    }
    result = RamTst_Info.RamTst_BlockResults[BlockID];
    RamTst_Info.RamTst_BlockResults[BlockID] = RAMTST_RESULT_NOT_TESTED;
    return result;
}

#endif


#if defined(RAMTST_GETTESTALGORITHM_API_ID) && (RAMTST_GETTESTALGORITHM_API == STD_ON)

RamTst_AlgorithmType RamTst_GetTestAlgorithm(void)
{
	int ts_error = 0;
    RAMTST_ASSERT((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_GETTESTALGORITHM_API_ID, RAMTST_E_UNINIT);
    if(ts_error != 0)
     {
        	return RAMTST_ALGORITHM_UNDEFINED;
     }
    return RamTst_Info.Algorithm;
}

#endif


#if defined(RAMTST_GETNUMBEROFTESTEDCELLS_API_ID) && (RAMTST_GETNUMBEROFTESTEDCELLS_API == STD_ON)

RamTst_NumberOfTestedCellsType RamTst_GetNumberOfTestedCells(void)
{
	int ts_error = 0;
	RAMTST_ASSERT((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_GETNUMBEROFTESTEDCELLS_API_ID, RAMTST_E_UNINIT);
    if(ts_error != 0)
     {
        	return 0;
     }
	return RamTst_Info.RamTstNumberOfTestedCells;
}

#endif


#if defined(RAMTST_CHANGETESTALGORITHM_API_ID) && (RAMTST_CHANGETESTALGORITHM_API == STD_ON)

void RamTst_ChangeTestAlgorithm(RamTst_AlgorithmType NewTestAlgorithm)
{
	const RamTst_AlgParamsType *algo_cfg;
	uint8 algo_detected=1, i;
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_CHANGETESTALGORITHM_API_ID, RAMTST_E_UNINIT);
    RAMTST_ASSERT_RETVOID(((NewTestAlgorithm <= RamTstConfig->RamTstNumberofAlgs) && (NewTestAlgorithm > 0)), RAMTST_CHANGETESTALGORITHM_API_ID, RAMTST_E_OUT_OF_RANGE);
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState == RAMTST_EXECUTION_STOPPED), RAMTST_CHANGETESTALGORITHM_API_ID, RAMTST_E_STATUS_FAILURE);

    for(i=0; ((i<RamTstConfig->RamTstNumberofAlgs) && algo_detected); i++)
    {
    	if(NewTestAlgorithm == RamTstConfig->AlgParamCfg[i].RamTstAlgId)
    	{
    		RamTst_Info.algo_idx = i /* index to access the algo config structure for corresponding algorithm */;
    		algo_detected = 0;
    	}
    }
    algo_cfg = (const RamTst_AlgParamsType *)&RamTstConfig->AlgParamCfg[RamTst_Info.algo_idx];

    RamTst_Info.block_id = 0;
    RamTst_Info.test_address = algo_cfg->BlockParamCfg[0].RamTstStartAddress;
    /* Keep the test results untouched*/
   	RamTst_Info.RamTstNumberOfTestedCells = algo_cfg->RamTstNumberofTestedCells;
   	RamTst_Info.Algorithm = NewTestAlgorithm;
}

#endif


#if defined(RAMTST_CHANGENUMBEROFTESTEDCELLS_API_ID) && (RAMTST_CHANGENUMBEROFTESTEDCELLS_API == STD_ON)

void RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells)
{
    const RamTst_AlgParamsType *algo_cfg;
    algo_cfg = (const RamTst_AlgParamsType *)&RamTstConfig->AlgParamCfg[RamTst_Info.algo_idx];

    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_CHANGENUMBEROFTESTEDCELLS_API_ID, RAMTST_E_UNINIT);
    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState == RAMTST_EXECUTION_STOPPED), RAMTST_CHANGENUMBEROFTESTEDCELLS_API_ID, RAMTST_E_STATUS_FAILURE);
    RAMTST_ASSERT_RETVOID(((NewNumberOfTestedCells >= algo_cfg->RamTstMinNumberOffTestedCells) && (NewNumberOfTestedCells <= algo_cfg->RamTstMaxNumberOfTestedCells)), RAMTST_CHANGEMAXNUMBEROFTESTEDCELLS_API_ID, RAMTST_E_OUT_OF_RANGE);


    if(NewNumberOfTestedCells < algo_cfg->RamTstMinNumberOffTestedCells)
    {
    	RamTst_Info.RamTstNumberOfTestedCells = algo_cfg->RamTstMinNumberOffTestedCells;
    }
    else if(NewNumberOfTestedCells > algo_cfg->RamTstMaxNumberOfTestedCells)
    {
    	RamTst_Info.RamTstNumberOfTestedCells = algo_cfg->RamTstMaxNumberOfTestedCells;
    }
    else
    {
    	RamTst_Info.RamTstNumberOfTestedCells = NewNumberOfTestedCells;
    }
}

#endif


void RamTst_MainFunction(void)
{
    uint32* end_address;
    uint32* start_address;
    uint32* error_address = NULL;
    const RamTst_AlgParamsType *algo_cfg;
    uint32 length, copy_length;
    uint16 number_of_test_cells, csa_unused;
    uint8 current_block,algorithm,block_over=0;

    RAMTST_ASSERT_RETVOID((RamTst_Info.ExecutionState != RAMTST_EXECUTION_STATE_UNDEFINED), RAMTST_MAINFUNCTION_API_ID, RAMTST_E_UNINIT);

    switch(RamTst_Info.ExecutionState)
    {
        case RAMTST_EXECUTION_UNINIT:
            RAMTST_ASSERT_RETVOID(0, RAMTST_MAINFUNCTION_API_ID, RAMTST_E_UNINIT);
            break;

        case RAMTST_EXECUTION_INIT:
            RamTst_Info.ExecutionState = RAMTST_EXECUTION_RUNNING;
            /* Fall through code, no break*/

        case RAMTST_EXECUTION_RUNNING:
        case RAMTST_EXECUTION_ALLOWED:
            /* Test "RamTstNumberOfTestedCells" in an region for each call of
             * "MainFunction". */
            /*1. Take the algorithm and start address
              2. Take backup of data if it is ND way.
              3. Perform the test for the region for "Number of tested cells"
              4. Keep the results. */
        	RamTst_Info.ExecutionState = RAMTST_EXECUTION_RUNNING;
        	SuspendAllInterrupts();
        	algorithm = RamTst_Info.algo_idx;
        	algo_cfg = (const RamTst_AlgParamsType *)&RamTstConfig->AlgParamCfg[algorithm];
        	current_block = RamTst_Info.block_id;
            number_of_test_cells = RamTst_Info.RamTstNumberOfTestedCells;
            start_address = RamTst_Info.test_address;
            if((current_block != algo_cfg->StackBlockID) && (current_block != algo_cfg->CSABlockID))
            {	/* Not an special block*/
            	end_address = algo_cfg->BlockParamCfg[current_block].RamTstEndAddress;
            }
            else if(current_block == algo_cfg->StackBlockID)
            {	/* Block belongs stack - very careful, should not be overlapped with other regions- only complete stack*/
            	end_address = RamTst_GetStackPointer() - (RAMTST_STACK_OFFSET/4);
            }
            else
            {	/* Block belongs CSA - very careful, should not be overlapped with other regions- only complete CSA*/
            	csa_unused = RamTst_GetCSA() * 64 /* Each CSA Size */;
            	end_address = (algo_cfg->BlockParamCfg[current_block].RamTstStartAddress + (csa_unused/4));
            }

            if((start_address + number_of_test_cells) < end_address)
            {
                length = number_of_test_cells;
                RamTst_Info.test_address = start_address + length;
            }
            else
            {
                length = end_address - start_address;
                RamTst_Info.block_id = current_block + 1;
                if(RamTst_Info.block_id >= algo_cfg->RamTstNumberOfBlocks)
                {
                	RamTst_Info.block_id = 0;
                    block_over = 1;
                }
                RamTst_Info.test_address = algo_cfg->BlockParamCfg[RamTst_Info.block_id].RamTstStartAddress;
            }
            ResumeAllInterrupts();

            if(algo_cfg->algo_type == RAMTST_ALGORITHM_DESTRUCTIVE){
                /* Call the algorithm - under interrupt protection */
                SuspendAllInterrupts();
                error_address = ramtst_algo[RamTst_Info.Algorithm].ramtst_algofunc(start_address, length);
                ResumeAllInterrupts();/*Todo - check interrupt routine usage*/
            }
            else
            {
                while(length)
                {
                    /* Copy the data from the region under test - 64bytes at a time*/
                    if(length >= 64)
                    {
                        copy_length = 64;
                    }
                    else
                    {
                        copy_length = length;
                    }
                    length = length - copy_length;
                    /* Call the algorithm - under interrupt protection*/
                    SuspendAllInterrupts();
                    RamTst_StoreData(start_address, copy_length);
                    error_address = ramtst_algo[RamTst_Info.Algorithm].ramtst_algofunc(start_address, copy_length);
                    RamTst_ReStoreData(start_address, copy_length);
                    ResumeAllInterrupts();/* Todo thva - check interrupt routine usage */
                    start_address = start_address + copy_length;
                    if(error_address != NULL)
                    {
                    	length = 0;
                    }
                }
            }
            if(RamTst_Info.RamTst_BlockResults[current_block] != RAMTST_RESULT_NOT_OK)
            {
            	if(error_address)
            	{
            		RamTst_Info.RamTst_BlockResults[current_block] = RAMTST_RESULT_NOT_OK;
            	}
            	else
            	{
            		RamTst_Info.RamTst_BlockResults[current_block] = RAMTST_RESULT_OK;
            	}
             }
            if(error_address != NULL)
            {
                Dem_ReportErrorStatus(RAMTST_E_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
                RamTst_Info.Result = RAMTST_RESULT_NOT_OK;
                RamTst_Info.ExecutionState = RAMTST_EXECUTION_SUSPENDED;
                /* Call the errro notification */
                if(RamTstConfig->RamTstErrorNotify){
                    RamTstConfig->RamTstErrorNotify();
                }
                return;
            }
            if(block_over == 1)
            {
            	if(RamTst_Info.Result!=RAMTST_RESULT_NOT_OK)
            	{
            		RamTst_Info.Result = RAMTST_RESULT_OK; /* Because SWS never asked to clear the error*/
            	}
                if(RamTstConfig->RamTstCompleteNotify)
                {
                    RamTstConfig->RamTstCompleteNotify();
                }
            }
            break;

        case RAMTST_EXECUTION_STOPPED:
        case RAMTST_EXECUTION_SUSPENDED:
            break;

        default:
        	break;
      }
}


void RamTst_FullTest(void)
{
	uint32* start_address;
	uint32* error_address = NULL;
	uint32* stackpointer;
    const RamTst_AlgParamsType *algo_cfg;
	int i;
	uint16 length, copy_length;
	uint8 break_loop=0;

	algo_cfg = (const RamTst_AlgParamsType *)&RamTstConfig->AlgParamCfg[RamTst_Info.algo_idx];


	for(i=0;((i<algo_cfg->RamTstNumberOfBlocks) && (!break_loop));i++){
		start_address = algo_cfg->BlockParamCfg[i].RamTstStartAddress;
		if(algo_cfg->StackBlockID != i){
			length = algo_cfg->BlockParamCfg[i].RamTstEndAddress - start_address;
		}
		else
		{
			/*Read SP and reduce the length => Add comment Start must be less than End
			 * assumed stack is growing upward */
			stackpointer = RamTst_GetStackPointer();
			length =  (stackpointer - algo_cfg->BlockParamCfg[i].RamTstStartAddress) - RAMTST_STACK_OFFSET;
		}

		if(algo_cfg->algo_type == RAMTST_ALGORITHM_DESTRUCTIVE)
		{
			error_address = ramtst_algo[RamTst_Info.Algorithm].ramtst_algofunc(start_address, length);
			/* WDT service hook */
            if(RamTstConfig->RamTstWdgHook)
            {
             	RamTstConfig->RamTstWdgHook();
            }
			if(error_address != NULL)
			{
				break_loop = 1;	/* to break the for loop*/
			}
		}
		else
		{
            while(length)
            {
                /* Copy the data from the region under test - 64bytes at a time*/
                if(length >= 64)
                {
                    copy_length = 64;
                }
                else
                {
                    copy_length = length;
                }
                length = length - copy_length;
                RamTst_StoreData(start_address, copy_length);
                error_address = ramtst_algo[RamTst_Info.Algorithm].ramtst_algofunc(start_address, copy_length);
                RamTst_ReStoreData(start_address, copy_length);
                start_address = start_address + copy_length;

                if(error_address != NULL)
                {
                	length = 0;
                	break_loop = 1;
                }
                /* WDT service hook */
                if(RamTstConfig->RamTstWdgHook)
                {
                	RamTstConfig->RamTstWdgHook();
                }
            }
		}
	}
	if(error_address != NULL)
	{
		Dem_ReportErrorStatus(RAMTST_E_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
		RamTst_Info.Result = RAMTST_RESULT_NOT_OK;
		/* error notification has to be called only from the background test(RamTst_MainFunction).*/
		return;
	}
	else
	{
		RamTst_Info.Result = RAMTST_RESULT_OK;
	}
}


#define RAMTST_STOP_SEC_CODE
#include "MemMap.h"
