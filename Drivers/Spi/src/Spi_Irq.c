/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Irq.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:22:02MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi_Irq.c  $
 *  Revision 1.1 2010/05/19 14:22:02MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Spi.h"
#include "Spi_Priv.h"

#define SPI_START_SEC_CODE
#include "MemMap.h"


#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_SSC0_RSRC) __enable_ Spi_Irq0(void)
#else
void Spi_Irq0func(void)
#endif
{
	Spi_InterruptHandler(0);
}


#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_SSC1_RSRC) __enable_ Spi_Irq1(void)
#else
void Spi_Irq1func(void)
#endif
{
	Spi_InterruptHandler(1);
}

#define SPI_STOP_SEC_CODE
#include "MemMap.h"
