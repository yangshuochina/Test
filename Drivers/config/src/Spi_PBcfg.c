/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Spi_PBcfg.c $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2011/02/02 13:38:48MEZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Spi_PBcfg.c  $
 * Revision 1.15 2011/02/02 13:38:48MEZ Cotor Vlad RGB (COTORV) 
 * - Update the SW to clean up the LOG files #3
 * Revision 1.14 2010/12/13 08:38:31MEZ Cotor Vlad RGB (COTORV)
 * - Correction of daisy chain SPI communication of TLE8104E chip
 * Revision 1.13 2010/11/29 14:51:49MEZ Cotor Vlad RGB (COTORV)
 * Implement and integrate complex device driver for electrical hazard protection
 * Revision 1.12 2010/11/25 08:10:21MEZ Cotor Vlad RGB (COTORV)
 * Update complex device driver for real time clock due to specification update
 * Revision 1.11 2010/10/26 18:10:22MESZ Cotor Vlad RGB (COTORV)
 * Correction of the SPI configuration due to erroneous real time clock functionality
 * Correct the elements number of rx_buffer and tx_buffer.
 * Revision 1.10 2010/10/14 18:09:18MESZ Cotor Vlad RGB (COTORV)
 * Update complex device driver for real time clock due to changed SPI configuration.
 * Revision 1.9 2010/10/11 13:02:40MESZ Cotor Vlad RGB (COTORV)
 * Update of the baud rate for all SPI jobs #2
 * Revision 1.8 2010/10/08 13:37:51MESZ Cotor Vlad RGB (COTORV)
 * Update of the baud rate for all SPI jobs
 * Revision 1.7 2010/10/07 13:58:12MESZ Cotor Vlad RGB (COTORV)
 * Implementation and integration of complex device driver for real time clock
 * Update SPI sequence's names due to integration of complex device driver for real time clock
 * Revision 1.6 2010/09/24 17:26:49MESZ Cotor Vlad RGB (COTORV)
 * Update scheduling behavior and add deactivation of programmable gain to extend measurement range
 * Reorganize the SPI Configuration switching to macros
 * Revision 1.5 2010/09/15 15:26:06MESZ Cotor Vlad RGB (COTORV)
 * Correction of SPI driver configuration due to SPI polarity and phase configuration
 * Revision 1.4 2010/08/24 10:21:00MESZ Cotor Vlad RGB (COTORV)
 * Update BSW for xCU power control complex device driver
 * Update due to calculation correction, resolution adjustment
 * Revision 1.3 2010/07/26 09:54:18MESZ Cotor Vlad RGB (COTORV)
 * Update BSW for contactor control complex device driver due to specification change
 * Revision 1.2 2010/07/12 09:44:03MESZ Cotor Vlad RGB (COTORV)
 * Integration of EcuM, Fee, NvM, RamTst
 * Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 * Upfate of ARE9018.lsl file due to EcuM
 * Revision 1.1 2010/05/20 11:29:06MESZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 * Revision 1.1 2010/03/08 14:06:25MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/config/src/project.pj
 * *****************************************************************************/

#include "Std_Types.h"
#include "Spi.h"
#include "Prj_MainCfg.h"

#define N_ELEMENTS(name)  (sizeof(name) / sizeof((name)[0]))


#define SPI_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"





/* Buffers  */
Spi_EBBuffer eb_buf_um1_cmd;
Spi_EBBuffer eb_buf_um1_data;

Spi_EBBuffer eb_buf_ut0_j0_cmd;
Spi_EBBuffer eb_buf_ut0_j0_data;
Spi_EBBuffer eb_buf_ut0_j1_cmd;
Spi_EBBuffer eb_buf_ut0_j1_data;
Spi_EBBuffer eb_buf_ut0_j2_cmd;
Spi_EBBuffer eb_buf_ut0_j2_data;

Spi_EBBuffer eb_buf_ul0;
Spi_EBBuffer eb_buf_ul1;
Spi_EBBuffer eb_buf_ul3_ul2_diag;
Spi_EBBuffer eb_buf_ul3_ul2_cmd;
Spi_EBBuffer eb_buf_ub0;
Spi_EBBuffer eb_buf_ub1;
Spi_EBBuffer eb_buf_uc6;

