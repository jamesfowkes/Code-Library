/*
 * lib_clk.h
 *
 *  Created on: 6 Jul 2012
 *      Author: james
 *      Description: Header file for TI MSP430 Clock Control
 */
#ifndef _CLK_H_
#define _CLK_H_

/*
 * The external oscillator(s) can operate in four drive modes of increasing speed (and power consumption)
 */

enum lib_clk_xtdrive_enum
{
        XTDRIVE_0, //Lowest current consumption for XT1 LF mode. XT1 oscillator operating range in HF mode is 4 MHz to 8 MHz
        XTDRIVE_1, // Increased drive strength for XT1 LF mode. XT1 oscillator operating range in HF mode is 8 MHz to 16 MHz.
        XTDRIVE_2, // Increased drive capability for XT1 LF mode. XT1 oscillator operating range in HF mode is 16 MHz to 24 MHz.
        XTDRIVE_3, //Maximum drive capability and maximum current consumption for XT1 LF mode. XT1 oscillator operating range in HF mode is 24 MHz to 32 MHz.
};
typedef enum lib_clk_xtdrive_enum LIB_CLK_XTDRIVE_ENUM;

/*
 * Clock SOURCES are inputs into the CS system, either external or internal oscillators
 */

enum lib_clk_clksrc_enum
{
	CLKSRC_XT1 = 0,       // External crystal/clock
	CLKSRC_VLO = 1,       // Internal very-low-power 10kHz oscillator
	CLKSRC_DCO = 3,       // Internal digital configurable oscillator
	CLKSRC_XT2 = 5,       // External crystal/clock (where avaliable)
};
typedef enum lib_clk_clksrc_enum LIB_CLK_CLKSRC_ENUM;

/*
 * Clock SIGNALs are internal clocks generated from the clock sources. They can be configured
 * for which source they run from and each have a configurable 1/2/4/8/16/32 frequency divider
 */
enum lib_clk_clksig_enum
{
	CLKSIG_MSTR,
	CLKSIG_SM,
	CLKSIG_AUX,
	CLKSIG_MOD
};
typedef enum lib_clk_clksig_enum LIB_CLK_CLKSIG_ENUM;

enum lib_clk_divideby_enum
{
	DIVIDEBY_1    = 1,
	DIVIDEBY_2    = 2,
	DIVIDEBY_4    = 4,
	DIVIDEBY_8    = 8,
	DIVIDEBY_16   = 16,
	DIVIDEBY_32   = 32
};
typedef enum lib_clk_divideby_enum LIB_CLK_DIVIDEBY_ENUM;

enum lib_clk_dcofreq_enum
{
	DCOFREQ_5MHZ33,
	DCOFREQ_6MHZ67,
	DCOFREQ_8MHZ,
	DCOFREQ_16MHZ,
	DCOFREQ_20MHZ,
	DCOFREQ_24MHZ,
};
typedef enum lib_clk_dcofreq_enum LIB_CLK_DCOFREQ_ENUM;

bool CLK_Init(void);

void CLK_SignalControl(LIB_CLK_CLKSIG_ENUM eClkSignal, LIB_CLK_DIVIDEBY_ENUM eDivideBy);
void CLK_SelectClockSource(LIB_CLK_CLKSIG_ENUM eClkSignal, LIB_CLK_CLKSRC_ENUM eClkSource);

void CLK_ConfigureXT1(bool enable, bool bypass, bool hfmode, LIB_CLK_XTDRIVE_ENUM eDrive);
void CLK_ConfigureXT2(bool enable, bool bypass, bool hfmode, LIB_CLK_XTDRIVE_ENUM eDrive);
void CLK_SMCLKControl(bool enable);

void CLK_DCO_SelectFrequency(LIB_CLK_DCOFREQ_ENUM eDcoFreq);

void CLK_RequestControl(LIB_CLK_CLKSIG_ENUM eClkSig, bool enable);

bool CLK_GetXT1FaultState(void);
bool CLK_GetXT2FaultState(void);

#endif
