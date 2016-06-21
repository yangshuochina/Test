/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: CAN_applCan.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   31.03.2010
 *  LAST REVISION:      $Date: 2011/03/02 17:53:35MEZ $
 *                      $Author: Opel Martin RGB (OPELM) $
 *
 * ***************************************************************************
 *
 *  $Log: CAN_applCan.c  $
 *  Revision 1.9 2011/03/02 17:53:35MEZ Opel Martin RGB (OPELM) 
 *  - update to Mathlib 2.1.0
 *  - generic Std_Types.h
 *  Revision 1.8 2010/09/01 17:25:25MESZ Cotor Vlad RGB (COTORV)
 *  Update the BCU communication modules due to naming convention
 *  Revision 1.7 2010/08/23 09:29:31MESZ Cotor Vlad RGB (COTORV)
 *  Implement, reorganize and update the BCU communication modules
 *  Revision 1.6 2010/08/18 14:35:16MESZ Cotor Vlad RGB (COTORV)
 *  Update Can application due to specification and DBC file update
 *  Revision 1.5 2010/07/19 10:45:26MESZ Cotor Vlad RGB (COTORV)
 *  Update CAN implementation due ti implementation of a 1ms function call
 *  Revision 1.4 2010/07/12 09:41:08MESZ Cotor Vlad RGB (COTORV)
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.3 2010/07/02 08:29:31MESZ Cotor Vlad RGB (COTORV)
 *  Integration of BCU communication
 *  Revision 1.2 2010/06/23 15:24:58MESZ Opel Martin RGB (OPELM)
 *  - parts of the new CAN configuration
 *  Revision 1.1 2010/05/20 14:19:21MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Can/codeCAN/canAppl/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_CAN_APPLCAN_C_              /* Master define for this module    */


/* ========== Includes ===================================================== */
#include "Std_Types.h"
#include "team-s.h"
#include "v_inc.h"
#include "il_inc.h"
#include "XcpProf.h"
#include "CAN_applCan.h"           /* module header file     */
#include "BcuCommunicationModuleController.h"



/* ========== Enumerations, Typedefinitions, Local Defines  ================ */



/* ========== Local Function Prototypes  =================================== */



/* ========== Local Variables  ============================================= */



/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */



/* ========== Global Functions ============================================= */

/******************************************************************************/
/*!
 * @fn void ApplCanInitPowerOn(void)
 *
 * @brief Initialization application of the CAN
 *
 * @details This function initialize the CAN and the INTERACTION LAYER. It contains:
 *
 * <b>CanInitPowerOn</b> - initializes the CAN Controller and the CAN Drivers internal variables.
 *
 * <b>CanOnline</b> - enables the CAN Driver's transmit path for all subsequent transmit requests
 * of CanTransmit(..).
 *
 * <b>CanTransmit</b> - checks if a transmit register in the CAN Controller is available. If so,
 * the transmit process is initiated in the CAN Controller and kCanTxOk is returned.
 * If not and if there is no transmit queue configured, the function call returns with
 * the error code kCanTxFailed or kCanTxPartOffline.
 *
 * <b>IlInitPowerOn</b> - This method initializes the Il_Vector on all channels. IlInit is called
 * for every channel.
 *
 * <b>IlRxStart</b> - This method enables the reception of messages. The transition "start" of
 * the Rx state machine is performed.
 *
 * <b>IlTxStart</b> - This method enables the transmission of messages and starts the transmission
 * of periodic messages. The transition "start" of the Tx state machine is performed.
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
#define CAN_START_SEC_CODE_INIT
#include "MemMap.h"
void ApplCanInitPowerOn(void)
{
   /* CAN driver */
   CanInitPowerOn();
   /* IL */

   CanOnline(CANChannel_0_XCP);
   CanTransmit(CanTxTxXCP);

   IlInitPowerOn();

   IlRxStart(CANChannel_0_XCP);
   IlTxStart(CANChannel_0_XCP);
   IlRxStart(CANChannel_1_PrivateCAN);
   IlTxStart(CANChannel_1_PrivateCAN);
}
#define CAN_STOP_SEC_CODE_INIT
#include "MemMap.h"


/******************************************************************************/
/*!
 * @fn void ApplCanTask_1ms(void)
 *
 * @brief
 *
 * @details
 *
 * Reccurence: 1ms
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
#define CAN_START_SEC_CODE_1MS
#include "MemMap.h"
void ApplCanTask_1ms(void)
{
}
#define CAN_STOP_SEC_CODE_1MS
#include "MemMap.h"


/******************************************************************************/
/*!
 * @fn void ApplCanTask_10ms(void)
 *
 * @brief preparation of the RX and TX Task
 *
 * @details This function is a scheduler which is called every 10ms. It contains:
 *
 * <b>IlRxTask</b> - Preparation of RX Task for a CAN channel. This method must be called periodically in the Rx task cycle time configured
 * in the generation tool.
 *
 * <b>IlTxTask</b> - Preparation of TX Task for a CAN channel. This method must be called periodically in the Tx task cycle time configured
 * in the generation tool.
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
#define CAN_START_SEC_CODE_10MS
#include "MemMap.h"
void ApplCanTask_10ms(void)
{
   IlRxTask(CANChannel_1_PrivateCAN);   /* Interaction layer task for Rx by CAN 1 */
   IlTxTask(CANChannel_1_PrivateCAN);   /* Interaction layer task for Tx by CAN 1 */
}
#define CAN_STOP_SEC_CODE_10MS
#include "MemMap.h"


