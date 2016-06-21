/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BAUXM_AuxMeasDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   23.04.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:06:48MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BAUXM_AuxMeasDrvr.h  $
 *  Revision 1.6 2010/11/22 07:06:48MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.5 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.4 2010/09/20 10:15:34MESZ Cotor Vlad RGB (COTORV) 
 *  LOG files clean up through SW correction
 *  Revision 1.3 2010/09/03 09:23:10MESZ Cotor Vlad RGB (COTORV) 
 *  Check and update all BSW modules due to scheduling functions
 *  Revision 1.2 2010/08/27 14:37:08MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.1 2010/08/25 16:41:06MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bauxm/src/project.pj
 *  Revision 1.2 2010/07/19 09:55:59MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for auxiliary measurements complex device driver due to specification update
 *  Revision 1.1 2010/05/20 14:04:13MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bauxm/src/project.pj
 *
 * ***************************************************************************/

#ifndef BAUXM_AUXMEASDRVR_H_
#define BAUXM_AUXMEASDRVR_H_

/* ========== Includes ===================================================== */
#include "Adc.h"

#include "BAUXM_AuxMeasDrvr_Cfg.h"


/* ========== Declare Alternate Control EXTERNAL_ definition =============== */
#ifdef  MASTER_BAUXM_AUXMEASDRVR_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/** @defgroup group_bauxm BAUXM
 *  @ingroup group_bauxm
 *
 *  Basic software for auxiliary measurements device driver */
/*@{*/

/** @page bauxm BAUXM
 *
 * @section bauxm_intro Introduction
 *  @li This driver shall handle analog and digital measurements using the corresponding microcontroller peripherals.
 */

/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */
#define BAUXM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   extern Adc_ValueGroupType BAUXM_AdcResult0[16];
   extern Adc_ValueGroupType BAUXM_AdcResult1[16];
#define BAUXM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */
#define BAUXM_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void BAUXM_RstInit(void)
 *
 * @details This function is called at RESET from operating system's StartUpHook and initialize all needed values.
 * If the initialization values are not specified, the variables will be initialized with 0.
 *
 * @param none
 *
 * @return void
 */
   extern void BAUXM_Ini(void);
/*****************************************************************************/
#define BAUXM_STOP_SEC_CODE_INIT
#include "MemMap.h"


#define BAUXM_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BAUXM_Scheduler_10ms(void)
 *
 * @details The 10ms scheduler function will be called from operating system in 10ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BAUXM_Scheduler_10ms(void);
/*****************************************************************************/
/**
 * @fn uint16 BAUXM_GetUAdc(uint8 PrmIdxChn)
 *
 * @brief Voltage acquirement from ADC
 *
 * @details This interface gets the analog to digital converter voltage of the channel PrmIdxChn
 *
 * @param uint8 PrmIdxChn - Index of ADC channel. \n
 * hex. limits: 00H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return uint16 PrmUAdc - ADC voltage. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 4.99966515 \n
 * unit: [V]
 */
extern uint16 BAUXM_GetUAdc(uint8 PrmIdxChn);
/*****************************************************************************/
/**
 * @fn void BAUXM_SetFacFilUAdc(uint8 PrmIdxChn, uint16 PrmFacFilUAdc)
 *
 * @details This interface sets the PT1-filter-constant of the channel PrmIdxChn for voltage measurement
 *
 * @param uint8 PrmIdxChn - Index of ADC channel. \n
 * hex. limits: 00H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @param uint16 PrmFacFil - PT1-filter-constant. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 0.99940875 \n
 * unit: [-] \n
 *
 * @return void
 */
extern void BAUXM_SetFacFilUAdc(uint8 PrmIdxChn, uint16 PrmFacFil);
/*****************************************************************************/
#define BAUXM_STOP_SEC_CODE_10MS
#include "MemMap.h"

/* ========== End of Global variables (End of header function) ============= */

/*@}*/

/* parasoft unsuppress item MISRA2004-8_5 */

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BAUXM_AUXMEASDRVR_H_ */
