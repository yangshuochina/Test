/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BAUXM_AuxMeasDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   23.04.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:06:15MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BAUXM_AuxMeasDrvr.c  $
 *  Revision 1.6 2010/11/22 07:06:15MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.5 2010/11/16 13:10:18MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.4 2010/09/03 09:53:24MESZ Cotor Vlad RGB (COTORV) 
 *  Update due to lack of local function prototype
 *  Revision 1.3 2010/09/03 09:22:10MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to scheduling functions
 *  Revision 1.2 2010/08/27 14:36:46MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.1 2010/08/25 16:41:05MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bauxm/src/project.pj
 *  Revision 1.2 2010/07/19 09:56:16MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for auxiliary measurements complex device driver due to specification update
 *  Revision 1.1 2010/05/20 14:04:13MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bauxm/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_BAUXM_AUXMEASDRVR_C_              /* Master define for this module    */


/* ========== Includes ===================================================== */
#include "Adc.h"

#include "BAUXM_AuxMeasDrvr.h"           /* module header file     */
#include "BSW_DrvrAux.h"


/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */
void BAUXM_AnDigCnvrOutClcd_U(void);

/* ========== Local Variables ============================================== */
#define BAUXM_START_SEC_VAR_16BIT
#include "MemMap.h"
   uint16 BAUXM_PrmFacFil[BAUXM_CfgNoChnAdc];
   uint16 UAdcAuxMeasData[BAUXM_CfgNoChnAdc];
#define BAUXM_STOP_SEC_VAR_16BIT
#include "MemMap.h"
/* ========================================================================= */
#define BAUXM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   Adc_ValueGroupType BAUXM_AdcResult0[16];
   Adc_ValueGroupType BAUXM_AdcResult1[16];
   #ifdef AuxMeas_CH_0
   Struct_PT1Fil  PT1Fil_ch0;
   #endif
   #ifdef AuxMeas_CH_1
   Struct_PT1Fil  PT1Fil_ch1;
   #endif
   #ifdef AuxMeas_CH_2
   Struct_PT1Fil  PT1Fil_ch2;
   #endif
   #ifdef AuxMeas_CH_3
   Struct_PT1Fil  PT1Fil_ch3;
   #endif
   #ifdef AuxMeas_CH_4
   Struct_PT1Fil  PT1Fil_ch4;
   #endif
   #ifdef AuxMeas_CH_5
   Struct_PT1Fil  PT1Fil_ch5;
   #endif
   #ifdef AuxMeas_CH_6
   Struct_PT1Fil  PT1Fil_ch6;
   #endif
   #ifdef AuxMeas_CH_7
   Struct_PT1Fil  PT1Fil_ch7;
   #endif
   #ifdef AuxMeas_CH_8
   Struct_PT1Fil  PT1Fil_ch8;
   #endif
   #ifdef AuxMeas_CH_9
   Struct_PT1Fil  PT1Fil_ch9;
   #endif
   #ifdef AuxMeas_CH_10
   Struct_PT1Fil  PT1Fil_ch10;
   #endif
   #ifdef AuxMeas_CH_11
   Struct_PT1Fil  PT1Fil_ch11;
   #endif
#define BAUXM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */
#define BAUXM_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BAUXM_AnDigCnvrOutClcd_U(void)
 *
 * @details This function fills the array UAdcAuxMeasData with filtered (with PT1 filter) values
 * to be analyzed and used at a particular desired time.
 *
 * @param none
 *
 * @return void
 */
