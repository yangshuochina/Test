/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Port.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:28:31MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Port.h  $
 *  Revision 1.4 2011/03/22 09:28:31MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.3 2010/11/22 08:33:43MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:48:46MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:19:24MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Port/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef PORT_H_
#define PORT_H_


#include "Std_Types.h"
#include "team-s.h"
#include "Port_Cfg.h"

/** @defgroup group_port Port
 *
 * @section port_intro Introduction
 * The port function implements the port functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_Port_Driver.pdf">AUTOSAR_SWS_PORT_Driver</a>.
 *
 * The Port module provides services for initializing the whole port structure of the microcontroller.
 * Pins and ports can be assigned to various functionalities:
 * <ul>
 * 	<li>General Purpose I/O</li>
 *  <li>General Purpose Timer Array (GPTA) </li>
 *  <li>Asynchronous/Synchronous Serial Interface (ASC)</li>
 *  <li>Synchronous Serial Interface (SSC)</li>
 *  <li>Micro Second Channel (MSC)</li>
 *  <li>Controller Area Network Controller (MultiCAN)</li>
 *  <li>Micro Link Interface (MultiCAN)</li>
 *  <li>Analog to Digital Converter (ADC)</li>
 * </ul>
 *
 *
 * @section port_autosar_conformance AutoSAR Conformance
 * The following services are not fully AutoSAR conform due to the microcontroller hardware implementation:
 * @li Port_SetPinDirection() - see \ref Port_SetPinDirectionRestriction "here".
 * @li Port_SetPinMode() - see \ref Port_SetPinModeRestriction "here".
 *
 *
 * @section port_configuration Port Module Configuration
 *
 * The port module requires a configuration before the module can be used. The configuration is described here
 * \ref port_config.
 *
 */
/*@{*/

#define PORT_MODULE_ID         (124)
#define PORT_VENDOR_ID         (TEAMS_VENDOR_ID)
#define PORT_AR_MAJOR_VERSION  (3)
#define PORT_AR_MINOR_VERSION  (0)
#define PORT_AR_PATCH_VERSION  (3)
#define PORT_SW_MAJOR_VERSION  (1)
#define PORT_SW_MINOR_VERSION  (0)
#define PORT_SW_PATCH_VERSION  (0)


/** @defgroup group_port_api_service_id Driver API service identifiers.
 * @ingroup group_port
 */
/*@{*/
#define PORT_INIT_API_ID                     	0 /**< The API identifier of service \ref Port_Init API. */
#define PORT_SET_PIN_DIRECTION_API_ID        	1 /**< The API identifier of service \ref Port_SetPinDirection API. */
#define PORT_SET_PIN_MODE_API_ID             	2 /**< The API identifier of service \ref Port_SetPinMode API. */
#define PORT_REFRESH_PORT_DIRECTION_API_ID    	3 /**< The API identifier of service \ref Port_RefreshPortDirection API. */
/*@}*/

/** @defgroup group_port_dev_erros Port Development Error Codes
 * @ingroup group_port
 */
/*@{*/
#define PORT_E_PARAM_PIN              ((uint8)0x0A) /**< port error code for invalid port pin ID requested. */
#define PORT_E_DIRECTION_UNCHANGEABLE ((uint8)0x0B) /**< port error code for port pin not configured as changeable. */
#define PORT_E_PARAM_CONFIG           ((uint8)0x0C) /**< port error code for API \ref Port_Init service called with wrong parameter. */
#define PORT_E_PARAM_INVALID_MODE     ((uint8)0x0D) /**< port error code for API \ref Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_MODE_UNCHANGEABLE      ((uint8)0x0E) /**< port error code for API \ref Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_UNINIT                 ((uint8)0x0F) /**< port error code for API service called without module initialization. */
/*@}*/

/** @defgroup group_port_pin_adr Port Pin Addressing
 *  @ingroup group_port
 *
 * The pin and port addressing is performed as described below.
 *
 *
 *  A Port is addressed using the port identifiers.
 *  @li \ref PORT_0
 *  @li \ref PORT_1
 *  @li \ref PORT_2
 *  @li \ref PORT_3
 *  @li \ref PORT_4
 *  @li \ref PORT_5
 *  @li \ref PORT_6
 *
 *  A pin is addressed using the pin identifiers.
 *  @li \ref PORT_PIN_0
 *  @li \ref PORT_PIN_1
 *  @li \ref PORT_PIN_2
 *  @li ...
 *  @li \ref PORT_PIN_15
 *
 *  A pin on a port is addressed using the port pin identifiers.
 *  @li \ref PORT_0_0
 *  @li \ref PORT_0_1
 *  @li ...
 *  @li \ref PORT_6_3
 *
 *  Alternatively a port pin can be addressed using the macro \ref PORT_PIN.
 *  @code
 *  PORT_PIN(PORT_0, PORT_PIN_5);
 *  @endcode
 *
 *
 */
/*@{*/
/* Port definitions */
#define PORT_0  ((Port_IdType) 0)    /**< The definition to be used for addressing port 0. */
#define PORT_1  ((Port_IdType) 1)    /**< The definition to be used for addressing port 1. */
#define PORT_2  ((Port_IdType) 2)    /**< The definition to be used for addressing port 2. */
#define PORT_3  ((Port_IdType) 3)    /**< The definition to be used for addressing port 3. */
#define PORT_4  ((Port_IdType) 4)    /**< The definition to be used for addressing port 4. */
#define PORT_5  ((Port_IdType) 5)    /**< The definition to be used for addressing port 5. */
#define PORT_6  ((Port_IdType) 6)    /**< The definition to be used for addressing port 6. */

/* Pin definitions */
#define PORT_PIN_0   ((Port_PinIdType) 0)   /**< The definition to be used for addressing pin 0 on a port. */
#define PORT_PIN_1   ((Port_PinIdType) 1)   /**< The definition to be used for addressing pin 1 on a port. */
#define PORT_PIN_2   ((Port_PinIdType) 2)   /**< The definition to be used for addressing pin 2 on a port. */
#define PORT_PIN_3   ((Port_PinIdType) 3)   /**< The definition to be used for addressing pin 3 on a port. */
#define PORT_PIN_4   ((Port_PinIdType) 4)   /**< The definition to be used for addressing pin 4 on a port. */
#define PORT_PIN_5   ((Port_PinIdType) 5)   /**< The definition to be used for addressing pin 5 on a port. */
#define PORT_PIN_6   ((Port_PinIdType) 6)   /**< The definition to be used for addressing pin 6 on a port. */
#define PORT_PIN_7   ((Port_PinIdType) 7)   /**< The definition to be used for addressing pin 7 on a port. */
#define PORT_PIN_8   ((Port_PinIdType) 8)   /**< The definition to be used for addressing pin 8 on a port. */
#define PORT_PIN_9   ((Port_PinIdType) 9)   /**< The definition to be used for addressing pin 9 on a port. */
#define PORT_PIN_10  ((Port_PinIdType) 10)   /**< The definition to be used for addressing pin 10 on a port. */
#define PORT_PIN_11  ((Port_PinIdType) 11)   /**< The definition to be used for addressing pin 11 on a port. */
#define PORT_PIN_12  ((Port_PinIdType) 12)   /**< The definition to be used for addressing pin 12 on a port. */
#define PORT_PIN_13  ((Port_PinIdType) 13)   /**< The definition to be used for addressing pin 13 on a port. */
#define PORT_PIN_14  ((Port_PinIdType) 14)   /**< The definition to be used for addressing pin 14 on a port. */
#define PORT_PIN_15  ((Port_PinIdType) 15)   /**< The definition to be used for addressing pin 15 on a port. */

/* configuration of symbolic names for port pin identifiers */
#define PORT_0_0  PORT_PIN(PORT_0, PORT_PIN_0)  /**< port pin identifier to access port 0 pin 0.*/
#define PORT_0_1  PORT_PIN(PORT_0, PORT_PIN_1)  /**< port pin identifier to access port 0 pin 1.*/
#define PORT_0_2  PORT_PIN(PORT_0, PORT_PIN_2)  /**< port pin identifier to access port 0 pin 2.*/
#define PORT_0_3  PORT_PIN(PORT_0, PORT_PIN_3)  /**< port pin identifier to access port 0 pin 3.*/
#define PORT_0_4  PORT_PIN(PORT_0, PORT_PIN_4)  /**< port pin identifier to access port 0 pin 4.*/
#define PORT_0_5  PORT_PIN(PORT_0, PORT_PIN_5)  /**< port pin identifier to access port 0 pin 5.*/
#define PORT_0_6  PORT_PIN(PORT_0, PORT_PIN_6)  /**< port pin identifier to access port 0 pin 6.*/
#define PORT_0_7  PORT_PIN(PORT_0, PORT_PIN_7)  /**< port pin identifier to access port 0 pin 7.*/
#define PORT_0_8  PORT_PIN(PORT_0, PORT_PIN_8)  /**< port pin identifier to access port 0 pin 8.*/
#define PORT_0_9  PORT_PIN(PORT_0, PORT_PIN_9)  /**< port pin identifier to access port 0 pin 9.*/
#define PORT_0_10 PORT_PIN(PORT_0, PORT_PIN_10) /**< port pin identifier to access port 0 pin 10.*/
#define PORT_0_11 PORT_PIN(PORT_0, PORT_PIN_11) /**< port pin identifier to access port 0 pin 11.*/
#define PORT_0_12 PORT_PIN(PORT_0, PORT_PIN_12) /**< port pin identifier to access port 0 pin 12.*/
#define PORT_0_13 PORT_PIN(PORT_0, PORT_PIN_13) /**< port pin identifier to access port 0 pin 13.*/
#define PORT_0_14 PORT_PIN(PORT_0, PORT_PIN_14) /**< port pin identifier to access port 0 pin 14.*/
#define PORT_0_15 PORT_PIN(PORT_0, PORT_PIN_15) /**< port pin identifier to access port 0 pin 15.*/

