/*****************************************************************************
 * bf592_m25p16_dpia.c
 *****************************************************************************/

#include <sys/platform.h>
#include <services/int/adi_sid.h>
#include "common/flash.h"
#include "common/spi.h"

static const char *pEzKitTitle = "ADSP-BF592 EZ-KIT Lite";

extern const struct spi_ctlr adi_spi_bf5xx_ctlr;
extern struct flash_info m25p16_info;

static void dpia_init(void)
{
	spi_ctlr = &adi_spi_bf5xx_ctlr;
	flash_info = &m25p16_info;

	/* Set up pinmux.  This can be removed when pinmux add-in
	 * is available for BF592.  */

#define SPI_SCK_PORTF_MUX  ((uint16_t) ((uint16_t) 0<<15))
#define SPI_MISO_PORTF_MUX  ((uint16_t) ((uint16_t) 0<<14))
#define SPI_MOSI_PORTF_MUX  ((uint16_t) ((uint16_t) 0<<13))

#define SPI_SCK_PORTF_FER  ((uint16_t) ((uint16_t) 1<<15))
#define SPI_MISO_PORTF_FER  ((uint16_t) ((uint16_t) 1<<14))
#define SPI_MOSI_PORTF_FER  ((uint16_t) ((uint16_t) 1<<13))

	/* PORTx_MUX registers */
	*pPORTF_MUX = SPI_SCK_PORTF_MUX | SPI_MISO_PORTF_MUX
			| SPI_MOSI_PORTF_MUX;

	/* PORTx_FER registers */
	*pPORTF_FER = SPI_SCK_PORTF_FER | SPI_MISO_PORTF_FER
			| SPI_MOSI_PORTF_FER;

	/* We use GPIO PF8 as SPI flash /CS.
	   We cannot use slave select since using it requires set CPHA to 1,
	   but we want CPHA 0.  */
	*pPORTF_FER &= 0xfeff;
	*pPORTFIO_DIR |= 0x100;
}

#include "common/dpia.c"
