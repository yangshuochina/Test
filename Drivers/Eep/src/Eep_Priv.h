/*
 * Eep_Priv.h
 *
 *  Created on: 2015-2-2
 *      Author: Administrator
 */

#ifndef EEP_PRIV_H_
#define EEP_PRIV_H_

#include "team-s.h"
#include "Eep_Cfg.h"

#if defined(EEP_DEV_ERROR_DETECT) && (EEP_DEV_ERROR_DETECT == STD_ON)
#  define EEP_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, EEP_MODULE_ID, EEP_INSTANCE_ID, api_id, error_id)
#  define EEP_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, EEP_MODULE_ID, EEP_INSTANCE_ID, api_id, error_id)
#  define EEP_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, EEP_MODULE_ID, EEP_INSTANCE_ID, api_id, error_id)
#else
#  define EEP_ASSERT_RETERR(cond, api_id, error_id)
#  define EEP_ASSERT_RETVOID(cond, api_id, error_id)
#  define EEP_ASSERT(cond, api_id, error_id)
#endif

#define CMD_READ 	0x03
#define CMD_WRITE	0x02
#define CMD_WREN	0x06
#define CMD_RDSR	0x05
#define MASK_WIP	0x01

typedef enum
{
	Eep_STATE_UNINIT,
	Eep_STATE_IDLE,
	Eep_STATE_ReadStatusWait,
	Eep_STATE_ReadDataWait,
	Eep_STATE_WriteDataWait,
	Eep_STATE_WriteDataWait2,
	Eep_STATE_Error,
	Eep_STATE_JobDone
}Eep_StateType;


typedef enum
{
	EEP_CMD_IDLE,
	EEP_CMD_READ,
	EEP_CMD_WRITE
}Eep_OperateType;

typedef struct
{
	Eep_OperateType pending;
	void* DataAddress;
	uint32 StartAddress;
	uint16 LeftSize;
	uint16 ThisSize;
} Eep_CmdType;

typedef struct {
	MemIf_StatusType    	Status;
	MemIf_JobResultType 	JobResult;
	Eep_StateType 			State;
	Eep_CmdType				Cmd;
	MemIf_ModeType			Mode;
}Eep_Var;

#endif /* EEP_PRIV_H_ */
