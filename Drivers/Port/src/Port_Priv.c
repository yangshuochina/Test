/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Port_Priv.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:19:25MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Port_Priv.c  $
 *  Revision 1.1 2010/05/19 14:19:25MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Port/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Port_Priv.h"

#define PORT_START_SEC_CONST_16BIT
#include "MemMap.h"

#if defined(PORT_DEV_ERROR_DETECT) && (PORT_DEV_ERROR_DETECT == STD_ON)
/**
 * This array contains the available pins of a port.
 * The first entry is Port 0, LSB is pin 0, MSB is pin 15.
 */
const uint16 port_pin_avail[] = {
		0xFFFF, /*< available pin mask for Port 0 - 0..15 */
		0xFFFF, /*< available pin mask for Port 1 - 0..15 */
		0x3FFF, /*< available pin mask for Port 2 - 0..13 */
		0xFFFF, /*< available pin mask for Port 3 - 0..15 */
		0x000F, /*< available pin mask for Port 4 - 0..3  */
		0xFFFF, /*< available pin mask for Port 5 - 0..15 */
		0x000F, /*< available pin mask for Port 6 - 0..3  */
};

#endif

#define PORT_STOP_SEC_CONST_16BIT
#include "MemMap.h"
