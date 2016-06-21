/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Mcu.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:27:26MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Mcu.h  $
 *  Revision 1.4 2011/03/22 09:27:26MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.3 2010/11/24 13:11:19MEZ Opel Martin RGB (OPELM) 
 *  - doxygen stuff
 *  Revision 1.2 2010/11/22 08:30:15MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:18:04MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Mcu/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef MCU_H_
#define MCU_H_

#include "team-s.h"
#include "Std_Types.h"

/** @defgroup group_mcu Mcu
 *  @ingroup group_mcu */
/*@{*/

#define MCU_MODULE_ID         (101)
#define MCU_VENDOR_ID         (TEAMS_VENDOR_ID)
#define MCU_AR_MAJOR_VERSION  (2)
#define MCU_AR_MINOR_VERSION  (2)
#define MCU_AR_PATCH_VERSION  (2)
#define MCU_SW_MAJOR_VERSION  (1)
#define MCU_SW_MINOR_VERSION  (0)
#define MCU_SW_PATCH_VERSION  (0)

/* Driver APIs Service ID */
#define MCU_INIT_API_ID             	((uint8)(0))
#define MCU_INITRAMSECTION_API_ID   	((uint8)(1))
#define MCU_INITCLOCK_API_ID        	((uint8)(2))
#define MCU_DISTRIBUTEPLLCLOCK_API_ID	((uint8)(3))
#define MCU_GETPLLSTATUS_API_ID			((uint8)(4))
#define MCU_GETRESETREASON_API_ID       ((uint8)(5))
#define MCU_GETRESETRAWVALUE_API_ID		((uint8)(6))
#define MCU_PERFORMRESET_API_ID			((uint8)(7))
#define MCU_SETMODE_API_ID      	    ((uint8)(8))
#define MCU_GETVERSIONINFO_API_ID		((uint8)(9))

/* MCU Development Error Codes */
#define MCU_E_PARAM_CONFIG				((uint8)(10))	/**< A wrong parameter to function Mcu_Init() was passed. */
#define MCU_E_PARAM_CLOCK				((uint8)(11))	/**< A wrong parameter to function Mcu_InitClock() was passed. */
#define MCU_E_PARAM_MODE				((uint8)(12))	/**< A wrong parameter to function Mcu_SetMode() was passed. */
#define MCU_E_PARAM_RAMSECTION			((uint8)(13))	/**< A wrong parameter to function Mcu_InitRamSection() was passed. */
#define MCU_E_PLL_NOT_LOCKED			((uint8)(14))	/**< The Mcu_DistributePllClock() was called but the PLL was not locked. */
#define MCU_E_UNINIT					((uint8)(15))	/**< A service was called without a prior Mcu_Init(). */
#define MCU_E_NO_OSC					((uint8)(16))	/**< The oscillator is not operating. */
#define MCU_E_TIMEOUT_K1RDY				((uint8)(17))	/**< The timeout #MCU_TIMEOUT_K1RDY was exceeded. */
#define MCU_E_TIMEOUT_VCOBYST			((uint8)(18))	/**< The timeout #MCU_TIMEOUT_VCOBYST was exceeded. */
#define MCU_E_TIMEOUT_VCOLOCK			((uint8)(19))	/**< The PLL did not get a lock within #MCU_TIMEOUT_VCOLOCK. */



