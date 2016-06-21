/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Spi_Buffer.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:21:56MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Spi_Buffer.c  $
 *  Revision 1.1 2010/05/19 14:21:56MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Spi/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Spi.h"
#include "Spi_Priv.h"


#define SPI_START_SEC_CODE
#include "MemMap.h"


#if defined(SPI_CHANNEL_BUFFERS_ALLOWED) && (SPI_CHANNEL_BUFFERS_ALLOWED == STD_ON)

Std_ReturnType Spi_SetupEB(Spi_ChannelType channel, const Spi_DataType *src_buf,
		Spi_DataType *dst_buf, Spi_NumberOfDataType length)
{
	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_SETUPEB_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(channel < spi_cfg->num_of_channels, SPI_SETUPEB_API_ID, SPI_E_PARAM_CHANNEL);
	SPI_ASSERT_RETERR(length <= spi_cfg->channels[channel].buffer_size, SPI_SETUPEB_API_ID, SPI_E_PARAM_LENGTH);
	SPI_ASSERT_RETERR(spi_cfg->channels[channel].eb_buf != NULL_PTR, SPI_SETUPEB_API_ID, SPI_E_PARAM_CONFIG);

	spi_cfg->channels[channel].eb_buf->cnt = length;
	spi_cfg->channels[channel].eb_buf->rx_buf = dst_buf;
	spi_cfg->channels[channel].eb_buf->tx_buf = src_buf;

	return E_OK;
}

#endif


Std_ReturnType Spi_WriteIB(Spi_ChannelType channel, const Spi_DataType *buffer)
{
	sint32 i;

	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_WRITEIB_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(channel < spi_cfg->num_of_channels, SPI_WRITEIB_API_ID, SPI_E_PARAM_CHANNEL);
	SPI_ASSERT_RETERR(spi_cfg->channels[channel].tx_buf != NULL_PTR, SPI_WRITEIB_API_ID, SPI_E_PARAM_LENGTH);

	for(i=0; i<spi_cfg->channels[channel].buffer_size; i++) {
		spi_cfg->channels[channel].tx_buf[i] = *buffer;
		buffer++;
	}

	return E_OK;
}


Std_ReturnType Spi_ReadIB(Spi_ChannelType channel, Spi_DataType *buffer)
{
	sint32 i;

	SPI_ASSERT_RETERR(spi_cfg != NULL_PTR, SPI_READIB_API_ID, SPI_E_UNINIT);
	SPI_ASSERT_RETERR(channel < spi_cfg->num_of_channels, SPI_READIB_API_ID, SPI_E_PARAM_CHANNEL);
	SPI_ASSERT_RETERR(spi_cfg->channels[channel].rx_buf != NULL_PTR, SPI_READIB_API_ID, SPI_E_PARAM_LENGTH);

	for(i=0; i<spi_cfg->channels[channel].buffer_size; i++) {
		*buffer = spi_cfg->channels[channel].rx_buf[i];
		buffer++;
	}

	return E_OK;
}

#define SPI_STOP_SEC_CODE
#include "MemMap.h"

