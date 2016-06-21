/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BRTC_RealTiClkDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   20.09.2010
 *  LAST REVISION:      $Date: 2012/08/14 15:49:00CEST $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BRTC_RealTiClkDrvr.c  $
 *  Revision 1.2 2012/08/14 15:49:00CEST Cotor Vlad RGB (COTORV) 
 *  - Initial version
 *
 * ***************************************************************************/

#define  MLIB_INLINE  0

/* ========== Includes ===================================================== */
#include "Mathlib.h"
#include "Std_Types.h"
#include "BRTC_RealTiClkDrvr.h"           /* module header file     */
#include "Spi.h"
#include "Prj_MainCfg.h"

/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */
uint8 Dec2HexDateConversion(uint8 var_Data);
uint8 Hex2DecDateConversion(uint8 var_Data);
uint16 Date2MinutesConversion(uint8 var_years, uint8 var_months, uint8 var_days,  uint8 var_hours, uint8 var_minutes);
void PCA21125Rx(void);
void StorePCA21125ClkReg(void);


/* ========== Local Variables ============================================== */
#define BRTC_START_SEC_VAR_8BIT
#include "MemMap.h"
   uint8   PCA21125_ClkReg[PCA21125_NOADDR_READ];
#define BRTC_STOP_SEC_VAR_8BIT
#include "MemMap.h"

/* ========== Local Macros ================================================= */



/* ========== PCA21125 Clock Register Map ==================================
 *
 * |======|===============|========|========|========|========|========|========|========|========|
 * |Addr. |Register name  |bit 7   |bit 6   |bit 5   |bit 4   |bit 3   |bit 2   |bit 1   |bit 0   |
 * |======|===============|========|========|========|========|========|========|========|========|==========|
 * |00h   |Control_1      |EXT_TEST|N       |STOP    |N       |POR_OVRD|12_24   |N       |N       |  Control |
 * |======|===============|========|========|========|========|========|========|========|========|   and    |
 * |01h   |Control_2      |MI      |SI      |MSF     |TI_TP   |AF      |TF      |AIE     |TIE     |  status  |
 * |======|===============|========|========|========|========|========|========|========|========|==========|
 * |02h   |Seconds        |RF      |SECONDS (0 to 59)                                             |          |
 * |======|===============|========|==============================================================|          |
 * |03h   |Minutes        |-       |MINUTES (0 to 59)                                             |          |
 * |======|===============|========|========|========|============================================|          |
 * |04h   |Hours          |-       |-       |AMPM    |HOURS ( 1 to 12) in 12 hour mode            |          |
 * |                      |========|========|========|============================================|          |
 * |                      |-       |-       |HOURS (0 to 23) in 24 hour mode                      |   Time   |
 * |======|===============|========|========|=====================================================|    and   |
 * |05h   |Days           |-       |-       |DAYS ( 1 to 31)                                      |   date   |
 * |======|===============|========|========|========|========|========|==========================|          |
 * |06h   |Weekdays       |-       |-       |-       |-       |-       |WEEKDAYS ( 0 to 6)        |          |
 * |======|===============|========|========|========|========|========|==========================|          |
 * |07h   |Months         |-       |-       |-       |MONTHS (1 to 12)                            |          |
 * |======|===============|========|========|========|============================================|          |
 * |08h   |Years          |YEARS (0 to 99)                                                        |          |
 * |======|===============|========|========|=====================================================|==========|
 * |09h   |Minute_alarm    AE_M    |MINUTE_ALARM (0 to 59)                                        |          |
 * |======|===============|========|========|========|============================================|          |
 * |0Ah   |Hour_alarm     |AE_H    |-       |AMPM    |HOUR_ALARM (1 to 12) in 12 hour mode        |          |
 * |      |               |        |========|========|============================================|  Alarm   |
 * |      |               |        |-       |HOUR_ALARM (0 to 23) in 24 hour mode                 |          |
 * |======|===============|========|========|=====================================================|          |
 * |0Bh   |Day_alarm       AE_D    |-       |DAY_ALARM (1 to 31)                                  |          |
 * |======|===============|========|========|=================|========|==========================|          |
 * |0Ch   |Weekday_alarm   AE_W    |-       |-       |-       |-       |WEEKDAY_ALARM (0 to 6)    |          |
 * |======|===============|========|========|========|========|========|==========================|==========|
 * |0Dh   |CLKOUT_control |-       |-       |-       |-       |-       |COF                       |  CLKOUT  |
 * |======|===============|========|========|========|========|========|========|========|========|==========|
 * |0Eh   |Timer_control  |TE      |-       |-       |-       |-       |-       |-       |-       |          |
 * |======|===============|========|========|========|========|========|========|========|========|  Timer   |
 * |0Fh   |Countdown_timer|COUNTDOWN_TIMER                                                        |          |
 * |======|===============|=======================================================================|==========|
 *
 * === Keys ===
 * |=================|====================================================|
 * | -               | bit not implemented and will return 0 when read    |
 * | N               | bit should always be written with logic 0 (unused) |
 * | EXT_TEST        | external clock test mode                           |
 * | STOP            | RTC stop                                           |
 * | POR_OVRD        | Power on reset override                            |
 * | 12_24           | 12 or 24 hour mode                                 |
 * | MI              | Minute interrupt                                   |
 * | SI              | Second interrupt                                   |
 * | MSF             | Minute or second interrupt flag                    |
 * | TI_TP           | Timer interrupt timer pulse                        |
 * | AF              | Alarm flag                                         |
 * | TF              | Countdown timer flag                               |
 * | AIE             | Alarm interrupt enabled                            |
 * | TIE             | Countdown timer enabled                            |
 * | RF              | Chip reset flag                                    |
 * | AMPM            | AM / PM hour mode                                  |
 * | AE_M            | Alarm enabled minute                               |
 * | AE_H            | Alarm enabled hour                                 |
 * | AE_D            | Alarm enabled day                                  |
 * | AE_W            | Alarm enabled week                                 |
 * | COF             | Clock output frequency                             |
 * | TE              | Timer enabled                                      |
 * | CTD             | Countdown timer frequency                          |
 * | COUNTDOWN_TIMER | Countdown period in seconds                        |
 * |=================|====================================================|
 *
 *
 *
 * 0  Bit inactive
 * 1  Bit active
 * x  Bit irrelevant (don't care) will be set to 0
 * R  Read and rewrite
 * W  Write Memory section of specific data
 *
 */

