/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Chipselect.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:22:00MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi_Chipselect.c  $
 *  Revision 1.1 2010/05/19 14:22:00MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Spi.h"
#include "Spi_Priv.h"


#define SPI_START_SEC_CODE
#include "MemMap.h"


void Spi_SetCS(const Spi_ConfigJobType *job)
{
	uint8 port;
	uint8 pin;

	port = (uint8)(job->cs >> 8);
	pin = (uint8)(job->cs);

	ts_wait(job->delay_clk_cs);

	if(job->cs_is_low_active) {
		ts_clrpin(port, pin);
	}
	else {
		ts_setpin(port, pin);
	}
}


void Spi_ReleaseCS(const Spi_ConfigJobType *job)
{
	uint8 port;
	uint8 pin;

	port = (uint8)(job->cs >> 8);
	pin = (uint8)(job->cs);

	ts_wait(job->delay_clk_cs);

	if(job->cs_is_low_active) {
		ts_setpin(port, pin);
	}
	else {
		ts_clrpin(port, pin);
	}
}

#define SPI_STOP_SEC_CODE
#include "MemMap.h"
