/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Transmit.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/02/02 09:18:02MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi_Transmit.c  $
 *  Revision 1.4 2011/02/02 09:18:02MEZ Cotor Vlad RGB (COTORV) 
 *  - Update the SW to clean up the LOG files #3
 *  Revision 1.3 2010/09/13 12:44:42MESZ Cotor Vlad RGB (COTORV) 
 *  Correction of SPI driver due to SPI transmission function.
 *  Revision 1.2 2010/07/12 09:51:21MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:22:14MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Spi.h"
#include "Spi_Priv.h"

#define SPI_START_SEC_VAR_32BIT
#include "MemMap.h"

Spi_UnitDataType spi_unit[SPI_MAX_UNIT];
Spi_SeqDataType spi_seq[SPI_MAX_SEQUENCE];

#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
BITARRAY(spi_canceled, SPI_MAX_SEQUENCE);
BITARRAY(spi_cancel_req, SPI_MAX_SEQUENCE);
#endif

#define SPI_STOP_SEC_VAR_32BIT
#include "MemMap.h"


#define SPI_START_SEC_CODE
#include "MemMap.h"



/*
 * Perform a priorization upon all started sequences of a single unit.
 * The priority of the next pending job of a sequence determines the
 * priorization.
 *
 * The next sequence ID that shall be started is stored in
 * spi_unit[unit].seq_active
 * If no sequence is found then this variable will become SPI_MAX_SEQUENCE
 */
static void Spi_Prioritize(Spi_HWUnitType unit)
{
	sint8_least priority = -1;
	Spi_SequenceType i;

	spi_unit[unit].seq_active = SPI_MAX_SEQUENCE;

	for(i=0; i<spi_cfg->num_of_sequences; i++) {
		if(spi_seq[i].started != NULL_PTR) {
			if(spi_seq[i].started->hw_unit == unit) {
				if(spi_seq[i].started->priority > priority) {
					priority = spi_seq[i].started->priority;
					spi_unit[unit].seq_active = i;
				}
			}
		}
	}
}


#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
#define isCanceled(seq_id) BITARRAY_GETBIT(spi_cancel_req, seq_id)

static void Spi_DoCancel(Spi_HWUnitType unit)
{
	Spi_SequenceType i;

	for(i=0; i<spi_cfg->num_of_sequences; i++) {
		if(spi_seq[i].started != NULL_PTR) {
			if(spi_seq[i].started->hw_unit == unit) {
				if(isCanceled(i) != 0) {
					BITARRAY_SETBIT(spi_canceled, i);
					BITARRAY_CLRBIT(spi_cancel_req, i);
					spi_seq[i].started = NULL_PTR;
				}
			}
		}
	}
}


#else
#define Spi_DoCancel(unit)
#define isCanceled(seq_id) 0
#endif

static void Spi_SendData(Spi_HWUnitType unit, const Spi_ConfigChannelType *channel)
{
    if(spi_unit[unit].tx_buf == NULL_PTR) {
		REG(SSC0_TB, unit)->U = channel->default_tx;
	}
	else {
		REG(SSC0_TB, unit)->U = spi_unit[unit].tx_buf[spi_unit[unit].tx_cnt];
	}
}


static void Spi_StartChannel(Spi_HWUnitType unit, const Spi_ConfigChannelType *channel)
{
	spi_unit[unit].tx_cnt = 0;
	spi_unit[unit].active_channel = channel;

	if(channel->eb_buf != NULL_PTR) {
		/* use the EB buffers */
		spi_unit[unit].tx_buf = channel->eb_buf->tx_buf;
		spi_unit[unit].rx_buf = channel->eb_buf->rx_buf;
		spi_unit[unit].tx_max = channel->eb_buf->cnt;
	}
	else {
		/* use the IB buffers */
		spi_unit[unit].tx_buf = channel->tx_buf;
		spi_unit[unit].rx_buf = channel->rx_buf;
		spi_unit[unit].tx_max = channel->buffer_size;
	}

	REG(SSC0_CON, unit)->B.BM = channel->tx_data_width - 1;
	REG(SSC0_CON, unit)->B.HB = channel->msb_first;

    Spi_SendData(unit, channel);
}


