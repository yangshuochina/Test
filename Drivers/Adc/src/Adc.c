/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:16:35MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Adc.c  $
 *  Revision 1.3 2011/02/02 09:16:35MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/16 13:40:28MESZ Cotor Vlad RGB (COTORV) 
 *  Update ADC due to AdcConversionMode
 *  Revision 1.1 2010/05/19 14:14:50MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Adc/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Adc.h"
#include "Adc_Priv.h"
#include "Os.h"

#define ADC_START_SEC_VAR_32BIT
#include "MemMap.h"

const Adc_ConfigType *adc_cfg;
Adc_RamInfoType Adc_RamInfo[ADC_NUM_OF_GROUPS_CONFIGURED];
Adc_ConvSrcInfoType Adc_ConvSrcInfo[4];

#define ADC_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define ADC_START_SEC_CODE
#include "MemMap.h"


static void Adc_NodeInit(const Adc_NodeType *adc_node)
{
	int i;
	uint32 *inpcr;
	uint8 node = adc_node->AdcHwNodeId;
	/* ADC Kernel(Node) setup - Below set of register available in each kernel */
	/* Startup and Post calibration enabled - StartUp calibration may take few ADC cycles and Conversion should not be started,
	 * expected before we finish initialization few cycles are over */
	REG(ADC0_GLOBCFG, node)->U = BITPOS(5);
	/* ADC Kernel Frequency setup and arbiter configuration with enabling of Analog module */
	REG(ADC0_GLOBCTR, node)->U = (ADC_PRESCALAR_VALUE | (ADC_ANALOG_ON << 8) | (ARB_SLOT_ENABLE << 10));
	/* Setup of conversion Request source operating mode - Cancel-Inject-Repeat mode with High Priority for SW Trigger and Low Priority for Continuous scan */
	REG(ADC0_RSPR0, node)->U = (((ADC_REQ_SRC_CANCEL_INJECT_REPEAT_MODE | ADC_REQ_SRC_PRIO_HIGH) << 4) | ((ADC_REQ_SRC_CANCEL_INJECT_REPEAT_MODE | ADC_REQ_SRC_PRIO_LOW) << 12));
	/* Slot 1 and 3 are enabled for SW trigger and Continuous Trigger respectively */
	REG(ADC0_ASENR, node)->U = 0xA;
	/* Setup of SW trigger conversion - Uses Conversion Request Mode 1 */
	REG(ADC0_CRMR1, node)->U = ((ADC_ENABLE_CONV) | (ADC_CLR_PENDING));
	/* Setup of Continuous trigger conversion - Uses Conversion Request Mode 3 */
	REG(ADC0_CRMR3, node)->U = ((ADC_ENABLE_CONV) | (ADC_CLR_PENDING));/*Continuous Trigger conversion  - clear all pending request and enable the Conversion source  */
	/* REG(ADC0_EVNPR, node)->U = ((ADC_SR_LINE0 << 4) | (ADC_SR_LINE1 << 12));ADC SR0 and SR1 are used for High and Low Priority conversion request sources */

	/*Channel Profile Initialization */
	inpcr = ((uint32 *)(&ADC0_INPCR0.U)) + (node * 0x100);
	for(i=0; i<adc_node->number_of_profile; i++)
	{
		/* Write the configured profiles in the INPCR */
		*inpcr = (adc_node->AdcChannelProfileDefinition[i].AdcChannelSampTime |(adc_node->AdcChannelProfileDefinition[i].AdcChannelResolution<<8));
		inpcr++;
	}
	for(i=0; i<16; i++)
	{
		/* Write the selected profiles(one among the four) for each channel */
		CH_REG(ADC0_CHCTR0, node, i)->U = adc_node->AdcProfileIndex[i] << 10;
	}
}

