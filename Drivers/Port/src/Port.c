/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Port.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:19:22MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Port.c  $
 *  Revision 1.1 2010/05/19 14:19:22MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Port/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Port.h"
#include "Port_Priv.h"



#define PORT_START_SEC_VAR_32BIT
#include "MemMap.h"
/*
 * Reference to active configuration.
 */
static const Port_ConfigType* port_cfg;

#define PORT_STOP_SEC_VAR_32BIT
#include "MemMap.h"



#define PORT_START_SEC_VAR_16BIT
#include "MemMap.h"


#if defined(PORT_SET_PIN_DIRECTION_API) && (PORT_SET_PIN_DIRECTION_API == STD_ON)
/*
 * Array containing the port pin direction changeable during runtime allowed information.
 * index 0: Port 0
 * index 1: Port 1
 * ...
 * index PORT_MAX_PORT: Port PORT_MAX_PORT
 */
static uint16 port_pin_dir_changeable[PORT_MAX_PORT + 1];
#endif


#if defined(PORT_SET_PIN_MODE_API) && (PORT_SET_PIN_MODE_API == STD_ON)
/*
 * Array containing the port pin mode changeable during runtime allowed information.
 * index 0: Port 0
 * index 1: Port 1
 * ...
 * index PORT_MAX_PORT: Port PORT_MAX_PORT
 */
static uint16 port_pin_mode_changeable[PORT_MAX_PORT + 1];
#endif

#define PORT_STOP_SEC_VAR_16BIT
#include "MemMap.h"



#define PORT_START_SEC_CODE
#include "MemMap.h"

void Port_Init(const Port_ConfigType* cfg)
{
	int idx;
	const uint32 *tmpPtr32;
	sint8_least i;
	Port_Config_Pn_PDRRegType pdr_reg;
	Port_IdType port;
	Port_PinIdType pin;
	const Port_ConfigPinType *pin_cfg;

	/* initialization not yet succeeded successfully */
	port_cfg = NULL_PTR;

	/* check for a valid configuration */
	PORT_ASSERT_RETVOID(cfg != NULL_PTR, PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);
	PORT_ASSERT_RETVOID(cfg->pinCfg != NULL_PTR || cfg->numOfPins==0, PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);
	PORT_ASSERT_RETVOID(cfg->portCfg != NULL_PTR || cfg->numOfPorts==0, PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);

	tmpPtr32 = (const uint32*)(cfg->portGpta0MultiplexerArrayCfg);
	if(tmpPtr32 != NULL_PTR) {
		/* setup GPTA0 Multiplexer Array configuration */
		/* 1. Disable interconnections of the multiplexer array */
		GPTA0_MRACTL.B.MAEN = 0;

		/* 2. Reset the write cycle counter to 0 */
		GPTA0_MRACTL.B.WCRES = 1;

		/* 3. Write sequentially the multiplexer control register contents one after the other
		 * (54 values) into MRADIN, starting with the register values for OTMCR1, OTMCR0,
		 * up to GIMCRH0, GIMCRL0.
		 */
		for(idx = 0; idx < 54; idx++) {
			GPTA0_MRADIN.U = tmpPtr32[idx];
		}

		/* 4. Enable the multiplexer array. This establishes and
		 * enables all programmed interconnections.
		 */
		GPTA0_MRACTL.B.MAEN = 1;
	}

	tmpPtr32 = (const uint32*)(cfg->portLtca2MultiplexerArrayCfg);
	if(tmpPtr32 != NULL_PTR) {
		/* setup LTCA2 Multiplexer Array configuration */
		/* 1. Disable interconnections of the multiplexer array */
		LTCA2_MRACTL.B.MAEN = 0;

		/* 2. Reset the write cycle counter to 0 */
		LTCA2_MRACTL.B.WCRES = 1;

		/* 3. Write sequentially the multiplexer control register contents one after the other
		 * (24 values) into MRADIN, starting with the register values for OMCRH10, OMCRL10,
		 * ... up to LIMCRH0, LIMCRL0
		 */
		for(idx = 0; idx < 24; idx++) {
			LTCA2_MRACTL.U = tmpPtr32[idx];
		}

		/* 4. Enable the multiplexer array. This establishes and
		 * enables all programmed interconnections.
		 */
		LTCA2_MRACTL.B.MAEN = 1;
	}


	pin_cfg = cfg->pinCfg;

	/* setup port configuration */
	for(i = 0; i < cfg->numOfPorts; i++) {
		port = cfg->portCfg[i].port;
		/* check range and availability of port */
		PORT_ASSERT_RETVOID((port <= PORT_MAX_PORT), PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);

		/* set port pad driver modes*/
		pdr_reg.pdrPads = cfg->portCfg[i].pdr;
		PORT_SetReg_Pn_PDR(port, pdr_reg.U);
	}


	/* setup port pin configuration */
	for(i=0; i < cfg->numOfPins; i++) {
		port = PORT_GetPort(pin_cfg[i].port_pin);
		pin = PORT_GetPortPin(pin_cfg[i].port_pin);

		/* check range and availability of port and pin */
		PORT_ASSERT_RETVOID((port <= PORT_MAX_PORT) && (pin < PORT_MAX_PIN_PER_PORT), PORT_INIT_API_ID, PORT_E_PARAM_PIN);
		PORT_ASSERT_RETVOID((port_pin_avail[port] & (1u << pin)) > 0 , PORT_INIT_API_ID, PORT_E_PARAM_PIN);

		/* set output state of pin (can be done for input too) */
		PORT_SetBit_Pn_OUT(port,pin,pin_cfg[i].initPinState);

		/* set port input/output control register */
		PORT_SetReg_Pn_IOCRx(port, pin, pin_cfg[i].portMode);

#if defined(PORT_SET_PIN_DIRECTION_API) && (PORT_SET_PIN_DIRECTION_API == STD_ON)
		/* store direction runtime changeable information */
		if(pin_cfg[i].directionChangeableDuringRuntime != FALSE) {
			/* changeable */
			port_pin_dir_changeable[port] |= (uint16)(((uint16)1) << pin);
		}
		else {
			/* not changeable */
			port_pin_dir_changeable[port] &= (uint16)(~(((uint16)1) << pin));
		}
#endif

#if defined(PORT_SET_PIN_MODE_API) && (PORT_SET_PIN_MODE_API == STD_ON)
		/* store direction runtime changeable information */
		if(pin_cfg[i].modeChangeableDuringRuntime != FALSE) {
			/* changeable */
			port_pin_mode_changeable[port] |= (uint16)(((uint16)1) << pin);
		}
		else {
			/* not changeable */
			port_pin_mode_changeable[port] &= (uint16)(~(((uint16)1) << pin));
		}
#endif
	}

	port_cfg = cfg;
}


#if defined(PORT_SET_PIN_DIRECTION_API) && (PORT_SET_PIN_DIRECTION_API == STD_ON)

void Port_SetPinDirection(Port_PinType port_pin, Port_PinDirectionType dir)
{
	Port_IdType port;
	Port_PinIdType pin;

	PORT_ASSERT_RETVOID(port_cfg != NULL_PTR, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_UNINIT);

	port = PORT_GetPort(port_pin);
	pin = PORT_GetPortPin(port_pin);

	/* check range, availability and changeability of direction of port and pin */
	PORT_ASSERT_RETVOID((port <= PORT_MAX_PORT) && (pin < PORT_MAX_PIN_PER_PORT), PORT_SET_PIN_DIRECTION_API_ID, PORT_E_PARAM_PIN);
	PORT_ASSERT_RETVOID((port_pin_avail[port] & (1u << pin)) > 0 , PORT_SET_PIN_DIRECTION_API_ID, PORT_E_PARAM_PIN);
	PORT_ASSERT_RETVOID((port_pin_dir_changeable[port] & (1u << pin)) > 0 , PORT_SET_PIN_DIRECTION_API_ID, PORT_E_DIRECTION_UNCHANGEABLE);

	if(dir == PORT_PIN_IN) {
		PORT_SetReg_Pn_IOCRx(port, pin, PCX_NO_INPUT_PULL_DEVICE_CONNECTED);
	}
	else {
		PORT_SetReg_Pn_IOCRx(port, pin, PCX_PUSH_PULL_GENERAL_PURPOSE_OUTPUT);
	}
}

#endif


void Port_RefreshPortDirection(void)
{
	Port_IdType port;
	Port_PinIdType pin;
	sint8_least i;
	const Port_ConfigPinType *pin_cfg;

	PORT_ASSERT_RETVOID(port_cfg != NULL_PTR, PORT_REFRESH_PORT_DIRECTION_API_ID, PORT_E_UNINIT);

	pin_cfg = port_cfg->pinCfg;

	/* setup port pin configuration */
	for(i = 0; i < port_cfg->numOfPins; i++) {
		port = PORT_GetPort(pin_cfg[i].port_pin);
		pin = PORT_GetPortPin(pin_cfg[i].port_pin);

#if defined(PORT_SET_PIN_DIRECTION_API) && (PORT_SET_PIN_DIRECTION_API == STD_ON)
		/* refresh only port pin directions, where direction is not changeable during runtime. */
		if((port_pin_dir_changeable[port] & (1u << pin)) == 0)
#endif
#if defined(PORT_SET_PIN_MODE_API) && (PORT_SET_PIN_MODE_API == STD_ON)
		/* refresh only port pin directions, where mode is not changeable during runtime. */
		if((port_pin_mode_changeable[port] & (1u << pin)) == 0)
#endif
		/* set port input/output control register */
		PORT_SetReg_Pn_IOCRx(port, pin, pin_cfg[i].portMode);
	}
}



#if defined(PORT_SET_PIN_MODE_API) && (PORT_SET_PIN_MODE_API == STD_ON)

void Port_SetPinMode(Port_PinType port_pin, Port_PinModeType mode)
{
	Port_IdType port;
	Port_PinIdType pin;

	PORT_ASSERT_RETVOID(port_cfg != NULL_PTR, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_UNINIT);

	port = PORT_GetPort(port_pin);
	pin = PORT_GetPortPin(port_pin);

	/* check the validity of the parameters */
	PORT_ASSERT_RETVOID((port <= PORT_MAX_PORT) && (pin < PORT_MAX_PIN_PER_PORT), PORT_SET_PIN_MODE_API_ID, PORT_E_PARAM_PIN);
	PORT_ASSERT_RETVOID((port_pin_avail[port] & (1u << pin)) > 0 , PORT_SET_PIN_MODE_API_ID, PORT_E_PARAM_PIN);
	PORT_ASSERT_RETVOID(((mode & 0xFu) == 0) && (mode <= 0x20 || mode >= 0x80), PORT_SET_PIN_MODE_API_ID, PORT_E_PARAM_INVALID_MODE)
	PORT_ASSERT_RETVOID((port_pin_mode_changeable[port] & (1u << pin)) > 0 , PORT_SET_PIN_MODE_API_ID, PORT_E_MODE_UNCHANGEABLE);

	/* set new mode */
	PORT_SetReg_Pn_IOCRx(port, pin, mode);
}

#endif


#define PORT_STOP_SEC_CODE
#include "MemMap.h"
