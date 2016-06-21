/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: BCTC_CtctrCtlDrvr.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.03.2010
 *  LAST REVISION:      $Date: 2010/12/15 15:27:22MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BCTC_CtctrCtlDrvr.c  $
 *  Revision 1.19 2010/12/15 15:27:22MEZ Cotor Vlad RGB (COTORV) 
 *  Check possible overflow of counter in BCTC_Scheduler_1ms
 *  Revision 1.18 2010/12/13 12:59:48MEZ Cotor Vlad RGB (COTORV)
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.17 2010/11/29 11:50:36MEZ Cotor Vlad RGB (COTORV)
 *  Implement and integrate complex device driver for electrical hazard protection
 *  Revision 1.16 2010/11/22 07:13:34MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.15 2010/11/16 13:11:31MEZ Cotor Vlad RGB (COTORV)
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.14 2010/10/14 17:53:05MESZ Cotor Vlad RGB (COTORV)
 *  Update complex device driver for real time clock due to changed SPI configuration.
 *  Revision 1.13 2010/10/07 14:10:13MESZ Cotor Vlad RGB (COTORV)
 *  Update SPI sequence's names due to integration of complex device driver for real time clock
 *  Revision 1.12 2010/09/30 10:07:40MESZ Cotor Vlad RGB (COTORV)
 *  Correction of BCTC_AS8510Rx_1 due to erroneous calculation of BCTC_GetUMeasDev1
 *  Revision 1.11 2010/09/24 18:06:49MESZ Cotor Vlad RGB (COTORV)
 *  Update scheduling behavior and add deactivation of programmable gain to extend measurement range
 *  Reorganize the SPI Configuration switching to macros
 *  Revision 1.10 2010/09/23 11:23:29MESZ Cotor Vlad RGB (COTORV)
 *  Correct the calculation of UActx in BCTC
 *  Revision 1.9 2010/09/20 10:24:47MESZ Cotor Vlad RGB (COTORV)
 *  LOG files clean up through SW correction
 *  Revision 1.8 2010/09/13 12:52:01MESZ Cotor Vlad RGB (COTORV)
 *  Correction of SPI driver due to SPI transmission function.
 *  Revision 1.7 2010/08/27 14:19:08MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.6 2010/08/25 16:18:43MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW complex device drivers names
 *  Revision 1.5 2010/08/25 14:39:09MESZ Cotor Vlad RGB (COTORV)
 *  Implement and integrate complex device driver for cooling fan control
 *  Revision 1.4 2010/08/20 13:33:56MESZ Opel Martin RGB (OPELM)
 *  - moved 16bit values to 16bit locate section
 *  Revision 1.3 2010/07/23 16:11:52MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification change
 *  Revision 1.2 2010/07/19 09:55:03MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification update
 *  Revision 1.1 2010/05/20 14:01:45MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bctc/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_BCTC_CTCTRCTLDRVR_C_    /*!< Master define for this module    */
#define  MLIB_INLINE  0

/* ========== Includes ===================================================== */
#include "Mathlib.h"

#include "BCTC_CtctrCtlDrvr.h"           /*!< module header file     */

/* ========== Enumerations, Typedefinitions, Local Defines  ================ */
typedef struct Ctctr_
{
   uint16   CtctrPos[BCTC_CfgNoElmMeasBufI];
   uint16   CtctrNeg[BCTC_CfgNoElmMeasBufI];
   uint16   CtctrPrec[BCTC_CfgNoElmMeasBufI];
} Struct_GetICtctr;

typedef struct RawCtctr_
{
   uint16   RawCtctrPos[BCTC_CfgNoElmMeasBufI];
   uint16   RawCtctrNeg[BCTC_CfgNoElmMeasBufI];
   uint16   RawCtctrPrec[BCTC_CfgNoElmMeasBufI];
} Struct_GetICtctrRaw;

typedef struct Meas_
{
   uint16   MeasDev1[BCTC_CfgNoElmMeasBuf1];
} Struct_GetUMeas;

typedef struct RawMeas_
{
   uint16   RawMeasDev1[BCTC_CfgNoElmMeasBuf1];
} Struct_GetUMeasRaw;

typedef struct buf_
{
   uint8 size;          /* ring buffer size */
   uint8 putIndex;      /* index position of the newest inserted element */
   boolean bufferEmpty; /* boolean value for empty ring buffer status */
   boolean getFirstRun; /* boolean value for first run status */
   uint16 *dataPtr;     /* pointer to the ring buffer contained data */
} BufferType;

typedef struct mux_
{
   uint16   MuxMeasSample[BCTC_CfgNoChnMux2];
   uint16   MuxResl[BCTC_CfgNoChnMux2];
   uint8    GainHW[BCTC_CfgNoChnMux2];
} Struct_Mux;

/* ========== Local function prototypes  =================================== */
void BCTC_AS8510Tx_1(void);
void BCTC_AS8510Rx_1(void);
void BCTC_AS8510Tx_2(void);
void BCTC_AS8510Rx_2(void);
void BCTC_TLE8104Rx(void);
void BCTC_TLE8104Tx(void);

/* ========== Local variables  ============================================= */
#define BCTC_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
   Struct_GetICtctr     GetICtctr;
   Struct_GetICtctrRaw  GetICtctrRaw;
   Struct_GetUMeas      GetUMeas;
   Struct_GetUMeasRaw   GetUMeasRaw;
   BufferType           buf1;
   BufferType           buf2;
   BufferType           buf3;
   BufferType           buf4;
   Struct_Mux           AS8510_2_Mux;
   Struct_TLE8104       TLE8104;
   Adc_ValueGroupType   BCTC_AdcResult1[16];
