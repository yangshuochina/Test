#ifndef EA_H_
#define EA_H_

#include "Std_Types.h"
#include "MemIf_Types.h"

#define EA_MODULE_ID            (40)
#define EA_VENDOR_ID            (TEAMS_VENDOR_ID)
#define EA_AR_MAJOR_VERSION     3
#define EA_AR_MINOR_VERSION     1
#define EA_AR_PATCH_VERSION     1
#define EA_SW_MAJOR_VERSION     1
#define EA_SW_MINOR_VERSION     0
#define EA_SW_PATCH_VERSION     0


#define EA_INIT_API_ID             		((uint8)(0))
#define EA_SETMODE_API_ID           	((uint8)(1))
#define EA_READ_API_ID             		((uint8)(2))
#define EA_WRITE_API_ID             	((uint8)(3))
#define EA_CANCEL_API_ID             	((uint8)(4))
#define EA_GETSTATUS_API_ID           	((uint8)(5))
#define EA_GETJOBRESULT_API_ID        	((uint8)(6))
#define EA_INVALIDATEBLOCK_API_ID       ((uint8)(7))
#define EA_GETVERSIONINFO_API_ID		((uint8)(8))
#define EA_ERASEIMMEDIATEBLOCK_API_ID	((uint8)(9))
#define EA_JOBENDNOTIFICATION_API_ID   ((uint8)(0x10))
#define EA_JOBERRORNOTIFICATION_API_ID ((uint8)(0x11))
#define EA_MAINFUNCTION_API_ID         ((uint8)(0x12))

#define EA_E_NOT_INITIALIZED			((uint8)(1))
#define EA_E_INVALID_BLOCK_NO			((uint8)(2))


typedef struct
{
	sint8 ActiveGeneration;
	sint8 ActivePageIndex;
}Ea_PageStatus;

typedef struct {
    uint16    EaBlockNumber;
    uint16    EaBlockSize;
    sint8     EaPageCount;
	Ea_PageStatus* PageStatus;
}Ea_BlockConfigType;

typedef struct {
	const Ea_BlockConfigType * blocks;
	uint16 num_of_blocks;
	uint8 virtual_page_size;
	uint8 max_page_num;
}Ea_ConfigType;

#define EA_START_SEC_CODE
#include "MemMap.h"

extern void Ea_Init(void);
extern Std_ReturnType Ea_Read(uint16 BlockNumber, uint16 BlockOffset,
					uint8* DataBufferPtr, uint16 Length);
extern Std_ReturnType Ea_Write(uint16 BlockNumber, const uint8* DataBufferPtr);
extern MemIf_StatusType Ea_GetStatus(void);
extern MemIf_JobResultType Ea_GetJobResult(void);
extern void Ea_MainFunction(void);
extern void Ea_GetVersionInfo(Std_VersionInfoType* versioninfo);
#define EA_STOP_SEC_CODE
#include "MemMap.h"


#endif /* EA_H_ */
