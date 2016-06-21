/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Port_PBcfg.c $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2011/02/01 12:45:34MEZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Port_PBcfg.c  $
 * Revision 1.12 2011/02/01 12:45:34MEZ Cotor Vlad RGB (COTORV) 
 * - BMCHIL SW update with interface for HV interlock PWM output
 * Revision 1.11 2011/01/31 14:22:14MEZ Cotor Vlad RGB (COTORV) 
 * - Change output pin for BCU watchdog out signal from P1.03 to P1.05
 * - Also change pin configuration of P1.05 to PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT.
 * Revision 1.10 2010/11/29 11:51:22MEZ Cotor Vlad RGB (COTORV) 
 * Implement and integrate complex device driver for electrical hazard protection
 * Revision 1.9 2010/11/25 09:37:42MEZ Cotor Vlad RGB (COTORV) 
 * Update BMCHIL: New trigger concept for MCs and current senso
 * Revision 1.8 2010/11/24 13:06:47MEZ Opel Martin RGB (OPELM) 
 * - doxygen stuff
 * Revision 1.7 2010/09/21 16:47:35MESZ Cotor Vlad RGB (COTORV) 
 * Correction of SPI PORT configuration due to wrong BCTC SPI communication
 * Revision 1.6 2010/09/20 17:47:29MESZ Cotor Vlad RGB (COTORV)
 * Deactivation of watchdog component in the development phase due to permanent resets
 * Revision 1.5 2010/09/13 12:53:31MESZ Cotor Vlad RGB (COTORV)
 * Correction of SPI driver due to SPI transmission function.
 * Revision 1.4 2010/08/24 10:20:23MESZ Cotor Vlad RGB (COTORV)
 * Update BSW for xCU power control complex device driver
 * Update due to calculation correction, resolution adjustment
 * Revision 1.3 2010/07/30 16:31:04MESZ Cotor Vlad RGB (COTORV)
 * Update BSW for contactor control complex device driver due to specification change
 * Revision 1.2 2010/07/30 16:06:05MESZ Cotor Vlad RGB (COTORV)
 * Update BSW for module controller interface and hardware interlock complex device driver due to BMCHIL scheduler and Dio_ReadChannel
 * Revision 1.1 2010/05/20 11:29:04MESZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 * Revision 1.1 2010/03/08 14:06:24MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/config/src/project.pj
 * *****************************************************************************/

#include "Port.h"

#define PORT_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

