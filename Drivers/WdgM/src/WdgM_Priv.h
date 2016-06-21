/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgM_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:18:45MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgM_Priv.h  $
 *  Revision 1.3 2011/02/02 09:18:45MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.2 2010/07/12 14:42:04MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/
/* $Id: WdgM_Priv.h 1.3 2011/02/02 09:18:45MEZ Cotor Vlad RGB (COTORV) Exp  $ */

#ifndef WDGM_PRIV_H_
#define WDGM_PRIV_H_

#include "team-s.h"
#include "WdgM_Cfg.h"

#if defined(WDGM_DEV_ERROR_DETECT) && (WDGM_DEV_ERROR_DETECT == STD_ON)
#  define WDGM_ASSERT_RETERR(cond, api_id, error_id)      TEAMS_ASSERT_RETERR(cond, WDGM_MODULE_ID, WDGM_INSTANCE_ID, api_id, error_id)
#  define WDGM_ASSERT_RETVOID(cond, api_id, error_id)     TEAMS_ASSERT_RETVOID(cond, WDGM_MODULE_ID, WDGM_INSTANCE_ID, api_id, error_id)
#  define WDGM_ASSERT(cond, api_id, error_id)             TEAMS_ASSERT(cond, WDGM_MODULE_ID, WDGM_INSTANCE_ID, api_id, error_id)
#  define WDGM_ASSERT_REPORTONLY(cond, api_id, error_id)  TEAMS_ASSERT_REPORTONLY(cond, WDGM_MODULE_ID, WDGM_INSTANCE_ID, api_id, error_id)
#else
#  define WDGM_ASSERT_RETERR(cond, api_id, error_id)
#  define WDGM_ASSERT_RETVOID(cond, api_id, error_id)
#  define WDGM_ASSERT(cond, api_id, error_id)
#  define WDGM_ASSERT_REPORTONLY(cond, api_id, error_id)
#endif

typedef struct {
		WdgM_ModeType CurrentMode; /**< Current mode of the WDGM. */
		uint16 wdg_TriggerCycles[WDGM_NUMBER_OF_WATCHDOGS_INSTANCES]; /**< number of cycles by which WDG re trigger should occur. */

		#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)
		uint8 CurExpCycTolerance;    /**< Entities status array. */
		WdgM_AliveSupervisionStatusType GlobalStatus ; /**< Global WDGM Status. */
		WdgM_AliveSupervisionStatusType SEStatus[WDGM_NUMBER_OF_SUPERVISED_ENTITIES]; /**< individual SE Status. */
		uint16 SEIndications[WDGM_NUMBER_OF_SUPERVISED_ENTITIES]; /**< SE indication counter incremented at every alive indication call. */
		uint16 SERefCycles[WDGM_NUMBER_OF_SUPERVISED_ENTITIES]; /**< The reference cycles passed. */
		uint16 SEExpCycles[WDGM_NUMBER_OF_SUPERVISED_ENTITIES]; /**< The number of expired cycles allowed before declare as EXPIRED.*/

		#endif
} WdgM_Container;

#define WDGM_START_SEC_VAR_32BIT
#include "MemMap.h"
extern WdgM_Container wdgm; /* WdgM driver variable container */
extern const WdgM_ConfigType*  wdgm_cfg; /* WDGM  config pointer */

#define WDGM_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#if defined(WDGM_ALIVE_SUPERVISION) && (WDGM_ALIVE_SUPERVISION == STD_ON)
extern void WDGM_AliveSupervisionAlgorithm (void);
#endif


#endif /* WDGM_PRIV_H_ */


