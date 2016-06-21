/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: ts_gpta_int.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/24 12:42:43MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: ts_gpta_int.c  $
 *  Revision 1.5 2011/02/24 12:42:43MEZ Cotor Vlad RGB (COTORV) 
 *  - Develop shutdown function for xCU power control
 *  Revision 1.4 2011/02/02 09:17:12MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/07/12 09:45:57MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.2 2010/05/20 11:21:24MESZ Cotor Vlad RGB (COTORV) 
 *  Member moved from ts_gpta_int.c in project /PSL/Bsw/Misc/DrvAux/src/project.pj to ts_gpta_int.c in project /PSL/Bsw/Misc/DrvAux/AA000/src/project.pj.
 *  Revision 1.1 2010/05/20 10:46:35CEST Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/Misc/DrvAux/src/project.pj
 *
 * ***************************************************************************/

#include "team-s.h"
#include "Os.h"


/* the entry 0 of this array is not used to detect uninitialized interrupts */
static const void (*ts_int_func[MAX_INT_FUNC + 1])(uint32 resource_no, uint8 param);

/* index to the last used function entry in the table ts_int_func */
static uint8 last_func_idx;

/* each LTC cell has it's own entry in this table */
static struct {
	uint8 func_idx;
	uint8 param;
} ts_ltc_decode[64 + 32];

/* each GTC cell has it's own entry in this table */
static struct {
	uint8 func_idx;
	uint8 param;
} ts_gtc_decode[32];


/* allocate a entry in the function table and return the allocation */
static uint8 ts_allocIntFuncEntry(const void (*func)(uint32 ltc_no, uint8 param))
{
	SuspendAllInterrupts();
	last_func_idx++;
	ts_int_func[last_func_idx] = func;
	ResumeAllInterrupts();

	return last_func_idx;
}


void ts_registerLTC(uint32 ltc_no, uint8 param, const void (*func)(uint32 ltc_no, uint8 param))
{
	uint8 func_idx;

	if(ts_ltc_decode[ltc_no].func_idx != 0) {
		/* already registered */

		func_idx = ts_ltc_decode[ltc_no].func_idx;
		if(ts_int_func[func_idx] != func) {
			/* new function -- alloc new entry */
			func_idx = ts_allocIntFuncEntry(func);
		}
	}
	else if(ts_int_func[last_func_idx] == func) {
		/* the previous entry has the same service function -- use it */
		func_idx = last_func_idx;
	}
	else {
		/* new function -- allocate an entry for it */
		func_idx = ts_allocIntFuncEntry(func);
	}

	/* make reference to used service entry and store the parameter */
	ts_ltc_decode[ltc_no].func_idx = func_idx;
	ts_ltc_decode[ltc_no].param = param;
}



void ts_registerGTC(uint32 gtc_no, uint8 param, const void (*func)(uint32 gtc_no, uint8 param))
{
	uint8 func_idx;

	if(ts_gtc_decode[gtc_no].func_idx != 0) {
		/* already registered */

		func_idx = ts_gtc_decode[gtc_no].func_idx;
		if(ts_int_func[func_idx] != func) {
			/* new function -- alloc new entry */
			func_idx = ts_allocIntFuncEntry(func);
		}
	}
	else if(ts_int_func[last_func_idx] == func) {
		/* the previous entry has the same service function -- use it */
		func_idx = last_func_idx;
	}
	else {
		/* new function -- allocate an entry for it */
		func_idx = ts_allocIntFuncEntry(func);
	}

	/* make reference to used service entry and store the parameter */
	ts_gtc_decode[gtc_no].func_idx = func_idx;
	ts_gtc_decode[gtc_no].param = param;
}


void ts_decodeLTC(uint32 ltc)
{
	volatile uint32 *srsc;
	uint32 pending;
	uint32 ltc_no;
	uint32 i;
	uint8 func_idx;

	if(ltc < 64) {
		/* base is GPTA0 */
		srsc = (volatile uint32 *)&GPTA0_SRSC2.U;
	}
	else {
		/* base is LTCA2 */
		srsc = (volatile uint32 *)&LTCA2_SRSC2.U;
	}

	ltc_no = ltc & (~(64u | 0x3u));

	if(ltc_no >= 32u) {
		/* use SRSC3 - LTC32..63 */
		srsc += 2;
	}

	/* get and clear the pendings */
	pending = *srsc & (0xFu << (ltc_no & 31));
	*srsc = pending;

	/* call the registered interrupt handlers */
	for(i=0; i<4; i++) {
		if(pending & BITPOS(ltc_no & 31)) {
			func_idx = ts_ltc_decode[ltc_no].func_idx;

			if(func_idx != 0) {
				ts_int_func[func_idx](ltc_no, ts_ltc_decode[ltc_no].param);
			}
		}

		ltc_no++;
	}
}


