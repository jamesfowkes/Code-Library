#ifndef _LIB_SPI_H_
#define _LIB_SPI_H_

enum libspi_mstrfreq_enum
{
	LIBSPI_MSTRFREQ_FOSC4		= 1,
	LIBSPI_MSTRFREQ_FOSC16		= 2,
	LIBSPI_MSTRFREQ_FOSC64		= 3,
	LIBSPI_MSTRFREQ_FOSC128 	= 4,
	
	// Frequency doubling with SPI2X - shift down 3 to get SPCR setting
	LIBSPI_MSTRFREQ_FOSC2		= (1 << 3),
	LIBSPI_MSTRFREQ_FOSC8		= (2 << 3),
	LIBSPI_MSTRFREQ_FOSC32		= (3 << 3),
	LIBSPI_MSTRFREQ_FOSC64X2	= (4 << 3),
};
typedef enum libspi_mstrfreq_enum LIBSPI_MSTRFREQ_ENUM;

void SPI_SetMaster(LIBSPI_MSTRFREQ_ENUM eMstrFreq);

#endif