void BAUXM_AnDigCnvrOutClcd_U(void)
{
#ifdef AuxMeas_CH_0     /* 1st analog input */
         PT1Fil_ch0.factor = BAUXM_PrmFacFil[0];
         PT1Fil_ch0.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_0;
         ClcdPT1Fil(&PT1Fil_ch0);
         UAdcAuxMeasData[0] = PT1Fil_ch0.v_ADC_out;
#endif

#ifdef AuxMeas_CH_1     /* 2nd analog input */
         PT1Fil_ch1.factor = BAUXM_PrmFacFil[1];
         PT1Fil_ch1.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_1;
         ClcdPT1Fil(&PT1Fil_ch1);
         UAdcAuxMeasData[1] = PT1Fil_ch1.v_ADC_out;
#endif

#ifdef AuxMeas_CH_2     /* 3rd analog input */
         PT1Fil_ch2.factor = BAUXM_PrmFacFil[2];
         PT1Fil_ch2.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_2;
         ClcdPT1Fil(&PT1Fil_ch2);
         UAdcAuxMeasData[2] = PT1Fil_ch2.v_ADC_out;
#endif

#ifdef AuxMeas_CH_3     /* 4th analog input */
         PT1Fil_ch3.factor = BAUXM_PrmFacFil[3];
         PT1Fil_ch3.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_3;
         ClcdPT1Fil(&PT1Fil_ch3);
         UAdcAuxMeasData[3] = PT1Fil_ch3.v_ADC_out;
#endif

#ifdef AuxMeas_CH_4     /* 5th analog input */
         PT1Fil_ch4.factor = BAUXM_PrmFacFil[4];
         PT1Fil_ch4.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_4;
         ClcdPT1Fil(&PT1Fil_ch4);
         UAdcAuxMeasData[4] = PT1Fil_ch4.v_ADC_out;
#endif

#ifdef AuxMeas_CH_5     /* 6th analog input */
         PT1Fil_ch5.factor = BAUXM_PrmFacFil[5];
         PT1Fil_ch5.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_5;
         ClcdPT1Fil(&PT1Fil_ch5);
         UAdcAuxMeasData[5] = PT1Fil_ch5.v_ADC_out;
#endif

#ifdef AuxMeas_CH_6     /* 7th analog input */
         PT1Fil_ch6.factor = BAUXM_PrmFacFil[6];
         PT1Fil_ch6.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_6;
         ClcdPT1Fil(&PT1Fil_ch6);
         UAdcAuxMeasData[6] = PT1Fil_ch6.v_ADC_out;
#endif

#ifdef AuxMeas_CH_7     /* 8th analog input */
         PT1Fil_ch7.factor = BAUXM_PrmFacFil[7];
         PT1Fil_ch7.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_7;
         ClcdPT1Fil(&PT1Fil_ch7);
         UAdcAuxMeasData[7] = PT1Fil_ch7.v_ADC_out;
#endif

#ifdef AuxMeas_CH_8     /* 9th analog input */
         PT1Fil_ch8.factor = BAUXM_PrmFacFil[8];
         PT1Fil_ch8.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_8;
         ClcdPT1Fil(&PT1Fil_ch8);
         UAdcAuxMeasData[8] = PT1Fil_ch8.v_ADC_out;
#endif

#ifdef AuxMeas_CH_9     /* 10th analog input */
         PT1Fil_ch9.factor = BAUXM_PrmFacFil[9];
         PT1Fil_ch9.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_9;
         ClcdPT1Fil(&PT1Fil_ch9);
         UAdcAuxMeasData[9] = PT1Fil_ch9.v_ADC_out;
#endif

#ifdef AuxMeas_CH_10    /* 11th analog input */
         PT1Fil_ch10.factor = BAUXM_PrmFacFil[10];
         PT1Fil_ch10.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_10;
         ClcdPT1Fil(&PT1Fil_ch10);
         UAdcAuxMeasData[10] = PT1Fil_ch10.v_ADC_out;
#endif

#ifdef AuxMeas_CH_11    /* 12th analog input */
         PT1Fil_ch11.factor = BAUXM_PrmFacFil[11];
         PT1Fil_ch11.v_ADC_meas = BAUXM_UAnDigCnvrMeasd_AuxMeas_CH_11;
         ClcdPT1Fil(&PT1Fil_ch11);
         UAdcAuxMeasData[11] = PT1Fil_ch11.v_ADC_out;
#endif
}
#define BAUXM_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/


/* ========== Global Functions ============================================= */
#define BAUXM_START_SEC_CODE_INIT
#include "MemMap.h"
void BAUXM_Ini(void)
{
   uint8 TmpCnt;
   for (TmpCnt = 0; TmpCnt < BAUXM_CfgNoChnAdc; TmpCnt++)
   { BAUXM_PrmFacFil[TmpCnt] = 0xFFFF; }
}
#define BAUXM_STOP_SEC_CODE_INIT
#include "MemMap.h"
/* ========== END BAUXM_RstInit ============================================ */


#define BAUXM_START_SEC_CODE_10MS
#include "MemMap.h"
void BAUXM_Scheduler_10ms(void)
{
   BAUXM_AnDigCnvrOutClcd_U();
}
#define BAUXM_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BAUXM_Scheduler_10ms ===================================== */


#define BAUXM_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BAUXM_GetUAdc(uint8 PrmIdxChn)
{
   return UAdcAuxMeasData[PrmIdxChn];
}
#define BAUXM_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BAUXM_GetUAdc ============================================ */


#define BAUXM_START_SEC_CODE_10MS
#include "MemMap.h"
void BAUXM_SetFacFilUAdc(uint8 PrmIdxChn, uint16 PrmFacFilUAdc)
{
   BAUXM_PrmFacFil[PrmIdxChn] = PrmFacFilUAdc;
}
#define BAUXM_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BAUXM_SetFacFilUAdc ====================================== */


/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif

/* undefine master define for this module    */
#ifdef MASTER_BAUXM_AUXMEASDRVR_C_
#undef MASTER_BAUXM_AUXMEASDRVR_C_
#endif

