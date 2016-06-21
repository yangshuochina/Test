/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc_Poll.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:42:13MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Adc_Poll.c  $
 *  Revision 1.2 2010/07/12 09:42:13MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:14:56MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Adc/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Adc.h"
#include "Adc_Priv.h"


#define ADC_START_SEC_CODE
#include "MemMap.h"


void Adc_SetAsyncMode(void) /* Use this interface only if Interrupt generation is supported */
{
	/* enable interrupt */
	DMA_SRC0.U = SRPN_DMA_SRC0 | BITPOS(12);
	DMA_SRC1.U = SRPN_DMA_SRC1 | BITPOS(12);
	DMA_SRC2.U = SRPN_DMA_SRC2 | BITPOS(12);
	DMA_SRC3.U = SRPN_DMA_SRC3 | BITPOS(12);
}



void Adc_SetPollingMode(void)
{
	/* enable interrupt */
	DMA_SRC0.B.SRE = 0;
	DMA_SRC1.B.SRE = 0;
	DMA_SRC2.B.SRE = 0;
	DMA_SRC3.B.SRE = 0;
}


void Adc_MainFunction_Handling(void) /* Use this interface only if the Polling mode is supported */
{
	/* abort if the interrupt mode is enabled */
	if((DMA_SRC0.B.SRE != 0) || (DMA_SRC1.B.SRE != 0) ||
	   (DMA_SRC2.B.SRE != 0) || (DMA_SRC3.B.SRE != 0)) {
		return;
	}

	if(DMA_SRC0.B.SRR != 0) {
		DMA_SRC0.B.CLRR = 1;
		Adc_InterruptHandler(0);
	}

	if(DMA_SRC1.B.SRR != 0) {
		DMA_SRC1.B.CLRR = 1;
		Adc_InterruptHandler(1);
	}

	if(DMA_SRC2.B.SRR != 0) {
		DMA_SRC2.B.CLRR = 1;
		Adc_InterruptHandler(2);
	}

	if(DMA_SRC3.B.SRR != 0) {
		DMA_SRC3.B.CLRR = 1;
		Adc_InterruptHandler(3);
	}
}
#define ADC_STOP_SEC_CODE
#include "MemMap.h"


