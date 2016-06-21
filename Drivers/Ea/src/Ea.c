/*
 * Ea.c
 *
 *  Created on: 2015-2-2
 *      Author: Administrator
 */
#include "team-s.h"
#include "Det.h"
#include "Ea.h"
#include "Ea_Cbk.h"
#include "Ea_Priv.h"
#include "Crc.h"

#define EA_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const Ea_ConfigType ea_cfg;

#define EA_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

#define EA_START_SEC_VAR_8BIT
#include "MemMap.h"

#pragma align 4
extern uint8 Ea_TempBuffer[];
#pragma align restore

extern sint8 PageGenerations[];

#define EA_STOP_SEC_VAR_8BIT
#include "MemMap.h"

#define EA_START_SEC_VAR_32BIT
#include "MemMap.h"

Ea_Var ea_var;
Find_Var find_var;

#define EA_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define EA_START_SEC_CODE
#include "MemMap.h"

static void findValidPageCount(void);
static uint8 getXorSum(void* block, uint16 size);
static uint16 getEaIndexFromBlockNumber(uint16 BlockNumber);
static uint16 getVirtualPageCountOfEaIndex(uint16 EaIndex);
static Eep_AddressType getPageEepAddrOfEaIndex(uint16 EaIndex, sint8 Page);
static sint8 getLatestGenerationPageCount(uint16 EaIndex);
static sint8 getBlockMaxAvailableGeneration(uint16 EaIndex);
static uint8 getCrc8Sum(void* block, uint16 size);

void Ea_Init(void)
{
	uint32 i32 = 0;

	ea_var.cfg = &ea_cfg;
	ea_var.Cmd.pending = EA_CMD_IDLE;
	ea_var.Cmd.BlockNumber = 0;
	ea_var.Cmd.DataAddress = NULL_PTR;
	ea_var.Cmd.DataLength = 0;
	ea_var.Cmd.Offset = 0;
	ea_var.Page.ActiveGeneration = -1;
	ea_var.Page.ActivePageIndex = -1;
	for(i32 = 0; i32< ea_var.cfg->num_of_blocks; i32++)
	{
		ea_var.cfg->blocks[i32].PageStatus->ActiveGeneration = -1;
		ea_var.cfg->blocks[i32].PageStatus->ActivePageIndex = -1;
	}
	ea_var.State = EA_STATE_IDLE;
	ea_var.Status = MEMIF_IDLE;
	ea_var.JobResult = MEMIF_JOB_OK;
}
void Ea_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = EA_MODULE_ID;
	versioninfo->sw_major_version = EA_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = EA_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = EA_SW_PATCH_VERSION;
	versioninfo->vendorID = EA_VENDOR_ID;
}

Std_ReturnType Ea_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
{
	uint16 EaIndex;

	EA_ASSERT_RETERR(ea_var.Status != MEMIF_UNINIT ,EA_READ_API_ID ,EA_E_NOT_INITIALIZED);
	EA_ASSERT_RETERR(BlockNumber <0xFFFFU, EA_READ_API_ID, EA_E_INVALID_BLOCK_NO);

	if(ea_var.Status != MEMIF_IDLE)
		return E_NOT_OK;

	ea_var.Status = MEMIF_BUSY_INTERNAL;
	EaIndex = getEaIndexFromBlockNumber(BlockNumber);
	if(EaIndex >= ea_var.cfg->num_of_blocks)
	{
		ea_var.Status = MEMIF_IDLE;
		return E_NOT_OK;
	}
	if (BlockOffset + Length > ea_var.cfg->blocks[EaIndex].EaBlockSize)
	{
		ea_var.Status = MEMIF_IDLE;
		return E_NOT_OK;
	}
	ea_var.Cmd.BlockNumber = BlockNumber;
	ea_var.Cmd.DataAddress = DataBufferPtr;
	ea_var.Cmd.DataLength = Length;
	ea_var.Cmd.Offset = BlockOffset;

	ea_var.EaIndex = EaIndex;
	ea_var.Page.ActiveGeneration = ea_var.cfg->blocks[EaIndex].PageStatus->ActiveGeneration;
	ea_var.Page.ActivePageIndex = ea_var.cfg->blocks[EaIndex].PageStatus->ActivePageIndex;
	ea_var.Cmd.pending = EA_CMD_READ;
	ea_var.JobResult = MEMIF_JOB_PENDING;
	ea_var.Status = MEMIF_BUSY;
	ea_var.ReadPages = 0;

	return E_OK;
}
Std_ReturnType Ea_Write(uint16 BlockNumber, const uint8* DataBufferPtr)
{
	uint16 EaIndex;
	EA_ASSERT_RETERR(ea_var.Status != MEMIF_UNINIT ,EA_WRITE_API_ID ,EA_E_NOT_INITIALIZED);
	EA_ASSERT_RETERR(BlockNumber <0xFFFFU, EA_WRITE_API_ID, EA_E_INVALID_BLOCK_NO);

	if(ea_var.Status != MEMIF_IDLE)
		return E_NOT_OK;

	ea_var.Status = MEMIF_BUSY_INTERNAL;
	EaIndex = getEaIndexFromBlockNumber(BlockNumber);
	if(EaIndex >= ea_var.cfg->num_of_blocks)
	{
		ea_var.Status = MEMIF_IDLE;
		return E_NOT_OK;
	}
	ea_var.Cmd.BlockNumber = BlockNumber;
	ea_var.EaIndex = EaIndex;
	ea_var.Cmd.DataLength = ea_var.cfg->blocks[EaIndex].EaBlockSize;
	ea_var.Cmd.DataAddress = (void*)DataBufferPtr;
	ea_var.Cmd.Offset = 0;
	ea_var.Page.ActivePageIndex = ea_var.cfg->blocks[EaIndex].PageStatus->ActivePageIndex;
	ea_var.Page.ActiveGeneration = ea_var.cfg->blocks[EaIndex].PageStatus->ActiveGeneration;

	ea_var.Cmd.pending = EA_CMD_WRITE;
	ea_var.Status = MEMIF_BUSY;
	ea_var.JobResult = MEMIF_JOB_PENDING;

	return E_OK;
}