#define PORT_1_0  PORT_PIN(PORT_1, PORT_PIN_0)  /**< port pin identifier to access port 1 pin 0.*/
#define PORT_1_1  PORT_PIN(PORT_1, PORT_PIN_1)  /**< port pin identifier to access port 1 pin 1.*/
#define PORT_1_2  PORT_PIN(PORT_1, PORT_PIN_2)  /**< port pin identifier to access port 1 pin 2.*/
#define PORT_1_3  PORT_PIN(PORT_1, PORT_PIN_3)  /**< port pin identifier to access port 1 pin 3.*/
#define PORT_1_4  PORT_PIN(PORT_1, PORT_PIN_4)  /**< port pin identifier to access port 1 pin 4.*/
#define PORT_1_5  PORT_PIN(PORT_1, PORT_PIN_5)  /**< port pin identifier to access port 1 pin 5.*/
#define PORT_1_6  PORT_PIN(PORT_1, PORT_PIN_6)  /**< port pin identifier to access port 1 pin 6.*/
#define PORT_1_7  PORT_PIN(PORT_1, PORT_PIN_7)  /**< port pin identifier to access port 1 pin 7.*/
#define PORT_1_8  PORT_PIN(PORT_1, PORT_PIN_8)  /**< port pin identifier to access port 1 pin 8.*/
#define PORT_1_9  PORT_PIN(PORT_1, PORT_PIN_9)  /**< port pin identifier to access port 1 pin 9.*/
#define PORT_1_10 PORT_PIN(PORT_1, PORT_PIN_10) /**< port pin identifier to access port 1 pin 10.*/
#define PORT_1_11 PORT_PIN(PORT_1, PORT_PIN_11) /**< port pin identifier to access port 1 pin 11.*/
#define PORT_1_12 PORT_PIN(PORT_1, PORT_PIN_12) /**< port pin identifier to access port 1 pin 12.*/
#define PORT_1_13 PORT_PIN(PORT_1, PORT_PIN_13) /**< port pin identifier to access port 1 pin 13.*/
#define PORT_1_14 PORT_PIN(PORT_1, PORT_PIN_14) /**< port pin identifier to access port 1 pin 14.*/
#define PORT_1_15 PORT_PIN(PORT_1, PORT_PIN_15) /**< port pin identifier to access port 1 pin 15.*/

#define PORT_2_0  PORT_PIN(PORT_2, PORT_PIN_0)  /**< port pin identifier to access port 2 pin 0.*/
#define PORT_2_1  PORT_PIN(PORT_2, PORT_PIN_1)  /**< port pin identifier to access port 2 pin 1.*/
#define PORT_2_2  PORT_PIN(PORT_2, PORT_PIN_2)  /**< port pin identifier to access port 2 pin 2.*/
#define PORT_2_3  PORT_PIN(PORT_2, PORT_PIN_3)  /**< port pin identifier to access port 2 pin 3.*/
#define PORT_2_4  PORT_PIN(PORT_2, PORT_PIN_4)  /**< port pin identifier to access port 2 pin 4.*/
#define PORT_2_5  PORT_PIN(PORT_2, PORT_PIN_5)  /**< port pin identifier to access port 2 pin 5.*/
#define PORT_2_6  PORT_PIN(PORT_2, PORT_PIN_6)  /**< port pin identifier to access port 2 pin 6.*/
#define PORT_2_7  PORT_PIN(PORT_2, PORT_PIN_7)  /**< port pin identifier to access port 2 pin 7.*/
#define PORT_2_8  PORT_PIN(PORT_2, PORT_PIN_8)  /**< port pin identifier to access port 2 pin 8.*/
#define PORT_2_9  PORT_PIN(PORT_2, PORT_PIN_9)  /**< port pin identifier to access port 2 pin 9.*/
#define PORT_2_10 PORT_PIN(PORT_2, PORT_PIN_10) /**< port pin identifier to access port 2 pin 10.*/
#define PORT_2_11 PORT_PIN(PORT_2, PORT_PIN_11) /**< port pin identifier to access port 2 pin 11.*/
#define PORT_2_12 PORT_PIN(PORT_2, PORT_PIN_12) /**< port pin identifier to access port 2 pin 12.*/
#define PORT_2_13 PORT_PIN(PORT_2, PORT_PIN_13) /**< port pin identifier to access port 2 pin 13.*/

#define PORT_3_0  PORT_PIN(PORT_3, PORT_PIN_0)  /**< port pin identifier to access port 3 pin 0.*/
#define PORT_3_1  PORT_PIN(PORT_3, PORT_PIN_1)  /**< port pin identifier to access port 3 pin 1.*/
#define PORT_3_2  PORT_PIN(PORT_3, PORT_PIN_2)  /**< port pin identifier to access port 3 pin 2.*/
#define PORT_3_3  PORT_PIN(PORT_3, PORT_PIN_3)  /**< port pin identifier to access port 3 pin 3.*/
#define PORT_3_4  PORT_PIN(PORT_3, PORT_PIN_4)  /**< port pin identifier to access port 3 pin 4.*/
#define PORT_3_5  PORT_PIN(PORT_3, PORT_PIN_5)  /**< port pin identifier to access port 3 pin 5.*/
#define PORT_3_6  PORT_PIN(PORT_3, PORT_PIN_6)  /**< port pin identifier to access port 3 pin 6.*/
#define PORT_3_7  PORT_PIN(PORT_3, PORT_PIN_7)  /**< port pin identifier to access port 3 pin 7.*/
#define PORT_3_8  PORT_PIN(PORT_3, PORT_PIN_8)  /**< port pin identifier to access port 3 pin 8.*/
#define PORT_3_9  PORT_PIN(PORT_3, PORT_PIN_9)  /**< port pin identifier to access port 3 pin 9.*/
#define PORT_3_10 PORT_PIN(PORT_3, PORT_PIN_10) /**< port pin identifier to access port 3 pin 10.*/
#define PORT_3_11 PORT_PIN(PORT_3, PORT_PIN_11) /**< port pin identifier to access port 3 pin 11.*/
#define PORT_3_12 PORT_PIN(PORT_3, PORT_PIN_12) /**< port pin identifier to access port 3 pin 12.*/
#define PORT_3_13 PORT_PIN(PORT_3, PORT_PIN_13) /**< port pin identifier to access port 3 pin 13.*/
#define PORT_3_14 PORT_PIN(PORT_3, PORT_PIN_14) /**< port pin identifier to access port 3 pin 14.*/
#define PORT_3_15 PORT_PIN(PORT_3, PORT_PIN_15) /**< port pin identifier to access port 3 pin 15.*/

#define PORT_4_0  PORT_PIN(PORT_4, PORT_PIN_0)  /**< port pin identifier to access port 4 pin 0.*/
#define PORT_4_1  PORT_PIN(PORT_4, PORT_PIN_1)  /**< port pin identifier to access port 4 pin 1.*/
#define PORT_4_2  PORT_PIN(PORT_4, PORT_PIN_2)  /**< port pin identifier to access port 4 pin 2.*/
#define PORT_4_3  PORT_PIN(PORT_4, PORT_PIN_3)  /**< port pin identifier to access port 4 pin 3.*/

#define PORT_5_0  PORT_PIN(PORT_5, PORT_PIN_0)  /**< port pin identifier to access port 5 pin 0.*/
#define PORT_5_1  PORT_PIN(PORT_5, PORT_PIN_1)  /**< port pin identifier to access port 5 pin 1.*/
#define PORT_5_2  PORT_PIN(PORT_5, PORT_PIN_2)  /**< port pin identifier to access port 5 pin 2.*/
#define PORT_5_3  PORT_PIN(PORT_5, PORT_PIN_3)  /**< port pin identifier to access port 5 pin 3.*/
#define PORT_5_4  PORT_PIN(PORT_5, PORT_PIN_4)  /**< port pin identifier to access port 5 pin 4.*/
#define PORT_5_5  PORT_PIN(PORT_5, PORT_PIN_5)  /**< port pin identifier to access port 5 pin 5.*/
#define PORT_5_6  PORT_PIN(PORT_5, PORT_PIN_6)  /**< port pin identifier to access port 5 pin 6.*/
#define PORT_5_7  PORT_PIN(PORT_5, PORT_PIN_7)  /**< port pin identifier to access port 5 pin 7.*/
#define PORT_5_8  PORT_PIN(PORT_5, PORT_PIN_8)  /**< port pin identifier to access port 5 pin 8.*/
#define PORT_5_9  PORT_PIN(PORT_5, PORT_PIN_9)  /**< port pin identifier to access port 5 pin 9.*/
#define PORT_5_10 PORT_PIN(PORT_5, PORT_PIN_10) /**< port pin identifier to access port 5 pin 10.*/
#define PORT_5_11 PORT_PIN(PORT_5, PORT_PIN_11) /**< port pin identifier to access port 5 pin 11.*/
#define PORT_5_12 PORT_PIN(PORT_5, PORT_PIN_12) /**< port pin identifier to access port 5 pin 12.*/
#define PORT_5_13 PORT_PIN(PORT_5, PORT_PIN_13) /**< port pin identifier to access port 5 pin 13.*/
#define PORT_5_14 PORT_PIN(PORT_5, PORT_PIN_14) /**< port pin identifier to access port 5 pin 14.*/
#define PORT_5_15 PORT_PIN(PORT_5, PORT_PIN_15) /**< port pin identifier to access port 5 pin 15.*/

#define PORT_6_0  PORT_PIN(PORT_6, PORT_PIN_0)  /**< port pin identifier to access port 6 pin 0.*/
#define PORT_6_1  PORT_PIN(PORT_6, PORT_PIN_1)  /**< port pin identifier to access port 6 pin 1.*/
#define PORT_6_2  PORT_PIN(PORT_6, PORT_PIN_2)  /**< port pin identifier to access port 6 pin 2.*/
#define PORT_6_3  PORT_PIN(PORT_6, PORT_PIN_3)  /**< port pin identifier to access port 6 pin 3.*/

#define PORT_PIN(port, pin)    ((Port_PinType)((port) | ((Port_PinType)(pin)*256)))
/*@}*/

/**
 * Enumeration defining the possible output states a port pin.
 */
typedef enum {
	PORT_PIN_LEVEL_LOW  = 0,       /**< The port pin output level is low.  */
	PORT_PIN_LEVEL_HIGH = 1,       /**< The port pin output level is high. */
}Port_PinOutputState;

/** Data type for the symbolic name of a port pin.
 *
 * The port and pin is encoded in the 16 bit type.
 * @li The upper 8 bit contain the pin identifier, see \ref port_pin_adr.
 * @li The lower 8 bit contain the port identifier. see \ref port_pin_adr.
 *
 *
 * @image latex external_images/port_pintype.pdf "Port PinType"
 */

/* @dot
 *   digraph Port_PinType {
 *     node [shape=plaintext fontsize="10.0"];
 *     Port_ConfigType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="type" BGCOLOR="grey" colspan="16">Port_PinType</td></tr>
 *                         <tr><td PORT="port" colspan="8">pin identifier</td>
 *                             <td PORT="pin" colspan="8">port identifier</td>
 *                         </tr>
 *                         <tr><td PORT="MSB" >MSB</td>
 *                             <td PORT="p14" >14</td>
 *                             <td PORT="p13" >13</td>
 *                             <td PORT="p12" >12</td>
 *                             <td PORT="p11" >11</td>
 *                             <td PORT="p10" >10</td>
 *                             <td PORT="p9" >9</td>
 *                             <td PORT="p8" >8</td>
 *                             <td PORT="p7" >7</td>
 *                             <td PORT="p6" >6</td>
 *                             <td PORT="p5" >5</td>
 *                             <td PORT="p4" >4</td>
 *                             <td PORT="p3" >3</td>
 *                             <td PORT="p2" >2</td>
 *                             <td PORT="p1" >1</td>
 *                             <td PORT="LSB" >LSB</td>
 *                          </tr>
 *       </table>>];
 *
 *   }
 * @enddot
 */
typedef uint16 Port_PinType;

