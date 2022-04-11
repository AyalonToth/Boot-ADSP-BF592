#include <stdlib.h>
#include <sys/platform.h>

#include <services/int/adi_int.h>
#include <services/int/adi_sid.h>

#include "../common/spi.h"
#include "../common/flash.h"
#include "../target.h"

static struct spi_message
{
	uint8_t *tbuf;
	uint8_t *rbuf;
	int size;
} spi_message;

static void adi_spi_bf5xx_recv_handler(uint32_t iid, void *handlerArg)
{
	struct spi_message *message = (struct spi_message *) handlerArg;

	if (message->size == 0)
	{
		uint16_t config;

		adi_int_EnableInt(ADI_SID_SPI_DATA, false);

		config = *pSPI_CTL;
		config &= ~ TIMOD;
		config |= TDBR_CORE;
		*pSPI_CTL = config;

		if (message->rbuf)
			*message->rbuf = *pSPI_RDBR;
		else
		{
			uint8_t dummy;
			dummy = *pSPI_RDBR;
		}

		config &= ~ TIMOD;
		config |= RDBR_CORE;
		*pSPI_CTL = config;

		message->size--;
		return;
	}

	if (message->tbuf)
	{
		*pSPI_TDBR = *message->tbuf;
		message->tbuf++;
	}
	else
		/* Send an arbitary value */
		*pSPI_TDBR = 0;

	if (message->rbuf)
	{
		*message->rbuf = *pSPI_RDBR;
		message->rbuf++;
	}
	else
	{
		uint8_t dummy;
		dummy = *pSPI_RDBR;
	}

	message->size--;
}

static int adi_spi_bf5xx_open(void)
{
	ADI_INT_STATUS result;

	result = adi_int_InstallHandler(ADI_SID_SPI_DATA,
			adi_spi_bf5xx_recv_handler,
			&spi_message,
			false);
	if (result != ADI_INT_SUCCESS)
		return -1;

	return 0;
}

static int adi_spi_bf5xx_close(void)
{
	return 0;
}

static int adi_spi_bf5xx_config(const struct flash_info *fi)
{
	uint16_t config;

	/* Set SPI Clock (SCK) Divide Factor to 8 */
	*pSPI_BAUD = 4;

	/* Set SPI CONTROL */
	config = MSTR;

	if (fi->cpha)
		config |= CPHA;

	if (fi->cpol)
		config |= CPOL;

	if (fi->lsb_first)
		config |= LSBF;

	config |= EMISO;

	*pSPI_CTL = config;

	return 0;
}

static int adi_spi_bf5xx_enable(void)
{
	uint16_t config;

	config = *pSPI_CTL;
	config |= SPE;
	*pSPI_CTL = config;

	return 0;
}

static int adi_spi_bf5xx_disable(void)
{
	uint16_t config;

	config = *pSPI_CTL;
	config &= ~ SPE;
	*pSPI_CTL = config;

	return 0;
}

static int adi_spi_bf5xx_send(uint8_t *buf, int count)
{
	uint8_t dummy;

	if (count < 0)
		return -1;
	if (count == 0)
		return 0;

	*pSPI_TDBR = buf[0];
	spi_message.tbuf = buf + 1;
	spi_message.rbuf = NULL;
	spi_message.size = count - 1;
	adi_int_EnableInt(ADI_SID_SPI_DATA, true);
	dummy = *pSPI_RDBR;
	while (spi_message.size != -1);
	return 0;
}

static int adi_spi_bf5xx_recv(uint8_t *buf, int count)
{
	uint8_t dummy;
	uint16_t config;

	if (count < 0)
		return -1;
	if (count == 0)
		return 0;

	/* In our use case, adi_spi_bf5xx_recv always follows
     adi_spi_bf5xx_send.  So we need to drop the first
     read of RX buffer.  */
	*pSPI_TDBR = 0;
	dummy = *pSPI_RDBR;

	spi_message.tbuf = NULL;
	spi_message.rbuf = buf;
	spi_message.size = count - 1;
	adi_int_EnableInt(ADI_SID_SPI_DATA, true);
	while (spi_message.size != -1);

	return 0;
}

static int adi_spi_bf5xx_recv_until(int (*condition)(const uint8_t))
{
	uint8_t dummy, data;

	/* Trigger the transfer.  */
	dummy = *pSPI_RDBR;

	do
	{
		while (!(*pSPI_STAT & RXS));
		data = *pSPI_RDBR;
	}
	while (!condition(data));

	return 0;
}

static int adi_spi_bf5xx_set_mode(spi_mode_t mode)
{
	return 0;
}

static int adi_spi_bf5xx_select_slave(int slave_no)
{
	/* currently we only handle slave_no == 1 */
	switch (slave_no)
	{
	case 0:
		SPI_UNSELECT_SLAVE;
		break;

	case 1:
		SPI_SELECT_SLAVE;
		break;

	default:
		return -1;
	}

	return 0;
}

const struct spi_ctlr adi_spi_bf5xx_ctlr =
{
	adi_spi_bf5xx_open,
	adi_spi_bf5xx_close,
	adi_spi_bf5xx_config,
	adi_spi_bf5xx_enable,
	adi_spi_bf5xx_disable,
	adi_spi_bf5xx_send,
	adi_spi_bf5xx_recv,
	adi_spi_bf5xx_recv_until,
	adi_spi_bf5xx_set_mode,
	adi_spi_bf5xx_select_slave,
};
