/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Dio_Channel.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:45:23MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Dio_Channel.c  $
 *  Revision 1.2 2010/07/12 09:45:23MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:15:50MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Dio/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Dio.h"
#include "Dio_Priv.h"


#define DIO_START_SEC_CODE
#include "MemMap.h"


Dio_LevelType Dio_ReadChannel(Dio_ChannelType channel_id)
{
	uint8 ts_error = 0;
	uint32 port;
	uint32 pin;
	Dio_PortLevelType port_val;

	port = (uint32)channel_id >> 8u;
	pin = (uint32)channel_id & 0xFFu;

	DIO_ASSERT(port <= 6, DIO_READCHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT(pin < 16, DIO_READCHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT(port != 2 || (pin < 14), DIO_READCHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT(port != 4 || (pin < 4), DIO_READCHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT(port != 6 || (pin < 4), DIO_READCHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);

	if(ts_error == 0) {
		port_val = *((volatile uint16 *)(&P0_IN.U) + (port * 0x80));

		if((port_val >> pin) & 1u) {
			return (Dio_LevelType)STD_HIGH;
		}
		else {
			return (Dio_LevelType)STD_LOW;
		}
	}

	return 0;
}


void Dio_WriteChannel(Dio_ChannelType channel_id, Dio_LevelType level)
{
	uint32 port;
	uint32 pin;

	port = (uint32)channel_id >> 8u;
	pin = (uint32)channel_id & 0xFFu;

	DIO_ASSERT_RETVOID(port <= 6, DIO_WRITECHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT_RETVOID(pin < 16, DIO_WRITECHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT_RETVOID((port != 2) || (pin < 14), DIO_WRITECHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT_RETVOID((port != 4) || (pin < 4), DIO_WRITECHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
	DIO_ASSERT_RETVOID((port != 6) || (pin < 4), DIO_WRITECHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);

	if(level == STD_HIGH) {
		ts_setpin(port, pin);
	}
	else {
		ts_clrpin(port, pin);
	}
}
#define DIO_STOP_SEC_CODE
#include "MemMap.h"

