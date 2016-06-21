/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BELHPN_EltlHzrdProtnDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   28.10.2010
 *  LAST REVISION:      $Date: 2011/02/11 15:43:37MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BELHPN_EltlHzrdProtnDrvr.c  $
 *  Revision 1.4 2011/02/11 15:43:37MEZ Cotor Vlad RGB (COTORV) 
 *  - Correction of output value calculation of interface BELHPN_GetUMeasIsln
 *  Revision 1.3 2010/12/15 15:02:31MEZ Cotor Vlad RGB (COTORV) 
 *  Update of electrical hazard protection complex device driver due to missing cyclical call of the 1ms scheduler.
 *  Revision 1.2 2010/12/01 13:30:08MEZ Cotor Vlad RGB (COTORV) 
 *  Implement and integrate complex device driver for electrical hazard protection
 *
 * ***************************************************************************/

#define  MASTER_BELHPN_ELTLHZRDPROTNDRVR_C_              /* Master define for this module    */
#define  MLIB_INLINE  0

/* ========== Includes ===================================================== */
#include "Mathlib.h"

#include "BELHPN_EltlHzrdProtnDrvr.h"           /* module header file     */

/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */
void BELHPN_AS8510Tx(void);
void BELHPN_AS8510Rx(void);


/* ========== Local Variables ============================================== */
#define BELHPN_START_SEC_VAR_8BIT
#include "MemMap.h"
   uint8 AS8510_0_Gain;
   uint8 AS8510_0_Gain_tx;
   uint8 AS8510_0_Gain_rx;
   uint8 Chip_AS8510_0_address_j3_data;
   uint8 Chip_AS8510_0_address_j4_data;
   uint8 BELHPN_time_stamp;
#define BELHPN_STOP_SEC_VAR_8BIT
#include "MemMap.h"


#define BELHPN_START_SEC_VAR_16BIT
#include "MemMap.h"
   uint16 BELHPN_AnChnSelnReg_data = 0x40;
   uint16 BELHPN_CurSrcSetReg_data = 0x00;
   uint16 BELHPN_AS8510_UAdc;
#define BELHPN_STOP_SEC_VAR_16BIT
#include "MemMap.h"


/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */
/**
 * @fn void BELHPN_AS8510Tx(void)
 *
 * @details Information sending through SPI unit to the AS8510 (UI1) \n
 * The driver shall continuously measure the input voltage of the AS8510,
 * used for isolation detection, in a recurrence of BELHPN_CfgMeasPerd
 * (recurrence of the software on the microcontroller)
 *
 * @param none
 *
 * @return void
 */
