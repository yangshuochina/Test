/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: BCTC_CtctrCtlDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.03.2010
 *  LAST REVISION:      $Date: 2011/02/14 13:01:10MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BCTC_CtctrCtlDrvr_Cfg.h  $
 *  Revision 1.10 2011/02/14 13:01:10MEZ Cotor Vlad RGB (COTORV) 
 *  - Implement OSEK changes due to XCP update - after review
 *  Revision 1.9 2011/02/09 15:34:24MEZ Cotor Vlad RGB (COTORV)
 *  - Change configuration BCTC_CfgUReslBas for high voltage measurement
 *  Revision 1.8 2010/12/13 13:00:21MEZ Cotor Vlad RGB (COTORV)
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.7 2010/11/22 07:17:47MEZ Cotor Vlad RGB (COTORV)
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.6 2010/09/24 18:07:20MESZ Cotor Vlad RGB (COTORV)
 *  Update scheduling behavior and add deactivation of programmable gain to extend measurement range
 *  Reorganize the SPI Configuration switching to macros
 *  Revision 1.5 2010/09/23 11:24:45MESZ Cotor Vlad RGB (COTORV)
 *  Correct the calculation of UActx in BCTC
 *  Revision 1.4 2010/08/25 14:39:33MESZ Cotor Vlad RGB (COTORV)
 *  Implement and integrate complex device driver for cooling fan control
 *  Revision 1.3 2010/07/23 15:34:27MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification change
 *  Revision 1.2 2010/07/19 09:57:00MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for contactor control complex device driver due to specification update
 *  Revision 1.1 2010/05/20 14:01:47MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Bctc/src/project.pj
 *
 * ***************************************************************************/

#ifndef BCTC_CTCTRCTLDRVR_CFG_H_
#define BCTC_CTCTRCTLDRVR_CFG_H_

/* ========== Includes ================================================ */
#include "Std_Types.h"     /*!< Contains the basic data types. */

#include "OsTaskRuntime.h"

/* ========== Declare Alternate Control _EXTERNAL_ definition ========= */
#ifdef  MASTER_BCTC_CTCTRCTLDRVR_CFG_C_  /*! Read from associated modul ?    */
   #define  EXTERNAL_           /*! yes: associated   */
#else
   #define  EXTERNAL_  extern   /*! no: extern modul  */
#endif

/** @defgroup group_bctc_config BCTC Configuration
 *  @ingroup group_bctc
 *
 * This module contains the configuration of the BCTC - Basic software for contactor control complex device driver.
 */
/*@{*/
/* ========== Enumerations, Typedefinitions =========================== */



/* ========== Global variables  ======================================= */



/* ========== Global macros =========================================== */
/* ADC configuration */
#define BCTC_IAnDigCnvrMeasd_ICtctrPos            BCTC_AdcResult1[3]     /**< AN 19 */
#define BCTC_IAnDigCnvrMeasd_ICtctrPrec           BCTC_AdcResult1[4]     /**< AN 20 */
#define BCTC_IAnDigCnvrMeasd_ICtctrNeg            BCTC_AdcResult1[5]     /**< AN 21 */




/* === Port configuration === */
/* Positive High Voltage Contactor */
#define DIO_PIN_BCTC_SetPwmCtctrPos_HS_Ctl      DIO_CHANNEL_ID(DIO_PORT_3, 10)   /**< PIN_137 - P3.10 - High side driver */
#define DIO_PIN_BCTC_SetPwmCtctrPos_LS_Ctl      DIO_CHANNEL_ID(DIO_PORT_2, 2)    /**< PIN_076 - P2.02 - Low side driver */
/* Negative High Voltage Contactor */
#define DIO_PIN_BCTC_SetPwmCtctrNeg_HS_Ctl      DIO_CHANNEL_ID(DIO_PORT_4, 0)    /**< PIN_086 - P4.00 - High side driver */
#define DIO_PIN_BCTC_SetPwmCtctrNeg_LS_Ctl      DIO_CHANNEL_ID(DIO_PORT_2, 5)    /**< PIN_079 - P2.05 - Low side driver */
/* Pre-Charge Contactor */
#define DIO_PIN_BCTC_SetPwmCtctrPrec_HS_Ctl     DIO_CHANNEL_ID(DIO_PORT_3, 11)   /**< PIN_144 - P3.11 - High side driver */
#define DIO_PIN_BCTC_SetPwmCtctrPrec_LS_Ctl     DIO_CHANNEL_ID(DIO_PORT_2, 4)    /**< PIN_078 - P2.04 - Low side driver */