/** @page mcu Mcu
 *
 * @section mcu_intro Introduction
 * The Mcu function implements the Mcu functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_MCU_Driver.pdf">AUTOSAR_SWS_MCU_Driver</a>.
 *
 * The MCU driver provides services for basic microcontroller initialization, power down
 * functionality, reset and microcontroller specific functions required from other MCAL
 * software modules. The initialization services allow a flexible and application related
 * MCU initialization in addition to the start-up code. The start-up code is very MCU specific.
 * The provided start-up code description in this document is for guidance and implies functionality
 * which has to be taken into account before standardized MCU initialization is able to start.
 *
 * The MCU driver accesses the microcontroller hardware directly and is located in the
 * Microcontroller Abstraction Layer (MCAL).
 *
 * MCU driver Features:
 * @li Initialization of MCU clock, PLL, clock prescalers and MCU clock distribution
 * @li Initialization of RAM sections
 * @li Activation of uC reduced power modes
 * @li Activation of a uC reset Provides a service to get the reset reason from hardware
 *
 * @warning The different power modes are not available in this implementation
 *
 * Before the MCU driver can be initialized, a basic initialization of the MCU has to
 * be executed. This MCU specific initialization is typically executed in a start-up code.
 * The start-up code of the MCU shall be executed after power up and any kind of microcontroller
 * reset. It shall perform very basic and microcontroller specific start-up initialization and
 * shall be kept short because the MCU clock and PLL are not yet initialized. The start-up code
 * shall cover MCU specific initialization which is not part of other MCU services or other
 * MCAL drivers. The following description summarizes the basic functionality to be included
 * in the start-up code. It is listed for guidance because some functionality might not be
 * supported in all MCU’s.
 *
 * @li The start-up code shall initialize the base addresses for interrupt and trap vector tables.
 *     These base addresses are provided as configuration parameters or linker/locator setting.
 * @li The start-up code shall initialize the interrupt stack pointer if an interrupt stack is
 *     supported by the MCU. The interrupt stack pointer base address and the stack size are
 *     provided as configuration parameter or linker/locator setting
 * @li The start-up code shall initialize the user stack pointer. The user stack pointer base
 *     address and the stack size are provided as configuration parameter or linker/locator setting.
 * @li If the MCU supports context save operation, the start-up code shall initialize the memory
 *     which is used for context save operation. The maximum amount of consecutive context
 *     save operations is provided as configuration parameter or linker/locator setting.
 * @li The start-up code shall ensure that the MCU internal watchdog shall not be serviced until
 *     the watchdog is initialized from the MCAL watchdog driver. This can be done for example by
 *     increasing the watchdog service time.
 * @li If the MCU supports cache memory for data and/or code, it shall be initialized and enabled
 *     in the start-up code.
 * @li The start-up code shall initialize MCU specific features with respect to internal memory as,
 *     for example, memory protection.
 * @li If external memory is used, the memory shall be initialized in the start-up code.
 * @li The start-up code shall be prepared to support different memory configurations depending
 *     on code location. Different configuration options shall be taken into account for code
 *     execution from external/internal memory.
 * @li The settings of the different memories shall be provided to the start-up code as
 *     configuration parameters.
 * @li In the start-up code a default initialization of the MCU clock system shall be performed
 *     including global clock prescalers. The start-up code shall enable protection mechanisms
 *     for special function registers (SFR’s) if supported by the MCU.
 * @li The start-up code shall initialize all necessary write once registers or registers common
 *     to several drivers where one write, rather than repeated writes, to the register is required
 *     or highly desirable.
 * @li The start-up code shall initialize a minimum amount of RAM in order to allow proper
 *     execution of the MCU driver services and the caller of these services.
 *
 * @section mcu_init Initialization of Mcu
 * Before any service to the Mcu can be used the initialization function Mcu_Init() with a pointer
 * proper configuration set (#Mcu_ConfigType) must be called.
 *
 * @section mcu_ram RAM Initialization
 * After the initialization of the Mcu function the RAM service Mcu_InitRamSection() can be used.
 * With this service the predefined sections of the configuration (#Mcu_RamConfigType) can be
 * initialized. The initialization of the sections can be either with 0 or with a predefined
 * image.
 *
 * The following diagram shows which areas can be initialized by Mcu_InitRamSection().
 *
 * @image latex external_images/memory_map.pdf "Memory Map" height=22cm
 *
 *
 * @warning It must be ensured that neither the variables of Mcu nor the stack is initialized.
 *
 *
 * @section mcu_clock Clock Initialization
 * @subsection mcu_clock_generation Clock Generation
 * The TC1767 provides the following operation modes of the PLL.
 * @li Freerunning Mode\n
 *     In this mode the output clock is generated by VCO only. This is the mode that is entered
 *     with a defect in the external oscillator (e.g. broken crystal).
 * @li Prescaler Mode\n
 *     In this mode the VCO is not used but the output clock is generated out of the oscillator
 *     only.
 * @li Normal Mode\n
 *     In this mode the PLL is active.
 *
 * The TC1767 hardware limits the transition between modes. The following diagram shows the
 * possible transitions between them.
 *
 * @image latex external_images/clock_states.pdf "Clock States"
 *
 * As one can see there is no direct transition from normal mode to normal mode or
 * from freerunning mode to normal mode. Therefore the Mcu always requires the
 * full configuration of all dividers even if the target operation mode shall be
 * the normal mode.
 *
 * In prescaler mode the output frequency is:
 *   @f[ f_{PLL} = \frac{f_{OSC}}{k1} @f]
 *
 * In normal mode the output frequency of is:
 *   @f[ f_{PLL} = \frac{n}{p \times k2} \times f_{OSC} @f]
 *
 * In normal mode the frequency for the VCO shall be between 700 MHz and 800 MHz.
 * The VCO frequency is determined by the following formula:
 *   @f[ f_{VCO} = \frac{f_{OSC}}{p} \times n @f]
 *
 * @subsubsection mcu_clock_distribution_osc Oscilator Watchdog
 * The TC1767 provides means to monitor the proper operation of the
 * oscilator. To enable this feature the configuration of Mcu_ClockConfigType::oscval
 * must contain a value >0.
 *
 * The oscilator watchdog frequency must be setup so that the following is fulfilled:
 *   @f[ 2 MHz < f_{OSCREF} < 3 MHz @f]
 *
 * It is recommended however that the frequency should be as close as
 * possible to 2.5 MHz.
 *
 * The oscilator watchdog frequency is setup with the following formula:
 *   @f[ f_{OSCREF} = \frac{f_{OSC}}{oscval} @f]
 * An oscval of 0 leaves the register SCU.OSCCON.OSCVAL unchanged.
 *
 * An invalid oscilator frequency is recognized if the following is no
 * longer true:
 *   @f[ 1.25 MHz \times oscval <= f_{OSC} <= 7.5 MHz \times oscval @f]
 *   @f[ 1.25 MHz <= f_{OSCREF} <= 7.5 MHz @f]
 *
 *
 * @subsection mcu_clock_distribution Clock Distribution
 * The clock distribution unit receives the clock from the clock generation unit.
 *
 *
 * @image latex external_images/clock_distribution.pdf "Clock distribution"
 *
 * The following restrictions regarding the frequencies apply:
 * @f{eqnarray*}{
 *   f_{LMB} &=& f_{FPI} \ \mbox{or} \ f_{LMB} = 2 \times f_{FPI}   \\
 *   f_{LMB} &=& f_{MCDS} \ \mbox{or} \ f_{LMB} = 2 \times f_{MCDS} \\
 *   f_{LMB} &=& f_{PCP} \ \mbox{or} \ f_{LMB} = 2 \times f_{PCP} \\
 *   f_{MCDS} &>=& f_{FPI} \\
 *   f_{PCP} &>=& f_{FPI}
 * @f}
 *
 *
 *
 * @section mcu_reset Reset Services
 * The Mcu function provides services issue a reset of the microcontroller (Mcu_PerformReset())
 * as well as to identify the last reason of a reset (Mcu_GetResetRawValue(), Mcu_GetResetReason()).
 *
 *
 * @section mcu_autosar AutoSAR Conformance
 * Mcu is not fully AutoSAR conforment due to the following:
 * @li Mcu_InitClock() performs the complete clock initialization
 * @li Mcu_SetMode() is not implemented
 *
 * Due to AutoSAR this driver cannot be used to turn on the PLL in an early start phase. Since the
 * driver needs to store the initialization information from Mcu_Init() for Mcu_InitClock() the
 * variable sections containing that variables need to be initialized already. For the same reason
 * the Mcu_InitRamSection() is less usable.
 *
 *
 * @section mcu_cfg Configuration
 * @subsection mcu_cfg_step_by_step Step by Step
 * @li Configure #Mcu_ClockConfigType with the setup the applies for the oscillator and TC1767
 * @li Configure #Mcu_RamConfigType with the RAM sections that need to be initialized and are
 *     not already initialized by the startup.
 * @li Configure #Mcu_ConfigType.
 * @li Call the function Mcu_Init() with the pointer to the configured structure #Mcu_ConfigType
 *     in the startup phase.
 * @li Call Mcu_InitClock() with the desired clock setup (#Mcu_ClockType).
 * @li Call Mcu_InitRamSection() for all RAM sections that shall be initialized and are not
 *     already initialized by the startup.
 *
 * @subsection mcu_cfg_by_example Example
 *
 * Configuration of #Mcu_ClockConfigType:
 * @code
 * const Mcu_ClockConfigType mcu_clock_cfg = {
 *   NULL_PTR,               Mcu_ClockConfigType::k2_ramp No K2 ramp shall be used
 *   0,                      Mcu_ClockConfigType::k2_ramp_delay no ramp delay
 *   MCU_CLOCK_MODE_NORMAL,  Mcu_ClockConfigType::clock_mode shall be normal mode
 *   8,                      Mcu_ClockConfigType::oscval
 *   1,                      Mcu_ClockConfigType::k1
 *   6,                      Mcu_ClockConfigType::k2
 *   2,                      Mcu_ClockConfigType::p
 *   80,                     Mcu_ClockConfigType::n
 *   0,                      Mcu_ClockConfigType::pcpdiv: divide by 1
 *   0,                      Mcu_ClockConfigType::lmbdiv: divide by 1
 *   1,                      Mcu_ClockConfigType::fpidiv: divide by 2
 *   1                       Mcu_ClockConfigType::mcdsdiv: divide by 2
 * };
 * @endcode
 *
 * Configuration of #Mcu_RamConfigType:
 * @code
 * const Mcu_RamConfigType mcu_ram_cfg[] = {
 *   {
 *     (uint32*)0xAFE80000,         Mcu_RamConfigType::start_addr
 *     NULL_PTR,                    Mcu_RamConfigType::start_image
 *     0x1000                       Mcu_RamConfigType::size
 *   },
 *   {
 *     (uint32*)0xF0050000,         Mcu_RamConfigType::start_addr
 *     (const uint32 *)0x80000000,  Mcu_RamConfigType::start_image
 *     0x2000                       Mcu_RamConfigType::size
 *   },
 * };
 * @endcode
 *
 * Configuration of #Mcu_ConfigType:
 * @code
 * const Mcu_ConfigType mcu_cfg = {
 *   0,              Mcu_ConfigType::Mcu_NumberOfMcuModes: no mode configuration
 *   1,              Mcu_ConfigType::Mcu_NumberOfClockSettings: 1 clock configuration
 *   2,              Mcu_ConfigType::Mcu_NumberOfRamSections: 2 RAM sections
 *   NULL_PTR,       no mode configuration: Mcu_ModeConfigType
 *   &mcu_clock_cfg, pointer to clock configuration: Mcu_ClockConfigType
 *   mcu_ram_cfg     pointer to RAM configuration: Mcu_RamConfigType
 * };
 * @endcode
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Mcu_Init(&mcu_cfg);
 *   ...
 *   Mcu_InitClock(0);
 *   Mcu_DistributePllClock();
 *   ...
 *   Mcu_InitRamSection(0);
 *   Mcu_InitRamSection(1);
 *   ...
 * }
 * @endcode
 *
 */

