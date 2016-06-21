#ifndef EA_CBK_H_
#define EA_CBK_H_



#define EA_START_SEC_CODE
#include "MemMap.h"
extern void Ea_JobEndNotification(void);
extern void Ea_JobErrorNotification(void);
#define EA_STOP_SEC_CODE
#include "MemMap.h"


#endif /* EA_CBK_H_ */
