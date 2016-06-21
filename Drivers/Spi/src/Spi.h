/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:29:04MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Spi.h  $
 *  Revision 1.3 2011/03/22 09:29:04MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.2 2010/11/22 08:37:54MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:21:52MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/


#ifndef SPI_H_
#define SPI_H_

#include "team-s.h"
#include "Std_Types.h"
#include "Spi_Cfg.h"

/** @defgroup group_spi Spi 
 *  @ingroup group_spi */
/*@{*/

#define SPI_MODULE_ID         (83)
#define SPI_VENDOR_ID         (TEAMS_VENDOR_ID)
#define SPI_AR_MAJOR_VERSION  (2)
#define SPI_AR_MINOR_VERSION  (2)
#define SPI_AR_PATCH_VERSION  (2)
#define SPI_SW_MAJOR_VERSION  (1)
#define SPI_SW_MINOR_VERSION  (0)
#define SPI_SW_PATCH_VERSION  (0)

/* Driver APIs Service ID */
#define SPI_INIT_API_ID             	((uint8)(0))
#define SPI_DEINIT_API_ID             	((uint8)(1))
#define SPI_WRITEIB_API_ID             	((uint8)(2))
#define SPI_ASYNCTRANSMIT_API_ID       	((uint8)(3))
#define SPI_READIB_API_ID             	((uint8)(4))
#define SPI_SETUPEB_API_ID             	((uint8)(5))
#define SPI_GETSTATUS_API_ID           	((uint8)(6))
#define SPI_GETJOBRESULT_API_ID        	((uint8)(7))
#define SPI_GETSEQUENCERESULT_API_ID   	((uint8)(8))
#define SPI_GETVERSIONINFO_API_ID		((uint8)(9))
#define SPI_SYNCTRANSMIT_API_ID        	((uint8)(10))
#define SPI_GETHWUNITSTATUS_API_ID     	((uint8)(11))
#define SPI_CANCEL_API_ID             	((uint8)(12))
#define SPI_SETASYNCMODE_API_ID        	((uint8)(13))
#define SPI_MAINFUNCTIONHANDLING_API_ID ((uint8)(16))

/* Development Error Codes */
#define SPI_E_PARAM_CHANNEL		((uint8)(0x0A))	/**< A wrong channel parameter to a service was passed. */
#define SPI_E_PARAM_JOB		 	((uint8)(0x0B))	/**< A wrong job parameter to a service was passed.  */
#define SPI_E_PARAM_SEQ 		((uint8)(0x0C))	/**< A wrong sequence parameter to a service was passed.  */
#define SPI_E_PARAM_LENGTH		((uint8)(0x0D))	/**< A wrong length parameter to a service was passed.  */
#define SPI_E_PARAM_UNIT  		((uint8)(0x0E))	/**< A wrong unit parameter to a service was passed.  */
#define SPI_E_UNINIT 			((uint8)(0x1A))	/**< A service was called without a prior usage of Spi_Init().  */
#define SPI_E_SEQ_PENDING 		((uint8)(0x2A))	/**< A service was called in a wrong sequence.  */
#define SPI_E_SEQ_IN_PROCESS 	((uint8)(0x3A))	/**< A synchronous transmission service was called at a wron time.  */
#define SPI_E_ALREADY_INITIALIZED ((uint8)(0x4A))	/**< Spi_Init() was called while Spi driver was already initialized.  */
#define SPI_E_PARAM_CONFIG		((uint8)0x50)	/**< A problem in the configuration was detected. */


