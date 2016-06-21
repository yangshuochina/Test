/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: MemIf_Types.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:31:08MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: MemIf_Types.h  $
 *  Revision 1.2 2010/11/22 08:31:08MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:07:14MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/EcuAbstractionLayer/MemIf/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef MEMIF_TYPES_H_
#define MEMIF_TYPES_H_

#include "team-s.h"
#include "Std_Types.h"

/** @defgroup group_memif MemIf
 *  @ingroup group_memif */
/*@{*/

#define MEMIF_MODULE_ID         (22)
#define MEMIF_VENDOR_ID         (TEAMS_VENDOR_ID)
#define MEMIF_AR_MAJOR_VERSION  (2)
#define MEMIF_AR_MINOR_VERSION  (2)
#define MEMIF_AR_PATCH_VERSION  (2)
#define MEMIF_SW_MAJOR_VERSION  (1)
#define MEMIF_SW_MINOR_VERSION  (0)
#define MEMIF_SW_PATCH_VERSION  (0)

/** Status of the device driver.
 *
 * This type denotes the current status of the underlying abstraction
 * module and device driver.
 */
typedef enum {
	MEMIF_IDLE = 0,				/**< The underlying abstraction module or device driver is currently idle. */
	MEMIF_BUSY_INTERNAL = 1,	/**< The underlying abstraction module is busy with internal management operations; the underlying device driver can be busy or idle. */
	MEMIF_BUSY = 3,				/**< The underlying abstraction module or device driver is currently busy. */
	MEMIF_UNINIT = 7,			/**< The underlying abstraction module or device driver has not been initialized (yet). */
} MemIf_StatusType;


/** Status of the last job.
 *
 * Denotes the result of the last job.
 */
typedef enum {
	MEMIF_JOB_OK,			/**< The job has been finished successfully. */
	MEMIF_JOB_FAILED,		/**< The job has not been finished successfully. */
	MEMIF_JOB_PENDING,		/**< The job has not yet been finished. */
	MEMIF_JOB_CANCELLED,	/**< The job has been canceled. */
	MEMIF_BLOCK_INCONSISTENT, /**< The requested block is inconsistent, it may contain corrupted data. */
	MEMIF_BLOCK_INVALID,	/**< The requested block has been marked as invalid, the requested operation can not be performed. */
} MemIf_JobResultType;

/** Mode of the device driver.
 *
 * Denotes the operation mode of the underlying abstraction
 * modules and device drivers.
 */
typedef enum {
	MEMIF_MODE_SLOW,		/**< The underlying memory abstraction modules and drivers are working in slow mode. */
	MEMIF_MODE_FAST,		/**< The underlying memory abstraction modules and drivers are working in fast mode. */
} MemIf_ModeType;



/*@}*/

#endif /* MEMIF_TYPES_H_ */