/*Low side Drivers*/

#define DIO_PIN_BCTC_SetIo_LS_Ctl0     DIO_CHANNEL_ID(DIO_PORT_1, 11)   /**< PIN_097 - P1.11 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl1     DIO_CHANNEL_ID(DIO_PORT_2, 0)    /**< PIN_074 - P2.00 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl2     DIO_CHANNEL_ID(DIO_PORT_2, 2)    /**< PIN_076 - P2.02 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl3     DIO_CHANNEL_ID(DIO_PORT_2, 3)    /**< PIN_077 - P2.03 - Low side driver */

#define DIO_PIN_BCTC_SetIo_LS_Ctl4     DIO_CHANNEL_ID(DIO_PORT_2, 4)    /**< PIN_078 - P2.04 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl5     DIO_CHANNEL_ID(DIO_PORT_2, 5)    /**< PIN_079 - P2.05 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl6     DIO_CHANNEL_ID(DIO_PORT_2, 6)    /**< PIN_080 - P2.06 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl7     DIO_CHANNEL_ID(DIO_PORT_2, 7)    /**< PIN_081 - P2.07 - Low side driver */

#define DIO_PIN_BCTC_SetIo_LS_Ctl8     DIO_CHANNEL_ID(DIO_PORT_5, 0)    /**< PIN_109 - P1.06 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl9     DIO_CHANNEL_ID(DIO_PORT_5, 1)    /**< PIN_094 - P1.08 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl10    DIO_CHANNEL_ID(DIO_PORT_5, 3)    /**< PIN_110 - P1.07 - Low side driver */
#define DIO_PIN_BCTC_SetIo_LS_Ctl11    DIO_CHANNEL_ID(DIO_PORT_5, 4)    /**< PIN_095 - P1.09 - Low side driver */

/*High side Drivers*/

#define DIO_PIN_BCTC_SetIo_HS_Ctl0     DIO_CHANNEL_ID(DIO_PORT_3, 10)    /**< PIN_137 - P1.09 - High side Drivers */
#define DIO_PIN_BCTC_SetIo_HS_Ctl1     DIO_CHANNEL_ID(DIO_PORT_3, 11)    /**< PIN_144 - P1.06 - High side Drivers */
#define DIO_PIN_BCTC_SetIo_HS_Ctl2     DIO_CHANNEL_ID(DIO_PORT_4, 0)     /**< PIN_086 - P1.08 - High side Drivers */
#define DIO_PIN_BCTC_SetIo_HS_Ctl3     DIO_CHANNEL_ID(DIO_PORT_4, 1)     /**< PIN_087 - P1.07 - High side Drivers */


/* ADG658 Multiplexer Logic Control */
#define DIO_PIN_BCTC_ADG658_Mux_A0              DIO_CHANNEL_ID(DIO_PORT_6, 0)    /**< PIN_156 - P6.00 - Multiplexer Logic Control Input 0 */
#define DIO_PIN_BCTC_ADG658_Mux_A1              DIO_CHANNEL_ID(DIO_PORT_6, 1)    /**< PIN_157 - P6.01 - Multiplexer Logic Control Input 1 */
#define DIO_PIN_BCTC_ADG658_Mux_A2              DIO_CHANNEL_ID(DIO_PORT_6, 2)    /**< PIN_158 - P6.02 - Multiplexer Logic Control Input 2 */

#define Cfg_PrmFlagCls                          1u          /**< configuration variable for PrmFlagCls */
#define Cfg_PrmPwmPeak                          0u          /**< configuration variable for PrmPwmPeak */
#define Cfg_PrmTiPeak                           0u          /**< configuration variable for PrmTiPeak */
#define Cfg_PrmPwmHld                           0u          /**< configuration variable for PrmPwmHld */