#define BCTC_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
/* ========================================================================= */
#define BCTC_START_SEC_VAR_16BIT
#include "MemMap.h"
   uint16   tmp_index;
   uint16   buf1Data[BCTC_CfgNoElmMeasBufI];
   uint16   buf2Data[BCTC_CfgNoElmMeasBufI];
   uint16   buf3Data[BCTC_CfgNoElmMeasBufI];
   uint16   buf4Data[BCTC_CfgNoElmMeasBuf1];
#define BCTC_STOP_SEC_VAR_16BIT
#include "MemMap.h"
/* ========================================================================= */
#define BCTC_START_SEC_VAR_8BIT
#include "MemMap.h"
   uint8    tmp_buf_size;
   uint8    BCTC_time_stamp;
   uint8    mux_ch_idx;
   uint8    mux_ch_idx_prev;
   uint8    mux_ch_idx_next;

   uint8    BCTC_CfgChnAdrMux2[BCTC_CfgNoChnMux2] = BCTC_CfgChnAdrMux2_default;
   uint8    BCTC_CfgGainUIni2[BCTC_CfgNoChnMux2] = BCTC_CfgGainUIni2_default;

   uint8    AS8510_1_Gain;
   uint8    AS8510_2_Gain[BCTC_CfgNoChnMux2];
#define BCTC_STOP_SEC_VAR_8BIT
#include "MemMap.h"
/* ========================================================================= */
#define BCTC_START_SEC_VAR_BOOLEAN
#include "MemMap.h"
   boolean  PrmFlagClsPos;
   boolean  PrmFlagClsNeg;
   boolean  PrmFlagClsPrec;
#define BCTC_STOP_SEC_VAR_BOOLEAN
#include "MemMap.h"

#define SPI_START_SEC_VAR_16BIT
#include "MemMap.h"

   Spi_DataType	tle8104_TxBuf[6];
   Spi_DataType	tle8104_RxBuf[6];

#define SPI_STOP_SEC_VAR_16BIT
#include "MemMap.h"

/* ========================================================================= */


/* ========== Local macros ================================================= */


/* ========== Local functions ============================================== */
#define BCTC_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void bufInit(BufferType *bufPtr, uint8 size, uint16 *data)
 *
 * @details This function initialize the ring buffer used by getting the voltage values
 *
 * @param BufferType *bufPtr - pointer to the ring buffer \n
 * @param uint8 size - ring buffer size \n
 * @param uint16 *data - pointer to the address of ring buffer content \n
 *
 * @return void
 */
void bufInit(BufferType *bufPtr, uint8 size, uint16 *data)
{
   bufPtr->bufferEmpty = TRUE;
   bufPtr->getFirstRun = TRUE;
   bufPtr->dataPtr = data;
   bufPtr->putIndex = 0;
   bufPtr->size = size;
}
#define BCTC_STOP_SEC_CODE_INIT
#include "MemMap.h"
/*****************************************************************************/


#define BCTC_START_SEC_CODE_INIT
#include "MemMap.h"
/**
 * @fn void bufferInit(void)
 *
 * @details This function initialize all the ring buffers by calling bufInit function with the respectively parameters.
 *
 * @param none
 *
 * @return void
 */
void bufferInit(void)
{
   /* initialize all buffers */
   bufInit(&buf1, BCTC_CfgNoElmMeasBufI, buf1Data);
   bufInit(&buf2, BCTC_CfgNoElmMeasBufI, buf2Data);
   bufInit(&buf3, BCTC_CfgNoElmMeasBufI, buf3Data);
   bufInit(&buf4, BCTC_CfgNoElmMeasBuf1, buf4Data);
}
#define BCTC_STOP_SEC_CODE_INIT
#include "MemMap.h"
/*****************************************************************************/


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void putValueToBuffer(uint16 MeasSample, BufferType *bufPtr)
 *
 * @details This function stores values in a ring buffer. The dimension of buffer is an input variable
 *
 * @param uint16 MeasSample -  measured value to be put in the ring buffer \n
 * @param BufferType *bufPtr - pointer to the ring buffer \n
 *
 * @return void
 */
