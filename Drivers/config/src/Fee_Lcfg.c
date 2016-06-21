/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fee_Lcfg.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/04 11:36:22MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fee_Lcfg.c  $
 *  Revision 1.4 2011/03/04 11:36:22MEZ Cotor Vlad RGB (COTORV) 
 *  - Update NvM due to CTC and DEH variables
 *  Revision 1.3 2011/02/28 10:09:42MEZ Cotor Vlad RGB (COTORV)
 *  - Develop shutdown function for xCU power control
 *  Revision 1.2 2010/07/12 13:50:12MESZ Cotor Vlad RGB (COTORV)
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "Fee.h"
#include "Fee_Cbk.h"


#define FEE_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"


/* A block contains the data that is written with the provided interface. A block
 * is always aligned to a page boundary. This also means that a block is always
 * a multiple of 128 bytes.
 *
 * ATTENTION: The Fee configuration MUST BE THE SAME with the configuration contain in NvM_Lcfg.h !!!
 *
 * ------------------------------
 * Fee_Lcfg.h   | NvM_Lcfg.h
 * ------------------------------
 * block number = block#
 * block size   = block_size
 * ------------------------------
 */
const Fee_BlockConfiguration fee_block_configuration[] = {
		/* block number, block size, immediate data */
		{  0,             84,        0 },
		{  1,            512,        0 },
		{  5,             32,        0 },
		{  6,              4,        0 },
		{  7,              4,        0 },
	   {  8,              4,        0 },
	   {  9,              4,        0 },
	   { 10,              4,        0 },
	   { 11,              1,        0 },
	   { 12,              1,        0 },
};


#define FEE_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"


#define FEE_START_SEC_CODE
#include "MemMap.h"

void Fee_JobEndNotification(void)
{
}

void Fee_JobErrorNotification(void)
{
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"