#define Cfg_PrmGain                             2u          /**< configuration variable for PrmGain */
#define Cfg_PrmIdx                              25u         /**< configuration variable for PrmIdx */


/* TLE */
#define Chip_TLE8104_1_init                     0x0030u
#define Chip_TLE8104_2_init                     0x0030u
#define Chip_TLE8104_3_init						0x0030u


/* AS8510 - all */
#define Chip_AS8510_GainOn                      0x15F3u
#define Chip_AS8510_GainOff                     0x15FFu

/* AS8510_1 */
#define Chip_AS8510_1_address_j0_cmd            0x80u        /**< Read command */
#define Chip_AS8510_1_address_j1_cmd            0x80u        /**< Read command */
#define Chip_AS8510_1_address_j2_cmd            0x80u        /**< Read command */
#define Chip_AS8510_1_address_j3_cmd            0x1300u      /**< Write command */

/* AS8510_2 */
#define Chip_AS8510_2_address_j0_cmd            0x80u        /**< Read command */
#define Chip_AS8510_2_address_j1_cmd            0x80u        /**< Read command */
#define Chip_AS8510_2_address_j2_cmd            0x80u        /**< Read command */
#define Chip_AS8510_2_address_j3_cmd            0x1300u      /**< Write command */



/*====================================================================*/
/**
 * Microcontroller measurement recurrence of AS8510_1. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 31.875 \n
 * unit        : [ms] \n
 */
#define BCTC_CfgMeasPerd1                       4u             /**< reccurence - 2ms */


/*====================================================================*/
/**
 * Hardware internal measurement recurrence of AS8510_1. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 31.875 \n
 * unit        : [ms] \n
 * \n
 * This value is used by the initialization and depends on the HW configuration. \n
 * Usually, internal clock (4MHz) is used and divided by 4 (=> fovs = 1MHz). \n
 * Example: BCTC_CfgMeasPerdHW1 = 1ms => fsout = fovs/1024 => register 45 data = 0100_1000. \n
 * \n
 * POR (Power-on Reset) Value = 0100 1000 \n
 *\n
 * ====================================================================\n
 * D[7]: This bit designate which down sampling rate needs to be used for current channel and voltage/Temp.
 * Default is 0 (Down Sampling Rate is 64) \n
 * \n
 * Values: \n
 * 0: Down Sampling Rate is  64 \n
 * 1: Down Sampling Rate is 128 \n
 * ====================================================================\n
 * D[6:5]: These two Bits tells the division ratio over sigma delta clock which will be used as chopper clock.
 * Default is 10 (divide by 512) \n
 * \n
 * Values: \n
 * 00: Chopper Clock Always High \n
 * 01: Divide by  256 \n
 * 10: Divide by  512 \n
 * 11: Divide by 1024 \n
 * ==================================================================== \n
 * D[4:1]: These four bits Tells that down sampling of second CIC stage Default is 0100(16) \n
 * \n
 * Values: \n
 * 0000:     1 \n
 * 0001:     2 \n
 * 0010:     4 \n
 * 0011:     8 \n
 * 0100:    16 \n
 * 0101:    32 \n
 * 0110:    64 \n
 * 0111:   128 \n
 * 1000:   256 \n
 * 1001:   512 \n
 * 1010:  1024 \n
 * 1011:  2048 \n
 * 1100:  4096 \n
 * 1101:  8192 \n
 * 1110: 16384 \n
 * 1111: 32768 \n
 * ==================================================================== \n
 * D[0]: Reserved \n
 * ==================================================================== \n*/
#define BCTC_CfgMeasPerdHW1                     0x0048u        /**< Binary value: 0100 1000 */


/*====================================================================*/
/**
 * Microcontroller measurement recurrence of AS8510_2. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 31.875 \n
 * unit        : [ms] */
#define BCTC_CfgMeasPerd2                       3u             /**< reccurence - 3ms */