Spi_EBBuffer eb_buf_ui1_j0_cmd;
Spi_EBBuffer eb_buf_ui1_j0_data;
Spi_EBBuffer eb_buf_ui1_j1_cmd;
Spi_EBBuffer eb_buf_ui1_j1_data;
Spi_EBBuffer eb_buf_ui1_j2_cmd;
Spi_EBBuffer eb_buf_ui1_j2_data;
Spi_EBBuffer eb_buf_ui1_j3_cmd;
Spi_EBBuffer eb_buf_ui1_j3_data;
Spi_EBBuffer eb_buf_ui1_j4_cmd;
Spi_EBBuffer eb_buf_ui1_j4_data;
Spi_EBBuffer eb_buf_ui1_j5_cmd;
Spi_EBBuffer eb_buf_ui1_j5_data;
Spi_EBBuffer eb_buf_ui1_ini_ch0;
Spi_EBBuffer eb_buf_ui1_ini_ch1;

Spi_EBBuffer eb_buf_uh2_j0_cmd;
Spi_EBBuffer eb_buf_uh2_j0_data1;
Spi_EBBuffer eb_buf_uh2_j0_data2;
Spi_EBBuffer eb_buf_uh2_j0_data3;
Spi_EBBuffer eb_buf_uh2_j1_cmd;
Spi_EBBuffer eb_buf_uh2_j1_data1;
Spi_EBBuffer eb_buf_uh2_j1_data2;
Spi_EBBuffer eb_buf_uh2_j1_data3;
Spi_EBBuffer eb_buf_uh2_j2_cmd;
Spi_EBBuffer eb_buf_uh2_j2_data1;
Spi_EBBuffer eb_buf_uh2_j2_data2;
Spi_EBBuffer eb_buf_uh2_j2_data3;
Spi_EBBuffer eb_buf_uh2_j3_cmd;
Spi_EBBuffer eb_buf_uh2_j4_cmd;

Spi_EBBuffer eb_buf_uh2_ini_ch0;
Spi_EBBuffer eb_buf_uh2_ini_ch1;

Spi_EBBuffer eb_buf_uh3_j0_cmd;
Spi_EBBuffer eb_buf_uh3_j0_data1;
Spi_EBBuffer eb_buf_uh3_j0_data2;
Spi_EBBuffer eb_buf_uh3_j0_data3;
Spi_EBBuffer eb_buf_uh3_j1_cmd;
Spi_EBBuffer eb_buf_uh3_j1_data1;
Spi_EBBuffer eb_buf_uh3_j1_data2;
Spi_EBBuffer eb_buf_uh3_j1_data3;
Spi_EBBuffer eb_buf_uh3_j2_cmd;
Spi_EBBuffer eb_buf_uh3_j2_data1;
Spi_EBBuffer eb_buf_uh3_j2_data2;
Spi_EBBuffer eb_buf_uh3_j2_data3;
Spi_EBBuffer eb_buf_uh3_j3_cmd;
Spi_EBBuffer eb_buf_uh3_j4_cmd;
Spi_EBBuffer eb_buf_uh3_ini_ch0;
Spi_EBBuffer eb_buf_uh3_ini_ch1;

Spi_EBBuffer eb_buf_uc6_cmd;

Spi_EBBuffer eb_buf_uc6_init_rst;
Spi_EBBuffer eb_buf_uc6_init_cfgmode;
Spi_EBBuffer eb_buf_uc6_init_data1;
Spi_EBBuffer eb_buf_uc6_init_data2;
Spi_EBBuffer eb_buf_uc6_init_data3;
Spi_EBBuffer eb_buf_uc6_init_txrtsctrl;
Spi_EBBuffer eb_buf_uc6_init_bfpctrl;
Spi_EBBuffer eb_buf_uc6_init_rxb0ctr;
Spi_EBBuffer eb_buf_uc6_init_rxb1ctr;
Spi_EBBuffer eb_buf_uc6_init_normode;

Spi_EBBuffer eb_buf_uc6_j0_txmsgid;
Spi_EBBuffer eb_buf_uc6_j0_txmsgdata;
Spi_EBBuffer eb_buf_uc6_j0_txmsgreq;

Spi_EBBuffer eb_buf_uc6_j1_rdstate;
Spi_EBBuffer eb_buf_uc6_j1_rdstatedata;

Spi_EBBuffer eb_buf_uc6_j2_rxmsgidcmd;
Spi_EBBuffer eb_buf_uc6_j2_rxmsgid;
Spi_EBBuffer eb_buf_uc6_j2_rxmsgdatacmd;
Spi_EBBuffer eb_buf_uc6_j2_rxmsgdata;
Spi_EBBuffer eb_buf_uc6_j2_rxmsgclr;

Spi_EBBuffer eb_buf_um1_wrt;

Spi_EBBuffer ChanBufEepReadStatusCmdRDSR;
Spi_EBBuffer ChanBufReadStatusRcvRDSR;

