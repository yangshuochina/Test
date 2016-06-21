/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Dio.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:24:49MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Dio.h  $
 *  Revision 1.3 2011/03/22 09:24:49MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.2 2010/11/22 07:55:52MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:15:49MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Dio/AA000/src/project.pj
 *
 * ***************************************************************************/
 
#ifndef DIO_H_
#define DIO_H_

#include "team-s.h"
#include "Std_Types.h"

/** @defgroup group_dio Dio 
 *  @ingroup group_dio */
/*@{*/

#define DIO_MODULE_ID         (120)
#define DIO_VENDOR_ID         (TEAMS_VENDOR_ID)
#define DIO_AR_MAJOR_VERSION  (2)
#define DIO_AR_MINOR_VERSION  (2)
#define DIO_AR_PATCH_VERSION  (2)
#define DIO_SW_MAJOR_VERSION  (1)
#define DIO_SW_MINOR_VERSION  (0)
#define DIO_SW_PATCH_VERSION  (0)

/* Driver APIs Service ID */
#define DIO_READCHANNEL_API_ID          ((uint8)(0x0))
#define DIO_WRITECHANNEL_API_ID         ((uint8)(0x1))
#define DIO_READPORT_API_ID             ((uint8)(0x2))
#define DIO_WRITEPORT_API_ID            ((uint8)(0x3))
#define DIO_READCHANNELGROUP_API_ID    	((uint8)(0x4))
#define DIO_WRITECHANNELGROUP_API_ID    ((uint8)(0x5))
#define DIO_GETVERSIONINFO_API_ID		((uint8)(0x12))

/* DIO Development Error Codes */
#define DIO_E_PARAM_INVALID_CHANNEL_ID	((uint8)(10))	/**< An invalid channel ID was requested. */
#define DIO_E_PARAM_INVALID_PORT_ID		((uint8)(20))	/**< An invalid port ID was requested. */
#define DIO_E_PARAM_INVALID_GROUP_ID	((uint8)(31))	/**< An invalid channel group id was passed. */



/** @page dio Dio
 *
 * @section dio_intro Introduction
 * The Dio function implements the Dio functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_DIO_Driver.pdf">AUTOSAR_SWS_DIO_Driver</a>.
 *
 * The TC1767 implements 10 ports with a variable number of pins on each port. The following
 * lists the available pins.
 * @li P0 16 pins
 * @li P1 16 pins
 * @li P2 14 pins
 * @li P3 16 pins
 * @li P4 4 pins
 * @li P5 16 pins
 * @li P6 4 pins
 *
 * To use these pins they have to be configured. This configuration is done by Port.h.
 *
 * @section dio_init Initialization of Dio
 * Before any service to the Dio can be used the initialization function of Port.h must be
 * used.
 *
 * @section dio_autosar AutoSAR Conformance
 * All services as requested by AutoSAR are implemented.
 *
 * @section dio_cfg Configuration
 * @subsection dio_cfg_step_by_step Step by Step
 * @li Configure the symbolic names for channel IDs in Dio_Cfg.h.
 * @li Configure the desired port groups (#Dio_ChannelGroupType)
 *
 * @subsection dio_cfg_by_example Example
 * Defintion of symbolic names:
 * @code
 * #define PIN_FOO   DIO_CHANNEL_ID(3, 2)   // FOO is connected to P3.2
 * @endcode
 *
 * Definition of a port group:
 * @code
 * const Dio_ChannelGroupType group_0 = {
 *   4,                 Dio_ChannelGroupType::offset
 *   1,                 Dio_ChannelGroupType::port
 *   0xF0               Dio_ChannelGroupType::mask
 * };
 * @endcode
 *
 * Usage of the above:
 * @code
 * // get the pin state of P3.2
 * level = Dio_ReadChannel(PIN_FOO);
 *
 * // set pins P1.4 and P1.5 and clear P1.6 and P1.7
 * Dio_WriteChannelGroup(&group0, 3);
 * @endcode
 */

#define DIO_PORT_0  ((Dio_PortType)(0))	/**< Port ID for Port 0. */
#define DIO_PORT_1  ((Dio_PortType)(1))	/**< Port ID for Port 1. */
#define DIO_PORT_2  ((Dio_PortType)(2))	/**< Port ID for Port 2. */
#define DIO_PORT_3  ((Dio_PortType)(3))	/**< Port ID for Port 3. */
#define DIO_PORT_4  ((Dio_PortType)(4))	/**< Port ID for Port 4. */
#define DIO_PORT_5  ((Dio_PortType)(5))	/**< Port ID for Port 5. */
#define DIO_PORT_6  ((Dio_PortType)(6))	/**< Port ID for Port 6. */

/** This macro returns a port ID for a given port.
 *
 * Instead of using this macro the direct definitions #DIO_PORT_0 to #DIO_PORT_6
 * can be used.
 *
 * The following example reads all pins the port 4.
 * @code
 * Dio_PortLevelType port_val = Dio_ReadPort(DIO_PORT_ID(4));
 * @endcode
 */
#define DIO_PORT_ID(port)          ((Dio_PortType)(port))


/** This macro returns a channel ID for a given port and pin combination.
 *
 * The following example reads pin 2 of the port 4.
 * @code
 * Dio_LevelType port_val = Dio_ReadChannel(DIO_CHANNEL_ID(4, 2));
 * @endcode
 */
#define DIO_CHANNEL_ID(port, pin)  ((Dio_ChannelType)(((port) << 8) | (pin)))


/** Numeric ID of a Dio channel. */
typedef uint16 Dio_ChannelType;