static void Adc_GroupInit(const Adc_GroupDefType *group, uint8 id)
{
	uint32 adc_local_channelset;
	uint8 adc_ch=0, count=0;
	uint8 first_ch, node, dma_sr_line;

	node = group->AdcHwNodeId;
	adc_local_channelset = group->AdcChannelSet;
	/*Get the first adc channel(highest numbered channel) in the group, for all the following channels this same channels' result register has to be programmed */
	first_ch = (32 - __clz(adc_local_channelset)) - 1;
	Adc_RamInfo[id].first_ch = first_ch;

	/* For all the channels in the group, program the first channel result register as their result register too. Implies all the results of channels in the group will go same result register */
	while(adc_local_channelset) {
		adc_ch = (32 -__clz(adc_local_channelset)) - 1;
		CH_REG(ADC0_CHCTR0, node, adc_ch)->U |= (first_ch << 12);
		adc_local_channelset &= ~(1u << (adc_ch));
		count++;
	}

	/* Record the number of channels for each group */
	Adc_RamInfo[id].number_of_channel = count;
	/* Keep the result buffer address where DMA must store the result for each group */
	Adc_RamInfo[id].AdcResultBufferPointer = group->AdcResultBufferPointer;

	/* For Continuous trigger group, do the complete DMA configuration */
	if(group->AdcConversionMode == ADC_CONV_MODE_CONTINUOUS)
	{
		/* For Continuous Trigger, DMA channel 1 and 3 are used for Node 0 and 1 respectively */
		dma_sr_line = 1 + (node * 2);
		Adc_DmaInit(group->DmaChannel, count);
		Adc_SetDmaSrcAddr(group->DmaChannel, CH_REG(ADC0_RESR0, node, first_ch));
		Adc_SetDmaCntr(group->DmaChannel, count);

		/* Load the conversion request register with the channel to be converted continuously */
		REG(ADC0_CRCR3, node)->U = group->AdcChannelSet;
	}
	else
	{
		/* For SW trigger group, do only the common DMA configuration, group specific configuration shall be done during StartGroup conversion request */
		Adc_DmaInit(group->DmaChannel, 0);
		/* For SWtrigger, DMA channel 0 and 2 are used in Node 0 and 1 respectively */
		dma_sr_line = 0 + (node * 2);
	}

	/* ADC <-> DMA request line configuration - Enable the DMA trigger in Result Control Register */
	if(first_ch < 8)
	{
		REG(ADC0_RNPR0, node)->U |= (dma_sr_line << (first_ch * 4));
		CH_REG(ADC0_RCR0, node, first_ch)->U = 0x10;
	}
	else
	{
		REG(ADC0_RNPR8, node)->U |= (dma_sr_line << ((first_ch-8) * 4));
		CH_REG(ADC0_RCR0, node, first_ch)->U = 0x10;
	}
}


void Adc_Init(const Adc_ConfigType *cfg)
{
	uint8_least i;

	ADC_ASSERT_RETVOID(cfg != NULL_PTR, ADC_INIT_API_ID, ADC_E_PARAM_CONFIG);
	ADC_ASSERT_RETVOID(adc_cfg == NULL_PTR, ADC_INIT_API_ID, ADC_E_ALREADY_INITIALIZED);

	adc_cfg = cfg;
	memset(Adc_RamInfo, 0, sizeof(Adc_RamInfo));
	memset(Adc_ConvSrcInfo, 0,  sizeof(Adc_ConvSrcInfo));

	for(i=0; i < adc_cfg->number_of_nodes; i++)
	{
		Adc_NodeInit(adc_cfg->AdcNodeDefinition+i);
	}

	/* Enable all the regions to be accessed by DMA, outside the loop as it is enough to done once */
	ts_endinit_clear();
	DMA_ME0AENR.U = 0xFFFFFFFFU;
	DMA_ME0ARR.U = 0xFFFFFFFFU;
	ts_endinit_set();

	for(i=0; i < adc_cfg->number_of_groups; i++)
	{
		Adc_GroupInit(adc_cfg->AdcGroupDefinition+i, (uint8)i);
	}

	Adc_SetAsyncMode();
}


#if defined(ADC_DEINIT_API) && (ADC_DEINIT_API == STD_ON)

void Adc_DeInit(void)
{
	ADC_ASSERT_RETVOID(adc_cfg != NULL_PTR, ADC_DEINIT_API_ID, ADC_E_UNINIT);
	ADC_ASSERT_RETVOID((Adc_ConvSrcInfo[0].Adc_SwTrigPending == 0), ADC_DEINIT_API_ID, ADC_E_BUSY);
	ADC_ASSERT_RETVOID((Adc_ConvSrcInfo[2].Adc_SwTrigPending == 0), ADC_DEINIT_API_ID, ADC_E_BUSY);

	REG(ADC0_ASENR, 0)->U = 0x0; /*No Conversion source participates in Arbitration, So no activity inside ADC */

	REG(ADC0_CRCR1, 0)->U = 0;
	REG(ADC0_CRCR3, 0)->U = 0;
	REG(ADC0_CRCR1, 1)->U = 0;
	REG(ADC0_CRCR3, 1)->U = 0;
	adc_cfg = NULL_PTR;
}

