/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: NvM_Types.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:32:11MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: NvM_Types.h  $
 *  Revision 1.3 2010/11/22 08:32:11MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:26:49MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef NVM_TYPES_H_
#define NVM_TYPES_H_

#include "team-s.h"
#include "Std_Types.h"
#include "MemIf_Types.h"

/** @defgroup group_nvm_types NvM_Types 
 *  @ingroup group_nvm*/
/*@{*/

#define NVM_REQ_OK 				((NvM_RequestResultType)0)  /**< The last asynchronous read/write/control request has been finished successfully. */
#define NVM_REQ_NOT_OK 			((NvM_RequestResultType)1) 	/**< The last asynchronous read/write/control request has been finished unsuccessfully.*/
#define NVM_REQ_PENDING 		((NvM_RequestResultType)2) 	/**< An asynchronous read/write/control request is currently pending. */
#define NVM_REQ_INTEGRITY_FAILED ((NvM_RequestResultType)3) /**< The result of the last asynchronous request NvM_ReadBlock() or NvM_ReadAll() had a data integrity failure. */
#define NVM_REQ_BLOCK_SKIPPED  	((NvM_RequestResultType)4)  /**< The referenced block was skipped during execution of NvM_ReadAll() or NvM_WriteAll(). */
#define NVM_REQ_NV_INVALIDATED 	((NvM_RequestResultType)5)  /**< The referenced NV block is invalidated. */
#define NVM_REQ_CANCELLED 		((NvM_RequestResultType)6) 	/**< The multi block request NvM_WriteAll was cancelled by calling NvM_CancelWriteAll(). */


/** Type for status information.
 *
 * This result is returned by the API service NvM_GetErrorStatus().
 * The possible values are:
 * - #NVM_REQ_OK
 * - #NVM_REQ_NOT_OK
 * - #NVM_REQ_PENDING
 * - #NVM_REQ_INTEGRITY_FAILED
 * - #NVM_REQ_BLOCK_SKIPPED
 * - #NVM_REQ_NV_INVALIDATED
 * - #NVM_REQ_CANCELLED
 */
typedef uint8 NvM_RequestResultType;


/** Identification of a NVRAM block via a unique block identifier.
 *
 * The NVRAM block IDs are expected to be in a sequential order, i.e. the
 * NVRAM manager in not capable of handling non-sequential NVRAM
 * block IDs.
 *
 * The ID reflects the index to the configuration of #NvM_ConfigBlockType.
 */
typedef uint16 NvM_BlockIdType;

/*@}*/

#endif /* NVM_TYPES_H_ */