/* @dot
 *   digraph memory_map {
 *     node [shape=record fontsize="10.0"];
 *     rankdir=LR;
 *     memory [label="<f0>Program Flash\n0x8000 0000 .. 0x801F FFFF\
 *                   |reserved\
 *                   |<df0>Data Flash Bank0\n0x8FE0 0000 .. 0x8FE0 7FFF\
 *                   |reserved\
 *                   |<df1>Data Flash Bank0\n0x8FE1 0000 .. 0x8FE1 7FFF\
 *                   |reserved\
 *                   |<ovr>Overlay Memory\n0x8FE8 0000 .. 0x8FE8 1FFF\
 *                   |reserved\
 *                   |<boot>Boot ROM\n0x8FFF C000 .. 0x8FFF FFFF\
 *                   |reserved\
 *                   |<mf0>Program Flash (mirror)\n0xA000 0000 .. 0xA01F FFFF\
 *                   |reserved\
 *                   |<mdf0>Data Flash Bank0\n0xAFE0 0000 .. 0xAFE0 7FFF\
 *                   |reserved\
 *                   |<mdf1>Data Flash Bank0\n0xAFE1 0000 .. 0xAFE1 7FFF\
 *                   |reserved\
 *                   |<movr>Overlay Memory\n0xAFE8 0000 .. 0xAFE8 1FFF\
 *                   |reserved\
 *                   |<mboot>Boot ROM\n0xAFFF C000 .. 0xAFFF FFFF\
 *                   |reserved\
 *                   |<spram>PMI Scratch Pad RAM\n(SPRAM)\n0xC000 0000 .. 0xC000 5FFF\
 *                   |reserved\
 *                   |<ldram>DMI Local Data RAM\n(LDRAM)\n0xD000 0000 .. 0xD001 1FFF\
 *                   |reserved\
 *                   |<mspram>PMI Scratch-Pad RAM\n(SPRAM)\n0xD400 0000 .. 0xD400 5FFF\
 *                   |reserved\
 *                   |<mldram>DMI Local Data RAM (mirror)\n(LDRAM)\n0xE840 0000 .. 0xE841 1FFF\
 *                   |reserved\
 *                   |<mspram2>PMI Scratch-Pad RAM (mirror)\n(SPRAM)\n0xE850 0000 .. 0xE850 5FFF\
 *                   |reserved\
 *                   |<pram>PCP Data Memory\n(PRAM)\n0xF005 0000 .. 0xF005 1FFF\
 *                   |reserved\
 *                   |<cmem>PCP Code Memory\n(CMEM)\n0xF006 0000 .. 0xF006 3FFF\
 *                   "];
 *     node [shape=plaintext fontsize="10.0"];
 *     covered [label="These memory\nregions can be\ninitialized by\nMcu_InitRamSection()" URL="\ref Mcu_InitRamSection"]
 *     covered -> memory:ovr;
 *     covered -> memory:movr;
 *     covered -> memory:ldram;
 *     covered -> memory:mldram;
 *     covered -> memory:spram;
 *     covered -> memory:mspram;
 *     covered -> memory:mspram2;
 *     covered -> memory:pram;
 *     covered -> memory:cmem;
 *   }
 * @enddot
 *
 * @dot
 *   digraph clock_distribution {
 *     node [shape=record fontsize="10.0"];
 *     rankdir=LR;
 *     ccu [label="<mcds>MCDSDIV\
 *                 |<fpi>FPIDIV\
 *                 |<lmb>LMBDIV\
 *                 |<pcp>PCPDIV\
 *                 |<refclk>REFCLK"];
 *     CGU -> ccu [label="fPLL" fontsize="8.0"];
 *     ccu:mcds -> MCDS [label="f MCDS" fontsize="8.0"];
 *     ccu:fpi -> FPI [label="f FPI" fontsize="8.0"];
 *     ccu:lmb -> LMB [label="f LMB" fontsize="8.0"];
 *     ccu:pcp -> PCP [label="f PCP" fontsize="8.0"];
 *     ccu:refclk -> REFCLK1 [label="f REFCLK1" fontsize="8.0"];
 *   }
 * @enddot
 */