#endif


#if defined(ADC_ENABLE_START_STOP_GROUP_API) && (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)

void Adc_StartGroupConversion(Adc_GroupType group)
{
	const Adc_GroupDefType *adc_group;
	uint8 node, conv_src_index;

	ADC_ASSERT_RETVOID(adc_cfg != NULL_PTR, ADC_STARTGROUPCONVERSION_API_ID, ADC_E_UNINIT);

	adc_group = adc_cfg->AdcGroupDefinition + group;
	node = adc_group->AdcHwNodeId;
	conv_src_index = adc_group->AdcConversionMode + (node * 2);

	ADC_ASSERT_RETVOID((adc_cfg->number_of_groups > group), ADC_STARTGROUPCONVERSION_API_ID, ADC_E_PARAM_GROUP);
	ADC_ASSERT_RETVOID(((Adc_RamInfo[group].AdcGroupStatus == ADC_IDLE) || (Adc_RamInfo[group].AdcGroupStatus == ADC_STREAM_COMPLETED)), ADC_STARTGROUPCONVERSION_API_ID, ADC_E_BUSY);
	ADC_ASSERT_RETVOID(((Adc_ConvSrcInfo[conv_src_index].Adc_SwTrigPending & (0x1u << group)) != (0x1u << group)), ADC_STARTGROUPCONVERSION_API_ID, ADC_E_BUSY);


	if((Adc_RamInfo[group].AdcGroupStatus == ADC_IDLE) || (Adc_RamInfo[group].AdcGroupStatus == ADC_STREAM_COMPLETED)) {
		/* a start is possible */
		if(adc_group->AdcConversionMode == ADC_CONV_MODE_CONTINUOUS) {
			/* Reset the result buffer location - DMA update*/
			Adc_ResetDma(adc_group->DmaChannel);
			/* Destination set again because the result buffer can be changed via "Adc_SetupResultBuffer"*/
			Adc_SetDmaDestAddr(adc_group->DmaChannel, Adc_RamInfo[group].AdcResultBufferPointer);
			Adc_SetDmaRequest(adc_group->DmaChannel);

			Adc_ConvSrcInfo[conv_src_index].Adc_SwTrigRunning = (1u << group);

			/* Start the conversion */
			REG(ADC0_CRMR3, node)->U |= (0x1u << 9);
		}
		else {
			/*Critical Sequence - As in the Interrupt Mode, the Adc_SwTrigPending could be modified, protect the seqeunce */
			SuspendAllInterrupts();
			if(Adc_ConvSrcInfo[conv_src_index].Adc_SwTrigPending == 0) {
				/* If there is no pending conversion, set the DMA interrupt manually and in the ISR start the conversion */
				Adc_TrigDma(adc_group->DmaChannel);
			}
			Adc_ConvSrcInfo[conv_src_index].Adc_SwTrigPending |= (1u << group);
			ResumeAllInterrupts();
		}
		Adc_RamInfo[group].AdcGroupStatus = ADC_BUSY;
	}
}
#endif


#if defined(ADC_ENABLE_START_STOP_GROUP_API) && (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)