/** @page spi Spi
 *
 * @section spi_intro Introduction
 * The Spi function implements the Spi functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_SPI_HandlerDriver.pdf">AUTOSAR_SWS_SPI_HandlerDriver</a>.
 *
 * @subsection spi_nomen Nomenclature
 * @subsubsection spi_names Sequences, Job, ...
 * The Spi specification uses the names sequences, jobs and channels. The following
 * shall ease the usage of them.
 *
 * A sequence contains at least one job. A job contains at least one channel. The
 * channel contains at least one transfer element. A transfer element is the basic
 * communication data element containint 2 to 16 bits. The data type #Spi_DataType is
 * used for the storage of the transfer element.
 *
 * The channel defines the basic transmission format that is valid for all transfer
 * elements of that channel.
 *
 * The job defines the handling of the chipselect. The chipselect remains active
 * for all channels contained in the job.
 *
 * The sequence is a collection of jobs. After each job of a sequence a repriorization
 * takes place. That means that a sequence might get interrupted by another sequence.
 * The repriorization can be disabled by the configuration
 * Spi_ConfigSequenceType::is_interruptable.
 *
 * The channels and jobs can be shared among the jobs and sequences.
 *
 *
 * @subsubsection spi_seq_id Sequence ID
 * Some service of Spi are using a sequence ID. This ID is the index of the
 * sequence in the configuration #Spi_ConfigSequenceType. This means that the
 * 3rd entry of the array in #Spi_ConfigSequenceType has the sequence ID number
 * 2.
 *
 *
 * @subsubsection spi_job_id Job ID
 * Some service of Spi are using a job ID. This ID is the index of the
 * job in the configuration #Spi_ConfigJobType. This means that the
 * 2nd entry of the array in #Spi_ConfigJobType has the job ID number
 * 1.
 *
 *
 * @subsubsection spi_channel_id Channel ID
 * Some service of Spi are using a channel ID. This ID is the index of the
 * channel in the configuration #Spi_ConfigChannelType. This means that the
 * 1st entry of the array in #Spi_ConfigChannelType has the channel ID number
 * 0.
 *
 *
 * @section spi_init Initialization of Spi
 * Before any service to the Spi can be used the initialization function Spi_Init() with a pointer
 * proper configuration set (#Spi_ConfigType) must be called.
 *
 *
 * @section spi_transmission Transmission
 * The Spi supports different transmission types. The Spi driver can operate with
 * or without interrupts controlled by Spi_SetAsyncMode(). Additionally it can
 * transfer data synchronously (Spi_SyncTransmit()) and asynchronously
 * (Spi_AsyncTransmit()). To increase the flexibility even more the transmit
 * and receive buffers can be fixed buffers via Spi_ReadIB() and Spi_WriteIB()
 * or passed via runtime via Spi_SetupEB().
 *
 *
 * @section spi_hw Hardware
 * The Spi driver uses the two SSC modules of the TC1767. The
 * chipselects provided by these modules are not used to support the polling mode
 * and to have no restrictions on the amount and location of the chipselects.
 *
 * The following pins are used by the SSC modules:
 * <table>
 *   <tr><th></th><th colspan="2">SSC0</th><th colspan="2">SSC1</th></tr>
 *   <tr><th></th><th>A</th><th>B</th><th>A</th><th>B</th></tr>
 *   <tr><td>CLK</td><td>P3.2</td><td>n/a</td><td>P2.11</td><td>P1.11</td></tr>
 *   <tr><td>MTSR</td><td>P3.4</td><td>n/a</td><td>P2.12</td><td>P1.8</td></tr>
 *   <tr><td>MRST</td><td>P3.3</td><td>n/a</td><td>P2.10</td><td>P1.9</td></tr>
 * </table>
 *
 * The selection of the MRST A or B lines is done with the setup of
 * Spi_ConfigType::hw_unit. The selection of CLK A or B and MTSR A or B is done
 * by @ref port configuration. The appropriate bits shall be set to
 * alternate 1 function.
 *
 * Example configuration of #Port_ConfigPinType:
 * @code
 * 	// SSC0.MTSR
 *	{ PORT_3_4, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
 *	// SSC0.MRST
 *	{ PORT_3_3, PORT_PIN_LEVEL_HIGH, PCX_NO_INPUT_PULL_DEVICE_CONNECTED, FALSE, FALSE },
 *	// SSC0.CLK
 *	{ PORT_3_2, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
 *
 *	// SSC1.MTSR
 *	{ PORT_2_12, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
 *	// SSC1.MRST
 *	{ PORT_2_10, PORT_PIN_LEVEL_HIGH, PCX_NO_INPUT_PULL_DEVICE_CONNECTED, FALSE, FALSE },
 *	// SSC1.CLK
 *	{ PORT_2_11, PORT_PIN_LEVEL_HIGH, PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1, FALSE, FALSE },
 * @endcode
 *
 * The Spi does not configure the pins with the appropriate modes.
 * This needs to be done with the @ref port driver.
 *
 *
 * @subsection spi_timing Timing
 * @subsubsection spi_timing_cs Chipselect Timing
 * The delay between the setting of chipselect active before the clock starts
 * and setting it inactive after the clock can be controlled with
 * Spi_ConfigJobType::delay_clk_cs. The units in this field are based on STM ticks.
 * After reset STM runs with \f$ \frac{f_{SYS}}{2} \f$. The divider for STM can
 * be changed in the STM_CLC register. This register is written by MCU with the
 * configuration #MCU_STM_CLC_VAL.
 *
 *
 * @subsubsection spi_timing_baud Baudrate
 * The frequency of the CLK output of SSC is controlled by the field
 * Spi_ConfigJobType::baud_rate. Since the SPI is a clock synchronous protocol
 * the frequency and baudrate is identical.
 * The frequency is based on the SSC module frequency \f$ f_{SSC} \f$. This
 * frequency is controlled by the registers SSC0_FDR and SSC1_FDR for SSC0 and
 * SSC1 respectively. These registers are written by MCU with the configuration
 * #MCU_SSC0_FDR_VAL and #MCU_SSC1_FDR_VAL.
 *
 * The following applies:
 * \f[ {baud\_rate} = \frac{f_{SSC}}{2 \times Baud rate} - 1 \f]
 * where (for normal divider mode):
 * \f[ f_{SSC} = f_{SYS} \times \frac{1}{1024 - {FDR.STEP}} \f]
 * or (for fractional divider mode):
 * \f[ f_{SSC} = f_{SYS} \times \frac{FDR.STEP}{1024} \f]
 *
 *
 * @section spi_autosar AutoSAR Conformity
 * The following limitations apply:
 * @li The Spi_ConfigChannelType::tx_data_width is limited to 2 to 16 bits.
 * @li The status #SPI_JOB_FAILED or #SPI_SEQ_FAILED can be never reached since a
 *     SPI master does not provide an error information.
 *
 *
 * @section spi_cfg Configuration
 * @subsection spi_cfg_step_by_step Step by Step
 * @li Configure the SPI pins MTSR and CLK as alternate 1 function pins (@ref port)
 * @li Configure #Spi_ConfigChannelType.
 * @li Configure #Spi_ConfigJobType.
 * @li Configure #Spi_ConfigSequenceType.
 * @li Configure #Spi_ConfigType.
 * @li Configure the interrupts.
 * @li Call the function Spi_Init() with the pointer to the configured structure #Spi_ConfigType
 *     in the startup phase.
 *
 *
 * @subsection spi_cfg_by_example Example
 * Configuration of #Spi_ConfigChannelType. The example shows the configuration
 * of two channels. The first one is set up using the internal buffers while the
 * second is set up using the external buffers.
 *
 * @code
 * // Buffers for first channel
 * Spi_DataType rx_buf0[10];
 * Spi_DataType tx_buf0[10];
 *
 * // Buffer for second channel
 * Spi_EBBuffer eb_buf1;
 *
 * const Spi_ConfigChannelType spi_cfg_ch[] = {
 *   {                   channel ID = 0
 *     NULL_PTR,         Spi_ConfigChannelType::eb_buf: Spi_SetupEB() cannot be used.
 *     rx_buf0,          Spi_ConfigChannelType::rx_buf: buffer area used for Spi_ReadIB()
 *     tx_buf0,          Spi_ConfigChannelType::tx_buf: buffer area used for Spi_WriteIB();
 *     10,               Spi_ConfigChannelType::buffer_size: size of the provided buffer are
 *     0x0,              Spi_ConfigChannelType::default_tx: not used since Spi_ConfigChannelType::tx_buf is specified
 *     8,                Spi_ConfigChannelType::tx_data_width: 8 data bits
 *     0,                Spi_ConfigChannelType::msb_first: LSB is transferred first
 *   },
 *   {                   channel ID = 1
 *     &eb_buf1,         Spi_ConfigChannelType::eb_buf: Spi_SetupEB() is used to setup the data.
 *     NULL_PTR,         Spi_ConfigChannelType::rx_buf: Spi_ReadIB() cannot be used.
 *     NULL_PTR,         Spi_ConfigChannelType::tx_buf: Spi_WriteIB()cannot be used.
 *     3000,             Spi_ConfigChannelType::buffer_size: max. transfer count of that is provided via Spi_SetupEB()
 *     0xFFFF,           Spi_ConfigChannelType::default_tx: if NULL_PTR is passed as transmit pointer to Spi_SetupEB() then this data is transferred.
 *     16,               Spi_ConfigChannelType::tx_data_width: 16 data bits
 *     1,                Spi_ConfigChannelType::msb_first: MSB is transferred first
 *   },
 * };
 * @endcode
 *
 *
 * Configuration of #Spi_ConfigJobType. The example shows the configuration
 * of two jobs.
 * @code
 * const Spi_ConfigJobType spi_cfg_job[] = {
 *   {                   job ID 0
 *     NULL_PTR,         Spi_ConfigJobType::notification: no notification is required
 *     &spi_cfg_ch[0],   Spi_ConfigJobType::first_ch: job starts with channel 0
 *     &spi_cfg_ch[0],   Spi_ConfigJobType::last_ch: job ends with channel 0
 *     100,              Spi_ConfigJobType::baud_rate: TODO: add baudrate (@ref spi_timing_baud)
 *     (6<<8) | 0,       Spi_ConfigJobType::cs: use P6.0 as chipselect
 *     SPI_UNIT_0,       Spi_ConfigJobType::hw_unit: use SSC0
 *     1,                Spi_ConfigJobType::cs_is_low_active: chipselect is low active
 *     0,                Spi_ConfigJobType::delay_clk_cs: no delay between CLK and chipselect (@ref spi_timing_cs)
 *     0,                Spi_ConfigJobType::clock_phase: Shift transmit data on the leading clock edge, latch on trailing edge
 *     0,                Spi_ConfigJobType::clock_polarity: Idle clock line is low, the leading clock edge is low-to-high transition
 *     1,                Spi_ConfigJobType::priority: priority is 1
 *   },
 *   {                   job ID 1
 *     NULL_PTR,         Spi_ConfigJobType::notification: no notification is required
 *     &spi_cfg_ch[0],   Spi_ConfigJobType::first_ch: job starts with channel 0
 *     &spi_cfg_ch[1],   Spi_ConfigJobType::last_ch: job ends with channel 1
 *     200,              Spi_ConfigJobType::baud_rate: TODO: add baudrate (@ref spi_timing_baud)
 *     (6<<8) | 1,       Spi_ConfigJobType::cs: use P6.1 as chipselect
 *     SPI_UNIT_1,       Spi_ConfigJobType::hw_unit: use SSC1
 *     0,                Spi_ConfigJobType::cs_is_low_active: chipselect is high active
 *     10,               Spi_ConfigJobType::delay_clk_cs: 10 STM ticks delay between CLK and chipselect (@ref spi_timing_cs)
 *     0,                Spi_ConfigJobType::clock_phase: Shift transmit data on the leading clock edge, latch on trailing edge
 *     0,                Spi_ConfigJobType::clock_polarity: Idle clock line is low, the leading clock edge is low-to-high transition
 *     0,                Spi_ConfigJobType::priority: priority is 0
 *   },
 * };
 * @endcode
 *
 *
 * Configuration of #Spi_ConfigSequenceType. The example shows the configuration
 * of two sequences. The first is containing job 1 and the second is containing job 0.
 * @code
 * const Spi_ConfigSequenceType spi_cfg_seq[] = {
 *   {                   sequence ID 0
 *     notifySeq0,       Spi_ConfigSequenceType::notification: call the function notifySeq0 at the end of the sequence.
 *     &spi_cfg_job[1],  Spi_ConfigSequenceType:first_job: first job is job ID 1
 *     &spi_cfg_job[1],  Spi_ConfigSequenceType:last_job: last job is job ID 1
 *     1,                Spi_ConfigSequenceType:is_interruptable: the sequence is interruptable.
 *   },
 *   {                   sequence ID 1
 *     NULL_PTR,         Spi_ConfigSequenceType::notification: No notification is required.
 *     &spi_cfg_job[0],  Spi_ConfigSequenceType:first_job: first job is job ID 0
 *     &spi_cfg_job[0],  Spi_ConfigSequenceType:last_job: last job is job ID 0
 *     0,                Spi_ConfigSequenceType:is_interruptable: the sequence is not interruptable.
 *   },
 * };
 * @endcode
 *
 *
 * Configuration of #Spi_ConfigType:
 * @code
 * const Spi_ConfigType spi_cfg = {
 *   spi_cfg_ch,         Spi_ConfigType::channels: channel configuration
 *   spi_cfg_job,        Spi_ConfigType::jobs: job configuration
 *   spi_cfg_seq,        Spi_ConfigType::sequences: sequence configuration
 *   { 0, 0 },           Spi_ConfigType::hw_unit: use MRST A on SSC0 and SSC1
 *   2,                  Spi_ConfigType::num_of_channels: 2 channels
 *   2,                  Spi_ConfigType::num_of_jobs: 2 jobs
 *   2,                  Spi_ConfigType::num_of_sequences: 2 sequences
 * };
 * @endcode
 *
 *
 * The configuration of the interrupts is only required if the interrupt
 * mode shall be used. Based on the operating conditions with or without
 * the OSEK OS the configuration might look different. The following shows
 * the configuration without OSEK OS.
 *
 * File Spi_Irq.c:
 * @code
 * // please note that the interrupt levels must be unique
 * #define SRPN_SSC0_RSRC 2   Interrupt level for SSC0
 * #define SRPN_SSC1_RSRC 3   Interrupt level for SSC1
 *
 * void __interrupt(SRPN_SSC0_RSRC) __enable_ Spi_Irq0(void)
 * {
 *   Spi_InterruptHandler(0);  // call the interrupt processing of Spi hardware unit 0
 * }
 *
 * void __interrupt(SRPN_SSC1_RSRC) __enable_ Spi_Irq1(void)
 * {
 *   Spi_InterruptHandler(1);  // call the interrupt processing of Spi hardware unit 1
 * }
 * @endcode
 *
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Spi_Init(&spi_cfg);
 *   ...
 *   Spi_SetAsyncMode(SPI_INTERRUPT_MODE);
 *   ...
 * }
 * @endcode
 *
 */