/** Mcu Clock configuration */
typedef struct {
	const uint8 *k2_ramp;			/**< The different K2 values for the ramp up of PLL divider K2; terminated with 0; use NULL_PTR for no ramp up. */
	uint32 k2_ramp_delay;			/**< Delay time that is waited between the K2 increments. */
	enum {
		MCU_CLOCK_MODE_PRESCALER,	/**< The target clock mode shall be prescaler mode. */
		MCU_CLOCK_MODE_NORMAL		/**< The target clock mode shall be normal mode. */
	} clock_mode;					/**< The selection of the target clock mode is done here. */
	uint8	oscval;					/**< This is the value for the oscilator watchdog [0..32]. */
	uint8	k1;						/**< The k1 divider [1..128] (see @ref mcu_clock_generation). */
	uint8	k2;						/**< The k2 divider [1..128] (see @ref mcu_clock_generation). */
	uint8	p;						/**< The p divider [1..16] (see @ref mcu_clock_generation). */
	uint8	n;						/**< The n divider [1..128] (see @ref mcu_clock_generation). */
	uint8	pcpdiv;					/**< Frequency for PCP [0..15]: @f$ f_{PCP} = \frac{f_{PLL}}{pcpdiv + 1} @f$ (see @ref mcu_clock_distribution). */
	uint8	lmbdiv;					/**< Frequency for LMB [0..15]: @f$ f_{LMB} = \frac{f_{PLL}}{lmbdiv + 1} @f$ (see @ref mcu_clock_distribution). */
	uint8	fpidiv;					/**< Frequency for FPI [0..15]: @f$ f_{FPI} = \frac{f_{PLL}}{fpidiv + 1} @f$ (see @ref mcu_clock_distribution). */
	uint8	mcdsdiv;				/**< Frequency for MCDS [0..15]: @f$ f_{MCDS} = \frac{f_{PLL}}{mcdsdiv + 1} @f$ (see @ref mcu_clock_distribution). */
} Mcu_ClockConfigType;