void Adc_StopGroupConversion(Adc_GroupType group)
{
	const Adc_GroupDefType *adc_group;
	uint8 node, conv_source;

	ADC_ASSERT_RETVOID(adc_cfg != NULL_PTR, ADC_STOPGROUPCONVERSION_API_ID, ADC_E_UNINIT);
	ADC_ASSERT_RETVOID((adc_cfg->number_of_groups > group), ADC_STOPGROUPCONVERSION_API_ID, ADC_E_PARAM_GROUP);
	ADC_ASSERT_RETVOID((Adc_RamInfo[group].AdcGroupStatus != ADC_IDLE), ADC_STOPGROUPCONVERSION_API_ID, ADC_E_IDLE);


	adc_group = adc_cfg->AdcGroupDefinition+group;
	node = adc_group->AdcHwNodeId;
	conv_source = adc_group->AdcConversionMode + (node * 2);

	SuspendAllInterrupts();
	if(Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning == (0x1u << group))
	{
		Adc_ResetDma(adc_group->DmaChannel); /* So no more result transfer */
		if(adc_group->AdcConversionMode == ADC_CONV_MODE_CONTINUOUS)
		{
			REG(ADC0_CRMR3, node)->U |= (0x1u << 8);
		}
		else
		{
			REG(ADC0_CRMR1, node)->U |= (0x1u << 8);

		}
		Adc_ConvSrcInfo[conv_source].Adc_SwTrigPending &= ~(Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning);
		Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning = 0;
		if(Adc_ConvSrcInfo[conv_source].Adc_SwTrigPending != 0)
		{
			/* If there is pending SW trigger request, set the DMA interrupt manually and in the ISR start the conversion */
			Adc_TrigDma(adc_group->DmaChannel);
		}
	}
	else
	{
		/* If the passed group is not a running SW trigger, then remove the conversion request from pending*/
		Adc_ConvSrcInfo[conv_source].Adc_SwTrigPending &= ~(0x1u << group);
	}
	Adc_RamInfo[group].AdcGroupStatus = ADC_IDLE;
	Adc_ConvSrcInfo[conv_source].Adc_GroupNotificationEnable &= ~(0x1u << group);
	ResumeAllInterrupts();
}
#endif


#if defined(ADC_GRP_NOTIF_CAPABILITY) && (ADC_GRP_NOTIF_CAPABILITY == STD_ON)

void ADC_EnableGroupNotification(Adc_GroupType group)
{
	const Adc_GroupDefType *adc_group;
	uint8 node, conv_src_index;

	ADC_ASSERT_RETVOID(adc_cfg != NULL_PTR, ADC_ENABLEGROUPNOTIFICATION_API_ID, ADC_E_UNINIT);
	adc_group = adc_cfg->AdcGroupDefinition + group;
	ADC_ASSERT_RETVOID((adc_cfg->number_of_groups > group), ADC_ENABLEGROUPNOTIFICATION_API_ID, ADC_E_PARAM_GROUP);
	ADC_ASSERT_RETVOID((adc_group->notification != NULL), ADC_ENABLEGROUPNOTIFICATION_API_ID, ADC_E_NOTIF_CAPABILITY);

	node = adc_group->AdcHwNodeId;
	conv_src_index = adc_group->AdcConversionMode + (node * 2);
	BITFIELD32_SETBIT(Adc_ConvSrcInfo[conv_src_index].Adc_GroupNotificationEnable, group);
}
#endif

#if defined(ADC_GRP_NOTIF_CAPABILITY) && (ADC_GRP_NOTIF_CAPABILITY == STD_ON)

void ADC_DisableGroupNotification(Adc_GroupType group)
{
	const Adc_GroupDefType *adc_group;
	uint8 node,conv_src_index;

	ADC_ASSERT_RETVOID(adc_cfg != NULL_PTR, ADC_DISABLEGROUPNOTIFICATION_API_ID, ADC_E_UNINIT);
	adc_group = adc_cfg->AdcGroupDefinition + group;
	ADC_ASSERT_RETVOID((adc_cfg->number_of_groups > group), ADC_DISABLEGROUPNOTIFICATION_API_ID, ADC_E_PARAM_GROUP);
	ADC_ASSERT_RETVOID((adc_group->notification != NULL), ADC_DISABLEGROUPNOTIFICATION_API_ID, ADC_E_NOTIF_CAPABILITY);

	node = adc_group->AdcHwNodeId;
	conv_src_index = adc_group->AdcConversionMode + (node * 2);
	BITFIELD32_CLRBIT(Adc_ConvSrcInfo[conv_src_index].Adc_GroupNotificationEnable, group);
}
#endif


#if defined(ADC_READ_GROUP_API) && (ADC_READ_GROUP_API == STD_ON)

