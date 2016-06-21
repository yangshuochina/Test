/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: team-s.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/02 17:32:00MEZ $
 *                      $Author: Opel Martin RGB (OPELM) $
 *
 * ***************************************************************************
 *
 *  $Log: team-s.h  $
 *  Revision 1.5 2011/03/02 17:32:00MEZ Opel Martin RGB (OPELM) 
 *  - Mathlib 2.1.0 update
 *  - Std_Types generic
 *  Revision 1.4 2011/02/02 09:17:00MEZ Cotor Vlad RGB (COTORV)
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/07/12 09:45:47MESZ Cotor Vlad RGB (COTORV)
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.2 2010/05/20 11:21:23MESZ Cotor Vlad RGB (COTORV)
 *  Member moved from team-s.h in project /PSL/Bsw/Misc/DrvAux/src/project.pj to team-s.h in project /PSL/Bsw/Misc/DrvAux/AA000/src/project.pj.
 *  Revision 1.1 2010/05/20 10:46:29CEST Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /PSL/Bsw/Misc/DrvAux/src/project.pj
 *
 * ***************************************************************************/

#ifndef TEAMS_H_
#define TEAMS_H_

#include "Std_Types.h"
#include <stdlib.h>  /* NULL,... */
#include <string.h>  /* memset, memcpy... */


/* to have the completion feature the SFR file is included */
#include "regtc1767.sfr"
#include "interrupt.h"

#include "Det.h"




/* max. number of individual interrupt functions of GPTA0, LTA2 */
#define MAX_INT_FUNC 5


/**
 * Access to the GPTA0 Global Timer Cell Control Registers - GTCCTRk.
 * @param number The number of the GTC cell 0..31.
 */
#define GTCCTR(number)   (*(GPTA0_GTCCTR00_type *)((volatile uint8 *)&GPTA0_GTCCTR00 + (number) * 8))

/**
 * Access to the GPTA0 Global Timer Cell X Registers - GTCXRk.
 * @param number The number of the GTC cell 0..31.
 */
#define GTCXR(number)   (*(GPTA0_GTCXR00_type *)((volatile uint8 *)&GPTA0_GTCXR00 + (number) * 8))



#define TEAMS_VENDOR_ID  (0)  /**< TODO find the correct vendor id */

/* assertion that calls DET with the appropriate error information and returns E_NOT_OK */
#define TEAMS_ASSERT_RETERR(cond, module_id, instance_id, api_id, error_id) { \
	if(!(cond)) {															\
		Det_ReportError(module_id, instance_id, api_id, error_id);			\
		return E_NOT_OK;													\
	}																		\
}

/* assertion that calls DET with the appropriate error information and returns without error information */
#define TEAMS_ASSERT_RETVOID(cond, module_id, instance_id, api_id, error_id) {	\
	if(!(cond)) {															\
		Det_ReportError(module_id, instance_id, api_id, error_id);			\
		return;																\
	}																		\
}

/* assertion that calls DET with the appropriate error information and returns a user value */
#define TEAMS_ASSERT_RETVAL(cond, module_id, instance_id, api_id, error_id, ret_val) {	\
	if(!(cond)) {															\
		Det_ReportError(module_id, instance_id, api_id, error_id);			\
		return (ret_val);													\
	}																		\
}

/* assertion that calls DET with the appropriate error information and sets the local variable "ts_error" with the passed error_id */
#define TEAMS_ASSERT(cond, module_id, instance_id, api_id, error_id) {		\
	if(!(cond)) {															\
		Det_ReportError(module_id, instance_id, api_id, error_id);			\
		if(ts_error == 0) {													\
			ts_error = (error_id);      			                        \
		}																	\
	}																		\
}

/* assertion that calls DET with the appropriate error information */
#define TEAMS_ASSERT_REPORTONLY(cond, module_id, instance_id, api_id, error_id) {		\
	if(!(cond)) {															\
		Det_ReportError(module_id, instance_id, api_id, error_id);			\
	}																		\
}


/* returns a value for a bit position */
#define BITPOS(v)  ((uint32)(((uint32)1u) << ((uint32)(v))))

/* define a bitfield with 32 bit */
#define BITFIELD32(name)  __align(4) int (name)

/* set a bit in the bitfield atomically */
#define BITFIELD32_SETBIT(name, bitno)  __putbit(1, &(name), (bitno))

/* clear a bit in the bitfield atomically */
#define BITFIELD32_CLRBIT(name, bitno)  __putbit(0, &(name), (bitno))

/* get a bit in the bitfield */
#define BITFIELD32_GETBIT(name, bitno)  __getbit(&(name), (bitno))

/* check if  no bits are set in the bitfield */
#define BITFIELD32_IS_CLEAR(name)  ((name) == 0)

/* define a bitarray bit enough for size bits */
#define BITARRAY(name, size)  __align(4) int (name)[(((size)-1)/32) + 1]

/* clear all bits in the bitarray */
#define BITARRAY_CLR(name)  memset((name), 0, sizeof(name))

/* set a bit in the bitarray atomically */
#define BITARRAY_SETBIT(name, bitno)  __putbit(1, &(name)[(bitno)/32], 31 - ((bitno) & 31u))

/* clear a bit in the bitarray atomically */
#define BITARRAY_CLRBIT(name, bitno)  __putbit(0, &(name)[(bitno)/32], 31 - ((bitno) & 31u))

/* get a bit in the bitarray */
#define BITARRAY_GETBIT(name, bitno)  __getbit(&(name)[(bitno)/32], 31 - ((bitno) & 31u))

