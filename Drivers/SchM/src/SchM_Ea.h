
#ifndef SCHM_EA_H_
#define SCHM_EA_H_

#include "team-s.h"
#include "Std_Types.h"
#include "SchM.h"
#include "Ea.h"

inline SchM_ReturnType SchM_ActMainFunction_Ea(uint8 activationPoint) {
	return SchM_ActMainFunction(EA_MODULE_ID, activationPoint);
}
inline SchM_ReturnType SchM_CancelMainFunction_Ea(uint8 activationPoint) {
	return SchM_CancelMainFunction(EA_MODULE_ID, activationPoint);
}

#endif
