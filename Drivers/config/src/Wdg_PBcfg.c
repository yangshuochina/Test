/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Wdg_PBcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:44:26MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Wdg_PBcfg.c  $
 *  Revision 1.2 2010/07/12 09:44:26MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:29:06MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#include "Wdg.h"

#define WDG_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

const Wdg_Int_ConfigType int_wdg_config = {
	WDGIF_SLOW_MODE,           /**< Initial WDG mode configuration. */
	0xA0,                    /**< Initial WDG password. */
	0x2710,                   /**< WDG timer timeout value in ticks*/
};

/** This is the configuration structure of Watchdog driver */
const Wdg_ConfigType wdg_config = {
	&int_wdg_config,       /**< Internal watchdog configuration. */
	NULL,                  /**< External watchdog configuration. */
};


#define WDG_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
