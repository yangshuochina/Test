/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Dio_Port.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:45:32MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Dio_Port.c  $
 *  Revision 1.2 2010/07/12 09:45:32MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:15:52MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Dio/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Dio.h"
#include "Dio_Priv.h"


#define DIO_START_SEC_CODE
#include "MemMap.h"

Dio_PortLevelType Dio_ReadPort(Dio_PortType port_id)
{
	uint8 ts_error = 0;
	Dio_PortLevelType port_val;

	DIO_ASSERT(port_id <= 6, DIO_READPORT_API_ID, DIO_E_PARAM_INVALID_PORT_ID);

	if(ts_error == 0) {
		port_val = (Dio_PortLevelType)(*((volatile uint16 *)(&P0_IN.U) + (port_id * 0x80)));
		return port_val;
	}

	return 0;
}


void Dio_WritePort(Dio_PortType port_id, Dio_PortLevelType levels)
{
	DIO_ASSERT_RETVOID(port_id <= 6, DIO_WRITEPORT_API_ID, DIO_E_PARAM_INVALID_PORT_ID);

	*((volatile uint16 *)(&P0_OUT.U) + (port_id * 0x80)) = levels;
}

#define DIO_STOP_SEC_CODE
#include "MemMap.h"


