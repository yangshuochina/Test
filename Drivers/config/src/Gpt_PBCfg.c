/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Gpt_PBCfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/24 12:44:59MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Gpt_PBCfg.c  $
 *  Revision 1.3 2011/02/24 12:44:59MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.2 2010/07/12 09:43:15MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:29:01MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#include "Gpt.h"
#include "WdgM.h"
#include "BPOCO_PwrCtlDrvr.h"


#define GPT_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

const Gpt_ConfigChannelType gpt_channel_config[] = {
		/* channel 0: used by EcuM */
		{
			NULL_PTR,             /**< no notification */
			GPT_MODE_ONE_SHOT,
			GPT_GT0,
			0,
		},
		/* channel 1: used by WdgM */
		{
			WdgM_Cbk_GptNotification,
			GPT_MODE_CONTINOUS,
			GPT_GT0,
			1,
		},
      /* channel 2: used by BPOCO for watchdog triggering in shutdown procedure */
      {
         WgdTrig_GptNotification,
         GPT_MODE_CONTINOUS,
         GPT_GT0,
         2,
      },
      /* channel 3: used by BPOCO for waiting for releasing the self-sustaining line from BCU */
      {
         ShtdnEcuWait_GptNotification,
         GPT_MODE_CONTINOUS,
         GPT_GT0,
         4,
      },
      {
    	 NULL_PTR,
    	 GPT_MODE_ONE_SHOT,
    	 GPT_GT0,
    	 3,
      }
};

const Gpt_ConfigType gpt_config = {
		sizeof(gpt_channel_config) / sizeof(Gpt_ConfigChannelType),
		gpt_channel_config
};

#define GPT_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