MemIf_StatusType Ea_GetStatus(void)
{
	return ea_var.Status;
}

MemIf_JobResultType Ea_GetJobResult(void)
{
	return ea_var.JobResult;
}
void Ea_MainFunction(void)
{
	Eep_AddressType page_addr;
	Std_ReturnType  eep_request_result;
	MemIf_JobResultType eep_job_result;
	uint8 ui8;
	uint8 uj8;

	if(Eep_GetStatus()!= MEMIF_IDLE)
		return;

	switch(ea_var.State)
	{
	case EA_STATE_UNINIT:
		ea_var.State = EA_STATE_IDLE;
		break;
	case EA_STATE_IDLE:
		{
			if(ea_var.Cmd.pending == EA_CMD_IDLE)
				break;
			if(ea_var.Page.ActivePageIndex < 0)
				ea_var.State = EA_STATE_FindPageStart;
			else
			{
				if(ea_var.Cmd.pending == EA_CMD_READ)
					ea_var.State = EA_STATE_ReadPageDataStart;
				else if(ea_var.Cmd.pending == EA_CMD_WRITE)
					ea_var.State = EA_STATE_WritePageDataStart;
			}
		} break;
	case EA_STATE_FindPageStart:
		{
			find_var.State = Init;
			ea_var.State = EA_STATE_FindPageWait;
		}break;
	case EA_STATE_FindPageWait:
		{
			if((ea_var.Page.ActivePageIndex < 0) && ( find_var.State != LoadAdminDone))
				findValidPageCount();
			else
			{
				if(ea_var.Page.ActivePageIndex < 0)
				{
					if(ea_var.Cmd.pending == EA_CMD_WRITE)
					{
						ea_var.Page.ActiveGeneration = -1;
						ea_var.Page.ActivePageIndex = -1;
						ea_var.State = EA_STATE_WritePageDataStart;
					}else
					{
						ea_var.JobResult = MEMIF_BLOCK_INVALID;
						ea_var.State = EA_STATE_IDLE;
						ea_var.Cmd.pending = EA_CMD_IDLE;
						Ea_JobErrorNotification();
						ea_var.Status = MEMIF_IDLE;
					}
				}else
				{
					ea_var.State = EA_STATE_IDLE;
				}
			}
		} break;
	case EA_STATE_ReadPageDataStart:
		{
			page_addr = getPageEepAddrOfEaIndex(ea_var.EaIndex,ea_var.Page.ActivePageIndex);
			eep_request_result = Eep_Read(page_addr,Ea_TempBuffer,
				ea_var.cfg->blocks[ea_var.EaIndex].EaBlockSize + sizeof(Ea_AdminBlock));
			if(eep_request_result == E_OK)
				ea_var.State = EA_STATE_ReadPageDataWait;
		} break;
	case EA_STATE_ReadPageDataWait:
		{
			eep_job_result = Eep_GetJobResult();
			switch(eep_job_result)
			{
			case MEMIF_JOB_PENDING:
				break;
			case MEMIF_JOB_CANCELLED:
			case MEMIF_BLOCK_INCONSISTENT:
			case MEMIF_BLOCK_INVALID:
			case MEMIF_JOB_FAILED:
				{
					ea_var.JobResult = MEMIF_JOB_FAILED;
					Ea_JobErrorNotification();
					ea_var.State = EA_STATE_IDLE;
					ea_var.Cmd.pending = EA_CMD_IDLE;
					ea_var.Status = MEMIF_IDLE;
				} break;
			case MEMIF_JOB_OK:
				{
					ui8 = getXorSum(Ea_TempBuffer,sizeof(Ea_AdminBlock));
					uj8 = getCrc8Sum(Ea_TempBuffer+sizeof(Ea_AdminBlock), ea_var.cfg->blocks[ea_var.EaIndex].EaBlockSize);
					if( (ui8 == 0)&& (uj8 == ((Ea_AdminBlock*)&Ea_TempBuffer[0])->DataCrc8))
					{
						memcpy(ea_var.Cmd.DataAddress,Ea_TempBuffer+sizeof(Ea_AdminBlock),ea_var.Cmd.DataLength);
						ea_var.Page.ActiveGeneration = ((Ea_AdminBlock*)&Ea_TempBuffer[0])->GenerNum;
						ea_var.cfg->blocks[ea_var.EaIndex].PageStatus->ActiveGeneration = ea_var.Page.ActiveGeneration;
						ea_var.cfg->blocks[ea_var.EaIndex].PageStatus->ActivePageIndex = ea_var.Page.ActivePageIndex;
						ea_var.JobResult = MEMIF_JOB_OK;
						ea_var.State = EA_STATE_IDLE;
						ea_var.Cmd.pending = EA_CMD_IDLE;
						ea_var.Status = MEMIF_IDLE;
						Ea_JobEndNotification();
					}else
					{
						ea_var.ReadPages++;
						if(ea_var.ReadPages >= ea_var.cfg->blocks[ea_var.EaIndex].EaPageCount)
						{
							if( ui8 == 0 && uj8 != ((Ea_AdminBlock*)Ea_TempBuffer)->DataCrc8)
								ea_var.JobResult = MEMIF_BLOCK_INCONSISTENT;
							else
								ea_var.JobResult = MEMIF_BLOCK_INVALID;
							ea_var.State = EA_STATE_IDLE;
							ea_var.Cmd.pending = EA_CMD_IDLE;
							ea_var.Status = MEMIF_IDLE;
							Ea_JobErrorNotification();
						}else
						{
							ea_var.Page.ActivePageIndex--;
							if(ea_var.Page.ActivePageIndex < 0)
								ea_var.Page.ActivePageIndex =
									ea_var.cfg->blocks[ea_var.EaIndex].EaPageCount -1;
							ea_var.State = EA_STATE_ReadPageDataStart;
						}
					}
				} break;
			}
		} break;
	case EA_STATE_WritePageDataStart:
		{
			/* Calculate next active generation and page index. */
			if(ea_var.Page.ActivePageIndex < 0) ea_var.NextPage.ActivePageIndex = 0;
			else ea_var.NextPage.ActivePageIndex = ea_var.Page.ActivePageIndex + 1;
			if(ea_var.Page.ActiveGeneration < 0) ea_var.NextPage.ActiveGeneration = 0;
			else ea_var.NextPage.ActiveGeneration = ea_var.Page.ActiveGeneration +1;

			if(ea_var.NextPage.ActivePageIndex > ea_var.cfg->blocks[ea_var.EaIndex].EaPageCount-1 )
				ea_var.NextPage.ActivePageIndex = 0;
			if(ea_var.NextPage.ActiveGeneration > getBlockMaxAvailableGeneration(ea_var.EaIndex))
				ea_var.NextPage.ActiveGeneration = 0;

			/* Set Ea_AdminBlock */
			memcpy(Ea_TempBuffer + sizeof(Ea_AdminBlock),ea_var.Cmd.DataAddress,ea_var.Cmd.DataLength);
			((Ea_AdminBlock*)Ea_TempBuffer)->BlockNum = (uint8)ea_var.Cmd.BlockNumber;
			((Ea_AdminBlock*)Ea_TempBuffer)->GenerNum = ea_var.NextPage.ActiveGeneration;
			((Ea_AdminBlock*)Ea_TempBuffer)->DataCrc8 = getCrc8Sum(Ea_TempBuffer+sizeof(Ea_AdminBlock),ea_var.Cmd.DataLength);
			((Ea_AdminBlock*)Ea_TempBuffer)->XorSum = getXorSum(Ea_TempBuffer, sizeof(Ea_AdminBlock)-1);
			page_addr = getPageEepAddrOfEaIndex(ea_var.EaIndex,ea_var.NextPage.ActivePageIndex);
			eep_request_result = Eep_Write(page_addr,Ea_TempBuffer,ea_var.Cmd.DataLength + sizeof(Ea_AdminBlock));
			if(eep_request_result == E_NOT_OK)
			{
				ea_var.JobResult = MEMIF_JOB_FAILED;
				ea_var.State = EA_STATE_IDLE;
				ea_var.Cmd.pending = EA_CMD_IDLE;
				ea_var.Status = MEMIF_IDLE;
				Ea_JobErrorNotification();
			}else
			{
				ea_var.State = EA_STATE_WritePageDataWait;
			}
		} break;
	case EA_STATE_WritePageDataWait:
		{
			eep_job_result = Eep_GetJobResult();
			switch(eep_job_result)
			{
			case MEMIF_JOB_PENDING:
				break;
			case MEMIF_JOB_CANCELLED:
			case MEMIF_JOB_FAILED:
				{
					ea_var.JobResult = MEMIF_JOB_FAILED;
					ea_var.State = EA_STATE_IDLE;
					ea_var.Cmd.pending = EA_CMD_IDLE;
					ea_var.Status = MEMIF_IDLE;
					Ea_JobErrorNotification();
				} break;
			case  MEMIF_JOB_OK:
				{
					ea_var.Page = ea_var.NextPage;
					ea_var.cfg->blocks[ea_var.EaIndex].PageStatus->ActiveGeneration
						= ea_var.Page.ActiveGeneration;
					ea_var.cfg->blocks[ea_var.EaIndex].PageStatus->ActivePageIndex
						= ea_var.Page.ActivePageIndex;
					ea_var.JobResult = MEMIF_JOB_OK;
					ea_var.State = EA_STATE_IDLE;
					ea_var.Cmd.pending = EA_CMD_IDLE;
					ea_var.Status = MEMIF_IDLE;
					Ea_JobEndNotification();
				}break;
			default:
				break;
			}
		} break;
	}
}
static void findValidPageCount(void)
{
	uint32 i32 = 0;
	MemIf_StatusType eep_status;
	MemIf_JobResultType eep_result;
	Std_ReturnType op_result;
	uint8 ui8 = 0;
	sint8 i8 = 0;
	uint8 u8BlockNumber = 0;

	switch(find_var.State)
	{
	case Invalid:
		break;
	case Init:
		{
			find_var.nowPage = 0;
			for(i32=0; i32<ea_var.cfg->max_page_num; i32++)
				PageGenerations[i32] = -1;
			if(ea_var.cfg->blocks[ea_var.EaIndex].EaPageCount<2)
			{
				ea_var.Page.ActivePageIndex = 0;
				find_var.State = LoadAdminDone;
			} else
			{
				find_var.State = LoadAdminStart;
			}
		}break;
	case LoadAdminStart:
		{
			eep_status =  Eep_GetStatus();
			if(eep_status == MEMIF_IDLE)
			{
				op_result = Eep_Read(getPageEepAddrOfEaIndex(ea_var.EaIndex,find_var.nowPage),
					Ea_TempBuffer +(sizeof(Ea_AdminBlock)*find_var.nowPage),sizeof(Ea_AdminBlock));
				if(op_result == E_OK)
					find_var.State = LoadAdminWait;
			}
		}break;
	case LoadAdminWait:
	    {
			eep_status = Eep_GetStatus();
			if(eep_status == MEMIF_IDLE)
			{
				eep_result = Eep_GetJobResult();
				if(eep_result == MEMIF_JOB_OK)
				{ /* Last read of page admin info eep function success.*/
					u8BlockNumber = ((Ea_AdminBlock*)(Ea_TempBuffer + sizeof(Ea_AdminBlock)*find_var.nowPage))->BlockNum;
					ui8 = getXorSum(Ea_TempBuffer + (sizeof(Ea_AdminBlock)*find_var.nowPage),sizeof(Ea_AdminBlock));
					if(ui8 == 0 &&(u8BlockNumber == ea_var.Cmd.BlockNumber))
						PageGenerations[find_var.nowPage] =
						((Ea_AdminBlock*)(Ea_TempBuffer + sizeof(Ea_AdminBlock)*find_var.nowPage))->GenerNum;
					else
						PageGenerations[find_var.nowPage] = -1;
				}else
				{/* Last read of page admin info eep function failed.*/
					PageGenerations[find_var.nowPage] = -1;
				}
				if(find_var.nowPage >= ea_var.cfg->blocks[ea_var.EaIndex].EaPageCount-1)
				{/* All page admin info loaded */
					ea_var.Page.ActivePageIndex =  getLatestGenerationPageCount(ea_var.EaIndex);
					if(ea_var.Page.ActivePageIndex >=0)
						ea_var.Page.ActiveGeneration = PageGenerations[ea_var.Page.ActivePageIndex];
					find_var.State = LoadAdminDone;
				}else
				{/* Still have page admin info not loaded */
					find_var.nowPage++;
					find_var.State = LoadAdminStart;
				}
			}
		}break;
	case LoadAdminDone:
		{
		} break;
	default:
		break;
	}
}

