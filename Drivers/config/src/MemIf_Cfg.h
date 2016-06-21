/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: MemIf_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:00:38MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: MemIf_Cfg.h  $
 *  Revision 1.3 2010/11/22 09:00:38MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:43:32MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:29:02MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

/** @ingroup group_memif */

#ifndef MEMIF_CFG_H_
#define MEMIF_CFG_H_

#include "Std_Types.h"
#include "MemIf_Types.h"


/** @defgroup group_memif_cfg MemIf Configuration Parameters */
/*@{*/
/*
 * Configuration of the subsystem to use. Set either
 * MEMIF_USE_FEE or MEMIF_USE_EA or both of them.
 */
#define MEMIF_USE_EA	/**< Configuration of FEE subsystem. */
#undef  MEMIF_USE_FEE	/**< Configuration of EA subsystem. */

/*@}*/

#ifdef MEMIF_USE_FEE
#include "Fee.h"
#endif

#ifdef MEMIF_USE_EA
#include "Ea.h"
#endif

#define MEMIF_DEV_ERROR_DETECT        STD_ON

#define MEMIF_INSTANCE_ID				0



#endif /* MEMIF_CFG_H_ */

