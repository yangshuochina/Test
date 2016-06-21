/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Crc.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 07:55:15MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Crc.h  $
 *  Revision 1.3 2010/11/22 07:55:15MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:44:49MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:24:58MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/ServiceLayer/Crc/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef CRC_H_
#define CRC_H_

#include "team-s.h"
#include "Std_Types.h"
#include "Crc_Cfg.h"

/** @defgroup group_crc Crc
 *  @ingroup group_crc*/
/*@{*/

#define CRC_MODULE_ID         (201)
#define CRC_VENDOR_ID         (TEAMS_VENDOR_ID)
#define CRC_AR_MAJOR_VERSION  (3)
#define CRC_AR_MINOR_VERSION  (0)
#define CRC_AR_PATCH_VERSION  (1)
#define CRC_SW_MAJOR_VERSION  (1)
#define CRC_SW_MINOR_VERSION  (0)
#define CRC_SW_PATCH_VERSION  (1)

/* Driver APIs Service ID */
#define CRC_CALCULATECRC8_API_ID		((uint8)(1))
#define CRC_CALCULATECRC16_API_ID		((uint8)(2))
#define CRC_CALCULATECRC32_API_ID		((uint8)(3))
#define CRC_GETVERSIONINFO_API_ID		((uint8)(4))

/* Development Error Codes */
/* None */

/** @page crc Crc
 *
 * @section crc_intro Introduction
 * The Crc function implements the Crc functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_CRC_Routines.pdf">AUTOSAR_SWS_CRC_Routines</a>.
 *
 *
 * @section Crc_autosar AutoSAR Conformance
 *  @li No HW support is available from TC1767 for CRC.
 *  @li HW CRC calculation is routed to Runtime calculation method for 8, 16 and 32 bit.
 *  @li <B> CRC8 result doesn't match with the specification </B>
 *      <a href="../external/autosar/AUTOSAR_SWS_CRC_Routines.pdf">AUTOSAR_SWS_CRC_Routines</a> test data tabel. \n
 *      Refer the online tool at http://zorc.breitbandkatze.de/crc.html for verification. \n
  *     Due to this CRC8 interface doesn't give result according to the AutoSAR Specification.\n
 *
 *
 *
 *
 * @section Crc Configuration
 * @li Select the start value to be passed. Refer #CRC_INITIAL_VALUE8, #CRC_INITIAL_VALUE16 or #CRC_INITIAL_VALUE32
 * @li Configure the Mode to be used for calculation.Refer #CRC_8_MODE , #CRC_16_MODE or #CRC_32_MODE
 * @li No initialization function is available for CRC routines.
 *
 *
 *
 */


#define CRC_START_SEC_CODE
#include "MemMap.h"

/** This service calculates an 8bit CRC for the given area.
 *
 * This service calculates the CRC starting from the address Crc_DataPtr . It
 * calculates for Crc_Length bytes. It uses Crc_StartValue8 as a start value
 * for the calculation.
 * The polynomial used is x8 + x4 + x3 + x2 + 1.
 * Name of the polynomial is CRC-8-SAE J1850.
 *
 * @note There is no parameter validation implemented.
 *
 * @param Crc_DataPtr  Pointer to the start address of the memory region.
 * @param Crc_Length  Size of memory to be used for calculation.
 * @param Crc_StartValue8  The start value of CRC.
 *
 * @return  CRC over a memory region.
 *
 */

extern uint8 Crc_CalculateCRC8(const uint8* Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8);
/** This service calculates an 16bit CRC for the given area.
 *
 * This service calculates the CRC starting from the address Crc_DataPtr . It
 * calculates for Crc_Length bytes. It uses Crc_StartValue16 as a start value
 * for the calculation. The polynomial used is x16 + x12 + x5 + 1.
 * Name of the polynomial is CRC-CCITT.
 *
 * @note There is no parameter validation implemented.
 *
 * @param Crc_DataPtr  Pointer to the start address of the memory region.
 * @param Crc_Length  Size of memory to be used for calculation.
 * @param Crc_StartValue16  The start value of CRC.
 *
 * @return  CRC over a memory region.
 *
 */
extern uint16 Crc_CalculateCRC16(const uint8* Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16);

/** This service calculates an 32bit CRC for the given area.
 *
 * This service calculates the CRC starting from the address Crc_DataPtr . It
 * calculates for Crc_Length bytes. It uses Crc_StartValue32 as a start value
 * for the calculation.
 * The polynomial used is x32 + x26 +  x23 + x22 + x16 + x12 +x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1.
 * Name of the polynomial is CCITT-32/CRC-32-IEEE 802.3.
 *
 * @note There is no parameter validation implemented.
 *
 * @param Crc_DataPtr  Pointer to the start address of the memory region.
 * @param Crc_Length  Size of memory to be used for calculation.
 * @param Crc_StartValue32  The start value of CRC.
 *
 * @return  CRC over a memory region.
 *
 */
extern uint32 Crc_CalculateCRC32(const uint8* Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32);



/** The function Crc_GetVersionInfo returns the version information of the Crc module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Crc_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter CrcVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Crc_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = CRC_MODULE_ID;
	versioninfo->sw_major_version = CRC_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = CRC_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = CRC_SW_PATCH_VERSION;
	versioninfo->vendorID = CRC_VENDOR_ID;
}

/*@}*/

#define CRC_STOP_SEC_CODE
#include "MemMap.h"
#endif /* CRC_H_ */