static sint8 getBlockMaxAvailableGeneration(uint16 EaIndex)
{
	return ea_var.cfg->blocks[EaIndex].EaPageCount;
}

static sint8 getLatestGenerationPageCount(uint16 EaIndex)
{
	sint8 prev = -1;
	sint8 next = -1;
	sint8 thisIndex = -1;
	sint8 findIndex =  -1;
	sint8 maxIndex = ea_var.cfg->blocks[EaIndex].EaPageCount-1;
	sint8 maxGeneration = getBlockMaxAvailableGeneration(ea_var.EaIndex);

	if(ea_var.cfg->blocks[EaIndex].EaPageCount < 2)
		return PageGenerations[0];

	for (thisIndex = 0; thisIndex <= maxIndex; thisIndex++)
	{
		if (thisIndex == 0) prev = maxIndex;
		else prev = thisIndex - 1;
		if (thisIndex >= maxIndex) next = 0;
		else next = thisIndex + 1;

		if ((PageGenerations[next] != PageGenerations[thisIndex] + 1) &&
			!(PageGenerations[thisIndex] == maxGeneration && PageGenerations[next] == 0x00))
		{
			if ((PageGenerations[thisIndex] == (PageGenerations[prev] + 1))
				|| (PageGenerations[prev] == maxGeneration && PageGenerations[thisIndex] == 0x00))
			{
				findIndex = thisIndex;
				break;
			}
		}
	}
	if(findIndex < 0 && PageGenerations[0] >= 0)
		return 0;
	else
		return findIndex;
}

