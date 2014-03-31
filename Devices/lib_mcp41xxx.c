/* 
Standard Library Includes
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Common Library Includes
 */

#include "lib_spi_common.h"

/*
 * Devices Library Includes
 */

#include "lib_mcp41xxx.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/*
 * Defines and Typedefs
 */

#define POR_STEPS 128UL
#define MAX_STEPS 255UL

#define P0_BIT			(0)
#define P1_BIT			(1)

#define C0_BIT			(4)
#define C1_BIT			(5)

#define NULL_COMMAND	(0x00 << C0_BIT)
#define WRITE_COMMAND	(0x01 << C0_BIT)
#define SHDN_COMMAND	(0x02 << C0_BIT)

#define POT_SELECT_NONE	(0x00 << P0_BIT)
#define POT_SELECT_ONE	(0x01 << P0_BIT)
#define POT_SELECT_TWO	(0x02 << P0_BIT)
#define POT_SELECT_BOTH	(0x03 << P0_BIT)

/*
 * Private Function Prototypes
 */

static void setSteps(MCP41XXX * dev, uint8_t steps, POT_SELECT ePot);
static void localSPICallback(SPI_DATA * dev);

/*
 * Private Variables
 */
 
static MCP41XXX * s_pPendingWriteDevice;

/*
 * Public Function Definitions
 */

void MCP41xxx_InitDevice(MCP41XXX * dev)
{
	dev->currentSteps = POR_STEPS;
	dev->spi.callback = localSPICallback;
}

void MCP41xxx_SetResistancePercent(MCP41XXX * dev, uint8_t percent, POT_SELECT ePot)
{
	percent = min(percent, 100);
	uint32_t steps = div_round_pos(MAX_STEPS * (uint32_t)percent, 100UL);
	setSteps(dev, (uint16_t)steps, ePot);
}

void MCP41xxx_SetResistanceSteps(MCP41XXX * dev, uint8_t steps, POT_SELECT ePot)
{
	setSteps(dev, steps, ePot);
}

/*
 * Private Function Definitions
 */
 
static void setSteps(MCP41XXX * dev, uint8_t steps, POT_SELECT ePot)
{
	uint8_t byte = ((uint8_t)ePot) << P0_BIT;
	byte |= WRITE_COMMAND;
	
	s_pPendingWriteDevice = dev;
	dev->currentSteps = steps;
	
	SPI_AssertCS(true);
	SPI_SendByte(byte, &(dev->spi));
}

static void localSPICallback(SPI_DATA * spi)
{
	if (s_pPendingWriteDevice)
	{
		if (&(s_pPendingWriteDevice->spi) == spi)
		{
			MCP41XXX * dev = s_pPendingWriteDevice;
			s_pPendingWriteDevice = NULL;
			SPI_SendByte(dev->currentSteps, &(dev->spi));
		}
	}
	else
	{
		SPI_AssertCS(false);
	}
}
