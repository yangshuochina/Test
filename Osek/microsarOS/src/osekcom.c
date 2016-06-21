/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 2.1 / Autosar OS 3.0
|    File Name: osekcom.c 
|
|  General code: @Version: 5.15@
|  Module version: $vv$=4.26
|
|  Description: OSEK-COM 3.0 conformant inter-task communication
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2006-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* next define is set in the makefile
 * switches of the include of (generated) tcb.h, includes instead
 * the library config file
 */
#ifdef osdGenerateLib
#define osdIncludeLibConfig 1
#endif

#if defined USE_QUOTE_INCLUDES
 #include "osek.h"
 #include "msg.h"
 #include "osekcom.h"
#else
 #include <osek.h>
 #include <msg.h>
 #include <osekcom.h>
#endif

/* Vector release management */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif
/* KB begin vrmReleaseNumber (overwritten) */
/* Source release number */
#define osdVrmMajRelNum 5
#define osdVrmMinRelNum 5
/* KB end vrmReleaseNumber */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif

/* CovComment 0:  file: osekcom.c */

#if defined USE_QUOTE_INCLUDES
 #include "osekext.h"
#else
 #include <osekext.h>
#endif

/* KB begin osekStartOfCModule (overwritten) */
/*
 * Some conditions in the source-code might be always true, because 
 * they depend on the value of generated macros. 
 * Switch of the warning about conditions, that are always true
 */
#pragma warning 549
/*
 * Switch of the warning about possible endless loops
 */
#pragma warning 557
/*
 * Switch of incorrect warning about possible uninitialized variable
 */
#pragma warning 507
/* KB end osekStartOfCModule */

#if (osdUseCOM != 0)
#if (osdNumberOfRxMessages > 0)
#if ((!defined osdInitMessageNotUsed) || (!defined osdSendMessageNotUsed) || (!defined osdReceiveMessageNotUsed))
/* KB begin comHWMemCopy (default) */

static osqFunc1 void osqFunc2 osMemCpy(ApplicationDataRef dest, const osuint8* src, osuint16 bytes);

/* MISRA RULE 14.1 VIOLATION: The function osMemCpy is used in the functions osSendMessage, 
 * osReceiveMessage and osInitMessage below. The actual usage depends on the configuration 
 * of the OS, so in some configurations osMemCpy might be unreachable so that MISRA checking 
 * tools complain.
 */
static osqFunc1 void osqFunc2 osMemCpy(ApplicationDataRef dest, const osuint8* src, osuint16 bytes)      /* PRQA S 3219 */
{
   /* CovComment 4:  Function: osMemCpy */
   while (bytes > 0)
   {
      *dest= *src;
      dest++;
      src++;
      bytes--;
      COVER(0x02D3)
   }
   COVER(0x02D4)
}

/* KB end comHWMemCopy */
#endif /* any API used */
#endif /* osdNumberOfRxMessages */

#if osdComErrorHook
#if (osdComStatusLevel==osdComExtended) || (osdCCC==osdCCCB) || (osdSC==SC3) || (osdSC==SC4)
static osqFunc1 void osqFunc2 osComErrorHook(StatusType ErrorCode);
#endif
#endif

/* MISRA RULE 8.7 VIOLATION: The variable osCOMStartupMode is used in the macro 
 * GetCOMApplicationMode() which is a part of the described API of the COM. 
 * When an application does not use this macro, a MISRA-checker would not see 
 * an external usage and complain that the variable should be made static. 
 */
osqTcbRAM1 osqTcbRAM2 osqTcbRAM3 COMApplicationModeType osCOMStartupMode;      /* PRQA S 1514 */  


/*****************************************************************************
Service name:    StartCOM
Syntax:          StatusType StartCOM (COMApplicationModeType <Mode>)
Parameter (in):  Mode COM application mode.
Parameter (out): None.
Description:     The service StartCOM starts and initialises the OSEK COM
                 implementation in the requested application mode.
                 If StartCOM fails, initialisation of the OSEK COM implementation
                 aborts and StartCOM returns a status code as specified below.
                 StartCOM shall be called from within a task if an OSEK-compliant
                 operating system is used.
                 Before returning, the service StartCOM calls the application function
                 StartCOMExtension.
Caveats:         The hardware and low-level resources used by OSEK COM shall be
                 initialised before StartCOM is called otherwise undefined behaviour
                 results.
                 StartCOM does not enable periodic transmission of messages. If
                 needed, StartPeriodic can be called from StartCOMExtension.
                 StartCOM does not stop periodic transmission when
                 StartCOMExtension returns.
                 StartCOM returns the status code returned by StartCOMExtension if
                 this is different from E_OK.
Status:
 Standard:
                 - This service returns E_OK if the initialisation completed
                   successfully.
                 - This service returns an implementation-specific status code if the
                   initialisation was not completed successfully.
 Extended:
                 In addition to the standard status codes defined above, the following
                 status code is supported:
                 - This service returns E_COM_ID if the parameter <Mode> is out
                   of range.
Context:         Task, Cat2-ISR, any interrupt enabling state         
*****************************************************************************/

#ifndef osdStartCOMNotUsed

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/StartCOM
 */