/*====================================================================*/
/**
 * Hardware internal measurement recurrence of AS8510_2. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 31.875 \n
 * unit        : [ms] \n
 * \n
  * This value is used by the initialization and depends on the HW configuration. \n
 * Usually, internal clock (4MHz) is used and divided by 4 (=> fovs = 1MHz). \n
 * Example: BCTC_CfgMeasPerdHW2 = 1ms => fsout = fovs/1024 => register 45 data = 0100_1000. \n
 * \n
 * POR (Power-on Reset) Value = 0100 1000 \n
 * \n
 * ==================================================================== \n
 * D[7]: This bit designate which down sampling rate needs to be used for current channel and voltage/Temp.
 * Default is 0 (Down Sampling Rate is 64) \n
 * \n
 * Values: \n
 * 0: Down Sampling Rate is  64 \n
 * 1: Down Sampling Rate is 128 \n
 * ==================================================================== \n
 * D[6:5]: These two Bits tells the division ratio over sigma delta clock which will be used as chopper clock.
 * Default is 10 (divide by 512) \n
 * \n
 * Values: \n
 * 00: Chopper Clock Always High \n
 * 01: Divide by  256 \n
 * 10: Divide by  512 \n
 * 11: Divide by 1024 \n
 * ==================================================================== \n
 * D[4:1]: These four bits Tells that down sampling of second CIC stage Default is 0100(16) \n
 * \n
 * Values: \n
 * 0000:     1 \n
 * 0001:     2 \n
 * 0010:     4 \n
 * 0011:     8 \n
 * 0100:    16 \n
 * 0101:    32 \n
 * 0110:    64 \n
 * 0111:   128 \n
 * 1000:   256 \n
 * 1001:   512 \n
 * 1010:  1024 \n
 * 1011:  2048 \n
 * 1100:  4096 \n
 * 1101:  8192 \n
 * 1110: 16384 \n
 * 1111: 32768 \n
 * ==================================================================== \n
 * D[0]: Reserved \n
 * ==================================================================== \n*/
#define BCTC_CfgMeasPerdHW2                     0x0044u        /**< Binary value: 0100 0100 */


/*====================================================================*/
/**
 * Size of measurement buffer for voltage measurement of AS8510_1. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 255 \n
 * unit        : [-] */
#define BCTC_CfgNoElmMeasBuf1                   5u


/*====================================================================*/
/**
 * Measurement recurrence of contactor primary current measurement. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 31.875 \n
 * unit        : [ms] */
#define BCTC_CfgMeasPerdI                       OS_TASK_10MS


/*====================================================================*/
/**
 * Size of measurement buffer for contactor primary current measurements. \n
 * hex. limits:   00H : FFH \n
 * phys. limits:     0 : 255 \n
 * unit        : [-] */
#define BCTC_CfgNoElmMeasBufI                   1u


/*====================================================================*/
/**
 * Number of used multiplexer channels for voltage measurement of AS8510_2. \n
 * hex. limits:    1H : 8H \n
 * phys. limits:     1 : 8 \n
 * unit        : [-] */
#define BCTC_CfgNoChnMux2                       7u


/*====================================================================*/
/**
 * Multiplexer channel addresses for used channels. - BCTC_CfgChnAdrMux2[BCTC_CfgNoChnMux2] \n\n
 * The number of elements content in this array should be consistent to BCTC_CfgNoChnMux2! \n\n
 * hex. limits	:    0H : 7H \n
 * phys. limits:     0 : 7 \n
 * unit        : [-] */
#define BCTC_CfgChnAdrMux2_default              {0u, 1u, 2u, 3u, 4u, 5u, 6u}

/*====================================================================*/
/**
 * Programmable gain of the voltage measurement for AS8510_1 (0: Gain = 5; 1: Gain = 25; 2: Gain = 40; 3: Gain = 100) \n
 * hex. limits:    0H : 3H \n
 * phys. limits:     0 : 3 \n
 * unit        : [-] \n
 * \n
 * POR (Power-on Reset) Value = 0101 0000 \n
 * \n
 * ==================================================================== \n
 * D[7:6]: These two bits specifies the gain setting of PGA. Default is 01 (G = 25) \n
 * \n
 * Values: \n
 * 00:   5 \n
 * 01:  25 \n
 * 10:  40 \n
 * 11: 100 \n
 * ==================================================================== \n
 * D[5:4]: These two bits specifies the gain setting of voltage channel. Default is 01 (G = 25) \n
 * \n
 * Values: \n
 * 00:   5 \n
 * 01:  25 \n
 * 10:  40 \n
 * 11: 100 \n
 * ==================================================================== \n
 * D[3:0]: Reserved \n
 * ====================================================================\n*/