#define BELHPN_START_SEC_CODE_1MS
#include "MemMap.h"
void BELHPN_AS8510Tx(void)
{
   if (Spi_GetSequenceResult(Sequence_0_UI1_FirstJob) == SPI_SEQ_PENDING)
   { /* do nothing */
     /* DIAG: insert a counter to see if a SPI is lost - to be made by testing */
   }
   else
   {
      /* UI1 - job 0 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j0_cmd] = Chip_AS8510_0_address_j0_cmd; /* for j0: cmd */
      Spi_SetupEB(Chip_AS8510_0_channel_j0_cmd,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j0_cmd], &rx_buffer[RxTx_Chip_AS8510_0_channel_j0_cmd], 1);
      Spi_SetupEB(Chip_AS8510_0_channel_j0_data, NULL_PTR,                                      &rx_buffer[RxTx_Chip_AS8510_0_channel_j0_data], 5); /* for j0: data */

      /* UI1 - job 1 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j1_cmd] = Chip_AS8510_0_address_j1_cmd; /* for j1: cmd */
      Spi_SetupEB(Chip_AS8510_0_channel_j1_cmd,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j1_cmd], &rx_buffer[RxTx_Chip_AS8510_0_channel_j1_cmd], 1);
      Spi_SetupEB(Chip_AS8510_0_channel_j1_data, NULL_PTR,                                      &rx_buffer[RxTx_Chip_AS8510_0_channel_j1_data], 5); /* for j1: data */

      /* UI1 - job 2 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j2_cmd] = Chip_AS8510_0_address_j2_cmd; /* for j2: cmd */
      Spi_SetupEB(Chip_AS8510_0_channel_j2_cmd,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j2_cmd], &rx_buffer[RxTx_Chip_AS8510_0_channel_j2_cmd], 1);
      Spi_SetupEB(Chip_AS8510_0_channel_j2_data, NULL_PTR,                                      &rx_buffer[RxTx_Chip_AS8510_0_channel_j2_data], 5); /* for j2: data */

      /* The gain of the AS8510 is set via the PGA control register (13h) and the power down control register (15h) */
      switch (AS8510_0_Gain)
      {
         case 1:     /* Gain 1 */
            Chip_AS8510_0_address_j3_data = 0xFFu;    /* power down control register (15h): 1111 1111 (FFh) */
            Chip_AS8510_0_address_j4_data = 0x00u;    /* PGA control register (13h)       : 0000 0000 (00h) */
            break;
         case 5:     /* Gain 5 */
            Chip_AS8510_0_address_j3_data = 0xF3u;    /* power down control register (15h): 1111 0011 (F3h) */
            Chip_AS8510_0_address_j4_data = 0x00u;    /* PGA control register (13h)       : 0000 0000 (00h) */
            break;
         case 25:     /* Gain 25 */
            Chip_AS8510_0_address_j3_data = 0xF3u;    /* power down control register (15h): 1111 0011 (F3h) */
            Chip_AS8510_0_address_j4_data = 0x50u;    /* PGA control register (13h)       : 0101 0000 (50h) */
            break;
         case 40:     /* Gain 40 */
            Chip_AS8510_0_address_j3_data = 0xF3u;    /* power down control register (15h): 1111 0011 (F3h) */
            Chip_AS8510_0_address_j4_data = 0xA0u;    /* PGA control register (13h)       : 1010 0000 (A0h) */
            break;
         case 100:     /* Gain 100 */
            Chip_AS8510_0_address_j3_data = 0xF3u;    /* power down control register (15h): 1111 0011 (F3h) */
            Chip_AS8510_0_address_j4_data = 0xF0u;    /* PGA control register (13h)       : 1111 0000 (F0h) */
            break;
         default:     /* Gain 1 */
            Chip_AS8510_0_address_j3_data = 0xFFu;    /* power down control register (15h): 1111 1111 (FFh) */
            Chip_AS8510_0_address_j4_data = 0x00u;    /* PGA control register (13h)       : 0000 0000 (00h) */
            break;
      }

      AS8510_0_Gain_tx = AS8510_0_Gain;

      /* Power down control register (Set Gprg): Reg 15 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j3_cmd] = Chip_AS8510_0_address_j3_cmd; /* for j3: cmd */
      Spi_SetupEB(Chip_AS8510_0_channel_j3_cmd,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j3_cmd], &rx_buffer[RxTx_Chip_AS8510_0_channel_j3_cmd], 1);
      tx_buffer[RxTx_Chip_AS8510_0_channel_j3_data] = Chip_AS8510_0_address_j3_data; /* for j3: data */
      Spi_SetupEB(Chip_AS8510_0_channel_j3_data,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j3_data], &rx_buffer[RxTx_Chip_AS8510_0_channel_j3_data], 1);

      /* PGA control register (Set Gprg): Reg 13 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j4_cmd] = Chip_AS8510_0_address_j4_cmd; /* for j4: cmd */
      Spi_SetupEB(Chip_AS8510_0_channel_j4_cmd,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j4_cmd], &rx_buffer[RxTx_Chip_AS8510_0_channel_j4_cmd], 1);
      tx_buffer[RxTx_Chip_AS8510_0_channel_j4_data] = Chip_AS8510_0_address_j4_data; /* for j4: data */
      Spi_SetupEB(Chip_AS8510_0_channel_j4_data,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j4_data], &rx_buffer[RxTx_Chip_AS8510_0_channel_j4_data], 1);

      /* Activation/Deactivation of the resistor switch: Reg 17 and Reg 18 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j5_cmd] = Chip_AS8510_0_address_j5_cmd; /* for j5: cmd */
      Spi_SetupEB(Chip_AS8510_0_channel_j5_cmd,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j5_cmd], &rx_buffer[RxTx_Chip_AS8510_0_channel_j5_cmd], 1);
      tx_buffer[RxTx_Chip_AS8510_0_channel_j5_data]     = BELHPN_AnChnSelnReg_data; /* for reg 17h: data */
      tx_buffer[RxTx_Chip_AS8510_0_channel_j5_data + 1] = BELHPN_CurSrcSetReg_data; /* for reg 18h: data */
      Spi_SetupEB(Chip_AS8510_0_channel_j5_data,  &tx_buffer[RxTx_Chip_AS8510_0_channel_j5_data], &rx_buffer[RxTx_Chip_AS8510_0_channel_j5_data], 2);

      Spi_AsyncTransmit(Sequence_0_UI1);
   }
}
#define BELHPN_STOP_SEC_CODE_1MS
#include "MemMap.h"