/* Port Pin configuration */
const Port_ConfigPinType port_config_pin[] = {
      /* EXTCLK0 */
      { PORT_4_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_3, FALSE, FALSE },

/* === Configuration Evaluation Board === */
      /* LED */
      { PORT_5_0,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_2,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_5,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_6,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
/* === End Configuration Evaluation Board === */

/* === Configuration FOTON === */

      /* CAN #0 */
      /* uC PIN 134 - P3.14 (RXDCAN) */
      { PORT_3_14, PORT_PIN_LEVEL_LOW,  PCX_NO_INPUT_PULL_DEVICE_CONNECTED, FALSE, FALSE },
      /* uC PIN 133 - P3.15 (TXDCAN) */
      { PORT_3_15, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },

      /* CAN #1 */
      /* uC PIN 143 - P3.12 (RXDCAN) */
      { PORT_3_12, PORT_PIN_LEVEL_LOW,  PCX_NO_INPUT_PULL_DEVICE_CONNECTED, FALSE, FALSE },
      /* uC PIN 142 - P3.13 (TXDCAN) */
      { PORT_3_13, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
      /* H-Bridge PWM */
      { PORT_5_1,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
      { PORT_5_4,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },

      /* Bctc */
      /* High Side Driver */
      { PORT_3_10, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_4_0,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_3_11, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /*JAC*/
      { PORT_4_1,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /* 4 High Side Driver */

      /* Low Side Driver */
      { PORT_2_2,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_2_5,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_2_4,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /*JAC*/

      { PORT_1_11, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      { PORT_2_0,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_2_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_2_6,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_2_7,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      { PORT_5_0,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_1,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_5_4,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /*CAN Tja1041 */

      { PORT_1_12,  PORT_PIN_LEVEL_HIGH,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },  //TJA1041  pinEN
      { PORT_1_13,  PORT_PIN_LEVEL_HIGH,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },  //TJA1041  pinSTB
      { PORT_2_13,  PORT_PIN_LEVEL_LOW,   PCX_INPUT_PULL_DOWN_DEVICE_CONNECTED, FALSE, FALSE },  //TJA1041  pinSTB

      /*16 Low side Drivers */


      /* ADG658 Multiplexer Logic Control */
      { PORT_6_0,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_6_1,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_6_2,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /* Belhpn */
      { PORT_0_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      { PORT_0_4,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /* Bpoco */
      /* Power supply enable line */
      { PORT_6_3,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      /* Watchdog line */
      { PORT_5_15, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      /* Fused power relay activation pin */
      { PORT_0_15, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /* Bmchil */
      /* Module controller power supply pin */
      { PORT_0_7,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      /* Module controller ID request line */
      { PORT_0_12, PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      /* High voltage interlock PWM output */
      { PORT_1_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE},
      /* Digital input with diagnostic */
      { PORT_0_8,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_0_9,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_0_10, PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_0_11, PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      /* Hardware interlock inputs */
      { PORT_0_0,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_1_1,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_1_2,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      /* Harware interlock enable line */
      { PORT_5_7,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_5_8,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      { PORT_5_9,  PORT_PIN_LEVEL_LOW,  PCX_INPUT_PULL_UP_DEVICE_CONNECTED, FALSE, FALSE },
      /* Sensor diagnostic signal */
      { PORT_0_2,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      /* Module controller watchdog signal */
      { PORT_1_5,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },
      /* Current sensor trigger signal */
      { PORT_1_4,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /*Bfanc*/
      /* Battery cooling fan */
      { PORT_2_3,  PORT_PIN_LEVEL_LOW,  PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, TRUE, TRUE },

      /* SSC0.MTSR - SPI 0 */
      { PORT_3_4,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
      /* SSC0.MRST */
      { PORT_3_3,  PORT_PIN_LEVEL_HIGH, PCX_NO_INPUT_PULL_DEVICE_CONNECTED, FALSE, FALSE },
      /* SSC0.CLK */
      { PORT_3_2,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },

      /* SSC1.MTSR - SPI 1 */
      { PORT_2_12, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
      /* SSC1.MRST */
      { PORT_2_10, PORT_PIN_LEVEL_HIGH, PCX_NO_INPUT_PULL_DEVICE_CONNECTED, FALSE, FALSE },
      /* SSC1.CLK */
      { PORT_2_11, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },

      /* SPI PINS */

      /* UM1 25LC1024 */
      { PORT_3_7,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UT0 M41T94 */
      { PORT_3_6,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UL0, UL1: TLE8102 */
      { PORT_2_8,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UL2, UL3: TLE8104 */
      { PORT_2_9,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UB0, UB1: TLE6209 */
      { PORT_5_13, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UC6: MCP2515 */
      { PORT_1_10, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UI1: AS8510 */
      { PORT_3_5,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UH2: AS8510 */
      { PORT_2_1,  PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
      /* UH3: AS8510 */
      { PORT_5_12, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT, FALSE, FALSE },
};

/* Port Pad Driver Mode configuration. */
const Port_ConfigPortType port_config_port[] = {
      {
            PORT_0, {
                  /* PD0 Class A1 P0.[7:0] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD1 Class A1 P0.[15:8] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD2 reserved */
                  PORT_PDR_RESERVED,
                  /* PD3 reserved */
                  PORT_PDR_RESERVED,
                  /* PD4 reserved */
                  PORT_PDR_RESERVED,
                  /* PD5 reserved */
                  PORT_PDR_RESERVED,
                  /* PD6 reserved */
                  PORT_PDR_RESERVED,
                  /* PD7 reserved */
                  PORT_PDR_RESERVED,
            }
      },
      {
            PORT_1, {
                  /* PD0 Class A1 P1.[3:1] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD1 Class A1 P1.[7:4] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PDEMUX Class A1 P1.[14:12] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD2 Class A2 P1.15 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PDSSC1B Class A2 P1[11;8] */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PDBRKOUT0 Class A2 P1.0 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD6 reserved */
                  PORT_PDR_RESERVED,
                  /* PD7 reserved */
                  PORT_PDR_RESERVED
            }
      },
      {
            PORT_2, {
                  /* PD0 Class A2 P2.4 P2.[7:6] */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD1 Class A1 P2.13 */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD2,reserved */
                  PORT_PDR_RESERVED,
                  /* PD3 reserved */
                  PORT_PDR_RESERVED,
                  /* PDMLI0 Class A2 P2.0 P2.[3:2] P2.5 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PDMSC0 Class A2 P2.1 P2.[9:8] */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PDSSC1 Class A2 P2.[12:10] */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD7 reserved */
                  PORT_PDR_RESERVED,
            }
      },
      {
            PORT_3, {
                  /* PD0 Class A1 P3.[11:10] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD1 Class A1 P3.9 P3.12 P3.14 */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD2,reserved */
                  PORT_PDR_RESERVED,
                  /* PD3 reserved */
                  PORT_PDR_RESERVED,
                  /* PDASC0 Class A1 P3.[1:0] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PDSSC0 Class A2 P3.[7:2] */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PDASC1 Class A2 P3.8 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PDCAN Class A2 P3.9 P3.12 P3.14 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
            }
      },
      {
            PORT_4, {
                  /* PD0 Class A1 P4.[1:0] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PDEXTCLK1 Class A2 P4.2 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD2,reserved */
                  PORT_PDR_RESERVED,
                  /* PD3 reserved */
                  PORT_PDR_RESERVED,
                  /* PDEXTCLK0 Class A2 P4.3 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD5 reserved */
                  PORT_PDR_RESERVED,
                  /* PD6 reserved */
                  PORT_PDR_RESERVED,
                  /* PD7 reserved */
                  PORT_PDR_RESERVED,
            }
      },
      {
            PORT_5, {
                  /* PD0 Class A1 P5.[3:0] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD1 Class A1 P5.[7:4] */
                  PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
                  /* PD2 Class A2 P 5.8 P5.9 P5.11 P5.14 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD3 reserved */
                  PORT_PDR_RESERVED,
                  /* PDMLI0 Class A2 P5.10 P5.[13:12] P5.15 */
                  PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE,
                  /* PD5 reserved */
                  PORT_PDR_RESERVED,
                  /* PD6 reserved */
                  PORT_PDR_RESERVED,
                  /* PD7 reserved */
                  PORT_PDR_RESERVED,
            }
      },
      {
            PORT_6, {
                  /* PD0 CMOS/LVDS P6.[1:0] */
                  PORT_PDR_CMOS_DRIVER,
                  /* PD1 CMOS/LVDS P6.[3:2] */
                  PORT_PDR_CMOS_DRIVER,
                  /* PD2 reserved */
                  PORT_PDR_RESERVED,
                  /* PD3 reserved */
                  PORT_PDR_RESERVED,
                  /* PD4 reserved */
                  PORT_PDR_RESERVED,
                  /* PD5 reserved */
                  PORT_PDR_RESERVED,
                  /* PD6 reserved */
                  PORT_PDR_RESERVED,
                  /* PD7 reserved */
                  PORT_PDR_RESERVED,
            }
      },
};



const Port_ConfigGptaMultiplexerArray port_config_gpta0_multiplexer_array = {
      /*
       * On-Chip Trigger and Gating Signal Multiplexer Control Registers.
       */
      {
            /* OTMCR1. */
            OTMCRX(7, OTMG_IN0) |
            OTMCRX(6, OTMG_IN0) |
            OTMCRX(5, OTMG_IN0) |
            OTMCRX(4, OTMG_IN0) |
            OTMCRX(3, OTMG_IN0) |
            OTMCRX(2, OTMG_IN0) |
            OTMCRX(1, OTMG_IN0) |
            OTMCRX(0, OTMG_IN0) ,
            /* OTMCR0. */
            OTMCRX(7, OTMG_IN0) |
            OTMCRX(6, OTMG_IN0) |
            OTMCRX(5, OTMG_IN0) |
            OTMCRX(4, OTMG_IN0) |
            OTMCRX(3, OTMG_IN0) |
            OTMCRX(2, OTMG_IN0) |
            OTMCRX(1, OTMG_IN0) |
            OTMCRX(0, OTMG_IN0) ,
      },
      /*
       * Output Multiplexer Control Registers.
       */
      {
            /* OMCRH13 OUT[111:108] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL13 OUT[107:104] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH12 OUT[103:100] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32] */
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL12 OUT[99:96] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32] */
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH11 OUT[95:92] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL11 OUT[91:88] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH10 OUT[87:84] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL10 OUT[83:80] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH9 OUT[79:76] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL9 OUT[75:72] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH8 OUT[71:68] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL8 OUT[67:64] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH7 OUT[63:60] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL7 OUT[59:56] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH6 IN[55:52]/OUT[55:52] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL6 IN[51:48]/OUT[51:48] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH5 IN[47:44]/OUT[47:44] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG_LTC32_63, OMG_IN4),

            /* OMCRL5 IN[43:40]/OUT[43:40] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG_LTC0_31, OMG_IN4) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH4 IN[39:36]/OUT[39:36]selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL4 IN[35:32]/OUT[35:32] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH3 IN[31:28]/OUT[31:28] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL3 IN[27:24]/OUT[27:24] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]*/
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH2 IN[23:20]/OUT[23:20] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL2 IN[19:16]/OUT[19:16] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]*/
            OMCRX(3, OMG_LTC0_31, OMG_IN3) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH1 IN[15:12]/OUT[15:12] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL1 IN[11:08]/OUT[11:08] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40] */
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),

            /* OMCRH0 IN[07:04]/OUT[07:04] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]*/
            OMCRX(7, OMG0, OMG_IN0) | OMCRX(6, OMG0, OMG_IN0) | OMCRX(5, OMG0, OMG_IN0) | OMCRX(4, OMG0, OMG_IN0),

            /* OMCRL0 IN[03:00]/OUT[03:00] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32] */
            OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG0, OMG_IN0) | OMCRX(0, OMG0, OMG_IN0),
      },
      /*
       * LTC Input Multiplexer Control Registers.
       */
      {
            /* LIMCRH7 LTC[63:60] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL7 LTC[59:56] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH6 LTC[55:52] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL6  LTC[51:48] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH5 LTC[47:44] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL5 LTC[43:40] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 1, LIMG_CLK0_7, LIMG_IN7),
            /* LIMCRH4 LTC[39:36] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL4 LTC[35:32] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH3 LTC[31:28] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL3 LTC[27:24] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH2 LTC[23:20] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL2 LTC[19:16] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH1 LTC[15:12] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 1, LIMG_CLK0_7, LIMG_IN7) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL1 LTC[11:08] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 1, LIMG_CLK0_7, LIMG_IN7),
            /* LIMCRH0 LTC[07:04]] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL0 LTC[03:00] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
      },
      /*
       * GTC Input Multiplexer Control Registers.
       */
      {
            /* GIMCRH3 GTC[31:28] selectable group IOG3, LTCG3=LTC[31:24], LTCG7=LTC[63:56], FPC[5:0]/INT[1:0] */
            GIMCRX(7, 0, GIMG0, GIMG_IN0) | GIMCRX(6, 0, GIMG0, GIMG_IN0) | GIMCRX(5, 0, GIMG0, GIMG_IN0) | GIMCRX(4, 0, GIMG0, GIMG_IN0),
            /* GIMCRL3 GTC[27:24] selectable group IOG3, LTCG3=LTC[31:24], LTCG7=LTC[63:56], FPC[5:0]/INT[1:0] */
            GIMCRX(3, 0, GIMG0, GIMG_IN0) | GIMCRX(2, 0, GIMG0, GIMG_IN0) | GIMCRX(1, 0, GIMG0, GIMG_IN0) | GIMCRX(0, 0, GIMG0, GIMG_IN0),
            /* GIMCRH2 GTC[23:20] selectable group IOG2, IOG6, LTCG2=LTC[23:16], LTCG6=LTC[55:48], FPC[5:0]/INT[1:0] */
            GIMCRX(7, 0, GIMG0, GIMG_IN0) | GIMCRX(6, 0, GIMG0, GIMG_IN0) | GIMCRX(5, 0, GIMG0, GIMG_IN0) | GIMCRX(4, 0, GIMG0, GIMG_IN0),
            /* GIMCRL2 GTC[19:16] selectable group IOG2, IOG6, LTCG2=LTC[23:16], LTCG6=LTC[55:48], FPC[5:0]/INT[1:0] */
            GIMCRX(3, 0, GIMG0, GIMG_IN0) | GIMCRX(2, 0, GIMG0, GIMG_IN0) | GIMCRX(1, 0, GIMG0, GIMG_IN0) | GIMCRX(0, 0, GIMG0, GIMG_IN0),
            /* GIMCRH1 GTC[15:12] selectable group IOG1, IOG5, LTCG1=LTC[15:8], LTCG5=LTC[47:40], FPC[5:0]/INT[1:0] */
            GIMCRX(7, 0, GIMG0, GIMG_IN0) | GIMCRX(6, 0, GIMG0, GIMG_IN0) | GIMCRX(5, 0, GIMG0, GIMG_IN0) | GIMCRX(4, 0, GIMG0, GIMG_IN0),
            /* GIMCRL1 GTC[11:08] selectable group IOG1, IOG5, LTCG1=LTC[15:8], LTCG5=LTC[47:40], FPC[5:0]/INT[1:0] */
            GIMCRX(3, 0, GIMG0, GIMG_IN0) | GIMCRX(2, 0, GIMG0, GIMG_IN0) | GIMCRX(1, 0, GIMG0, GIMG_IN0) | GIMCRX(0, 0, GIMG0, GIMG_IN0),
            /* GIMCRH0 GTC[07:04] selectable group IOG0, IOG4, LTCG0=LTC[7:0], LTCG4=LTC[39:32], FPC[5:0]/INT[1:0] */
            GIMCRX(7, 0, GIMG0, GIMG_IN0) | GIMCRX(6, 0, GIMG0, GIMG_IN0) | GIMCRX(5, 0, GIMG0, GIMG_IN0) | GIMCRX(4, 0, GIMG0, GIMG_IN0),
            /* GIMCRL0 GTC[03:00] selectable group IOG0, IOG4, LTCG0=LTC[7:0], LTCG4=LTC[39:32], FPC[5:0]/INT[1:0] */
            GIMCRX(3, 0, GIMG0, GIMG_IN0) | GIMCRX(2, 0, GIMG0, GIMG_IN0) | GIMCRX(1, 0, GIMG0, GIMG_IN0) | GIMCRX(0, 0, GIMG0, GIMG_IN0),

      },
};

const Port_ConfigLtcaMultiplexerArray port_config_ltca2_multiplexer_array = {
      /*
       * Output Multiplexer Control Registers.
       */
      {
            /* OMCRH13 OUT[111:108] selectable group LTCG1=LTC[15:8]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL13 OUT[107:104] selectable group LTCG1=LTC[15:8]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH12 OUT[103:100] selectable group LTCG0=LTC[7:0]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL12 OUT[99:96] selectable group LTCG0=LTC[7:0]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH11 OUT[95:92] selectable group LTCG3=LTC[31:24]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL11 OUT[91:88] selectable group LTCG3=LTC[31:24]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH10 OUT[87:84] selectable group LTCG2=LTC[23:16]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL10 OUT[83:80] selectable group LTCG2=LTC[23:16]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH3 IN[31:28]/OUT[31:28] selectable group LTCG3=LTC[31:24]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL3 IN[27:24]/OUT[27:24] selectable group LTCG3=LTC[31:24]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH2 IN[23:20]/OUT[23:20] selectable group LTCG2=LTC[23:16]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL2 IN[19:16]/OUT[19:16] selectable group LTCG2=LTC[23:16]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH1 IN[15:12]/OUT[15:12] selectable group LTCG1=LTC[15:8]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL1 IN[11:08]/OUT[11:08] selectable group LTCG1=LTC[15:8]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
            /* OMCRH0 IN[07:04]/OUT[07:04] selectable group LTCG0=LTC[7:0]*/
            OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
            /* OMCRL0 IN[03:00]/OUT[03:00] selectable group LTCG0=LTC[7:0]*/
            OMCRX(3, OMG1, OMG_IN0) | OMCRX(2, OMG1, OMG_IN0) | OMCRX(1, OMG1, OMG_IN0) | OMCRX(0, OMG1, OMG_IN0),
      },
      /*
       * LTC Input Multiplexer Control Registers.
       */
      {
            /* LIMCRH3 LTC[31:28] selectable group IOG3, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL3 LTC[27:24] selectable group IOG3, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH2 LTC[23:20] selectable group IOG2, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL2 LTC[19:16] selectable group IOG2, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH1 LTC[15:12] selectable group IOG1, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 1, LIMG0, LIMG_IN0),
            /* LIMCRL1 LTC[11:08] selectable group IOG1, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
            /* LIMCRH0 LTC[07:04] selectable group IOG0, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
            /* LIMCRL0 LTC[03:00] selectable group IOG0, CLOCK[7:0], PDL[3:0]/INT[3:0] */
            LIMCRX(3, 0, LIMG0, LIMG_IN0) | LIMCRX(2, 0, LIMG0, LIMG_IN0) | LIMCRX(1, 0, LIMG0, LIMG_IN0) | LIMCRX(0, 0, LIMG0, LIMG_IN0),
      },
};


const Port_ConfigType port_config = {
      sizeof(port_config_pin) / sizeof(Port_ConfigPinType),
      sizeof(port_config_port) / sizeof(Port_ConfigPortType),
      port_config_pin,
      port_config_port,
      &port_config_gpta0_multiplexer_array,
      &port_config_ltca2_multiplexer_array,
};


#define PORT_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

