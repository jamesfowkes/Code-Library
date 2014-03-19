#ifndef _LIB_SWS_H_
#define _LIB_SWS_H_

/*
 * Public Datatype Definitions
 */

#define sws_var_dump(v, fmt) { void * p = &v; SWS_Transmit(#v "=" fmt, 0, &p, 1); }

enum lib_sws_baudrate
{
	LIB_SWS_BAUD_300 = 300,
	LIB_SWS_BAUD_600 = 600,
	LIB_SWS_BAUD_1200 = 1200,
	LIB_SWS_BAUD_2400 = 2400,
	LIB_SWS_BAUD_4800 = 4800,
	LIB_SWS_BAUD_9600 = 9600
};
typedef enum lib_sws_baudrate LIB_SWS_BAUDRATE_ENUM;

void SWS_TxInit(IO_PORT_ENUM ePort, uint8_t pin);
void SWS_RxInit(IO_PORT_ENUM ePort, uint8_t pin);
void SWS_SetBaudRate(LIB_SWS_BAUDRATE_ENUM eBaudrate);

uint8_t SWS_Receive(char * rxBuffer, uint8_t n, bool breakOnNull);
void SWS_SimpleTransmit(char const * const buffer);

void SWS_Transmit(char const * const buffer, uint8_t size, void * args[], uint8_t nargs);

#ifdef TEST_HARNESS
char * SWS_GetTxBuffer(void);
#endif

#endif
