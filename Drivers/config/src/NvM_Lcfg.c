/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM_Lcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/04 14:33:59MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM_Lcfg.c  $
 *  Revision 1.6 2011/03/04 14:33:59MEZ Cotor Vlad RGB (COTORV) 
 *  - Update NvM due to CTC and DEH variables
 *  Revision 1.5 2011/03/04 12:29:09MEZ Kubot Markus RGB (KUBOTM) 
 *  Update common files for integration #6
 *  Revision 1.4 2011/03/03 17:11:49MEZ Cotor Vlad RGB (COTORV) 
 *  - Update NvM Configuration due to default rom data
 *  Revision 1.3 2011/02/28 10:08:50MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.2 2010/07/12 14:01:56MESZ Cotor Vlad RGB (COTORV)
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "NvM.h"
#include "NvM_Cbk.h"
#include "MemIf.h"
#include "EcuM.h"

#include "CTC_Nvmy.h"
#include "DEH_Nvmy.h"
#include "CHA_Nvmy.h"


#include "BPN_Nvmy.h"
#include "BSTC_Nvmy.h"
#include "BBALC_Nvmy.h"

#define NVM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"

NvM_BlockStatus nvm_status[NVM_MAX_NUM_OF_BLOCKS];
boolean ecum_nvm_job_error;
boolean ecum_nvm_job_done;

#define NVM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"


