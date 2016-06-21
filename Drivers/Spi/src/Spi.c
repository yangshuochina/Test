/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:21:36MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi.c  $
 *  Revision 1.1 2010/05/19 14:21:36MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include <string.h>

#include "Spi.h"
#include "Spi_Priv.h"

#define SPI_START_SEC_VAR_32BIT
#include "MemMap.h"

const Spi_ConfigType *spi_cfg;

#define SPI_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define SPI_START_SEC_CODE
#include "MemMap.h"


void Spi_Init(const Spi_ConfigType *cfg)
{
	uint8_least i;

	SPI_ASSERT_RETVOID(spi_cfg == NULL_PTR, SPI_INIT_API_ID, SPI_E_ALREADY_INITIALIZED);
	SPI_ASSERT_RETVOID(cfg != NULL_PTR, SPI_INIT_API_ID, SPI_E_PARAM_CONFIG);
	SPI_ASSERT_RETVOID(cfg->num_of_channels <= SPI_MAX_CHANNEL, SPI_INIT_API_ID, SPI_E_PARAM_CHANNEL);
	SPI_ASSERT_RETVOID(cfg->num_of_jobs <= SPI_MAX_JOB, SPI_INIT_API_ID, SPI_E_PARAM_JOB);
	SPI_ASSERT_RETVOID(cfg->num_of_sequences <= SPI_MAX_CHANNEL, SPI_INIT_API_ID, SPI_E_PARAM_SEQ);
	SPI_ASSERT_RETVOID(cfg->channels != NULL_PTR || cfg->num_of_channels==0, SPI_INIT_API_ID, SPI_E_PARAM_CHANNEL);
	SPI_ASSERT_RETVOID(cfg->jobs != NULL_PTR || cfg->num_of_jobs==0, SPI_INIT_API_ID, SPI_E_PARAM_JOB);
	SPI_ASSERT_RETVOID(cfg->sequences != NULL_PTR || cfg->num_of_sequences==0, SPI_INIT_API_ID, SPI_E_PARAM_SEQ);

	spi_cfg = cfg;

	for(i=0; i<SPI_MAX_UNIT; i++) {
		REG(SSC0_PISEL, i)->U = cfg->hw_unit[i].pisel;
	}

	/* clear the variables */
	memset(spi_unit, 0, sizeof(spi_unit));
	memset(spi_seq, 0, sizeof(spi_seq));
#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
	memset(spi_canceled, 0, sizeof(spi_canceled));
	memset(spi_cancel_req, 0, sizeof(spi_cancel_req));
#endif
}


Std_ReturnType Spi_DeInit(void)
{
	uint8_least i;

	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_DEINIT_API_ID, SPI_E_UNINIT);

	for(i=0; i<SPI_MAX_UNIT; i++) {
		if(spi_unit[i].spi_in_use) {
			return E_NOT_OK;
		}
	}

	/* go back to polling mode to let the init start with the default */
#if defined(SPI_LEVEL_DELIVERED) && SPI_LEVEL_DELIVERED == 2
	Spi_SetAsyncMode(SPI_POLLING_MODE);
#endif

	/* clear the variables */
	memset(spi_unit, 0, sizeof(spi_unit));
	memset(spi_seq, 0, sizeof(spi_seq));
#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
	memset(spi_canceled, 0, sizeof(spi_canceled));
	memset(spi_cancel_req, 0, sizeof(spi_cancel_req));
#endif
	spi_cfg = NULL_PTR;

	return E_OK;
}
#define SPI_STOP_SEC_CODE
#include "MemMap.h"


