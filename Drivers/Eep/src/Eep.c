/*
 * Eep.c
 *
 *  Created on: 2015-2-2
 *      Author: Administrator
 */
#include "Det.h"
#include "Eep.h"
#include "Eep_Cbk.h"
#include "Eep_Priv.h"
#include "Spi.h"
#include "Prj_MainCfg.h"
#include "Gpt.h"

#define BUFFER_RD_CMD()

#define BUFFER_RD_ADDR()	do{\
								eep_tx_buffer[1] = ((uint8)(eep_var.Cmd.StartAddress >> 16));\
								eep_tx_buffer[2] = ((uint8)(eep_var.Cmd.StartAddress >> 8));\
								eep_tx_buffer[3] = eep_var.Cmd.StartAddress & 0x00ff;\
							}while(0)

#define BUFFER_WR_CMD()
#define BUFFER_WR_ADDR()	do{\
								eep_tx_buffer[2] = ((uint8)(eep_var.Cmd.StartAddress >> 16));\
								eep_tx_buffer[3] = ((uint8)(eep_var.Cmd.StartAddress >> 8));\
								eep_tx_buffer[4] = eep_var.Cmd.StartAddress & 0x00ff;\
							}while(0)
#define BUFFER_ADDR_SIZE	3

#define EEP_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
extern const Eep_ConfigType eep_cfg;
#define EEP_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"


#define EEP_START_SEC_VAR_32BIT
#include "MemMap.h"
Eep_Var eep_var;
#define EEP_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define EEP_START_SEC_VAR_16BIT
#include "MemMap.h"
Spi_DataType	eep_tx_buffer[EEP_PAGE_SIZE+5];
Spi_DataType	eep_rx_buffer[EEP_PAGE_SIZE+5];
#define EEP_STOP_SEC_VAR_16BIT
#include "MemMap.h"

#define EEP_START_SEC_CODE
#include "MemMap.h"
void Eep_Init(void)
{
	eep_var.Status = MEMIF_UNINIT;
	eep_var.JobResult = MEMIF_JOB_OK;
	eep_var.Cmd.pending = EEP_CMD_IDLE;
	eep_var.Mode = eep_cfg.DefaultMode;
	eep_var.State = Eep_STATE_IDLE;
	eep_var.Status = MEMIF_IDLE;

}

void Eep_SetMode(MemIf_ModeType Mode)
{
	EEP_ASSERT_RETVOID(eep_var.Status != MEMIF_UNINIT,EEP_SETMODE_API_ID,EEP_E_UNINIT);
	EEP_ASSERT_RETVOID(eep_var.Status == MEMIF_IDLE, EEP_SETMODE_API_ID,EEP_E_BUSY);
	eep_var.Mode = Mode;
}

Std_ReturnType Eep_Read(Eep_AddressType EepromAddress, uint8 *TargetAddressPtr, Eep_LengthType Length)
{
	uint32 addr;

	EEP_ASSERT_RETERR(eep_var.Status != MEMIF_UNINIT,EEP_READ_API_ID,EEP_E_UNINIT);
	EEP_ASSERT_RETERR(eep_var.Status == MEMIF_IDLE, EEP_READ_API_ID,EEP_E_BUSY);
	EEP_ASSERT_RETERR(TargetAddressPtr != NULL_PTR, EEP_READ_API_ID,EEP_E_PARAM_DATA);
	addr = EepromAddress + EEP_BASE_ADDRESS;
	EEP_ASSERT_RETERR((addr < EEP_BASE_ADDRESS + EEP_SIZE),EEP_READ_API_ID,EEP_E_PARAM_ADDRESS);
	EEP_ASSERT_RETERR((addr+Length)<(EEP_BASE_ADDRESS + EEP_SIZE),EEP_READ_API_ID,EEP_E_PARAM_LENGTH);

	eep_var.Status = MEMIF_BUSY_INTERNAL;
	eep_var.Cmd.DataAddress = TargetAddressPtr;
	eep_var.Cmd.LeftSize = Length;
	eep_var.Cmd.StartAddress = addr;
	eep_var.Cmd.pending = EEP_CMD_READ;
	eep_var.JobResult = MEMIF_JOB_PENDING;
	eep_var.Status = MEMIF_BUSY;

	return E_OK;
}

