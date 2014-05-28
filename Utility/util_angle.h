#ifndef _UTIL_ANGLE_H_
#define _UTIL_ANGLE_H_

#ifdef __cplusplus
extern "C"{
#endif

uint16_t reciprocal_deg(int16_t a);
uint16_t reciprocal_tdeg(int16_t a);

uint16_t mirror_deg(int16_t a, int16_t mirror);
uint16_t mirror_tdeg(int16_t a, int16_t mirror);

#ifdef __cplusplus
};
#endif

#endif