Spi_EBBuffer ChanBufEepReadDataCmdREAD;
Spi_EBBuffer ChanBufEepReadDataAddrREAD;
Spi_EBBuffer ChanBufEepReadDataRcvRead;

Spi_EBBuffer ChanBufEepWriteDataCmdWREN;
Spi_EBBuffer ChanBufEepWriteDataCmdWRITE;
Spi_EBBuffer ChanBufEepWriteDataAddrWRITE;
Spi_EBBuffer ChanBufEepWriteDataDataWRITE;


#define SPI_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"


#define SPI_START_SEC_VAR_16BIT
#include "MemMap.h"

uint16 rx_buffer[RxTx_buffer_elements];
uint16 tx_buffer[RxTx_buffer_elements];

#define SPI_STOP_SEC_VAR_16BIT
#include "MemMap.h"

#define BaudRate(BRate)             ((40000u / (2u * BRate)) - 1u)

/* Channel macro */
#define Chan1MaxBufSize(sz)	(sz)
#define Chan2DefaultTx(tx)	(tx)
#define Chan3DataWidth(wid)	(wid)
#define Chan4MSBfirst		(1)
#define Chan4LSBfirst		(0)

/* Job baudrate */
#define Job1BaudRate(baud)	            BaudRate(baud)
#define Job2ChipCs(port,pin)			(((port)<<8)|(pin))
#define Job3HwUnit0						(SPI_UNIT_0)
#define Job3HwUnit1					    (SPI_UNIT_1)
#define Job4CsActiveLow					(1)
#define Job4CsActiveHigh				(0)
#define Job5DelayClkCs(clks)			(clks)
#define Job6LatchLeadingEdge			(1)
#define Job6LatchFallingEdge			(0)
#define Job7IdleClkLow					(0)
#define Job7IdleClkHigh					(1)
#define Job8Priority(prio)				(prio)

/* EEPROM AT25LC1024 */
#define Chan2_AT25LC1024	Chan2DefaultTx(0)
#define Chan3_AT25LC1024	Chan3DataWidth(8)
#define Chan4_AT25LC1024	Chan4MSBfirst

#define Job1_AT25LC1024		Job1BaudRate(500)
#define Job2_AT25LC1024		Job2ChipCs(3,7)
#define Job3_AT25LC1024		Job3HwUnit1
#define Job4_AT25LC1024		Job4CsActiveLow
#define Job5_AT25LC1024		Job5DelayClkCs(0)
#define Job6_AT25LC1024		Job6LatchLeadingEdge
#define Job7_AT25LC1024		Job7IdleClkLow
#define Job8_AT25LC1024		Job8Priority(1)


