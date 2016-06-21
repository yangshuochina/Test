/*
 * Ea_Priv.h
 *
 *  Created on: 2015-2-2
 *      Author: Administrator
 */

#ifndef EA_PRIV_H_
#define EA_PRIV_H_



#if defined(EA_DEV_ERROR_DETECT) && (EA_DEV_ERROR_DETECT == STD_ON)
#  define EA_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, EA_MODULE_ID, 0, api_id, error_id)
#  define EA_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, EA_MODULE_ID, 0, api_id, error_id)
#  define EA_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, EA_MODULE_ID, 0, api_id, error_id)
#else
#  define EA_ASSERT_RETERR(cond, api_id, error_id)
#  define EA_ASSERT_RETVOID(cond, api_id, error_id)
#  define EA_ASSERT(cond, api_id, error_id)
#endif

#include "Eep.h"

typedef enum
{
	EA_STATE_UNINIT,
	EA_STATE_IDLE,
	EA_STATE_FindPageStart,
	EA_STATE_FindPageWait,
	EA_STATE_ReadPageDataStart,
	EA_STATE_ReadPageDataWait,
	EA_STATE_WritePageDataStart,
	EA_STATE_WritePageDataWait,
}Ea_StateType;
typedef enum
{
		EA_CMD_IDLE,
		EA_CMD_WRITE,
		EA_CMD_READ,
		EA_CMD_INVALIDATE_BLOCK,
		EA_CMD_ERASE_IMMEDIATE_BLOCK
}Ea_OperateType;
typedef struct
{
	Ea_OperateType  pending;
	void* DataAddress;
	uint16 DataLength;
	uint16 Offset;
	uint16 BlockNumber;
} Ea_CmdType;

typedef struct {
	sint8  GenerNum;
	uint8  BlockNum;
	uint8  DataCrc8;
	uint8  XorSum;
} Ea_AdminBlock;

typedef struct {
	const Ea_ConfigType *   cfg;
	MemIf_StatusType    	Status;
	MemIf_JobResultType 	JobResult;
	Ea_StateType 			State;
	Ea_CmdType				Cmd;
	Ea_PageStatus			Page;
	Ea_PageStatus			NextPage;
	uint16					EaIndex;
	sint8					ReadPages;
}Ea_Var;

typedef enum
{
	Invalid,
	Init,
	LoadAdminStart,
	LoadAdminWait,
	LoadAdminDone
}Find_State;

typedef struct
{
	Find_State	State;
	sint8       nowPage;
}Find_Var;

#endif /* EA_PRIV_H_ */