/*****************************************************************************/
void putValueToBuffer(uint16 MeasSample, BufferType *bufPtr)
{
   if (bufPtr->bufferEmpty)
   {
      bufPtr->bufferEmpty = FALSE;
   }
   else
   {
      /* normal write */
      if((bufPtr->putIndex+1) >= bufPtr->size)
      {
         bufPtr->putIndex = 0;
      }
      else
      {
         bufPtr->putIndex++;
      }
   }
   bufPtr->dataPtr[bufPtr->putIndex] = MeasSample;
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/******************************************************************************/


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn uint16 getValueFromBuffer(BufferType *bufPtr, uint8 tmp_index)
 *
 * @details This function get the values one by one from ring buffer.
 * The start position is the current (the last entry in buffer) index.
 *
 * @param BufferType *bufPtr - pointer to the ring buffer
 * @param uint8 tmp_index - ring buffer's index where the desired value is stored.
 *
 * @return uint16 MeasSample - measured value stored in the ring buffer
 *
 */
uint16 getValueFromBuffer(BufferType *bufPtr, uint8 tmp_index)
{
   uint16 MeasSample;
   if (tmp_index < bufPtr->size)
   {
      if (tmp_index <= bufPtr->putIndex)
      {
         MeasSample = bufPtr->dataPtr[bufPtr->putIndex - tmp_index];
      }
      else
      {
         MeasSample = bufPtr->dataPtr[bufPtr->putIndex + bufPtr->size - tmp_index];
      }
   }
   else
   {
      MeasSample = 0;
   }
   return MeasSample;
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/*****************************************************************************/


/*****************************************************************************/
/* AS8510 - WRITE and READ Commands === */
/*
 * WRITE Command
 * |0/1|<-register address->|<--------data--------->|
 * |---|--------------------|-----------------------|
 * | 0 |A6|A5|A4|A3|A2|A1|A0|D7|D6|D5|D4|D3|D2|D1|D0|    SDI
 * | W |--------------------|-----------------------|
 *
 * example:
 * | W |<------ 13H ------->|<-------- 03H -------->|     = 1303H
 *   0   0  0  1  0  0  1  1  0  0  0  0  0  0  1  1
 *
 *
 * READ Command
 * |0/1|<-register address->|
 * |---|--------------------|-----------------------|
 * | 1 |A6|A5|A4|A3|A2|A1|A0|                       |    SDI
 * |-R-|--------------------|-----------------------|
 *                          |<--------data--------->|
 * |------------------------|-----------------------|
 * |                        |D7|D6|D5|D4|D3|D2|D1|D0|    SDO
 * |------------------------------------------------|
 *
 * example:
 * | R |<------ 13H ------->|
 *   1   0  0  1  0  0  1  1                              = 9300
 *                          |<-------- 03H -------->|     = 0003H
 *                            0  0  0  0  0  0  1  1
 */
/*****************************************************************************/
#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BCTC_AS8510Tx_1(void)
 *
 * @details Information sending through SPI unit to the AS8510 (UH2)\n
 * The AS8510_1 device shall measure the input voltage U2 in a configurable recurrence
 * (BCTC_CfgMeasPerd1) by getting the measured value via SPI from the AS8510_1. \n\n
 *
 * This function prepares and sends (transmits) the information through SPI unit to the chip AS8510 (UH2). \n
 * Chip_AS8510_1_channel_xxx: \n
 * channel index. Channel  configuration holds all properties linked
 * to a channel and these are valid for all transfer elements. \n\n
 *
 * Sequence_0_UH2: \n
 * sequence index. Sequence configuration holds all properties linked
 * to a sequence and these are valid for all jobs contained in the sequence.\n\n
 *
 * Chip_AS8510_1_address_xxx: \n
 * read or write address. This is build as follow: \n
 * | 0 |A6|A5|A4|A3|A2|A1|A0| \n
 * The MSB decide between read(1) or write(0) sequence. [A6:A0] is the Control register's address \n
 *
 * @param none
 *
 * @return void
 */
void BCTC_AS8510Tx_1(void)
{
   if (Spi_GetSequenceResult(Sequence_0_UH2) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {  /* UH2 - job 0 */
      tx_buffer[RxTx_Chip_AS8510_1_channel_j0_cmd] =   Chip_AS8510_1_address_j0_cmd; /* for j0: cmd */
      Spi_SetupEB(Chip_AS8510_1_channel_j0_cmd,   &tx_buffer[RxTx_Chip_AS8510_1_channel_j0_cmd],   &rx_buffer[RxTx_Chip_AS8510_1_channel_j0_cmd],   1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j0_data1] = 0x0000;                       /* for j0: data1 */
      Spi_SetupEB(Chip_AS8510_1_channel_j0_data1, &tx_buffer[RxTx_Chip_AS8510_1_channel_j0_data1], &rx_buffer[RxTx_Chip_AS8510_1_channel_j0_data1], 1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j0_data2] = 0x0000;                       /* for j0: data2 */
      Spi_SetupEB(Chip_AS8510_1_channel_j0_data2, &tx_buffer[RxTx_Chip_AS8510_1_channel_j0_data2], &rx_buffer[RxTx_Chip_AS8510_1_channel_j0_data2], 1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j0_data3] = 0x0000;                       /* for j0: data3 */
      Spi_SetupEB(Chip_AS8510_1_channel_j0_data3, &tx_buffer[RxTx_Chip_AS8510_1_channel_j0_data3], &rx_buffer[RxTx_Chip_AS8510_1_channel_j0_data3], 1);


      /* UH2 - job 1 */
      tx_buffer[RxTx_Chip_AS8510_1_channel_j1_cmd] =   Chip_AS8510_1_address_j1_cmd; /* for j1: cmd */
      Spi_SetupEB(Chip_AS8510_1_channel_j1_cmd,   &tx_buffer[RxTx_Chip_AS8510_1_channel_j1_cmd],   &rx_buffer[RxTx_Chip_AS8510_1_channel_j1_cmd],   1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j1_data1] = 0x0000;                       /* for j1: data1 */
      Spi_SetupEB(Chip_AS8510_1_channel_j1_data1, &tx_buffer[RxTx_Chip_AS8510_1_channel_j1_data1], &rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data1], 1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j1_data2] = 0x0000;                       /* for j1: data2 */
      Spi_SetupEB(Chip_AS8510_1_channel_j1_data2, &tx_buffer[RxTx_Chip_AS8510_1_channel_j1_data2], &rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data2], 1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j1_data3] = 0x0000;                       /* for j1: data3 */
      Spi_SetupEB(Chip_AS8510_1_channel_j1_data3, &tx_buffer[RxTx_Chip_AS8510_1_channel_j1_data3], &rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data3], 1);


      /* UH2 - job 2 */
      tx_buffer[RxTx_Chip_AS8510_1_channel_j2_cmd] =   Chip_AS8510_1_address_j2_cmd; /* for j2: cmd */
      Spi_SetupEB(Chip_AS8510_1_channel_j2_cmd,   &tx_buffer[RxTx_Chip_AS8510_1_channel_j2_cmd],   &rx_buffer[RxTx_Chip_AS8510_1_channel_j2_cmd],   1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j2_data1] = 0x0000;                       /* for j2: data1 */
      Spi_SetupEB(Chip_AS8510_1_channel_j2_data1, &tx_buffer[RxTx_Chip_AS8510_1_channel_j2_data1], &rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data1], 1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j2_data2] = 0x0000;                       /* for j2: data2 */
      Spi_SetupEB(Chip_AS8510_1_channel_j2_data2, &tx_buffer[RxTx_Chip_AS8510_1_channel_j2_data2], &rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data2], 1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_j2_data3] = 0x0000;                       /* for j2: data3 */
      Spi_SetupEB(Chip_AS8510_1_channel_j2_data3, &tx_buffer[RxTx_Chip_AS8510_1_channel_j2_data3], &rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data3], 1);


      /* UH2 - job 3 */
      tx_buffer[RxTx_Chip_AS8510_1_channel_j3_cmd] = Chip_AS8510_1_address_j3_cmd | (returnCfgGain(AS8510_1_Gain) << 6) | (returnCfgGain(AS8510_1_Gain) << 4);      /* for j3: cmd */
      Spi_SetupEB(Chip_AS8510_1_channel_j3_cmd,   &tx_buffer[RxTx_Chip_AS8510_1_channel_j3_cmd],   &rx_buffer[RxTx_Chip_AS8510_1_channel_j3_cmd],   1);

      /* UH2 - job 4 */
      if (AS8510_1_Gain == 1u)
      {
         tx_buffer[RxTx_Chip_AS8510_1_channel_j4_cmd] = Chip_AS8510_GainOff;
      }
      else
      {
         tx_buffer[RxTx_Chip_AS8510_1_channel_j4_cmd] = Chip_AS8510_GainOn;
      }
      Spi_SetupEB(Chip_AS8510_1_channel_j4_cmd,   &tx_buffer[RxTx_Chip_AS8510_1_channel_j4_cmd],   &rx_buffer[RxTx_Chip_AS8510_1_channel_j4_cmd],   1);

      Spi_AsyncTransmit(Sequence_0_UH2);
   }
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/*****************************************************************************/


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BCTC_AS8510Rx_1(void)
 *
 * @details This interface reads the data stored in the SPI rx_buffer for AS8510Rx_1 and puts them in the ring buffer.
 * This procedure will take place after a plausibility check.
 *
 * @param none
 *
 * @return void
 */