#define BELHPN_START_SEC_CODE_1MS
#include "MemMap.h"
void BELHPN_AS8510Rx(void)
{
   uint16 TmpSpiValue_data1;
   /* check data plausibility  */
   if ((rx_buffer[RxTx_Chip_AS8510_0_channel_j2_data]     == rx_buffer[RxTx_Chip_AS8510_0_channel_j1_data]) &&
       (rx_buffer[RxTx_Chip_AS8510_0_channel_j2_data + 1] == rx_buffer[RxTx_Chip_AS8510_0_channel_j1_data + 1]))
   {
      TmpSpiValue_data1 = rx_buffer[RxTx_Chip_AS8510_0_channel_j2_data];
   }
   else
   {
      if ((rx_buffer[RxTx_Chip_AS8510_0_channel_j1_data]     == rx_buffer[RxTx_Chip_AS8510_0_channel_j0_data]) &&
          (rx_buffer[RxTx_Chip_AS8510_0_channel_j1_data + 1] == rx_buffer[RxTx_Chip_AS8510_0_channel_j0_data + 1]))
      {
         TmpSpiValue_data1 = rx_buffer[RxTx_Chip_AS8510_0_channel_j1_data];
      }
      else
      {
         TmpSpiValue_data1 = rx_buffer[RxTx_Chip_AS8510_0_channel_j2_data];
      }
   }
   BELHPN_AS8510_UAdc = TmpSpiValue_data1;

   AS8510_0_Gain_rx = AS8510_0_Gain_tx;

}
#define BELHPN_STOP_SEC_CODE_1MS
#include "MemMap.h"


/* ========== Global Functions ============================================= */
#define BELHPN_START_SEC_CODE_INIT
#include "MemMap.h"
void BELHPN_Ini(void)
{
   /* Set the return value of BELHPN_GetUMeasIsln to 0 */
   BELHPN_AS8510_UAdc = 0x8000;

   if (Spi_GetSequenceResult(Sequence_1_UI1) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      /* Set the measurement recurrence of the AS8510 to BELHPN_CfgMeasPerdHW. The measurement recurrence BELHPN_CfgMeasPerdHW shall represent the register value, sent to the AS8510 */
      tx_buffer[RxTx_Chip_AS8510_0_channel_0_init] = BELHPN_CfgMeasPerdHW;
      Spi_SetupEB(Chip_AS8510_0_channel_0_init,   &tx_buffer[RxTx_Chip_AS8510_0_channel_0_init],   &rx_buffer[RxTx_Chip_AS8510_0_channel_0_init],   1);

      /* Set the AS8510 to normal mode 1 and set the start bit of the mode control register: send write command on address 0Ah with data 0000_0001. (0x0A01) */
      tx_buffer[RxTx_Chip_AS8510_0_channel_1_init] = 0x0A01;
      Spi_SetupEB(Chip_AS8510_0_channel_1_init,   &tx_buffer[RxTx_Chip_AS8510_0_channel_1_init],   &rx_buffer[RxTx_Chip_AS8510_0_channel_1_init],   1);

      Spi_AsyncTransmit(Sequence_1_UI1);
   }

   /* Set the voltage measurement gain of the AS8510 to BELHPN_CfgGainIni. */
   AS8510_0_Gain = AS8510_0_Gain_rx = AS8510_0_Gain_tx = BELHPN_CfgGainIni;

   /* Deactivate both measurement switches for HV+ and HV- */
   /* Deactivate the switch for the parallel isolation detection resistor. (Same behavior as calling BELHPN_SetChnIsln with PrmChn = 0 and PrmROn = 0.) */
   BELHPN_SetChnIsln(0, 0);

}
#define BELHPN_STOP_SEC_CODE_INIT
#include "MemMap.h"
/* ========== END BELHPN_RstInit =========================================== */


#define BELHPN_START_SEC_CODE_1MS
#include "MemMap.h"
void BELHPN_Scheduler_1ms(void)
{
   if (BELHPN_time_stamp == BELHPN_CfgMeasPerd - 1)
   {
      BELHPN_AS8510Rx();
      BELHPN_AS8510Tx();
      BELHPN_time_stamp = 0;
   }
   else
   {
      BELHPN_time_stamp++;
   }
}
#define BELHPN_STOP_SEC_CODE_1MS
#include "MemMap.h"
/* ========== END BCTC_Scheduler_1ms ======================================= */


