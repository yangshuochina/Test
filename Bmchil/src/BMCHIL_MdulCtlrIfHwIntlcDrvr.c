/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BMCHIL_MdulCtlrIfHwIntlcDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   13.04.2010
 *  LAST REVISION:      $Date: 2011/02/11 16:19:08MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BMCHIL_MdulCtlrIfHwIntlcDrvr.c  $
 *  Revision 1.9 2011/02/11 16:19:08MEZ Cotor Vlad RGB (COTORV) 
 *  - Limit PWM period time for interface BMCHIL_SetPwmHiVIntlc to 200ms
 *  Revision 1.8 2011/02/02 07:28:03MEZ Cotor Vlad RGB (COTORV)
 *  - BMCHIL SW update with interface for HV interlock PWM output
 *  Revision 1.7 2010/11/26 15:00:07MEZ Cotor Vlad RGB (COTORV)
 *  Update BMCHIL: New trigger concept for MCs and current senso
 *  Revision 1.6 2010/11/22 07:22:40MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.5 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV)
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.4 2010/09/23 13:49:23MESZ Cotor Vlad RGB (COTORV)
 *  Correct offset of MC power supply current measurement. Insert configurable offset.
 *  Revision 1.3 2010/09/08 10:15:29MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for complex device driver BMCHIL
 *  - correction of BMCHIL_GetWdgHwSts function.
 *  Revision 1.2 2010/08/27 14:25:54MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.1 2010/08/25 16:41:09MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bmchil/src/project.pj
 *  Revision 1.3 2010/07/30 15:38:56MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for module controller interface and hardware interlock complex device driver due to BMCHIL scheduler and Dio_ReadChannel
 *  Revision 1.2 2010/07/19 10:53:16MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for controller interface and hardware interlock complex device driver due to specification update
 *  Revision 1.1 2010/05/20 13:59:51MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bmchil/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_BMCHIL_MDULCTLRIFHWINTLCDRVR_C_              /* Master define for this module    */
#define  MLIB_INLINE  0

/* ========== Includes ===================================================== */
#include "Mathlib.h"

#include "Adc.h"
#include "Dio.h"
#include "Pwm.h"

#include "BMCHIL_MdulCtlrIfHwIntlcDrvr.h"           /* module header file     */
#include "BSW_DrvrAux.h"


/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */
void ClcdDutyPerdInPwm(void);
void BMCHIL_AnDigCnvrOutClcd_PwrSply_UI(void);



/* ========== Local Variables ============================================== */
#define BMCHIL_START_SEC_VAR_16BIT
#include "MemMap.h"
   uint16 BMCHIL_UPwrSplyData;
   uint16 BMCHIL_IPwrSplyData;
   uint16 BMCHIL_PrmFacFil;
#define BMCHIL_STOP_SEC_VAR_16BIT
#include "MemMap.h"
/* ========================================================================= */
#define BMCHIL_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   #ifdef BMCHIL_Pwm_CH_0
   Struct_InPwm pwm_data_ch0;
   #endif

   #ifdef BMCHIL_Pwm_CH_1
   Struct_InPwm pwm_data_ch1;
   #endif

   #ifdef BMCHIL_Pwm_CH_2
   Struct_InPwm pwm_data_ch2;
   #endif

   #ifdef BMCHIL_Pwm_CH_3
   Struct_InPwm pwm_data_ch3;
   #endif

   #ifdef BMCHIL_Pwm_CH_4
   Struct_InPwm pwm_data_ch4;
   #endif

   #ifdef BMCHIL_Pwm_CH_5
   Struct_InPwm pwm_data_ch5;
   #endif

   #ifdef BMCHIL_Pwm_CH_6
   Struct_InPwm pwm_data_ch6;
   #endif

   Struct_PT1Fil  PT1Fil_UPwrSply;
   Struct_PT1Fil  PT1Fil_IPwrSply;

   Adc_ValueGroupType BMCHIL_AdcResult1[16];
#define BMCHIL_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
/* ========================================================================= */

/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */

