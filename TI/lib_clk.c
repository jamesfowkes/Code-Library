/*
 * lib_clk.c
 *
 *  Created on: 12 Jul 2012
 *      Author: james
 *      Description: Library file for TI MSP430 CS Peripheral
 */
#include <stdbool.h>
#include <stdint.h>

#include "msp430.h"

#include "assert.h"

#include "lib_sys.h"
#include "lib_clk.h"

/*
 * Private Function Prototypes
 */
void UnlockCSRegisters(void);
void LockCSRegisters(void);
void OnOscillatorFault(void);

/*
 * Private Data
 */

/* ACLK, SMCLK, MCLK signals are controlled by register entries in these bit positions */
static const uint16_t signalShifts[] = {
        0, // CLKSIG_MSTR
        4, // CLKSIG_SM
        8, // CLKSIG_AUX
};

/*
 * Public Functions
 */

bool CLK_Init(void)
{
	bool success = true;
	success &= SYS_AttachHandler(SYSSRC_OSCFAULT, OnOscillatorFault);

	assert(success);

	return success;
}

void CLK_SignalControl(LIB_CLK_CLKSIG_ENUM eClkSignal, LIB_CLK_DIVIDEBY_ENUM eDivideBy)
{
	uint16_t csctl3 = 0;

	// Read the current state of the divider selection register
	csctl3 = CSCTL3;

	// Mask out and preserve all other bits except the signal of interest
	switch (eClkSignal)
	{
	case CLKSIG_AUX:
		csctl3 &= ~(DIVA0 | DIVA1 | DIVA2);
		break;
	case CLKSIG_MSTR:
		csctl3 &= ~(DIVM0 | DIVM1 | DIVM2);
		break;
	case CLKSIG_SM:
		csctl3 &= ~(DIVS0 | DIVS1 | DIVS2);
		break;
    case CLKSIG_MOD:
        assert(false);
		break;
	}

	if (CLKSIG_MOD != eClkSignal)
	{
		// Then OR in new division (shifted) and write
		csctl3 |= eDivideBy << signalShifts[eClkSignal];

		UnlockCSRegisters();
		CSCTL3 = csctl3;
		LockCSRegisters();
	}
}

void CLK_SelectClockSource(LIB_CLK_CLKSIG_ENUM eClkSignal, LIB_CLK_CLKSRC_ENUM eClkSource)
{
	uint16_t csctl2 = 0;

	// Read the current state of the source selection register
	csctl2 = CSCTL2;

	// Mask out and preserve all other bits except the signal of interest
	switch(eClkSignal)
	{
	case CLKSIG_AUX:
		csctl2 &= ~(SELA0 | SELA1 | SELA2);
		break;
	case CLKSIG_MSTR:
		csctl2 &= ~(SELM0 | SELM1 | SELM2);
		break;
	case CLKSIG_SM:
		csctl2 &= ~(SELS0 | SELS1 | SELS2);
		break;
	case CLKSIG_MOD:
		assert(false);
		break;
	}

	if (CLKSIG_MOD != eClkSignal)
	{
		// Then OR in new selection (shifted) and write
		csctl2 |= eClkSource << signalShifts[eClkSignal];

		UnlockCSRegisters();
		CSCTL2 = csctl2;
		LockCSRegisters();
	}
}

void CLK_DCO_SelectFrequency(LIB_CLK_DCOFREQ_ENUM eDcoFreq)
{
	uint16_t dcoSetting = 0;

	// Upper three selections need DCORSEL setting (frequency range)
	if (eDcoFreq == DCOFREQ_16MHZ ||
	  eDcoFreq == DCOFREQ_20MHZ ||
	  eDcoFreq == DCOFREQ_24MHZ)
	{
	dcoSetting |= DCORSEL;
	}

	// Select frequency
	switch (eDcoFreq)
	{
	case DCOFREQ_5MHZ33:
	case DCOFREQ_16MHZ:
		dcoSetting |= DCOFSEL_0;
		break;
	case DCOFREQ_6MHZ67:
	case DCOFREQ_20MHZ:
		dcoSetting |= DCOFSEL_1;
		break;
	case DCOFREQ_8MHZ:
	case DCOFREQ_24MHZ:
		dcoSetting |= DCOFSEL_3;
		break;
	}

	UnlockCSRegisters();
	CSCTL1 = dcoSetting;
	LockCSRegisters();
}

