#ifndef _LIB_MAX111X_H_
#define _LIB_MAX111X_H_

/* Clock type to match control byte */
enum max111x_clocktype_enum
{
	CLOCKTYPE_INTERNAL	= 0x00,
	CLOCKTYPE_EXTERNAL	= 0x01,
};
typedef enum max111x_clocktype_enum MAX111X_CLOCKTYPE_ENUM;

/* Power state to match control byte */
enum max111x_powerstate_enum
{
	POWERSTATE_POWERDOWN	= 0x00,
	POWERSTATE_OPERATIONAL	= 0x02,
};
typedef enum max111x_powerstate_enum MAX111X_POWERSTATE_ENUM;

/* Input type to match control byte */
enum max111x_inputtype_enum
{
	INPUTTYPE_DIFFERENTIAL	= 0x00,
	INPUTTYPE_SINGLEENDED	= 0x04,
};
typedef enum max111x_inputtype_enum MAX111X_INPUTTYPE_ENUM;

/* Polarity mode to match control byte */
enum max111x_polmode_enum
{
	POLMODE_BIPOLAR		= 0x00,
	POLMODE_UNIPOLAR	= 0x08,
};
typedef enum max111x_polmode_enum MAX111X_POLMODE_ENUM;

typedef (*void)(uint16_t result) MAX111X_CALLBACK;

void MAX111x_StartConversion(UINT16 channel, MAX111X_INPUTTYPE_ENUM eInputType, MAX111X_POLMODE_ENUM ePolMode, MAX111X_CLOCKTYPE_ENUM eClockType);

void MAX111x_SetPowerState(MAX111X_POWERSTATE_ENUM ePowerState);

#endif
