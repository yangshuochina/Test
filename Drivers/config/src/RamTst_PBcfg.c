/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: RamTst_PBcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:16:48MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: RamTst_PBcfg.c  $
 *  Revision 1.3 2011/02/02 09:16:48MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 14:15:24MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "RamTst.h"
#include "RamTst_Priv.h"

#define NO_STACK_BLOCK	(0xFF)	/* This implies no stack region configured for testing */
#define NO_CSA_BLOCK	(0xFF)	/* This implies no stack region configured for testing */


#define RAMTST_START_SEC_VAR_32BIT
#include "MemMap.h"

uint8 ramtest_completed;

#define RAMTST_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define RAMTST_START_SEC_CODE
#include "MemMap.h"

extern uint32* RamTst_Algo_MarchTest(uint32* low_add, uint32 size );
extern uint32* RamTst_Algo_GalpatTest(uint32* low_add, uint32 size);
extern uint32* RamTst_Algo_CheckerboardTest(uint32* low_add, uint32 size );
extern uint32* RamTst_Algo_WalkpathTest(uint32* low_add, uint32 size );

void RamTst_TestCompletedNotification(void)
{
	ramtest_completed++;
}

void RamTst_ErrorNotification(void)
{
	/* ramtest_error = 1; */
}
#define RAMTST_STOP_SEC_CODE
#include "MemMap.h"


#define RAMTST_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

const RamTstAlgorithmType ramtst_algo[] = {
		{ NULL },	/**< No Specific algorithm, Must be NULL always.*/
	    #if (RAMTST_CHECKERBOARD_TEST_SELECTED == STD_ON)
		{	RamTst_Algo_CheckerboardTest },		/**< Check Board algorithm is selected. */
	    #endif
	    #if (RAMTST_MARCH_TEST_SELECTED == STD_ON)
		{    RamTst_Algo_MarchTest },				/**< March test algorithm is selected. */
	    #endif
	    #if (RAMTST_WALK_PATH_TEST_SELECTED == STD_ON)
		{    RamTst_Algo_WalkpathTest },			/**< Walk path algorithm is selected. */
	    #endif
	    #if (RAMTST_GALPAT_TEST_SELECTED == STD_ON)
		{    RamTst_Algo_GalpatTest },				/**< GalPat test algorithm is selected. */
	    #endif
};

const RamTst_BlockParamsType RamTstBlockCfg[] = {
	{(uint32*)0xD0004000,  (uint32*)0xD0005000, 0},
	{(uint32*)0xD0006000,  (uint32*)0xD0006480, 1},
	{(uint32*)0xD0004808,  (uint32*)0xD0005808, 2}, // STACK
	{(uint32*)0xD0004400,  (uint32*)0xD0004808, 3},
	{(uint32*)0xD0008000,  (uint32*)0xD0008880, 4},
	{(uint32*)0xd0000000,  (uint32*)0xd0001000, 5},
	{(uint32*)0xD0009000,  (uint32*)0xD0009990, 6}
};

const RamTst_AlgParamsType RamTstAlgParamCfg[] = {
		{
			&RamTstBlockCfg[2],
			RAMTST_MARCH_TEST,
			RAMTST_ALGORITHM_DESTRUCTIVE,
			0x200,
			0,
			0x400,
			0x100,
			3,
			0,
			NO_CSA_BLOCK,
		},
		{
		&RamTstBlockCfg[0],
		RAMTST_CHECKERBOARD_TEST,
		RAMTST_ALGORITHM_NONDESTRUCTIVE,
		0x100,				//words
		0,
		0x100,
		0x64,
		2,
		NO_STACK_BLOCK, /* No stack region configured for testing, otherwise provide block number */
		NO_CSA_BLOCK,
	},
	{
		&RamTstBlockCfg[5],
		RAMTST_GALPAT_TEST,
		RAMTST_ALGORITHM_NONDESTRUCTIVE,
		0x100,
		0,
		0x100,
		0x0,
		2,
		NO_STACK_BLOCK,
		0
	},

	{
		&RamTstBlockCfg[0],
		RAMTST_WALK_PATH_TEST,
		RAMTST_ALGORITHM_DESTRUCTIVE,
		0x200,
		0,
		0x400,
		0x100,
		2,
		NO_STACK_BLOCK,
		NO_CSA_BLOCK,
	},

};

const RamTst_ConfigType RamTstConfig[] = {
	{
	&RamTstAlgParamCfg[0],
	RamTst_TestCompletedNotification,
	RamTst_ErrorNotification,
	NULL,
	RAMTST_MARCH_TEST,
	4
	}
};



#define RAMTST_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