#define SPI_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"
static const Spi_ConfigChannelType spi_cfg_ch[] = {
   {  /* channel 0: UM1 25LC1024 (command) */
      &eb_buf_um1_cmd,        /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      4,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 1: UM1 25LC1024 (data) */
      &eb_buf_um1_data,       /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      256,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 2: UT0 PCA21125 */
      &eb_buf_ut0_j0_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      0x41,                   /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 3: UT0 PCA21125 - 7 bytes */
      &eb_buf_ut0_j0_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      0x41,                   /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 4: UT0 PCA211254 */
      &eb_buf_ut0_j1_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      0x41,                   /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 5: UT0 PCA21125 - 2 bytes */
      &eb_buf_ut0_j1_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      0x41,                   /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 6: UT0 PCA21125 */
      &eb_buf_ut0_j2_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      0x41,                   /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 7: UT0 PCA21125 - 7 bytes */
      &eb_buf_ut0_j2_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      0x41,                   /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 8: UL0 TLE8102 */
      &eb_buf_ul0,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      2,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 9: UL1 TLE8102 */
      &eb_buf_ul1,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      2,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {	/* channel 10: UL3/UL2 cmd TLE8104 */
      &eb_buf_ul3_ul2_cmd,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      3,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 11: UL3/UL2 diag TLE8104 */
      &eb_buf_ul3_ul2_diag,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      3,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 12: UB0 TLE6209 */
      &eb_buf_ub0,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      1,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      0,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 13: UB1 TLE6209 */
      &eb_buf_ub1,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      1,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      0,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 14: UC6 MCP2515 */
      &eb_buf_uc6,            /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      4,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 15: UI1 AS8510  (command) */
      &eb_buf_ui1_j0_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 16: UI1 AS8510 (data)*/
      &eb_buf_ui1_j0_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 17: UI1 AS8510 (command)*/
      &eb_buf_ui1_j1_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 18: UI1 AS8510 (data)*/
      &eb_buf_ui1_j1_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 19: UI1 AS8510 (command)*/
      &eb_buf_ui1_j2_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 20: UI1 AS8510 (data)*/
      &eb_buf_ui1_j2_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 21: UI1 AS8510 (command)*/
      &eb_buf_ui1_j3_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 22: UI1 AS8510 (data)*/
      &eb_buf_ui1_j3_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 23: UI1 AS8510 (command)*/
      &eb_buf_ui1_j4_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 24: UI1 AS8510 (data)*/
      &eb_buf_ui1_j4_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 25: UI1 AS8510 (command)*/
      &eb_buf_ui1_j5_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 26: UI1 AS8510 (data)*/
      &eb_buf_ui1_j5_data,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 27: UI1 AS8510 (init)*/
      &eb_buf_ui1_ini_ch0,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 28: UI1 AS8510 (init)*/
      &eb_buf_ui1_ini_ch1,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 29: UH2 AS8510 (command) */
      &eb_buf_uh2_j0_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 30: UH2 AS8510 (data1) */
      &eb_buf_uh2_j0_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 31: UH2 AS8510 (data2) */
      &eb_buf_uh2_j0_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 32: UH2 AS8510 (data3) */
      &eb_buf_uh2_j0_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   /* --------------------------------------------------------------- */
   {  /* channel 33: UH2 AS8510 (command) */
      &eb_buf_uh2_j1_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 34: UH2 AS8510 (data1) */
      &eb_buf_uh2_j1_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 35: UH2 AS8510 (data2) */
      &eb_buf_uh2_j1_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 36: UH2 AS8510 (data3) */
      &eb_buf_uh2_j1_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 37: UH2 AS8510 (command) */
      &eb_buf_uh2_j2_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 38: UH2 AS8510 (data1) */
      &eb_buf_uh2_j2_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 39: UH2 AS8510 (data2) */
      &eb_buf_uh2_j2_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 40: UH2 AS8510 (data3) */
      &eb_buf_uh2_j2_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 41: UH2 AS8510 (command) */
      &eb_buf_uh2_j3_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 42: UH2 AS8510 (command) */
      &eb_buf_uh2_j4_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 43: UH2 AS8510 (init ch0) */
      &eb_buf_uh2_ini_ch0,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 44: UH2 AS8510 (init ch1) */
      &eb_buf_uh2_ini_ch1,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 45: UH3 AS8510 (command) */
      &eb_buf_uh3_j0_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 46: UH3 AS8510 (data1) */
      &eb_buf_uh3_j0_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 47: UH3 AS8510 (data2) */
      &eb_buf_uh3_j0_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 48: UH3 AS8510 (data3) */
      &eb_buf_uh3_j0_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   /* --------------------------------------------------------------- */
   {  /* channel 49: UH3 AS8510 (command) */
      &eb_buf_uh3_j1_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 50: UH3 AS8510 (data1) */
      &eb_buf_uh3_j1_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 51: UH3 AS8510 (data2) */
      &eb_buf_uh3_j1_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 52: UH3 AS8510 (data3) */
      &eb_buf_uh3_j1_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 53: UH3 AS8510 (command) */
      &eb_buf_uh3_j2_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 54: UH3 AS8510 (data1) */
      &eb_buf_uh3_j2_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 55: UH3 AS8510 (data2) */
      &eb_buf_uh3_j2_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
   {  /* channel 56: UH3 AS8510 (data3) */
      &eb_buf_uh3_j2_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 57: UH3 AS8510 (command) */
      &eb_buf_uh3_j3_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 58: UH3 AS8510 (command) */
      &eb_buf_uh3_j4_cmd,     /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 59: UH3 AS8510 (init ch0) */
      &eb_buf_uh3_ini_ch0,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
/* --------------------------------------------------------------- */
   {  /* channel 60: UH3 AS8510 (init ch1) */
      &eb_buf_uh3_ini_ch1,    /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                    /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      16,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 61: UC6 MCP2515 */
      &eb_buf_uc6_cmd,       /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 62: UC6 MCP2515 */
      &eb_buf_uc6_init_rst,       /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 63: UC6 MCP2515 */
      &eb_buf_uc6_init_cfgmode,       /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 64: UC6 MCP2515 */
      &eb_buf_uc6_init_data1,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 65: UC6 MCP2515 */
      &eb_buf_uc6_init_data2,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 66: UC6 MCP2515 */
      &eb_buf_uc6_init_data3,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 67: UC6 MCP2515 */
      &eb_buf_uc6_init_txrtsctrl,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 68: UC6 MCP2515 */
      &eb_buf_uc6_init_bfpctrl,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 69: UC6 MCP2515 */
      &eb_buf_uc6_init_rxb0ctr,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 70: UC6 MCP2515 */
      &eb_buf_uc6_init_rxb1ctr,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 71: UC6 MCP2515 */
      &eb_buf_uc6_init_normode,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 72: UC6 MCP2515 */
      &eb_buf_uc6_j0_txmsgid,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 73: UC6 MCP2515 */
      &eb_buf_uc6_j0_txmsgdata,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 74: UC6 MCP2515 */
      &eb_buf_uc6_j0_txmsgreq,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },


   {  /* channel 75: UC6 MCP2515 */
      &eb_buf_uc6_j1_rdstate,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 76: UC6 MCP2515 */
      &eb_buf_uc6_j1_rdstatedata,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },


   {  /* channel 77: UC6 MCP2515 */
      &eb_buf_uc6_j2_rxmsgidcmd,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 78: UC6 MCP2515 */
      &eb_buf_uc6_j2_rxmsgid,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },


   {  /* channel 79: UC6 MCP2515 */
      &eb_buf_uc6_j2_rxmsgdatacmd,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 80: UC6 MCP2515 */
      &eb_buf_uc6_j2_rxmsgdata,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 81: UC6 MCP2515 */
      &eb_buf_uc6_j2_rxmsgclr,   /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      128,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                      /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },

   {  /* channel 82: UM1 25LC1024 (wrt) */
      &eb_buf_um1_wrt,        /* Spi_ConfigChannelType::eb_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::rx_buf */
      NULL_PTR,               /* Spi_ConfigChannelType::tx_buf */
      1,                      /* Spi_ConfigChannelType::buffer_size */
      0x0,                    /* Spi_ConfigChannelType::default_tx */
      8,                     /* Spi_ConfigChannelType::tx_data_width */
      1,                      /* Spi_ConfigChannelType::msb_first */
   },
	{/* channel 83: ChannelIdEepReadStatusCmdRDSR */
			&ChanBufEepReadStatusCmdRDSR,
			NULL_PTR,
			NULL_PTR,
			Chan1MaxBufSize(1),
			Chan2_AT25LC1024,
			Chan3_AT25LC1024,
			Chan4_AT25LC1024
	},
	{/* channel 84: ChannelIdEepReadStatusRcvRDSR */
			&ChanBufReadStatusRcvRDSR,
			NULL_PTR,
			NULL_PTR,
			Chan1MaxBufSize(1),
			Chan2_AT25LC1024,
			Chan3_AT25LC1024,
			Chan4_AT25LC1024
	},
	{/* channel 85: ChannelIdEepReadDataCmdREAD */
			&ChanBufEepReadDataCmdREAD,
			NULL_PTR,
			NULL_PTR,
			Chan1MaxBufSize(1),
			Chan2_AT25LC1024,
			Chan3_AT25LC1024,
			Chan4_AT25LC1024
	},
	{/* channel 86: ChannelIdEepReadDataAddrREAD */
			&ChanBufEepReadDataAddrREAD,
			NULL_PTR,
			NULL_PTR,
			Chan1MaxBufSize(3),
			Chan2_AT25LC1024,
			Chan3_AT25LC1024,
			Chan4_AT25LC1024
	},
	{/* channel 87: ChannelIdEepReadDataRcvRead */
			&ChanBufEepReadDataRcvRead,
			NULL_PTR,
			NULL_PTR,
			Chan1MaxBufSize(256),
			Chan2_AT25LC1024,
			Chan3_AT25LC1024,
			Chan4_AT25LC1024
	},
	{/* channel 88: ChannelIdEepWriteDataCmdWREN */
			&ChanBufEepWriteDataCmdWREN,
			NULL_PTR,
			NULL_PTR,
			Chan1MaxBufSize(1),
			Chan2_AT25LC1024,
			Chan3_AT25LC1024,
			Chan4_AT25LC1024
	},
	{/* channel 89: ChannelIdEepWriteDataCmdWRITE */
		&ChanBufEepWriteDataCmdWRITE,
		NULL_PTR,
		NULL_PTR,
		Chan1MaxBufSize(1),
		Chan2_AT25LC1024,
		Chan3_AT25LC1024,
		Chan4_AT25LC1024
	},
	{/* channel 90: ChannelIdEepWriteDataAddrWRITE */
		&ChanBufEepWriteDataAddrWRITE,
		NULL_PTR,
		NULL_PTR,
		Chan1MaxBufSize(3),
		Chan2_AT25LC1024,
		Chan3_AT25LC1024,
		Chan4_AT25LC1024
	},
	{/* channel 91: ChannelIdEepWriteDataDataWRITE */
		&ChanBufEepWriteDataDataWRITE,
		NULL_PTR,
		NULL_PTR,
		Chan1MaxBufSize(256),
		Chan2_AT25LC1024,
		Chan3_AT25LC1024,
		Chan4_AT25LC1024
	}
/* --------------------------------------------------------------- */
};




static const Spi_ConfigJobType spi_cfg_job[] = {
   {  /* job 0: UM1: 25LC1024 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_25LC1024_channel_cmd],         /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_25LC1024_channel_data],        /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_25LC1024,                         /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 7,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase */
      0,                                              /* Spi_ConfigJobType::clock_polarity  */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 1: UT0: PCA21125 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_PCA21125_channel_j0_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_PCA21125_channel_j0_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_PCA21125_j0,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 6,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      0,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 2: UT0: PCA21125 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_PCA21125_channel_j1_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_PCA21125_channel_j1_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_PCA21125_j1,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 6,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      0,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 3: UT0: PCA21125 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_PCA21125_channel_j2_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_PCA21125_channel_j2_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_PCA21125_j2,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 6,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      0,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 4: UL0, UL1: TLE8102 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_TLE8102_1_channel],            /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_TLE8102_1_channel],            /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_TLE8102_1,                        /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 8,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 5: UL0, UL1: TLE8102 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_TLE8102_2_channel],            /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_TLE8102_2_channel],            /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_TLE8102_2,                        /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 8,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 6: UL2, UL3: TLE8104 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_TLE8104_channel_j0_cmd],            /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_TLE8104_channel_j0_cmd],            /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_TLE8104_1,                        /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 9,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 7: UL2, UL3: TLE8104 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_TLE8104_channel_j1_diag],       /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_TLE8104_channel_j1_diag],       /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_TLE8104_2,                        /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 9,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 8: UB0: TLE6209 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_TLE6209_1_channel],            /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_TLE6209_2_channel],            /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_TLE6209_1,                        /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 13,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 9: UB1: TLE6209 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_TLE6209_2_channel],            /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_TLE6209_2_channel],            /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_TLE6209_2,                        /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 13,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 10: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_MCP2515_channel],              /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_MCP2515_channel],              /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 11: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_j0_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_j0_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_j0,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 12: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_j1_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_j1_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_j1,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 13: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_j2_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_j2_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_j2,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 14: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_j3_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_j3_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_j3,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 15: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_j4_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_j4_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_j4,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 16: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_j5_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_j5_data],     /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_j5,                      /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 17: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_0_init],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_0_init],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_ch0_init,                /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 18: UI1: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_0_channel_1_init],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_0_channel_1_init],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_0_ch1_init,                /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 5,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 19: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_j0_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_j0_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_j0,                      /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 20: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_j1_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_j1_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_j1,                      /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 21: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_j2_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_j2_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_j2,                      /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 22: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_j3_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_j3_cmd],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_j3,                      /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 23: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_j4_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_j4_cmd],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_j4,                      /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 24: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_0_init],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_0_init],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_ch0_init,                /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 25: UH2: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_1_channel_1_init],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_1_channel_1_init],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_1_ch1_init,                /* Spi_ConfigJobType::baud_rate */
      (2<<8) | 1,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 26: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_j0_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_j0_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_j0,                      /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 27: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_j1_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_j1_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_j1,                      /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 28: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_j2_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_j2_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_j2,                      /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 29: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_j3_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_j3_cmd],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_j3,                      /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 30: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_j4_cmd],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_j4_cmd],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_j4,                      /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
