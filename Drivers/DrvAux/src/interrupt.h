/* ******************************************************************************
 *
 * COPYRIGHT:  $Company: AVL Software and Functions GmbH $
 * FILENAME:   $Source: interrupt.h $
 * COMPILER:   TaskingCTC v3.3r1
 * PROCESSOR:  TC1767
 *
 * DATE OF CREATION:  2010/02/17
 * LAST REVISION:     $Date: 2011/02/24 12:43:11MEZ $
 *                    $Author: Cotor Vlad RGB (COTORV) $
 *
 * ==============================================================================
 * $Log: interrupt.h  $
 * Revision 1.4 2011/02/24 12:43:11MEZ Cotor Vlad RGB (COTORV) 
 * - Develop shutdown function for xCU power control
 * Revision 1.3 2010/11/22 07:56:50MEZ Cotor Vlad RGB (COTORV) 
 * Update BSW software modules with doxygen documentation comments
 * Revision 1.2 2010/05/20 11:21:23MESZ Cotor Vlad RGB (COTORV) 
 * Member moved from interrupt.h in project /PSL/Bsw/Misc/DrvAux/src/project.pj to interrupt.h in project /PSL/Bsw/Misc/DrvAux/AA000/src/project.pj.
 * Revision 1.1 2010/05/20 10:46:17CEST Cotor Vlad RGB (COTORV) 
 * Initial revision
 * Member added to project /PSL/Bsw/Misc/DrvAux/src/project.pj
 * Revision 1.1 2010/03/08 14:06:36MEZ Cotor Vlad RGB (COTORV)
 * Initial revision
 * Member added to project /Controls/29015/ARE9018/autosar/driver/src/project.pj
 * *****************************************************************************/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "Std_Types.h"

/** @defgroup group_driver_misc BSW driver miscellaneous
 *  @ingroup group_driver_misc */


/** @defgroup group_int_cfg Interrupt Level Configuration
 *  @ingroup group_driver_misc */
/*@{*/

/* interrupt levels */
#define SRPN_SSC0_RSRC     10    /**< Interrupt Level SSC0 used by @ref spi.  */
#define SRPN_SSC1_RSRC     11    /**< Interrupt Level SSC1 used by @ref spi.  */
#define SRPN_DMA_SRC0      12    /**< Interrupt Level DMA0 used by @ref adc.  */
#define SRPN_DMA_SRC2      13    /**< Interrupt Level DMA2 used by @ref adc.  */
#define SRPN_DMA_SRC1      14    /**< Interrupt Level DMA1 used by @ref adc.  */
#define SRPN_DMA_SRC3      15    /**< Interrupt Level DMA3 used by @ref adc.  */
#define SRPN_GPTA0_SRC07   16    /**< Interrupt Level GTC7 used by @ref gpta. */
#define SRPN_GPTA0_SRC08   17    /**< Interrupt Level GTC8 used by @ref gpta. */


/*@}*/

#endif /* INTERRUPT_H_ */

