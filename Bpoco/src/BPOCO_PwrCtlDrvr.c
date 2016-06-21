/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: BPOCO_PwrCtlDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.03.2010
 *  LAST REVISION:      $Date: 2011/03/17 15:30:39MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BPOCO_PwrCtlDrvr.c  $
 *  Revision 1.12 2011/03/17 15:30:39MEZ Cotor Vlad RGB (COTORV) 
 *  Update NvM calling function due to power control
 *  Revision 1.11 2011/03/07 16:23:20MEZ Cotor Vlad RGB (COTORV) 
 *  - After test update of xCU power control due to CTC and DEH variables
 *  Revision 1.10 2011/03/04 11:00:06MEZ Cotor Vlad RGB (COTORV) 
 *  Update NvM due to NvM scheduler
 *  Revision 1.9 2011/02/28 14:00:55MEZ Cotor Vlad RGB (COTORV)
 *  - Develop shutdown function for xCU power control
 *  Revision 1.8 2010/11/22 09:08:35MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.7 2010/11/16 13:11:38MEZ Cotor Vlad RGB (COTORV)
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.6 2010/08/27 14:32:07MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.5 2010/08/25 16:18:54MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW complex device drivers names
 *  Revision 1.4 2010/08/24 10:18:02MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver
 *  Update due to calculation correction, resolution adjustment
 *  Revision 1.3 2010/07/30 14:42:39MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver due to introduction of BPOCO scheduler
 *  Revision 1.2 2010/07/19 09:58:58MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver due to specification update
 *  Revision 1.1 2010/05/20 13:58:07MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bpoco/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_BPOCO_PWRCTLDRVR_C_    /*!< Master define for this module    */
#define  MLIB_INLINE  0

/* ========== Includes ===================================================== */
#include "Mathlib.h"
#include "regtc1767.sfr"
#include "team-s.h"

#include "Dio.h"
#include "Adc.h"
#include "NvM.h"
#include "Fee.h"
#include "Fls.h"
#include "EcuM.h"
#include "EcuM_Cfg.h"
#include "Gpt.h"

#include "NvM_Scheduler.h"
#include "BSW_DrvrAux.h"
#include "Prj_MainCfg.h"
#include "BCTC_CtctrCtlDrvr.h"
#include "BPOCO_PwrCtlDrvr.h"           /*!< module header file     */
#include "BCTC_CtctrCtlDrvr_Cfg.h"
#include "BcuComTestInterface.h"

/* ========== Enumerations, Typedefinitions, Local Defines  ================ */

typedef struct Struct_UBatUWkData_{
   uint16   UBat;
#ifdef BPOCO_WAKE_1
   uint16   UWk1;
#endif
#ifdef BPOCO_WAKE_2
   uint16   UWk2;
#endif
   uint16   UGndDig;
   uint16   UGndPwr;
}Struct_UBatUWkData;


typedef struct Struct_UPwrIPwrData_{
#ifdef BPOCO_FUSE_1
   uint16 UPwr1;
   uint16 IPwr1;
#endif
#ifdef BPOCO_FUSE_2
   uint16 UPwr2;
   uint16 IPwr2;
#endif
#ifdef BPOCO_FUSE_3
   uint16 UPwr3;
   uint16 IPwr3;
#endif
}Struct_UPwrIPwrData;


/* ========== Local function prototypes  =================================== */
void BPOCO_AnDigCnvrOutClcd_UBatUWk(void);
void BPOCO_AnDigCnvrOutClcd_UPwrIPwr(void);
void BPOCO_AnDigCnvrOutClcd_UAsi06(void);



/* ========== Local variables  ============================================= */
#define BPOCO_START_SEC_VAR_8BIT
#include "MemMap.h"
   uint8 WdgDirectionFlag = 1;
   uint8 SetWdgTiShtdnWasCalled = 0;
#define BPOCO_STOP_SEC_VAR_8BIT
#include "MemMap.h"