osqFunc1 StatusType osqFunc2 osStartCOM(COMApplicationModeType Mode)
{
   /* CovComment 4:  Function: osStartCOM */
#if osdComStartComExtension
   StatusType retval;
#endif
#if (osdNumberOfAllMessages > 0)
#if osdNumberOfMsgFlagNotifics || osdNumberOfMessageQueues
   osuint8 i;
#endif
#endif

   OS_SC_ENTRY()

#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_StartCOM & osCurrentContext) == 0)
   {
      COVER(0x02D5)
      #if osdComErrorHook 
      COVER(0x02D6)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveStartCOM_ApplicationModeID(Mode)
      #endif
      osCOMAPIError(E_COM_SYS_CALLEVEL, osdErrSCCallContext);
      #if osdComErrorHook 
      COVER(0x02D7)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SC_EXIT()
      return E_COM_SYS_CALLEVEL;
   }
   ELSEWITHCOVER(0x02D8)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdComStatusLevel == osdComExtended)
   if (Mode >= osrNumberOfCOMModes)
   {
      COVER(0x02D9)
      #if osdComErrorHook
      COVER(0x02DA)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveStartCOM_ApplicationModeID(Mode)
      #endif

      osCOMAPIError(E_COM_ID, osdErrSCWrongModeID);

      #if osdComErrorHook
      COVER(0x02DB)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif

      OS_SC_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x02DC)
#endif

   COVER(0x02DD)

   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_SC_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

#if (osdNumberOfAllMessages > 0)
   COVER(0x02DE)

#if osdNumberOfMsgFlagNotifics
   for(i=0; i<osrNumberOfMsgFlagNotifics; i++)
   {
      COVER(0x02DF)
      osMsgFlags[i]=0;
   }
#endif

#if osdNumberOfMessageQueues
   for(i=0; i<osrNumberOfMessageQueues; i++)
   {
      COVER(0x02E0)
      osMsgQueueWriteIndex[i]       =0; 
      osMsgQueueReadIndex[i]        =0;
      osMsgQueueReceivedMessages[i]= 0;
      osMsgStatus[i]= E_COM_NOMSG;
   }
#endif
#endif

#if ((osdNumberOfMessagesToInitialize > 0) || osdLib)
   COVER(0x02E1)
   osInitMessagesCOMStart();
#endif

   osCOMStartupMode= Mode;

   /* COM Startup Hook */
#if osdComStartComExtension
   COVER(0x02E2)
   retval=osStartCOMExtension();
#endif
   
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SC_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
   

   OS_SC_EXIT()

#if osdComStartComExtension
   COVER(0x02E3)
   return retval;
#else
   COVER(0x02E4)
   return E_OK;
#endif

} /* END OF StartCOM */
#endif


/*****************************************************************************
Service name:    StopCOM
Syntax:          StatusType StopCOM (COMShutdownModeType <Mode>)
Parameter (in):  Mode COM_SHUTDOWN_IMMEDIATE
                 The shutdown occurs immediately without waiting for pending
                 operations to complete.
Parameter (out): None.
Description:     The service StopCOM causes all OSEK COM activity to cease
                 immediately. All resources used by OSEK COM are returned or left in
                 an inactive state. Data loss is possible.
                 StopCOM stops all periodic transmission of messages.
                 When StopCOM completes successfully the system is left in a state in
                 which StartCOM can be called to re-initialise OSEK COM.
Status:
 Standard:
                 - This service returns E_OK if OSEK COM was shut down
                   successfully.
                 - This service returns an implementation-specific status code if the
                   shutdown was not completed successfully.
 Extended:
                 In addition to the standard status codes defined above, the following
                 status code is supported:
                 - This service returns E_COM_ID if the parameter <Mode> is out
                   of range.
Context:         Task, Cat2-ISR, ErrorHook, ShutdownHook, COMErrorHook,
                 any interrupt enabling state         
*****************************************************************************/

#ifndef osdStopCOMNotUsed
/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/StopCOM
 */