/* --------------------------------------------------------------- */
   {  /* job 31: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_0_init],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_0_init],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_ch0_init,                /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },
   {  /* job 32: UH3: AS8510 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_AS8510_2_channel_1_init],      /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_AS8510_2_channel_1_init],      /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_AS8510_2_ch1_init,                /* Spi_ConfigJobType::baud_rate */
      (5<<8) | 12,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_0,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      0,                                              /* Spi_ConfigJobType::clock_phase: PH */
      1,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 33: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_cmd],           /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_cmd],           /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */

   },

   {  /* job 34: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_rest],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_rest],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 35: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_cfgmode],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_cfgmode],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },


   {  /* job 36: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_data1],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_data1],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 37: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_data2],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_data2],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 38: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_data3],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_data3],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 39: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_txrtsctrl],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_txrtsctrl],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 40: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_bfpctrl],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_bfpctrl],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 41: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_rxb0ctr],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_rxb0ctr],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 42: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_rxb1ctr],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_rxb1ctr],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 43: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_init_normode],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_init_normode],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },


   {  /* job 44: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j0_txmsgid],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j0_txmsgid],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 45: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j0_txmsgdata],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j0_txmsgdata],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 46: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j0_txmsgreq],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j0_txmsgreq],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },


   {  /* job 47: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j1_rdstatcmd],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j1_rdstatdata],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 48: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j2_rxmsgidcmd],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j2_rxmsgid],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 49: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j2_rxmsgdatacmd],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j2_rxmsgdata],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 50: UC6: MCP2515 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_ExtCan_channel_j2_rxclr],    /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_ExtCan_channel_j2_rxclr],    /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_MCP2515,                          /* Spi_ConfigJobType::baud_rate */
      (1<<8) | 10,                                    /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase: PH */
      0,                                              /* Spi_ConfigJobType::clock_polarity: PO */
      1,                                              /* Spi_ConfigJobType::priority */
   },

   {  /* job 51: UM1: 25LC1024 */
      NULL_PTR,                                       /* Spi_ConfigJobType::notification */
      &spi_cfg_ch[Chip_25LC1024_channel_wrt],         /* Spi_ConfigJobType::first_ch */
      &spi_cfg_ch[Chip_25LC1024_channel_wrt],        /* Spi_ConfigJobType::last_ch */
      BaudRate_Chip_25LC1024,                         /* Spi_ConfigJobType::baud_rate */
      (3<<8) | 7,                                     /* Spi_ConfigJobType::cs */
      SPI_UNIT_1,                                     /* Spi_ConfigJobType::hw_unit */
      1,                                              /* Spi_ConfigJobType::cs_is_low_active */
      0,                                              /* Spi_ConfigJobType::delay_clk_cs */
      1,                                              /* Spi_ConfigJobType::clock_phase */
      0,                                              /* Spi_ConfigJobType::clock_polarity  */
      1,                                              /* Spi_ConfigJobType::priority */
   },
	{ /* job 52: Eep: JobIdEepSeuenReadStatus */
		NULL_PTR,
		&spi_cfg_ch[ChannelIdEepReadStatusCmdRDSR],
		&spi_cfg_ch[ChannelIdEepReadStatusRcvRDSR],
		Job1_AT25LC1024,
		Job2_AT25LC1024,
		Job3_AT25LC1024,
		Job4_AT25LC1024,
		Job5_AT25LC1024,
		Job6_AT25LC1024,
		Job7_AT25LC1024,
		Job8_AT25LC1024
	},
	{ /* job 53: Eep: JobIdEepReadData */
		NULL_PTR,
		&spi_cfg_ch[ChannelIdEepReadDataCmdREAD],
		&spi_cfg_ch[ChannelIdEepReadDataRcvRead],
		Job1_AT25LC1024,
		Job2_AT25LC1024,
		Job3_AT25LC1024,
		Job4_AT25LC1024,
		Job5_AT25LC1024,
		Job6_AT25LC1024,
		Job7_AT25LC1024,
		Job8_AT25LC1024
	},
	{/* job 54: Eep: JobIdEepWriteDataJobWren */
		NULL_PTR,
		&spi_cfg_ch[ChannelIdEepWriteDataCmdWREN],
		&spi_cfg_ch[ChannelIdEepWriteDataCmdWREN],
		Job1_AT25LC1024,
		Job2_AT25LC1024,
		Job3_AT25LC1024,
		Job4_AT25LC1024,
		Job5_AT25LC1024,
		Job6_AT25LC1024,
		Job7_AT25LC1024,
		Job8_AT25LC1024
	},
	{/* job 55: Eep: JobIdEepWriteDataJobWrite */
		NULL_PTR,
		&spi_cfg_ch[ChannelIdEepWriteDataCmdWRITE],
		&spi_cfg_ch[ChannelIdEepWriteDataDataWRITE],
		Job1_AT25LC1024,
		Job2_AT25LC1024,
		Job3_AT25LC1024,
		Job4_AT25LC1024,
		Job5_AT25LC1024,
		Job6_AT25LC1024,
		Job7_AT25LC1024,
		Job8_AT25LC1024
	}
