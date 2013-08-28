/*
 * Each member of the struct is the location in a char
 * where the bit representing that segment is.
 */
struct seven_segment_map
{
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t F;
	uint8_t G;
	uint8_t DP;
};
typedef struct seven_segment_map SEVEN_SEGMENT_MAP;

uint8_t SSEG_CreateDigit(uint8_t digit, SEVEN_SEGMENT_MAP * map, bool tails);
void SSEG_AddDecimal(uint8_t *pDisplay, SEVEN_SEGMENT_MAP * map);
