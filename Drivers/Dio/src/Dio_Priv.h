/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Dio_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:15:53MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Dio_Priv.h  $
 *  Revision 1.1 2010/05/19 14:15:53MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Dio/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef DIO_PRIV_H_
#define DIO_PRIV_H_

#include "team-s.h"
#include "Dio_Cfg.h"


#if defined(DIO_DEV_ERROR_DETECT) && (DIO_DEV_ERROR_DETECT == STD_ON)
#  define DIO_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, DIO_MODULE_ID, DIO_INSTANCE_ID, api_id, error_id)
#  define DIO_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, DIO_MODULE_ID, DIO_INSTANCE_ID, api_id, error_id)
#  define DIO_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, DIO_MODULE_ID, DIO_INSTANCE_ID, api_id, error_id)
#else
#  define DIO_ASSERT_RETERR(cond, api_id, error_id)
#  define DIO_ASSERT_RETVOID(cond, api_id, error_id)
#  define DIO_ASSERT(cond, api_id, error_id)
#endif


#endif /* DIO_PRIV_H_ */
