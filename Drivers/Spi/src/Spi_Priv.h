/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Priv.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:22:08MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Spi_Priv.h  $
 *  Revision 1.1 2010/05/19 14:22:08MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef SPI_PRIV_H_
#define SPI_PRIV_H_

#include "team-s.h"
#include "Spi_Cfg.h"


#if defined(SPI_DEV_ERROR_DETECT) && (SPI_DEV_ERROR_DETECT == STD_ON)
#  define SPI_ASSERT_RETERR(cond, api_id, error_id)  TEAMS_ASSERT_RETERR(cond, SPI_MODULE_ID, SPI_INSTANCE_ID, api_id, error_id)
#  define SPI_ASSERT_RETVOID(cond, api_id, error_id) TEAMS_ASSERT_RETVOID(cond, SPI_MODULE_ID, SPI_INSTANCE_ID, api_id, error_id)
#  define SPI_ASSERT(cond, api_id, error_id)         TEAMS_ASSERT(cond, SPI_MODULE_ID, SPI_INSTANCE_ID, api_id, error_id)
#else
#  define SPI_ASSERT_RETERR(cond, api_id, error_id)
#  define SPI_ASSERT_RETVOID(cond, api_id, error_id)
#  define SPI_ASSERT(cond, api_id, error_id)
#endif


#define REG(reg, unit)  ((reg##_type *) ((volatile uint8 *)(&(reg)) + (0x100 * (unit))))



#define SPI_START_SEC_VAR_32BIT
#include "MemMap.h"

extern const Spi_ConfigType *spi_cfg;

typedef struct {
	/* channel related data */
	uint32 spi_in_use;
	const Spi_ConfigChannelType *active_channel;
	const Spi_DataType *tx_buf;
	Spi_DataType *rx_buf;
	Spi_NumberOfDataType tx_cnt;
	Spi_NumberOfDataType tx_max;

	/* sequence related data */
	Spi_SequenceType seq_active;
} Spi_UnitDataType;
extern Spi_UnitDataType spi_unit[SPI_MAX_UNIT];


/* sequence related data */
typedef struct {
	const Spi_ConfigJobType *started;
} Spi_SeqDataType;
extern Spi_SeqDataType spi_seq[SPI_MAX_SEQUENCE];


#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
extern BITARRAY(spi_canceled, SPI_MAX_SEQUENCE);
extern BITARRAY(spi_cancel_req, SPI_MAX_SEQUENCE);
#endif


#define SPI_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define SPI_START_SEC_CODE
#include "MemMap.h"

void Spi_SetCS(const Spi_ConfigJobType *job);
void Spi_ReleaseCS(const Spi_ConfigJobType *job);
void Spi_InterruptHandler(Spi_HWUnitType unit);

#define SPI_STOP_SEC_CODE
#include "MemMap.h"


#endif /* SPI_PRIV_H_ */


