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

void SWS_Init(IO_PORT_ENUM ePort, uint8_t pin, LIB_SWS_BAUDRATE_ENUM eBaudrate);
void SWS_SetBaudrate(LIB_SWS_BAUDRATE_ENUM eBaudrate);
void SWS_Transmit(uint8_t const * const buffer, uint8_t size);

#endif
