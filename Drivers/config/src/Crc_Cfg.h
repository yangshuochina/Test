/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Crc_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:59:27MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Crc_Cfg.h  $
 *  Revision 1.3 2010/11/22 08:59:27MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:42:44MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:28:59MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#ifndef CRC_CFG_H_
#define CRC_CFG_H_

#include "Std_Types.h"

/** @defgroup group_crc_cfg Crc Configuration Parameters
 *  @ingroup group_crc */
/*@{*/
#define CRC_INSTANCE_ID		(0x0)

#define CRC_8_TABLE     (0x01) /**< Lookup Table method for CRC8 calculation. */
#define CRC_8_RUNTIME   (0x02) /**< CRC calculation at the runtime. */
#define CRC_8_HARDWARE  (0x03) /**< Re directed to #CRC_8_RUNTIME. */

#define CRC_16_TABLE    (0x04) /**< Lookup Table method for CRC16 calculation. */
#define CRC_16_RUNTIME  (0x05) /**< CRC calculation at the runtime. */
#define CRC_16_HARDWARE (0x06) /**< Re directed to #CRC_16_RUNTIME. */

#define CRC_32_TABLE    (0x07) /**< Lookup Table method for CRC16 calculation. */
#define CRC_32_RUNTIME  (0x08) /**< CRC calculation at the runtime. */
#define CRC_32_HARDWARE (0x09) /**< Re directed to #CRC_32_RUNTIME. */

#define CRC_8_MODE   CRC_8_TABLE   /**< Configuration of CRC8 calculation. */
#define CRC_16_MODE  CRC_16_HARDWARE  /**< Configuration of CRC16 calculation. */
#define CRC_32_MODE  CRC_32_HARDWARE  /**< Configuration of CRC32 calculation.*/

#define CRC_INITIAL_VALUE8   (0x00)        /**< Start value for CRC8 calculation. */
#define CRC_INITIAL_VALUE16  (0xFFFF)      /**< Start value for CRC16 calculation. */
#define CRC_INITIAL_VALUE32  (0xFFFFFFFF)  /**< Start value for CRC32 calculation. */


/*@}*/

#endif /* CRC_CFG_H_ */