void BCTC_AS8510Rx_1(void)
{
   uint16 TmpSpiValue_data2;
   /* check data plausibility  */
   if ((rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data1] == rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data1]) &&
       (rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data2] == rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data2]))
   {
      TmpSpiValue_data2 = rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data2];
   }
   else
   {
      if ((rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data1] == rx_buffer[RxTx_Chip_AS8510_1_channel_j0_data1]) &&
          (rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data2] == rx_buffer[RxTx_Chip_AS8510_1_channel_j0_data2]))
      {
         TmpSpiValue_data2 = rx_buffer[RxTx_Chip_AS8510_1_channel_j1_data2];
      }
      else
      {
         TmpSpiValue_data2 = rx_buffer[RxTx_Chip_AS8510_1_channel_j2_data2];
      }
   }
   if (TmpSpiValue_data2 <= 0x8000)
   {
      TmpSpiValue_data2 = 0;
   }
   else
   {
      TmpSpiValue_data2 = MLIB_Uint16Uint32(MLIB_Uint32MulUint16Uint16((TmpSpiValue_data2 - 0x8000), (uint16)BCTC_CfgUReslBas) / AS8510_1_Gain / (uint16)0x2710);
   }
   putValueToBuffer(TmpSpiValue_data2, &buf4);
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/*****************************************************************************/


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BCTC_AS8510Tx_2(void)
 *
 * @details Information sending through SPI unit to the AS8510 (UH3) \n
 * The AS8510_2 device shall measure up to 8 input voltages which are multiplexed to the one voltage
 * measurement channel of the AS8510_2. \n\n
 *
 * This function prepares and sends (transmits) the information through SPI unit to the chip AS8510 (UH3). \n
 * Chip_AS8510_2_channel_xxx: \n
 * channel index. Channel  configuration holds all properties linked
 * to a channel and these are valid for all transfer elements. \n\n
 *
 * Sequence_0_UH3: \n
 * sequence index. Sequence configuration holds all properties linked
 * to a sequence and these are valid for all jobs contained in the sequence.\n\n
 *
 * Chip_AS8510_2_address_xxx: \n
 * read or write address. This is build as follow: \n
 * | 0 |A6|A5|A4|A3|A2|A1|A0| \n
 * The MSB decide between read(1) or write(0) sequence. [A6:A0] is the Control register's address \n
 *
 * @param none
 *
 * @return void
 */
