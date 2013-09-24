#ifndef _LIB_I2C_PRIVATE_H_
#define _LIB_I2C_PRIVATE_H_

/*
 * Private Defines
 */

#define MAKE_SM_FUNCTION(mode) I2C_ ## mode ## _GetSM(void)
#define MAKE_START_FUNCTION(mode) I2C_ ## mode ## _Start(void)
#define NULL_I2C_DEFINES(mode) \
	#define MAKE_SM_FUNCTION(mode) NULL \
	#define MAKE_START_FUNCTION(mode) NULL

/*
 * Private Typedefs
 */
enum i2c_state_enum
{
	I2CS_IDLE,
	I2CS_STARTING,
	I2CS_ADDRESSING,
	I2CS_TRANSFERRING,
};
typedef enum i2c_state_enum I2C_STATE_ENUM;

enum i2c_mode_enum
{
	I2CM_MT,
	I2CM_MR,
	I2CM_ST,
	I2CM_SR
};
typedef enum i2c_mode_enum I2C_MODE_ENUM;

typedef void (*I2C_SM_FUNC)(void);

struct i2c_statemachineentry_struct
{
	I2C_STATE_ENUM	currentState;
	uint8_t			event;
	I2C_SM_FUNC		handler;
	I2C_STATE_ENUM	nextState;
};
typedef struct i2c_statemachineentry_struct I2C_STATEMACHINEENTRY;

struct i2c_statemachine_struct
{
	bool	busy;
	uint8_t	currentEvent;
	uint8_t	currentState;
	I2C_STATEMACHINEENTRY* entries;
};
typedef struct i2c_statemachine_struct I2C_STATEMACHINE;

#ifdef I2C_MT
I2C_STATEMACHINE * I2C_MT_GetSM(void);
void I2C_MT_Start(void);
#else
NULL_I2C_DEFINES(MT);
#endif

I2C_STATEMACHINE * I2C_MR_GetSM(void);
I2C_STATEMACHINE * I2C_ST_GetSM(void);
I2C_STATEMACHINE * I2C_SR_GetSM(void);

I2C_TRANSFER_DATA * data(void);

void I2C_Done(bool success);

bool I2C_BufferFull(void);
bool I2C_BufferUsed(void);


void I2C_MR_Start(void);
bool I2C_ST_Start(void);
bool I2C_SR_Start(void);

void I2C_ST_RegisterHandler(I2C_SLAVE_HANDLER handler);
void I2C_SR_RegisterHandler(I2C_SLAVE_HANDLER handler);

#endif