/** Type definition for a port identifier.
 *
 * This type definition is used to define a port identifier.
 */
typedef uint8 Port_IdType;

/** Type definition for a pin identifier.
 *
 *  The type definition is used to define a pin identifier.
 */
typedef uint8 Port_PinIdType;

/** Enumeration defining the possible directions of a port pin.
 *
 * The enumeration is defining the possible directions of a port pin.
 */
typedef enum {
	PORT_PIN_IN,     /**< Sets the port pin as input. */
	PORT_PIN_OUT     /**< Sets the port pin as output. */
}Port_PinDirectionType;

/** Type definition for port pin modes.
 *
 * The type definition is used to define a port pin mode.
 */
typedef uint8 Port_PinModeType;


/** @defgroup group_port_config Port Configuration
 *  @ingroup group_port
 *
 * This module contains the configuration of the Port module.
 *
 * The Port module contains two configuration parts.
 * @li \ref port_pct_config
 * @li \ref port_pbt_config
 */
/*@{*/
/*@}*/


/** @defgroup group_port_pbt_config Port Post Build Time Configuration
 *  @ingroup group_port_config
 *
 * @section port_pbt_intro Introduction
 *
 *  The post build time configuration \ref Port_ConfigType is passed to the port module during runtime.
 *
 *  The configuration consists of the following parts:
 *  @li The pin configuration \ref Port_ConfigPinType.
 *  @li The port configuration \ref Port_ConfigPortType.
 *  @li The GPTA0 multiplexer configuration \ref Port_ConfigGptaMultiplexerArray.
 *  @li the LTCA2 multiplexer configuration \ref Port_ConfigLtcaMultiplexerArray.
 *
 * @section port_pbt_description Configuration Description
 *
 * The port module post build configuration is performed in the configuration file Port_PBCfg.c.
 *
 * The port module post build configuration is performed in the following steps.
 * <ol>
 *  <li>The port pin configuration is performed, see \ref port_pin_configuration_example.
 *  For each port pin that shall be configured an entry is made.
 *  The structure \ref Port_ConfigPinType is used for configuration.
 *  </li>
 *  <li>The port configuration is performed, see \ref port_port_configuration_example.
 *  The structure \ref Port_ConfigPortType is used for configuration.
 *  </li>
 *  <li> The GPTA0 multiplexer configuration is performed.
 *  The structure \ref Port_ConfigGptaMultiplexerArray is used for configuration.
 *  </li>
 *  <li> The LTCA2 multiplexer configuration is performed.
 *  The structure \ref Port_ConfigLtcaMultiplexerArray is used for configuration.
 *  </li>
 *  <li>The port module configuration is performed, see \ref port_configuration_example. The structure \ref Port_ConfigType is used for configuration.
 *  @li The number of pins in the pin configuration is inserted.
 *  @li The number of ports in the port configuration is inserted.
 *  @li The reference for the pin configuration is inserted.
 *  @li The reference to the port configuration is inserted.
 *  @li The reference to the GPTA0 multiplexer array configuration is inserted.
 *  @li The reference to the LTCA2 multiplexer array configuration is inserted.
 *  </li>
 *
 *  <li> The service \ref Port_Init is called and the configuration is passed as parameter.
 * </ol>
 *
 * The following figure shows the hierarchy of the configuration.
 *
 * @image latex external_images/port_configuration.pdf "Port Configuration" height=22cm
 *
 *
 * @section configuration_example Configuration Example
 *
 * The following example code shows a possible configuration.
 *
 *
 * @subsection port_pin_configuration_example Port Pin Configuration Example
 *
 * The example shows the configuration of port pins.
 * The first entry configures Pin5 on Port 0
 * @li Pin 5 on Port 0,
 * @li with an initial pin level low (ignored if input),
 * @li as input with
 * @li internal pull up,
 * @li the direction can be modified at runtime,
 * @li the mode can be modified at runtime.
 *
 * @code
 * const Port_ConfigPinType port_config_pin[] = {
 *		{
 *			    PORT_0_5, PORT_PIN_LEVEL_LOW, PCX_INPUT_PULL_UP_DEVICE_CONNECTED, TRUE, TRUE
 *		},
 *		{
 *			    PORT_1_0, PORT_PIN_LEVEL_LOW, PCX_INPUT_PULL_UP_DEVICE_CONNECTED, TRUE, TRUE
 *		},
 *		{
 *		        ->next port pin configuration
 *		},
 *		...
 * };
 * @endcode
 *
 * @subsection port_port_configuration_example Port Configuration Example
 * The example shows the configuration of Port 0.
 * The pad driver mode of port 0 is setup. The pad driver for
 * @li PD0 Class A1 pins P0.[7:0] is set to class A1 medium driver
 * @li PD1 Class A1 pins P0.[15:8] is set to class A1 medium driver
 * @li All other PDx pads (here PD2-PD7) are reserved at this port. Thus \ref PORT_PDR_RESERVED must be configured.
 * Port 1 to 6 are not shown in the example.
 *
 * @code
 * const Port_ConfigPortType port_config_port[] = {
 *		{
 *				PORT_0, {
 *						PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
 *						PORT_PDR_CLASS_A1_MEDIUM_DRIVER,
 *						PORT_PDR_RESERVED,
 *						PORT_PDR_RESERVED,
 *						PORT_PDR_RESERVED,
 *						PORT_PDR_RESERVED,
 *						PORT_PDR_RESERVED,
 *						PORT_PDR_RESERVED,
 *				}
 *		},
 *		configure other ports here
 *		...
 * };
 * @endcode
 *
 * @subsection port_gpta0_configuration_example Port GPTA0 Multiplexer Configuration Example
 *
 * This configuration sets up the GPTA0 multiplexer. For an understanding the chapter
 * <b>20.3.4 Input/Output Line Sharing Block (IOLS)</b> has to be read in the micro controller manual
 * <a href="../external/infineon/TC1767_UM_V1.1.pdf">TC1767 32-Bit Single-Chip Microcontroller Users Manual</a>.
 *
 * Assume the following example:
 * The LTC12 on GPTA0 shall be connected to Port 5.1 as output. Thus the output multiplexer needs to be configured.
 * @li Port 5.1 is connected to OUT41.
 * @li OUT41 is configured at OMCRL5 (OMCRL5 configures OUT[43:40]).
 * @li OUT41 is the field 1 in the OMCRL5 register.
 * @li LTC12 is in the multiplexer group 1 LTCG1, the selection is \ref OMG1 or \ref OMG_LTC0_31.
 * @li LTC12 is the 4th line in the multiplexer group 1 LTCG1 LTC[15:08], the selection is \ref OMG_IN4.
 * @li The complete configuration of the field 1 in the OMCRL5 register is OMCRX(1, OMG_LTC0_31, OMG_IN4).
 *
 * The LTC12 needs to be feed with a clock from clock bus 2. Thus the LTC input multiplexer needs to be configured.
 * @li LTC12 is configured in the LIMCRH1 register (LIMCRH1 configures LTC group LTC[15:12])
 * @li LTC12 is the field 4 in the LIMCRH1 register.
 * @li CLK2 is in the multiplexer group 3 CLK[7:0], the selection is \ref LIMG3 or \ref LIMG_CLK0_7.
 * @li CLK2 is the 2nd line the in the multiplexer group 3 CLK[7:0], the selection is LIMG_IN2.
 * @li The multiplexer is enabled by setting a 1 (LIMEN field).
 * @li The complete configuration of the field 0 in the LIMCRH1 register is LIMCRX(4, 1, LIMG_CLK0_7, LIMG_IN2).
 *
 * @code
 * const Port_ConfigGptaMultiplexerArray port_config_gpta0_multiplexer_array = {
 *      On-Chip Trigger and Gating Signal Multiplexer Control Registers
 *
 *      To configure a trigger and gating signal multiplexer control register, the macro OTMCRX is used.
 *
 *      The first parameter selects the field in the register.
 *      - 0..7 - OTM0- OTM7
 *
 *      The second parameter selects the multiplexer line selection.
 *      The following selections can be made:
 *      - OTMG_IN0
 *      - OTMG_IN1
 *      - OTMG_IN2
 *      - OTMG_IN3
 *      - OTMG_IN4
 *      - OTMG_IN5
 *      - OTMG_IN6
 *      - OTMG_IN7
 *		{
 *				OTMCRX(7, OTMG_IN0) |
 *				OTMCRX(6, OTMG_IN0) |
 *				OTMCRX(5, OTMG_IN0) |
 *				OTMCRX(4, OTMG_IN0) |
 *				OTMCRX(3, OTMG_IN0) |
 *				OTMCRX(2, OTMG_IN0) |
 *				OTMCRX(1, OTMG_IN0) |
 *				OTMCRX(0, OTMG_IN0) ,
 *				OTMCRX(7, OTMG_IN0) |
 *				OTMCRX(6, OTMG_IN0) |
 *				OTMCRX(5, OTMG_IN0) |
 *				OTMCRX(4, OTMG_IN0) |
 *				OTMCRX(3, OTMG_IN0) |
 *				OTMCRX(2, OTMG_IN0) |
 *				OTMCRX(1, OTMG_IN0) |
 *				OTMCRX(0, OTMG_IN0) ,
 *		},
 *      Output Multiplexer Control Registers
 *
 *      To configure an output multiplexer control register, the macro OMCRX is used.
 *
 *      The first parameter selects the field in the register.
 *      - 0..3 - field OML0/OMG0..OML3/OMG3 for a OMCRL register
 *      - 4..7 - field OML4/OMG4..OML7/OMG7 for a OMCRH register
 *
 *      The second parameter selects the multiplexer group OMG<field>.
 *      The following selections can be choosen:
 *      - OMG0, OMG_GTC0_31
 *      - OMG1, OMG_LTC0_31
 *      - OMG2, OMG_LTC32_63
 *
 *      The third parameter selects the multiplexer line selection OML<field>.
 *      The following selections can be choosen:
 *      - OMG_IN0
 *      - OMG_IN1
 *      - OMG_IN2
 *      - OMG_IN3
 *      - OMG_IN4
 *      - OMG_IN5
 *      - OMG_IN6
 *      - OMG_IN7
 *		{
 *		 ...
 *				OMCRL5 IN[43:40]/OUT[43:40] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]
 *				OMCRX(3, OMG0, OMG_IN0) | OMCRX(2, OMG0, OMG_IN0) | OMCRX(1, OMG_LTC0_31, OMG_IN4) | OMCRX(0, OMG0, OMG_IN0),
 *		 ...
 *		},

 *		LTC Input Multiplexer Control Registers
 *
 *      The first parameter selects the field in the register.
 *      - 0..3 - field LIML0/LIMG0/LIMEN0..LIML3/LIMG3/LIMEN3 for a LIMCRL register
 *      - 4..7 - field LIML4/LIMG4/LIMEN4..LIML7/LIMG7/LIMEN7 for a LIMCRH register
 *
 *      The second parameter selects the enable multiplexer connection field LIMEN<field>.
 *      The following selections can be choosen:
 *      - 0 disable
 *      - 1 enable

 *      The third parameter selects the multiplexer group LIMG<field>.
 *      The following selections can be choosen:
 *      - LIMG0, LIMG_IOG0_3
 *      - LIMG1, LIMG_IOG4_6
 *      - LIMG2, LIMG_GTC0_31
 *      - LIMG3, LIMG_CLK0_7
 *      - LIMG4, LIMG_PDL_INT0_3
 *
 *      The fourth parameter selects the multiplexer line selection LIML<field>.
 *      The following selections can be choosen:
 *      - LIMG_IN0
 *      - LIMG_IN1
 *      - LIMG_IN2
 *      - LIMG_IN3
 *      - LIMG_IN4
 *      - LIMG_IN5
 *      - LIMG_IN6
 *      - LIMG_IN7
 *
 *		{
 *		 ...
 *				LIMCRH1 LTC[15:12] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0]
 *				LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 1, LIMG_CLK0_7, LIMG_IN2),
 *		 ...
 *		},
 *		GTC Input Multiplexer Control Registers
 *
 *      The first parameter selects the field in the register.
 *      - 0..3 - field GIML0/GIMG0/GIMEN0..GIML3/GIMG3/GIMEN3 for a GIMCRL register
 *      - 4..7 - field GIML4/GIMG4/GIMEN4..GIML7/GIMG7/GIMEN7 for a GIMCRH register
 *
 *      The second parameter selects the enable multiplexer connection field GIMEN<field>.
 *      The following selections can be choosen:
 *      - 0 disable
 *      - 1 enable

 *      The third parameter selects the multiplexer group GIMG<field>.
 *      The following selections can be choosen:
 *      - GIMG0, GIMG_IOG0_3
 *      - GIMG1, GIMG_IOG4_6
 *      - GIMG2, GIMG_LTC0_31
 *      - GIMG3, GIMG_LTC32_63
 *      - GIMG4, GIMG_FPC0_5_INT0_1
 *
 *      The fourth parameter selects the multiplexer line selection GIML<field>.
 *      The following selections can be choosen:
 *      - GIMG_IN0
 *      - GIMG_IN1
 *      - GIMG_IN2
 *      - GIMG_IN3
 *      - GIMG_IN4
 *      - GIMG_IN5
 *      - GIMG_IN6
 *      - GIMG_IN7
 *
 *		{
 *				GIMCRH3 GTC[31:28] selectable group IOG3, LTCG3=LTC[31:24], LTCG7=LTC[63:56], FPC[5:0]/INT[1:0]
 *				GIMCRX(7, 0, GIMG0, GIMG_IN0) | GIMCRX(6, 0, GIMG0, GIMG_IN0) | GIMCRX(5, 0, GIMG0, GIMG_IN0) | GIMCRX(4, 0, GIMG0, GIMG_IN0),
 *		...
 *		},
 *	};
 * @endcode
 *
 * @subsection port_ltca2_configuration_example Port LTCA2 Multiplexer Configuration Example
 *
 * This configuration sets up the LTCA2 multiplexer. For an understanding the chapter
 * <b>20.5.2 Input/Output Line Sharing Block (IOLS)</b> has to be read in the micro controller manual
 * <a href="../external/infineon/TC1767_UM_V1.1.pdf">TC1767 32-Bit Single-Chip Microcontroller Users Manual</a>.
 *
 * @code
 * const Port_ConfigLtcaMultiplexerArray port_config_ltca2_multiplexer_array = {
 *
 *      Output Multiplexer Control Registers.
 *
 *      To configure an output multiplexer control register, the macro OMCRX is used.
 *
 *      The first parameter selects the field in the register.
 *      - 0..3 - field OML0/OMG0..OML3/OMG3 for a OMCRL register
 *      - 4..7 - field OML4/OMG4..OML7/OMG7 for a OMCRH register
 *
 *      The second parameter selects the multiplexer group OMG<field>.
 *      The following selections can be choosen:
 *      - OMG1, OMG_LTC0_31
 *
 *      The third parameter selects the multiplexer line selection OML<field>.
 *      The following selections can be choosen:
 *      - OMG_IN0
 *      - OMG_IN1
 *      - OMG_IN2
 *      - OMG_IN3
 *      - OMG_IN4
 *      - OMG_IN5
 *      - OMG_IN6
 *      - OMG_IN7		{
 *				OMCRH13 OUT[111:108] selectable group LTCG1=LTC[15:8]
 * 				OMCRX(7, OMG1, OMG_IN0) | OMCRX(6, OMG1, OMG_IN0) | OMCRX(5, OMG1, OMG_IN0) | OMCRX(4, OMG1, OMG_IN0),
 * 		...
 * 		},
 *
 * 		LTC Input Multiplexer Control Registers
 *
 *      The first parameter selects the field in the register.
 *      - 0..3 - field LIML0/LIMG0/LIMEN0..LIML3/LIMG3/LIMEN3 for a LIMCRL register
 *      - 4..7 - field LIML4/LIMG4/LIMEN4..LIML7/LIMG7/LIMEN7 for a LIMCRH register
 *
 *      The second parameter selects the enable multiplexer connection field LIMEN<field>.
 *      The following selections can be choosen:
 *      - 0 disable
 *      - 1 enable

 *      The third parameter selects the multiplexer group LIMG<field>.
 *      The following selections can be choosen:
 *      - LIMG0, LIMG_IOG0_3
 *      - LIMG3, LIMG_CLK0_7
 *      - LIMG4, LIMG_PDL_INT0_3
 *
 *      The fourth parameter selects the multiplexer line selection LIML<field>.
 *      The following selections can be choosen:
 *      - LIMG_IN0
 *      - LIMG_IN1
 *      - LIMG_IN2
 *      - LIMG_IN3
 *      - LIMG_IN4
 *      - LIMG_IN5
 *      - LIMG_IN6
 *      - LIMG_IN7
 * 		{
 * 				LIMCRH3 LTC[31:28] selectable group IOG3, CLOCK[7:0], PDL[3:0]/INT[3:0]
 *				LIMCRX(7, 0, LIMG0, LIMG_IN0) | LIMCRX(6, 0, LIMG0, LIMG_IN0) | LIMCRX(5, 0, LIMG0, LIMG_IN0) | LIMCRX(4, 0, LIMG0, LIMG_IN0),
 *
 * 		...
 * 		}
 * 	};
 * @endcode
 *
 *
 * @subsection port_configuration_example Port Module Configuration Example
 * @code
 *
 * const Port_ConfigType port_config = {
 *		sizeof(port_config_pin) / sizeof(Port_ConfigPinType),
		sizeof(port_config_port) / sizeof(Port_ConfigPortType),
		port_config_pin,
		port_config_port,
		&port_config_gpta0_multiplexer_array,
		&port_config_ltca2_multiplexer_array,
 * };
 * @endcode
 *
 * @subsection port_configuraion_file_example Complete Configuration Example
 *
 * A complete configuration is as follows. Note that not all possible pins are configured.
 * \include Port_PBcfg.c
 *
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Port_Init(&port_config);
 *   ...
 * }
 * @endcode
 *
 *
 *
 *
 */
