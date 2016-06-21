/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: Spi_Cfg.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2010/12/13 08:38:20MEZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: Spi_Cfg.h  $
 * Revision 1.12 2010/12/13 08:38:20MEZ Cotor Vlad RGB (COTORV) 
 * - Correction of daisy chain SPI communication of TLE8104E chip
 * Revision 1.11 2010/11/29 13:43:34MEZ Cotor Vlad RGB (COTORV) 
 * Implement and integrate complex device driver for electrical hazard protection
 * Revision 1.10 2010/11/25 08:10:52MEZ Cotor Vlad RGB (COTORV) 
 * Update complex device driver for real time clock due to specification update
 * Revision 1.9 2010/11/22 09:01:13MEZ Cotor Vlad RGB (COTORV) 
 * Update BSW software modules with doxygen documentation comments
 * Revision 1.8 2010/10/14 18:06:31MESZ Cotor Vlad RGB (COTORV) 
 * Update complex device driver for real time clock due to changed SPI configuration.
 * Revision 1.7 2010/10/08 18:48:57MESZ Cotor Vlad RGB (COTORV) 
 * Update of the baud rate for all SPI jobs
 * Revision 1.5 2010/10/07 13:39:29MESZ Cotor Vlad RGB (COTORV) 
 * Implementation and integration of complex device driver for real time clock
 * Update SPI sequence's names due to integration of complex device driver for real time clock
 * Revision 1.4 2010/09/24 17:26:36MESZ Cotor Vlad RGB (COTORV) 
 * Update scheduling behavior and add deactivation of programmable gain to extend measurement range
 * Reorganize the SPI Configuration switching to macros
 * Revision 1.3 2010/09/09 16:13:43MESZ Cotor Vlad RGB (COTORV) 
 * Update of SPI Configuration due to:
 * SPI_MAX_CHANNEL, SPI_MAX_JOB and SPI_MAX_SEQUENCE
 * Revision 1.2 2010/07/30 16:31:16MESZ Cotor Vlad RGB (COTORV) 
 * Update BSW for contactor control complex device driver due to specification change
 * Revision 1.1 2010/05/20 11:29:05MESZ Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 * Revision 1.1 2010/03/08 14:06:24MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/config/src/project.pj
 * *****************************************************************************/


#ifndef SPI_CFG_H_
#define SPI_CFG_H_

#include "Std_Types.h"

/** @defgroup group_spi_cfg Spi Configuration Parameters
 *  @ingroup group_spi */
/*@{*/

#define SPI_INSTANCE_ID				0

#define SPI_DEV_ERROR_DETECT          STD_ON    /**< Enable/disable the development support. */
#define SPI_CHANNEL_BUFFERS_ALLOWED   STD_ON    /**< Enable/disable support for external buffers. */
#define SPI_LEVEL_DELIVERED           2         /**< Level x interfaces are supported. */
#define SPI_CANCEL_API                STD_OFF   /**< Support for the cancel API. */
#define SPI_HWUNITSTATUS_API          STD_ON    /**< Enable/disable of the service Spi_GetHWUnitStatus(). */

#define SPI_MAX_CHANNEL               92        /**< The maximum number of channels that can be configured. */
#define SPI_MAX_JOB                   56        /**< The maximum number of jobs that can be configured. */
#define SPI_MAX_SEQUENCE              23        /**< The maximum number of sequences that can be configured. */
#define SPI_MAX_UNIT                  2         /**< The maximum number of units that can be configured. */



extern uint16 rx_buffer[];
extern uint16 tx_buffer[];




/*@}*/

#endif /* SPI_CFG_H_ */

