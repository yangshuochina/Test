#ifndef EEP_CBK_H_
#define EEP_CBK_H_

#define EEP_START_SEC_CODE
#include "MemMap.h"
extern void EepJobEndNotification(void);
extern void EepJobErrorNotification(void);
#define EEP_STOP_SEC_CODE
#include "MemMap.h"


#endif /* EEP_CBK_H_ */