/*
 * @dot
 *   digraph Port_Configuration {
 *     node [shape=plaintext fontsize="6.0"];
 *     rankdir=LR;
 *     Port_ConfigType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="type" BGCOLOR="grey">Port_ConfigType</td></tr>
 *                         <tr><td PORT="numOfPins">uint16 numOfPins</td></tr>
 *                         <tr><td PORT="numOfPorts">uint8 numOfPorts</td></tr>
 *                         <tr><td PORT="pinCfg">const Port_ConfigPinType *pinCfg</td></tr>
 *                         <tr><td PORT="portCfg">const Port_ConfigPortType *portCfg</td></tr>
 *                         <tr><td PORT="gptA0MulCfg">const Port_ConfigGptaMultiplexerArray *portGpta0MultiplexerArrayCfg</td></tr>
 *                         <tr><td PORT="ltcA2MulCfg">const Port_ConfigLtcaMultiplexerArray *portLtca2MultiplexerArrayCfg</td></tr>
 *       </table>>];
 *
 *     Port_ConfigPortType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="grey">Port_ConfigPortType</td></tr>
 *                         <tr><td PORT="port">Port_IdType port</td></tr>
 *                         <tr><td PORT="pdr">Port_ConfigPDRPadType pdr</td></tr>
 *       </table>>];
 *
 *     Port_ConfigPDRPadType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="grey">Port_Config_Pn_PDRPadType</td></tr>
 *                         <tr><td PORT="pd0">uint32 pd0:4</td></tr>
 *                         <tr><td PORT="pd1">uint32 pd1:4</td></tr>
 *                         <tr><td PORT="pd2">uint32 pd2:4</td></tr>
 *                         <tr><td PORT="pd3">uint32 pd3:4</td></tr>
 *                         <tr><td PORT="pd4">uint32 pd4:4</td></tr>
 *                         <tr><td PORT="pd5">uint32 pd5:4</td></tr>
 *                         <tr><td PORT="pd6">uint32 pd6:4</td></tr>
 *                         <tr><td PORT="pd7">uint32 pd7:4</td></tr>
 *       </table>>];
 *
 *     Port_ConfigPinType [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="grey">Port_ConfigPinType</td></tr>
 *                         <tr><td PORT="port_pin">Port_PinType port_pin</td></tr>
 *                         <tr><td PORT="initPinState">Port_PinOutputState initPinState</td></tr>
 *                         <tr><td PORT="portMode">Port_PinModeType portMode</td></tr>
 *                         <tr><td PORT="directionChangeableDuringRuntime">boolean directionChangeableDuringRuntime</td></tr>
 *                         <tr><td PORT="modeChangeableDuringRuntime">boolean modeChangeableDuringRuntime</td></tr>
 *       </table>>];
 *
 *     Port_ConfigGptaMultiplexerArray [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="grey">Port_ConfigGptaMultiplexerArray</td></tr>
 *                         <tr><td PORT="gpta0_otm">otm</td></tr>
 *                         <tr><td PORT="gpta0_om">om</td></tr>
 *                         <tr><td PORT="gpta0_lim">lim</td></tr>
 *                         <tr><td PORT="gpta0_gim">gim</td></tr>
 *       </table>>];
 *
 *     Port_ConfigGptaMultiplexerArrayOtm [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="yellow">On-Chip Trigger and Gating Signal Multiplexer</td></tr>
 *                         <tr><td PORT="otmcr1">otmcr1</td></tr>
 *                         <tr><td PORT="otmcr0">otmcr0</td></tr>
 *       </table>>];
 *
 *     Port_ConfigGptaMultiplexerArrayOm [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="yellow">Output Multiplexer</td></tr>
 *                         <tr><td PORT="omcrh13">omcrh13</td></tr>
 *                         <tr><td PORT="omcrl13">omcrl13</td></tr>
 *                         <tr><td PORT="omcrh12">omcrh12</td></tr>
 *                         <tr><td PORT="omcrl12">omcrl12</td></tr>
 *                         <tr><td PORT="omcrh11">omcrh11</td></tr>
 *                         <tr><td PORT="omcrl11">omcrl11</td></tr>
 *                         <tr><td PORT="omcrh10">omcrh10</td></tr>
 *                         <tr><td PORT="omcrl10">omcrl10</td></tr>
 *                         <tr><td PORT="omcrh9">omcrh9</td></tr>
 *                         <tr><td PORT="omcrl9">omcrl9</td></tr>
 *                         <tr><td PORT="omcrh8">omcrh8</td></tr>
 *                         <tr><td PORT="omcrl8">omcrl8</td></tr>
 *                         <tr><td PORT="omcrh7">omcrh7</td></tr>
 *                         <tr><td PORT="omcrl7">omcrl7</td></tr>
 *                         <tr><td PORT="omcrh6">omcrh6</td></tr>
 *                         <tr><td PORT="omcrl6">omcrl6</td></tr>
 *                         <tr><td PORT="omcrh5">omcrh5</td></tr>
 *                         <tr><td PORT="omcrl5">omcrl5</td></tr>
 *                         <tr><td PORT="omcrh4">omcrh4</td></tr>
 *                         <tr><td PORT="omcrl4">omcrl4</td></tr>
 *                         <tr><td PORT="omcrh3">omcrh3</td></tr>
 *                         <tr><td PORT="omcrl3">omcrl3</td></tr>
 *                         <tr><td PORT="omcrh2">omcrh2</td></tr>
 *                         <tr><td PORT="omcrl2">omcrl2</td></tr>
 *                         <tr><td PORT="omcrh1">omcrh1</td></tr>
 *                         <tr><td PORT="omcrl1">omcrl1</td></tr>
 *                         <tr><td PORT="omcrh0">omcrh0</td></tr>
 *                         <tr><td PORT="omcrl0">omcrl0</td></tr>
 *       </table>>];
 *
 *     Port_ConfigGptaMultiplexerArrayLim [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="yellow">LTC input Multiplexer</td></tr>
 *                         <tr><td PORT="limcrh7">limcrh7</td></tr>
 *                         <tr><td PORT="limcrl7">limcrl7</td></tr>
 *                         <tr><td PORT="limcrh6">limcrh6</td></tr>
 *                         <tr><td PORT="limcrl6">limcrl6</td></tr>
 *                         <tr><td PORT="limcrh5">limcrh5</td></tr>
 *                         <tr><td PORT="limcrl5">limcrl5</td></tr>
 *                         <tr><td PORT="limcrh4">limcrh4</td></tr>
 *                         <tr><td PORT="limcrl4">limcrl4</td></tr>
 *                         <tr><td PORT="limcrh3">limcrh3</td></tr>
 *                         <tr><td PORT="limcrl3">limcrl3</td></tr>
 *                         <tr><td PORT="limcrh2">limcrh2</td></tr>
 *                         <tr><td PORT="limcrl2">limcrl2</td></tr>
 *                         <tr><td PORT="limcrh1">limcrh1</td></tr>
 *                         <tr><td PORT="limcrl1">limcrl1</td></tr>
 *                         <tr><td PORT="limcrh0">limcrh0</td></tr>
 *                         <tr><td PORT="limcrl0">limcrl0</td></tr>
 *       </table>>];
 *
 *     Port_ConfigGptaMultiplexerArrayGim [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="yellow">GTC input Multiplexer</td></tr>
 *                         <tr><td PORT="gimcrh3">gimcrh3</td></tr>
 *                         <tr><td PORT="gimcrl3">gimcrl3</td></tr>
 *                         <tr><td PORT="gimcrh2">gimcrh2</td></tr>
 *                         <tr><td PORT="gimcrl2">gimcrl2</td></tr>
 *                         <tr><td PORT="gimcrh1">gimcrh1</td></tr>
 *                         <tr><td PORT="gimcr11">gimcrl1</td></tr>
 *                         <tr><td PORT="gimcrh0">gimcrh1</td></tr>
 *                         <tr><td PORT="gimcrl0">gimcrl0</td></tr>
 *       </table>>];
 *
 *     Port_ConfigLtcaMultiplexerArray [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="grey">Port_ConfigLtcaMultiplexerArray</td></tr>
 *                         <tr><td PORT="ltca2_om">om</td></tr>
 *                         <tr><td PORT="ltca2_lim">lim</td></tr>
 *       </table>>];
 *
 *     Port_ConfigLtcaMultiplexerArrayOm [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="yellow">Output Multiplexer</td></tr>
 *                         <tr><td PORT="omcrh13">omcrh13</td></tr>
 *                         <tr><td PORT="omcrl13">omcrl13</td></tr>
 *                         <tr><td PORT="omcrh12">omcrh12</td></tr>
 *                         <tr><td PORT="omcrl12">omcrl12</td></tr>
 *                         <tr><td PORT="omcrh11">omcrh11</td></tr>
 *                         <tr><td PORT="omcrl11">omcrl11</td></tr>
 *                         <tr><td PORT="omcrh10">omcrh10</td></tr>
 *                         <tr><td PORT="omcrl10">omcrl10</td></tr>
 *                         <tr><td PORT="omcrh9">omcrh9</td></tr>
 *                         <tr><td PORT="omcrl9">omcrl9</td></tr>
 *                         <tr><td PORT="omcrh8">omcrh8</td></tr>
 *                         <tr><td PORT="omcrl8">omcrl8</td></tr>
 *                         <tr><td PORT="omcrh7">omcrh7</td></tr>
 *                         <tr><td PORT="omcrl7">omcrl7</td></tr>
 *                         <tr><td PORT="omcrh6">omcrh6</td></tr>
 *                         <tr><td PORT="omcrl6">omcrl6</td></tr>
 *                         <tr><td PORT="omcrh5">omcrh5</td></tr>
 *                         <tr><td PORT="omcrl5">omcrl5</td></tr>
 *                         <tr><td PORT="omcrh4">omcrh4</td></tr>
 *                         <tr><td PORT="omcrl4">omcrl4</td></tr>
 *                         <tr><td PORT="omcrh3">omcrh3</td></tr>
 *                         <tr><td PORT="omcrl3">omcrl3</td></tr>
 *                         <tr><td PORT="omcrh2">omcrh2</td></tr>
 *                         <tr><td PORT="omcrl2">omcrl2</td></tr>
 *                         <tr><td PORT="omcrh1">omcrh1</td></tr>
 *                         <tr><td PORT="omcrl1">omcrl1</td></tr>
 *                         <tr><td PORT="omcrh0">omcrh0</td></tr>
 *                         <tr><td PORT="omcrl0">omcrl0</td></tr>
 *       </table>>];
 *
 *     Port_ConfigLtcaMultiplexerArrayLim [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td PORT="Type" BGCOLOR="yellow">LTC input Multiplexer</td></tr>
 *                         <tr><td PORT="limcrl3">limcrl3</td></tr>
 *                         <tr><td PORT="limcrh2">limcrh2</td></tr>
 *                         <tr><td PORT="limcrl2">limcrl2</td></tr>
 *                         <tr><td PORT="limcrh1">limcrh1</td></tr>
 *                         <tr><td PORT="limcrl1">limcrl1</td></tr>
 *                         <tr><td PORT="limcrh0">limcrh0</td></tr>
 *                         <tr><td PORT="limcrl0">limcrl0</td></tr>
 *       </table>>];
 *
 *     Port_ConfigType:pinCfg -> Port_ConfigPinType:Type [label="array [1..numOfPins]" fontsize="8.0"];
 *     Port_ConfigType:portCfg -> Port_ConfigPortType:Type [label="array [1..numOfPorts]" fontsize="8.0"];
 *     Port_ConfigPortType:pdr -> Port_ConfigPDRPadType:Type;
 *
 *     Port_ConfigType:gptA0MulCfg -> Port_ConfigGptaMultiplexerArray:Type;
 *     Port_ConfigGptaMultiplexerArray:gpta0_otm -> Port_ConfigGptaMultiplexerArrayOtm:Type;
 *     Port_ConfigGptaMultiplexerArray:gpta0_om -> Port_ConfigGptaMultiplexerArrayOm:Type;
 *     Port_ConfigGptaMultiplexerArray:gpta0_lim -> Port_ConfigGptaMultiplexerArrayLim:Type;
 *     Port_ConfigGptaMultiplexerArray:gpta0_gim -> Port_ConfigGptaMultiplexerArrayGim:Type;

 *     Port_ConfigType:ltcA2MulCfg -> Port_ConfigLtcaMultiplexerArray:Type;
 *     Port_ConfigLtcaMultiplexerArray:ltca2_om -> Port_ConfigLtcaMultiplexerArrayOm:Type;
 *     Port_ConfigLtcaMultiplexerArray:ltca2_lim -> Port_ConfigLtcaMultiplexerArrayLim:Type;
 *   }
 * @enddot
 */
