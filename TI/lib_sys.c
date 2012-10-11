/*
 * lib_sys.c
 *
 *      Author: james
 *      Description: Library file for TI MSP430 SYS module
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <isr_compat.h>

#include "msp430.h"

#include "assert.h"
#include "lib_sys.h"

/*
 * Private Data
 */

static NMIHANDLER InterruptHandlers[SYSSRC_MAX];

/*
 * Public Functions
 */

bool SYS_Init(void)
{
	return true;
}

bool SYS_AttachHandler(LIB_SYS_INTERRUPTSOURCES_ENUM eSource, NMIHANDLER pHandler)
{

	assert(NULL == InterruptHandlers[eSource]);

	bool success = false;

	if (NULL == InterruptHandlers[eSource])
	{
		InterruptHandlers[eSource] = pHandler;
		success = true;
	}

	return success;
}

/*
 * Interrupt Vectors
 */

ISR(UNMI, UNMI_ISR)
{
	// Read the SYSUNIV value and jump to interrupt handler
	uint16_t sysuniv = SYSUNIV;
	NMIHANDLER pHandler = NULL;

	switch (sysuniv)
	{
	case SYSUNIV_NONE:
		return;
		break;
	case SYSUNIV_NMIIFG:
		pHandler = InterruptHandlers[SYSSRC_RST];
		break;
	case SYSUNIV_OFIFG:
		pHandler = InterruptHandlers[SYSSRC_OSCFAULT];
		break;
	default:
		assert(false);
		break;
	}

	if (pHandler) { pHandler(); }
}

ISR(SYSNMI, SYSNMI_ISR)
{

	// Read the SYSSNIV value and jump to interrupt handler
	uint16_t syssniv = SYSSNIV;
	NMIHANDLER pHandler = NULL;

	switch (syssniv)
	{
	case SYSSNIV_NONE:
		return;
	case SYSSNIV_SBDIFG:	// Single bit error
	case SYSSNIV_DBDIFG:	// Single bit error
	case SYSSNIV_ACCTIMIFG:	// Access time violation
	case SYSSNIV_ACCVIFG:	// Access violation
		pHandler = InterruptHandlers[SYSSRC_FRAMERROR];
		break;

	case SYSSNIV_MPUSEGIIFG:
	case SYSSNIV_MPUSEG1IFG:
	case SYSSNIV_MPUSEG2IFG:
	case SYSSNIV_MPUSEG3IFG:
		pHandler = InterruptHandlers[SYSSRC_SEGMENTVIOLATION];
		break;
	case SYSSNIV_VMAIFG:
		pHandler = InterruptHandlers[SYSSRC_VACANTACCESS];
		break;
	case SYSSNIV_JMBINIFG:
		pHandler = InterruptHandlers[SYSSRC_JTAGMAILIN];
		break;
	case SYSSNIV_JMBOUTIFG:
		pHandler = InterruptHandlers[SYSSRC_JTAGMAILOUT];
		break;
	default:
		assert(false);
		break;
	}

	if (pHandler) { pHandler(); }
}

ISR(RESET, RSTNMI_ISR)
{

	// Read the SYSRSTIV value and jump to interrupt handler
	uint16_t sysrstiv = SYSRSTIV;
	NMIHANDLER pHandler = NULL;

	switch (sysrstiv)
	{
	case SYSRSTIV_NONE:
		return;
	case SYSRSTIV_BOR:
		break;
	case SYSRSTIV_RSTNMI:
		break;
	case SYSRSTIV_DOBOR:
		break;
	case SYSRSTIV_LPM5WU:
		break;
	case SYSRSTIV_SECYV:
		break;
	case SYSRSTIV_SVSLIFG:
		break;
	case SYSRSTIV_SVSHIFG:
		break;
	case SYSRSTIV_RES10:
		break;
	case SYSRSTIV_RES12:
		break;
	case SYSRSTIV_DOPOR:
		break;
	case SYSRSTIV_WDTTO:
		break;
	case SYSRSTIV_WDTKEY:
		break;
	case SYSRSTIV_FRCTLPW:
		break;
	case SYSRSTIV_DBDIFG:
		break;
	case SYSRSTIV_PERF:
		break;
	case SYSRSTIV_PMMKEY:
		break;
	case SYSRSTIV_MPUKEY:
		break;
	case SYSRSTIV_CSKEY:
		break;
	case SYSRSTIV_MPUSEGIIFG:
	case SYSRSTIV_MPUSEG1IFG:
	case SYSRSTIV_MPUSEG2IFG:
	case SYSRSTIV_MPUSEG3IFG:
		pHandler = InterruptHandlers[SYSSRC_SEGMENTVIOLATION];
		break;
	default:
		assert(false);
		break;
	}

	if (pHandler) { pHandler(); }
}