void ts_decodeGTC(uint32 gtc)
{
	uint32 pending;
	uint32 i;
	uint8 func_idx;

	/* get and clear the pendings */
	pending = GPTA0_SRSC1.U & (0x3u << gtc);
	GPTA0_SRSC1.U = pending;

	/* call the registered interrupt handlers */
	for(i=0; i<2; i++) {
		if(pending & BITPOS(gtc)) {
			func_idx = ts_gtc_decode[gtc].func_idx;

			if(func_idx != 0) {
				ts_int_func[func_idx](gtc, ts_gtc_decode[gtc].param);
			}
		}

		gtc++;
	}
}


void ts_clrReqLTC(uint32 ltc_no)
{
	if(ltc_no < 64) {
		if(ltc_no < 32) {
			GPTA0_SRSC2.U = BITPOS(ltc_no);
		}
		else {
			GPTA0_SRSC3.U = BITPOS(ltc_no - 32);
		}
	}
	else {
		LTCA2_SRSC2.U = BITPOS(ltc_no - 64);
	}
}


void ts_initGPTAInt(void)
{
#ifdef SRPN_GPTA0_SRC06
	GPTA0_SRC06.U = SRPN_GPTA0_SRC06 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC07
	GPTA0_SRC07.U = SRPN_GPTA0_SRC07 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC08
	GPTA0_SRC08.U = SRPN_GPTA0_SRC08 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC09
	GPTA0_SRC09.U = SRPN_GPTA0_SRC09 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC10
	GPTA0_SRC10.U = SRPN_GPTA0_SRC10 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC11
	GPTA0_SRC11.U = SRPN_GPTA0_SRC11 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC12
	GPTA0_SRC12.U = SRPN_GPTA0_SRC12 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC13
	GPTA0_SRC13.U = SRPN_GPTA0_SRC13 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC14
	GPTA0_SRC14.U = SRPN_GPTA0_SRC14 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC15
	GPTA0_SRC15.U = SRPN_GPTA0_SRC15 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC16
	GPTA0_SRC16.U = SRPN_GPTA0_SRC16 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC17
	GPTA0_SRC17.U = SRPN_GPTA0_SRC17 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC18
	GPTA0_SRC18.U = SRPN_GPTA0_SRC18 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC19
	GPTA0_SRC19.U = SRPN_GPTA0_SRC19 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC20
	GPTA0_SRC20.U = SRPN_GPTA0_SRC20 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC21
	GPTA0_SRC21.U = SRPN_GPTA0_SRC21 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC22
	GPTA0_SRC22.U = SRPN_GPTA0_SRC22 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC23
	GPTA0_SRC23.U = SRPN_GPTA0_SRC23 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC24
	GPTA0_SRC24.U = SRPN_GPTA0_SRC24 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC25
	GPTA0_SRC25.U = SRPN_GPTA0_SRC25 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC26
	GPTA0_SRC26.U = SRPN_GPTA0_SRC26 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC27
	GPTA0_SRC27.U = SRPN_GPTA0_SRC27 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC27
	GPTA0_SRC27.U = SRPN_GPTA0_SRC27 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC28
	GPTA0_SRC28.U = SRPN_GPTA0_SRC28 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC29
	GPTA0_SRC29.U = SRPN_GPTA0_SRC29 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC30
	GPTA0_SRC30.U = SRPN_GPTA0_SRC30 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC31
	GPTA0_SRC31.U = SRPN_GPTA0_SRC31 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC32
	GPTA0_SRC32.U = SRPN_GPTA0_SRC32 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC33
	GPTA0_SRC33.U = SRPN_GPTA0_SRC33 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC34
	GPTA0_SRC34.U = SRPN_GPTA0_SRC34 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC35
	GPTA0_SRC35.U = SRPN_GPTA0_SRC35 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC36
	GPTA0_SRC36.U = SRPN_GPTA0_SRC36 | BITPOS(12);
#endif
#ifdef SRPN_GPTA0_SRC37
	GPTA0_SRC37.U = SRPN_GPTA0_SRC37 | BITPOS(12);
#endif

#ifdef SRPN_LTCA2_SRC00
	LTCA2_SRC00.U = SRPN_LTCA2_SRC00 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC01
	LTCA2_SRC01.U = SRPN_LTCA2_SRC01 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC02
	LTCA2_SRC02.U = SRPN_LTCA2_SRC02 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC03
	LTCA2_SRC03.U = SRPN_LTCA2_SRC03 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC04
	LTCA2_SRC04.U = SRPN_LTCA2_SRC04 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC05
	LTCA2_SRC05.U = SRPN_LTCA2_SRC05 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC06
	LTCA2_SRC06.U = SRPN_LTCA2_SRC06 | BITPOS(12);
#endif
#ifdef SRPN_LTCA2_SRC07
	LTCA2_SRC07.U = SRPN_LTCA2_SRC07 | BITPOS(12);
#endif
}


