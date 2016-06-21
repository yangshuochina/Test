/* Kernbauer Version: 2.02 Konfiguration: TriCore Erzeugungsgangnummer: 200 Zweig: 3 */

/*****************************************************************************
| Project Name: OSEK 2.2 / Autosar OS 3.0
|    File Name: osekcom.h
|
|  General code: @Version: 5.15@
|  Module version: $vv$=4.08
|
|  Description: OSEK-COM 3.0 conformant inter-task communication
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2006-2010 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/


/* double include preventer */
#ifndef __OSEKCOM__
#define __OSEKCOM__

/*lint -save Messages inhibited in this file, will be re-enabled at the end of file */
/*lint -e539 Did not expect positive indentation */

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


#if (osdUseCOM != 0)

/* base for mandatory COM error codes */
#define E_COM_ERROR_CODE_BASE 32u
/* base for implementation specific COM error codes */
#define E_COM_SYS_ERROR_CODE_BASE 48u

/* error codes */
/* no longer used: E_COM_BUSY (E_COM_ERROR_CODE_BASE) */
#define E_COM_ID (E_COM_ERROR_CODE_BASE+1u)
#define E_COM_LIMIT (E_COM_ERROR_CODE_BASE+2u)
/* no longer used: E_COM_LOCKED (E_COM_ERROR_CODE_BASE+3) */
#define E_COM_NOMSG (E_COM_ERROR_CODE_BASE+4u)
/* no longer used: E_COM_RX_ON (E_COM_ERROR_CODE_BASE+5)    / * for compatibilty reasons */
#define E_COM_ACCESS (E_COM_ERROR_CODE_BASE+6u)

#define E_COM_SYS_NOT_SUPPORTED   (E_COM_SYS_ERROR_CODE_BASE)
#define E_COM_SYS_NOTIFY_FAILED   (E_COM_SYS_ERROR_CODE_BASE+1u)
#define E_COM_SYS_ILLEGAL_ADDRESS (E_COM_SYS_ERROR_CODE_BASE+2u)
#define E_COM_SYS_CALLEVEL        (E_COM_SYS_ERROR_CODE_BASE+3u)

/* shutdown modes */
#define COM_SHUTDOWN_IMMEDIATE 0

/* actions */
#define osdCOM_ACTION_NONE 0
#define osdCOM_ACTION_TASK 1
#define osdCOM_ACTION_EVENT 2
#define osdCOM_ACTION_CALLBACK 3
#define osdCOM_ACTION_FLAG 4

#ifndef COMCallback
#define COMCallback(x) void x(void)
#endif

/* types */
typedef osuint16 MessageIdentifier;
typedef osuint8 *ApplicationDataRef;
typedef osuint8 COMApplicationModeType;
typedef osuint8 COMShutdownModeType;

#define COM_FALSE 0
#define COM_TRUE  1
typedef osuint16 COMServiceIdType;

typedef void (*osComCbFct)(void);
typedef osuint8 FlagType;
typedef osuint8 FlagValue;

/* prototypes */
/* KB begin osekCallBackSectionStart (default) */
/* KB end osekCallBackSectionStart */
osqFunc1 StatusType osqFunc2 StartCOMExtension(void);
void COMErrorHook(StatusType Error);
/* KB begin osekCallBackSectionEnd (default) */
/* KB end osekCallBackSectionEnd */

#define StartPeriodic() ((StatusType) E_OK)
#define StopPeriodic()  ((StatusType) E_OK)

osqFunc1 StatusType osqFunc2 osStartCOM(COMApplicationModeType Mode);
#if (osdComStatusLevel == osdComExtended)
osqFunc1 StatusType osqFunc2 osStopCOM(COMShutdownModeType ShutdownMode);
#else
osqFunc1 StatusType osqFunc2 osStopCOM(void);
#endif
osqFunc1 StatusType osqFunc2 osInitMessage(MessageIdentifier MessageID, ApplicationDataRef DataRef);
osqFunc1 StatusType osqFunc2 osSendMessage(MessageIdentifier MessageID, ApplicationDataRef DataRef);
osqFunc1 StatusType osqFunc2 osReceiveMessage(MessageIdentifier MessageID, ApplicationDataRef DataRef);
/* The function SendZeroMessage is not relevant for inter-task-communication */
osqFunc1 StatusType osqFunc2 osGetMessageStatus(MessageIdentifier MessageID);