#define NVM_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
const uint8 NvmDef_02[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_03[4] = {
      0u, 0u, 0u, 0u
	  };
const uint8 NvmDef_04[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_05[1] = {
	  0u
	  };
const uint8 NvmDef_06[1] = {
	  0u
	  };
const uint8 NvmDef_07[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_08[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_09[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_10[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_11[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_12[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_13[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_14[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_15[4] = {
      0u, 0u, 0u, 0u
      };
const uint8 NvmDef_16[10] = {
      0u, 0u, 0u, 0u,0u, 0u, 0u, 0u,0u, 0u
      };
const uint8 NvmDef_17[10] = {
      0u, 0u, 0u, 0u,0u, 0u, 0u, 0u,0u, 0u
      };

const NvM_ConfigBlockType nvm_cfg_blocks[] = {
/*
 * crc:     NVM_CRC_NONE = 0, NVM_CRC16 = 1, NVM_CRC32 = 2
 * device:  MEMIF_FEE_ID = 0
 *
 *    | r | c | d | b |
 *    | e | r | e | l |
 *    | a | c | v | o |
 *    | d |   | i | c |
 *    | a |   | c | k |
 *    | l |   | e | # |
 *    | l |   |   |   | block_size                           ram_img                                                      rom_img    status          init_blk  callback */
/* ==================================================================================================================================================================== */
/* 0*/{ 0,  0,  1,  0,  0,                                   NULL_PTR,                                                    NULL_PTR,  NULL_PTR,       NULL_PTR, NULL_PTR },
/* 1*/{ 1,  0,  1,  1,  sizeof(NvM_Block1),                  NULL_PTR,                                                    NULL_PTR,  &nvm_status[1], NULL_PTR, NULL_PTR },
/* === DO NOT CHANGE THE 1st AND THE 2nd BLOCKS ======================================================================================================================= */
/* 2*/{ 1,  0,  1,  2,  sizeof(NVM_CTC_CtctrDtrtPos),        &NVM_CTC_CtctrDtrtPos.nvmy_CTC_CtctrDtrtPos_0,               NvmDef_02,  &nvm_status[2], NULL_PTR, NULL_PTR },
/* 3*/{ 1,  0,  1,  3,  sizeof(NVM_CTC_CtctrDtrtNeg),        &NVM_CTC_CtctrDtrtNeg.nvmy_CTC_CtctrDtrtNeg_0,               NvmDef_03,  &nvm_status[3], NULL_PTR, NULL_PTR },
/* 4*/{ 1,  0,  1, 	4,  sizeof(NVM_CTC_CtctrDtrtPrec),       &NVM_CTC_CtctrDtrtPrec.nvmy_CTC_CtctrDtrtPrec_0,             NvmDef_04,  &nvm_status[4], NULL_PTR, NULL_PTR },
/* 5*/{ 1,  0,  1,  5,  sizeof(NVM_DEH_ErrLvl7Ct),           &NVM_DEH_ErrLvl7Ct.nvmy_DEH_ErrLvl7Ct,                       NvmDef_05,  &nvm_status[5], NULL_PTR, NULL_PTR },
/* 6*/{ 1,  0,  1,  6,  sizeof(NVM_DEH_ErrLvl7Acv),          &NVM_DEH_ErrLvl7Acv.nvmy_DEH_ErrLvl7Acv,                     NvmDef_06,  &nvm_status[6], NULL_PTR, NULL_PTR },
/* 7*/{ 1,  0,  1,  7,  sizeof(NVM_CHA_Reserved1),           &NVM_CHA_Reserved1.nvmy_CHA_Reserved1_0,                     NvmDef_07,  &nvm_status[7], NULL_PTR, NULL_PTR },
/* 8*/{ 1,  0,  1,  8,  sizeof(NVM_CHA_Reserved2),           &NVM_CHA_Reserved2.nvmy_CHA_Reserved2_0,                     NvmDef_08,  &nvm_status[8], NULL_PTR, NULL_PTR },
/* 9*/{ 1,  0,  1,  9,  sizeof(NVM_CHA_Reserved3),           &NVM_CHA_Reserved3.nvmy_CHA_Reserved3_0,                     NvmDef_09,  &nvm_status[9], NULL_PTR, NULL_PTR },
/*10*/{ 1,  0,  1,  10,  sizeof(NVM_BPN_EnerWH),              &NVM_BPN_EnerWH.nvmy_BPN_EnerWH_0,                           NvmDef_10,  &nvm_status[10],NULL_PTR, NULL_PTR },
/*11*/{ 1,  0,  1,  11,  sizeof(NVM_BSTC_Date),              &NVM_BSTC_Date.nvmy_BSTC_Year,                               NvmDef_11,  &nvm_status[11],NULL_PTR, NULL_PTR },
/*12*/{ 1,  0,  1,  12,  sizeof(NVM_BSTC_Time),              &NVM_BSTC_Time.nvmy_BSTC_Hour,                               NvmDef_12,  &nvm_status[12],NULL_PTR, NULL_PTR },
/*13*/{ 1,  0,  1,  13,  sizeof(NVM_BPN_SocClcn),            &NVM_BPN_SocClcn.nvmy_BPN_SocClcn_0,                         NvmDef_13,  &nvm_status[13],NULL_PTR, NULL_PTR },
/*14*/{ 1,  0,  1,  14,  sizeof(NVM_BPN_SohAh),              &NVM_BPN_SohAh.nvmy_BPN_SohAh_0,                             NvmDef_14,  &nvm_status[14],NULL_PTR, NULL_PTR },
/*15*/{ 1,  0,  1,  15,  sizeof(NVM_BPN_SohClcn),            &NVM_BPN_SohClcn.nvmy_BPN_SohClcn_0,                         NvmDef_15,  &nvm_status[15],NULL_PTR, NULL_PTR },
/*16*/{ 1,  0,  1,  16,  sizeof(NVM_BBALC_UMax),             &NVM_BBALC_UMax.nvmy_BBALC_UMax[0],                          NvmDef_16,  &nvm_status[16],NULL_PTR, NULL_PTR },
/*17*/{ 1,  1,  0,  17,  sizeof(NVM_GBChaTimes),          	 &NVM_GBChaTimes.nvmy_GBChaTimes_0,                           NvmDef_17, &nvm_status[16],NULL_PTR, NULL_PTR },
/* ==================================================================================================================================================================== */
};
/* A block contains the data that is written with the provided interface. A block
 * is always aligned to a page boundary. This also means that a block is always
 * a multiple of 128 bytes.
 *
 * Our case:
 *
 * start_block#1: 0
 * sizeof(NvM_Block1) = 84                    => blocks_needed#1: 1
 * ==========================================
 * start block#2: 1 (start_block#1 + blocks_needed#1 = 1)
 * sizeof(NVM_BCOR_CovariMtrxSqrt) = 512      => blocks_needed#2: 4
 * ==========================================
 * start block#3: 5 (start_block#2 + blocks_needed#2 = 5)
 * sizeof(NVM_BCOR_SocMdul) = 32              => blocks_needed#3: 1
 * ==========================================
 * start block#4: 6 (start_block#3 + blocks_needed#3 = 6)
 * sizeof(NVM_ELHPN_RIslnDetnCnctMem) = 4     => blocks_needed#4: 1
 * ==========================================
 * start block#5: 7 (start_block#4 + blocks_needed#4 = 7)
 * sizeof(NVM_ELHPN_RIslnDetnDcnctMem) = 4    => blocks_needed#5: 1
 * 
 * ATTENTION: The NvM configuration MUST BE THE SAME with the configuration contain in Fee_Lcfg.h !!!
 *
 * ------------------------------
 * NvM_Lcfg.h   | Fee_Lcfg.h
 * ------------------------------
 * block#       = block number
 * block_size   = block size
 * ------------------------------
 */


const NvM_ConfigType nvm_cfg = {
	nvm_cfg_blocks,                                      /* Pointer to the block configuration. */
	sizeof(nvm_cfg_blocks) / sizeof(nvm_cfg_blocks[0]),  /* Number of blocks contained in the configuration. */
	100,                                                 /* Number of bytes to be checked in NvM_MainFunction(). */
	1                                                    /* ID for the configuration of the blocks. */
};


#define NVM_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"


#define NVM_START_SEC_CODE
#include "MemMap.h"


void NvM_JobEndNotification(void)
{
	ecum_nvm_job_done = 1u;
	EcuM_CB_NfyNvMJobEnd(0, 0);
}


void NvM_JobErrorNotification(void)
{
   ecum_nvm_job_error = 1u;
}
#define NVM_STOP_SEC_CODE
#include "MemMap.h"

