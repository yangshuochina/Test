/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:16:41MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Adc_Priv.h  $
 *  Revision 1.4 2011/02/02 09:16:41MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/07/16 13:40:50MESZ Cotor Vlad RGB (COTORV) 
 *  Update ADC due to AdcConversionMode
 *  Revision 1.2 2010/07/12 09:42:21MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:14:57MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Adc/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef ADC_PRIV_H_
#define ADC_PRIV_H_

#include "team-s.h"
#include "Adc_Cfg.h"


#if defined(ADC_DEV_ERROR_DETECT) && (ADC_DEV_ERROR_DETECT == STD_ON)
#  define ADC_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, ADC_MODULE_ID, ADC_INSTANCE_ID, api_id, error_id)
#  define ADC_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, ADC_MODULE_ID, ADC_INSTANCE_ID, api_id, error_id)
#  define ADC_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, ADC_MODULE_ID, ADC_INSTANCE_ID, api_id, error_id)
#else
#  define ADC_ASSERT_RETERR(cond, api_id, error_id)
#  define ADC_ASSERT_RETVOID(cond, api_id, error_id)
#  define ADC_ASSERT(cond, api_id, error_id)
#endif

#pragma nomisrac 19.10
#define REG(reg, unit)  ((reg##_type *) ((volatile uint8 *)(&reg) + (0x400 * (unit))))
#define CH_REG(reg, unit, ch)((reg##_type *) ((volatile uint8 *)(&reg) + ((0x400 * (unit))+ (ch*4))))
#define ADC_INT_CH(node, conv_src)	(((node) << 4) + ((conv_src) << 0))
#pragma nomisrac default


#define ADC_MAX_REQ_SOURCES		(4)

#define ADC_REQ_SRC_CANCEL_INJECT_REPEAT_MODE	(0x8u)
#define ADC_REQ_SRC_PRIO_HIGH					(0x3u)
#define ADC_REQ_SRC_PRIO_LOW					(0x2u)
#define ADC_ANALOG_ON							(0x3u)
#define ADC_ENABLE_CONV							(0x1u)
#define ADC_CLR_PENDING							(0x1u << 8)
#define ADC_SR_LINE0							(0u)
#define ADC_SR_LINE1							(1u)
#define ARB_SLOT_ENABLE							(1u)

typedef ADC0_CRMR1_type ADC0_CRMR3_type;
typedef ADC0_CRCR1_type ADC0_CRCR3_type;


/** This is the internal structure to the ADC driver where the necessary info is stored for each group and updated during runtime.
 *
 * The number of instance of this structure depends upon the total number of groups.
 */

typedef struct
{
	uint8 first_ch;									/* First adc channel in each group, necessary for DMA usage */
	uint8 number_of_channel;
	uint8 results_available;						/* set to 1 if a conversion was performed for this group */
	Adc_StatusType AdcGroupStatus;					/* Current status of the ADC Group */
	Adc_ValueGroupType * AdcResultBufferPointer;	/* Result pointer where results are stored */
}Adc_RamInfoType;

/**This is the internal structure to the ADC driver where the necessary info is stored for each conversion type.
 *
 * In each node, there two conversion type(SW and Continuous), So an array of size 4 will be created of this type.
 * Index 0 and 1 shall have information about SW trigger while 1 and 3 will have info about Continuous trigger.
 */

typedef struct
{
	BITFIELD32(Adc_SwTrigPending);	/* Any request for SW trigger is pending */
	uint32 Adc_SwTrigRunning;
	uint32 Adc_GroupNotificationEnable;		/*Enable or disable */
}Adc_ConvSrcInfoType;


#define ADC_START_SEC_CODE
#include "MemMap.h"

void Adc_DmaInit(uint8 dma_channel, uint8 number_of_channels);
void Adc_SetDmaSrcAddr(uint8 dma_channel, void volatile * const src_addr);
void Adc_SetDmaDestAddr(uint8 dma_channel, void volatile * const dst_addr);
void Adc_SetDmaCntr(uint8 dma_channel, uint8 count);
void Adc_SetDmaRequest(uint8 dma_channel);
void Adc_ResetDma(uint8 dma_channel);
void Adc_SetDmaRequest(uint8 dma_channel);
void Adc_TrigDma(uint8 dma_channel);

void Adc_SetAsyncMode(void);
void Adc_InterruptHandler(uint8 conv_source);

#define ADC_STOP_SEC_CODE
#include "MemMap.h"

#endif /* ADC_PRIV_H_ */


