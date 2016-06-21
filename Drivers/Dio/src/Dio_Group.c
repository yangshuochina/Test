/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Dio_Group.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:15:51MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Dio_Group.c  $
 *  Revision 1.1 2010/05/19 14:15:51MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Dio/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Dio.h"
#include "Dio_Priv.h"


#define DIO_START_SEC_CODE
#include "MemMap.h"


Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *channel_group)
{
	Dio_PortLevelType port_val;
	Dio_PortType port_id;

	DIO_ASSERT_RETERR(channel_group != NULL_PTR, DIO_READCHANNELGROUP_API_ID, DIO_E_PARAM_INVALID_GROUP_ID);
	port_id = channel_group->port;
	DIO_ASSERT_RETERR(port_id <= 6, DIO_READCHANNELGROUP_API_ID, DIO_E_PARAM_INVALID_PORT_ID);

	port_val = (Dio_PortLevelType)(*((volatile uint16 *)(&P0_IN.U) + (port_id * 0x80)));
	port_val &= channel_group->mask;
	port_val >>= channel_group->offset;
	return port_val;
}


void Dio_WriteChannelGroup(const Dio_ChannelGroupType *channel_group, Dio_PortLevelType levels)
{
	Dio_PortType port_id;
	volatile uint32 *omr;
	uint32 level_clr;
	uint32 level_set;

	DIO_ASSERT_RETVOID(channel_group != NULL_PTR, DIO_WRITECHANNELGROUP_API_ID, DIO_E_PARAM_INVALID_GROUP_ID);
	port_id = channel_group->port;
	DIO_ASSERT_RETVOID(port_id <= 6, DIO_WRITECHANNELGROUP_API_ID, DIO_E_PARAM_INVALID_PORT_ID);

	/* prepare the set and clear mask */
	levels <<= channel_group->offset;
	level_set = levels & channel_group->mask;
	level_clr = (~(uint32)levels) & (uint32)channel_group->mask;

	/* make a single 32 bit write to set and clear all requested ports */
	omr = (volatile uint32 *)(&P0_OMR.U) + (port_id * 0x40);
	*omr = level_set | (level_clr << 16);
}
#define DIO_STOP_SEC_CODE
#include "MemMap.h"

