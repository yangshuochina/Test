/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Poll.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:22:06MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi_Poll.c  $
 *  Revision 1.1 2010/05/19 14:22:06MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Spi.h"
#include "Spi_Priv.h"



#define SPI_START_SEC_CODE
#include "MemMap.h"



#if defined(SPI_LEVEL_DELIVERED) && (SPI_LEVEL_DELIVERED == 2)

Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType mode)
{
	if(mode == SPI_POLLING_MODE) {
		if(Spi_GetStatus() != SPI_IDLE) return E_NOT_OK;

		/* disable interrupt */
		SSC0_RSRC.U = 0;
		SSC1_RSRC.U = 0;
	}
	else if(mode == SPI_INTERRUPT_MODE) {
		/* enable interrupt */
		SSC0_RSRC.U = SRPN_SSC0_RSRC | BITPOS(12);
		SSC1_RSRC.U = SRPN_SSC1_RSRC | BITPOS(12);
	}

	return E_OK;
}

#endif


void Spi_MainFunction_Handling(void)
{
	/* abort if the interrupt mode is enabled */
	if(SSC0_RSRC.B.SRE != 0) return;

	if(SSC0_RSRC.B.SRR != 0) {
		SSC0_RSRC.B.CLRR = 1;
		Spi_InterruptHandler(0);
	}

	if(SSC1_RSRC.B.SRR != 0) {
		SSC1_RSRC.B.CLRR = 1;
		Spi_InterruptHandler(1);
	}
}

#define SPI_STOP_SEC_CODE
#include "MemMap.h"