/*@{*/
#define PORT_PIN_INIT_STATE_NA

/* definition of port pin functionality */
#define PCX_NO_INPUT_PULL_DEVICE_CONNECTED               ((Port_PinModeType) 0x00) /**< No input pull device is connected.*/
#define PCX_INPUT_PULL_DOWN_DEVICE_CONNECTED             ((Port_PinModeType) 0x10) /**< Input pull down device is connected.*/
#define PCX_INPUT_PULL_UP_DEVICE_CONNECTED               ((Port_PinModeType) 0x20) /**< Input pull up device is connected.*/
#define PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT             ((Port_PinModeType) 0x80) /**< Push pull general purpose output is connected.*/
#define PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1        ((Port_PinModeType) 0x90) /**< Push pull alternate output function 1 is connected.*/
#define PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_2        ((Port_PinModeType) 0xA0) /**< Push pull alternate output function 2 is connected.*/
#define PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_3        ((Port_PinModeType) 0xB0) /**< Push pull alternate output function 3 is connected.*/
#define PCX_OPEN_DRAIN_GENERAL_PURPOSE_OUTPUT            ((Port_PinModeType) 0xC0) /**< Open drain general purpose output is connected.*/
#define PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_1       ((Port_PinModeType) 0xD0) /**< Open drain alternate output function 1 is connected.*/
#define PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_2       ((Port_PinModeType) 0xE0) /**< Open drain alternate output function 2 is connected.*/
#define PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_3       ((Port_PinModeType) 0xF0) /**< Open drain alternate output function 3 is connected.*/

