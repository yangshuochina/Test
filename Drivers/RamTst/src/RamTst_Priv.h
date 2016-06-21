/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: RamTst_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:30:01MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: RamTst_Priv.h  $
 *  Revision 1.2 2010/07/12 14:30:01MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/
 
#ifndef RAMTST_PRIV_H_
#define RAMTST_PRIV_H_

#include "team-s.h"
#include "RamTst_Cfg.h"


#if defined(RAMTST_DEV_ERROR_DETECT) && (RAMTST_DEV_ERROR_DETECT == STD_ON)
#  define RAMTST_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, api_id, error_id)
#  define RAMTST_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, api_id, error_id)
#  define RAMTST_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, api_id, error_id)
#else
#  define RAMTST_ASSERT_RETERR(cond, api_id, error_id)
#  define RAMTST_ASSERT_RETVOID(cond, api_id, error_id)
#  define RAMTST_ASSERT(cond, api_id, error_id)
#endif

#define RAMTST_STACK_OFFSET			(64)	/*Size in bytes to reserve the location inside Stack before start testing*/

typedef struct
{
		uint32* (*const ramtst_algofunc)(uint32* start_addr, uint32 length);
} RamTstAlgorithmType;

typedef struct
{
	RamTst_ExecutionStatusType ExecutionState;
    RamTst_TestResultType Result;
    RamTst_AlgorithmType Algorithm;
    RamTst_NumberOfTestedCellsType RamTstNumberOfTestedCells;
    uint32* test_address;
    uint32* ErrorAddr;
    uint32 backup_area[64];
    uint8 algo_idx;
    uint8 block_id;
    RamTst_TestResultType RamTst_BlockResults[RAMTST_NUMBER_OF_BLOCKS];
}RamTst_DataStruct;


#endif /* RAMTST_PRIV_H_ */