#ifdef SRPN_GPTA0_SRC06
void __interrupt(SRPN_GPTA0_SRC06) __enable_ Gpta0Src06(void)
{
	ts_decodeGTC(0);
}
#endif

#ifdef SRPN_GPTA0_SRC07
#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_GPTA0_SRC07) __enable_ Gpta0Src07(void)
#else
void Gpt_Irq1func(void)
#endif
{
	ts_decodeGTC(2);
}
#endif

#ifdef SRPN_GPTA0_SRC08
#ifdef TEAMS_TEST_ENV
void __interrupt(SRPN_GPTA0_SRC08) __enable_ Gpta0Src08(void)
#else
void Gpt_Irq2func(void)
#endif
{
	ts_decodeGTC(4);
}
#endif

#ifdef SRPN_GPTA0_SRC09
void __interrupt(SRPN_GPTA0_SRC09) __enable_ Gpta0Src09(void)
{
	ts_decodeGTC(6);
}
#endif

#ifdef SRPN_GPTA0_SRC10
void __interrupt(SRPN_GPTA0_SRC10) __enable_ Gpta0Src10(void)
{
	ts_decodeGTC(8);
}
#endif

#ifdef SRPN_GPTA0_SRC11
void __interrupt(SRPN_GPTA0_SRC11) __enable_ Gpta0Src11(void)
{
	ts_decodeGTC(10);
}
#endif

#ifdef SRPN_GPTA0_SRC12
void __interrupt(SRPN_GPTA0_SRC12) __enable_ Gpta0Src12(void)
{
	ts_decodeGTC(12);
}
#endif

#ifdef SRPN_GPTA0_SRC13
void __interrupt(SRPN_GPTA0_SRC13) __enable_ Gpta0Src13(void)
{
	ts_decodeGTC(14);
}
#endif

#ifdef SRPN_GPTA0_SRC14
void __interrupt(SRPN_GPTA0_SRC14) __enable_ Gpta0Src14(void)
{
	ts_decodeGTC(16);
}
#endif

#ifdef SRPN_GPTA0_SRC15
void __interrupt(SRPN_GPTA0_SRC15) __enable_ Gpta0Src15(void)
{
	ts_decodeGTC(18);
}
#endif

#ifdef SRPN_GPTA0_SRC16
void __interrupt(SRPN_GPTA0_SRC16) __enable_ Gpta0Src16(void)
{
	ts_decodeGTC(20);
}
#endif

#ifdef SRPN_GPTA0_SRC17
void __interrupt(SRPN_GPTA0_SRC17) __enable_ Gpta0Src17(void)
{
	ts_decodeGTC(22);
}
#endif

#ifdef SRPN_GPTA0_SRC18
void __interrupt(SRPN_GPTA0_SRC18) __enable_ Gpta0Src18(void)
{
	ts_decodeGTC(24);
}
#endif

#ifdef SRPN_GPTA0_SRC19
void __interrupt(SRPN_GPTA0_SRC19) __enable_ Gpta0Src19(void)
{
	ts_decodeGTC(26);
}
#endif

#ifdef SRPN_GPTA0_SRC20
void __interrupt(SRPN_GPTA0_SRC20) __enable_ Gpta0Src20(void)
{
	ts_decodeGTC(28);
}
#endif

#ifdef SRPN_GPTA0_SRC21
void __interrupt(SRPN_GPTA0_SRC21) __enable_ Gpta0Src21(void)
{
	ts_decodeGTC(30);
}
#endif

#ifdef SRPN_GPTA0_SRC22
void __interrupt(SRPN_GPTA0_SRC22) __enable_ Gpta0Src22(void)
{
	ts_decodeLTC(0);
}
#endif

#ifdef SRPN_GPTA0_SRC23
void __interrupt(SRPN_GPTA0_SRC23) __enable_ Gpta0Src23(void)
{
	ts_decodeLTC(4);
}
#endif

