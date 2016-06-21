/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Fls_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:16:41MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Fls_Priv.h  $
 *  Revision 1.1 2010/05/19 14:16:41MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Fls/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef FLS_PRIV_H_
#define FLS_PRIV_H_

#include "team-s.h"
#include "Fls_Cfg.h"

#define FLS_PAGE_SIZE_DATA_FLASH (128 / 4)
#define FLS_PAGE_SIZE_PROG_FLASH (256 / 4)

#define FLS_START_PFLASH  ((uint32 *)0xA0000000u)
#define FLS_END_PFLASH    ((uint32 *)0xA0200000u)
#define FLS_START_DFLASH0 ((uint32 *)0xAFE00000u)
#define FLS_END_DFLASH0   ((uint32 *)0xAFE08000u)
#define FLS_START_DFLASH1 ((uint32 *)0xAFE10000u)
#define FLS_END_DFLASH1   ((uint32 *)0xAFE18000u)



#if defined(FLS_DEV_ERROR_DETECT) && (FLS_DEV_ERROR_DETECT == STD_ON)
#  define FLS_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, FLS_MODULE_ID, FLS_INSTANCE_ID, api_id, error_id)
#  define FLS_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, FLS_MODULE_ID, FLS_INSTANCE_ID, api_id, error_id)
#  define FLS_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, FLS_MODULE_ID, FLS_INSTANCE_ID, api_id, error_id)
#else
#  define FLS_ASSERT_RETERR(cond, api_id, error_id)
#  define FLS_ASSERT_RETVOID(cond, api_id, error_id)
#  define FLS_ASSERT(cond, api_id, error_id)
#endif


#define FLS_START_SEC_VAR_32BIT
#include "MemMap.h"

extern const Fls_ConfigType *fls_cfg;

#define FLS_STOP_SEC_VAR_32BIT
#include "MemMap.h"

#define FLS_START_SEC_CODE
#include "MemMap.h"

extern void Fls_AccFinish(uint32 *addr);
extern void Fls_AccEraseSector(uint32 *addr, uint32 page_size);
extern void Fls_AccWritePage(uint32 *addr, const uint32 *data, uint32 page_size);
extern void Fls_AccCopyEraseToRam(uint32 *ram_addr);
extern void Fls_AccClearEraseFromRam(uint32 *ram_addr);
extern void Fls_AccCopyWriteToRam(uint32 *ram_addr);
extern void Fls_AccClearWriteFromRam(uint32 *ram_addr);


#define FLS_STOP_SEC_CODE
#include "MemMap.h"


#endif /* FLS_PRIV_H_ */