/** Numeric ID of a Dio Port. */
typedef uint8 Dio_PortType;

/** Level of the Dio channel. */
typedef uint8 Dio_LevelType;

/** Levels of all pins of a port. */
typedef uint16 Dio_PortLevelType;

/** This type is used for the definition of a channel group, which consists of several
 * adjoining channels within a port.
 *
 * Here is an example how the channel group interpretation works:
 *
 * @image latex external_images/dio_group.pdf "DIO Group"
 */
/* @dot
 *   digraph dio_group {
 *     node [fontsize="10.0"];
 *     offset_val [label="offset=2"];
 *     port_val [label="port=4"];
 *     mask_val [label="mask=0xC"];
 *     node [shape=plaintext fontsize="12.0"];
 *     port [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td rowspan="2" BGCOLOR="grey">Port 4</td>
 *                             <td BGCOLOR="lightblue" PORT="b3">3</td>
 *                             <td BGCOLOR="lightblue" PORT="b2">2</td>
 *                             <td BGCOLOR="lightblue" PORT="b1">1</td>
 *                             <td BGCOLOR="lightblue" PORT="b0">0</td></tr>
 *                         <tr><td PORT="v3">1</td>
 *                             <td PORT="v2">0</td>
 *                             <td PORT="v1">1</td>
 *                             <td PORT="v0">1</td>
 *                         </tr></table>>];
 *     mask [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td rowspan="2" BGCOLOR="grey">mask</td>
 *                             <td BGCOLOR="lightblue" PORT="b3">3</td>
 *                             <td BGCOLOR="lightblue" PORT="b2">2</td>
 *                             <td BGCOLOR="lightblue" PORT="b1">1</td>
 *                             <td BGCOLOR="lightblue" PORT="b0">0</td></tr>
 *                         <tr><td PORT="v3">1</td>
 *                             <td PORT="v2">1</td>
 *                             <td PORT="v1">0</td>
 *                             <td PORT="v0">0</td>
 *                         </tr></table>>];
 *     result [label=<
 *       <table BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *                         <tr><td rowspan="2" BGCOLOR="grey">result</td>
 *                             <td BGCOLOR="lightblue" PORT="b3">3</td>
 *                             <td BGCOLOR="lightblue" PORT="b2">2</td>
 *                             <td BGCOLOR="lightblue" PORT="b1">1</td>
 *                             <td BGCOLOR="lightblue" PORT="b0">0</td></tr>
 *                         <tr><td PORT="v3">0</td>
 *                             <td PORT="v2">0</td>
 *                             <td PORT="v1">1</td>
 *                             <td PORT="v0">0</td>
 *                         </tr></table>>];
 *     { rank=same;  port; port_val; }
 *     { rank=same;  mask; mask_val; }
 *     { rank=same;  result; offset_val; }
 *     port:v3 -> mask:b3;
 *     mask:v3 -> result:b1;
 *     port:v2 -> mask:b2;
 *     mask:v2 -> result:b0;
 *   }
 * @enddot
 */
typedef struct {
	uint8 offset;  		/**< This element is the position of the Channel Group on the port, counted from the LSB. */
	Dio_PortType port;  /**< This element is the port on which the Channel group is defined. */
	uint16 mask;		/**< This element is the mask which defines the positions of the channel group. */
} Dio_ChannelGroupType;

#define DIO_START_SEC_CODE
#include "MemMap.h"


/** This service return the level of the specified Dio channel.
 *
 * @param channel_id  the ID of the Dio channel
 * @return #STD_HIGH: The physical pin level is high\n
 *         #STD_LOW:  the physical pin level is low
 */
extern Dio_LevelType Dio_ReadChannel(Dio_ChannelType channel_id);

/** This service sets the level of the specified Dio channel.
 *
 * @param channel_id  the ID of the Dio channel
 * @param level       #STD_HIGH: The physical pin level shall be high\n
 *                    #STD_LOW:  the physical pin level shall be low
 */
extern void Dio_WriteChannel(Dio_ChannelType channel_id, Dio_LevelType level);

/** This service return the levels of the pins contained in an port.
 *
 * @param port_id  the ID of the Dio port
 * @return bit mask of all pins contained on the port.
 */
extern Dio_PortLevelType Dio_ReadPort(Dio_PortType port_id);

/** This service sets the levels of all pins of a port.
 *
 * @param port_id  the ID of the Dio channel
 * @param levels   Bit masked value containing all pins of a port
 */
extern void Dio_WritePort(Dio_PortType port_id, Dio_PortLevelType levels);

/** This service return the levels of the pins contained in the requested channel group.
 *
 * @param channel_group  definition of the channel group (see #Dio_ChannelGroupType)
 * @return bit mask of all pins contained in the channel group.
 */
extern Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *channel_group);

/** This service sets the levels of all pins contained in the requested channel group.
 *
 * @param channel_group  definition of the channel group (see #Dio_ChannelGroupType)
 * @param levels   Bit masked value containing all pins of the group
 */
extern void Dio_WriteChannelGroup(const Dio_ChannelGroupType *channel_group, Dio_PortLevelType levels);


/** The function Dio_GetVersionInfo returns the version information of the Dio module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Dio_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter DioVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function+
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Dio_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = DIO_MODULE_ID;
	versioninfo->sw_major_version = DIO_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = DIO_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = DIO_SW_PATCH_VERSION;
	versioninfo->vendorID = DIO_VENDOR_ID;
}

#define DIO_STOP_SEC_CODE
#include "MemMap.h"

/*@}*/

#endif /* DIO_H_ */
