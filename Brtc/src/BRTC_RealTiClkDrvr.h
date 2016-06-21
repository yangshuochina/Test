/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BRTC_RealTiClkDrvr.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   20.09.2010
 *  LAST REVISION:      $Date: 2012/08/14 15:48:33CEST $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BRTC_RealTiClkDrvr.h  $
 *  Revision 1.2 2012/08/14 15:48:33CEST Cotor Vlad RGB (COTORV) 
 *  - Initial version
 *
 * ***************************************************************************/

#ifndef BRTC_REALTICLKDRVR_H_
#define BRTC_REALTICLKDRVR_H_

/* ========== Includes ===================================================== */

#include "BRTC_RealTiClkDrvr_Cfg.h"

/* ========== Declare Alternate Control EXTERNAL_ definition =============== */

/** @defgroup group_brtc BRTC
 *  @ingroup group_brtc
 *
 *  Basic software for real time clock complex device driver */
/*@{*/

/** @page brtc BRTC
 *
 * @section brtc_intro Introduction
 *  @li This driver shall handle all basic software activities related to the real time clock module.
 *  @li The general task of this module is to communicate with the real time using lower level SPI basic software.
 *  This includes reading and setting the actual clock time as well as initializing and resetting the device.
 *  @li As it is not possible to write the registers bit wise it might be necessary to read one register byte and
 *  rewrite it afterwards with only the relevant bit(s) changed. Write commands are represented as follows: \n
 *  0  Bit inactive \n
 *  1  Bit active \n
 *  X  Bit irrelevant \n
 *  R  Read and rewrite bit \n
 *  W  Write Memory section of specific data \n
 *  \n\n
 *  @note Irrelevant bits (X) shall be set to 0 when writing a memory section.
 *  @note The sequence of read and write events shall follow the memory register r/w command layout
 *  (e.g. the topmost command shall be executed first while the lower most command shall be executed last). \n
 *  @li To be able to execute efficient read and re-write events this basic software shall use a shadowed
 *  (local) copy of the relevant real time clock memory which gets recurrently updated.
 *
 */
/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */



/* ========== Global function prototypes =================================== */

#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BRTC_Scheduler_10ms_1(void)
 *
 * @details The 10ms scheduler function will be called from operating system in 10ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void    BRTC_Scheduler_10ms_1(void);
/******************************************************************************/
/**
 * @fn void BRTC_Scheduler_10ms_2(void)
 *
 * @details The 10ms scheduler function will be called from operating system in 10ms recurrence.
 *
 * @param none
 *
 * @return void
 */
extern void BRTC_Scheduler_10ms_2(void);
/******************************************************************************/
/**
 * @fn void BRTC_SetOnline(void)
 *
 * @details This interface sets the real time clock device to online mode in order to have
 * the time registers getting updated by the internal real time clock memory.
 *
 * @param none
 *
 * @return void
 */
extern void BRTC_SetOnline(void);
/******************************************************************************/
/**
 * @fn void BRTC_SetClkRst(void)
 *
 * @details This interface sets all time registers to 0 in order to reset the real time clock value.
 *
 * @param none
 *
 * @return void
 */
extern void BRTC_SetClkRst(void);
/******************************************************************************/
/**
 * @fn uint16 BRTC_GetClk(void)
 *
 * @details Getting the elapsed time since last clock reset. This interface gets the stored clock time from
 * the clock time registers copy.
 *
 * @param none
 *
 * @return
 * uint16 BRTC_GetClk - Clock time in minutes. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits: 0 : 65535 \n
 * unit: [min] \n
 */
extern uint16 BRTC_GetClk (void);

extern void   BRTC_GetRtcTime(uint8 time[7]);

extern void   BRTC_SetClk(uint8 timeReg[7]);

/******************************************************************************/
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

/* ========== End of Global variables (End of header function) ============= */

#endif    /* BRTC_REALTICLKDRVR_H_ */
