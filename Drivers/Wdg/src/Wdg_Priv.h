/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Wdg_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/11/22 08:55:41MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Wdg_Priv.h  $
 *  Revision 1.3 2010/11/22 08:55:41MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/11/22 08:47:55MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:23:38MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Wdg/AA000/src/project.pj
 *
 * ***************************************************************************/

/** @ingroup group_wdg */

#ifndef WDG_PRIV_H_
#define WDG_PRIV_H_

#include "team-s.h"
#include "Wdg_Cfg.h"


#if defined(WDG_DEV_ERROR_DETECT) && (WDG_DEV_ERROR_DETECT == STD_ON)
#  define WDG_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, WDG_MODULE_ID, WDG_INSTANCE_ID, api_id, error_id)
#  define WDG_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, WDG_MODULE_ID, WDG_INSTANCE_ID, api_id, error_id)
#  define WDG_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, WDG_MODULE_ID, WDG_INSTANCE_ID, api_id, error_id)
#  define WDG_UPDATE_STATUS(st) wdg_state =(st)
#else
#  define WDG_ASSERT_RETERR(cond, api_id, error_id)
#  define WDG_ASSERT_RETVOID(cond, api_id, error_id)
#  define WDG_ASSERT(cond, api_id, error_id)
#  define WDG_UPDATE_STATUS(st)
#endif

#define WDG_SETTINGS_SLOW (0x01U)
#define WDG_SETTINGS_FAST (0x05U)
#define WDG_SETTINGS_OFF  (0x09U)

#endif /* WDG_PRIV_H_ */