/******************************************************************************/
/*!
 * @fn void ApplCanTask_100ms(void)
 *
 * @brief Scheduler called every 100ms.
 *
 * @details This function is a scheduler which is called every 100ms. It contains:
 *
 * Reccurence: 1ms
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
#define CAN_START_SEC_CODE_100MS
#include "MemMap.h"
void ApplCanTask_100ms(void)
{
	uint8 Can0DrSt = 0;
	uint8 Can1DrSt = 0;

 	Can0DrSt = CanGetStatus(0);
 	Can1DrSt = CanGetStatus(1);

 	if(Can0DrSt!=0x01)
 	{
 		ApplCanInitPowerOn();
 		XcpInit();
 		ApplXcpInit();
 	}

 	if(Can1DrSt!=0x01)
 	{
 		ApplCanInitPowerOn();
 		XcpInit();
 		ApplXcpInit();
 	}

	if(CAN_NCR0.U != 0x00000008)
	{
		CAN_NSR0.U = 0x00000018;
		CAN_NCR0.U = 0x00000008;

		BCOM_CanNCR0Ctr++;

		if(BCOM_CanNCR0Ctr > 255)
		{
			BCOM_CanNCR0Ctr = 0;
		}
	}

	if(CAN_NCR1.U != 0x00000008)
	{
		CAN_NSR1.U = 0x00000018;
		CAN_NCR1.U = 0x00000008;
	}
}
#define CAN_STOP_SEC_CODE_100MS
#include "MemMap.h"


/******************************************************************************/
/*!
 * @fn void ApplCanFullCanOverrun(uint8 channel)
 *
 * @brief Overrun Diagnostic Callbacks
 *
 * @details
 *
 * Reccurence: on Reception of McInit frame
 *
 * @param rcvObject
 *
 * @return void
 */
/*****************************************************************************/
void ApplCanFullCanOverrun(uint8 channel)
{
   BCOM_FullCanOverrunCtr++;
}


/******************************************************************************/
/*!
 * @fn void ApplCanOverrun(uint8 channel)
 *
 * @brief Overrun Diagnostic Callbacks
 *
 * @details
 *
 * Reccurence: on Reception of McInit frame
 *
 * @param channel
 *
 * @return void
 */
/*****************************************************************************/
void ApplCanOverrun(uint8 channel)
{
   BCOM_CanOverrunCtr++;
}


/******************************************************************************/
/*!
 * @fn void ApplCanBusOff(void)
 *
 * @brief ApplCanBusOff - only prototype declaration
 *
 * @details This callback function is called if the CAN Controller enters BusOff state.
 * The function is called in the error interrupt, CanTask() or CanErrorTask().
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanBusOff(CAN_CHANNEL_CANTYPE_ONLY)
{
}


/******************************************************************************/
/*!
 * @fn void ApplCanWakeUp(void)
 *
 * @brief Brief description
 *
 * @details This callback function is called if a wake-up condition on the CAN bus
 is detected during sleep mode of the CAN Controller. The function is called in the
 wakeup interrupt, in the CanTask() or in the CanWakeupTask().
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanWakeUp(CAN_CHANNEL_CANTYPE_ONLY)
{
}


/******************************************************************************/
/*!
 * @fn void ApplIlFatalError(vuint8 errorNumber)
 *
 * @brief This function indicates IL errors.
 *
 * @details If assertions are configured, this function is called to indicate invalid
 user conditions (API, reentrance), inconsistent generated data, hardware errors and
 internal errors.
 *
 * @param errorNumber - numeric error code
 *
 * @return void
 */
/*****************************************************************************/
void ApplIlFatalError(vuint8 errorNumber)
{
 vuint8 error;
 error = errorNumber;

 IlFatalErr++;

 while(1);
}


/******************************************************************************/
/*!
 * @fn void ApplCanFatalError(vuint8 channel, vuint8 errorNumber)
 *
 * @brief This function indicates CAN errors
 *
 * @details If assertions are configured, the callback function ApplCanFatalError(..)
 is called in case of invalid user conditions (Application interface, reentrance),
 inconsistent generated data, hardware errors or internal errors (queue). An error
 number is passed by the parameter. The function is called on interrupt and task level.
 *
 * @param channel - Handle of a CAN channel
 * errorNumber - Error identification number
 *
 * @return void
 */
/*****************************************************************************/
void ApplCanFatalError(vuint8 channel, vuint8 errorNumber)
{
 vuint8 error;
 error = errorNumber;
 while(1);
}


/******************************************************************************/
/*!
 * @fn void ApplCanCancelNotification(CanTransmitHandle txHandle)
 *
 * @brief This function informs the Application about that a message was cancelled
 *
 * @details This function will be called if a transmit message is deleted
 (CanCancelTransmit, CanOffline or CanInit). This function could be called in Interrupt
 or Task context.
 *
 * @param txHandle - Handle of cancelled transmit object
 *
 * @return void
 */
/*****************************************************************************/
/* This function is needed if the Interaction Layer is used ONLY for one CAN Channel !*/
void ApplCanCancelNotification(CanTransmitHandle txHandle)
{
}

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif

/* undefine master define for this module    */
#ifdef MASTER_CAN_APPLCAN_C_
#undef MASTER_CAN_APPLCAN_C_
#endif