#define BPOCO_START_SEC_VAR_16BIT
#include "MemMap.h"
   uint16 UBatUAsi06;

   uint16 BPOCO_FacFilBat;
   uint16 BPOCO_FacFilPwr;
   uint16 BPOCO_FacFilAsi06;
   uint16 WdgTiShtdn_PrmTiLo;
   uint16 WdgTiShtdn_PrmTiHi;
#define BPOCO_STOP_SEC_VAR_16BIT
#include "MemMap.h"

#define BPOCO_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   Adc_ValueGroupType BPOCO_AdcResult0[16];
   Adc_ValueGroupType BPOCO_AdcResult1[16];

   Struct_UBatUWkData UBatUWkData;
   Struct_UPwrIPwrData UPwrIPwrData;

   Struct_PT1Fil  PT1Fil_UBatUWkUBat;
   #ifdef BPOCO_WAKE_1
   Struct_PT1Fil  PT1Fil_UBatUWkUWk1;
   #endif
   #ifdef BPOCO_WAKE_2
   Struct_PT1Fil  PT1Fil_UBatUWkUWk2;
   #endif
   Struct_PT1Fil  PT1Fil_UBatUWkUGndDig;
   Struct_PT1Fil  PT1Fil_UBatUWkUGndPwr;

   #ifdef BPOCO_FUSE_1
   Struct_PT1Fil  PT1Fil_UPwrIPwrUPwr1;
   Struct_PT1Fil  PT1Fil_UPwrIPwrIPwr1;
   #endif
   #ifdef BPOCO_FUSE_2
   Struct_PT1Fil  PT1Fil_UPwrIPwrUPwr2;
   Struct_PT1Fil  PT1Fil_UPwrIPwrIPwr2;
   #endif
   #ifdef BPOCO_FUSE_3
   Struct_PT1Fil  PT1Fil_UPwrIPwrUPwr3;
   Struct_PT1Fil  PT1Fil_UPwrIPwrIPwr3;
   #endif

   Struct_PT1Fil  PT1Fil_UAsi06;

#define BPOCO_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"


/* ========== Local macros ================================================= */



/* ========== Local functions ============================================== */
#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BPOCO_AnDigCnvrOutClcd_UBatUWk(void)
 *
 * @details This function calculates the voltages from battery supply, wake1 line, wake2 line, digital ground and digital power.
 *
 * @param none
 *
 * @return void
 */