#if (osdComStatusLevel == osdComExtended)
osqFunc1 StatusType osqFunc2 osStopCOM(COMShutdownModeType ShutdownMode)
#else
osqFunc1 StatusType osqFunc2 osStopCOM(void)
#endif
{
   /* CovComment 4:  Function: osStopCOM */
   OS_TC_ENTRY()
   COVER(0x02E5)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_StopCOM & osCurrentContext) == 0)
   {
      COVER(0x02E6)
      #if osdComErrorHook 
      COVER(0x02E7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveStopCOM_ApplicationModeID(ShutdownMode)
      #endif
      osCOMAPIError(E_COM_SYS_CALLEVEL, osdErrTCCallContext);
      #if osdComErrorHook 
      COVER(0x02E8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TC_EXIT()
      return E_COM_SYS_CALLEVEL;
   }
   ELSEWITHCOVER(0x02E9)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdComStatusLevel == osdComExtended)
   if (ShutdownMode != COM_SHUTDOWN_IMMEDIATE)
   {
      COVER(0x02EA)
      #if osdComErrorHook
      COVER(0x02EB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveStopCOM_ApplicationModeID(ShutdownMode)
      #endif

      osCOMAPIError(E_COM_ID, osdErrTCWrongModeID);

      #if osdComErrorHook
      COVER(0x02EC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif

      OS_TC_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x02ED)
#endif

   COVER(0x02EE)
   OS_TC_EXIT()
   return E_OK;
}
#endif



#if (osdNumberOfAllMessages > 0)

/*****************************************************************************
Service name:    SendMessage
Syntax:          StatusType SendMessage (
                                           MessageIdentifier <Message>,
                                           ApplicationDataRef <DataRef>
                                        )
Parameter (in):  Message Message identifier (C identifier).
                 DataRef Reference to the application's message data to be transmitted.
Parameter (out): None.
Description:     The service SendMessage updates the message object identified by
                 <Message> with the application message referenced by the
                 <DataRef> parameter.
 External 
 communication:  Not supported
 Internal 
 communication:
                 The message <Message> is routed to the receiving part of the IL.
Status:
 Standard:
                 - This service returns E_OK if the service operation completed
                   successfully.
 Extended:
                 In addition to the standard status code defined above, the following
                 status code is supported:
                 - This service returns E_COM_ID if the parameter <Message> is
                   out of range or if it refers to a message that is received or to a
                   dynamic-length or zero-length message.
Context:         Task, Cat2-ISR, COMCallback, any interrupt enabling state         
*****************************************************************************/

#ifndef osdSendMessageNotUsed

/* MISRA RULE 16.7 VIOLATION: The OSEK COM standard defines that the parameter 
 * DataRef is of type ApplicationDataRef. The same type is used to point to constant 
 * and non constant memory. So this pointer cannot point to a constant. (3673) */
osqFunc1 StatusType osqFunc2 osSendMessage(MessageIdentifier MessageID, 
                                           ApplicationDataRef DataRef) /* PRQA S 3673 */
{
   /* CovComment 4:  Function: osSendMessage */
#if (osdNumberOfRxMessages>0)
   osuint16 i;
#endif   
#if osdCCC==osdCCCB
   ApplicationDataRef ptr;
   osuint8 queueIndex;
#endif
#if ((osdSC== SC3) || (osdSC== SC4))
#if osdNumberOfMsgCallbackNotifics
   osuint16 savedContext;
#endif
#endif

   OS_SM_ENTRY()
   COVER(0x02EF)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   COVER(0x02F0)
   /* check call context */
   if ((osdValidCtx_SendMessage & osCurrentContext) == 0)
   {
      COVER(0x02F1)
      #if osdComErrorHook 
      COVER(0x02F2)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveSendMessage_Message(MessageID)
      osSaveSendMessage_DataRef(DataRef)
      #endif
      osCOMAPIError(E_COM_SYS_CALLEVEL, osdErrSMCallContext);
      #if osdComErrorHook 
      COVER(0x02F3)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SM_EXIT()
      return E_COM_SYS_CALLEVEL;
   }
   ELSEWITHCOVER(0x02F4)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdComStatusLevel == osdComExtended)
   COVER(0x02F5)
   if (((MessageID & osdMsgIdIsTransmit) == 0) 
   #if (osdNumberOfRxMessages > 0)
       || ((MessageID & ~osdMsgIdIsTransmit) >= osrNumberOfRxMessages)
       || (oskMsgNumberOfReceiveMsgs[MessageID & ~osdMsgIdIsTransmit] == 0)
   #endif
       )
   {
      COVER(0x02F6)
      #if osdComErrorHook
      COVER(0x02F7)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveSendMessage_Message(MessageID)
      osSaveSendMessage_DataRef(DataRef)
      #endif

      osCOMAPIError(E_COM_ID, osdErrSMWrongID);

      #if osdComErrorHook
      COVER(0x02F8)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif

      OS_SM_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x02F9)
#endif

   /* clean the MessageID from the 'transmit id'-bit */
   MessageID &= (MessageIdentifier)~osdMsgIdIsTransmit;

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplTxMessageAccess[MessageID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x02FA)
      #if osdComErrorHook 
      COVER(0x02FB)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveSendMessage_Message(MessageID)
      osSaveSendMessage_DataRef(DataRef)
      #endif
      osCOMAPIError(E_COM_ACCESS, osdErrSMNoAccess);
      #if osdComErrorHook 
      COVER(0x02FC)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SM_EXIT()
      return E_COM_ACCESS;
   }
   ELSEWITHCOVER(0x02FD)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
   /* = API function SuspendOSInterrupts */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_SM_START_CRITICAL()
   /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
   osSaveDisableGlobalNested();
   /* KB end osekHWosSaveDisableGlobalNested */
   #endif    
   /* KB end osekHWosDisableTimProtInterrupt */

   /* go through all receive messages attached to this transmit message */
   #if (osdNumberOfRxMessages>0)
   COVER(0x02FE)
      for (i= MessageID; i < (MessageID+oskMsgNumberOfReceiveMsgs[MessageID]); i++)
      {
         COVER(0x02FF)
      #if osdCCC==osdCCCB
         COVER(0x0300)
         if (oskMsgQueueSize[i] == 0)
         {
            COVER(0x0301)
      #endif
            COVER(0x0302)
            /* unqueued message */

            /* update message object */
            osMemCpy(oskMsgDataBuffer[i], DataRef, oskMsgLength[i]);

      #if osdCCC==osdCCCB
         }
         else
         {
            COVER(0x0303)
            /* queued message */

            /* update message object */
            queueIndex= oskMsgIndexToQueueIndex[i];

            /* check if full */
            if (osMsgQueueReceivedMessages[queueIndex] >= oskMsgQueueSize[i])
            {
               COVER(0x0304)
               /* fifo full */
               osMsgStatus[queueIndex]=E_COM_LIMIT;
            }
            else
            {
               COVER(0x0305)
               ptr= oskMsgDataBuffer[i];
               ptr+= osMsgQueueWriteIndex[queueIndex] * oskMsgLength[i];

               osMemCpy(ptr, DataRef, oskMsgLength[i]);
               osMsgQueueReceivedMessages[queueIndex]++;

               osMsgQueueWriteIndex[queueIndex]++;
               if (osMsgQueueWriteIndex[queueIndex] >= oskMsgQueueSize[i])
               {
                  COVER(0x0306)
                  osMsgQueueWriteIndex[queueIndex]= 0;
               }
               ELSEWITHCOVER(0x0307)
            
               osMsgStatus[queueIndex]=E_OK;
            }
         }

      #endif /*osdCCC==osdCCCB*/
      }
   #endif  /* #if (osdNumberOfRxMessages>0) */

   
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_SM_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
   

   /* notification if required */
   /* go through all receive messages attached to this transmit message */
   #if (osdNumberOfRxMessages>0)

      COVER(0x0308)
      #if ((osdFullPreempt || osdMixedPreempt))
         COVER(0x0309)
         osLockDispatcher++;
      #endif

      for (i= MessageID; i < (MessageID+oskMsgNumberOfReceiveMsgs[MessageID]); i++)
      {
#if (osdNumberOfMsgActivateTaskNotifics > 0)
         COVER(0x030A)
         if (oskMsgNotifications[i] == osdCOM_ACTION_TASK)
         {
            #if STATUS_LEVEL == EXTENDED_STATUS
            StatusType atResult;
            #endif
#if ((osdSC == SC3) || (osdSC == SC4))
            ApplicationType savedATApplID;
            
            savedATApplID = osActiveApplicationID;
            osActiveApplicationID = oskRxMessageToAppl[i];
            COVER(0x030B)
#endif
            COVER(0x030C)
            /* activate task */
            #if STATUS_LEVEL == EXTENDED_STATUS
            COVER(0x030D)
/* MISRA RULE 12.10 VIOLATION: The comma operator is required if ErrorInfoLevel = Modulnames.
 * See Deviation Document for details.
 */
               atResult = ActivateTask(oskMsgNotifyTask[i]); /* PRQA S 3417 */
               if (atResult != E_OK)
               {
                  COVER(0x030E)
                  OS_SM_EXIT()
                  return E_COM_SYS_NOTIFY_FAILED;
               }
               ELSEWITHCOVER(0x030F)
            #else
               COVER(0x0310)
               (void) ActivateTask(oskMsgNotifyTask[i]);
            #endif
#if ((osdSC == SC3) || (osdSC == SC4))
               osActiveApplicationID = savedATApplID;
               COVER(0x0311)
#endif
         }
#endif
#if (osdNumberOfMsgSetEventNotifics > 0)
 #if (osdNumberOfMsgActivateTaskNotifics > 0)
         else
 #endif
         if (oskMsgNotifications[i] == osdCOM_ACTION_EVENT)
         {
            #if STATUS_LEVEL == EXTENDED_STATUS
            StatusType seResult;
            #endif
#if ((osdSC == SC3) || (osdSC == SC4))
            ApplicationType savedSEApplID;
            
            savedSEApplID = osActiveApplicationID;
            osActiveApplicationID = oskRxMessageToAppl[i];
            COVER(0x0312)
#endif
            COVER(0x0313)
            /* set event */
            #if STATUS_LEVEL == EXTENDED_STATUS
               COVER(0x0314)
/* MISRA RULE 12.10 VIOLATION: The comma operator is required if ErrorInfoLevel = Modulnames.
 * See Deviation Document for details.
 */
               seResult = SetEvent(oskMsgNotifyTask[i], oskMsgNotifyEvent[i]); /* PRQA S 3417 */
               if (seResult != E_OK)
               {
                  COVER(0x0315)
                  OS_SM_EXIT()
                  return E_COM_SYS_NOTIFY_FAILED;
               }
               ELSEWITHCOVER(0x0316)
            #else
               COVER(0x0317)
               (void) SetEvent(oskMsgNotifyTask[i], oskMsgNotifyEvent[i]);
            #endif
#if ((osdSC == SC3) || (osdSC == SC4))
               osActiveApplicationID = savedSEApplID;
#endif
         }
#endif
#if osdNumberOfMsgCallbackNotifics
 #if (osdNumberOfMsgActivateTaskNotifics > 0) || (osdNumberOfMsgSetEventNotifics > 0)
         else
 #endif
         if (oskMsgNotifications[i] == osdCOM_ACTION_CALLBACK)
         {
            COVER(0x0318)
            /* call the callback function */
#if ((osdSC== SC3) || (osdSC== SC4))
            COVER(0x0319)
            savedContext = osCurrentContext;
            osCurrentContext = osdCtxCOMCallback;
#endif

            oskMsgNotifyCallBack[i]();

#if ((osdSC== SC3) || (osdSC== SC4))
            COVER(0x031A)
            osCurrentContext = savedContext;
#endif
         }
#endif /*osdNumberOfMsgCallbackNotifics*/

#if (osdNumberOfMsgFlagNotifics > 0)
 #if (osdNumberOfMsgActivateTaskNotifics > 0) || (osdNumberOfMsgSetEventNotifics > 0) || (osdNumberOfMsgCallbackNotifics > 0)
         else
 #endif
         if(oskMsgNotifications[i] == osdCOM_ACTION_FLAG)
         {
            COVER(0x031B)
            osMsgFlags[oskMsgNotifyFlagIndex[i]]=osdTRUE;
         }
#endif
#if (osdNumberOfMsgActivateTaskNotifics > 0) || (osdNumberOfMsgSetEventNotifics > 0) || (osdNumberOfMsgCallbackNotifics > 0) || (osdNumberOfMsgFlagNotifics > 0)
         else
         {
            COVER(0x031C)
         }
#endif
      }

      #if ((osdFullPreempt || osdMixedPreempt))
      COVER(0x031D)
      osLockDispatcher--;
/* MISRA RULE 12.4 not violated: Reading the variable osStartDispatcher is sideeffect free.
 */
      if ((osLockDispatcher == 0) && (osStartDispatcher != 0))      /* PRQA S 3415 */
      {
         COVER(0x031E)
         OS_SM_EXIT()
         OS_START_DISPATCH()
         /* KB begin osekHWcallosDispatcher (default) */
            /* KB begin osekHWosDispatcher (overwritten) */
            osDispatch()
            /* KB end osekHWosDispatcher */
         /* KB end osekHWcallosDispatcher */
      }
      else
      #endif /* ((osdFullPreempt || osdMixedPreempt)) */
   #endif /* #if (osdNumberOfRxMessages>0) */
   {
      COVER(0x031F)
      OS_SM_EXIT()
   }
   return E_OK;
}
#endif