/* --------------------------------------------------------------- */
};

static const Spi_ConfigSequenceType spi_cfg_seq[] = {
   {  /* sequence 0: UM1 25LC1024 */
      NULL_PTR,                               /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_UM1_FirstJob],    /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_UM1_LastJob],     /* Spi_ConfigSequenceType:last_job */
      1,                                      /* Spi_ConfigSequenceType:is_interruptable */
   },
   {	/* sequence 1: UT0 PCA21125 */
      NULL_PTR,                               /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_0_UT0_FirstJob],  /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_0_UT0_LastJob],   /* Spi_ConfigSequenceType:last_job */
      1,                                      /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 2: UT0 PCA21125 */
      NULL_PTR,                               /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_1_UT0_FirstJob],  /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_1_UT0_LastJob],   /* Spi_ConfigSequenceType:last_job */
      1,                                      /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 3: UT0 PCA21125 */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_2_UT0_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_2_UT0_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 4: UL0, UL1: TLE8102 */
      NULL_PTR,                                /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_UL0_UL1_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_UL0_UL1_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                       /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 5: UL2, UL3: TLE8104 */
      NULL_PTR,                                  /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_0_UL2_UL3_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_0_UL2_UL3_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                         /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 6: UB0, UB1: TLE6209 */
      NULL_PTR,                                /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_UB0_UB1_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_UB0_UB1_FirstJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                       /* Spi_ConfigSequenceType:is_interruptable */
   },

   {  /* sequence 7: UC6: MCP2515 */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_UC6_FirstJob],   /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_UC6_LastJob],    /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 8: UI1: AS8510 */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_0_UI1_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_0_UI1_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 9: UI1: AS8510 */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_1_UI1_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_1_UI1_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 10: UH2: AS8510 */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_0_UH2_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_0_UH2_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 11: UH2: AS8510 */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_1_UH2_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_1_UH2_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 12: UH3: AS8510 - From MUX */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_0_UH3_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_0_UH3_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 13: UH3: AS8510 - From MUX */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_1_UH3_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_1_UH3_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },

   {  /* sequence 14: UC6: Mcp2515 - ExtCAN  cmd  */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_0_EXTCAN_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_0_EXTCAN_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },

   {  /* sequence 15: UC6: Mcp2515 - ExtCAN  inti  */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_1_EXTCAN_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_1_EXTCAN_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },

   {  /* sequence 16: UC6: Mcp2515 - ExtCAN  tx msg */
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_2_EXTCAN_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_2_EXTCAN_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },
   {  /* sequence 17: UC6: Mcp2515 - ExtCAN read state*/
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_3_EXTCAN_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_3_EXTCAN_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },

   {  /* sequence 18: UC6: Mcp2515 - ExtCAN rx msg*/
      NULL_PTR,                              /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_4_EXTCAN_FirstJob], /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_4_EXTCAN_LastJob],  /* Spi_ConfigSequenceType:last_job */
      1,                                     /* Spi_ConfigSequenceType:is_interruptable */
   },

   {  /* sequence 19: UM1 25LC1024 */
      NULL_PTR,                               /* Spi_ConfigSequenceType::notification */
      &spi_cfg_job[Sequence_UM1_WrtJob],    /* Spi_ConfigSequenceType:first_job */
      &spi_cfg_job[Sequence_UM1_WrtJob],     /* Spi_ConfigSequenceType:last_job */
      1,                                      /* Spi_ConfigSequenceType:is_interruptable */
   },
	{ /* sequence 20: SequenceIdEepReadStatus */
			NULL_PTR,
			&spi_cfg_job[JobIdEepSeuenReadStatus],
			&spi_cfg_job[JobIdEepSeuenReadStatus],
			1
	},
	{/* sequence 21: SequenceIdEepReadData */
			NULL_PTR,
			&spi_cfg_job[JobIdEepReadData],
			&spi_cfg_job[JobIdEepReadData],
			1
	},
	{/* sequence 22: SequenceIdEepWriteData*/
			NULL_PTR,
			&spi_cfg_job[JobIdEepWriteDataJobWren],
			&spi_cfg_job[JobIdEepWriteDataJobWrite],
			1
	}
};

const Spi_ConfigType spi_config = {
   spi_cfg_ch,                /* Spi_ConfigType::channels  */
   spi_cfg_job,               /* Spi_ConfigType::jobs */
   spi_cfg_seq,               /* Spi_ConfigType::sequences */
   { {0}, {0}},               /* Spi_ConfigType::hw_unit */
   N_ELEMENTS(spi_cfg_ch),    /* Spi_ConfigType::num_of_channels */
   N_ELEMENTS(spi_cfg_job),   /* Spi_ConfigType::num_of_jobs */
   N_ELEMENTS(spi_cfg_seq),   /* Spi_ConfigType::num_of_sequences */
};

#define SPI_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"