/* definition for the HW units */
#define SPI_UNIT_0 0		/**< SSC0 shall be used. */
#define SPI_UNIT_1 1		/**< SSC1 shall be used. */


/** Type for the chipselect reference.
 *
 * The upper byte of this type contains the port number.
 * The lower byte contains the pin number.
 *
 * This format is compatible to the format used in @ref port and @ref dio.
 */
typedef uint16 Spi_ChipselectType;

/** This type defines the different status the Spi driver can have.
 *
 * The status can be obtained by Spi_GetStatus() or Spi_GetHWUnitStatus().
 */
typedef enum {
	SPI_UNINIT,		/**< The Spi driver is not initialized and therefore not usable */
	SPI_IDLE,		/**< The Spi driver is currently not transmitting any job. */
	SPI_BUSY		/**< The Spi driver is currently performing a job. */
} Spi_StatusType;


/** This type defines the different status of the jobs.
 *
 * This status can be obtained with the service Spi_GetJobResult() with
 * the appropriate job ID as parameter.
 */
typedef enum {
	SPI_JOB_OK,			/**< The last transmission has be completed successfully. */
	SPI_JOB_PENDING,	/**< The job is still pending. */
	SPI_JOB_FAILED		/**< The transmission of the has failed. */
} Spi_JobResultType;


