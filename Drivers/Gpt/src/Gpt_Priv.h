/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Gpt_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:17:24MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Gpt_Priv.h  $
 *  Revision 1.1 2010/05/19 14:17:24MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Gpt/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef GPT_PRIV_H_
#define GPT_PRIV_H_

#include "team-s.h"
#include "Gpt_Cfg.h"

#if defined(GPT_DEV_ERROR_DETECT) && (GPT_DEV_ERROR_DETECT == STD_ON)
#  define GPT_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, GPT_MODULE_ID, GPT_INSTANCE_ID, api_id, error_id)
#  define GPT_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, GPT_MODULE_ID, GPT_INSTANCE_ID, api_id, error_id)
#  define GPT_ASSERT_RETVAL(cond, api_id, error_id, ret_val) TEAMS_ASSERT_RETVAL(cond, GPT_MODULE_ID, GPT_INSTANCE_ID, api_id, error_id, ret_val)
#else
#  define GPT_ASSERT_RETERR(cond, api_id, error_id)
#  define GPT_ASSERT_RETVOID(cond, api_id, error_id)
#  define GPT_ASSERT_RETVAL(cond, api_id, error_id, ret_val)
#endif


#endif /* GPT_PRIV_H_ */