Std_ReturnType Adc_ReadGroup(Adc_GroupType group, Adc_ValueGroupType * DataBufferPtr)
{
	int i;
	const Adc_GroupDefType *adc_group;

	ADC_ASSERT_RETERR(adc_cfg != NULL_PTR, ADC_READGROUP_API_ID, ADC_E_UNINIT);
	ADC_ASSERT_RETERR(adc_cfg->number_of_groups > group, ADC_READGROUP_API_ID, ADC_E_PARAM_GROUP);
	ADC_ASSERT_RETERR(Adc_RamInfo[group].results_available, ADC_READGROUP_API_ID, ADC_E_IDLE);

	adc_group = adc_cfg->AdcGroupDefinition + group;

	/* copy the conversion results */
	for(i=(Adc_RamInfo[group].number_of_channel-1); i >= 0 ; i--) {
		*DataBufferPtr = Adc_RamInfo[group].AdcResultBufferPointer[i];
		DataBufferPtr++;
	}

	/* start a new conversion if the mode is continuous, for all other modes */
	/* the status is changed to IDLE */
	if(adc_group->AdcConversionMode == ADC_CONV_MODE_CONTINUOUS) {
		if(Adc_RamInfo[group].AdcGroupStatus == ADC_STREAM_COMPLETED) {
			Adc_RamInfo[group].AdcGroupStatus = ADC_BUSY;
			Adc_SetDmaDestAddr(adc_group->DmaChannel, Adc_RamInfo[group].AdcResultBufferPointer);
			Adc_SetDmaRequest(adc_group->DmaChannel);
			REG(ADC0_CRMR3, adc_group->AdcHwNodeId)->U |= (0x1u << 9);
			return E_OK;
		}
	}
	else {
		Adc_RamInfo[group].AdcGroupStatus = ADC_IDLE;
	}

	return E_OK;
}

#endif


Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType group, Adc_ValueGroupType * DataBufferPtr)
{
	ADC_ASSERT_RETERR(adc_cfg != NULL_PTR, ADC_SETUPRESULTBUFFER_API_ID, ADC_E_UNINIT);
	ADC_ASSERT_RETERR((adc_cfg->number_of_groups > group), ADC_SETUPRESULTBUFFER_API_ID, ADC_E_PARAM_GROUP);
	Adc_RamInfo[group].AdcResultBufferPointer = DataBufferPtr;
	return E_OK;
}

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType group)
{
	ADC_ASSERT_RETERR(adc_cfg != NULL_PTR, ADC_GETGROUPSTATUS_API_ID, ADC_E_UNINIT);
	ADC_ASSERT_RETERR((adc_cfg->number_of_groups > group), ADC_GETGROUPSTATUS_API_ID, ADC_E_PARAM_GROUP);

	return Adc_RamInfo[group].AdcGroupStatus;
}

void Adc_InterruptHandler(uint8 conv_source)
{
	uint8 group, node, DmaChannel;

	if(Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning)
	{
		group = (32 - __clz(Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning))-1;
		Adc_RamInfo[group].AdcGroupStatus = ADC_STREAM_COMPLETED;
		Adc_RamInfo[group].results_available = 1;
		if(Adc_ConvSrcInfo[conv_source].Adc_GroupNotificationEnable & (0x1u << group))
		{
			adc_cfg->AdcGroupDefinition[group].notification(group);
		}
		if((conv_source%2) == 0) /* Clear only SW trigger group */
		{
			BITFIELD32_CLRBIT(Adc_ConvSrcInfo[conv_source].Adc_SwTrigPending, group);
		}
	}

	if((conv_source == 0) || (conv_source == 2)) /* If it is SW trigger conversion source */
	{
		if(Adc_ConvSrcInfo[conv_source].Adc_SwTrigPending)
		{
			group = (32 - __clz(Adc_ConvSrcInfo[conv_source].Adc_SwTrigPending)) - 1;
			node = adc_cfg->AdcGroupDefinition[group].AdcHwNodeId;
			Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning = 0x1u << group;
			DmaChannel = adc_cfg->AdcGroupDefinition[group].DmaChannel;
			Adc_ResetDma(DmaChannel);

				/* Load the corresponding group's channel set value */
			REG(ADC0_CRCR1, node)->U = adc_cfg->AdcGroupDefinition[group].AdcChannelSet;

			Adc_SetDmaSrcAddr(DmaChannel, CH_REG(ADC0_RESR0, node, Adc_RamInfo[group].first_ch));
			Adc_SetDmaDestAddr(DmaChannel, Adc_RamInfo[group].AdcResultBufferPointer);
			Adc_SetDmaCntr(DmaChannel, Adc_RamInfo[group].number_of_channel);
			Adc_SetDmaRequest(DmaChannel);
			REG(ADC0_CRMR1, node)->U |= (0x1u << 9);
			Adc_RamInfo[group].AdcGroupStatus = ADC_BUSY;
		}
		else
		{
			Adc_ConvSrcInfo[conv_source].Adc_SwTrigRunning = 0;
		}
	}
}


#define ADC_STOP_SEC_CODE
#include "MemMap.h"