/*****************************************************************************
Service name:    ReceiveMessage
Syntax:          StatusType ReceiveMessage (
                                              MessageIdentifier <Message>,
                                              ApplicationDataRef <DataRef>
                                           )
Parameter (in):  Message Message identifier (C identifier).
Parameter (out): DataRef Reference to the application's message area in which to store the
                 received data.
Description:     The service ReceiveMessage updates the application message
                 referenced by <DataRef> with the data in the message object
                 identified by <Message>. It resets all flags (Notification classes 1 and
                 3) associated with <Message>.
Status:
 Standard:
                 - This service returns E_OK if data in the queued or unqueued
                   message identified by <Message> are available and returned to
                   the application successfully.
                 - This service returns E_COM_NOMSG if the queued message
                   identified by <Message> is empty.
                 - This service returns E_COM_LIMIT if an overflow of the message
                   queue identified by <Message> occurred since the last call to
                   ReceiveMessage for <Message>. E_COM_LIMIT indicates that
                   at least one message has been discarded since the message
                   queue filled. Nevertheless the service is performed and a
                   message is returned. The service ReceiveMessage clears the
                   overflow condition for <Message>.
 Extended:
                 In addition to the standard status codes defined above, the following
                 status code is supported:
                 - This service returns E_COM_ID if the parameter <Message> is
                   out of range or if it refers to message that is sent or to a dynamiclength
                   or zero-length message.
Context:         Task, Cat2-ISR, COMCallback, any interrupt enabling state         
*****************************************************************************/

