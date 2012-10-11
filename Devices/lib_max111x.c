
#include <stdbool.h>

#include "max111x_lib.h"

/* MAX111x Control Byte Format */

/* Bits 4-6 - Defines the channel selection */
#define CH_BIT		4
#define	CH_LEN		3
#define CH_MASK		((1 << CH_LEN) - 1)
/* Bit7: START - Defines the beginning of the control byte */
#define START_BIT	7
#define	START		(1 << START_BIT)

/* Conversion states */
enum max111x_state_enum
{
	IDLE,		// Waiting for a new conversion to start
	SENTSTART,	// Sent the start byte
	SENTNULL1	// Sent first NULL byte
	SENTNULL2	// Sent second NULL byte
};
typedef enum max111x_state_enum MAX111X_STATE_ENUM;

void SPICallback(uint8_t reply);

static MAX111X_STATE_ENUM		state		= IDLE;
static uint16_t					result		= 0;
static MAX111X_CALLBACK			callback	= NULL;
static MAX111X_POWERSTATE_ENUM	powerState	= POWERSTATE_POWERDOWN;
static MAX111X_CLOCKTYPE_ENUM	clockType 	= CLOCKTYPE_EXTERNAL;

void MAX111x_StartConversion(uint16_t channel, MAX111X_INPUTTYPE_ENUM eInputType, MAX111X_POLMODE_ENUM ePolMode, MAX111X_CALLBACK cb)
{
	assert(channel < 8);
	assert(state == IDLE);
	assert(cb == NULL);
	
	callback = cb;
	
	/* Build the control byte */
	uint8_t control = 0;
	
	control |= (uint8_t)clockType;							// Bit0: Clock type
	control |= (uint8_t)powerState;							// Bit1: Power state to return to after conversion
	control |= (uint8_t)eInputType;							// Bit2: Single/differential input selection
	control |= (uint8_t)ePolMode;							// Bit3: Unipolar/bipolar mode selection
	control |= (uint8_t)((channel << CH_BIT) & CH_MASK);	// Bits 4-6: Channel selection
	control |= START;										// Bit7: Start conversion
	
	// TODO: Send over SPI
	// SPI_SetClockPolarity(SPICPOL_ACTIVELOW);
	// SPI_SetClockPhase(SPICPHA_1);
	// SPI_AssertCS(true);
	// SPI_SendByte(control, SPICallback)
	
	state = SENTSTART;
}

void MAX111x_SetPowerState(MAX111X_POWERSTATE_ENUM ePowerState)
{
	powerState = ePowerState;
}

void MAX111x_SetClockType(MAX111X_CLOCKTYPE_ENUM eClockType)
{
	clockType = eClockType;
}
void SPICallback(uint8_t reply)
{
	assert( (state = SENTSTART) || (state = SENTNULL1) || (state = SENTNULL2));
	
	switch (state)
	{
	case SENTSTART:
		// Ignore the reply, send first null byte
		// TODO: Send over SPI
		// SPI_SendByte(0, SPICallback);
		state = SENTNULL1;
		break;
		
	case SENTNULL1:
		// Reply contains upper 6 bits of result
		assert(result == 0);
		result |= (uint16_t)(reply << 8);
		
		// Send the next null byte
		// TODO: Send over SPI
		// SPI_SendByte(0, SPICallback);
		state = SENTNULL2;
		break;
		
	case SENTNULL2:
		// Reply contains lower 2 bits of result
		result |= (uint16_t)(reply);
		result >>= 6;
		
		// TODO: De-assert CS
		// SPI_AssertCS(false);
		state = IDLE;
		
		if (callback)
		{
			callback(result);
		}
		callback = NULL;
		break;
		
	default:
		assert(false);
		break;
	}
}