#define BELHPN_START_SEC_CODE_10MS
#include "MemMap.h"
void BELHPN_SetChnIsln (uint8 PrmChn, boolean PrmROn)
{
   /**
   * @li In case PrmChn = 0, both switches (for HV+ and HV-) shall be deactivated.
   * @li In case PrmChn = 1, the switch for HV- shall be deactivated and the switch for HV+ shall be activated.
   * @li In case PrmChn = 2, the switch for HV+ shall be deactivated and the switch for HV- shall be activated.
   * @li In any other case, both switches for HV+ and HV- shall be deactivated.
   */
   switch (PrmChn)
   {
      case 0:
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Pos_HV, STD_LOW);   /* HV+ low  - deactivated */
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Neg_HV, STD_LOW);   /* HV- low  - deactivated */
         break;

      case 1:
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Neg_HV, STD_LOW);   /* HV- low  - deactivated */
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Pos_HV, STD_HIGH);  /* HV+ high - activated */
         break;

      case 2:
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Pos_HV, STD_LOW);   /* HV+ low  - deactivated */
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Neg_HV, STD_HIGH);  /* HV- high - activated */
         break;

      default:
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Pos_HV, STD_LOW);   /* HV+ low  - deactivated */
         Dio_WriteChannel(DIO_PIN_BCTC_SetChnIsln_Neg_HV, STD_LOW);   /* HV- low  - deactivated */
         break;
   }

/**
 * Activation of the resistor switch is done by setting a certain current to the programmable current source of the AS8510
 * and selecting the ETR channel. This can be done by setting the analog channel selection register (17h) and the current
 * source setting register (18h) of the AS8510 according to: \n \n
 *
 * @li Activate resistor switch \n
 *  @li Analog channel selection register (17h): 0101 0000 (50h) \n
 *  @li current source setting register   (18h): 1111 1000 (F8h) \n
 *  \n
 * @li Deactivate resistor switch \n
 *  @li Analog channel selection register (17h): 0100 0000 (40h) \n
 *  @li current source setting register   (18h): 0000 0000 (00h) \n
 * \n
 *
 * @li In case PrmROn = 0, the switch for the parallel isolation detection resistor shall be deactivated.
 * @li In case PrmROn = 1, the switch for the parallel isolation detection resistor shall be activated.
 * @li In the default case, the switch for the parallel isolation detection resistor shall be deactivated.
 */
   switch (PrmROn)
   {
      case 0:
         BELHPN_AnChnSelnReg_data = 0x40;
         BELHPN_CurSrcSetReg_data = 0x00;
         break;
      case 1:
         BELHPN_AnChnSelnReg_data = 0x50;
         BELHPN_CurSrcSetReg_data = 0xF8;
         break;
      default:
         BELHPN_AnChnSelnReg_data = 0x40;
         BELHPN_CurSrcSetReg_data = 0x00;
         break;
   }
}
#define BELHPN_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BELHPN_SetChnIsln ======================================== */


#define BELHPN_START_SEC_CODE_10MS
#include "MemMap.h"
sint16 BELHPN_GetUMeasIsln(void)
{
   sint16 PrmU;
   if (BELHPN_AS8510_UAdc >= 0x8000)
   {
      PrmU = MLIB_Sint16Uint32((uint32)(BELHPN_AS8510_UAdc - 0x8000) * BELHPN_CfgUReslBas / AS8510_0_Gain_rx / 2000);
   }
   else
   {
	   PrmU = -(MLIB_Sint16Uint32((uint32)(0x8000 - BELHPN_AS8510_UAdc) * BELHPN_CfgUReslBas / AS8510_0_Gain_rx / 2000));
   }
   return PrmU;
}
#define BELHPN_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BELHPN_GetUMeasIsln ====================================== */


#define BELHPN_START_SEC_CODE_10MS
#include "MemMap.h"
void BELHPN_SetGainADC(uint8 PrmGain)
{
   /* plausibility check of ADC gain - only gain 1, 5, 25, 40 or 100 are allowed */
   if (PrmGain == 1u || PrmGain == 5u || PrmGain == 25u || PrmGain == 40u || PrmGain == 100u)
   { AS8510_0_Gain = PrmGain; }
   else
   { AS8510_0_Gain = 1u; }  /* default gain: 1 */
}
#define BELHPN_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BELHPN_SetGainADC ======================================== */

/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif

/* undefine master define for this module    */
#ifdef MASTER_BELHPN_ELTLHZRDPROTNDRVR_C_
#undef MASTER_BELHPN_ELTLHZRDPROTNDRVR_C_
#endif

