/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fls_ac.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/02 16:52:58MEZ $
 *                      $Author: Opel Martin RGB (OPELM) $
 *
 * ***************************************************************************
 *
 *  $Log: Fls_ac.c  $
 *  Revision 1.4 2011/03/02 16:52:58MEZ Opel Martin RGB (OPELM) 
 *  - Mathlib 2.1.0 update
 *  - Std_Types generic
 *  Revision 1.3 2011/02/02 10:18:06MEZ Cotor Vlad RGB (COTORV)
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 09:46:28MESZ Cotor Vlad RGB (COTORV)
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:16:40MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Fls/AA000/src/project.pj
 *
 * ***************************************************************************/

#include <string.h>
#include "Fls.h"
#include "Fls_Priv.h"

#define FLS_START_SEC_CODE
#include "MemMap.h"

/* let the compiler not generate constants in flash */
#pragma immediate_in_code on



/*
 * Nomenclature for flash commands:
 * fa: flash address (any address where the flash is located
 * pa: start address of the page
 * sa: start address of the sector
 */

/*
 * enter page mode for DFLASH
 */
static inline void Fls_CmdEnterPageModeDflash(volatile uint32 *fa)
{
	volatile uint32 *addr5554 = (uint32 *)(((uint32)fa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 dummy;

	*addr5554 = 0x5D;
	dummy = PMU0_ID.U;
}

/*
 * enter page mode for PFLASH
 */
static inline void Fls_CmdEnterPageModePflash(volatile uint32 *fa)
{
	volatile uint32 *addr5554 = (uint32 *)(((uint32)fa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 dummy;

	*addr5554 = 0x50;
	dummy = PMU0_ID.U;
}


/*
 * Load Page command
 */
static inline void Fls_CmdLoadPage(volatile uint32 *fa, uint32 data)
{
	volatile uint32 *addr55F0 = (uint32 *)(((uint32)fa & 0xFFFF0000u) | 0x55F0u);
	volatile uint32 dummy;

	addr55F0 = (uint32 *) ((uint32)addr55F0 | ((uint32)fa & 0x4));

	*addr55F0 = data;
	dummy = PMU0_ID.U;
}


static inline void Fls_CmdLoadPage64(volatile uint32 *fa, uint64 data)
{
	volatile uint64 *addr55F0 = (uint64 *)(((uint32)fa & 0xFFFF0000u) | 0x55F0u);
	volatile uint32 dummy;

	*addr55F0 = data;
	dummy = PMU0_ID.U;
}


/*
 * Load Page command
 */
static inline void Fls_CmdWritePage(volatile uint32 *pa)
{
	volatile uint32 *addr5554 = (uint32 *)(((uint32)pa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 *addrAAA8 = (uint32 *)(((uint32)pa & 0xFFFF0000u) | 0xAAA8u);
	volatile uint32 dummy;

	*addr5554 = 0xAA;
	*addrAAA8 = 0x55;
	*addr5554 = 0xA0;
	*pa = 0xAA;
	dummy = PMU0_ID.U;
}



/*
 * erase physical sector command
 * cannot be used for 16K sectors of PFLASH
 */
static inline void Fls_CmdErasePhysSector(volatile uint32 *sa)
{
	volatile uint32 *addr54 = (uint32 *)(((uint32)sa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 *addrA8 = (uint32 *)(((uint32)sa & 0xFFFF0000u) | 0xAAA8u);
	volatile uint32 dummy;

	*addr54 = 0xAA;
	*addrA8 = 0x55;
	*addr54 = 0x80;
	*addr54 = 0xAA;
	*addrA8 = 0x55;
	*sa = 0x40;
	dummy = PMU0_ID.U;
}


/*
 * erase sector command
 * cannot be used for data flash sectors
 */
static inline void Fls_CmdEraseSector(volatile uint32 *sa)
{
	volatile uint32 *addr54 = (uint32 *)(((uint32)sa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 *addrA8 = (uint32 *)(((uint32)sa & 0xFFFF0000u) | 0xAAA8u);
	volatile uint32 dummy;

	*addr54 = 0xAA;
	*addrA8 = 0x55;
	*addr54 = 0x80;
	*addr54 = 0xAA;
	*addrA8 = 0x55;
	*sa = 0x30;
	dummy = PMU0_ID.U;
}


/*
 * clear status command
 */
static inline void Fls_CmdClearStatus(volatile uint32 *fa)
{
	volatile uint32 *addr5554 = (uint32 *)(((uint32)fa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 dummy;

	*addr5554 = 0xF5;
	dummy = PMU0_ID.U;
}


/*
 * reset to read command
 */
static inline void Fls_CmdResetToRead(volatile uint32 *fa)
{
	volatile uint32 *addr5554 = (uint32 *)(((uint32)fa & 0xFFFF0000u) | 0x5554u);
	volatile uint32 dummy;

	*addr5554 = 0xF0;
	dummy = PMU0_ID.U;
}


void Fls_AccEraseSector(uint32 *addr, uint32 page_size)
{
	Fls_CmdClearStatus(addr);
	Fls_CmdResetToRead(addr);

	if(page_size == FLS_PAGE_SIZE_DATA_FLASH) {
		Fls_CmdErasePhysSector(addr);
	}
	else {
		Fls_CmdEraseSector(addr);
	}

	/* do not return as long as program flash is busy */
	while(FLASH0_FSR.B.PBUSY) {
	}
	if(page_size != FLS_PAGE_SIZE_DATA_FLASH) {
		Fls_CmdResetToRead(addr);
	}
}


void Fls_AccCopyEraseToRam(uint32 *ram_addr)
{
	memcpy(ram_addr, Fls_AccEraseSector, (uint8 *)Fls_AccCopyEraseToRam - (uint8 *)Fls_AccEraseSector);
}


void Fls_AccClearEraseFromRam(uint32 *ram_addr)
{
	memset(ram_addr, 0, (uint8 *)Fls_AccCopyEraseToRam - (uint8 *)Fls_AccEraseSector);
}


void Fls_AccWritePage(uint32 *addr, const uint32 *data, uint32 page_size)
{
	static uint32 i;
	uint64 tmp;

	Fls_CmdClearStatus(addr);
	Fls_CmdResetToRead(addr);

	if(page_size == FLS_PAGE_SIZE_DATA_FLASH) {
		/* Data Flash Bank 0 or 1 */
		Fls_CmdEnterPageModeDflash(addr);
	}
	else {
		/* Program Flash */
		Fls_CmdEnterPageModePflash(addr);
	}

	for(i=0; i<page_size; i+=2) {
		tmp = (uint64)*data;
		tmp |= ((uint64)*(data+1) << 32);
		Fls_CmdLoadPage64(addr, tmp);
		data += 2;
	}
	Fls_CmdWritePage(addr);

	/* do not return as long as program flash is busy */
	while(FLASH0_FSR.B.PBUSY) {
	}

	if(page_size != FLS_PAGE_SIZE_DATA_FLASH) {
		Fls_CmdResetToRead(addr);
	}
}


void Fls_AccCopyWriteToRam(uint32 *ram_addr)
{
	memcpy(ram_addr, Fls_AccWritePage, (uint8 *)Fls_AccCopyWriteToRam - (uint8 *)Fls_AccWritePage);
}


void Fls_AccClearWriteFromRam(uint32 *ram_addr)
{
	memset(ram_addr, 0, (uint8 *)Fls_AccCopyWriteToRam - (uint8 *)Fls_AccWritePage);
}



void Fls_AccFinish(uint32 *addr)
{
	Fls_CmdResetToRead(addr);
}
#define FLS_STOP_SEC_CODE
#include "MemMap.h"


