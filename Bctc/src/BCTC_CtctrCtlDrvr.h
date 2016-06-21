/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: BCTC_CtctrCtlDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.03.2010
 *  LAST REVISION:      $Date: 2010/12/13 08:35:58MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BCTC_CtctrCtlDrvr.h  $
 *  Revision 1.7 2010/12/13 08:35:58MEZ Cotor Vlad RGB (COTORV) 
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.6 2010/11/22 07:16:35MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.5 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.4 2010/09/24 18:07:03MESZ Cotor Vlad RGB (COTORV) 
 *  Update scheduling behavior and add deactivation of programmable gain to extend measurement range
 *  Reorganize the SPI Configuration switching to macros
 *  Revision 1.3 2010/07/23 16:13:03MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification change
 *  Revision 1.2 2010/07/19 09:56:55MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification update
 *  Revision 1.1 2010/05/20 14:01:46MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bctc/src/project.pj
 *
 * ***************************************************************************/

#ifndef BCTC_CTCTRCTLDRVR_H_
#define BCTC_CTCTRCTLDRVR_H_

/* ========== Includes ================================================ */
#include "Std_Types.h"     /*!< Contains the basic data types. */

#include "Dio.h"
#include "Dio_Cfg.h"
#include "Spi.h"
#include "Adc.h"

#include "BSW_DrvrAux.h"
#include "BCTC_CtctrCtlDrvr_Cfg.h"

#include "Prj_MainCfg.h"


/* ========== Declare Alternate Control _EXTERNAL_ definition ========= */
#ifdef  MASTER_BCTC_CTCTRCTLDRVR_C_  /*! Read from associated modul ?    */
   #define  EXTERNAL_           /*! yes: associated   */
#else
   #define  EXTERNAL_  extern   /*! no: extern modul  */
#endif

/** @defgroup group_bctc BCTC
 *  @ingroup group_bctc
 *
 *  Basic software for contactor control complex device driver */
/*@{*/

/** @page bctc BCTC
 *
 * @section bctc_intro Introduction
 *  @li This driver shall handle all basic software activities related to the main contactor
 *  control for battery control units.
 *  @li In this version, intelligent contactors shall be used, which are either reducing the
 *  primary current on their own or do not need any current reduction at all. Hence, no peak
 *  and hold functionality has to be implemented in software. Nevertheless, the interfaces to
 *  the application layer shall be defined in a way that will allow a peak and hold
 *  implementation later on without changing the interfaces.
 *  @li The general task of this module is to close and open each of the three main contactors
 *  if commanded by the application layer using lower level basic software modules like Dio,
 *  Adc, Spi and so on. In addition, all related current and voltage measurements shall be handled.
 *  This includes primary current measurement of the contactors as well as high voltage measurement
 *  for contactor diagnosis.
 *
 */
/* ========== Enumerations, Typedefinitions =========================== */
typedef struct TLE8104_chip_
{
   uint16 rx_buffer_UL2;
   uint16 rx_buffer_UL3;
   uint16 rx_buffer_UL4;
}Struct_TLE8104;


/* ========== Global variables  ======================================= */
#define BCTC_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   extern Struct_TLE8104 TLE8104;
#define BCTC_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"


#define BCTC_START_SEC_VAR_BOOLEAN
#include "MemMap.h"
   extern boolean PrmFlagClsPos;
   extern boolean PrmFlagClsNeg;
   extern boolean PrmFlagClsPrec;
#define BCTC_STOP_SEC_VAR_BOOLEAN
#include "MemMap.h"

/* ========== Global macros =========================================== */



/* ========== Global function prototypes  ============================= */
#define BCTC_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void BCTC_RstInit(void);
 *
 * @details This function is called at RESET from operating system's StartUpHook and initialize all needed values.
 * If the initialization values are not specified, the variables will be initialized with 0.
 *
 * @param none
 *
 * @return void
 */
   extern void    BCTC_Ini(void);
/*****************************************************************************/
#define BCTC_STOP_SEC_CODE_INIT
#include "MemMap.h"


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BCTC_Scheduler_1ms(void)
 *
 * @details The 1ms scheduler function will be called from operating system in 1ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BCTC_Scheduler_1ms(void);