#ifndef osdReceiveMessageNotUsed

osqFunc1 StatusType osqFunc2 osReceiveMessage(MessageIdentifier MessageID, 
                                              ApplicationDataRef DataRef)
{
   /* CovComment 4:  Function: osReceiveMessage */
#if osdCCC==osdCCCB
   ApplicationDataRef ptr;
   osuint8 queueIndex;
#endif

   OS_RM_ENTRY()
   COVER(0x0320)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_ReceiveMessage & osCurrentContext) == 0)
   {
      COVER(0x0321)
      #if osdComErrorHook 
      COVER(0x0322)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveReceiveMessage_Message(MessageID)
      osSaveReceiveMessage_DataRef(DataRef)
      #endif
      osCOMAPIError(E_COM_SYS_CALLEVEL, osdErrRMCallContext);
      #if osdComErrorHook 
      COVER(0x0323)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RM_EXIT()
      return E_COM_SYS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0324)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdComStatusLevel == osdComExtended)
   if (((MessageID & osdMsgIdIsTransmit) != 0) || (MessageID >= osrNumberOfRxMessages))
   {
      COVER(0x0325)
      #if osdComErrorHook
      COVER(0x0326)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveReceiveMessage_Message(MessageID)
      osSaveReceiveMessage_DataRef(DataRef)
      #endif

      osCOMAPIError(E_COM_ID, osdErrRMWrongID);

      #if osdComErrorHook
      COVER(0x0327)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif

      OS_RM_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x0328)
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplRxMessageAccess[MessageID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0329)
      #if osdComErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveReceiveMessage_Message(MessageID)
      osSaveReceiveMessage_DataRef(DataRef)
      #endif
      osCOMAPIError(E_COM_ACCESS, osdErrRMNoAccess);
      #if osdComErrorHook 
      COVER(0x032A)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RM_EXIT()
      return E_COM_ACCESS;
   }
   ELSEWITHCOVER(0x032B)
   if (osInSystemFct == 0)
   {   /* check rights for reference */
      if (osAddressParamValid((osuint32)DataRef, (osuint32)oskMsgLength[MessageID]) == osdFALSE)
      {  /* no writeable area found -> no write access rights for result */
          COVER(0x032C)
          #if osdComErrorHook 
          COVER(0x032D)
          /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
          /* = API function SuspendOSInterrupts */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */

          osSaveReceiveMessage_Message(MessageID)
          osSaveReceiveMessage_DataRef(DataRef)
          #endif
          osCOMAPIError(E_COM_SYS_ILLEGAL_ADDRESS, osdErrRMIllegalAddr);
          #if osdComErrorHook 
          COVER(0x032E)
          /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
          /* = API function ResumeOSInterrupts */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          #endif
          OS_RM_EXIT()
          return E_COM_SYS_ILLEGAL_ADDRESS;
      }
      ELSEWITHCOVER(0x032F)
   }
   ELSEWITHCOVER(0x0330)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/