/** This type defines the different status of the sequences.
 *
 * This status can be obtained with the service Spi_GetSequenceResult() with
 * the appropriate sequence ID as parameter.
 *
 * The following shows the states and the possible activities related to them.
 *
 * @image latex external_images/seq_states.pdf "SEQ States"
 */
/* @dot
 *   digraph seq_states {
 *     node [style="rounded" shape="box" fontsize="10.0"];
 *     edge [fontsize="8.0"];
 *
 *     OK [label = "SPI_SEQ_OK"];
 *     PENDING [label = "SPI_SEQ_PENDING (waiting)"];
 *     PENDING_ [label = "SPI_SEQ_PENDING (transmitting)"];
 *     PENDING_C [label = "SPI_SEQ_PENDING (cancel requested)"];
 *     CANCELLED [label = "SPI_SEQ_CANCELLED"];
 *
 *     CANCELLED -> PENDING [label="Spi_AsyncTransmit()"];
 *     OK -> PENDING [label="Spi_AsyncTransmit()"];
 *     PENDING -> PENDING_ [label="prioritized"];
 *     PENDING_ -> OK [label="finished / call of notify function"];
 *
 *     PENDING -> PENDING_C [label="Spi_Cancel()"];
 *     PENDING_ -> PENDING_C [label="Spi_Cancel()"];
 *     PENDING_C -> CANCELLED [label="cancel accepted"];
 *     PENDING_C -> PENDING_C [label="Spi_Cancel()"];
 *     CANCELLED -> CANCELLED [label="Spi_Cancel()"];
 *   }
 * @enddot
 */
