/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Dio_Cfg.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/11/22 08:59:33MEZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Dio_Cfg.h  $
 * Revision 1.3 2010/11/22 08:59:33MEZ Cotor Vlad RGB (COTORV) 
 * Update BSW software modules with doxygen documentation comments
 * Revision 1.2 2010/07/30 16:30:35MESZ Cotor Vlad RGB (COTORV) 
 * Update BSW for contactor control complex device driver due to specification change
 * Revision 1.1 2010/05/20 11:29:00MESZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 * Revision 1.1 2010/03/08 14:06:22MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/config/src/project.pj
 * *****************************************************************************/

/** @ingroup group_dio */

#ifndef DIO_CFG_H_
#define DIO_CFG_H_

#include "Std_Types.h"

#define DIO_DEV_ERROR_DETECT        STD_ON
#define DIO_INSTANCE_ID				0

/* === Configuration Evaluation Board === */
#define LED_0  DIO_CHANNEL_ID(DIO_PORT_5, 0)
#define LED_1  DIO_CHANNEL_ID(DIO_PORT_5, 1)
#define LED_2  DIO_CHANNEL_ID(DIO_PORT_5, 2)
#define LED_3  DIO_CHANNEL_ID(DIO_PORT_5, 3)
#define LED_4  DIO_CHANNEL_ID(DIO_PORT_5, 4)
#define LED_5  DIO_CHANNEL_ID(DIO_PORT_5, 5)
#define LED_6  DIO_CHANNEL_ID(DIO_PORT_5, 6)
/* === End Configuration Evaluation Board === */

/*@}*/


#endif /* DIO_CFG_H_ */
