/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BRTC_RealTiClkDrvr_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   20.09.2010
 *  LAST REVISION:      $Date: 2012/08/14 16:02:19CEST $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BRTC_RealTiClkDrvr_Cfg.h  $
 *  Revision 1.2 2012/08/14 16:02:19CEST Cotor Vlad RGB (COTORV) 
 *  - Initial revision
 *  Revision 1.1 2012/08/14 15:59:32MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/CplxDrvr/Cfg/src/project.pj
 *
 * ***************************************************************************/

#ifndef BRTC_REALTICLKDRVR_CFG_H_
#define BRTC_REALTICLKDRVR_CFG_H_

/** @defgroup group_brtc_config BRTC Configuration
 *  @ingroup group_brtc
 *
 * This module contains the configuration of the BRTC - Basic software for real time clock complex device driver.
 */
/*@{*/


/* ========== Includes ===================================================== */



/* ========== Enumerations, Typedefinitions ================================ */



/* ========== Global variables ============================================= */



/* ========== Global macros ================================================ */
#define PCA21125_NOADDR_READ                      (7u)

#define Chip_PCA21125_RegAddr_00                (0x00u)
#define Chip_PCA21125_RegAddr_01                (0x01u)
#define Chip_PCA21125_RegAddr_02                (0x02u)
#define Chip_PCA21125_RegAddr_03                (0x03u)
#define Chip_PCA21125_RegAddr_04                (0x04u)
#define Chip_PCA21125_RegAddr_05                (0x05u)
#define Chip_PCA21125_RegAddr_06                (0x06u)
#define Chip_PCA21125_RegAddr_07                (0x07u)
#define Chip_PCA21125_RegAddr_08                (0x08u)


/* ========== Global function prototypes =================================== */



/* ========== End of Global variables (End of header function) ============= */

/*@}*/

#endif    /* BRTC_REALTICLKDRVR_CFG_H_ */
