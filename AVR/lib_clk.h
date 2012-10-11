#ifndef _LIB_CLK_H_
#define _LIB_CLK_H_

enum lib_clk_src_enum
{
	LIB_CLK_SRC_RC,
	LIB_CLK_SRC_EXT
};
typedef enum lib_clk_src_enum LIB_CLK_SRC_ENUM;

bool CLK_Init(const uint32_t fExtSet);
void CLK_SourceSelect(LIB_CLK_SRC_ENUM eSourceSelect);
void CLK_SetPrescaler(const clock_div_t eSetPrescaler);
bool CLK_IsSourceRunning(LIB_CLK_SRC_ENUM eSource);

#endif
