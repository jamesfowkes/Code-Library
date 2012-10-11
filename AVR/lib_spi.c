#include <stdbool.h>

#include "lib_spi_common.h"
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
#else
 #error unknown processor - add to spi.h
#endif

static SPICALLBACK callback = NULL;

void SPI_SetSlave(void)
{
	// Clear frequency selection bits from register
	uint8_t spcr = SPCR;
	spcr &= ~(1 << (SPR0 | SPR1));
	SPCR = spcr;
	
	// Setup IO for slave
	DDRB &= ~(1<<SPI_MOSI_PIN);
	DDRB |= (1<<SPI_MISO_PIN);
	DDRB &= ~(1<<SPI_SS_PIN);
	DDRB &= ~(1<<SPI_SCK_PIN);

}
void SPI_SetMaster(LIBSPI_MSTRCLK_ENUM eMstrFreq)
{
	uint8_t spcr = SPCR;
	uint8_t spsr = SPSR;

	// Clear then set the master frequency doubling bit
	#ifdef SPI2X
	spsr &= ~(1 << SPI2X);
	if ((LIBSPI_MSTRFREQ_FOSC2 == eMstrFreq) ||
		(LIBSPI_MSTRFREQ_FOSC8 == eMstrFreq) ||
		(LIBSPI_MSTRFREQ_FOSC32 == eMstrFreq) ||
		(LIBSPI_MSTRFREQ_FOSC64 == eMstrFreq))
	{
		spsr |= (1 << SPI2X) : 0;
		eMstrFreq = (LIBSPI_MSTRCLK_ENUM)((uint8_t)eMstrFreq >> 3); // Divide by 8 to get SPCR setting
	}
	
	SPSR = spsr;
	#endif
	
	// Clear and then set frequency bits
	spcr &= ~(1 << (SPR0 | SPR1));
	spcr |= (uint8_t)(eMstrFreq);
	SPCR = spcr;

	// Setup IO for master
	DDRB |= (1<<SPI_MOSI_PIN);
	DDRB &= ~(1<<SPI_MISO_PIN);
	DDRB |= (1<<SPI_SCK_PIN);
	DDRB |= (1<<SPI_SS_PIN);
}

void SPI_SetDataOrder(LIBSPI_DATAORDER_ENUM eOrder)
{
	uint8_t spcr = SPCR;
	
	// Clear and then set DORD bit
	spcr &= ~(1 << DORD);
	spcr |= (eOrder == LIBSPI_DATAORDER_LSBFIRST) ? (1 << DORD) : 0;
	
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

void SPI_SendByte(uint8_t byte, SPICALLBACK cb)
{
	assert(NULL == callback)
	
	callback = cb;
	
	SPDR = byte;
}

ISR(SPI_STC_vect)
{
	// Read byte from recieve register and post to callback
	uint8_t reply = SPDR;
	
	if (callback)
	{
		callback(reply);
	}
	callback = NULL;
}