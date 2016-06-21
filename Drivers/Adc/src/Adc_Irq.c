/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc_Irq.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:14:54MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Adc_Irq.c  $
 *  Revision 1.1 2010/05/19 14:14:54MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Adc/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Adc.h"
#include "Adc_Priv.h"


#define ADC_START_SEC_CODE
#include "MemMap.h"


#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_DMA_SRC0) __enable_ Adc_Irq0(void)
#else
void Adc_Irq0func(void)
#endif
{
	Adc_InterruptHandler(0); /* Node 0 ContinueTrigger Interrupt source */
}

#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_DMA_SRC1) __enable_ Adc_Irq1(void)
#else
void Adc_Irq1func(void)
#endif
{
	Adc_InterruptHandler(1); /* Node 0 SwTrigger Interrupt source */
}

#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_DMA_SRC2) __enable_ Adc_Irq2(void)
#else
void Adc_Irq2func(void)
#endif
{
	Adc_InterruptHandler(2); /* Node 1 ContinueTrigger Interrupt source */
}

#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_DMA_SRC3) __enable_ Adc_Irq3(void)
#else
void Adc_Irq3func(void)
#endif
{
	Adc_InterruptHandler(3);  /* Node 1 SwTrigger Interrupt source */
}

#define ADC_STOP_SEC_CODE
#include "MemMap.h"