void BPOCO_AnDigCnvrOutClcd_UBatUWk(void)
{
/* calculation of UBatUWkData.UBat */
   PT1Fil_UBatUWkUBat.factor = BPOCO_FacFilBat;
   PT1Fil_UBatUWkUBat.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UBat;
   ClcdPT1Fil(&PT1Fil_UBatUWkUBat);
   UBatUWkData.UBat = PT1Fil_UBatUWkUBat.v_ADC_out;

#ifdef BPOCO_WAKE_1
/* calculation of UBatUWkData.UWk1 */
   PT1Fil_UBatUWkUWk1.factor = BPOCO_FacFilBat;
   PT1Fil_UBatUWkUWk1.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UWk1;
   ClcdPT1Fil(&PT1Fil_UBatUWkUWk1);
   UBatUWkData.UWk1 = PT1Fil_UBatUWkUWk1.v_ADC_out;
#endif

#ifdef BPOCO_WAKE_2
/* calculation of UBatUWkData.UWk2 */
   PT1Fil_UBatUWkUWk2.factor = BPOCO_FacFilBat;
   PT1Fil_UBatUWkUWk2.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UWk2;
   ClcdPT1Fil(&PT1Fil_UBatUWkUWk2);
   UBatUWkData.UWk2 = PT1Fil_UBatUWkUWk2.v_ADC_out;
#endif

/* calculation of UBatUWkData.UGndDig */
   PT1Fil_UBatUWkUGndDig.factor = BPOCO_FacFilBat;
   PT1Fil_UBatUWkUGndDig.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UGndDig;
   ClcdPT1Fil(&PT1Fil_UBatUWkUGndDig);
   UBatUWkData.UGndDig = PT1Fil_UBatUWkUGndDig.v_ADC_out;

/* calculation of UBatUWkData.UGndPwr */
   PT1Fil_UBatUWkUGndPwr.factor = BPOCO_FacFilBat;
   PT1Fil_UBatUWkUGndPwr.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UGndPwr;
   ClcdPT1Fil(&PT1Fil_UBatUWkUGndPwr);
   UBatUWkData.UGndPwr = PT1Fil_UBatUWkUGndPwr.v_ADC_out;
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BPOCO_AnDigCnvrOutClcd_UPwrIPwr(void)
 *
 * @details This function calculates the fused power voltages.
 *
 * @param none
 *
 * @return void
 */
void BPOCO_AnDigCnvrOutClcd_UPwrIPwr(void)
{
#ifdef BPOCO_FUSE_1
/* calculation of UPwrIPwrData.UPwr1 */
   PT1Fil_UPwrIPwrUPwr1.factor = BPOCO_FacFilPwr;
   PT1Fil_UPwrIPwrUPwr1.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UPwr1;
   ClcdPT1Fil(&PT1Fil_UPwrIPwrUPwr1);
   UPwrIPwrData.UPwr1 = PT1Fil_UPwrIPwrUPwr1.v_ADC_out;

/* calculation of UPwrIPwrData.IPwr1 */
   PT1Fil_UPwrIPwrIPwr1.factor = BPOCO_FacFilPwr;
   PT1Fil_UPwrIPwrIPwr1.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_IPwr1;
   ClcdPT1Fil(&PT1Fil_UPwrIPwrIPwr1);
   UPwrIPwrData.IPwr1 = PT1Fil_UPwrIPwrIPwr1.v_ADC_out;
#endif

#ifdef BPOCO_FUSE_2
/* calculation of UPwrIPwrData.UPwr2 */
   PT1Fil_UPwrIPwrUPwr2.factor = BPOCO_FacFilPwr;
   PT1Fil_UPwrIPwrUPwr2.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UPwr2;
   ClcdPT1Fil(&PT1Fil_UPwrIPwrUPwr2);
   UPwrIPwrData.UPwr2 = PT1Fil_UPwrIPwrUPwr2.v_ADC_out;

/* calculation of UPwrIPwrData.IPwr2 */
   PT1Fil_UPwrIPwrIPwr2.factor = BPOCO_FacFilPwr;
   PT1Fil_UPwrIPwrIPwr2.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_IPwr2;
   ClcdPT1Fil(&PT1Fil_UPwrIPwrIPwr2);
   UPwrIPwrData.IPwr2 = PT1Fil_UPwrIPwrIPwr2.v_ADC_out;
#endif

#ifdef BPOCO_FUSE_3
/* calculation of UPwrIPwrData.UPwr3 */
   PT1Fil_UPwrIPwrUPwr3.factor = BPOCO_FacFilPwr;
   PT1Fil_UPwrIPwrUPwr3.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UPwr3;
   ClcdPT1Fil(&PT1Fil_UPwrIPwrUPwr3);
   UPwrIPwrData.UPwr3 = PT1Fil_UPwrIPwrUPwr3.v_ADC_out;

/* calculation of UPwrIPwrData.IPwr3 */
   PT1Fil_UPwrIPwrIPwr3.factor = BPOCO_FacFilPwr;
   PT1Fil_UPwrIPwrIPwr3.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_IPwr3;
   ClcdPT1Fil(&PT1Fil_UPwrIPwrIPwr3);
   UPwrIPwrData.IPwr3 = PT1Fil_UPwrIPwrIPwr3.v_ADC_out;
#endif
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/

/* ========== Local functions ============================================== */
#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BPOCO_AnDigCnvrOutClcd_UAsi06(void)
 *
 * @details This function calculates the voltages from ASI06  Refrost rely
 *
 * @param none
 *
 * @return void
 */
void BPOCO_AnDigCnvrOutClcd_UAsi06(void)
{
/* calculation of UASI06 */
   PT1Fil_UAsi06.factor = BPOCO_FacFilAsi06;
   PT1Fil_UAsi06.v_ADC_meas = BPOCO_UAnDigCnvrMeasd_UASI06;
   ClcdPT1Fil(&PT1Fil_UAsi06);
   UBatUAsi06 = PT1Fil_UAsi06.v_ADC_out;
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/


/* ========== Global functions ============================================= */

#define BPOCO_START_SEC_CODE_INIT
#include "MemMap.h"
void BPOCO_Ini(void)
{
   BPOCO_FacFilBat = 0xFFFF;
   BPOCO_FacFilPwr = 0xFFFF;
   BPOCO_FacFilAsi06 = 0xFFFF;
}
#define BPOCO_STOP_SEC_CODE_INIT
#include "MemMap.h"
/* ========== END BPOCO_RstInit ============================================ */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
   void BPOCO_Scheduler_10ms(void)
   {
      BPOCO_AnDigCnvrOutClcd_UBatUWk();
      BPOCO_AnDigCnvrOutClcd_UPwrIPwr();
      BPOCO_AnDigCnvrOutClcd_UAsi06();
   }
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_Scheduler_10ms ===================================== */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetPwrSplyEna(boolean PrmFlagEna)
{
	if (PrmFlagEna)
	{ Dio_WriteChannel(DIO_PIN_BPOCO_SetPwrSplyEna, STD_HIGH); }
	else
	{ Dio_WriteChannel(DIO_PIN_BPOCO_SetPwrSplyEna, STD_LOW);  }
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetPwrSplyEna ====================================== */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetWdgSig(boolean PrmFlagEna)
{
	if (PrmFlagEna)
	{ Dio_WriteChannel(DIO_PIN_BPOCO_SetWdgSig, STD_HIGH); }
	else
	{ Dio_WriteChannel(DIO_PIN_BPOCO_SetWdgSig, STD_LOW);	 }
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetWdgSig ========================================== */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetRlyPwrEna(boolean PrmFlagEna)
{
	BCTC_SetPwmLS10(PrmFlagEna);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetRlyPwrEna ======================================= */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetFacFilUAdcBat(uint16 PrmFacFil_Bat)
{
   BPOCO_FacFilBat = PrmFacFil_Bat;
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetFacFilUAdcBat =================================== */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetFacFilUAdcPwr(uint16 PrmFacFil_Pwr)
{
   BPOCO_FacFilPwr = PrmFacFil_Pwr;
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetFacFilUAdcPwr =================================== */

#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetFacFilUAdcAsi06(uint16 PrmFacFil_Asi)
{
	BPOCO_FacFilAsi06 = PrmFacFil_Asi;
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetFacFilUAdcPwr =================================== */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BPOCO_GetUBat(void)
{
/*
 * 0..FFFF         0..FFFF            0..FFFF
 * 0..39.9999426   0..63.99902344     0..4.99966515
 * PrmU (OUT)    = BPOCO_CfgFacUBat * UBatUWkData.UBat;
 */
	return MLIB_Uint16Uint32(((uint32)BPOCO_CfgFacUBat * UBatUWkData.UBat) >> 13);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUBat ============================================ */

#ifdef BPOCO_WAKE_1
#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BPOCO_GetUWk1(void)
{
/*
 * 0..FFFF         0..FFFF            0..FFFF
 * 0..39.9999426   0..63.99902344     0..4.99966515
 * PrmU (OUT)    = BPOCO_CfgFacUBat * UBatUWkData.UWk1;
 */
   return MLIB_Uint16Uint32(((uint32)BPOCO_CfgFacUBat * UBatUWkData.UWk1) >> 13);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUWk1 ============================================ */
#endif


#ifdef BPOCO_WAKE_2
#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BPOCO_GetUWk2(void)
{
/*
 * 0..FFFF         0..FFFF            0..FFFF
 * 0..39.9999426   0..63.99902344     0..4.99966515
 * PrmU (OUT)    = BPOCO_CfgFacUBat * UBatUWkData.UWk2;
 */
	return MLIB_Uint16Uint32(((uint32)BPOCO_CfgFacUBat * UBatUWkData.UWk2) >> 13);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUWk2 ============================================ */
#endif


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
sint16 BPOCO_GetUGndDig(void)
{
   return MLIB_Sint16Sint32(MLIB_Sint32MulSint16Uint16(MLIB_Sint16SubUint16Uint16(UBatUWkData.UGndDig, BPOCO_CfgUOfsGnd), (uint16)BPOCO_CfgFacUGnd) >> 10);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUGndDig ========================================= */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
sint16 BPOCO_GetUGndPwr(void)
{
   return MLIB_Sint16Sint32(MLIB_Sint32MulSint16Uint16(MLIB_Sint16SubUint16Uint16(UBatUWkData.UGndPwr, BPOCO_CfgUOfsGnd), (uint16)BPOCO_CfgFacUGnd) >> 10);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUGndDig ========================================= */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BPOCO_GetUPwr(uint8 PrmIdx)
{
	uint16 UPwrIPwrData_tmp = 0;
	switch(PrmIdx)
	{
#ifdef BPOCO_FUSE_1
	   case 0:       /* 1st fuse */
	      UPwrIPwrData_tmp = UPwrIPwrData.UPwr1;
	      break;
#endif

#ifdef BPOCO_FUSE_2
	   case 1:       /* 2nd fuse */
	      UPwrIPwrData_tmp = UPwrIPwrData.UPwr2;
         break;
#endif

#ifdef BPOCO_FUSE_3
      case 2:       /* 3rd fuse */
         UPwrIPwrData_tmp = UPwrIPwrData.UPwr3;
         break;
#endif
	}

	return MLIB_Uint16Uint32(((uint32)BPOCO_CfgFacUBat * UPwrIPwrData_tmp) >> 13);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUPwr ============================================ */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"

sint16 BPOCO_GetIPwr(uint8 PrmIdx)
{
	uint16 UBatUWkData_tmp = 0;
   switch(PrmIdx)
   {
#ifdef BPOCO_FUSE_1
      case 0:
         UBatUWkData_tmp = UPwrIPwrData.IPwr1;
         break;
#endif
#ifdef BPOCO_FUSE_2
      case 1:
         UBatUWkData_tmp = UPwrIPwrData.IPwr2;
         break;
#endif
#ifdef BPOCO_FUSE_3
      case 2:
         UBatUWkData_tmp = UPwrIPwrData.IPwr3;
         break;
#endif
   }

   return MLIB_Sint16Sint32(MLIB_Sint32MulSint16Uint16(MLIB_Sint16SubUint16Uint16(UBatUWkData_tmp, BPOCO_CfgUOfsIPwr), (uint16)BPOCO_CfgFacCnvnIPwr) >> 14);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetIPwr ============================================ */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_ShtdnEcu(void)
{
/*
   if(SetWdgTiShtdnWasCalled == 0u)
   {  do not change the watchdog signal   }
   else
   {
      if ((WdgTiShtdn_PrmTiLo == 0u) && (WdgTiShtdn_PrmTiHi == 0u))
      {  do not change the watchdog signal   }
      else
      {
         if ((WdgTiShtdn_PrmTiLo == 0u) && (WdgTiShtdn_PrmTiHi != 0u))
         { Dio_WriteChannel(DIO_PIN_BPOCO_SetWdgSig, STD_HIGH); }
         else
         {
            if ((WdgTiShtdn_PrmTiLo != 0u) && (WdgTiShtdn_PrmTiHi == 0u))
            { Dio_WriteChannel(DIO_PIN_BPOCO_SetWdgSig, STD_LOW); }  ATTENTION: In this case, a watchdog reset will be started !!!
            else
            {
                Start timer for Wdg triggering
               Gpt_StartTimer(2u, GPT_TIMER_VALUE(WdgTiShtdn_PrmTiLo));
               Gpt_EnableNotification(2u);
            }
         }
      }
*/

      /* converting the float nvmy variables into unsigned integer */
      NvM_WriteAll_for_App();

      /*Disable TJA1041(Veh CAN)*/
      Dio_WriteChannel(DIO_CHANNEL_ID(DIO_PORT_1, 13), STD_LOW);
      Dio_WriteChannel(DIO_CHANNEL_ID(DIO_PORT_1, 12), STD_LOW);

      /* Release the self-sustaining line */
      BPOCO_SetPwrSplyEna(0u);


      /* Start timer for releasing the self-sustaining line from BCU  */
      Gpt_StartTimer(3u, GPT_TIMER_VALUE(BPOCO_CfgTiWaitShtdn));
      Gpt_EnableNotification(3u);

      while(1)
         {/* Waiting for SW reset */};
  // }
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_ShtdnEcu =========================================== */


#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
void BPOCO_SetWdgTiShtdn(uint16 PrmTiLo, uint16 PrmTiHi)
{
   SetWdgTiShtdnWasCalled = 1;
   WdgTiShtdn_PrmTiLo = PrmTiLo;
   WdgTiShtdn_PrmTiHi = PrmTiHi;
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_SetWdgTiShtdn ====================================== */


void WgdTrig_GptNotification(void)
{
   uint16 WdgTiShtdn_PrmTi_tmp;

   if(WdgDirectionFlag == 1)
   {
      Dio_WriteChannel(DIO_PIN_BPOCO_SetWdgSig, STD_HIGH);
      WdgDirectionFlag = 0;
      WdgTiShtdn_PrmTi_tmp = WdgTiShtdn_PrmTiHi;
   }
   else
   {
      Dio_WriteChannel(DIO_PIN_BPOCO_SetWdgSig, STD_LOW);
      WdgDirectionFlag = 1;
      WdgTiShtdn_PrmTi_tmp = WdgTiShtdn_PrmTiLo;
   }
   if (WdgTiShtdn_PrmTiLo != WdgTiShtdn_PrmTiHi)
   {
      Gpt_StopTimer(2u);
      Gpt_StartTimer(2u, GPT_TIMER_VALUE(WdgTiShtdn_PrmTi_tmp));
      Gpt_EnableNotification(2u);
   }
   else
   { /*everything is ok proceed*/}
}
/* ========== END WgdTrig_GptNotification ================================= */

void ShtdnEcuWait_GptNotification(void)
{
   /* SW Reset generation */
   ts_endinit_clear();
   SCU_SWRSTCON.B.SWRSTREQ = 1u;    /* software reset - TC1767_UM_1.1 - page 3-60 */
   /* TODO: -- Attention: external Wdg if Timeout occurs = open point */
}
/* ========== END ShtdnEcuWait_GptNotification ============================= */

#define BPOCO_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BPOCO_GetUASI06(void)
{
/*
 * 0..FFFF         0..FFFF            0..FFFF
 * 0..39.9999426   0..63.99902344     0..4.99966515
 * PrmU (OUT)    = BPOCO_CfgFacUBat * UBatUWkData.UBat;
 */
	return MLIB_Uint16Uint32(((uint32)BPOCO_CfgFacUAsi06 * UBatUAsi06) >> 13);
}
#define BPOCO_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BPOCO_GetUBat ============================================ */

/*!< undefine master define for this module    */
#ifdef MASTER_BPOCO_PWRCTLDRVR_C_
#undef MASTER_BPOCO_PWRCTLDRVR_C_
#endif