/* get the first set bit in bitarray starting with "start"  bit
 * it returns -1 if no set bit was found in the range from [start, num_of_bits[
 *
 * If a "start" is passed that is bigger then the number of bits available
 * in the array then -1 is returned.
 */
#define BITARRAY_GETFIRSTBIT(name, start)   ts_getFirstBit(name, start, sizeof(name)*8)


/* atomically set/clear bits defined by a mask */
#pragma nomisrac 16.7
static inline void ts_bfld(volatile uint32 *addr, uint32 mask, uint32 val)
{
	uint64 mask_val =((uint64)mask << 32) | val;

	__asm("LDMST [%0], %1"
			:
			: "a" (addr), "e" (mask_val));
}
#pragma nomisrac default


/* Clears the ENDINIT bit in the WDT_CON0 register in order
 * to disable the write-protection for registers protected
 * via the EndInit feature (ie. WDT_CON1, BTV, BIV, ISP, ...
 */
extern void ts_endinit_clear(void);

/* Sets the ENDINIT bit in the WDT_CON0 register in order
 * to enable the write-protection for registers protected
 * via the EndInit feature (ie. WDT_CON1, BTV, BIV, ISP, ...)
 */
extern void ts_endinit_set(void);

/* opens the WDTCON register by writing the password to it */
extern void ts_wdtpassword(void);

/* busy wait for a number of STM_TIM0 ticks. After reset STM_TIM0 is running with fsys/2 */
extern void ts_wait(uint32 ticks);

/* set pin */
#define ts_setpin(port, pin) 								\
{															\
	volatile uint32 *omr;									\
															\
	omr = (volatile uint32 *)(&P0_OMR.U) + ((port) * 0x40);	\
	/* write to PS */										\
	*omr = (uint32)1 << (pin);								\
}

/* reset pin */
#define ts_clrpin(port, pin) 								\
{															\
	volatile uint32 *omr;									\
															\
	omr = (volatile uint32 *)(&P0_OMR.U) + ((port) * 0x40);	\
	/* write to PR */										\
	*omr = (uint32)0x10000 << (pin);						\
}


extern int ts_getFirstBit(const int *name, int start, int num_of_bits);

/** Register a LTC interrupt handler.
 *
 * The registered function is called upon an interrupt request of the
 * corresponding LTC cell with the passed parameter (param).
 *
 * If the same interrupt handler was registered before then the function
 * updated the passing parameters.
 *
 * @warning This function is not reentrant.
 *
 * @param ltc_no  LTC number [0..63] for GPTA0, [64..95] for LTCA2.
 * @param param   User defined parameter that is passed to the function that gets called [0..255].
 * @param func    Function that gets called upon an interrupt.
 */
extern void ts_registerLTC(uint32 ltc_no, uint8 param, const void (*func)(uint32 ltc_no, uint8 param));


/** Register a GTC interrupt handler.
 *
 * The registered function is called upon an interrupt request of the
 * corresponding GTC cell with the passed parameter (param).
 *
 * If the same interrupt handler was registered before then the function
 * updated the passing parameters.
 *
 * @warning This function is not reentrant.
 *
 * @param gtc_no  LTC number [0..31] for GPTA0.
 * @param param   User defined parameter that is passed to the function that gets called [0..255].
 * @param func    Function that gets called upon an interrupt.
 */
extern void ts_registerGTC(uint32 gtc_no, uint8 param, const void (*func)(uint32 gtc_no, uint8 param));


/**
 * Decode the interrupt for the passed LTC cell group.
 *
 * The LTC number is the first LTC cell number of a group of 4 cells (e.g.
 * 0, 4, 8, ...).
 *
 * This function shall be called if a request from SRN group
 * number 22 to 37 is pending (refer to Table 20-16 in TC1767 user manual).
 *
 * @param ltc   LTC number [0..63] for GPTA0, [64..95] for LTCA2.
 */
extern void ts_decodeLTC(uint32 ltc);


/**
 * Decode the interrupt for the passed GTC cell group.
 *
 * The GTC number is the first GTC cell number of a group of 2 cells (e.g.
 * 0, 2, 4, ...).
 *
 * This function shall be called if a request from SRN group
 * number 6 to 21 is pending (refer to Table 20-16 in TC1767 user manual).
 *
 * @param gtc   GTC number [0..31] for GPTA0.
 */
extern void ts_decodeGTC(uint32 gtc);


/** Clear pending requests for LTC cells.
 *
 * @param ltc_no   LTC number [0..63] for GPTA0, [64..95] for LTCA2.
 */
extern void ts_clrReqLTC(uint32 ltc_no);


/** Clear pending requests for GTC cells.
 *
 * @param gtc_no   GTC number [0..31] for GPTA0.
 */
#define ts_clrReqGTC(gtc_no)	{ GPTA0_SRSC1.U = BITPOS(gtc_no); }

/** Get a pending request for GTC cells.
 *
 * @param gtc_no   GTC number [0..31] for GPTA0.
 * @return 0: no request is pending \n
 *         1: request is pending
 */
#define ts_getReqGTC(gtc_no)	((GPTA0_SRSC1.U >> (gtc_no)) & 1u)


/** Initialize all interrupt source of GPTA0 and LTCA2.
 *
 * This function initializes all configured interrupt sources of GTPA0 and LTCA2.
 * The configuration is done in interrupt.h.
 */
extern void ts_initGPTAInt(void);

#endif /* TEAMS_H_ */

/*@}*/