/*****************************************************************************/
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BCTC_Scheduler_10ms(void)
 *
 * @details The 10ms scheduler function will be called from operating system in 10ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BCTC_Scheduler_10ms(void);
/******************************************************************************/
/**
 * @fn void BCTC_SetPwmCtctrPos(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld)
 *
 * @details This interface closes/opens the positive high voltage contactor (HV+)
 * and sets the peak and hold PWM and time parameters in case the contactor is closed.
 *
 * @param boolean PrmFlagCls - This flag indicates, that the contactor shall be closed (=1) or opened (=0). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @param uint8 PrmPwmPeak - This parameter sets the pulse width of the contactor during the peak phase when the contactor is closed. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 99.609375 \n
 * unit: [%] \n
 *
 * @param uint16 PrmTiPeak - This parameter sets the duration of the peak current phase for the contactor after the contactor is closed. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [ms] \n
 *
 * @param uint8 PrmPwmHld - This parameter sets the pulse width of the contactor after the peak phase when the contactor is closed. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 99.609375 \n
 * unit: [%] \n
 *
 * @note The definition of range and resolution of the parameters PrmPwmPeak, PrmTiPeak and PrmPwmHld is preliminary,
 * since those parameters are not used in this release.
 *
 * @return void
 */
extern void BCTC_SetPwmCtctrPos(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld);
/******************************************************************************/
/**
 * @fn void BCTC_SetPwmCtctrNeg(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld)
 *
 * @details This interface closes/opens the negative high voltage contactor (HV-)
 * and sets the peak and hold PWM and time parameters in case the contactor is closed.
 *
 * @param boolean PrmFlagCls - This flag indicates, that the contactor shall be closed (=1) or opened (=0). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @param uint8 PrmPwmPeak - This parameter sets the pulse width of the contactor during the peak phase when the contactor is closed. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 99.609375 \n
 * unit: [%] \n
 *
 * @param uint16 PrmTiPeak - This parameter sets the duration of the peak current phase for the contactor after the contactor is closed. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [ms] \n
 *
 * @param uint8 PrmPwmHld - This parameter sets the pulse width of the contactor after the peak phase when the contactor is closed. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 99.609375 \n
 * unit: [%] \n
 *
 * @note The definition of range and resolution of the parameters PrmPwmPeak, PrmTiPeak and PrmPwmHld is preliminary,
 * since those parameters are not used in this release.
 *
 * @return void
 */
extern void BCTC_SetPwmCtctrNeg(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld);
/******************************************************************************/
/**
 * @fn void BCTC_SetPwmCtctrPrec(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld)
 *
 * @details This interface closes/opens the precharge contactor (Prec)
 * and sets the peak and hold PWM and time parameters in case the contactor is closed.
 *
 * @param boolean PrmFlagCls - This flag indicates, that the contactor shall be closed (=1) or opened (=0). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 *
 * @param uint8 PrmPwmPeak - This parameter sets the pulse width of the contactor during the peak phase when the contactor is closed. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 99.609375 \n
 * unit: [%] \n
 *
 * @param uint16 PrmTiPeak - This parameter sets the duration of the peak current phase for the contactor after the contactor is closed. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [ms] \n
 *
 * @param uint8 PrmPwmHld - This parameter sets the pulse width of the contactor after the peak phase when the contactor is closed. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 99.609375 \n
 * unit: [%] \n
 *
 * @note The definition of range and resolution of the parameters PrmPwmPeak, PrmTiPeak and PrmPwmHld is preliminary,
 * since those parameters are not used in this release.
 *
 * @return void
 */
