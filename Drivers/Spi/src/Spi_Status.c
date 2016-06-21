/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Status.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:22:11MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi_Status.c  $
 *  Revision 1.1 2010/05/19 14:22:11MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Spi.h"
#include "Spi_Priv.h"


#define SPI_START_SEC_CODE
#include "MemMap.h"


Spi_StatusType Spi_GetStatus(void)
{
	Spi_HWUnitType unit;

	if(spi_cfg == NULL_PTR) {
		return SPI_UNINIT;
	}

	for(unit=0; unit<SPI_MAX_UNIT; unit++) {
		if(spi_unit[unit].spi_in_use != 0) {
			return SPI_BUSY;
		}
	}

	return SPI_IDLE;
}


Spi_JobResultType Spi_GetJobResult(Spi_JobType job_id)
{
	const Spi_ConfigJobType *job;
	Spi_SequenceType seq_id;

	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_GETJOBRESULT_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(job_id < spi_cfg->num_of_jobs, SPI_GETJOBRESULT_API_ID, SPI_E_PARAM_JOB);

	job = &spi_cfg->jobs[job_id];
	for(seq_id=0; seq_id<spi_cfg->num_of_sequences; seq_id++) {
		if(spi_seq[seq_id].started != NULL_PTR) {
			if(job >= spi_seq[seq_id].started && job <= spi_cfg->sequences[seq_id].last_job) {
				/* the job is part of a started sequence, so it is pending */
				return SPI_JOB_PENDING;
			}

		}
	}

	/* not contained in any started sequence, so it is finished */
	return SPI_JOB_OK;
}


Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType seq_id)
{
	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_GETSEQUENCERESULT_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(seq_id < spi_cfg->num_of_sequences, SPI_GETSEQUENCERESULT_API_ID, SPI_E_PARAM_SEQ);

#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
	if(BITARRAY_GETBIT(spi_canceled, seq_id) != 0) {
		return SPI_SEQ_CANCELLED;
	}
#endif

	if(spi_seq[seq_id].started == NULL_PTR) {
		return SPI_SEQ_OK;
	}
	else {
		return SPI_SEQ_PENDING;
	}
}



#if defined(SPI_HWUNITSTATUS_API) && (SPI_HWUNITSTATUS_API == STD_ON)

Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType unit)
{
	if(spi_cfg == NULL_PTR) {
		return SPI_UNINIT;
	}

	SPI_ASSERT_RETERR(unit < SPI_MAX_UNIT, SPI_GETHWUNITSTATUS_API_ID, SPI_E_PARAM_UNIT);

	if(spi_unit[unit].spi_in_use == 0) {
		return SPI_IDLE;
	}
	else {
		return SPI_BUSY;
	}
}

#endif

#define SPI_STOP_SEC_CODE
#include "MemMap.h"