/* Definition of port pad driver modes */
#define PORT_PDR_CLASS_A1_MEDIUM_DRIVER             ((uint32) 0) /**< pad class A1 medium driver selection - used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CLASS_A1_WEAK_DRIVER               ((uint32) 1) /**< pad class A1 weak driver selection - used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE  ((uint32) 0) /**< pad class A2 strong driver sharp edge selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CLASS_A2_STRONG_DRIVER_MEDIUM_EDGE ((uint32) 1) /**< pad class A2 strong driver medium edge selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CLASS_A2_STRONG_DRIVER_SOFT_EDGE   ((uint32) 2) /**< pad class A2 strong driver soft edge selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CLASS_A2_WEAK_DRIVER               ((uint32) 3) /**< pad class A2 weak driver selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CLASS_A2_MEDIUM_DRIVER             ((uint32) 4) /**< pad class A2 medium driver selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_CMOS_DRIVER                        ((uint32) 0) /**< pad CMOS driver selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_LVDS_DRIVER                        ((uint32) 4) /**< pad LVDS driver selection- used for configuration of \ref Port_ConfigPDRPadType. */
#define PORT_PDR_RESERVED                           ((uint32) 0) /**< pad reserved - used for configuration of \ref Port_ConfigPDRPadType. */

#define OTMG_IN0 ((uint32)(0x00u)) /**< Selects OTMG input IN0. */
#define OTMG_IN1 ((uint32)(0x01u)) /**< Selects OTMG input IN1. */
#define OTMG_IN2 ((uint32)(0x02u)) /**< Selects OTMG input IN2. */
#define OTMG_IN3 ((uint32)(0x03u)) /**< Selects OTMG input IN3. */
#define OTMG_IN4 ((uint32)(0x04u)) /**< Selects OTMG input IN4. */
#define OTMG_IN5 ((uint32)(0x05u)) /**< Selects OTMG input IN5. */
#define OTMG_IN6 ((uint32)(0x06u)) /**< Selects OTMG input IN6. */
#define OTMG_IN7 ((uint32)(0x07u)) /**< Selects OTMG input IN7. */

#define OTMCRX(field, otm) ((uint32)((otm) << ((field) * 4u))) /**< This macro is used to setup a field OTM0..OTM7 in a OTMCR register. */

#define OMG_IN0 ((uint32)(0x00u)) /**< Selects OMG input IN0. */
#define OMG_IN1 ((uint32)(0x01u)) /**< Selects OMG input IN1. */
#define OMG_IN2 ((uint32)(0x02u)) /**< Selects OMG input IN2. */
#define OMG_IN3 ((uint32)(0x03u)) /**< Selects OMG input IN3. */
#define OMG_IN4 ((uint32)(0x04u)) /**< Selects OMG input IN4. */
#define OMG_IN5 ((uint32)(0x05u)) /**< Selects OMG input IN5. */
#define OMG_IN6 ((uint32)(0x06u)) /**< Selects OMG input IN6. */
#define OMG_IN7 ((uint32)(0x07u)) /**< Selects OMG input IN7. */

#define OMG0 ((uint32)(0x00u << 4u)) /**< Selects OMG0g. */
#define OMG1 ((uint32)(0x01u << 4u)) /**< Selects OMG1g. */
#define OMG2 ((uint32)(0x02u << 4u)) /**< Selects OMG2g. */

#define OMG_GTC0_31		OMG0	/**< Output is controlled by GTC0..31 */
#define OMG_LTC0_31		OMG1	/**< Output is controlled by LTC0..31 */
#define OMG_LTC32_63	OMG2	/**< Output is controlled by LTC32..63 */

#define OMCRX(pos, omg, oml) ((uint32)(((omg) | (oml)) << ((pos & 3u) * 8))) /**< This macro is used to setup one field
OML0/OMG0..OML3/OMG3 in the OMCRL register or
one field OML4/OMG4..OML7/OMG7 in the OMCRH register. */


#define GIMG_IN0  ((uint32)(0x00u)) /**< Selects GIMG input IN0. */
#define GIMG_IN1  ((uint32)(0x01u)) /**< Selects GIMG input IN1. */
#define GIMG_IN2  ((uint32)(0x02u)) /**< Selects GIMG input IN2. */
#define GIMG_IN3  ((uint32)(0x03u)) /**< Selects GIMG input IN3. */
#define GIMG_IN4  ((uint32)(0x04u)) /**< Selects GIMG input IN4. */
#define GIMG_IN5  ((uint32)(0x05u)) /**< Selects GIMG input IN5. */
#define GIMG_IN6  ((uint32)(0x06u)) /**< Selects GIMG input IN6. */
#define GIMG_IN7  ((uint32)(0x07u)) /**< Selects GIMG input IN7. */

#define GIMG0  ((uint32)(0x00u)) /**< Selects GIMG0g. */
#define GIMG1  ((uint32)(0x01u)) /**< Selects GIMG1g. */
#define GIMG2  ((uint32)(0x02u)) /**< Selects GIMG2g. */
#define GIMG3  ((uint32)(0x03u)) /**< Selects GIMG3g. */
#define GIMG4  ((uint32)(0x04u)) /**< Selects GIMG4g. */

#define GIMG_IOG0_3         GIMG0  /**< Selects IOG0. */
#define GIMG_IOG4_6         GIMG1  /**< Selects IOG1. */
#define GIMG_LTC0_31       GIMG2  /**< Selects LTC0..31. */
#define GIMG_LTC32_63      GIMG3  /**< Selects LTC32..64. */
#define GIMG_FPC0_5_INT0_1 GIMG4  /**< Selects FPC0..5 or INT0..1. */

#define GIMCRX(pos, gimen, gimg, giml) ((uint32)((((gimen) << 7) | (gimg) | (giml)) << ((pos & 3u) * 8))) /**<
This macro is used to setup the fields field GIML0/GIMG0..GIML3/GIMG3, GIMEN0..3 in a GIMCRL register or
GIML4/GIMG4..GIML7/GIMG7, GIMEN4..7 in a GIMCRH register. */

#define LIMG_IN0  ((uint32)(0x00u)) /**< Selects LIMG input IN0. */
#define LIMG_IN1  ((uint32)(0x01u)) /**< Selects LIMG input IN1. */
#define LIMG_IN2  ((uint32)(0x02u)) /**< Selects LIMG input IN2. */
#define LIMG_IN3  ((uint32)(0x03u)) /**< Selects LIMG input IN3. */
#define LIMG_IN4  ((uint32)(0x04u)) /**< Selects LIMG input IN4. */
#define LIMG_IN5  ((uint32)(0x05u)) /**< Selects LIMG input IN5. */
#define LIMG_IN6  ((uint32)(0x06u)) /**< Selects LIMG input IN6. */
#define LIMG_IN7  ((uint32)(0x07u)) /**< Selects LIMG input IN7. */

#define LIMG0  ((uint32)(0x00u << 4u)) /**< Selects LIMG0g. */
#define LIMG1  ((uint32)(0x01u << 4u)) /**< Selects LIMG1g. */
#define LIMG2  ((uint32)(0x02u << 4u)) /**< Selects LIMG2g. */
#define LIMG3  ((uint32)(0x03u << 4u)) /**< Selects LIMG3g. */
#define LIMG4  ((uint32)(0x04u << 4u)) /**< Selects LIMG4g. */

#define LIMG_IOG0_3      LIMG0 /**< Selects LIMG IOG0..3. */
#define LIMG_IOG4_6      LIMG1 /**< Selects LIMG IOG4..6. */
#define LIMG_GTC0_31     LIMG2 /**< Selects LIMG GTC0..31. */
#define LIMG_CLK0_7      LIMG3 /**< Selects LIMG CLK0..7. */
#define LIMG_PDL_INT0_3  LIMG4 /**< Selects LIMG CLK/PDL0..3. */

#define LIMCRX(pos, limen, limg, liml) ((uint32)((((limen) << 7) | (limg) | (liml)) << ((pos  & 3u) * 8))) /**<
This macro is used to setup the fields field LIML0/LIMG0..LIML3/LIMG3, LIMEN0..3 in a LIMCRL register or
LIML4/LIMG4..LIML7/LIMG7, LIMEN4..7 in a LIMCRH register. */
/*@}*/

/**
 * The structure is used to define the configuration of a port pin.
 * @ingroup group_port_pbt_config
 *
 * For the element Port_ConfigPinType::initPinState the following selection is possible
 * @li \ref PORT_PIN_LEVEL_LOW
 * @li \ref PORT_PIN_LEVEL_HIGH
 *
 * For the element Port_ConfigPinType::portMode the following selection is possible
 * @li \ref PCX_NO_INPUT_PULL_DEVICE_CONNECTED
 * @li \ref PCX_INPUT_PULL_DOWN_DEVICE_CONNECTED
 * @li \ref PCX_INPUT_PULL_UP_DEVICE_CONNECTED
 * @li \ref PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT
 * @li \ref PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1
 * @li \ref PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_2
 * @li \ref PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_3
 * @li \ref PCX_OPEN_DRAIN_GENERAL_PURPOSE_OUTPUT
 * @li \ref PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_1
 * @li \ref PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_2
 * @li \ref PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_3
 */
typedef struct {
	Port_PinType port_pin;                    /**< The the pin on a port to be configured. */
	Port_PinOutputState initPinState;         /**< The init state of the port pin after execution of \ref Port_Init. */
	Port_PinModeType portMode;                /**< The control coding of the port pin - mode. */
	boolean directionChangeableDuringRuntime; /**< The direction of the port pin is changeable during runtime. */
	boolean modeChangeableDuringRuntime;      /**< The mode of the port pin is changeable during runtime. */
} Port_ConfigPinType;


/**
 * The configuration data structure is used to define the pad driver modes of a port in the configuration
 * structure \ref Port_ConfigPortType.
 * @ingroup group_port_pbt_config
 *
 *
 * @note Each port contains its own specific PDR register, described additionally at each port,
 * that can contain between one and eight PDx fields. Each field controls a number of pins,
 * normally four, but in general between one and sixteen.
 *
 * The pad driver mode register contains the following fields.
 * @image html  external_images/tc1767_Px_PDR.png "Pad Driver Mode Register"
 * @image latex external_images/tc1767_Px_PDR.png "Pad Driver Mode Register"
 *
 * Each field in the pad driver mode register has to be filled with a pad driver mode selection.
 * The pad driver mode selection is shown in the following figure.
 * @image html  external_images/tc1767_PadDriverModeSelection.png "Pad Driver Mode Field Selection"
 * @image latex external_images/tc1767_PadDriverModeSelection.png "Pad Driver Mode Field Selection"
 *
 * A pad driver mode selection defines the pad driver class of a pad driver.
 * A selection of Class A1, Class A2 or Class F (Port6 only) can be made.
 * The selection depends on the capability of the pad driver of a port pin. The capability of each
 * port pin is described in the controller users manual chapters "Port X Pad Driver Mode Register and
 * Pad Classes".
 * The following figure shows the pad driver classes characteristics.
 * @image html  external_images/tc1767_PadClassesSummary.png "Pad Driver Classes Summary"
 * @image latex external_images/tc1767_PadClassesSummary.png "Pad Driver Classes Summary"
 *
 * The following definitions are used to setup the configuration of the pad driver mode fields.
 *
 * @li \ref PORT_PDR_CLASS_A1_MEDIUM_DRIVER
 * @li \ref PORT_PDR_CLASS_A1_WEAK_DRIVER
 * @li \ref PORT_PDR_CLASS_A2_STRONG_DRIVER_SHARP_EDGE
 * @li \ref PORT_PDR_CLASS_A2_STRONG_DRIVER_MEDIUM_EDGE
 * @li \ref PORT_PDR_CLASS_A2_STRONG_DRIVER_SOFT_EDGE
 * @li \ref PORT_PDR_CLASS_A2_WEAK_DRIVER
 * @li \ref PORT_PDR_CLASS_A2_MEDIUM_DRIVER
 * @li \ref PORT_PDR_CMOS_DRIVER
 * @li \ref PORT_PDR_LVDS_DRIVER
 */