#define BCTC_CfgGainUIni1                       1u             /**< Gain  5: Binary value: 0000 0000 */

/*====================================================================*/
/**
 * Programmable gain of the voltage measurement for AS8510_2 (0: Gain = 5; 1: Gain = 25; 2: Gain = 40; 3: Gain = 100). \n
 * hex. limits:    0H : 3H \n
 * phys. limits:     0 : 3 \n
 * unit        : [-] \n
 * \n
 * POR (Power-on Reset) Value = 0101 0000 \n
 * \n
 * ==================================================================== \n
 * D[7:6]: These two bits specifies the gain setting of PGA. Default is 01 (G = 25)\n
 * \n
 * Values: \n
 * 00:   5 \n
 * 01:  25 \n
 * 10:  40 \n
 * 11: 100 \n
 * ==================================================================== \n
 * D[5:4]: These two bits specifies the gain setting of voltage channel. Default is 01 (G = 25) \n
 * \n
 * Values: \n
 * 00:   5 \n
 * 01:  25 \n
 * 10:  40 \n
 * 11: 100 \n
 * ==================================================================== \n
 * D[3:0]: Reserved \n
 * ==================================================================== \n*/
#define BCTC_CfgGainUIni2_default               {1u, 5u, 5u, 1u, 5u, 5u, 5u}

/*====================================================================*/
/**
 * Conversion factor between ADC voltage and contactor primary current. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits:     0 : 63.99902344 \n
 * unit        : [A/V] */
#define BCTC_CfgFacCtctrUI                      0x2400u        /**< Decimal value: 9 */


/*====================================================================*/
/**
 * Current threshold for electrical diagnosis of high side drivers. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits:     0 : 79.9987793 \n
 * unit        : [A] */
#define BCTC_CfgIThdErrHi                       0x8666u        /**< Decimal value: 41.99951172 */


/*====================================================================*/
/**
 * Resolution of measured ADC voltage by the AS8510 without gain. \n
 * hex. limits: 0000H : FFFFH \n
 * phys. limits:     0 : 65.535e-3 \n
 * unit        : [V] */
#define BCTC_CfgUReslBas                        0x58A5u        /**< Decimal value: 22.693e-3 */


/*====================================================================*/
/*
 * TLE8104E - SPI Control - Serial Output
 * +--------------+--------------+--------------+--------------+
 * |     CH4      |     CH3      |     CH2      |     CH1      |
 * +--------------+--------------+--------------+--------------+
 * | ch4_1  ch4_0 | ch3_1  ch3_0 | ch2_1  ch2_0 | ch1_1  ch1_0 |
 * +--------------+--------------+--------------+--------------+
 * | MASK  = 0xC0 | MASK  = 0x30 | MASK  = 0x0C | MASK  = 0x03 |
 * | SHIFT = 6    | SHIFT = 4    | SHIFT = 2    | SHIFT = 0    |
 * +--------------+--------------+--------------+--------------+
 */
/* for BCTC_GetErrEltlLoCtctrPos */
#define MASK_LoCtctrPos                         0x30u    /**< 3rd channel of UL2 */
#define SHIFT_LoCtctrPos                        4u
/* for BCTC_GetErrEltlLoCtctrPrec */
#define MASK_LoCtctrPrec                        0x03u    /**< 1st channel of UL3 */
#define SHIFT_LoCtctrPrec                       0u
/* for BCTC_GetErrEltlLoCtctrNeg */
#define MASK_LoCtctrNeg                         0x0Cu    /**< 2nd channel of UL3 */
#define SHIFT_LoCtctrNeg                        2u

/* ========== Global function prototypes  ============================= */



/* ========== End of Global variables (End of header function) ======== */

/*@}*/

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* BCTC_CTCTRCTLDRVR_CFG_H_ */



