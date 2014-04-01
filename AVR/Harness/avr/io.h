#ifndef _IO_H_
#define _IO_H_

/* IO Registers */
uint8_t PORTA;
uint8_t PORTB;
uint8_t PORTC;
uint8_t PORTD;

uint8_t PINA;
uint8_t PINB;
uint8_t PINC;
uint8_t PIND;

uint8_t DDRA;
uint8_t DDRB;
uint8_t DDRC;
uint8_t DDRD;

/* ADC Registers */
uint8_t ADCSRA;
uint8_t ADMUX;

uint8_t	ADCH;
uint8_t	ADCL;

/* ADC Defines */
#define ADEN	(7)
#define ADSC	(6)
#define ADFR	(5)
#define ADIF	(4)
#define ADIE	(3)
#define ADPS2	(2)
#define ADPS1	(1)
#define ADPS0	(0)

#define	REFS1   (7)
#define	REFS0   (6)
#define	ADLAR   (5)
#define	MUX3    (3)
#define	MUX2    (2)
#define	MUX1    (1)
#define	MUX0    (0)

/* USI Registers */

uint8_t USIDR;
uint8_t USRBR;
uint8_t USISR;
uint8_t USICR;

/* USI Defines */
#define USISIF	(7)
#define USIOIF  (6)
#define USIPF   (5)
#define USIDC   (4)
#define USICNT3 (3)
#define USICNT2 (2)
#define USICNT1 (1)
#define USICNT0	(0)

#define USISIE	(7)
#define USIOIE  (6)
#define USIWM1  (5)
#define USIWM0  (4)
#define USICS1  (3)
#define USICS0  (2)
#define USICLK  (1)
#define USITC   (0)

#endif
