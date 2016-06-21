/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc_Dma.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:42:01MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Adc_Dma.c  $
 *  Revision 1.2 2010/07/12 09:42:01MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:14:53MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Adc/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Adc.h"
#include "Adc_Priv.h"


#define DMAREG(reg, unit)  ((reg##_type *) ((volatile uint8 *)(&reg) + (0x20 * (unit))))

#define ADC_START_SEC_CODE
#include "MemMap.h"


void Adc_DmaInit(uint8 dma_channel, uint8 number_of_channels)
{
	uint16 prio=0;

	if(number_of_channels) /* Pass with non-zero value only for Continuous scan*/
	{
		/* prio = 0;  DMA is set with LOW Prio for Continuous trigger mode */
	}
	else
	{
		prio = 3u; /* DMA is set with HIGH Prio for SwTrigger mode */
	}
	/* 0x3 << 12 - Selects the SR line from ADC to trigger the DMA */
	/* 0x1 << 21 - Each move is an halfword data */
	/* pior << 30 - Sets the priority based on conv node*/
	DMAREG(DMA_CHCR00, dma_channel)->U = (0x3u << 12) | (0x1u << 21) | (prio << 30) | (number_of_channels);
	/* Source is always same result register, so no modification */
	/* Destination is incremented by 2bytes and results are stored till Tlength(end of queue) */
	DMAREG(DMA_ADRCR00, dma_channel)->U = 0xF080u; /* Destination offset used to increment the result location, upper 16bit dest addr bits are untouched */
	/*DMA node pointer(SRN) is selected and INT is enabled */
	DMAREG(DMA_CHICR00, dma_channel)->U = (dma_channel << 8) | (1u << 3);
}

void Adc_SetDmaSrcAddr(uint8 dma_channel, void volatile * const src_addr)
{
	DMAREG(DMA_SADR00, dma_channel)->U = (uint32)src_addr;
}

void Adc_SetDmaDestAddr(uint8 dma_channel, void volatile * const dst_addr)
{
	DMAREG(DMA_DADR00, dma_channel)->U = (uint32)dst_addr;
}

void Adc_SetDmaCntr(uint8 dma_channel, uint8 count)
{
	DMAREG(DMA_CHCR00, dma_channel)->B.TREL = count;
}

void Adc_SetDmaRequest(uint8 dma_channel)
{
	DMA_HTREQ.U = 1u << dma_channel;
}

void Adc_ResetDma(uint8 dma_channel)
{
	DMA_CHRSTR.U |= 1u << dma_channel;
}

uint8 Adc_GetDmaIntStatus(uint8 dma_channel)
{
	uint8 status;
	status = ((uint8)DMA_INTSR.U) & (1u << dma_channel);
	return status;
}

void Adc_TrigDma(uint8 dma_channel)
{
	*(((uint32*)&DMA_SRC0.U) - dma_channel) |= 0x8000u;
}
#define ADC_STOP_SEC_CODE
#include "MemMap.h"