void BCTC_AS8510Tx_2(void)
{
   if (Spi_GetSequenceResult(Sequence_0_UH3) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      /* UH3 - job 0 */
      tx_buffer[RxTx_Chip_AS8510_2_channel_j0_cmd] =   Chip_AS8510_2_address_j0_cmd; /* for j0: cmd */
      Spi_SetupEB(Chip_AS8510_2_channel_j0_cmd,   &tx_buffer[RxTx_Chip_AS8510_2_channel_j0_cmd],   &rx_buffer[RxTx_Chip_AS8510_2_channel_j0_cmd],   1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j0_data1] = 0x0000;                       /* for j0: data1 */
      Spi_SetupEB(Chip_AS8510_2_channel_j0_data1, &tx_buffer[RxTx_Chip_AS8510_2_channel_j0_data1], &rx_buffer[RxTx_Chip_AS8510_2_channel_j0_data1], 1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j0_data2] = 0x0000;                       /* for j0: data2 */
      Spi_SetupEB(Chip_AS8510_2_channel_j0_data2, &tx_buffer[RxTx_Chip_AS8510_2_channel_j0_data2], &rx_buffer[RxTx_Chip_AS8510_2_channel_j0_data2], 1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j0_data3] = 0x0000;                       /* for j0: data3 */
      Spi_SetupEB(Chip_AS8510_2_channel_j0_data3, &tx_buffer[RxTx_Chip_AS8510_2_channel_j0_data3], &rx_buffer[RxTx_Chip_AS8510_2_channel_j0_data3], 1);


      /* UH3 - job 1 */
      tx_buffer[RxTx_Chip_AS8510_2_channel_j1_cmd] =   Chip_AS8510_2_address_j1_cmd; /* for j1: cmd */
      Spi_SetupEB(Chip_AS8510_2_channel_j1_cmd,   &tx_buffer[RxTx_Chip_AS8510_2_channel_j1_cmd],   &rx_buffer[RxTx_Chip_AS8510_2_channel_j1_cmd],   1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j1_data1] = 0x0000;                       /* for j1: data1 */
      Spi_SetupEB(Chip_AS8510_2_channel_j1_data1, &tx_buffer[RxTx_Chip_AS8510_2_channel_j1_data1], &rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data1], 1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j1_data2] = 0x0000;                       /* for j1: data2 */
      Spi_SetupEB(Chip_AS8510_2_channel_j1_data2, &tx_buffer[RxTx_Chip_AS8510_2_channel_j1_data2], &rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data2], 1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j1_data3] = 0x0000;                       /* for j1: data3 */
      Spi_SetupEB(Chip_AS8510_2_channel_j1_data3, &tx_buffer[RxTx_Chip_AS8510_2_channel_j1_data3], &rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data3], 1);


      /* UH3 - job 2 */
      tx_buffer[RxTx_Chip_AS8510_2_channel_j2_cmd] =   Chip_AS8510_2_address_j2_cmd; /* for j2: cmd */
      Spi_SetupEB(Chip_AS8510_2_channel_j2_cmd,   &tx_buffer[RxTx_Chip_AS8510_2_channel_j2_cmd],   &rx_buffer[RxTx_Chip_AS8510_2_channel_j2_cmd],   1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j2_data1] = 0x0000;                       /* for j2: data1 */
      Spi_SetupEB(Chip_AS8510_2_channel_j2_data1, &tx_buffer[RxTx_Chip_AS8510_2_channel_j2_data1], &rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data1], 1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j2_data2] = 0x0000;                       /* for j2: data2 */
      Spi_SetupEB(Chip_AS8510_2_channel_j2_data2, &tx_buffer[RxTx_Chip_AS8510_2_channel_j2_data2], &rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data2], 1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_j2_data3] = 0x0000;                       /* for j2: data3 */
      Spi_SetupEB(Chip_AS8510_2_channel_j2_data3, &tx_buffer[RxTx_Chip_AS8510_2_channel_j2_data3], &rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data3], 1);


      /* UH3 - job 3 and 4*/
      tx_buffer[RxTx_Chip_AS8510_2_channel_j3_cmd] = Chip_AS8510_2_address_j3_cmd | (returnCfgGain(AS8510_2_Mux.GainHW[mux_ch_idx_next]) << 6) | (returnCfgGain(AS8510_2_Mux.GainHW[mux_ch_idx_next]) << 4);      /* for j3: cmd */
      Spi_SetupEB(Chip_AS8510_2_channel_j3_cmd,   &tx_buffer[RxTx_Chip_AS8510_2_channel_j3_cmd],   &rx_buffer[RxTx_Chip_AS8510_2_channel_j3_cmd],   1);

      if (AS8510_2_Gain[mux_ch_idx_next] == 1u)
      {
         tx_buffer[RxTx_Chip_AS8510_2_channel_j4_cmd] = Chip_AS8510_GainOff;
      }
      else
      {
         tx_buffer[RxTx_Chip_AS8510_2_channel_j4_cmd] = Chip_AS8510_GainOn;
      }
      Spi_SetupEB(Chip_AS8510_2_channel_j4_cmd,   &tx_buffer[RxTx_Chip_AS8510_2_channel_j4_cmd],   &rx_buffer[RxTx_Chip_AS8510_2_channel_j4_cmd],   1);

      AS8510_2_Mux.GainHW[mux_ch_idx_next] = AS8510_2_Gain[mux_ch_idx_next];

      Spi_AsyncTransmit(Sequence_0_UH3);
   }
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/*****************************************************************************/


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
/**
 * @fn void BCTC_AS8510Rx_2(void)
 *
 * @details This interface reads the data stored in the SPI rx_buffer for AS8510Rx_2 and store them in an array for each multiplexer input.
 * This procedure will take place after a plausibility check.
 *
 * @param none
 *
 * @return void
 */
void BCTC_AS8510Rx_2(void)
{
   uint16 TmpSpiValue_data2;
   /* check data plausibility  */
   if ((rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data1] == rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data1]) &&
       (rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data2] == rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data2]))
   {
      TmpSpiValue_data2 = rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data2];
   }
   else
   {
      if ((rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data1] == rx_buffer[RxTx_Chip_AS8510_2_channel_j0_data1]) &&
          (rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data2] == rx_buffer[RxTx_Chip_AS8510_2_channel_j0_data2]))
      {
         TmpSpiValue_data2 = rx_buffer[RxTx_Chip_AS8510_2_channel_j1_data2];
      }
      else
      {
         TmpSpiValue_data2 = rx_buffer[RxTx_Chip_AS8510_2_channel_j2_data2];
      }
   }

   if(TmpSpiValue_data2 <= 0x8000)
   {
      AS8510_2_Mux.MuxMeasSample[mux_ch_idx_prev] = 0;
   }
   else
   {
      AS8510_2_Mux.MuxMeasSample[mux_ch_idx_prev] = TmpSpiValue_data2 - 0x8000;
   }

   AS8510_2_Mux.MuxResl[mux_ch_idx_prev] = BCTC_CfgUReslBas / AS8510_2_Mux.GainHW[mux_ch_idx_prev];
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/*****************************************************************************/


