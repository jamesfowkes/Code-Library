/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * Common and Generic Includes
 */
#include "lib_spi_common.h"

/*
 * AVR Library Includes
 */
#include "lib_io.h"
#include "lib_spi.h"

// Create alias for the different SPI chip pins - code assumes all on port B
#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__))
	#define SPI_SS_PIN PORTB0
	#define SPI_SCK_PIN PORTB1
	#define SPI_MOSI_PIN PORTB2
	#define SPI_MISO_PIN PORTB3
#elif (defined(__AVR_ATmega48__) || defined(_AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	#define SPI_SS_PIN PORTB2
	#define SPI_SCK_PIN PORTB5
	#define SPI_MOSI_PIN PORTB3
	#define SPI_MISO_PIN PORTB4
#elif (defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) \
	|| defined(__AVR_ATtiny24A__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny84A__))
	#include "lib_usi.h" // Need to use USI module
	#define SPI_SCK_PIN PORTA4
	#define SPI_MOSI_PIN PORTA6
	#define SPI_MISO_PIN PORTA5
	#define DDR	DDRA
#else
 #error unknown processor - add to spi.h
#endif

#if (defined(SPCR) && defined(SPSR) && defined(DORD) && defined (CPOL))

SPI_DATA * pData = NULL;

void SPI_SetSlave(uint8_t firstByte, SPI_DATA * data)
{
	// Clear frequency selection bits from register
	uint8_t spcr = SPCR;
	spcr &= ~(1 << (SPR0 | SPR1));
	
	// Setup IO for slave
	DDRB &= ~(1<<SPI_MOSI_PIN);
	DDRB |= (1<<SPI_MISO_PIN);
	DDRB &= ~(1<<SPI_SS_PIN);
	DDRB &= ~(1<<SPI_SCK_PIN);

	SPI_SetReply(firstByte, data);

	// Enable the SPI peripheral
	spcr |= (1 << SPE)| (1 << SPIE);

	SPCR = spcr;

}
void SPI_SetMaster(LIBSPI_MSTRFREQ_ENUM eMstrFreq)
{
	uint8_t spcr = SPCR;
	uint8_t spsr = SPSR;

	// Clear then set the master frequency doubling bit
	#ifdef SPI2X
	spsr &= ~(1 << SPI2X);
	if ((LIBSPI_MSTRFREQ_FOSC2 == eMstrFreq) ||
		(LIBSPI_MSTRFREQ_FOSC8 == eMstrFreq) ||
		(LIBSPI_MSTRFREQ_FOSC32 == eMstrFreq) ||
		(LIBSPI_MSTRFREQ_FOSC64X2 == eMstrFreq))
	{
		spsr |= (1 << SPI2X);
		eMstrFreq = (LIBSPI_MSTRFREQ_ENUM)((uint8_t)eMstrFreq >> 3); // Divide by 8 to get SPCR setting
	}
	
	SPSR = spsr;
	#endif
	
	// Clear and then set frequency bits
	spcr &= ~(1 << (SPR0 | SPR1));
	spcr |= (uint8_t)(eMstrFreq);

	// Setup IO for master
	IO_SetMode(IO_PORTB, SPI_MOSI_PIN, IO_MODE_OUTPUT);
	IO_SetMode(IO_PORTB, SPI_MISO_PIN, IO_MODE_INPUT);
	IO_SetMode(IO_PORTB, SPI_SCK_PIN, IO_MODE_OUTPUT);
	IO_SetMode(IO_PORTB, SPI_SS_PIN, IO_MODE_OUTPUT);

	// Enable the SPI peripheral and interrupts
	spcr |= (1 <<SPE) | (1 << MSTR) | (1 << SPIE);

	SPCR = spcr;
}

void SPI_SetDataOrder(LIBSPI_DATADIRECTION_ENUM eOrder)
{
	uint8_t spcr = SPCR;
	
	// Clear and then set DORD bit
	spcr &= ~(1 << DORD);
	spcr |= (eOrder == LIBSPI_DATADIRECTION_LSBFIRST) ? (1 << DORD) : 0;
	
	SPCR = spcr;
}

void SPI_SetClockPolarity(LIBSPI_CPOL_ENUM eCpol)
{
	uint8_t spcr = SPCR;
	
	// Clear and then set CPOL bit
	spcr &= ~(1 << CPOL);
	spcr |= (eCpol == LIBSPI_CPOL_ACTIVEHIGH) ? (1 << CPOL) : 0;
	
	SPCR = spcr;
}

void SPI_SetClockPhase(LIBSPI_CPHA_ENUM eCPha)
{
	uint8_t spcr = SPCR;
	
	// Clear and then set CPHA bit
	spcr &= ~(1 << CPHA);
	spcr |= (eCPha == LIBSPI_CPHA_1) ? (1 << CPHA) : 0;
	
	SPCR = spcr;
}

void SPI_AssertCS(bool assert)
{

}

void SPI_SendByte(uint8_t byte, SPI_DATA * data)
{
	pData = data;
	pData->transferComplete = false;
	SPDR = byte;
}

void SPI_SetReply(uint8_t byte, SPI_DATA * data)
{
	pData = data;
	pData->transferComplete = false;
	SPDR = byte;
}

ISR(SPI_STC_vect)
{
	if (pData)
	{
		pData->byte = SPDR;
		pData->transferComplete = true;
	}
}

#else

void SPI_SetMaster(LIBSPI_MSTRFREQ_ENUM eMstrFreq)
{
	USI_SetMode(USI_MODE_THREE_WIRE);
	USI_CounterInterruptEnable(true);
	USI_SetSources(USI_CLK_SRC_EXT_POS, USI_COUNT_SRC_EXT);

	// Setup IO for master
	DDR |= (1<<SPI_MOSI_PIN);
	DDR &= ~(1<<SPI_MISO_PIN);
	DDR |= (1<<SPI_SCK_PIN);
}

void SPI_SetSlave(uint8_t firstByte, SPI_DATA * data)
{
	USI_SetMode(USI_MODE_THREE_WIRE);
	USI_CounterInterruptEnable(true);
	USI_SetSources(USI_CLK_SRC_EXT_POS, USI_COUNT_SRC_EXT);

	// Setup IO for slave
	DDR &= ~(1<<SPI_MOSI_PIN);
	DDR |= (1<<SPI_MISO_PIN);
	DDR &= ~(1<<SPI_SCK_PIN);

	USI_SetReply(firstByte, (USI_DATA*)data);
}

void SPI_SendByte(uint8_t byte, SPI_DATA * data)
{
	/* USI output is not interrupt based, so just set the reply now */
	data->byte = USI_SendByte(byte);
	data->transferComplete = true;
}

void SPI_SetReply(uint8_t byte, SPI_DATA * data)
{
	USI_SetReply(byte, (USI_DATA*)data);
}

void SPI_SetClockPhase(LIBSPI_CPHA_ENUM eCPha)
{
	switch (eCPha)
	{
	case LIBSPI_CPHA_0:
		USI_SetSources(USI_CLK_SRC_EXT_NEG, USI_COUNT_SRC_EXT);
		break;
	case LIBSPI_CPHA_1:
		USI_SetSources(USI_CLK_SRC_EXT_POS, USI_COUNT_SRC_EXT);
		break;
	}
}

#endif

bool SPI_TestAndCallback(SPI_DATA * data)
{
	cli();
	bool complete = data->transferComplete;
	data->transferComplete = false;
	sei();
	
	if (complete && data->callback)
	{
		data->callback(data);
	}
	
	return complete;
}