void CLK_ConfigureXT1(bool enable, bool bypass, bool hfmode, LIB_CLK_XTDRIVE_ENUM eDrive)
{
	uint16_t csctl4 = CSCTL4;

	// Mask out XT2 configuration
	csctl4 &= XT2DRIVE0 | XT2DRIVE1 | XT2BYPASS | XT2OFF;

	// Configure XT1 as requested
	csctl4 |= (uint16_t)eDrive << 6;
	csctl4 |= bypass ? XT1BYPASS : 0;
	csctl4 |= enable ? 0 : XT1OFF;
	csctl4 |= hfmode ? 0 : XTS;

	UnlockCSRegisters();
	CSCTL4 = csctl4;
	LockCSRegisters();
}

void CLK_ConfigureXT2(bool enable, bool bypass, bool hfmode, LIB_CLK_XTDRIVE_ENUM eDrive)
{
	uint16_t csctl4 = CSCTL4;

	// Mask out XT1 and SMCLK configuration
	csctl4 &= XT1DRIVE0 | XT1DRIVE1 | XT1BYPASS | XT1OFF | SMCLKOFF | XTS;

	// Configure XT1 as requested
	csctl4 |= (uint16_t)eDrive << 14;
	csctl4 |= bypass ? XT2BYPASS : 0;
	csctl4 |= enable ? 0 : XT2OFF;

	UnlockCSRegisters();
	CSCTL4 = csctl4;
	LockCSRegisters();
}

void CLK_SMCLKControl(bool enable)
{
	uint16_t csctl4 = CSCTL4;

	// Mask out XT1 ,XT2 and SMCLK configuration
	csctl4 &= XT1DRIVE0 | XT1DRIVE1 | XT1BYPASS | XT1OFF | XTS;
	csctl4 &= XT2DRIVE0 | XT2DRIVE1 | XT2BYPASS | XT2OFF;

	// Configure SMCLK as requested
	csctl4 |= enable ? 0 : SMCLKOFF;

	UnlockCSRegisters();
	CSCTL4 = csctl4;
	LockCSRegisters();
}

void CLK_RequestControl(LIB_CLK_CLKSIG_ENUM eClkSig, bool enable)
{
	uint16_t csctl6 = CSCTL6;

	switch(eClkSig)
	{
	case CLKSIG_MSTR:
		csctl6 |= enable ? MCLKREQEN : ~((uint16_t)MCLKREQEN);
		break;
	case CLKSIG_SM:
		csctl6 |= enable ? SMCLKREQEN : ~((uint16_t)MCLKREQEN);
		break;
	case CLKSIG_AUX:
		csctl6 |= enable ? ACLKREQEN : ~((uint16_t)MCLKREQEN);
		break;
	case CLKSIG_MOD:
		csctl6 |= enable ? MODCLKREQEN : ~((uint16_t)MCLKREQEN);
		break;
	}

	UnlockCSRegisters();
	CSCTL6 = csctl6;
	LockCSRegisters();
}

bool CLK_GetXT1FaultState(void)
{
	return ((CSCTL5 & XT1OFFG_L) == XT1OFFG_L);
}

bool CLK_GetXT2FaultState(void)
{
    return ((CSCTL5 & XT2OFFG_L) == XT2OFFG_L);
}

/*
 * Clock System Interrupt Callbacks
 */
void OnOscillatorFault(void)
{
	// Currently do nothing on oscillator fault!
}

/*
 * Private Functions
 */

void UnlockCSRegisters(void)
{
	CSCTL0_H = 0xA5;// Correct password for unlocking
}

void LockCSRegisters(void)
{
	CSCTL0_H = 0x00; // Any incorrect password will lock
}