static uint16 getEaIndexFromBlockNumber(uint16 BlockNumber)
{
	uint16 EaIndex;
	for(EaIndex=0; EaIndex <ea_var.cfg->num_of_blocks; EaIndex++)
	{
		if(BlockNumber == ea_var.cfg->blocks[EaIndex].EaBlockNumber )
		{
			break;
		}
	}
	return EaIndex;
}

static uint16 getVirtualPageCountOfEaIndex(uint16 EaIndex)
{
	uint16 pagesize = ea_var.cfg->blocks[EaIndex].EaBlockSize + sizeof(Ea_AdminBlock);
	uint16 numOfPage = pagesize / ea_var.cfg->virtual_page_size;
	if (pagesize % ea_var.cfg->virtual_page_size)
			numOfPage++;
	return numOfPage;
}

static Eep_AddressType getPageEepAddrOfEaIndex(uint16 EaIndex, sint8 Page)
{
	Eep_AddressType EepAddress;
	const Ea_BlockConfigType *blocks = ea_var.cfg->blocks;
	uint32 totalNumOfPages = 0;
	uint16 blockNum = blocks[EaIndex].EaBlockNumber;
	uint16 i=0;

	for (i = 0; i < ea_var.cfg->num_of_blocks; i++)
		if (blocks[i].EaBlockNumber < blockNum)
			totalNumOfPages = totalNumOfPages + getVirtualPageCountOfEaIndex(i) * blocks[i].EaPageCount;

	EepAddress =  (totalNumOfPages + Page * getVirtualPageCountOfEaIndex(EaIndex)) * ea_var.cfg->virtual_page_size;

	return EepAddress;
}

static uint8 getXorSum(void* block, uint16 size)
{
	uint8* ptr = (uint8*)block;
	uint8 xorsum = *ptr;
	uint32 datcnt;

	for(datcnt=1; datcnt < size; datcnt++ )
		xorsum ^= *(ptr + datcnt);
	return xorsum;
}

static uint8 getCrc8Sum(void* block, uint16 size)
{
	return Crc_CalculateCRC8(block,size,0);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"
