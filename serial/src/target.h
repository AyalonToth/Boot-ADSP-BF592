#ifndef _TARGET_H_
#define _TARGET_H_

/* Define macros used by adi_spi_bf5xx.c */

#define ADI_SID_SPI_DATA		ADI_SID_DMA5
#define pSPI_TDBR				pSPI0_TDBR
#define pSPI_RDBR				pSPI0_RDBR
#define pSPI_BAUD				pSPI0_BAUD
#define pSPI_CTL				pSPI0_CTL
#define pSPI_STAT				pSPI0_STAT
#define SPI_UNSELECT_SLAVE		do { *pPORTFIO_SET = 0x100; } while (0)
#define SPI_SELECT_SLAVE		do { *pPORTFIO_CLEAR = 0x100; } while (0)

#endif /* _TARGET_H_ */
