#ifndef _LIB_SWS_H_
#define _LIB_SWS_H_

/*
 * Public Datatype Definitions
 */

enum lib_sws_baudrate
{
	LIB_SWS_BAUD_300,
	LIB_SWS_BAUD_600,
	LIB_SWS_BAUD_1200,
	LIB_SWS_BAUD_2400,
	LIB_SWS_BAUD_4800,
	LIB_SWS_BAUD_9600,
};
typedef enum lib_sws_baudrate LIB_SWS_BAUDRATE_ENUM;

#ifndef PC_TEST_HARNESS
void SWS_Init(IO_PORT_ENUM ePort, uint8_t pin, LIB_SWS_BAUDRATE_ENUM eBaudrate);
void SWS_SetBaudrate(LIB_SWS_BAUDRATE_ENUM eBaudrate);
#endif
void SWS_Transmit(char const * const buffer, uint8_t size, void * args[], uint8_t nargs);

#endif
