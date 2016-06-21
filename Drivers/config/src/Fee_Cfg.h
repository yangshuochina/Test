/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fee_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:59:58MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fee_Cfg.h  $
 *  Revision 1.3 2010/11/22 08:59:58MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 12:55:43MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef FEE_CFG_H_
#define FEE_CFG_H_

#include "Std_Types.h"
#include "MemIf_Types.h"


#define FEE_DEV_ERROR_DETECT        STD_ON

#define FEE_INSTANCE_ID				0


/** @defgroup group_fee_cfg Fee Configuration Parameters
 *  @ingroup group_fee*/
/*@{*/

#define FEE_VIRTUAL_PAGE_SIZE		128		/**< Number of bytes a page has; do not change. */
#define FEE_NUMBER_OF_WRITE_CYCLES  100000  /**< Number of write cycles that shall be guaranteed; could be used to calculate FEE_NUM_OF_GENERATIONS. */
#define FEE_NUM_OF_IMMEDIATE_BLOCKS	4		/**< Number of blocks that require an immediate write access. */
#define FEE_NUM_OF_MGMT_BLOCKS		50		/**< Number of mgmt. blocks; each write occupies one mgmt. block. */
#define FEE_NUM_OF_GENERATIONS      4		/**< Number of generations a segment shall be able to hold. */
#define FEE_NUM_OF_PAGES_IN_GENERATION  ((255 - FEE_NUM_OF_MGMT_BLOCKS) / FEE_NUM_OF_GENERATIONS)	/**< Number of pages in a single generation. */

/*@}*/

#endif /* FEE_CFG_H_ */

