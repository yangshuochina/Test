/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fee_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:23:28MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fee_Priv.h  $
 *  Revision 1.2 2010/07/12 14:23:28MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef FEE_PRIV_H_
#define FEE_PRIV_H_

#include "team-s.h"
#include "Fee_Cfg.h"


#if defined(FEE_DEV_ERROR_DETECT) && (FEE_DEV_ERROR_DETECT == STD_ON)
#  define FEE_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, FEE_MODULE_ID, FEE_INSTANCE_ID, api_id, error_id)
#  define FEE_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, FEE_MODULE_ID, FEE_INSTANCE_ID, api_id, error_id)
#  define FEE_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, FEE_MODULE_ID, FEE_INSTANCE_ID, api_id, error_id)
#else
#  define FEE_ASSERT_RETERR(cond, api_id, error_id)
#  define FEE_ASSERT_RETVOID(cond, api_id, error_id)
#  define FEE_ASSERT(cond, api_id, error_id)
#endif



#define FEE_NUM_OF_SEGMENTS			(2)
#define FEE_PAGE_SIZE				(128)	/* bytes */
#define FEE_PAGE_SIZE_IN_WORDS		(FEE_PAGE_SIZE / 4)
#define FEE_NUM_OF_PAGES_PER_SECTOR	(256)



typedef struct {
	uint8 block[127]; /* 0=empty/inconsistent, 1..254=generation, 255=invalid */
	uint8 serial;
} Fee_MgmtType;


typedef enum {
	FEE_STATE_UNINIT,
	FEE_STATE_LOAD_PAGES_START,
	FEE_STATE_LOAD_PAGES_READ,
	FEE_STATE_LOAD_PAGES_EVAL,
	FEE_STATE_COPYPAGES_START,
	FEE_STATE_COPYPAGES_CONT,
	FEE_STATE_COPYPAGES_LOADPAGE,
	FEE_STATE_COPYPAGES_WRITEPAGE,
	FEE_STATE_COPYPAGES_WRITEMGMT,
	FEE_STATE_COPYPAGES_ERASESEG,
	FEE_STATE_CHECKFILL_LEVEL,
	FEE_STATE_INITSEG,
	FEE_STATE_CMD_READ,
	FEE_STATE_CMD_READ_WAIT,
	FEE_STATE_CMD_WRITE,
	FEE_STATE_CMD_WRITE_MGMT,
	FEE_STATE_CMD_INVALIDATE_BLOCK,
	FEE_STATE_CMD_ERASE_IMMEDIATE_BLOCK,
	FEE_STATE_ERROR,
	FEE_STATE_SUCCESS,
	FEE_STATE_READY,
} Fee_StateType;


typedef struct {
	uint32 serial_seg;	/* serial number, incremented each time a new sector is started; 0 indicates uninitialized */
	uint32 num_mgmt;	/* number of mgmt blocks */
	uint32 num_of_pages_in_generation; /* number of pages in one generation */
	Fee_MgmtType mgmt;
	BITARRAY(used, FEE_NUM_OF_PAGES_PER_SECTOR);
	uint8 used_mgmt_page; /* 0=none used, 1=first, ... */
} Fee_SegInformationType;


typedef struct {
	enum {
		FEE_CMD_IDLE,
		FEE_CMD_WRITE,
		FEE_CMD_READ,
		FEE_CMD_INVALIDATE_BLOCK,
		FEE_CMD_ERASE_IMMEDIATE_BLOCK
	} pending;
	uint16 block_number;
	uint8 *buf;
	uint16 block_offset;
	uint16 length;
} Fee_CmdType;


#define FEE_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Fee_BlockConfiguration fee_block_configuration[];

#define FEE_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"


#define FEE_START_SEC_VAR_32BIT
#include "MemMap.h"

extern Fee_StateType fee_state;
extern Fee_CmdType fee_cmd;
extern Fee_SegInformationType fee_seg_info[FEE_NUM_OF_SEGMENTS];
extern MemIf_JobResultType fee_job_result;

#define FEE_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#endif /* FEE_PRIV_H_ */


