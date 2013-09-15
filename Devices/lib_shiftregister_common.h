#ifndef _LIB_SHIFTREGISTERCOMMON_H_
#define _LIB_SHIFTREGISTERCOMMON_H_

/*
 * Public Datatype Definitions
 */

enum sr_clkedge_enum
{
	SR_CLKEDGE_POS,
	SR_CLKEDGE_NEG,
};
typedef enum sr_clkedge_enum SR_CLKEDGE_ENUM;

typedef void (*SR_IO_FN)(bool);
typedef void (*SR_SHIFT_FN)(uint8_t* data, uint8_t nBytes, SR_CLKEDGE_ENUM edge);

struct sr_control_struct
{
	SR_SHIFT_FN shiftOutFn;
};
typedef struct sr_control_struct SR_CONTROL;

#endif