Std_ReturnType Eep_Erase(Eep_AddressType TargetAddress, Eep_LengthType Length)
{
	uint32 addr;

	EEP_ASSERT_RETERR(eep_var.Status != MEMIF_UNINIT,EEP_ERASE_API_ID,EEP_E_UNINIT);
	EEP_ASSERT_RETERR(eep_var.Status == MEMIF_IDLE, EEP_ERASE_API_ID,EEP_E_BUSY);
	addr = TargetAddress + EEP_BASE_ADDRESS;
	EEP_ASSERT_RETERR((addr < EEP_BASE_ADDRESS + EEP_SIZE),EEP_ERASE_API_ID,EEP_E_PARAM_ADDRESS);
	EEP_ASSERT_RETERR((addr+Length)<(EEP_BASE_ADDRESS + EEP_SIZE),EEP_ERASE_API_ID,EEP_E_PARAM_LENGTH);

	return E_OK;
}

Std_ReturnType Eep_Write(Eep_AddressType EepromAddress, const uint8* DataBufferPtr, Eep_LengthType Length)
{
	uint32 addr;

	EEP_ASSERT_RETERR(eep_var.Status != MEMIF_UNINIT,EEP_WRITE_API_ID,EEP_E_UNINIT);
	EEP_ASSERT_RETERR(eep_var.Status == MEMIF_IDLE, EEP_WRITE_API_ID,EEP_E_BUSY);
	EEP_ASSERT_RETERR(DataBufferPtr != NULL_PTR, EEP_WRITE_API_ID,EEP_E_PARAM_DATA);
	addr = EepromAddress + EEP_BASE_ADDRESS;
	EEP_ASSERT_RETERR((addr < (EEP_BASE_ADDRESS + EEP_SIZE)),EEP_WRITE_API_ID,EEP_E_PARAM_ADDRESS);
	EEP_ASSERT_RETERR((addr+Length)<(EEP_BASE_ADDRESS + EEP_SIZE),EEP_WRITE_API_ID,EEP_E_PARAM_LENGTH);

	eep_var.Status = MEMIF_BUSY_INTERNAL;
	eep_var.Cmd.DataAddress = (void*)DataBufferPtr;
	eep_var.Cmd.LeftSize = Length;
	eep_var.Cmd.StartAddress = addr;
	eep_var.Cmd.pending = EEP_CMD_WRITE;
	eep_var.JobResult = MEMIF_JOB_PENDING;
	eep_var.Status = MEMIF_BUSY;
	return E_OK;
}

void Eep_Cancel(void)
{
	EEP_ASSERT_RETVOID(eep_var.Status != MEMIF_UNINIT,EEP_CANCEL_API_ID,EEP_E_UNINIT);
}

MemIf_StatusType Eep_GetStatus(void)
{
	return eep_var.Status;
}

MemIf_JobResultType Eep_GetJobResult(void)
{
	return eep_var.JobResult;
}

