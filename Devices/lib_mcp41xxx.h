#ifndef _LIB_MCP41XXX_H_
#define _LIB_MCP41XXX_H_

/*
 * Defines and Typedefs 
 */
 
struct mcp41xxx_struct
{
	uint8_t currentSteps;
	SPI_DATA spi;
};
typedef struct mcp41xxx_struct MCP41XXX;

enum pot_select
{
	POT_NONE,
	POT_1,
	POT_2,
	POT_BOTH
};
typedef enum pot_select POT_SELECT;

/*
 * Public Function Prototypes
 */
 
void MCP41xxx_InitDevice(MCP41XXX * dev);
void MCP41xxx_SetResistancePercent(MCP41XXX * dev, uint8_t percent, POT_SELECT ePot);
void MCP41xxx_SetResistanceSteps(MCP41XXX * dev, uint8_t steps, POT_SELECT ePot);

#endif