/* ========== Local Functions ============================================== */
#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn uint8 Dec2HexDateConversion(uint8 var_Data)
 *
 * @details This function converts the decimal date into hex value as follow
 * 0010 0101 = 0x25 => through convert: 25[dec] = 0x19 (00011001)
 * This value 25 doesn't express a hex value but a decimal one. This must be converted to hex.
 * In our case this 25[dec] must be converted to hex (19h).
 *
 * @param var_Data - register value which must be converted. \n
 * hex. limits: 00H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return This function returns the converted hex value of the decimal value contained in the register. \n
 * hex. limits: 00H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 */
uint8 Dec2HexDateConversion(uint8 var_Data)
{
   return (((uint8)(var_Data >> 4u) * 10u) + (uint8)(var_Data & 0xFu));
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/******************************************************************************/


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn uint8 Hex2DecDateConversion(uint8 var_Data)
 *
 * @brief Hex date conversion to decimal
 *
 * @details This function converts the hex date into decimal value as follow
 * 0x25 = 37[dec] = 00100101 => through convert: 37 (0011 0111).
 * This value 37 doesn't express a decimal value but a hex one. This must be converted to decimal.
 * In our case this 37[dec] must be converted to 0x37
 *
 * @param var_Data - register value which must be converted. \n
 * hex. limits: 00H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return This function returns the converted hex value of the decimal value contained in the register. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 255 \n
 * unit: [-] \n
 */
uint8 Hex2DecDateConversion(uint8 var_Data)
{
   return MLIB_Uint8Uint16(((var_Data / 10u) << 4u) | ((var_Data % 10u) & 0xFu));
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/******************************************************************************/


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn uint16 DateMinutesConversion(uint8 var_years, uint8 var_months, uint8 var_days,  uint8 var_hours, uint8 var_minutes)
 *
 * @brief Calculation of time (years, months, days, hours and minutes) in minutes
 *
 * @details This function calculates the BCU sleep time, stored in the register, in minutes.
 * This time is limited to 65535 minutes, that means approximatively 45,5 days.
 *
 * @param var_years, var_months, var_days,  var_hours, var_minutes
 *
 * are hex converted values of the values stored in the registers.
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 255 \n
 * unit        : [-] \n
 *
 * @return
 * This function returns the calculated time in minutes.\n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits:     0 : 65535 \n
 * unit        : [-] \n
 */
uint16 Date2MinutesConversion(uint8 var_years, uint8 var_months, uint8 var_days,  uint8 var_hours, uint8 var_minutes)
{
   uint16 TmpDate2MinutesConversion = 0u;

   if ((var_years > 0u) || (var_months > 2u))
   { TmpDate2MinutesConversion = UINT16_MAX;   /* the time must be limited to 65535 min. = approx. 45,5 days */ }
   else
   {
      /* TmpDate2MinutesConversion = var_minutes               +
                                    ((var_hours       *    60) +
                                    ((var_days - 1u)  *  1440) +
                                    ((var_months - 1) * 44640)
       */
      TmpDate2MinutesConversion =  MLIB_Uint16AddUint16Uint16((uint16)var_minutes      , MLIB_Uint16MulUint16Uint16(var_hours        ,   60u));
      TmpDate2MinutesConversion =  MLIB_Uint16AddUint16Uint16(TmpDate2MinutesConversion, MLIB_Uint16MulUint16Uint16((var_days - 1u)  ,  1440u));
      TmpDate2MinutesConversion =  MLIB_Uint16AddUint16Uint16(TmpDate2MinutesConversion, MLIB_Uint16MulUint16Uint16((var_months - 1u), 44640u));
   }

   return TmpDate2MinutesConversion;
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/******************************************************************************/


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void PCA21125Rx(void)
 *
 * @details This function makes the SPI communication in reading mode for PCA21125 chip. The rx_buffer will be actualized
 * with values contained in the PCA21125 chip registers.
 *
 * @param none
 *
 * @return void
 */
/* parasoft suppress item MISRA2004-16_10 reason "The returned value is always E_OK" */
void PCA21125Rx(void)
{
   /* local variables */
   uint8 AsyncTransmitStatus;

   if (Spi_GetSequenceResult(Sequence_0_UT0) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      tx_buffer[RxTx_Chip_PCA21125_channel_j0_cmd] = 0x90u | (uint16)Chip_PCA21125_RegAddr_02;
      Spi_SetupEB(Chip_PCA21125_channel_j0_cmd,
                  &tx_buffer[RxTx_Chip_PCA21125_channel_j0_cmd],
                  &rx_buffer[RxTx_Chip_PCA21125_channel_j0_cmd],
                  1u);

      Spi_SetupEB(Chip_PCA21125_channel_j0_data,
                  NULL_PTR,
                  &rx_buffer[RxTx_Chip_PCA21125_channel_j0_data],
                  7u);

      AsyncTransmitStatus = Spi_AsyncTransmit(Sequence_0_UT0);
      if (AsyncTransmitStatus != E_OK)
      {}
   }
}
/* parasoft unsuppress item MISRA2004-16_10 */
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/******************************************************************************/

#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void StorePCA21125ClkReg(void)
 *
 * @details This function stores the rx_buffer (register values) values into global PCA21125_ClkReg array to be analyzed and used
 * at a particular desired time. It is only relevant to store the registers 0x02 till 0x08.
 *
 * @param none
 *
 * @return void
 */
void StorePCA21125ClkReg(void)
{
   PCA21125_ClkReg[0] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data]      & 0xFFu);    /* register 0x02 - seconds */
   PCA21125_ClkReg[1] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data + 1u] & 0xFFu);    /* register 0x03 - minutes */
   PCA21125_ClkReg[2] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data + 2u] & 0xFFu);    /* register 0x04 - hours */
   PCA21125_ClkReg[3] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data + 3u] & 0xFFu);    /* register 0x05 - days */
   PCA21125_ClkReg[4] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data + 4u] & 0xFFu);    /* register 0x06 - weekdays */
   PCA21125_ClkReg[5] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data + 5u] & 0xFFu);    /* register 0x07 - months */
   PCA21125_ClkReg[6] = (uint8)(rx_buffer[RxTx_Chip_PCA21125_channel_j0_data + 6u] & 0xFFu);    /* register 0x08 - years */
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/******************************************************************************/