typedef enum {
	SPI_SEQ_OK,			/**< The last transmission has be completed successfully. */
	SPI_SEQ_PENDING,	/**< The job is still pending. */
	SPI_SEQ_FAILED,		/**< The transmission of the has failed. */
	SPI_SEQ_CANCELLED	/**< The last transmission has been canceled by the user. */
} Spi_SeqResultType;


/** This type is used for the buffer elements of the channels.
 *
 * This forms the transmission element. Since the TC1767 supports only max.
 * 16 bit for a single transmission the size of this data type is 16 bit.
 */
typedef uint16 Spi_DataType;


/** This type is used for the number of data elements of the type #Spi_DataType
 * that needs to be received/transmitted.
 */
typedef uint16 Spi_NumberOfDataType;


/** This data type is used for the identification of the channel (channel ID).
 *
 * The channel ID is the index in to the channel configuration array
 * #Spi_ConfigChannelType (@ref spi_channel_id).
 */
typedef uint8 Spi_ChannelType;


/** This data type is used for the identification of the job (job ID).
 *
 * The job ID is the index in to the job configuration array
 * #Spi_ConfigJobType (@ref spi_job_id).
 * */
typedef uint16 Spi_JobType;


/** This data type is used for the identification of the sequence (sequence ID).
 *
 * The sequence ID is the index in to the sequence configuration array
 * #Spi_ConfigSequenceType (@ref spi_seq_id).
 * */
typedef uint8 Spi_SequenceType;


/** This data type is used for the identification of the HW unit.
 *
 * The possible values are #SPI_UNIT_0 or #SPI_UNIT_1.
 */
typedef uint8 Spi_HWUnitType;


/** This type is used for the specification of the asynchronous mechanism for
 * the SPI units.
 *
 * If the interrupt mode is used the Spi_Irq.c must be adapted to provide the
 * interrupts to the Spi driver.
 *
 * This can only be used if #SPI_LEVEL_DELIVERED is 2.
 */
typedef enum {
	SPI_POLLING_MODE,	/**< The asynchronous mechanism is ensured by polling. */
	SPI_INTERRUPT_MODE	/**< The asynchronous mechanism is ensured by interrupts. */
} Spi_AsyncModeType;


typedef struct {
	const Spi_DataType *tx_buf;
	Spi_DataType *rx_buf;
	Spi_NumberOfDataType cnt;
} Spi_EBBuffer;


/** Type for the configuration of the channel.
 *
 * The channel configuration holds all properties linked to a channel. These properties
 * are valid for all transfer elements.
 */
typedef struct {
	Spi_EBBuffer *eb_buf;		/**< Pointer to memory storage containing EB rx and tx pointers (set to NULL_PTR if IB is required). */
	Spi_DataType *rx_buf;		/**< Pointer to IB receive buffer (set to NULL_PTR if EB is required). */
	Spi_DataType *tx_buf;		/**< Pointer to IB transmit buffer (set to NULL_PTR if EB is required). */
	uint16 buffer_size;			/**< This parameter holds the max. buffer size [1..65535]. */
	Spi_DataType default_tx;	/**< If a NULL_PTR is passed to a transmit function then this value is transferred. */
	uint8 tx_data_width;		/**< This parameter holds the transmit data width [2..16]. */
	uint8 msb_first;			/**< If this parameter is 1 the MSB is transfered first, otherwise the LSB. */
} Spi_ConfigChannelType;


/** Type for the configuration of a job.
 *
 * The job configuration holds all properties linked to a job. These properties
 * are valid for all channels contained in the job. The channels contained in the
 * job are defined by Spi_ConfigJobType::first_ch and Spi_ConfigJobType::last_ch.
 * Both elements are pointer to the respective channel configuration
 * (#Spi_ConfigChannelType). All channels in between them belong to the set that
 * is contained in the job. To be compliant with ISO-C the channel configuration
 * shall be an array of channels.
 *
 * If the job shall only contains a single channel then Spi_ConfigJobType::first_ch and
 * Spi_ConfigJobType::last_ch shall be set to the same #Spi_ConfigChannelType.
 *
 * For the definition of the chipselect the macro #DIO_CHANNEL_ID can be used.
 */
