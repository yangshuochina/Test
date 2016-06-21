
#ifndef SCHM_EEP_H_
#define SCHM_EEP_H_

#include "team-s.h"
#include "Std_Types.h"
#include "SchM.h"
#include "Eep.h"

inline SchM_ReturnType SchM_ActMainFunction_Eep(uint8 activationPoint) {
	return SchM_ActMainFunction(EEP_MODULE_ID, activationPoint);
}
inline SchM_ReturnType SchM_CancelMainFunction_Eep(uint8 activationPoint) {
	return SchM_CancelMainFunction(EEP_MODULE_ID, activationPoint);
}

#endif