#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/**
 * @fn void BCTC_TLE8104Tx(void)
 *
 * @details This function make the SPI communication with TLE8104 chip.
 *
 * @param uint8 TLE8104_channel - channel index. Channel  configuration holds all properties linked to a channel
 * and these are valid for all transfer elements. \ n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @param uint8 RxTx_TLE8104_channel - rx_buffer index. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @param uint8 TLE8104_sequence - sequence index. Sequence configuration holds all properties linked to a sequence
 * and these are valid for all jobs contained in the sequence.\n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @param uint8 TLE8104_data - data section to be read or write. \n
 * hex. limits: 0H : FFH \n
 * phys. limits: 0 : 255 \n
 * unit: [-] \n
 *
 * @return void
 */
void BCTC_TLE8104Tx(void)
{
   if (Spi_GetSequenceResult(Sequence_UL2_UL3_UL4) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
/* TLE8104E Daisy Chain
 *         TLE8104E        TLE8104E        TLE8104E
 *         ---------       ---------       ---------
 *         |       |       |       |       |       |
 *  ------>|  UL2  |------>|  UL3  |------>|  UL4  |
 *         |       |       |       |       |       |
 *         ---------       ---------       ---------
 *     <  UL4  > <  UL3  > <  UL2  >
 * Tx: 0000 0000 0000 0000      0000 0000 0000 0000     0000 0000 0000 0000
 * ---|___________________|---|___________________|---|___________________|---
 * Rx:                       <DiagUL4> <DiagUL3> <DiagUL2>
 *
 */

      Spi_SetupEB(Chip_TLE8104_channel_j0_cmd,  NULL_PTR, NULL_PTR, 3);
      Spi_SetupEB(Chip_TLE8104_channel_j1_diag, NULL_PTR, &rx_buffer[RxTx_Chip_TLE8104_channel_j1_diag], 3);

      Spi_AsyncTransmit(Sequence_UL2_UL3_UL4);

   }
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/*****************************************************************************/

void BCTC_TLE8104Rx(void)
{
   TLE8104.rx_buffer_UL4 = rx_buffer[RxTx_Chip_TLE8104_channel_j1_diag];
   TLE8104.rx_buffer_UL3 = rx_buffer[RxTx_Chip_TLE8104_channel_j1_diag+1];
   TLE8104.rx_buffer_UL2 = rx_buffer[RxTx_Chip_TLE8104_channel_j1_diag+2];
}

/* ========== Global functions ============================================= */
#define BCTC_START_SEC_CODE_INIT
#include "MemMap.h"
void BCTC_Ini(void)
{
/* === General initialization ======================= */
   bufferInit(); /* ring buffer initialization */
   AS8510_1_Gain = BCTC_CfgGainUIni1;

   if (Spi_GetSequenceResult(Sequence_1_UH2) == SPI_SEQ_PENDING)
   { /* do nothing */
   }
   else
   {
      /* === initialization of AS8510 Device 1 ============ */
      /* Register address 45 and 0A*/
      tx_buffer[RxTx_Chip_AS8510_1_channel_0_init] = 0x4500 | BCTC_CfgMeasPerdHW1;
      Spi_SetupEB(Chip_AS8510_1_channel_0_init,   &tx_buffer[RxTx_Chip_AS8510_1_channel_0_init],   &rx_buffer[RxTx_Chip_AS8510_1_channel_0_init],   1);

      tx_buffer[RxTx_Chip_AS8510_1_channel_1_init] = 0x0A01;
      Spi_SetupEB(Chip_AS8510_1_channel_1_init,   &tx_buffer[RxTx_Chip_AS8510_1_channel_1_init],   &rx_buffer[RxTx_Chip_AS8510_1_channel_1_init],   1);

      Spi_AsyncTransmit(Sequence_1_UH2);
   }

/* === initialization of MUX structure ============== */
   int i;
   for (i=0; i<BCTC_CfgNoChnMux2;i++)
   {
      AS8510_2_Mux.MuxMeasSample[i] = 0;
      AS8510_2_Mux.MuxResl[i] = 0;
      AS8510_2_Mux.GainHW[i] = BCTC_CfgGainUIni2[i];
      AS8510_2_Gain[i] = BCTC_CfgGainUIni2[i];
   }
   /* Set multiplexer logic control input for initialization */
   mux_ch_idx = 0;
   Dio_WriteChannel(DIO_PIN_BCTC_ADG658_Mux_A0,  BCTC_CfgChnAdrMux2[mux_ch_idx] & 0x1);
   Dio_WriteChannel(DIO_PIN_BCTC_ADG658_Mux_A1, (BCTC_CfgChnAdrMux2[mux_ch_idx] & 0x2) >> 1);
   Dio_WriteChannel(DIO_PIN_BCTC_ADG658_Mux_A2, (BCTC_CfgChnAdrMux2[mux_ch_idx] & 0x4) >> 2);

   if (Spi_GetSequenceResult(Sequence_1_UH3) == SPI_SEQ_PENDING)
   { /* do nothing */
   }
   else
   {
   /* === initialization of AS8510 Device 2 ============ */
   /* Register address 45 and 0A*/
      tx_buffer[RxTx_Chip_AS8510_2_channel_0_init] = 0x4500 | BCTC_CfgMeasPerdHW2;
      Spi_SetupEB(Chip_AS8510_2_channel_0_init,   &tx_buffer[RxTx_Chip_AS8510_2_channel_0_init],   &rx_buffer[RxTx_Chip_AS8510_2_channel_0_init],   1);

      tx_buffer[RxTx_Chip_AS8510_2_channel_1_init] = 0x0A01;
      Spi_SetupEB(Chip_AS8510_2_channel_1_init,   &tx_buffer[RxTx_Chip_AS8510_2_channel_1_init],   &rx_buffer[RxTx_Chip_AS8510_2_channel_1_init],   1);

      Spi_AsyncTransmit(Sequence_1_UH3);
   }

   if (Spi_GetSequenceResult(Sequence_UL2_UL3_UL4) == SPI_SEQ_PENDING)
   { /* do nothing */ }
   else
   {
      tx_buffer[RxTx_Chip_TLE8104_channel_j0_cmd]     = Chip_TLE8104_1_init;
      tx_buffer[RxTx_Chip_TLE8104_channel_j0_cmd + 1] = Chip_TLE8104_2_init;
      Spi_SetupEB(Chip_TLE8104_channel_j0_cmd,  &tx_buffer[RxTx_Chip_TLE8104_channel_j0_cmd],   &rx_buffer[RxTx_Chip_TLE8104_channel_j0_cmd], 2);

      Spi_SetupEB(Chip_TLE8104_channel_j1_diag, NULL_PTR, &rx_buffer[RxTx_Chip_TLE8104_channel_j1_diag], 2);

      Spi_AsyncTransmit(Sequence_UL2_UL3_UL4);

   }
   /* === END initialization of UL3: TLE8104 =========== */
}
#define BCTC_STOP_SEC_CODE_INIT
#include "MemMap.h"
/* ========== END BCTC_Ini ================================================= */


#define BCTC_START_SEC_CODE_1MS
#include "MemMap.h"
void BCTC_Scheduler_1ms(void)
{
   /* scheduler for Device 1 (UH2) */
   if (BCTC_time_stamp >= BCTC_CfgMeasPerd1 - 1)
   {

      BCTC_AS8510Rx_1();
      BCTC_AS8510Tx_1();
      BCTC_time_stamp = 0;
   }
   else if (BCTC_time_stamp == BCTC_CfgMeasPerd1 / 2 - 1)
   {
      /* scheduler for Device 2 (UH3) */


      BCTC_AS8510Rx_2();
      BCTC_AS8510Tx_2();
      BCTC_time_stamp++;
   }
   else if (BCTC_time_stamp == BCTC_CfgMeasPerd1 / 2)
   {
     /* increment mux channel */
      mux_ch_idx_prev = mux_ch_idx;
      mux_ch_idx++;
      if (mux_ch_idx >= BCTC_CfgNoChnMux2)
      {
         mux_ch_idx = 0;
      }
      mux_ch_idx_next = mux_ch_idx + 1;
      if (mux_ch_idx_next >= BCTC_CfgNoChnMux2)
      {
         mux_ch_idx_next = 0;
      }

      /* Set multiplexer logic control input for next channel */
      Dio_WriteChannel(DIO_PIN_BCTC_ADG658_Mux_A0,  BCTC_CfgChnAdrMux2[mux_ch_idx] & 0x1);
      Dio_WriteChannel(DIO_PIN_BCTC_ADG658_Mux_A1, (BCTC_CfgChnAdrMux2[mux_ch_idx] & 0x2) >> 1);
      Dio_WriteChannel(DIO_PIN_BCTC_ADG658_Mux_A2, (BCTC_CfgChnAdrMux2[mux_ch_idx] & 0x4) >> 2);

      BCTC_time_stamp++;
   }
   else
   {
      BCTC_time_stamp++;
   }
}
#define BCTC_STOP_SEC_CODE_1MS
#include "MemMap.h"
/* ========== END BCTC_Scheduler_1ms ======================================= */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_Scheduler_10ms(void)
{
   putValueToBuffer((uint16)(((uint32)BCTC_IAnDigCnvrMeasd_ICtctrPos * BCTC_CfgFacCtctrUI) >> 10)  , &buf1);
   putValueToBuffer((uint16)(((uint32)BCTC_IAnDigCnvrMeasd_ICtctrNeg * BCTC_CfgFacCtctrUI) >> 10)  , &buf2);
   putValueToBuffer((uint16)(((uint32)BCTC_IAnDigCnvrMeasd_ICtctrPrec * BCTC_CfgFacCtctrUI) >> 10)  , &buf3);


   BCTC_TLE8104Rx();
   BCTC_TLE8104Tx();
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_Scheduler_10ms ====================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmCtctrPos(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPos_HS_Ctl, STD_HIGH);	/* HS_Ctl - high */
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPos_LS_Ctl, STD_HIGH);	/* LS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPos_HS_Ctl, STD_LOW);		/* HS_Ctl - low */
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPos_LS_Ctl, STD_LOW);		/* LS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_SetPwmCtctrPos ====================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmCtctrNeg(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld)
{
   PrmFlagClsNeg = PrmFlagCls;
	if (PrmFlagClsNeg == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrNeg_HS_Ctl, STD_HIGH);	/* HS_Ctl - high */
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrNeg_LS_Ctl, STD_HIGH);	/* LS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrNeg_HS_Ctl, STD_LOW);		/* HS_Ctl - low */
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrNeg_LS_Ctl, STD_LOW);		/* LS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_SetPwmCtctrNeg ====================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmCtctrPrec(boolean PrmFlagCls, uint8 PrmPwmPeak, uint16 PrmTiPeak, uint8 PrmPwmHld)
{
   PrmFlagClsPrec = PrmFlagCls;
	if (PrmFlagClsPrec == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPrec_HS_Ctl, STD_HIGH);	/* HS_Ctl - high */
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPrec_LS_Ctl, STD_HIGH);	/* LS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPrec_HS_Ctl, STD_LOW);	/* HS_Ctl - low */
		Dio_WriteChannel(DIO_PIN_BCTC_SetPwmCtctrPrec_LS_Ctl, STD_LOW);	/* LS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_SetPwmCtctrPrec ===================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetGainADC1(uint8 PrmGain)
{
   /* plausibility check of ADC gain - only gain 1, 5, 25, 40 or 100 are allowed!*/
   if (PrmGain == 1u || PrmGain == 5u || PrmGain == 25u || PrmGain == 40u || PrmGain == 100u)
   {
      AS8510_1_Gain = PrmGain;
   }
   else
   {
      AS8510_1_Gain = 5u;  /*default gain: 5*/
   }
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_SetGainADC1 ========================================= */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetGainADC2(uint8 PrmIdx, uint8 PrmGain)
{
   /* plausibility check of ADC gain - only gain 1, 5, 25, 40 or 100 are allowed!*/
   if (PrmGain == 1u || PrmGain == 5u || PrmGain == 25u || PrmGain == 40u || PrmGain == 100u)
   {
      AS8510_2_Gain[PrmIdx] = PrmGain;
   }
   else
   {
      AS8510_2_Gain[PrmIdx] = 5u;  /*default gain: 5*/
   }
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_SetGainADC2 ========================================= */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BCTC_GetUMeasDev1(uint8 PrmIdx)
{
	return getValueFromBuffer(&buf4, PrmIdx);
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetUMeasDev1 ======================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BCTC_GetUMeasDev2(uint8 PrmIdx)
{
   return AS8510_2_Mux.MuxMeasSample[PrmIdx];
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetUMeasDev2 ======================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BCTC_GetUReslMeasDev2(uint8 PrmIdx)
{
   return AS8510_2_Mux.MuxResl[PrmIdx];
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetUReslMeasDev2 ======================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BCTC_GetICtctrPos(uint8 PrmIdx)
{
	return getValueFromBuffer(&buf1, PrmIdx);
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetUMeasDev2 ======================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BCTC_GetICtctrNeg(uint8 PrmIdx)
{
   return getValueFromBuffer(&buf2, PrmIdx);
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetICtctrNeg ======================================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint16 BCTC_GetICtctrPrec(uint8 PrmIdx)
{
   return getValueFromBuffer(&buf3, PrmIdx);
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetICtctrPrec ======================================= */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
boolean BCTC_GetErrEltlHiCtctrPos(void)
{
   static boolean PrmFlagErr;
   if (getValueFromBuffer(&buf1, 0) > BCTC_CfgIThdErrHi)
   {
      PrmFlagErr = 1;
   }
   else
   {
      PrmFlagErr = 0;
   }
	return PrmFlagErr;
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetErrEltlHiCtctrPos ================================ */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
boolean BCTC_GetErrEltlHiCtctrNeg(void)
{
   static boolean PrmFlagErr;
   if (getValueFromBuffer(&buf2, 0) > BCTC_CfgIThdErrHi)
   {
      PrmFlagErr = 1;
   }
   else
   {
      PrmFlagErr = 0;
   }
   return PrmFlagErr;
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetErrEltlHiCtctrNeg ================================ */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
boolean BCTC_GetErrEltlHiCtctrPrec(void)
{
   static boolean PrmFlagErr;
   if (getValueFromBuffer(&buf3, 0) > BCTC_CfgIThdErrHi)
   {
      PrmFlagErr = 1;
   }
   else
   {
      PrmFlagErr = 0;
   }
   return PrmFlagErr;
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetErrEltlHiCtctrPrec =============================== */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 BCTC_GetErrEltlLoCtctrPos(void)
{
   uint8 PrmErrSym = 0;
   if (PrmFlagClsPos == 1)
   {
      PrmErrSym = returnPrmErrSym((TLE8104.rx_buffer_UL2 & MASK_LoCtctrPos) >> SHIFT_LoCtctrPos);
   }
   else
   { /* do nothing */ }
	return PrmErrSym;
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetErrEltlLoCtctrPos ================================ */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 BCTC_GetErrEltlLoCtctrNeg(void)
{
   uint8 PrmErrSym = 0;
   if (PrmFlagClsNeg == 1)
   {
      PrmErrSym = returnPrmErrSym((TLE8104.rx_buffer_UL3 & MASK_LoCtctrNeg) >> SHIFT_LoCtctrNeg);
   }
   else
   { /* do nothing */ }
   return PrmErrSym;
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetErrEltlLoCtctrNeg ================================ */


#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 BCTC_GetErrEltlLoCtctrPrec(void)
{
   uint8 PrmErrSym = 0;
   if (PrmFlagClsPrec == 1)
   {
      PrmErrSym = returnPrmErrSym((TLE8104.rx_buffer_UL3 & MASK_LoCtctrPrec) >> SHIFT_LoCtctrPrec);
   }
   else
   { /* do nothing */ }
   return PrmErrSym;
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END BCTC_GetErrEltlLoCtctrPrec =============================== */



#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS0(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl0, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl0, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS1(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl1, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl1, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS2(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl2, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl2, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS3(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl3, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl3, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS4(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl4, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl4, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS5(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl5, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl5, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS6(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl6, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl6, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS7(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl7, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl7, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS8(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl8, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl8, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS9(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl9, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl9, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS10(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl10, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl10, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmLS11(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl11, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_LS_Ctl11, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"




#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmHS0(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl0, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl0, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmHS1(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl1, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl1, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmHS2(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl2, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl2, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"

#define BCTC_START_SEC_CODE_10MS
#include "MemMap.h"
void BCTC_SetPwmHS3(boolean PrmFlagCls)
{
   PrmFlagClsPos = PrmFlagCls;
   if (PrmFlagClsPos == 1)
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl3, STD_HIGH);	/* HS_Ctl - high */
	}
	else
	{
		Dio_WriteChannel(DIO_PIN_BCTC_SetIo_HS_Ctl3, STD_LOW);		/* HS_Ctl - low */
	}
}
#define BCTC_STOP_SEC_CODE_10MS
#include "MemMap.h"




boolean BCTC_GetDSI01(void)
{
	return TRUE;
}


/*!< undefine master define for this module    */
#ifdef MASTER_BCTC_CTCTRCTLDRVR_C_
#undef MASTER_BCTC_CTCTRCTLDRVR_C_
#endif