typedef struct {
	void (*notification)(void); /**< This function get called at the end of the completed job or NULL_PTR for no notification. */
	const Spi_ConfigChannelType *first_ch;	/**< This parameter holds the pointer to the first channel contained in this job. */
	const Spi_ConfigChannelType *last_ch;	/**< This parameter holds the pointer to the last channel contained in this job. */
	uint16 baud_rate;			/**< This parameter contains the baudrate for this job. */
	Spi_ChipselectType cs;		/**< This parameter contains the chipselect to use. */
	Spi_HWUnitType hw_unit;		/**< This parameter assigns a HW-Unit to the job (#SPI_UNIT_0 or #SPI_UNIT_1). */
	uint8 cs_is_low_active;		/**< If this parameter is set then the chipselect is low active. */
	uint8  delay_clk_cs;		/**< This defines the delay between clock and CS [0..100us]. */
	uint8 clock_phase;			/**< 0: Shift transmit data on the leading clock edge, latch on trailing edge; 1: Latch receive data on leading clock edge, shift on trailing edge. */
	uint8 clock_polarity;		/**< 0: Idle clock line is low, the leading clock edge is low-to-high transition; 1: Idle clock line is high, the leading clock edge is high-to-low transition. */
	uint8 priority;				/**< This parameter holds the priority of the job [0..3] where 0 is the lowest priority. */
} Spi_ConfigJobType;


/** Type for the configuration of a sequence.
 *
 * The sequence configuration holds all properties linked to a sequence. These properties
 * are valid for all jobs contained in the sequence. The jobs contained in the
 * sequence are defined by Spi_ConfigSequenceType::first_job and
 * Spi_ConfigSequenceType::last_job.
 * Both elements are pointer to the respective job configuration
 * (#Spi_ConfigJobType). All channels in between them belong to the set that
 * is contained in the job. To be compliant with ISO-C the channel configuration
 * shall be an array of channels.
 *
 * If the sequence shall only contains a single job then Spi_ConfigSequenceType::first_job and
 * Spi_ConfigSequenceType::last_job shall be set to the same #Spi_ConfigJobType.
 *
 * The element Spi_ConfigSequenceType::is_interruptable can be set to 0. In this case
 * no repriorization at the end of the job is performed. In this case the performance
 * of the driver is better.
 */
typedef struct {
	void (*notification)(void); /**< This function get called at the end of the completed sequence (set to NULL_PTR if a notification is not required). */
	const Spi_ConfigJobType *first_job;		/**< This parameter holds the pointer to the first job contained in this sequence. */
	const Spi_ConfigJobType *last_job;		/**< This parameter holds the pointer to the last job contained in this sequence. */
	uint8 is_interruptable;		/**< If this parameter is set then the sequence is interruptible after each job. */
} Spi_ConfigSequenceType;


/** This is the initial Spi configuration structure.
 *
 * The configuration structure contains the pointer to all channels, jobs and sequences.
 * Furthermore it contains the configuration for the SSC peripherals.
 *
 * The setup of the element pisel is done with the following layout:
 * <table>
 *   <tr><th>15</th><th>14</th><th>13</th><th>12</th><th>11</th><th>10</th><th>9</th><th>8</th>
 *       <th>7</th><th>6</th><th>5</th><th>4</th><th>3</th><th>2</th><th>1</th><th>0</th></tr>
 *   <tr><th colspan="7">0</th>
 *       <th>STIP</th>
 *       <th colspan=2>0</th>
 *       <th colspan=3>SLSIS</th>
 *       <th>SCIS</th>
 *       <th>SRIS</th>
 *       <th>MRIS</th></tr>
 *   </table>
 *
 *   <table>
 *     <tr><th>Field</th><th>Description</th></tr>
 *     <tr><td>MRIS</td><td>MRIS selects the receive input line in Master Mode. <br>
 *                          0 Receive input line MRSTA is selected<br>
 *                          1 Receive input line MRSTB is selected</td></tr>
 *     <tr><td>other bits</td><td>all other bits are used only in slave mode. The slave mode is not supported - set these bits to 0</td></tr>
 *  </table>
 *
 */
typedef struct {
	const Spi_ConfigChannelType *channels;		/**< This parameter holds the pointer to the list of channels. */
	const Spi_ConfigJobType *jobs;				/**< This parameter holds the pointer to the list of jobs. */
	const Spi_ConfigSequenceType *sequences;	/**< This parameter holds the pointer to the list of sequences. */
	struct {
		uint16 pisel;			/**< This parameter holds the value that is loaded to the SSC.PISEL register. */
	} hw_unit[2];				/**< This array holds the configuration of the SSC modules. */
	uint8 num_of_channels;		/**< This parameter holds the number of channels in this configuration. */
	uint8 num_of_jobs;			/**< This parameter holds the number of jobs in this configuration. */
	uint8 num_of_sequences;		/**< This parameter holds the number of sequences in this configuration. */
} Spi_ConfigType;


#define SPI_START_SEC_CODE
#include "MemMap.h"