typedef struct {
	uint32 pd0:4; /**< Pad driver mode field 0 in a Pn_PDR register.*/
	uint32 pd1:4; /**< Pad driver mode field 1 in a Pn_PDR register.*/
	uint32 pd2:4; /**< Pad driver mode field 2 in a Pn_PDR register.*/
	uint32 pd3:4; /**< Pad driver mode field 3 in a Pn_PDR register.*/
	uint32 pd4:4; /**< Pad driver mode field 4 in a Pn_PDR register.*/
	uint32 pd5:4; /**< Pad driver mode field 5 in a Pn_PDR register.*/
	uint32 pd6:4; /**< Pad driver mode field 6 in a Pn_PDR register.*/
	uint32 pd7:4; /**< Pad driver mode field 7 in a Pn_PDR register.*/
} Port_ConfigPDRPadType;

/**
 * The configuration structure is used to configure the port related hardware.
 * @ingroup group_port_pbt_config
 *
 * The configuration contains the
 * @li The port identifier for the port that is configured.
 * @li Pad Driver Mode Selection of the port.
 *
 * @image html  external_images/tc1767_Px_PDR.png "Pad Driver Mode Selection"
 * @image latex external_images/tc1767_Px_PDR.png "Pad Driver Mode Selection"
 */
typedef struct {
	Port_IdType port;          /**< The port identifier denoting the configuration to be used for the port. */
	Port_ConfigPDRPadType pdr; /**< The pad driver mode of the port - loaded to register Px_PDR. */
} Port_ConfigPortType;


/**
 * On-Chip Trigger and Gating Signal Multiplexer Control Registers.
 * @ingroup group_port_pbt_config
 *
 * OTMCR controls the connections of I/O output group signals to the Trigger and Gating
 * Signals TRIGgn.
 *
 */
typedef struct {
	uint32 otmcr1; /**< GPTA0 On-Chip Trigger and Gating Multiplexer Control Register of Group 1. */
	uint32 otmcr0; /**< GPTA0 On-Chip Trigger and Gating Multiplexer Control Register of Group 0. */
}Port_ConfigGPTAOtmType;

/**
 * This configuration structure is used to configure the GPTA Multiplexer Register Array.
 * @ingroup group_port_pbt_config
 *
 * A total of 54 control registers are required to program the configuration of the output
 * multiplexer, the On-chip trigger and gating multiplexer, and the two input multiplexers of
 * the Input/Output Line Sharing Block.
 *
 * @image html  external_images/tc1767_GPTAMultiplexerFIFO.png "GPTA Multiplexer Array Control FIFO Structure"
 * @image latex external_images/tc1767_GPTAMultiplexerFIFO.png "GPTA Multiplexer Array Control FIFO Structure" width=14cm
 *
 * <b>On-Chip Trigger and Gating Signal Multiplexer Control Registers</b> - \ref Port_ConfigGptaMultiplexerArray::otm
 *
 * OTMCR controls the connections of I/O output group signals to the Trigger and Gating
 * Signals TRIGgn.
 *
 * <b>Output Multiplexer Control Registers</b> - \ref Port_ConfigGptaMultiplexerArray::om
 *
 * Two registers, OMCRL and OMCRH, are assigned to each I/O Group IOG[6:0] and each
 * Output Group OG[6:0]. OMCRL[6:0]/OMCRH[6:0] are assigned to IOG[6:0] and
 * OMCRL[13:7]/OMCRH[13:7] are assigned to OG[6:0].
 * OMCRL controls the connections of group pins 0 to 3. OMCRH controls the connections
 * of group pins 4 to 7.
 *
 * <b>LTC Input Multiplexer Control Registers</b> - \ref Port_ConfigGptaMultiplexerArray::lim
 *
 * Two registers, LIMCRL and LIMCRH, are assigned to each LTC group. LIMCRL controls
 * the connections of LTC group cells with index 0 to 3. LIMCRH controls the connections
 * of LTC group cells with index 4 to 7.
 *mn bvcxz
 *
 * <b>GTC Input Multiplexer Control Registers</b> - \ref Port_ConfigGptaMultiplexerArray::gim
 *
 * Two registers, GIMCRL and GIMCRH, are assigned to each GTCG[3:0]. GIMCRL
 * controls the connections of cells 0 to 3 in a GTC Group. GIMCRH controls the
 * connections of cells 4 to 7 in a GTC Group.
 *
 *
 */
typedef struct {
	struct {
		uint32 otmcr1; /**< GPTA0 On-Chip Trigger and Gating Multiplexer Control Register of Group 1. */
		uint32 otmcr0; /**< GPTA0 On-Chip Trigger and Gating Multiplexer Control Register of Group 0. */
	} otm; /**< On-Chip Trigger and Gating Signal Multiplexer Control Registers. */
	struct {
		uint32 omcrh13; /**< OMCRH13 OUT[111:108] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrl13; /**< OMCRL13 OUT[107:104] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrh12; /**< OMCRH12 OUT[103:100] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrl12; /**< OMCRL12 OUT[99:96] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrh11; /**< OMCRH11 OUT[95:92] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]. */
		uint32 omcrl11; /**< OMCRL11 OUT[91:88] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]. */
		uint32 omcrh10; /**< OMCRH10 OUT[87:84] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]. */
		uint32 omcrl10; /**< OMCRL10 OUT[83:80] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]. */
		uint32 omcrh9;  /**< OMCRH9 OUT[79:76] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrl9;  /**< OMCRL9 OUT[75:72] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrh8;  /**< OMCRH8 OUT[71:68] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrl8;  /**< OMCRL8 OUT[67:64] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrh7;  /**< OMCRH7 OUT[63:60] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]. */
		uint32 omcrl7;  /**< OMCRL7 OUT[59:56] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]. */
		uint32 omcrh6;  /**< OMCRH6 IN[55:52]/OUT[55:52] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]. */
		uint32 omcrl6;  /**< OMCRL6 IN[51:48]/OUT[51:48] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]. */
		uint32 omcrh5;  /**< OMCRH5 IN[47:44]/OUT[47:44] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrl5;  /**< OMCRL5 IN[43:40]/OUT[43:40] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrh4;  /**< OMCRH4 IN[39:36]/OUT[39:36]selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrl4;  /**< OMCRL4 IN[35:32]/OUT[35:32] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrh3;  /**< OMCRH3 IN[31:28]/OUT[31:28] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]. */
		uint32 omcrl3;  /**< OMCRL3 IN[27:24]/OUT[27:24] selectable group GTCG3=GTC[31:24], LTCG3=LTC[31:24], LTCG7=LTC[63:56]. */
		uint32 omcrh2;  /**< OMCRH2 IN[23:20]/OUT[23:20] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]. */
		uint32 omcrl2;  /**< OMCRL2 IN[19:16]/OUT[19:16] selectable group GTCG2=GTC[23:16], LTCG2=LTC[23:16], LTCG6=LTC[55:48]. */
		uint32 omcrh1;  /**< OMCRH1 IN[15:12]/OUT[15:12] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrl1;  /**< OMCRL1 IN[11:08]/OUT[11:08] selectable group GTCG1=GTC[15:8], LTCG1=LTC[15:8], LTCG5=LTC[47:40]. */
		uint32 omcrh0;  /**< OMCRH0 IN[07:04]/OUT[07:04] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
		uint32 omcrl0;  /**< OMCRL0 IN[03:00]/OUT[03:00] selectable group GTCG0=GTC[7:0], LTCG0=LTC[7:0], LTCG4=LTC[39:32]. */
	}om; /**< Output Multiplexer Control Registers. */
	struct {
		uint32 limcrh7; /**< LIMCRH7 LTC[63:60] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl7; /**< LIMCRL7 LTC[59:56] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh6; /**< LIMCRH6 LTC[55:52] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl6; /**< LIMCRL6  LTC[51:48] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh5; /**< LIMCRH5 LTC[47:44] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl5; /**< LIMCRL5 LTC[43:40] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh4; /**< LIMCRH4 LTC[39:36] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl4; /**< LIMCRL4 LTC[35:32] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh3; /**< LIMCRH3 LTC[31:28] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl3; /**< LIMCRL3 LTC[27:24] selectable group IOG3, GTCG3=GTC[31:24], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh2; /**< LIMCRH2 LTC[23:20] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl2; /**< LIMCRL2 LTC[19:16] selectable group IOG2, IOG6, GTCG2=GTC[23:16], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh1; /**< LIMCRH1 LTC[15:12] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl1; /**< LIMCRL1 LTC[11:08] selectable group IOG1, IOG5, GTCG1=GTC[15:8], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh0; /**< LIMCRH0 LTC[07:04]] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl0; /**< LIMCRL0 LTC[03:00] selectable group IOG0, IOG4, GTCG0=GTC[7:0], CLK[7:0], PDL[3:0]/INT[3:0]. */
	}lim; /**< LTC Input Multiplexer Control Registers. */
	struct {
		uint32 gimcrh3; /**< GIMCRH3 GTC[31:28] selectable group IOG3, LTCG3=LTC[31:24], LTCG7=LTC[63:56], FPC[5:0]/INT[1:0]. */
		uint32 gimcrl3; /**< GIMCRL3 GTC[27:24] selectable group IOG3, LTCG3=LTC[31:24], LTCG7=LTC[63:56], FPC[5:0]/INT[1:0]. */
		uint32 gimcrh2; /**< GIMCRH2 GTC[23:20] selectable group IOG2, IOG6, LTCG2=LTC[23:16], LTCG6=LTC[55:48], FPC[5:0]/INT[1:0]. */
		uint32 gimcrl2; /**< GIMCRL2 GTC[19:16] selectable group IOG2, IOG6, LTCG2=LTC[23:16], LTCG6=LTC[55:48], FPC[5:0]/INT[1:0]. */
		uint32 gimcrh1; /**< GIMCRH1 GTC[15:12] selectable group IOG1, IOG5, LTCG1=LTC[15:8], LTCG5=LTC[47:40], FPC[5:0]/INT[1:0]. */
		uint32 gimcrl1; /**< GIMCRL1 GTC[11:08] selectable group IOG1, IOG5, LTCG1=LTC[15:8], LTCG5=LTC[47:40], FPC[5:0]/INT[1:0]. */
		uint32 gimcrh0; /**< GIMCRH0 GTC[07:04] selectable group IOG0, IOG4, LTCG0=LTC[7:0], LTCG4=LTC[39:32], FPC[5:0]/INT[1:0]. */
		uint32 gimcrl0; /**< GIMCRL0 GTC[03:00] selectable group IOG0, IOG4, LTCG0=LTC[7:0], LTCG4=LTC[39:32], FPC[5:0]/INT[1:0]. */
	}gim; /**< GTC Input Multiplexer Control Registers. */
}Port_ConfigGptaMultiplexerArray;