#if osdCCC==osdCCCB
   COVER(0x0331)
   if (oskMsgQueueSize[MessageID] == 0)
   {
      COVER(0x0332)
#endif
      COVER(0x0333)
      /* unqueued message */

      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      OS_RM_START_CRITICAL()
      /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      #endif    
      /* KB end osekHWosDisableTimProtInterrupt */

      /* update message object */
      osMemCpy(DataRef, oskMsgDataBuffer[MessageID], oskMsgLength[MessageID]);

#if osdCCC==osdCCCB
   }
   else
   {
      COVER(0x0334)
      /* queued message */

      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      OS_RM_START_CRITICAL()
      /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      #endif    
      /* KB end osekHWosDisableTimProtInterrupt */

      queueIndex= oskMsgIndexToQueueIndex[MessageID];

      if (osMsgQueueReceivedMessages[queueIndex] > 0)
      {
         COVER(0x0335)
         ptr= oskMsgDataBuffer[MessageID];
         ptr+= osMsgQueueReadIndex[queueIndex] * oskMsgLength[MessageID];

         osMemCpy(DataRef, ptr, oskMsgLength[MessageID]);
         osMsgQueueReceivedMessages[queueIndex]--;

         osMsgQueueReadIndex[queueIndex]++;
         if (osMsgQueueReadIndex[queueIndex] >= oskMsgQueueSize[MessageID])
         {
            COVER(0x0336)
            osMsgQueueReadIndex[queueIndex]= 0;
         }
         ELSEWITHCOVER(0x0337)

         if(osMsgStatus[queueIndex] == E_COM_LIMIT)
         {
            /* message lost */
            if(osMsgQueueReceivedMessages[queueIndex] == 0)
            {
               COVER(0x0338)
               osMsgStatus[queueIndex]= E_COM_NOMSG;
            }
            else
            {
               COVER(0x0339)
               osMsgStatus[queueIndex]= E_OK;
            }
            
            /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
            #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
            /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
            osRestoreEnableGlobalNested();
            /* KB end osekHWosRestoreEnableGlobalNested */
            #endif   
            /* KB end osekHWosEnableTimProtInterrupt */
            OS_RM_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */

            COVER(0x033A)
            #if osdComErrorHook
            COVER(0x033B)
            /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
            /* = API function SuspendOSInterrupts */
            osSaveDisableLevelNested();
            /* KB end osekHWosSaveDisableLevelNested */
      
            osSaveReceiveMessage_Message(MessageID)
            osSaveReceiveMessage_DataRef(DataRef)
            #endif
      
            osCOMAPIError(E_COM_LIMIT, osdErrRMLimit);
      
            #if osdComErrorHook
            COVER(0x033C)
            /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
            /* = API function ResumeOSInterrupts */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            #endif


            OS_RM_EXIT()
            return E_COM_LIMIT;
         }
         ELSEWITHCOVER(0x033D)
      }
      else
      {
         COVER(0x033E)
         /* fifo empty */

         /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
         #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
         /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
         osRestoreEnableGlobalNested();
         /* KB end osekHWosRestoreEnableGlobalNested */
         #endif   
         /* KB end osekHWosEnableTimProtInterrupt */
         OS_RM_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         COVER(0x033F)
         #if osdComErrorHook
         COVER(0x0340)
         /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
         /* = API function SuspendOSInterrupts */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */

   
         osSaveReceiveMessage_Message(MessageID)
         osSaveReceiveMessage_DataRef(DataRef)
         #endif
   
         osCOMAPIError(E_COM_NOMSG, osdErrRMNoMessage);
   
         #if osdComErrorHook
         COVER(0x0341)
         /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
         /* = API function ResumeOSInterrupts */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_RM_EXIT()
         return E_COM_NOMSG;
      }
      COVER(0x0342)
   }
#endif   
   COVER(0x0343)
   /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
   #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
   /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
   osRestoreEnableGlobalNested();
   /* KB end osekHWosRestoreEnableGlobalNested */
   #endif   
   /* KB end osekHWosEnableTimProtInterrupt */
   OS_RM_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
   /* = API function ResumeOSInterrupts */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   OS_RM_EXIT()

   return E_OK;
}

#endif


#if osdCCC==osdCCCB

/*****************************************************************************
Service name:    GetMessageStatus
Syntax:          StatusType GetMessageStatus (
                                                MessageIdentifier <Message>
                                             )
Parameter (in):  Message Message identifier (C identifier).
Parameter (out): None.
Description:     The service GetMessageStatus returns the current status of the
                 message object <Message>.
Status:
 Standard:
                 - This service returns E_COM_NOMSG if the message queue
                   identified by <Message> is empty.
                 - This service returns E_COM_LIMIT if an overflow of the message
                   queue identified by <Message> occurred since the last call to
                   ReceiveMessage for <Message>.
                 - This service returns E_OK if none of the conditions specified
                   above is applicable or fulfilled and no error indication is present.
 Extended:
                 In addition to the standard status codes defined above, the following
                 status code is supported:
                 - This service returns E_COM_ID if the parameter <Message> is
                   out of range or if it does not refer to a queued message.
Context:         Task, Cat2-ISR, COMCallback, COMErrorHook, 
                 any interrupt enabling state         
*****************************************************************************/

#ifndef osdGetMessageStatusNotUsed