/** This service initializes the Spi driver.
 *
 * The function Spi_Init initializes the Spi module. Spi_Init() can only be used
 * on a not initialized Spi function. If a re-initialization with different configuration
 * parameters is required the service Spi_DeInit() needs to be called first.
 *
 * @note After the execution of the function Spi_Init(), the configuration data are accessible
 * and can be used by the Spi module functions.
 *
 * @param cfg   Pointer to the configuration.
 */
extern void Spi_Init(const Spi_ConfigType *cfg);


/** This service shuts down the Spi driver operation.
 *
 * If the Spi driver is busy while this service is used this service will
 * terminate unsuccessfully.
 *
 * @return #E_OK: The shutdown of the Spi driver was successful.\n
 *         #E_NOT_OK: The shutdown of the Spi driver did not succeed.
 */
extern Std_ReturnType Spi_DeInit(void);


/** The function Spi_GetVersionInfo returns the version information of the Spi module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Spi_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter SpiVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = SPI_MODULE_ID;
	versioninfo->sw_major_version = SPI_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = SPI_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = SPI_SW_PATCH_VERSION;
	versioninfo->vendorID = SPI_VENDOR_ID;
}


/** This service switches between the synchronous and asynchronous mode.
 *
 * This function shall only be called while the Spi driver is in #SPI_IDLE.
 * After initialization the Spi driver is in #SPI_POLLING_MODE.
 *
 * @note To use the #SPI_INTERRUPT_MODE the interrupts must be
 *       configured correctly.
 *
 * This service is only available if #SPI_LEVEL_DELIVERED is 2
 *
 * @param mode #SPI_POLLING_MODE: @copydoc #SPI_POLLING_MODE \n
 *             #SPI_INTERRUPT_MODE: @copydoc #SPI_INTERRUPT_MODE
 */
extern Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType mode);


/** This service handles the Spi operation in case of the polling mode.
 *
 * If the polling mode is requested then this function must be periodically
 * called if Spi_AsyncTransmit() is used. It does not harm if this function
 * is called also in #SPI_INTERRUPT_MODE however it is not required to do so.
 *
 * The frequency of the calling of this function determines the transmission
 * rate that can be achieved by the driver. At each call max. one transmission
 * element is transferred.
 */
extern void Spi_MainFunction_Handling(void);


/** This service returns the current status of the Spi driver.
 *
 * This service can be called even with an uninitalized Spi driver. In
 * this case the service will return #SPI_UNINIT.
 *
 * The function will return #SPI_IDLE only if all hardware units are idle.
 *
 * @return #SPI_UNINIT: @copydoc #SPI_UNINIT\n
 *         #SPI_IDLE: @copydoc #SPI_IDLE\n
 *         #SPI_BUSY: @copydoc #SPI_BUSY
 */
extern Spi_StatusType Spi_GetStatus(void);


/** This service returns the current status of a HW unit.
 *
 * This service can be called even with an uninitialized Spi driver. In
 * this case the service will return #SPI_UNINIT.
 *
 * This service is only available if #SPI_HWUNITSTATUS_API is set to STD_ON.
 *
 * @param  unit HW unit number
 * @return #SPI_UNINIT: @copydoc #SPI_UNINIT\n
 *         #SPI_IDLE: @copydoc #SPI_IDLE\n
 *         #SPI_BUSY: @copydoc #SPI_BUSY
 */
extern Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType unit);


/** This service returns the current status of a job.
 *
 * This function returns the current status of a job. The status
 * #SPI_JOB_FAILED is never reached since in SPI Master mode
 * a job can never fail.
 *
 * @param  job_id   A valid job ID (@ref spi_job_id) for which the status shall be returned.
 * @return #SPI_JOB_OK: @copydoc #SPI_JOB_OK\n
 *         #SPI_JOB_PENDING: @copydoc #SPI_JOB_PENDING\n
 *         #SPI_JOB_FAILED: @copydoc #SPI_JOB_FAILED
 */
extern Spi_JobResultType Spi_GetJobResult(Spi_JobType job_id);


/** This service returns the current status of a sequence.
 *
 * This function returns the current status of a sequence. The status
 * #SPI_SEQ_FAILED is never reached since in SPI Master mode
 * a sequence can never fail.
 *
 * The status #SPI_SEQ_CANCELLED can only be returned if the Spi_Cancel()
 * service is available.
 *
 * @param seq_id a valid sequence ID (@ref spi_seq_id) for which the status shall be returned
 * @return #SPI_SEQ_OK: @copydoc #SPI_SEQ_OK\n
 *         #SPI_SEQ_PENDING: @copydoc #SPI_SEQ_PENDING
 *         #SPI_SEQ_FAILED: @copydoc #SPI_SEQ_FAILED
 *         #SPI_SEQ_CANCELLED: @copydoc #SPI_SEQ_CANCELLED
 */
extern Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType seq_id);


/** This service writes data to the internal buffers of the specified channel.
 *
 * This function copies the data from the provide buffer to the internal buffer.
 * To avoid the copying of the data the service Spi_SetupEB() should be used.
 *
 * This function does not initiate a transmission.
 *
 * @warning This service is not tested.
 *
 * @param channel      channel ID (@ref spi_channel_id).
 * @param[in] buffer   A Pointer to the source buffer containing the data shall be transferred.
 *
 * @return #E_OK:     The data was successfully copied.\n
 *         #E_NOT_OK: An error has been occurred.
 */
