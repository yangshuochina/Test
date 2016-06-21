/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:36:37MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_Priv.h  $
 *  Revision 1.3 2010/11/22 08:36:37MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:31:46MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef SCHM_PRIV_H_
#define SCHM_PRIV_H_

#include "team-s.h"
#include "SchM_Cfg.h"

/** @ingroup group_schm_priv */

#if defined(SCHM_DEV_ERROR_DETECT) && (SCHM_DEV_ERROR_DETECT == STD_ON)
#  define SCHM_ASSERT_RETERR(cond, instance_id, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, SCHM_MODULE_ID, instance_id, api_id, error_id)
#  define SCHM_ASSERT_RETVOID(cond, instance_id, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, SCHM_MODULE_ID, instance_id, api_id, error_id)
#  define SCHM_ASSERT_RETVAL(cond, instance_id, api_id, error_id, ret_val) TEAMS_ASSERT_RETVAL(cond, SCHM_MODULE_ID, instance_id, api_id, error_id, ret_val)
#else
#  define SCHM_ASSERT_RETERR(cond, instance_id, api_id, error_id)
#  define SCHM_ASSERT_RETVOID(cond, instance_id, api_id, error_id)
#  define SCHM_ASSERT_RETVAL(cond, instance_id, api_id, error_id, ret_val)
#endif

#endif /* SCHM_PRIV_H_ */