#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn ClcdDutyPerdInPwm(void)
 *
 * @details This function assign the PWM pin for input PWM measurement and calls the PWM calculation function. \n
 * This function is called every 1ms.
 *
 * @param none
 *
 * @return void
 */
void ClcdDutyPerdInPwm(void)
{
#ifdef BMCHIL_Pwm_CH_0
   pwm_data_ch0.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_0;
   InPwmClcd(&pwm_data_ch0);
#endif
#ifdef BMCHIL_Pwm_CH_1
   pwm_data_ch1.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_1;
   InPwmClcd(&pwm_data_ch1);
#endif
#ifdef BMCHIL_Pwm_CH_2
   pwm_data_ch2.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_2;
   InPwmClcd(&pwm_data_ch2);
#endif
#ifdef BMCHIL_Pwm_CH_3
   pwm_data_ch3.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_3;
   InPwmClcd(&pwm_data_ch3);
#endif
#ifdef BMCHIL_Pwm_CH_4
   pwm_data_ch4.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_4;
   InPwmClcd(&pwm_data_ch4);
#endif
#ifdef BMCHIL_Pwm_CH_5
   pwm_data_ch5.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_5;
   InPwmClcd(&pwm_data_ch5);
#endif
#ifdef BMCHIL_Pwm_CH_6
   pwm_data_ch6.PwmInPin =  BMCHIL_DIO_PIN_Pwm_CH_6;
   InPwmClcd(&pwm_data_ch6);
#endif
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn BMCHIL_AnDigCnvrOutClcd_PwrSply_UI(void)
 *
 * @details This function assign the necessary configuration and calculates the voltage and the current from MC power supply. \n
 * This function is called every 10ms.
 *
 * @param none
 *
 * @return void
 */
void BMCHIL_AnDigCnvrOutClcd_PwrSply_UI(void)
{
   PT1Fil_UPwrSply.factor = BMCHIL_PrmFacFil;
   PT1Fil_UPwrSply.v_ADC_meas = BMCHIL_UAnDigCnvrMeasd_PwrSply;
   ClcdPT1Fil(&PT1Fil_UPwrSply);
   BMCHIL_UPwrSplyData = PT1Fil_UPwrSply.v_ADC_out;

   PT1Fil_IPwrSply.factor = BMCHIL_PrmFacFil;
   PT1Fil_IPwrSply.v_ADC_meas = BMCHIL_IAnDigCnvrMeasd_PwrSply;
   ClcdPT1Fil(&PT1Fil_IPwrSply);
   BMCHIL_IPwrSplyData = PT1Fil_IPwrSply.v_ADC_out;
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/


/* ========== Global Functions ============================================= */
#define BMCHIL_START_SEC_CODE_INIT
#include "MemMap.h"
void BMCHIL_Ini(void)
{
   BMCHIL_PrmFacFil = 0xFFFF;
}
#define BMCHIL_STOP_SEC_CODE_INIT
#include "MemMap.h"
/* ========== END BMCHIL_RstInit =========================================== */


#define BMCHIL_START_SEC_CODE_1MS
#include "MemMap.h"
void BMCHIL_Scheduler_1ms(void)
{
   ClcdDutyPerdInPwm();
}
#define BMCHIL_STOP_SEC_CODE_1MS
#include "MemMap.h"
/* ========== END BMCHIL_Scheduler_1ms ===================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_Scheduler_10ms(void)
{
   BMCHIL_AnDigCnvrOutClcd_PwrSply_UI();
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_Scheduler_10ms ==================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetMdulCtlrPwrSplyEna(boolean PrmFlagEna)
{
   if (PrmFlagEna)
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetMdulCtlrPwrSplyEna, STD_HIGH); }
   else
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetMdulCtlrPwrSplyEna, STD_LOW);  }
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetMdulCtlrPwrSplyEna ============================= */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetFacFilPwrSply(uint16 PrmFacFil_UI)
{
   BMCHIL_PrmFacFil = PrmFacFil_UI;
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetFacFilPwrSply ================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BMCHIL_GetUMdulCtlrPwrSply(void)
{
   return MLIB_Uint16Uint32(MLIB_Uint32MulUint16Uint16(BMCHIL_CfgFacUMdulPwrSply, BMCHIL_UPwrSplyData) / 0x2000u); /* div 8192 - RightShift 13 */
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
 /* ========== END BMCHIL_GetUMdulCtlrPwrSply =============================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BMCHIL_GetIMdulCtlrPwrSply(void)
{
   return MLIB_Uint16Uint32(MLIB_Uint32MulUint16Uint16(BMCHIL_CfgFacCnvnIMdulPwrSply, MLIB_Uint16SubUint16Uint16(BMCHIL_IPwrSplyData, BMCHIL_CfgUOfsIMdul)) / 0x2000u); /* div 8192 - RightShift 13 */
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_GetIMdulCtlrPwrSply =============================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetPwmHiVIntlc(uint16 PrmTiPerd, uint16 PrmDutyCyc)
{
   /* Pwm_SetPeriodAndDuty(channel_id, period, dutycycle) */
   /* period - This element is the period of the Pwm channel in ticks [2 .. 65535] */
   /* dutycycle - This element defines the initial dutycycle of the Pwm [0..0x8000] */

   if (PrmTiPerd > 20000u /* 200ms (5Hz) */)
   { PrmTiPerd = 20000u; }
   else
   { /* do not change the inserted value - do nothing */ }
   Pwm_SetPeriodAndDuty(BMCHIL_CfgPwmCh_SetPwmHiVIntlc, PrmTiPerd * BMCHIL_CfgPwmCnvnFac_SetPwmHiVIntlc, PrmDutyCyc >> 1 /* div 2 */);
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetPwmHiVIntlc ==================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetWdgOutSig(boolean PrmFlagEna)
{
   if (PrmFlagEna)
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetWdgOutSig, STD_HIGH); }
   else
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetWdgOutSig, STD_LOW); }

}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetWdgOutSig ====================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetCurSnsrTrig(boolean PrmFlagEna)
{
   if (PrmFlagEna)
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetCurSnsrTrig, STD_HIGH);  }
   else
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetCurSnsrTrig, STD_LOW); }
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetCurSnsrTrig ==================================== */
#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetMdulCtlrIdReq(boolean PrmFlagEna)
{
   if (PrmFlagEna)
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetMdulCtlrIdReqLine, STD_HIGH); }
   else
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SetMdulCtlrIdReqLine, STD_LOW);  }
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetMdulCtlrIdReq ================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 BMCHIL_GetWdgHwSts(uint8 PrmIdx)
{
   uint16 TmpCh = 0;

   switch(PrmIdx)
   {
#ifdef BMCHIL_Pwm_CH_0
      case 0:
         TmpCh = BMCHIL_DIO_PIN_HwIntlc_0;
          break;
#endif

#ifdef BMCHIL_Pwm_CH_1
      case 1:
         TmpCh = BMCHIL_DIO_PIN_HwIntlc_1;
         break;
#endif

#ifdef BMCHIL_Pwm_CH_2
      case 2:
         TmpCh = BMCHIL_DIO_PIN_HwIntlc_2;
         break;
#endif
   }
   return Dio_ReadChannel(TmpCh);
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_GetWdgHwSts ======================================= */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BMCHIL_GetPwmTiPerd(uint8 PrmIdx)
{
   uint16 PrmTiPerd = 0;
   if (BMCHIL_CfgMeasPwmEna == 1)
   {
      switch(PrmIdx)
      {
#ifdef BMCHIL_Pwm_CH_0
         case 0:
            PrmTiPerd = pwm_data_ch0.InPerd;
            break;
#endif

#ifdef BMCHIL_Pwm_CH_1
         case 1:
            PrmTiPerd = pwm_data_ch1.InPerd;
            break;
#endif

#ifdef BMCHIL_Pwm_CH_2
         case 2:
            PrmTiPerd = pwm_data_ch2.InPerd;
            break;
#endif

#ifdef BMCHIL_Pwm_CH_3
         case 3:
            PrmTiPerd = pwm_data_ch3.InPerd;
            break;
#endif

#ifdef BMCHIL_Pwm_CH_4
         case 4:
            PrmTiPerd = pwm_data_ch4.InPerd;
            break;
#endif

#ifdef BMCHIL_Pwm_CH_5
         case 5:
            PrmTiPerd = pwm_data_ch5.InPerd;
            break;
#endif

#ifdef BMCHIL_Pwm_CH_6
         case 6:
            PrmTiPerd = pwm_data_ch6.InPerd;
            break;
#endif
      }
   }
   else
   {
      PrmTiPerd = 0;
   }

   return PrmTiPerd;
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_GetPwmTiPerd ====================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BMCHIL_GetPwmDutyCyc(uint8 PrmIdx)
{
   uint16 PrmDutyCyc = 0;
   if (BMCHIL_CfgMeasPwmEna == 1)
      {
         switch(PrmIdx)
         {
         #ifdef BMCHIL_Pwm_CH_0
            case 0:
               PrmDutyCyc = pwm_data_ch0.InDuty;
               break;
         #endif

         #ifdef BMCHIL_Pwm_CH_1
            case 1:
               PrmDutyCyc = pwm_data_ch1.InDuty;
               break;
         #endif

         #ifdef BMCHIL_Pwm_CH_2
            case 2:
               PrmDutyCyc = pwm_data_ch2.InDuty;
               break;
         #endif

         #ifdef BMCHIL_Pwm_CH_3
            case 3:
               PrmDutyCyc = pwm_data_ch3.InDuty;
               break;
         #endif

         #ifdef BMCHIL_Pwm_CH_4
            case 4:
               PrmDutyCyc = pwm_data_ch4.InDuty;
               break;
         #endif

         #ifdef BMCHIL_Pwm_CH_5
            case 5:
               PrmDutyCyc = pwm_data_ch5.InDuty;
               break;
         #endif

         #ifdef BMCHIL_Pwm_CH_6
            case 6:
               PrmDutyCyc = pwm_data_ch6.InDuty;
               break;
         #endif
         }
      }
   else
   {
      PrmDutyCyc = 0;
   }
   return PrmDutyCyc;
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_GetPwmDutyCyc ===================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 BMCHIL_GetDigInpSts(uint8 PrmIdx)
{
   uint8  PrmFlagSts;
   uint16 TmpCh = 0;
   switch (PrmIdx)
   {
      #ifdef BMCHIL_Dig_CH_0
      case 0:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_0;
         break;
      #endif

      #ifdef BMCHIL_Dig_CH_1
      case 1:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_1;
         break;
      #endif

      #ifdef BMCHIL_Dig_CH_2
      case 2:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_2;
         break;
      #endif

      #ifdef BMCHIL_Dig_CH_3
      case 3:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_3;
         break;
      #endif

      #ifdef BMCHIL_Dig_CH_4
      case 4:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_4;
         break;
      #endif

      #ifdef BMCHIL_Dig_CH_5
      case 5:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_5;
         break;
      #endif

      #ifdef BMCHIL_Dig_CH_6
      case 6:
         TmpCh = BMCHIL_DIO_PIN_Dig_CH_6;
         break;
      #endif
   }

   if (BMCHIL_CfgMeasDigInpEna == 1)
   { PrmFlagSts = Dio_ReadChannel(TmpCh); }
   else
   { PrmFlagSts = 0; }
   return PrmFlagSts;
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_GetDigInpSts ====================================== */


#define BMCHIL_START_SEC_CODE_10MS
#include "MemMap.h"
void BMCHIL_SetDigInpDiagSig(boolean PrmFlagEna)
{
   if (PrmFlagEna)
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SnsrDiagSig, STD_HIGH); }
   else
   { Dio_WriteChannel(DIO_PIN_BMCHIL_SnsrDiagSig, STD_LOW);  }
}
#define BMCHIL_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BMCHIL_SetDigInpDiagSig ================================== */


/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif

/* undefine master define for this module    */
#ifdef MASTER_BMCHIL_MDULCTLRIFHWINTLCDRVR_C_
#undef MASTER_BMCHIL_MDULCTLRIFHWINTLCDRVR_C_
#endif