/** Mcu RAM configuration.
 *
 * This is the configuration of the initialization of one RAM section. This configuration
 * has 3 parameters. The parameter start_addr holds the start address of the RAM section
 * that shall be initialized.
 * The parameter #size holds the size of the RAM section.
 * The parameter #start_image can be either NULL_PTR if the RAM section shall be filled
 * with 0 or it can hold the start of the image that shall be copied to the RAM section.
 *
 * @note The parameter start_image and start_addr must be 4 byte aligned.
 * @note The parameter size must be a multiple of 4.
 */
typedef struct {
	uint32       *start_addr;	/**< This is the start address of the region that shall be initialized. */
	const uint32 *start_image;  /**< This is a pointer to the image from which the RAM section shall be initialized or NULL_PTR if it shall be cleared only. */
	sint32        size;       	/**< This is the size of the region in bytes that shall be initialized. */
} Mcu_RamConfigType;


/** Mcu Mode configuration.
 *
 * Different MCU modes are not supported by this implementation.
 */
typedef struct {
	uint8 dummy;
} Mcu_ModeConfigType;


/** This is the initial Mcu configuration structure */
typedef struct {
	uint8 Mcu_NumberOfMcuModes;                  /**< This parameter holds the number of modes in this configuration. */
	uint8 Mcu_NumberOfClockSettings;             /**< This parameter holds the number of clock settings in this configuration. */
	uint8 Mcu_NumberOfRamSections;               /**< This parameter holds the number of RAM sections contained in this configuration. */
	const Mcu_ModeConfigType  *Mcu_ModeConfig;   /**< This parameter holds the pointer to the mode configurations. */
	const Mcu_ClockConfigType *Mcu_ClockConfig;  /**< This parameter holds the pointer to the clock configurations. */
	const Mcu_RamConfigType   *Mcu_RamConfig;    /**< This parameter holds the pointer to the RAM section configurations. */
} Mcu_ConfigType;



