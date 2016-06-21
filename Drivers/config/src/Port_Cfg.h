/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Port_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:00:46MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Port_Cfg.h  $
 *  Revision 1.2 2010/11/22 09:00:46MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/20 11:29:02MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

/** @ingroup group_port */

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

#include "Port.h"
#include "Std_Types.h"

#define PORT_INSTANCE_ID				0


/** @defgroup group_port_pct_config Port Pre Compile Time Configuration
 *  @ingroup group_port_config
 */
/*@{*/

/**
 * Enable or disable development error detection.
 * Selection STD_ON, STD_OFF.
 */
#define PORT_DEV_ERROR_DETECT           STD_ON

/**
 * Enables the usage of the port pin direction api \ref Port_SetPinDirection.
 * Selection STD_ON, STD_OFF.
 */
#define PORT_SET_PIN_DIRECTION_API      STD_ON

/**
 * Enables the usage of the port pin mode api \ref Port_SetPinMode.
 * Selection STD_ON, STD_OFF.
 */
#define PORT_SET_PIN_MODE_API           STD_ON


/**
 * Enables the usage of the port version api \ref Port_GetVersionInfo.
 * Selection STD_ON, STD_OFF.
 */
#define PORT_VERSION_INFO_API           STD_ON

/*@}*/

#endif