static void Spi_SequenceFinished(Spi_SequenceType seq_id)
{
	const Spi_ConfigSequenceType *seq = &spi_cfg->sequences[seq_id];

	spi_seq[seq_id].started = NULL_PTR;
	if(seq->notification != NULL_PTR) {
		seq->notification();
	}
}


static void Spi_StartJob(const Spi_ConfigJobType *job)
{
	Spi_HWUnitType unit = job->hw_unit;

	REG(SSC0_BR, unit)->U = job->baud_rate;
	REG(SSC0_CON, unit)->U = ((uint32)job->clock_phase << 5u)     | /* PH */
	                         ((uint32)job->clock_polarity << 6u)  | /* PO */
	                         (BITPOS(14u))                        | /* MS */
	                         (BITPOS(15u));                         /* END */
	Spi_SetCS(job);

	Spi_StartChannel(unit, job->first_ch);
}


static void Spi_JobFinished(const Spi_ConfigJobType *job)
{
	Spi_HWUnitType unit = job->hw_unit;
	const Spi_ConfigSequenceType *seq;
	Spi_SequenceType seq_id;

	Spi_ReleaseCS(job);

	if(job->notification != NULL_PTR) {
		job->notification();
	}
	seq = &spi_cfg->sequences[spi_unit[unit].seq_active];
	if(seq->last_job == job) {
		/* sequence is complete */
		Spi_SequenceFinished(spi_unit[unit].seq_active);
		Spi_DoCancel(unit);
		Spi_Prioritize(unit);
	}
	else {
		/* advance to next job in the sequence */
		seq_id = spi_unit[unit].seq_active;
		spi_seq[seq_id].started++;

		if(seq->is_interruptable || isCanceled(spi_unit[unit].seq_active)) {
			Spi_DoCancel(unit);
			Spi_Prioritize(unit);
		}

		if(spi_seq[seq_id].started->hw_unit != unit) {
			/* there was a change of the HW units for this sequence */
			/* is an immediate start on the other unit possible? */
			if(__swap((unsigned int *)&spi_unit[spi_seq[seq_id].started->hw_unit].spi_in_use, 1) == 0) {
				/* set this job as started and start with the first job */
				spi_unit[spi_seq[seq_id].started->hw_unit].seq_active = seq_id;
				Spi_StartJob(spi_seq[seq_id].started);
			}
			/* otherwise it becomes started as soon as the priority allows it */
		}
	}

	/* start the next job on this HW unit */
	seq_id = spi_unit[unit].seq_active;
	if(seq_id < SPI_MAX_SEQUENCE) {
		/* start the next job */
		Spi_StartJob(spi_seq[seq_id].started);
	}
	else {
		/* this SPI unit becomes free now */
		spi_unit[unit].spi_in_use = 0;
	}
}


/*
 * called if the receive interrupt is pending
 */
void Spi_InterruptHandler(Spi_HWUnitType unit)
{
	const Spi_ConfigChannelType *channel = spi_unit[unit].active_channel;
	const Spi_ConfigJobType *job;

	if(spi_unit[unit].rx_buf != NULL_PTR) {
		/* store the received data */
		spi_unit[unit].rx_buf[spi_unit[unit].tx_cnt] = (uint16)(REG(SSC0_RB, unit)->U);
	}
	spi_unit[unit].tx_cnt++;

	if(spi_unit[unit].tx_cnt < spi_unit[unit].tx_max) {
		Spi_SendData(unit, channel);
	}
	else {
		/* this channel is complete */
		job = spi_seq[spi_unit[unit].seq_active].started;
		if(job->last_ch == spi_unit[unit].active_channel) {
			/* job is complete */
			Spi_JobFinished(job);
		}
		else {
			/* next channel of the job */
			Spi_StartChannel(unit, spi_unit[unit].active_channel + 1);
		}
	}
}