/** This is a status value returned by the function Mcu_GetPllStatus of the MCU module. */
typedef enum {
	MCU_PLL_LOCKED,            /**< PLL is locked */
	MCU_PLL_UNLOCKED,          /**< PLL is unlocked */
	MCU_PLL_STATUS_UNDEFINED   /**< PLL Status is unknown */
} Mcu_PllStatusType;


/** This is the type of the reset enumerator containing the subset of reset types. */
typedef enum {
	MCU_POWER_ON_RESET, /**< Power On Reset (default) */
	MCU_WATCHDOG_RESET, /**< Internal Watchdog Timer Reset */
	MCU_SW_RESET,       /**< Software Reset */
	MCU_RESET_UNDEFINED /**< For all other reset sources this value is returned */
} Mcu_ResetType;


/** Specifies the identification (ID) for a clock setting, which is configured in the configuration structure */
typedef uint8 Mcu_ClockType;


/** This type specifies the reset reason in raw register format read from a reset status register. */
typedef uint32 Mcu_RawResetType;


/** This type specifies the identification (ID) for a MCU mode, which is configured in the configuration structure. */
typedef uint8 Mcu_ModeType;


/** This type specifies the identification (ID) for a RAM section, which is configured in the configuration structure. */
typedef uint8 Mcu_RamSectionType;


#define MCU_START_SEC_CODE
#include "MemMap.h"


/** This service initializes the MCU driver.
 *
 * The function Mcu_Init initializes the MCU module.
 *
 * @note After the execution of the function Mcu_Init(), the configuration data are accessible
 * and can be used by the MCU module functions as, e.g., Mcu_InitRamSection.
 *
 * @param cfg This is a pointer to the configuration set that shall be used by the driver.
 */
extern void Mcu_Init(const Mcu_ConfigType *cfg);