#ifndef osdGetCOMApplicationModeNotUsed
#define GetCOMApplicationMode() (osCOMStartupMode)
#endif /*osdGetCOMApplicationModeNotUsed*/

#ifdef SendMessage
   #undef SendMessage
#endif

#if (osdComStatusLevel == osdComExtended) && !defined(osdSuppressFilenames)
   
   #ifndef osdStartCOMNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define StartCOM(x) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osPsysStartCOM(x))
      #else
         #define StartCOM(x) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osStartCOM(x))
      #endif
   #endif

   #ifndef osdStopCOMNotUsed
      #if (osdComStatusLevel == osdComExtended)
         #if ((osdSC== SC3) || (osdSC== SC4))
            #define StopCOM(x) (osActiveTaskLineNumber= __LINE__,\
               osActiveTaskModule= __FILE__,\
               osPsysStopCOM(x))
         #else
            #define StopCOM(x) (osActiveTaskLineNumber= __LINE__,\
               osActiveTaskModule= __FILE__,\
               osStopCOM(x))
         #endif
      #else
         #if ((osdSC== SC3) || (osdSC== SC4))
            #define StopCOM(x) (osActiveTaskLineNumber= __LINE__,\
               osActiveTaskModule= __FILE__,\
               osPsysStopCOM())
         #else
            #define StopCOM(x) (osActiveTaskLineNumber= __LINE__,\
               osActiveTaskModule= __FILE__,\
               osStopCOM())
         #endif
      #endif
   #endif
   
   #ifndef osdSendMessageNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define SendMessage(x, y) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osPsysSendMessage((x), (y)))
      #else
         #define SendMessage(x, y) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osSendMessage((x), (y)))
      #endif
   #endif

   #ifndef osdReceiveMessageNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define ReceiveMessage(x, y) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osPsysReceiveMessage((x), (y)))
      #else
         #define ReceiveMessage(x, y) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osReceiveMessage((x), (y)))
      #endif
   #endif

   #ifndef osdGetMessageStatusNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define GetMessageStatus(x) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osPsysGetMessageStatus(x))
      #else
         #define GetMessageStatus(x) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osGetMessageStatus(x))
      #endif
   #endif

   #ifndef osdInitMessageNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define InitMessage(x, y) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osPsysInitMessage((x), (y)))
      #else
         #define InitMessage(x, y) (osActiveTaskLineNumber= __LINE__,\
            osActiveTaskModule= __FILE__,\
            osInitMessage((x), (y)))
      #endif
   #endif

#else /*STATUS_LEVEL...*/

   #ifndef osdStartCOMNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define StartCOM(x) (osPsysStartCOM(x))
      #else
         #define StartCOM(x) (osStartCOM(x))
      #endif
   #endif

   #ifndef osdStopCOMNotUsed
      #if (osdComStatusLevel == osdComExtended)
         #if ((osdSC== SC3) || (osdSC== SC4))
            #define StopCOM(x) (osPsysStopCOM(x))
         #else
            #define StopCOM(x) (osStopCOM(x))
         #endif
      #else
         #if ((osdSC== SC3) || (osdSC== SC4))
            #define StopCOM(x) (osPsysStopCOM())
         #else
            #define StopCOM(x) (osStopCOM())
         #endif
      #endif
   #endif
   
   #ifndef osdSendMessageNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define SendMessage(x, y) (osPsysSendMessage((x), (y)))
      #else
         #define SendMessage(x, y) (osSendMessage((x), (y)))
      #endif
   #endif

   #ifndef osdReceiveMessageNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define ReceiveMessage(x, y) (osPsysReceiveMessage((x), (y)))
      #else
         #define ReceiveMessage(x, y) (osReceiveMessage((x), (y)))
      #endif
   #endif

   #ifndef osdGetMessageStatusNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define GetMessageStatus(x) (osPsysGetMessageStatus(x))
      #else
         #define GetMessageStatus(x) (osGetMessageStatus(x))
      #endif
   #endif

   #ifndef osdInitMessageNotUsed
      #if ((osdSC== SC3) || (osdSC== SC4))
         #define InitMessage(x, y) (osPsysInitMessage((x), (y)))
      #else
         #define InitMessage(x, y) (osInitMessage((x), (y)))
      #endif
   #endif

#endif /*STATUS_LEVEL...*/

#endif /*osdUseCOM*/


/*lint -restore re-enable messages*/

#endif /* double include preventer */