osqFunc1 StatusType osqFunc2 osGetMessageStatus(MessageIdentifier MessageID)
{
   /* CovComment 4:  Function: osGetMessageStatus */

   osuint8 queueIndex;

   OS_MS_ENTRY()
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_GetMessageStatus & osCurrentContext) == 0)
   {
      COVER(0x0344)
      #if osdComErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x0345)
      #endif

      osCOMAPIError(E_COM_SYS_CALLEVEL, osdErrMSCallContext);

      #if osdComErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x0346)
      #endif

      OS_MS_EXIT()
      return E_COM_SYS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0347)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdComStatusLevel == osdComExtended)
   if ((MessageID & osdMsgIdIsTransmit) != 0)
   {
      COVER(0x0348)
      /* transmit message */
      #if osdComErrorHook
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x0349)
      #endif

      osCOMAPIError(E_COM_ID, osdErrMSWrongID);

      #if osdComErrorHook
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x034A)
      #endif

      OS_MS_EXIT()
      return E_COM_ID; /* transmit messages never have a buffer */
   }
   ELSEWITHCOVER(0x034B)

   /* receive message */

   if (MessageID >= osrNumberOfRxMessages)
   {
      COVER(0x034C)
      #if osdComErrorHook
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x034D)
      #endif

      osCOMAPIError(E_COM_ID, osdErrMSWrongID);

      #if osdComErrorHook
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x034E)
      #endif

      OS_MS_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x034F)

   if (oskMsgQueueSize[MessageID] == 0)
   {
      COVER(0x0350)
      /* unqueued message, no status */
      #if osdComErrorHook
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x0351)
      #endif

      osCOMAPIError(E_COM_ID, osdErrMSWrongID);

      #if osdComErrorHook
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x0352)
      #endif

      OS_MS_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x0353)
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplRxMessageAccess[MessageID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0354)
      #if osdComErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x0355)
      #endif

      osCOMAPIError(E_COM_ACCESS, osdErrMSNoAccess);

      #if osdComErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x0356)
      #endif

      OS_MS_EXIT()
      return E_COM_ACCESS;
   }
   ELSEWITHCOVER(0x0357)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

   /* queued message */
   queueIndex = oskMsgIndexToQueueIndex[MessageID];
   if (osMsgQueueReceivedMessages[queueIndex] == 0)
   {
      COVER(0x0358)
      #if osdComErrorHook
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x0359)
      #endif

      osCOMAPIError(E_COM_NOMSG, osdErrMSNoMessage);

      #if osdComErrorHook
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x035A)
      #endif

      OS_MS_EXIT()
      return E_COM_NOMSG;
   }
   ELSEWITHCOVER(0x035B)

   /* check all queues for overflow */
   if (osMsgStatus[queueIndex] == E_COM_LIMIT)
   {
      #if osdComErrorHook
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetMessageStatus_Message(MessageID)
      COVER(0x035C)
      #endif

      osCOMAPIError(E_COM_LIMIT, osdErrMSLimit);

      #if osdComErrorHook
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      COVER(0x035D)
      #endif

      OS_MS_EXIT()
      return E_COM_LIMIT;
   }
   ELSEWITHCOVER(0x035E)

   OS_MS_EXIT()
   COVER(0x035F)
   return E_OK;
}
#endif
#endif /* osdCCCB */

#ifndef osdInitMessageNotUsed

/****************************************************************************************
Service name:    InitMessage
Syntax:          StatusType InitMessage (
                                           MessageIdentifier <Message>,
                                           ApplicationDataRef <DataRef>
                                        )
Parameter (in):  Message Message identifier (C identifier).
                 DataRef Reference to the application's message initialisation data.
Parameter (out): none
Description:     The service InitMessage initialises the message object identified by
                 <Message> with the application data referenced by the <DataRef>
                 parameter.
Particularities: This function may be called in StartCOMExtension in order to change
                 the default initialisation.
                 For dynamic-length messages, the length of the message is initialised
                 to its maximum.
Status:
 Standard:
                 - This service returns E_OK if the initialisation of the message
                   object completed successfully.
                 - This service returns an implementation-specific status code if the
                   initialisation did not complete successfully.
 Extended:
                 In addition to the standard status code defined above, the following
                 status code is supported:
                 - This service returns E_COM_ID if the parameter <Message> is
                   out of range or refers to a zero-length message or to an internal
                   transmit message. 
Context:         Task, Cat2-ISR, COMCallback, COMErrorHook, StartCOMExtension,
                 any interrupt enabling state         
*******************************************************************************************/

/* If this function is reported by a MISRA checking tool to violate Rule 14.1 (because this function is never called)
 * the API function may be switched off by the OS configuration parameter OS/APIOptimization/Manual/InitMessage
 */
/* MISRA RULE 16.7 VIOLATION: The OSEK COM standard defines that the parameter 
 * DataRef is of type ApplicationDataRef. The same type is used to point to constant 
 * and non constant memory. So this pointer cannot point to a constant. (3673) 
 */