/* @dot
*   digraph ram_init_example {
*     rankdir=LR;
*
*     node [shape=plaintext fontsize="10.0"];
*     label_cfg [label="Mcu_ConfigType" URL="\ref Mcu_ConfigType"]
*     label_ramcfg [label="Mcu_RamConfigType" URL="\ref Mcu_RamConfigType"]
*     label_memory [label="Memory Areas"]
*
*     node [shape=record];
*     cfg [label="Mcu_NumberOfMcuModes=0|Mcu_NumberOfClockSettings=0|Mcu_NumberOfRamSections=3|Mcu_ClockConfig=NULL_PTR|Mcu_ModeConfig=NULL_PTR|<c>Mcu_RamConfig"];
*     ram_cfg [label="<c0>0: start_addr=A, size=S1|<c1>1: start_addr=B, size=S2|<c2>2: start_addr=C, size=S3, start_image=D}"];
*     ram0 [label="RAM area at A with size S1"]
*     ram1 [label="RAM area at B with size S2"]
*     ram2 [label="RAM area at C with size S3"]
*     image [label="image at D with size s3"]
*     { rank=same; label_cfg cfg }
*     { rank=same; label_ramcfg ram_cfg }
*     { rank=same; label_memory image ram0 ram1 ram2 }
*
*     cfg:c -> ram_cfg:c0;
*     ram_cfg:c0 -> ram0 [style="dashed"];
*     ram_cfg:c1 -> ram1;
*     ram_cfg:c2 -> ram2 [style="dashed"];
*     ram_cfg:c2 -> image [style="dashed"];
*   }
* @enddot
*/

/** This service initializes a RAM section.
 *
 * The function Mcu_InitRamSection initializes the RAM section. The definition of the section
 * and the initialization value is provided by the configuration structure.
 * The MCU modules environment shall call the function Mcu_InitRamSection() only after the
 * MCU module has been initialized using the function Mcu_Init().
 *
 * Example:\n
 * @code
 *     const Mcu_RamConfigType ram_cfg[] = {
 *       { A, S1, NULL_PTR },
 *       { B, S2, NULL_PTR },
 *       { C, S3, D },
 *     };
 *
 *     const Mcu_ConfigType cfg = {
 *       0, 0, 3, NULL_PTR, NULL_PTR, ram_cfg
 *     };
 *
 *     void f(void)
 *     {
 *       Mcu_Init(&cfg);
 *       ...
 *       Mcu_InitRamSection(1);
 *     }
 * @endcode
 *
 * In this example the second entry of the configuration would be used for the initialization of
 * the RAM located at address B.
 *
 * @image latex external_images/ram_init_example.pdf "Ram Init Example" width=14cm
 *
 * @param num_of_ram_section  Selects RAM memory section provided in configuration set
 * @return E_OK: command has been accepted \n
 *         E_NOT_OK: command has not been accepted e.g. due to a parameter error
 */
extern Std_ReturnType Mcu_InitRamSection(Mcu_RamSectionType num_of_ram_section);


/** This service initializes the PLL and other MCU specific clock options.
 *
 * The function Mcu_InitClock() initializes the PLL and other MCU specific clock options.
 * The clock configuration parameters are provided via the configuration structure.
 *
 * @warning In contrast to the AutoSAR specification this function waits for the
 *          PLL lock. This is done due to the TC1767 specific clock switching behavior.
 *          Since a change to the normal mode always requires a change to the
 *          prescaler mode a waiting at that point is already required. To relieve
 *          the user from a knowledge of the TC1767 specific clock switching behavior
 *          the complete logic is already included in the Mcu_InitClock() function.
 *
 * @param num_of_clock_set Selects a clock setting provided in the configuration set
 * @return #E_OK: command has been accepted \n
 *         #E_NOT_OK: command has not been accepted e.g. due to a parameter error
 */
extern Std_ReturnType Mcu_InitClock(Mcu_ClockType num_of_clock_set);


/** This service activates the PLL clock to the MCU clock distribution.
 *
 * The function Mcu_DistributePllClock() activates the distribution of the clock
 * to the peripherals by writing to the respective CLC and FDR registers.
 * The values that a written to the registers can be changed in the Mcu_Cfg.h header
 * file.
 */
