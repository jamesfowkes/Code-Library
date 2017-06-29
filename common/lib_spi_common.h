#ifndef _LIB_SPI_COMMON_H_
#define _LIB_SPI_COMMON_H_

/* SPI_DATA needs to be forward-declared */
typedef struct spi_data_struct SPI_DATA;

typedef void (*SPI_TRANSFER_COMPLETE_HANDLER)(SPI_DATA * data);

enum libspi_cpol_enum
{
	LIBSPI_CPOL_ACTIVELOW,
	LIBSPI_CPOL_ACTIVEHIGH,
};
typedef enum libspi_cpol_enum LIBSPI_CPOL_ENUM;

enum libspi_cpha_enum
{
	LIBSPI_CPHA_0,
	LIBSPI_CPHA_1,
};
typedef enum libspi_cpha_enum LIBSPI_CPHA_ENUM;

enum libspi_datadirection_enum
{
	LIBSPI_DATADIRECTION_MSBFIRST,
	LIBSPI_DATADIRECTION_LSBFIRST
};
typedef enum libspi_datadirection_enum LIBSPI_DATADIRECTION_ENUM;

struct spi_data_struct
{
	bool transferComplete;
	SPI_TRANSFER_COMPLETE_HANDLER callback;
	uint8_t byte;
};

void SPI_SetDataOrder(LIBSPI_DATADIRECTION_ENUM eOrder);
void SPI_SetClockPolarity(LIBSPI_CPOL_ENUM eCpol);
void SPI_SetClockPhase(LIBSPI_CPHA_ENUM eCPha);
void SPI_AssertCS(bool assert);

void SPI_SendByte(uint8_t byte, SPI_DATA * data);
void SPI_SetReply(uint8_t byte, SPI_DATA * data);

void SPI_SetSlave(uint8_t firstByte, SPI_DATA * data);

bool SPI_TestAndCallback(SPI_DATA * data);

#endif
