/*
 * Ea_Lcfg.c
 *
 *  Created on: 2015-2-2
 *      Author: Administrator
 */
#include "Ea.h"
#include "Ea_Cbk.h"
#include "NvM_Cbk.h"
#include "MemIf.h"
#include "NvM.h"
#include "Spi.h"
#include "Ea_Cfg.h"
#include "Eep_Cfg.h"
#include "CTC_Nvmy.h"
#include "DEH_Nvmy.h"
#include "CHA_Nvmy.h"
#include "BPN_Nvmy.h"
#include "BSTC_Nvmy.h"
#include "BBALC_Nvmy.h"

#define EA_START_SEC_VAR_32BIT
#include "MemMap.h"
Ea_PageStatus status[EA_NUMBER_OF_BLOCKS];
#define EA_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define EA_START_SEC_VAR_8BIT
#include "MemMap.h"

sint8 PageGenerations[EA_MAX_PAGE_NUM];

#pragma align 4
uint8 Ea_TempBuffer[EEP_PAGE_SIZE];
#pragma align restore

#define EA_STOP_SEC_VAR_8BIT
#include "MemMap.h"


#define EA_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

const Ea_BlockConfigType ea_cfg_blocks[] =
{
		/* B  B									B		B
		 * l  l									l		l
		 * o  o									o		o
		 * c  c									c		c
		 * k  k									k		k
		 *										P		P
		 * N  S									a		a
		 * u  i									g		g
		 * m  z									e		e
		 * b  e									C		S
		 * e									n		t
		 * r  *									t		aus	*/
/* 0*/	  {0,	0,								0,	&status[0]},
/* 1*/	  {1,  sizeof(NvM_Block1),				4,	&status[1]},
/* 2*/	  {2,  sizeof(NVM_CTC_CtctrDtrtPos),	4,	&status[2]},
/* 3*/	  {3,  sizeof(NVM_CTC_CtctrDtrtNeg), 	4,	&status[3]},
/* 4*/	  {4,  sizeof(NVM_CTC_CtctrDtrtPrec),	4,	&status[4]},
/* 5*/	  {5,  sizeof(NVM_DEH_ErrLvl7Ct),		4,	&status[5]},
/* 6*/	  {6,  sizeof(NVM_DEH_ErrLvl7Acv),		4,	&status[6]},
/* 7*/	  {7,  sizeof(NVM_CHA_Reserved1),		4,	&status[7]},
/* 8*/	  {8,  sizeof(NVM_CHA_Reserved2),		4,	&status[8]},
/* 9*/	  {9,  sizeof(NVM_CHA_Reserved3),		4,	&status[9]},
/*10*/	  {10,  sizeof(NVM_BPN_EnerWH),			4,	&status[10]},
/*11*/	  {11,  sizeof(NVM_BSTC_Date),			4,	&status[11]},
/*12*/	  {12,  sizeof(NVM_BSTC_Time),			4,	&status[12]},
/*13*/	  {13,  sizeof(NVM_BPN_SocClcn),		4,	&status[13]},
/*14*/	  {14,  sizeof(NVM_BPN_SohAh),			4,	&status[14]},
/*15*/	  {15,  sizeof(NVM_BPN_SohClcn),		4,	&status[15]},
/*16*/	  {16,  sizeof(NVM_BBALC_UMax),			4,	&status[16]},
/*17*/	  {17,  sizeof(NVM_GBChaTimes),			4,	&status[17]},
};

const Ea_ConfigType ea_cfg = {
		ea_cfg_blocks,
		sizeof(ea_cfg_blocks)/sizeof(ea_cfg_blocks[0]),
		8,
		sizeof(PageGenerations) / sizeof(PageGenerations[0])
};

#define EA_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

#define EA_START_SEC_CODE
#include "MemMap.h"

void Ea_JobEndNotification(void)
{
}

void Ea_JobErrorNotification(void)
{
}
#define EA_STOP_SEC_CODE
#include "MemMap.h"

