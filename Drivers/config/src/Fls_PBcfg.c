/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fls_PBcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/07/12 14:00:48MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fls_PBcfg.c  $
 *  Revision 1.2 2010/07/12 14:00:48MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/
 
#include "Fls.h"


#define FLS_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

const Fls_ConfigType fls_config = {
		(uint32 *)0xAFE00000u,		/* Base address of the flash memory */
		100,		/* Number of STM ticks in slow mode. */
		1000,		/* Number of STM ticks in fast mode. */
		NULL_PTR,	/* Base address where to copy the RAM image */
		NULL_PTR,	/* The function that gets called upon completion of a job */
		NULL_PTR	/* The function that gets called in case of an error */
};


#define FLS_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"
