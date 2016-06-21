#ifndef EEP_CFG_H_
#define EEP_CFG_H_

#include "MemIf_Types.h"
#include "Eep.h"
#include "Spi.h"

#define EEP_DEV_ERROR_DETECT	STD_ON

#define EEP_INSTANCE_ID					 0

#define EEP_PAGE_SIZE					(256)

#define EEP_WRITE_WAIT_MS				(6u)

#define EEP_NORMAL_WRITE_BLOCK_SIZE		(EEP_PAGE_SIZE)
#define EEP_NORMAL_READ_BLOCK_SIZE		(EEP_PAGE_SIZE)
#define EEP_FAST_WRITE_BLOCK_SIZE		(EEP_PAGE_SIZE)
#define EEP_FAST_READ_BLOCK_SIZE		(EEP_PAGE_SIZE)
#define EEP_SIZE						((Eep_LengthType)(32*1024))
#define EEP_BASE_ADDRESS				((Eep_AddressType)0)




#endif /* EEP_CFG_H_ */