extern void BCTC_SetPwmCtctrPrec(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld);
/******************************************************************************/
/**
 * @fn void BCTC_SetGainADC1(uint8 PrmGain)
 *
 * @details This interface sets the programmable gain of the voltage measurement of the AS8510_1.
 *
 * @param uint8 PrmGain - Programmable gain of the voltage measurement (allowed values are 1, 5, 25, 40 and 100). \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BCTC_SetGainADC1(uint8 PrmGain);
/******************************************************************************/
/**
 * @fn void BCTC_SetGainADC2(uint8 PrmIdx, uint8 PrmGain)
 *
 * @details This interface sets the programmable gain of the voltage measurement of the AS8510_2.
 *
 * @param uint8 PrmIdx - Index of multiplexer channel. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @param uint8 PrmGain - Programmable gain of the voltage measurement (allowed values are 1, 5, 25, 40 and 100). \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BCTC_SetGainADC2(uint8 PrmIdx, uint8 PrmGain);
/******************************************************************************/
/**
 * @fn uint16 BCTC_GetUMeasDev1(uint8 PrmIdx)
 *
 * @details This interface gets the measured voltage of the AS8510_1 in a buffer
 * (first element of the buffer is the most recent sample).
 *
 * @param uint8 PrmIdx - ring buffer index
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return PrmU[BCTC_CfgNoElmMeasBuf1] - Buffered voltage, measured by AS8510_1. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 655.35 \n
 * unit: [V] \n
 */
extern uint16 BCTC_GetUMeasDev1(uint8 PrmIdx);
/******************************************************************************/

/**
 * @fn uint16 BCTC_GetUMeasDev2(uint8 PrmIdx)
 *
 * @details This interface gets the measured voltage of the AS8510_2 (multiplexer channel PrmIdx).
 *
 * @param uint8 PrmIdx - Index of multiplexer channel. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmU - Measured voltage of multiplexer channel PrmIdx (ADC register content). \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [V] \n
 */
extern uint16 BCTC_GetUMeasDev2(uint8 PrmIdx);
/******************************************************************************/
/**
 * @fn uint16 BCTC_GetUReslMeasDev2(uint8 PrmIdx)
 *
 * @details This interface gets the resolution of the measured voltage of the AS8510_2 (multiplexer channel PrmIdx).
 *
 * @param uint8 PrmIdx - Index of multiplexer channel. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmUResl - Resolution of measured ADC voltage of multiplexer channel PrmIdx. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 65.535e-3 \n
 * unit: [V] \n
 */
extern uint16 BCTC_GetUReslMeasDev2(uint8 PrmIdx);
/******************************************************************************/
/**
 * @fn uint16 BCTC_GetICtctrPos(uint8 PrmIdx)
 *
 * @details This interface gets the measured primary current of the HV+ contactor in a buffer
 * (first element of the buffer is the most recent sample).
 *
 * @param uint8 PrmIdx - ring buffer index
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmI[BCTC_CfgNoElmMeasBufI] - Buffered current. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 79.9987793 \n
 * unit: [A] \n
 */
extern uint16 BCTC_GetICtctrPos(uint8 PrmIdx);
/******************************************************************************/
/**
 * @fn uint16 BCTC_GetICtctrNeg(uint8 PrmIdx)
 *
 * @details This interface gets the measured primary current of the HV- contactor in a buffer
 * (first element of the buffer is the most recent sample).
 *
 * @param uint8 PrmIdx - ring buffer index
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmI[BCTC_CfgNoElmMeasBufI] - Buffered current. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 79.9987793 \n
 * unit: [A] \n
 */
extern uint16 BCTC_GetICtctrNeg(uint8 PrmIdx);
/******************************************************************************/
/**
 * @fn uint16 BCTC_GetICtctrPrec(uint8 PrmIdx)
 *
 * @details This interface gets the measured primary current of the precharge contactor in a buffer
 * (first element of the buffer is the most recent sample).
 *
 * @param uint8 PrmIdx - ring buffer index
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmI[BCTC_CfgNoElmMeasBufI] - Buffered current. \n
 * hex. limits: 0H : FFFFH \n
 * phys. limits: 0 : 79.9987793 \n
 * unit: [A] \n
 */
extern uint16 BCTC_GetICtctrPrec(uint8 PrmIdx);
/******************************************************************************/
/**
 * @fn boolean BCTC_GetErrEltlHiCtctrPos(void)
 *
 * @details This interface gets the electrical error status of the high side driver used to control the HV+ contactor.
 *
 * @param none
 *
 * @return boolean PrmFlagErr - Error status (1: error, 0: no error). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 */
