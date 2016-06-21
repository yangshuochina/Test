/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc_PBcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/12/10 16:20:31MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Adc_PBcfg.c  $
 *  Revision 1.4 2010/12/10 16:20:31MEZ Cotor Vlad RGB (COTORV) 
 *  Update of ADC Driver due to rough resolution
 *  Revision 1.3 2010/08/19 16:57:59MESZ Opel Martin RGB (OPELM) 
 *  - improved MemMap sections
 *  Revision 1.2 2010/07/30 16:30:20MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification change
 *  Revision 1.1 2010/05/20 11:28:59MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#include "Adc.h"


#define DMA_CHANNEL_0   (0)
#define DMA_CHANNEL_1   (1)
#define DMA_CHANNEL_2   (2)
#define DMA_CHANNEL_3   (3)

#define ADC_START_SEC_VAR_16BIT
#include "MemMap.h"
uint16 AdcGroup0_Res[16];
uint16 AdcGroup1_Res[16];
#define ADC_STOP_SEC_VAR_16BIT
#include "MemMap.h"


#define ADC_START_SEC_VAR_8BIT
#include "MemMap.h"
uint8 AdcProfileIndex[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
#define ADC_STOP_SEC_VAR_8BIT
#include "MemMap.h"


#define ADC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"

const Adc_ChannelProfileType AdcChannelProfile[4] = {
         { 2, ADC_RESOLUTION_12BIT },
         { 2, ADC_RESOLUTION_12BIT },
         { 4, ADC_RESOLUTION_12BIT },
         { 8, ADC_RESOLUTION_12BIT },
};

const Adc_GroupDefType	AdcChannelGroup[]= {
      {
            BITPOS(0) |          /* AN_0 */
            BITPOS(1) |          /* AN_1 */
            BITPOS(2) |          /* AN_2 */
            BITPOS(3) |          /* AN_3 */
            BITPOS(4) |          /* AN_4 */
            BITPOS(5) |          /* AN_5 */
            BITPOS(6) |          /* AN_6 */
            BITPOS(7) |          /* AN_7 */
            BITPOS(8) |          /* AN_8 */
            BITPOS(9) |          /* AN_9 */
            BITPOS(10) |         /* AN_10 */
            BITPOS(11) |         /* AN_11 */
            BITPOS(12) |         /* AN_12 */
            BITPOS(13) |         /* AN_13 */
            BITPOS(14) |         /* AN_14 */
            BITPOS(15),          /* AN_15 */
                                 /* Adc Channel which are part of group, each bit represents corresponding analog channel */
            ADC_NODE_0,          /* Adc Node 0  */
            ADC_CONV_MODE_CONTINUOUS,  /* Oneshot or Continuous  */
            DMA_CHANNEL_1,       /* DMA channel to be used for transfer of result */
            NULL_PTR,            /* call back notification function */
            AdcGroup0_Res,       /* Pointer where results are stored for this group */
      },
      {
            BITPOS(0) |          /* AN_16 */
            BITPOS(1) |          /* AN_17 */
            BITPOS(2) |          /* AN_18 */
            BITPOS(3) |          /* AN_19 */
            BITPOS(4) |          /* AN_20 */
            BITPOS(5) |          /* AN_21 */
            BITPOS(6) |          /* AN_22 */
            BITPOS(7) |          /* AN_23 */
            BITPOS(8) |          /* AN_24 */
            BITPOS(9) |          /* AN_25 */
            BITPOS(10) |         /* AN_26 */
            BITPOS(11) |         /* AN_27 */
            BITPOS(12) |         /* AN_28 */
            BITPOS(13) |         /* AN_29 */
            BITPOS(14) |         /* AN_30 */
            BITPOS(15),          /* AN_31 */
                                 /* Adc Channel which are part of group, each bit represents corresponding analog channel */
            ADC_NODE_1,          /* Adc Node 1  */
            ADC_CONV_MODE_CONTINUOUS,  /* Oneshot or Continuous  */
            DMA_CHANNEL_3,       /* DMA channel to be used for transfer of result */
            NULL_PTR,            /* call back notification function */
            AdcGroup1_Res,       /* Pointer where results are stored for this group */
      },
};


const Adc_NodeType AdcNode[] = {
   {
      0,                        /* Adc Node 0 */
      sizeof(AdcChannelProfile) /
      sizeof(AdcChannelProfile[0]),  /* Number of profile configuration, 1..4 */
      {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
      },                          /* Array(16) of profile index to be written to each analog channel */
      &AdcChannelProfile[0]             /* Pointer to profile configuration structure */
   },
   {
      1,                          /* Adc Node 1 for 1767*/
      sizeof(AdcChannelProfile) /
      sizeof(AdcChannelProfile[0]),     /* Number of profile configuration, 1..4 */
      {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
      },                          /* Array(16) of profile index to be written to each analog channel */
      &AdcChannelProfile[0]             /* Pointer to profile configuration structure */
   }
};


const Adc_ConfigType adc_config = {
   sizeof(AdcChannelGroup) / sizeof(AdcChannelGroup[0]),
   sizeof(AdcNode) / sizeof(AdcNode[0]),
   AdcChannelGroup,
   AdcNode
};

#define ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"