extern void Mcu_DistributePllClock(void);


/** The function Mcu_GetPllStatus() returns the lock status of the PLL.
 *
 * The function Mcu_GetPllStatus() returns #MCU_PLL_STATUS_UNDEFINED if this function is called
 * prior to calling of the function Mcu_Init().
 *
 * @return #MCU_PLL_LOCKED: @copydoc #MCU_PLL_LOCKED\n
 *         #MCU_PLL_UNLOCKED: @copydoc #MCU_PLL_UNLOCKED\n
 *         #MCU_PLL_STATUS_UNDEFINED: @copydoc #MCU_PLL_STATUS_UNDEFINED
 */
extern Mcu_PllStatusType Mcu_GetPllStatus(void);


/** The service reads the reset type from the hardware.
 *
 * The function Mcu_GetResetReason() reads the reset reason from the hardware and return this reason.
 * The function Mcu_GetResetReason() returns #MCU_RESET_UNDEFINED if this function is called prior to
 * calling of the function Mcu_Init() since it is specified that way in the AutoSAR specification.
 *
 * @return #MCU_POWER_ON_RESET: @copydoc #MCU_POWER_ON_RESET\n
 *         #MCU_WATCHDOG_RESET: @copydoc #MCU_WATCHDOG_RESET\n
 *         #MCU_SW_RESET: @copydoc #MCU_SW_RESET\n
 *         #MCU_RESET_UNDEFINED: @copydoc #MCU_RESET_UNDEFINED
 */
extern Mcu_ResetType Mcu_GetResetReason(void);


/** The service reads the reset type from the hardware register.
 *
 * This function returns the current value of the RSTSTAT register. This function can
 * be called before the initialization of Mcu is performed (prior to Mcu_Init()).
 *
 * The function returns a bitmask with the following format. The reset source that caused
 * the most recent reset is indicated by a set bit.
 *
 * @image html  tc1767_RSTSTAT.png "RSTSTAT Register"
 * @image latex tc1767_RSTSTAT.png "RSTSTAT Register"
 *
 * bit 0: ESR0: Reset caused by reset request trigger 0\n
 * bit 1: ESR1: Reset caused by reset request trigger 1\n
 * bit 3: WDT:  Reset caused by watchdog timer\n
 * bit 4: SW:   Reset caused by software request\n
 * bit 16: PORST: Reset caused by power on reset\n
 * bit 17: OCDS: Reset caused by OCDS\n
 * bit 18: CB0:  Reset caused by Cerberus\n
 * bit 19: CB1:  Reset caused by Cerberus\n
 * bit 20: CB3:  Reset caused by Cerberus\n
 * bit 21: TP:   Reset caused by Flash tuning protection
 *
 * @warning since this function returns the value of a hardware register it is not recommended
 *          to use it in components that shall be portable.
 *
 * @return current value of RSTSTAT register
 */
extern Mcu_RawResetType Mcu_GetResetRawValue(void);


/** The service performs a microcontroller reset.
 *
 * The function performs a reset of the microcontroller. After the reset the function
 * Mcu_GetResetReason() will return #MCU_SW_RESET.
 *
 * This function will never return.
 */
extern void Mcu_PerformReset(void);


/** Set different MCU power modes configured in the configuration set.
 *
 * The function Mcu_SetMode() sets the MCU power mode. In case of the CPU switched off, the function
 * Mcu_SetMode() returns after it has performed a wakeup.
 * The MCU modules environment shall only call the function Mcu_SetMode() after the MCU module has
 * been initialized by the function Mcu_Init().
 *
 * @note The environment of the function Mcu_SetMode() has to ensure that the ECU is ready for
 *       reduced power mode activation.
 * @warning Not supported by this implementation.
 */
extern void Mcu_SetMode(Mcu_ModeType McuMode);


/** The function Mcu_GetVersionInfo returns the version information of the Mcu module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Mcu_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter McuVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function+
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Mcu_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = MCU_MODULE_ID;
	versioninfo->sw_major_version = MCU_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = MCU_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = MCU_SW_PATCH_VERSION;
	versioninfo->vendorID = MCU_VENDOR_ID;
}


#define MCU_STOP_SEC_CODE
#include "MemMap.h"


/*@}*/

#endif /* MCU_H_ */