/**
 * This configuration structure is used to configure the LTCA Multiplexer Register Array.
 * @ingroup group_port_pbt_config
 *
 * A total of 24 control registers are required to program the configuration of the output
 * multiplexer and the LTC input multiplexer of the Input/Output Line Sharing Block.
 *
 * @image html  external_images/tc1767_LTCAMultiplexerFIFO.png "LTCA Multiplexer Array Control FIFO Structure"
 * @image latex external_images/tc1767_LTCAMultiplexerFIFO.png "LTCA Multiplexer Array Control FIFO Structure" width=14cm
 *
 *
 * <b>Output Multiplexer Control Registers</b> - \ref Port_ConfigLtcaMultiplexerArray::om
 *
 * Two registers, OMCRL and OMCRH, are assigned to each I/O Group IOG[3:0] and each
 * Output Group OG[3:0]. OMCRL[3:0]/OMCRH[3:0] are assigned to IOG[3:0] and
 * OMCRL[10:7]/OMCRH[10:7] are assigned to OG[3:0].
 * OMCRL controls the connections of group pins 0 to 3. OMCRH controls the connections
 * of group pins 4 to 7.
 *
 * <b>LTC Input Multiplexer Control Registers</b> - \ref Port_ConfigLtcaMultiplexerArray::lim
 *
 * Two registers, LIMCRL and LIMCRH, are assigned to each LTC group. LIMCRL controls
 * the connections of LTC group cells with index 0 to 3. LIMCRH controls the connections
 * of LTC group cells with index 4 to 7.
 */
typedef struct {
	struct {
		uint32 omcrh13; /**< OMCRH13 OUT[111:108] selectable group LTCG1=LTC[15:8]. */
		uint32 omcrl13; /**< OMCRL13 OUT[107:104] selectable group LTCG1=LTC[15:8]. */
		uint32 omcrh12; /**< OMCRH12 OUT[103:100] selectable group LTCG0=LTC[7:0]. */
		uint32 omcrl12; /**< OMCRL12 OUT[99:96] selectable group LTCG0=LTC[7:0]. */
		uint32 omcrh11; /**< OMCRH11 OUT[95:92] selectable group LTCG3=LTC[31:24]. */
		uint32 omcrl11; /**< OMCRL11 OUT[91:88] selectable group LTCG3=LTC[31:24]. */
		uint32 omcrh10; /**< OMCRH10 OUT[87:84] selectable group LTCG2=LTC[23:16]. */
		uint32 omcrl10; /**< OMCRL10 OUT[83:80] selectable group LTCG2=LTC[23:16]. */
		uint32 omcrh3; /**< OMCRH3 IN[31:28]/OUT[31:28] selectable group LTCG3=LTC[31:24]. */
		uint32 omcrl3; /**< OMCRL3 IN[27:24]/OUT[27:24] selectable group LTCG3=LTC[31:24]. */
		uint32 omcrh2; /**< OMCRH2 IN[23:20]/OUT[23:20] selectable group LTCG2=LTC[23:16]. */
		uint32 omcrl2; /**< OMCRL2 IN[19:16]/OUT[19:16] selectable group LTCG2=LTC[23:16]. */
		uint32 omcrh1; /**< OMCRH1 IN[15:12]/OUT[15:12] selectable group LTCG1=LTC[15:8]. */
		uint32 omcrl1; /**< OMCRL1 IN[11:08]/OUT[11:08] selectable group LTCG1=LTC[15:8]. */
		uint32 omcrh0; /**< OMCRH0 IN[07:04]/OUT[07:04] selectable group LTCG0=LTC[7:0]. */
		uint32 omcrl0; /**< OMCRL0 IN[03:00]/OUT[03:00] selectable group LTCG0=LTC[7:0]. */
	}om; /**< Output Multiplexer Control Registers. */
	struct {
		uint32 limcrh3; /**< LIMCRH3 LTC[31:28] selectable group IOG3, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl3; /**< LIMCRL3 LTC[27:24] selectable group IOG3, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh2; /**< LIMCRH2 LTC[23:20] selectable group IOG2, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl2; /**< LIMCRL2 LTC[19:16] selectable group IOG2, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh1; /**< LIMCRH1 LTC[15:12] selectable group IOG1, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl1; /**< LIMCRL1 LTC[11:08] selectable group IOG1, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrh0; /**< LIMCRH0 LTC[07:04] selectable group IOG0, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
		uint32 limcrl0; /**< LIMCRL0 LTC[03:00] selectable group IOG0, CLOCK[7:0], PDL[3:0]/INT[3:0]. */
	}lim; /**< LTC Input Multiplexer Control Registers. */
}Port_ConfigLtcaMultiplexerArray;

/**
 * The initialization data structure used to define the initialization data for the port driver.
 * @ingroup group_port_pbt_config
 *
 */
typedef struct {
	uint16 numOfPins;                         /**< The number of pin configurations in the pin configuration array. */
	uint8 numOfPorts;                         /**< The number of port configurations in the port configuration array. */
	const Port_ConfigPinType *pinCfg;         /**< The reference to the pin configuration array. */
	const Port_ConfigPortType *portCfg;       /**< The reference to the port configuration array. */
	const Port_ConfigGptaMultiplexerArray *portGpta0MultiplexerArrayCfg; /**< The reference to the GPTA0 multiplexer array configuration. */
	const Port_ConfigLtcaMultiplexerArray *portLtca2MultiplexerArrayCfg; /**< The reference to the LTCA2 multiplexer array configuration. */
} Port_ConfigType;



#define PORT_START_SEC_CODE
#include "MemMap.h"

/** This service initializes the Port driver.
 *
 * This function uses the passed configuration data set and sets the pin configuration
 * according to this configuration set. Pins that are not contained in the
 * configuration set remain unchanged.
 *
 * This function can be called multiple times, however only the latest configuration
 * set will be used by the service Port_RefreshPortDirection().
 *
 * @param cfg The pointer to the configuration to be used for port initialization.
 */
extern void Port_Init(const Port_ConfigType* cfg);


/** This service sets the port pin direction.
 *
 * The service Port_SetPinDirection set the port pin direction.
 *
 * @note The direction is only changeable during runtime, if the configuration \ref Port_ConfigPinType::directionChangeableDuringRuntime is set to TRUE.
 *
 * @note \anchor Port_SetPinDirectionRestriction The setting of the direction is not possible without choosing the output characteristics as well
 *       as the selection of the pull up/down and  the selection of the output function. This leads to the fact that a fixed setup is choosen:
 *       @li setting the direction to PORT_PIN_IN will select input with no pull device connected \ref PCX_NO_INPUT_PULL_DEVICE_CONNECTED,
 *       @li setting the direction to PORT_PIN_OUT will select push pull general purpose output \ref PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT.
 *          @image html  external_images/tc1767_PortControlCoding.png "Port Control Coding"
 *          @image latex external_images/tc1767_PortControlCoding.png "Port Control Coding"
 *
 * @param port_pin The port pin, where the direction shall be set.
 * @param dir The direction to be set, \ref PORT_PIN_IN, \ref PORT_PIN_OUT.
 */
extern void Port_SetPinDirection(Port_PinType port_pin, Port_PinDirectionType dir);


/** This service refreshes the port direction.
 *
 * The service refreshes the port direction. The refresh is done on all IOCRx registers
 * that whose direction is not changeable by software. This service only considers the
 * pins contained in the last configuration set.
 *
 * @warning The service refreshes also the output characteristics, the selected pull up/pull down as well as the
 *          selected output function of the port pin. This is due to the fact that there is no dedicated port
 *          direction register in the port control coding of the TC1767.
 *          @image html  external_images/tc1767_PortControlCoding.png "Port Control Coding"
 *          @image latex external_images/tc1767_PortControlCoding.png "Port Control Coding"
 *
 * Example:
 * @code
 * Port_SetPinDirection(PORT_0_2, PORT_PIN_IN);
 * @endcode
 */
extern void Port_RefreshPortDirection(void);


/** This service sets the port pin mode.
 *
 * The service Port_SetPinMode sets the mode of a port pin.
 * Possible modes are:
 * <ul>
 * <li> \ref PCX_NO_INPUT_PULL_DEVICE_CONNECTED        </li>
 * <li> \ref PCX_INPUT_PULL_DOWN_DEVICE_CONNECTED      </li>
 * <li> \ref PCX_INPUT_PULL_UP_DEVICE_CONNECTED        </li>
 * <li> \ref PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT      </li>
 * <li> \ref PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_1 </li>
 * <li> \ref PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_2 </li>
 * <li> \ref PCX_PUSH_PULL_ALTERNATE_OUTPUT_FUNCTION_3 </li>
 * <li> \ref PCX_OPEN_DRAIN_GENERAL_PURPOSE_OUTPUT     </li>
 * <li> \ref PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_1</li>
 * <li> \ref PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_2</li>
 * <li> \ref PCX_OPEN_DRAIN_ALTERNATE_OUTPUT_FUNCTION_3</li>
 * </ul>
 *
 * @note     The mode is only changeable during runtime, if the configuration \ref Port_ConfigPinType::modeChangeableDuringRuntime is set to TRUE.
 * @note     The respective alternate output functions that can be selected for a port pin are described in microcontroller manual.
 *
 * @note \anchor Port_SetPinModeRestriction The setting of the mode is not possible without choosing the pin direction as well as the selection
 *       of the pull up/down and the selection of the output function. This leads to the fact that a fixed setup is choosen:
 *          @image html  external_images/tc1767_PortControlCoding.png "Port Control Coding"
 *          @image latex external_images/tc1767_PortControlCoding.png "Port Control Coding"
 *
 * @param port_pin The port pin where the mode shall be set.
 * @param mode The mode of the pin to be set.
 *
 * Example:
 * @code
 * Port_SetPinMode(PORT_0_5, PCX_OPEN_DRAIN_GENERAL_PURPOSE_OUTPUT);
 * @endcode
 */
extern void Port_SetPinMode(Port_PinType port_pin, Port_PinModeType mode);

#if defined(PORT_VERSION_INFO_API) && (PORT_VERSION_INFO_API == STD_ON)
/** The function Port_GetVersionInfo returns the version information of the Port module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Port_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter PortVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function+
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = PORT_MODULE_ID;
	versioninfo->sw_major_version = PORT_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = PORT_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = PORT_SW_PATCH_VERSION;
	versioninfo->vendorID = PORT_VENDOR_ID;
}
#endif


#define PORT_STOP_SEC_CODE
#include "MemMap.h"

/*@}*/

#endif