osqFunc1 StatusType osqFunc2 osInitMessage(MessageIdentifier MessageID, ApplicationDataRef DataRef) /* PRQA S 3673 */
{
   /* CovComment 4:  Function: osInitMessage */

#if osdCCC==osdCCCB
   osuint8 queueIndex;
#endif

   OS_IM_ENTRY()
   COVER(0x0360)
#if ((osdSC== SC3) || (osdSC== SC4)) 
   /* check call context */
   if ((osdValidCtx_InitMessage & osCurrentContext) == 0)
   {
      COVER(0x0361)
      #if osdComErrorHook 
      COVER(0x0362)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveInitMessage_Message(MessageID)
      osSaveInitMessage_DataRef(DataRef)
      #endif
      osCOMAPIError(E_COM_SYS_CALLEVEL, osdErrIMCallContext);
      #if osdComErrorHook 
      COVER(0x0363)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_IM_EXIT()
      return E_COM_SYS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0364)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if (osdComStatusLevel == osdComExtended)
   if (((MessageID & osdMsgIdIsTransmit) != 0) || (MessageID >= osrNumberOfRxMessages))
   {
      COVER(0x0365)
      #if osdComErrorHook
      COVER(0x0366)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveInitMessage_Message(MessageID)
      osSaveInitMessage_DataRef(DataRef)
      #endif

      osCOMAPIError(E_COM_ID, osdErrIMWrongID);

      #if osdComErrorHook
      COVER(0x0367)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif

      OS_IM_EXIT()
      return E_COM_ID; /* transmit messages never have a buffer */
   }
   ELSEWITHCOVER(0x0368)
#endif

#if ((osdSC== SC3) || (osdSC== SC4))
   /* check access rights for this object */
   if (((oskApplRxMessageAccess[MessageID] & (1UL << osActiveApplicationID)) == 0))
   {
      COVER(0x0369)
      #if osdComErrorHook 
      COVER(0x036A)
      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      osSaveInitMessage_Message(MessageID)
      osSaveInitMessage_DataRef(DataRef)
      #endif
      osCOMAPIError(E_COM_ACCESS, osdErrIMNoAccess);
      #if osdComErrorHook 
      COVER(0x036B)
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_IM_EXIT()
      return E_COM_ACCESS;
   }
   ELSEWITHCOVER(0x036C)
#endif /*((osdSC== SC3) || (osdSC== SC4))*/

#if osdCCC==osdCCCB
   COVER(0x036D)
   if (oskMsgQueueSize[MessageID] == 0)
   {
      COVER(0x036E)
#endif
      COVER(0x036F)
      /* unqueued message */

      /* KB begin osekHWosSaveDisableLevelNested (overwritten) */
      /* = API function SuspendOSInterrupts */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      OS_IM_START_CRITICAL()
      /* KB begin osekHWosDisableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosSaveDisableGlobalNested (overwritten) */
      osSaveDisableGlobalNested();
      /* KB end osekHWosSaveDisableGlobalNested */
      #endif    
      /* KB end osekHWosDisableTimProtInterrupt */

      /* update message object */
      osMemCpy(oskMsgDataBuffer[MessageID], DataRef, oskMsgLength[MessageID]);

      /* KB begin osekHWosEnableTimProtInterrupt (overwritten) */
      #if (osdTimingProtectionUsed != 0) && (osdKillTaskIsrPossible != 0)
      /* KB begin osekHWosRestoreEnableGlobalNested (overwritten) */
      osRestoreEnableGlobalNested();
      /* KB end osekHWosRestoreEnableGlobalNested */
      #endif   
      /* KB end osekHWosEnableTimProtInterrupt */
      OS_IM_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevelNested (overwritten) */
      /* = API function ResumeOSInterrupts */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */


#if osdCCC==osdCCCB
   }
   else
   {
      COVER(0x0370)
      /* queued message */
      queueIndex= oskMsgIndexToQueueIndex[MessageID];

      /* set number of received messages to zero */
      osMsgQueueReceivedMessages[queueIndex]= 0;
   }
#endif

   COVER(0x0371)
   OS_IM_EXIT()
   return E_OK;

} /* END OF osInitMessage */
#endif

#endif /* osdNumberOfAllMessages */

/*-+--------------------------------------------------------------------------
 / osComErrorHook
 / Funktion:
 /               - wrap Errorhook: disable interrupts and prevent recursive call
 / Parameter:    StatusType ErrorCode
 / Returnvalue:  none
 / Context:      Interrupts disabled up to system level at least
 /               This function is meant for OS internal use only
 /--------------------------------------------------------------------------*/

#if osdComErrorHook
#if (osdComStatusLevel==osdComExtended) || (osdCCC==osdCCCB) || (osdSC==SC3) || (osdSC==SC4)
static osqFunc1 void osqFunc2 osComErrorHook(StatusType ErrorCode)
{
   /* CovComment 4:  Function: osComErrorHook */

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   osuint8 osSavedAPIStatus;
#endif

   OS_CRHOOK_ENTRY()
   COVER(0x0372)

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x0373)
   /* save and reset API status to prevent IntAPI errors inside errorhook */
   osSavedAPIStatus = osIntAPIStatus;
   osSetIntAPIStatus(0);
#endif

   /* KB begin osekAssignGlobalError (default) */
   #if osdORTIDebug
   osLastErrorCode=ErrorCode;
   #endif
   /* KB end osekAssignGlobalError */
   
   if (osCOMErrorFlag  == osdFALSE)
   {
      COVER(0x0374)
      osCOMErrorFlag = osdTRUE; /* first entry in COMErrorHook, lock COMErrorHook */

#if (osdComStatusLevel == osdComExtended)
      COVER(0x0375)
      osSysErrAssert((osCheckInterruptsDisabled() != 0), osdErrCRInterruptsEnabled)
#endif
      osInterCOMErrorHook(ErrorCode);
      COVER(0x0376)

      osCOMErrorFlag = osdFALSE; /* unlock COMErrorHook */
   }
   ELSEWITHCOVER(0x0377)

#if ((STATUS_LEVEL == EXTENDED_STATUS) || (osdComStatusLevel == osdComExtended))
   COVER(0x0378)
   /* restore old IntAPI status */
   osSetIntAPIStatus(osSavedAPIStatus);
#endif

   OS_CRHOOK_EXIT()
} /* END OF osComErrorHook */
#endif /* (osdComStatusLevel==osdComExtended) || (osdCCC==osdCCCB) || (osdSC==SC3) || (osdSC==SC4) */
#endif /* osdComErrorHook */


#endif /*osdUseCOM*/


/* KB begin osekEndOfCModule (default) */
/* KB end osekEndOfCModule */
