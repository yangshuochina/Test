/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: RamTst_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:15:05MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: RamTst_Cfg.h  $
 *  Revision 1.2 2010/07/12 14:15:05MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef RAMTST_CFG_H_
#define RAMTST_CFG_H_

#include "team-s.h"

/* Definition to include the algorithm*/
#define RAMTST_CHECKERBOARD_TEST_SELECTED	(STD_ON)
#define RAMTST_MARCH_TEST_SELECTED			(STD_ON)
#define RAMTST_WALK_PATH_TEST_SELECTED		(STD_ON)
#define RAMTST_GALPAT_TEST_SELECTED			(STD_ON)
#define RAMTST_TRANSP_GALPAT_TEST_SELECTED	(STD_OFF)
#define RAMTST_ABRAHAM_TEST_SELECTED		(STD_OFF)

#define RAMTST_INSTANCE_ID					(0)
/* Definition to enable the DET */
#define RAMTST_DEV_ERROR_DETECT				(STD_OFF)

#define RAMTST_NUMBER_OF_BLOCKS				(3) /*Specify the max number of blocks out of all algo config*/

/* API SELECTION */

#define RAMTST_STOP_API                          STD_ON
#define RAMTST_ALLOW_API                         STD_ON
#define RAMTST_GETEXECUTIONSTATUS_API            STD_ON
#define RAMTST_GETTESTRESULT_API                 STD_ON
#define RAMTST_GETTESTRESULTPERBLOCK_API         STD_ON
#define RAMTST_GETTESTALGORITHM_API              STD_ON
#define RAMTST_CHANGENUMBEROFTESTEDCELLS_API     STD_ON
#define RAMTST_CHANGEMAXNUMBEROFTESTEDCELLS_API  STD_ON
#define RAMTST_GETNUMBEROFTESTEDCELLS_API        STD_ON
#define RAMTST_GETVERSIONINFO_API                STD_ON
#define RAMTST_CHANGETESTALGORITHM_API           STD_ON
#define RAMTST_SUSPEND_API                       STD_ON
#define RAMTST_RESUME_API                        STD_ON


#endif /* RAMTST_CFG_H_ */
