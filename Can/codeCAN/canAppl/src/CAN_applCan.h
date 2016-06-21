/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: CAN_applCan.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   31.03.2010
 *  LAST REVISION:      $Date: 2010/08/23 09:29:42MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: CAN_applCan.h  $
 *  Revision 1.6 2010/08/23 09:29:42MESZ Cotor Vlad RGB (COTORV) 
 *  Implement, reorganize and update the BCU communication modules
 *  Revision 1.5 2010/08/18 14:36:06MESZ Cotor Vlad RGB (COTORV) 
 *  Update Can application due to specification and DBC file update
 *  Revision 1.4 2010/07/19 10:45:35MESZ Cotor Vlad RGB (COTORV)
 *  Update CAN implementation due ti implementation of a 1ms function call
 *  Revision 1.3 2010/07/12 09:41:16MESZ Cotor Vlad RGB (COTORV)
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.2 2010/07/02 07:18:35MESZ Cotor Vlad RGB (COTORV)
 *  Integration of BCU communication
 *  Revision 1.1 2010/05/20 14:19:21MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Can/codeCAN/canAppl/src/project.pj
 *
 * ***************************************************************************/

#ifndef CAN_APPLCAN_H_
#define CAN_APPLCAN_H_

/* ========== Includes ================================================ */
#include "Std_Types.h"     /* Contains the basic data types. */
#include "Prj_MainCfg.h"


/* ========== Declare Alternate Control EXTERNAL_ definition ========= */
#ifdef  MASTER_CAN_APPLCAN_C_  /* Read from associated modul ?    */
   #define  EXTERNAL_           /* yes: associated   */
#else
   #define  EXTERNAL_  extern   /* no: extern modul  */
#endif

/* parasoft suppress item MISRA2004-8_5 reason "Master-define mechanism prevents multiple definition of functions and objects in header files" */

/* ========== Enumerations, Typedefinitions =========================== */



/* ========== Global variables  ======================================= */



/* ========== Global macros =========================================== */



/* ========== Global function prototypes  ============================= */
extern C_CALLBACK_1 void C_CALLBACK_2 ApplCanBusOff(CAN_CHANNEL_CANTYPE_ONLY);
extern C_CALLBACK_1 void C_CALLBACK_2 ApplCanWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
extern void    ApplCanFullCanOverrun(uint8 channel);
extern void    ApplCanOverrun(uint8 channel);
/* ==================================================================== */
#define CAN_START_SEC_CODE_INIT
#include "MemMap.h"
   void ApplCanInitPowerOn(void);
#define CAN_STOP_SEC_CODE_INIT
#include "MemMap.h"
/* ==================================================================== */
#define CAN_START_SEC_CODE_1MS
#include "MemMap.h"
   void ApplCanTask_1ms(void);
#define CAN_STOP_SEC_CODE_1MS
#include "MemMap.h"
/* ==================================================================== */
#define CAN_START_SEC_CODE_10MS
#include "MemMap.h"
   void ApplCanTask_10ms(void);
#define CAN_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ==================================================================== */
#define CAN_START_SEC_CODE_100MS
#include "MemMap.h"
   void ApplCanTask_100ms(void);
#define CAN_STOP_SEC_CODE_100MS
#include "MemMap.h"
/* ==================================================================== */


/* ========== End of Global variables (End of header function) ======== */

/* parasoft unsuppress item MISRA2004-8_5 */

#ifdef  EXTERNAL_
#undef  EXTERNAL_
#endif


#endif    /* CAN_APPLCAN_H_ */