/* ========== Global Functions ============================================= */
#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BRTC_Scheduler_10ms_1(void)
{
   StorePCA21125ClkReg();
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BRTC_Scheduler_10ms_1 ==================================== */


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BRTC_Scheduler_10ms_2(void)
{
   PCA21125Rx();
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BRTC_Scheduler_10ms_2 ==================================== */


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/* parasoft suppress item MISRA2004-16_10 reason "The returned value is always E_OK" */
void BRTC_SetOnline(void)
{
   /* local variables */
   uint8 AsyncTransmitStatus;

   if (Spi_GetSequenceResult(Sequence_1_UT0) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      tx_buffer[RxTx_Chip_PCA21125_channel_j1_cmd] = 0x10u | Chip_PCA21125_RegAddr_00;
      Spi_SetupEB(Chip_PCA21125_channel_j1_cmd,
                  &tx_buffer[RxTx_Chip_PCA21125_channel_j1_cmd],
                  &rx_buffer[RxTx_Chip_PCA21125_channel_j1_cmd],
                  1u);

      tx_buffer[RxTx_Chip_PCA21125_channel_j1_data]      = 0x00u; /* 00h   0 X 0 X 0 0 X X */
      tx_buffer[RxTx_Chip_PCA21125_channel_j1_data + 1u] = 0x00u; /* 01h   0 0 0 0 0 0 0 0 */
      Spi_SetupEB(Chip_PCA21125_channel_j1_data,
                  &tx_buffer[RxTx_Chip_PCA21125_channel_j1_data],
                  &rx_buffer[RxTx_Chip_PCA21125_channel_j1_data],
                  2u);

      AsyncTransmitStatus = Spi_AsyncTransmit(Sequence_1_UT0);
      if (AsyncTransmitStatus != E_OK)
      {}
   }
}
/* parasoft unsuppress item MISRA2004-16_10 */
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BRTC_SetOnline =========================================== */


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/* parasoft suppress item MISRA2004-16_10 reason "The returned value is always E_OK" */
void BRTC_SetClkRst(void)
{
   /* local variables */
   uint8 AsyncTransmitStatus;

   if (Spi_GetSequenceResult(Sequence_2_UT0) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_cmd] = 0x10u | (uint16)Chip_PCA21125_RegAddr_02;
      Spi_SetupEB(Chip_PCA21125_channel_j2_cmd, &tx_buffer[RxTx_Chip_PCA21125_channel_j2_cmd], &rx_buffer[RxTx_Chip_PCA21125_channel_j2_cmd], 1u);

      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data]      = (uint16)0x00;   /* 02h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 1u] = (uint16)0x00;   /* 03h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 2u] = (uint16)0x00;   /* 04h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 3u] = (uint16)0x01;   /* 05h  0 0 0 0 0 0 0 1 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 4u] = (uint16)0x00;   /* 06h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 5u] = (uint16)0x01;   /* 07h  0 0 0 0 0 0 0 1 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 6u] = (uint16)0x00;   /* 08h  0 0 0 0 0 0 0 0 */

      Spi_SetupEB(Chip_PCA21125_channel_j2_data,
                  &tx_buffer[RxTx_Chip_PCA21125_channel_j2_data],
                  &rx_buffer[RxTx_Chip_PCA21125_channel_j2_data],
                  7u);

      AsyncTransmitStatus = Spi_AsyncTransmit(Sequence_2_UT0);
      if (AsyncTransmitStatus != E_OK)
      { }
   }
}
/* parasoft unsuppress item MISRA2004-16_10 */
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BRTC_SetClkRst =========================================== */


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BRTC_GetClk (void)
{
   /* local variables */
   uint8 tmp_years = 0u;
   uint8 tmp_months = 0u;
   uint8 tmp_days = 0u;
   uint8 tmp_hours = 0u;
   uint8 tmp_minutes = 0u;
   
   tmp_years   = Dec2HexDateConversion(PCA21125_ClkReg[6]);    /* register 0x08 - years   */
   tmp_months  = Dec2HexDateConversion(PCA21125_ClkReg[5]);    /* register 0x07 - months  */
   tmp_days    = Dec2HexDateConversion(PCA21125_ClkReg[3]);    /* register 0x05 - days    */
   tmp_hours   = Dec2HexDateConversion(PCA21125_ClkReg[2]);    /* register 0x04 - hours   */
   tmp_minutes = Dec2HexDateConversion(PCA21125_ClkReg[1]);    /* register 0x03 - minutes */

   return Date2MinutesConversion(tmp_years, tmp_months, tmp_days, tmp_hours, tmp_minutes);
}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BRTC_GetClk ============================================== */