#ifdef SRPN_GPTA0_SRC24
void __interrupt(SRPN_GPTA0_SRC24) __enable_ Gpta0Src24(void)
{
	ts_decodeLTC(8);
}
#endif

#ifdef SRPN_GPTA0_SRC25
void __interrupt(SRPN_GPTA0_SRC25) __enable_ Gpta0Src25(void)
{
	ts_decodeLTC(12);
}
#endif

#ifdef SRPN_GPTA0_SRC26
void __interrupt(SRPN_GPTA0_SRC26) __enable_ Gpta0Src26(void)
{
	ts_decodeLTC(16);
}
#endif

#ifdef SRPN_GPTA0_SRC27
void __interrupt(SRPN_GPTA0_SRC27) __enable_ Gpta0Src27(void)
{
	ts_decodeLTC(20);
}
#endif

#ifdef SRPN_GPTA0_SRC28
void __interrupt(SRPN_GPTA0_SRC28) __enable_ Gpta0Src28(void)
{
	ts_decodeLTC(24);
}
#endif

#ifdef SRPN_GPTA0_SRC29
void __interrupt(SRPN_GPTA0_SRC29) __enable_ Gpta0Src29(void)
{
	ts_decodeLTC(28);
}
#endif

#ifdef SRPN_GPTA0_SRC30
void __interrupt(SRPN_GPTA0_SRC30) __enable_ Gpta0Src30(void)
{
	ts_decodeLTC(32);
}
#endif

#ifdef SRPN_GPTA0_SRC31
void __interrupt(SRPN_GPTA0_SRC31) __enable_ Gpta0Src31(void)
{
	ts_decodeLTC(36);
}
#endif

#ifdef SRPN_GPTA0_SRC32
void __interrupt(SRPN_GPTA0_SRC32) __enable_ Gpta0Src32(void)
{
	ts_decodeLTC(40);
}
#endif

#ifdef SRPN_GPTA0_SRC33
void __interrupt(SRPN_GPTA0_SRC33) __enable_ Gpta0Src33(void)
{
	ts_decodeLTC(44);
}
#endif

#ifdef SRPN_GPTA0_SRC34
void __interrupt(SRPN_GPTA0_SRC34) __enable_ Gpta0Src34(void)
{
	ts_decodeLTC(48);
}
#endif

#ifdef SRPN_GPTA0_SRC35
void __interrupt(SRPN_GPTA0_SRC35) __enable_ Gpta0Src35(void)
{
	ts_decodeLTC(52);
}
#endif

#ifdef SRPN_GPTA0_SRC36
void __interrupt(SRPN_GPTA0_SRC36) __enable_ Gpta0Src36(void)
{
	ts_decodeLTC(56);
}
#endif

#ifdef SRPN_GPTA0_SRC37
void __interrupt(SRPN_GPTA0_SRC37) __enable_ Gpta0Src37(void)
{
	ts_decodeLTC(60);
}
#endif



#ifdef SRPN_LTCA2_SRC00
void __interrupt(SRPN_LTCA2_SRC00) __enable_ Ltca2Src00(void)
{
	ts_decodeLTC(64 + 0);
}
#endif

#ifdef SRPN_LTCA2_SRC01
void __interrupt(SRPN_LTCA2_SRC01) __enable_ Ltca2Src01(void)
{
	ts_decodeLTC(64 + 4);
}
#endif

#ifdef SRPN_LTCA2_SRC02
void __interrupt(SRPN_LTCA2_SRC02) __enable_ Ltca2Src02(void)
{
	ts_decodeLTC(64 + 8);
}
#endif

#ifdef SRPN_LTCA2_SRC03
void __interrupt(SRPN_LTCA2_SRC03) __enable_ Ltca2Src03(void)
{
	ts_decodeLTC(64 + 12);
}
#endif

#ifdef SRPN_LTCA2_SRC04
void __interrupt(SRPN_LTCA2_SRC04) __enable_ Ltca2Src04(void)
{
	ts_decodeLTC(64 + 16);
}
#endif

#ifdef SRPN_LTCA2_SRC05
void __interrupt(SRPN_LTCA2_SRC05) __enable_ Ltca2Src05(void)
{
	ts_decodeLTC(64 + 20);
}
#endif

#ifdef SRPN_LTCA2_SRC06
void __interrupt(SRPN_LTCA2_SRC06) __enable_ Ltca2Src06(void)
{
	ts_decodeLTC(64 + 24);
}
#endif

#ifdef SRPN_LTCA2_SRC07
void __interrupt(SRPN_LTCA2_SRC07) __enable_ Ltca2Src07(void)
{
	ts_decodeLTC(64 + 28);
}
#endif