extern Std_ReturnType Spi_WriteIB(Spi_ChannelType channel, const Spi_DataType *buffer);


/** This service reads data to the internal buffers of the specified channel.
 *
 * This function copies the data from the internal buffer to the buffer provided
 * to this function. It is not possible to directly access the internal buffer due
 * to the AutoSAR specification.
 *
 * To avoid the copying of the data the service Spi_SetupEB() should be used.
 *
 * This function does not initiate a transmission.
 *
 * @warning This service is not tested.
 *
 * @param channel      channel ID (@ref spi_channel_id).
 * @param[out] buffer  A Pointer to the destination buffer to which the received data shall be copied.
 *
 * @return #E_OK:     The data was successfully copied.\n
 *         #E_NOT_OK: An error has been occurred.
 */
extern Std_ReturnType Spi_ReadIB(Spi_ChannelType channel, Spi_DataType *buffer);



/** This service sets up the buffers of the specified channel.
 *
 * The setup function needs to be called only once. The data that is passed here
 * will be used for all subsequent transmissions until the setup is called again
 * with a different data set.
 *
 * If the src_buf is passed as a NULL_PTR then the default data will be transmitted.
 * If the dst_buf is passed as a NULL_PTR then the received data will not be stored.
 *
 * @warning This interface shall be used only if the channel is currently not part of
 *          any started sequence/job.
 *
 * This interface is only available if #SPI_CHANNEL_BUFFERS_ALLOWED is set to STD_ON.
 * This interface can only be used if the configuration of Spi_ConfigChannelType::eb_buf
 * is not a NULL_PTR.
 *
 * This function does not initiate a transmission.
 *
 * @param channel channel ID (@ref spi_channel_id).
 * @param src_buf Pointer to the source buffer or NULL_PTR.
 * @param dst_buf Pointer to the destination buffer or NULL_PTR.
 * @param length  Number of data elements to be transmitted.
 *
 * @return #E_OK: The command has been accepted\n
 *         #E_NOT_OK: An error has been occurred.
 */
extern Std_ReturnType Spi_SetupEB(Spi_ChannelType channel, const Spi_DataType *src_buf,
		Spi_DataType *dst_buf, Spi_NumberOfDataType length);


/** This service triggers the transmission of a complete sequence.
 *
 * This service triggers the transmission of a complete sequence. The
 * function will not return until the complete sequence is transmitted.
 *
 * This interface is only available if #SPI_LEVEL_DELIVERED is 0 or 2.
 *
 * @warning This interface is untested.
 *
 * @param  seq_id     The sequence ID (@ref spi_seq_id).
 * @return #E_OK:     The sequence is scheduled for transmission\n
 *         #E_NOT_OK: The sequence was already scheduled for transmission\n
 *                    Jobs belonging to that sequence are already scheduled for transmission\n
 */
extern Std_ReturnType Spi_SyncTransmit(Spi_SequenceType seq_id);


/** This service triggers a transmission of a complete sequence.
 *
 * This service triggers the transmission of a complete sequence. If the
 * hardware unit of the first job in the sequence is available then
 * the transmission is immediately started. If it is not available
 * then the job is delayed until there is no other job with a higher
 * priority pending (see #Spi_SeqResultType).
 *
 * The sequence is always transmitted as configured. This means even
 * if a job on the other hardware unit might be executed this job
 * is delayed until the previous jobs are finished.
 *
 * This service checks whether a job belonging to this sequence is already
 * scheduled for transmission due to another sequence. If this is the
 * case then the sequence is not scheduled for transmission, the service
 * would return #E_NOT_OK in this case.
 *
 * This interface is only available if #SPI_LEVEL_DELIVERED is 1 or 2.
 *
 * @param  seq_id     The sequence ID (@ref spi_seq_id).
 * @return #E_OK:     The sequence is scheduled for transmission\n
 *         #E_NOT_OK: The sequence was already scheduled for transmission\n
 *                    Jobs belonging to that sequence are already scheduled for transmission\n
 */
extern Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType seq_id);


/** This service cancels the transmission of a sequence.
 *
 * If a job of that sequence is currently in transmission then the active job will be
 * completed but no new job will be started. If the sequence is not yet scheduled
 * for transmission then no job of this sequence is started.
 *
 * It is not possible to start this sequence again until the cancellation has
 * been processed. It is processed as soon as the active job has been finished.
 * The service Spi_GetSequenceResult() is returning #SPI_SEQ_CANCELLED in that
 * case (see #Spi_SeqResultType).
 *
 * This service is only available if #SPI_CANCEL_API is set to STD_ON.
 *
 * @warning This interface is untested.
 *
 * @param  seq_id        The sequence ID (@ref spi_seq_id).
 */
extern void Spi_Cancel(Spi_SequenceType seq_id);


#define SPI_STOP_SEC_CODE
#include "MemMap.h"

/*@}*/

#endif /* SPI_H_ */