#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
/* parasoft suppress item MISRA2004-16_10 reason "The returned value is always E_OK" */
void BRTC_SetClk(uint8 timeReg[7])
{
   timeReg[0] = Hex2DecDateConversion(timeReg[0]);
   timeReg[1] = Hex2DecDateConversion(timeReg[1]);
   timeReg[2] = Hex2DecDateConversion(timeReg[2]);
   timeReg[3] = Hex2DecDateConversion(timeReg[3]);
   timeReg[4] = Hex2DecDateConversion(timeReg[4]);
   timeReg[5] = Hex2DecDateConversion(timeReg[5]);
   timeReg[6] = Hex2DecDateConversion(timeReg[6]);

   /* local variables */
   uint8 AsyncTransmitStatus;

   if (Spi_GetSequenceResult(Sequence_2_UT0) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_cmd] = 0x10u | (uint16)Chip_PCA21125_RegAddr_02;
      Spi_SetupEB(Chip_PCA21125_channel_j2_cmd, &tx_buffer[RxTx_Chip_PCA21125_channel_j2_cmd], &rx_buffer[RxTx_Chip_PCA21125_channel_j2_cmd], 1u);

      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data]      = (uint16)timeReg[0];   /* 02h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 1u] = (uint16)timeReg[1];   /* 03h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 2u] = (uint16)timeReg[2];   /* 04h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 3u] = (uint16)timeReg[3];   /* 05h  0 0 0 0 0 0 0 1 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 4u] = (uint16)timeReg[4];   /* 06h  0 0 0 0 0 0 0 0 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 5u] = (uint16)timeReg[5];   /* 07h  0 0 0 0 0 0 0 1 */
      tx_buffer[RxTx_Chip_PCA21125_channel_j2_data + 6u] = (uint16)timeReg[6];   /* 08h  0 0 0 0 0 0 0 0 */

      Spi_SetupEB(Chip_PCA21125_channel_j2_data,
                  &tx_buffer[RxTx_Chip_PCA21125_channel_j2_data],
                  &rx_buffer[RxTx_Chip_PCA21125_channel_j2_data],
                  7u);

      AsyncTransmitStatus = Spi_AsyncTransmit(Sequence_2_UT0);
      if (AsyncTransmitStatus != E_OK)
      { }
   }
}
/* parasoft unsuppress item MISRA2004-16_10 */
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"




#define BRTC_START_SEC_CODE_10MS
#include "MemMap.h"
void   BRTC_GetRtcTime(uint8 time[7])
{
	uint8 cunt;

/*	*timeVar =     Dec2HexDateConversion(PCA21125_ClkReg[0]);
	*(timeVar+1) = Dec2HexDateConversion(PCA21125_ClkReg[1]);
	*(timeVar+2) = Dec2HexDateConversion(PCA21125_ClkReg[2]);
	*(timeVar+3) = Dec2HexDateConversion(PCA21125_ClkReg[3]);
	*(timeVar+4) = Dec2HexDateConversion(PCA21125_ClkReg[4]);
	*(timeVar+5) = Dec2HexDateConversion(PCA21125_ClkReg[5]);
	*(timeVar+6) = Dec2HexDateConversion(PCA21125_ClkReg[6]);*/

	for(cunt=0;cunt<7;cunt++)
	{
	  time[cunt]=Dec2HexDateConversion(PCA21125_ClkReg[cunt]);
	}

}
#define BRTC_STOP_SEC_CODE_10MS
#include "MemMap.h"


/* ========== END BRTC_GetClk ============================================== */


/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif
