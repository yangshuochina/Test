/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Port_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:19:26MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Port_Priv.h  $
 *  Revision 1.1 2010/05/19 14:19:26MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Port/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef PORT_PRIV_H_
#define PORT_PRIV_H_

#include "Std_Types.h"
#include "team-s.h"
#include "Port_Cfg.h"


#define PORT_MAX_PORT   6
#define PORT_MAX_PIN_PER_PORT   16


/*
 * This support function returns the port coded in variable of type \ref Port_PinType.
 * @param port_pin The variable containing the port and pin information.
 * @return The port as Port_IdType value.
 */
#define PORT_GetPort(port_pin)     ((Port_IdType)((Port_PinType)(port_pin)))

/*
 * This support function returns the pin coded in variable of type \ref Port_PinType.
 * @param port_pin The variable containing the port and pin information.
 * @return The pin as Port_PinIdType value.
 */
#define PORT_GetPortPin(port_pin)  ((Port_PinIdType)((Port_PinType)(port_pin) / (Port_PinType)256))


#define PCX_PORT_OUTPUT_MODE ((uint8)0x80) /* Mask to check if port is operating in output mode. */


#define PORT_PERIPHERAL_BASE_ADDRESS (0xF0000C00u)
#define PORT_REG_IOCR0_OFFSET  (0x10u)
#define PORT_REG_IOCR4_OFFSET  (0x14u)
#define PORT_REG_IOCR8_OFFSET  (0x18u)
#define PORT_REG_IOCR12_OFFSET (0x1Cu)
#define PORT_REG_PDR_OFFSET    (0x40u)
#define PORT_REG_OMR_OFFSET    (0x04u)
#define PORT_REG_ESR_OFFSET    (0x50u)
#define PORT_REG_OUT_OFFSET    (0x00u)
#define PORT_REG_IN_OFFSET     (0x24u)



#define PORT_REG_ADDRESS(port,offset) (PORT_PERIPHERAL_BASE_ADDRESS + (offset) + ((port) * 0x100))


#define PORT_REG_PTR8(port, reg_offset)  ((volatile uint8*) PORT_REG_ADDRESS(port, reg_offset))
#define PORT_REG_PTR16(port, reg_offset) ((volatile uint16*)PORT_REG_ADDRESS(port, reg_offset))
#define PORT_REG_PTR32(port, reg_offset) ((volatile uint32*)PORT_REG_ADDRESS(port, reg_offset))

#define PORT_SetReg(addr, val) (*(addr) = (val))
#define PORT_SetRegBit32(addr, bitpos, val) (*(addr) = (*(addr) & (~(uint32)(1u << (bitpos)))) | (((uint32)(val)) << (bitpos)))
#define PORT_BfldReg32(addr, mask, val)  (*(addr) = ((*(addr))) & (~((uint32)(mask))) | ((val) & (((uint32)(mask)))))
#define PORT_BfldOffsReg32(addr, mask, val, offset)  (*(addr) = (*(addr) & (~((uint32)(mask) << (offset)))) | (((val) << (offset)) & (((uint32)(mask)) << (offset)))


#define PORT_SetReg_Pn_IOCR0(port, val) PORT_SetReg(PORT_REG_PTR32(port, PORT_REG_IOCR0_OFFSET), val)
#define PORT_SetReg_Pn_IOCR4(port, val) PORT_SetReg(PORT_REG_PTR32(port, PORT_REG_IOCR4_OFFSET), val)
#define PORT_SetReg_Pn_IOCR8(port, val) PORT_SetReg(PORT_REG_PTR32(port, PORT_REG_IOCR8_OFFSET), val)
#define PORT_SetReg_Pn_IOCR12(port, val) PORT_SetReg(PORT_REG_PTR32(port, PORT_REG_IOCR12_OFFSET), val)
#define PORT_SetReg_Pn_IOCRx(port, pin, val) PORT_SetReg(PORT_REG_PTR8(port, PORT_REG_IOCR0_OFFSET + (pin)), (uint8)(val))

/*
 * Helper macro to set the PDR register of a port - Pn_PDR.
 * @param port The port identifier.
 * @param val The PDR value to set for the port.
 */
#define PORT_SetReg_Pn_PDR(port, val) PORT_SetReg(PORT_REG_PTR32((Port_IdType)(port), PORT_REG_PDR_OFFSET), (uint32)(val))

/*
 * Helper macro to set the output register of a port pin - Pn_OUT.
 * @param port The port identifier.
 * @param pin The pin identifier on the port.
 * @param val The output value to set for the port pin.
 */
#define PORT_SetBit_Pn_OUT(port, pin, val)   PORT_SetRegBit32(PORT_REG_PTR32((Port_IdType)(port), PORT_REG_OUT_OFFSET),   (Port_PinIdType)(pin), (uint32)(val))

/*
 * Type definition to map the bitfield define by \ref Port_ConfigPDRPadType to a uint32 type.
 */
typedef union {
	Port_ConfigPDRPadType pdrPads;
	uint32 U;
}Port_Config_Pn_PDRRegType;


/*-------------------------Debug for development/production------------------------------------------------------*/
#if defined(PORT_DEV_ERROR_DETECT) && (PORT_DEV_ERROR_DETECT == STD_ON)

#define PORT_START_SEC_CONST_16BIT
#include "MemMap.h"

extern const uint16 port_pin_avail[7];

#define PORT_STOP_SEC_CONST_16BIT
#include "MemMap.h"

#endif


#if defined(PORT_DEV_ERROR_DETECT) && (PORT_DEV_ERROR_DETECT == STD_ON)
#  define PORT_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, PORT_MODULE_ID, PORT_INSTANCE_ID, api_id, error_id)
#  define PORT_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, PORT_MODULE_ID, PORT_INSTANCE_ID, api_id, error_id)
#else
#  define PORT_ASSERT_RETERR(cond, api_id, error_id)
#  define PORT_ASSERT_RETVOID(cond, api_id, error_id)
#endif


#endif