void Eep_MainFunction(void)
{
	EEP_ASSERT_RETVOID(eep_var.Status != MEMIF_UNINIT,EEP_WRITE_API_ID,EEP_E_UNINIT);

	Spi_SeqResultType spi_result;
	Std_ReturnType err;
	uint32 i32;
	uint16 i16;
	uint8 i8;
	if(Spi_GetSequenceResult(SequenceIdEepReadStatus) == SPI_SEQ_PENDING
			|| Spi_GetSequenceResult(SequenceIdEepReadData) ==  SPI_SEQ_PENDING
					|| Spi_GetSequenceResult(SequenceIdEepWriteData) == SPI_SEQ_PENDING)
					{
						return;
					}

	switch(eep_var.State)
	{
	case Eep_STATE_IDLE:
		if(eep_var.Cmd.pending == EEP_CMD_IDLE)
		{
		}else if(eep_var.Cmd.pending == EEP_CMD_WRITE)
		{
			eep_tx_buffer[0] = CMD_RDSR;
			err = Spi_SetupEB(ChannelIdEepReadStatusCmdRDSR,eep_tx_buffer,eep_rx_buffer,1);
			if( err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}
			err = Spi_SetupEB(ChannelIdEepReadStatusRcvRDSR,NULL_PTR,&eep_rx_buffer[1],1);
			if(err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}
			err = Spi_AsyncTransmit(SequenceIdEepReadStatus);
			if(err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}else
			{
				eep_var.State = Eep_STATE_ReadStatusWait;
				break;
			}
		}else
		{
			eep_tx_buffer[0] = CMD_READ;
			BUFFER_RD_CMD();
			err = Spi_SetupEB(ChannelIdEepReadDataCmdREAD,eep_tx_buffer,eep_rx_buffer,1);
			if( err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}
			BUFFER_RD_ADDR();
			err = Spi_SetupEB(ChannelIdEepReadDataAddrREAD,&eep_tx_buffer[1],&eep_rx_buffer[1],BUFFER_ADDR_SIZE);
			if(err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}
			/* Align to block read size */
			i32 = eep_var.Cmd.StartAddress - EEP_BASE_ADDRESS;
			if( eep_var.Mode == MEMIF_MODE_SLOW )
			{
				i32 = (i32 / EEP_NORMAL_READ_BLOCK_SIZE + 1) * EEP_NORMAL_READ_BLOCK_SIZE
						- (eep_var.Cmd.StartAddress - EEP_BASE_ADDRESS);
			}else
			{
				i32 = (i32 / EEP_FAST_READ_BLOCK_SIZE + 1) * EEP_FAST_READ_BLOCK_SIZE
						- (eep_var.Cmd.StartAddress - EEP_BASE_ADDRESS);
			}
			eep_var.Cmd.ThisSize = (uint16)__minu(i32,(uint32)eep_var.Cmd.LeftSize);
			err = Spi_SetupEB(ChannelIdEepReadDataRcvRead,NULL_PTR,&eep_rx_buffer[4],eep_var.Cmd.ThisSize);
			if(err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}
			err = Spi_AsyncTransmit(SequenceIdEepReadData);
			if(err == E_NOT_OK)
			{
				eep_var.State = Eep_STATE_Error;
				break;
			}else
			{
				eep_var.State = Eep_STATE_ReadDataWait;
				break;
			}
		}
		break;
	case Eep_STATE_ReadStatusWait:
		spi_result = Spi_GetSequenceResult(SequenceIdEepReadStatus);
		if(spi_result == SPI_SEQ_PENDING)
		{
			break;
		}else if(spi_result == SPI_SEQ_FAILED || spi_result == SPI_SEQ_CANCELLED)
		{
			eep_var.State = Eep_STATE_Error;
			break;
		}else
		{
			i8 = (uint8)eep_rx_buffer[1];
			if(!(i8 & MASK_WIP) && (eep_var.Cmd.pending == EEP_CMD_WRITE) )
			{/* Write available */
				eep_tx_buffer[0] = CMD_WREN;
				err = Spi_SetupEB(ChannelIdEepWriteDataCmdWREN,eep_tx_buffer,eep_rx_buffer,1);
				if( err == E_NOT_OK)
				{
					eep_var.State = Eep_STATE_Error;
					break;
				}
				eep_tx_buffer[1] = CMD_WRITE;
				BUFFER_WR_CMD();
				err = Spi_SetupEB(ChannelIdEepWriteDataCmdWRITE,&eep_tx_buffer[1],&eep_rx_buffer[1],1);
				if(err == E_NOT_OK)
				{
					eep_var.State = Eep_STATE_Error;
					break;
				}
				BUFFER_WR_ADDR();
				err = Spi_SetupEB(ChannelIdEepWriteDataAddrWRITE,&eep_tx_buffer[2],&eep_rx_buffer[2],BUFFER_ADDR_SIZE);
				if(err == E_NOT_OK)
				{
					eep_var.State = Eep_STATE_Error;
					break;
				}
				i32 = eep_var.Cmd.StartAddress - EEP_BASE_ADDRESS;
				if( eep_var.Mode == MEMIF_MODE_SLOW )
				{
					i32 = (i32 / EEP_NORMAL_WRITE_BLOCK_SIZE + 1) * EEP_NORMAL_WRITE_BLOCK_SIZE
							- (eep_var.Cmd.StartAddress - EEP_BASE_ADDRESS);
				}else
				{
					i32 = (i32 / EEP_FAST_READ_BLOCK_SIZE + 1) * EEP_FAST_READ_BLOCK_SIZE
							- (eep_var.Cmd.StartAddress - EEP_BASE_ADDRESS);
				}
				eep_var.Cmd.ThisSize = (uint16)__minu(i32,(uint32)eep_var.Cmd.LeftSize);
				for(i16=5; i16 <eep_var.Cmd.ThisSize+5; i16++)
				{
					eep_tx_buffer[i16] = *((uint8*)eep_var.Cmd.DataAddress);
					eep_var.Cmd.DataAddress = (uint8*)eep_var.Cmd.DataAddress + 1;
				}
				err = Spi_SetupEB(ChannelIdEepWriteDataDataWRITE,&eep_tx_buffer[5],&eep_rx_buffer[5],eep_var.Cmd.ThisSize);
				if(err == E_NOT_OK)
				{
					eep_var.State = Eep_STATE_Error;
					break;
				}
				err = Spi_AsyncTransmit(SequenceIdEepWriteData);
				if(err == E_NOT_OK)
				{
					eep_var.State = Eep_STATE_Error;
					break;
				}else
				{
					eep_var.State = Eep_STATE_WriteDataWait;
					break;
				}
			}else
			{
				eep_var.State = Eep_STATE_IDLE;
			}
		}
		break;
	case Eep_STATE_ReadDataWait:
		spi_result = Spi_GetSequenceResult(SequenceIdEepReadData);
		if(spi_result == SPI_SEQ_PENDING)
		{
			break;
		}else if(spi_result == SPI_SEQ_FAILED || spi_result == SPI_SEQ_CANCELLED)
		{
			eep_var.State = Eep_STATE_Error;
			break;
		}else
		{
			for(i16 = 4; i16 < eep_var.Cmd.ThisSize + 4; i16++ )
			{
			  *(uint8*)eep_var.Cmd.DataAddress = (uint8)eep_rx_buffer[i16];
			  eep_var.Cmd.DataAddress = (uint8*)eep_var.Cmd.DataAddress + 1;
			}
			eep_var.Cmd.LeftSize = eep_var.Cmd.LeftSize - eep_var.Cmd.ThisSize;
			eep_var.Cmd.StartAddress += eep_var.Cmd.ThisSize;
			if( eep_var.Cmd.LeftSize <= 0 )
			{
				eep_var.State = Eep_STATE_JobDone;
			}else
			{
				eep_var.State = Eep_STATE_IDLE;
			}
		}
		break;
	case Eep_STATE_WriteDataWait:
		spi_result = Spi_GetSequenceResult(SequenceIdEepWriteData);
		if(spi_result == SPI_SEQ_PENDING)
		{
			break;
		}else if(spi_result == SPI_SEQ_FAILED || spi_result == SPI_SEQ_CANCELLED)
		{
			eep_var.State = Eep_STATE_Error;
			break;
		}else
		{
			eep_var.State = Eep_STATE_WriteDataWait2;
		    Gpt_StartTimer(4u, GPT_TIMER_VALUE(EEP_WRITE_WAIT_MS));
		}
		break;
	case Eep_STATE_WriteDataWait2:
		{
			if( Gpt_GetTimeRemaining(4u) > 0)
				break;
			Gpt_StopTimer(4u);
			eep_var.Cmd.LeftSize = eep_var.Cmd.LeftSize - eep_var.Cmd.ThisSize;
			eep_var.Cmd.StartAddress += eep_var.Cmd.ThisSize;
			if( eep_var.Cmd.LeftSize <= 0 )
				eep_var.State = Eep_STATE_JobDone;
			else
				eep_var.State = Eep_STATE_IDLE;
		}
		break;
	case Eep_STATE_Error:
		eep_var.Cmd.pending = EEP_CMD_IDLE;
		eep_var.JobResult = MEMIF_JOB_FAILED;
		EepJobErrorNotification();
		eep_var.Status = MEMIF_IDLE;
		eep_var.State = Eep_STATE_IDLE;
		break;
	case Eep_STATE_JobDone:
		eep_var.Cmd.pending = EEP_CMD_IDLE;
		eep_var.JobResult = MEMIF_JOB_OK;
		EepJobEndNotification();
		eep_var.Status = MEMIF_IDLE;
		eep_var.State = Eep_STATE_IDLE;
		break;
	default:
		break;
	}
}
#define EEP_STOP_SEC_CODE
#include "MemMap.h"


