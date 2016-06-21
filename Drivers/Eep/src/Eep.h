#ifndef EEP_H_
#define EEP_H_

#include "Std_Types.h"
#include "Eep_Cfg.h"

#define EEP_MODULE_ID            (90)
#define EEP_VENDOR_ID            (TEAMS_VENDOR_ID)
#define EEP_AR_MAJOR_VERSION     2
#define EEP_AR_MINOR_VERSION     2
#define EEP_AR_PATCH_VERSION     2
#define EEP_SW_MAJOR_VERSION     1
#define EEP_SW_MINOR_VERSION     0
#define EEP_SW_PATCH_VERSION     0


#define EEP_INIT_API_ID             	((uint8)(0))
#define EEP_SETMODE_API_ID           	((uint8)(1))
#define EEP_READ_API_ID             	((uint8)(2))
#define EEP_WRITE_API_ID             	((uint8)(3))
#define EEP_ERASE_API_ID             	((uint8)(4))
#define EEP_CANCEL_API_ID				((uint8)(6))
#define EEP_GETSTATUS_API_ID           	((uint8)(7))
#define EEP_GETJOBRESULT_API_ID        	((uint8)(8))
#define EEP_MAINFUNCTION_API_ID			((uint8)(9))

#define EEP_E_PARAM_CONFIG				((uint8)(0x10))
#define EEP_E_PARAM_ADDRESS				((uint8)(0x11))
#define EEP_E_PARAM_DATA				((uint8)(0x12))
#define EEP_E_PARAM_LENGTH				((uint8)(0x13))
#define EEP_E_UNINIT					((uint8)(0x20))
#define EEP_E_BUSY						((uint8)(0x21))


typedef uint32 Eep_AddressType;

typedef uint16 Eep_LengthType;

typedef struct
{
	Spi_SequenceType SequenceReadStatus;
	Spi_SequenceType SequenceReadData;
	Spi_SequenceType SequenceWriteData;
}Eep_ExternalDriverType;

typedef struct
{
	Eep_ExternalDriverType SpiDriver;
	MemIf_ModeType DefaultMode;
	Eep_LengthType EepSize;
	Eep_LengthType EepPageSize;
}Eep_ConfigType;

#define EEP_START_SEC_CODE
#include "MemMap.h"

extern void Eep_Init(void);
extern void Eep_SetMode(MemIf_ModeType Mode);
extern Std_ReturnType Eep_Read(Eep_AddressType EepromAddress, uint8* DataBufferPtr, Eep_LengthType Length);
extern Std_ReturnType Eep_Write(Eep_AddressType EepromAddress,const uint8* DataBufferPtr,Eep_LengthType Length);
extern MemIf_StatusType Eep_GetStatus(void);
extern MemIf_JobResultType Eep_GetJobResult(void);
extern void Eep_MainFunction(void);

inline void Eep_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = EEP_MODULE_ID;
	versioninfo->sw_major_version = EEP_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = EEP_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = EEP_SW_PATCH_VERSION;
	versioninfo->vendorID = EEP_VENDOR_ID;
}
#define EEP_STOP_SEC_CODE
#include "MemMap.h"

#endif /* EEP_H_ */
