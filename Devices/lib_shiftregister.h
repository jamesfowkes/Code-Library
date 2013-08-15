#ifndef _LIB_SHIFTREGISTER_H_
#define _LIB_SHIFTREGISTER_H_

/*
 * Public Datatype Definitions
 */
 
enum sr_clkedge_enum
{
	SR_CLKEDGE_POS,
	SR_CLKEDGE_NEG,
};
tyepdef enum sr_clkedge_enum SR_CLKEDGE_ENUM;

struct sr_control_struct
{
	EXT_IO_FN clkFn;
	EXT_IO_FN dataFn;
	SR_CLKEDGE_ENUM edge;
};
typedef struct sr_control_struct SR_CONTROL;

void SR_ShiftOut(uint8_t* data, uint8_t nBytes, SR_CONTROL * ctrl);

#endif