static void Spi_StartSequence(Spi_SequenceType seq_id)
{
	const Spi_ConfigJobType *job;

#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
	BITARRAY_CLRBIT(spi_cancel_req, seq_id);
	BITARRAY_CLRBIT(spi_canceled, seq_id);
#endif
	job = spi_cfg->sequences[seq_id].first_job;

	/* immediate start possible? */
	if(__swap((unsigned int *)&spi_unit[job->hw_unit].spi_in_use, 1) == 0) {
		/* set this job as started and start with the first job */
		spi_seq[seq_id].started = job;
		spi_unit[job->hw_unit].seq_active = seq_id;
		Spi_StartJob(job);
	}
	else {
		/* set this job as pending */
		spi_seq[seq_id].started = job;
	}
}


#if defined(SPI_LEVEL_DELIVERED) && (SPI_LEVEL_DELIVERED >= 1)

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType seq_id)
{
	uint8 ts_error = 0;
	Spi_SequenceType i;
	const Spi_ConfigSequenceType *my_seq;
	const Spi_ConfigSequenceType *other_seq;

	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_ASYNCTRANSMIT_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(seq_id < spi_cfg->num_of_sequences, SPI_ASYNCTRANSMIT_API_ID, SPI_E_PARAM_SEQ);

	/* it is not allowed to start a sequence that is already started */
	if(spi_seq[seq_id].started != NULL_PTR) {
		ts_error = SPI_E_SEQ_PENDING;
	}

	/* it is not allowed to start a sequence that shares jobs with another sequence that is already active */
	my_seq = &spi_cfg->sequences[seq_id];
	for(i=0; i<spi_cfg->num_of_sequences; i++) {
		if(spi_seq[i].started) {
			other_seq = &spi_cfg->sequences[i];
			if(other_seq->first_job > my_seq->last_job) {
				/* the others first job is beyond my last job -> no problem */
			}
			else if(other_seq->last_job < my_seq->first_job) {
				/* the others first last job is before my first job -> no problem */
			}
			else {
				/* we have an overlap -> problem */
				ts_error = SPI_E_SEQ_PENDING;
			}
		}
	}

	if(ts_error == 0) {
		Spi_StartSequence(seq_id);
		return E_OK;
	}
	else {
		SPI_ASSERT_RETERR(0, SPI_ASYNCTRANSMIT_API_ID, ts_error);
		return E_NOT_OK;
	}
}

#endif



#if defined(SPI_LEVEL_DELIVERED) && ((SPI_LEVEL_DELIVERED == 0) || (SPI_LEVEL_DELIVERED == 2))

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType seq_id)
{
	uint8 ts_error = 0;

	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_SYNCTRANSMIT_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(seq_id < spi_cfg->num_of_sequences, SPI_SYNCTRANSMIT_API_ID, SPI_E_PARAM_SEQ);

	/* it is not allowed to start a sequence that is already started */
	if(spi_seq[seq_id].started != NULL_PTR) {
		ts_error = SPI_E_SEQ_PENDING;
	}

	if(ts_error == 0) {
		Spi_StartSequence(seq_id);

		while(spi_seq[seq_id].started != NULL_PTR) {
			Spi_MainFunction_Handling();
		}

		return E_OK;
	}
	else {
		SPI_ASSERT_RETERR(0, SPI_ASYNCTRANSMIT_API_ID, ts_error);
		return E_NOT_OK;
	}
}

#endif


#if defined(SPI_CANCEL_API) && (SPI_CANCEL_API == STD_ON)
void Spi_Cancel(Spi_SequenceType seq_id)
{
	SPI_ASSERT_RETVOID(spi_cfg != NULL_PTR, SPI_CANCEL_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETVOID(seq_id < spi_cfg->num_of_sequences, SPI_CANCEL_API_ID, SPI_E_PARAM_SEQ);

	BITARRAY_SETBIT(spi_cancel_req, seq_id);
}

#endif

#define SPI_STOP_SEC_CODE
#include "MemMap.h"