extern boolean BCTC_GetErrEltlHiCtctrPos(void);
/******************************************************************************/
/**
 * @fn boolean BCTC_GetErrEltlHiCtctrNeg(void)
 *
 * @details This interface gets the electrical error status of the high side driver used to control the HV- contactor.
 *
 * @param none
 *
 * @return boolean PrmFlagErr - Error status (1: error, 0: no error). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 */
extern boolean BCTC_GetErrEltlHiCtctrNeg(void);
/******************************************************************************/
/**
 * @fn boolean BCTC_GetErrEltlHiCtctrPrec(void)
 *
 * @details This interface gets the electrical error status of the high side driver used to control the precharge contactor.
 *
 * @param none
 *
 * @return boolean PrmFlagErr - Error status (1: error, 0: no error). \n
 * hex. limits: 0H : 1H \n
 * phys. limits: 0 : 1 \n
 * unit: [-] \n
 */
extern boolean BCTC_GetErrEltlHiCtctrPrec(void);
/******************************************************************************/
/**
 * @fn uint8 BCTC_GetErrEltlLoCtctrPos(void)
 *
 * @details This interface gets the electrical error status of the low side driver used to control the HV+ contactor.
 *
 * @param none
 *
 * @return uint8 PrmErrSym - Error status (0: no error; 1: short circuit to ground; 2: open load; 3: over load / over temperature). \n
 * hex. limits: 0H : 3H \n
 * phys. limits: 0 : 3 \n
 * unit: [-] \n
 */
extern uint8 BCTC_GetErrEltlLoCtctrPos(void);
/******************************************************************************/
/**
 * @fn uint8 BCTC_GetErrEltlLoCtctrNeg(void)
 *
 * @details DThis interface gets the electrical error status of the low side driver used to control the HV- contactor.
 *
 * @param none
 *
 * @return uint8 PrmErrSym - Error status (0: no error; 1: short circuit to ground; 2: open load; 3: over load / over temperature). \n
 * hex. limits: 0H : 3H \n
 * phys. limits: 0 : 3 \n
 * unit: [-] \n
 */
extern uint8 BCTC_GetErrEltlLoCtctrNeg(void);
/******************************************************************************/
/**
 * @fn uint8 BCTC_GetErrEltlLoCtctrPrec(void)
 *
 * @details This interface gets the electrical error status of the low side driver used to control the precharge contactor.
 *
 * @param none
 *
 * @return uint8 PrmErrSym - Error status (0: no error; 1: short circuit to ground; 2: open load; 3: over load / over temperature). \n
 * hex. limits: 0H : 3H \n
 * phys. limits: 0 : 3 \n
 * unit: [-] \n
 */
extern uint8 BCTC_GetErrEltlLoCtctrPrec(void);

extern void BCTC_SetPwmLS0(boolean PrmFlagCls);
extern void BCTC_SetPwmLS1(boolean PrmFlagCls);
extern void BCTC_SetPwmLS2(boolean PrmFlagCls);
extern void BCTC_SetPwmLS3(boolean PrmFlagCls);

extern void BCTC_SetPwmLS4(boolean PrmFlagCls);
extern void BCTC_SetPwmLS5(boolean PrmFlagCls);
extern void BCTC_SetPwmLS6(boolean PrmFlagCls);
extern void BCTC_SetPwmLS7(boolean PrmFlagCls);

extern void BCTC_SetPwmLS8(boolean PrmFlagCls);
extern void BCTC_SetPwmLS9(boolean PrmFlagCls);
extern void BCTC_SetPwmLS10(boolean PrmFlagCls);
extern void BCTC_SetPwmLS11(boolean PrmFlagCls);


extern void BCTC_SetPwmHS0(boolean PrmFlagCls);
extern void BCTC_SetPwmHS1(boolean PrmFlagCls);
extern void BCTC_SetPwmHS2(boolean PrmFlagCls);
extern void BCTC_SetPwmHS3(boolean PrmFlagCls);
/*****************************************************************************/
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"


/* ========== End of Global variables (End of header function) ======== */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BCTC_CTCTRCTLDRVR_H_ */



