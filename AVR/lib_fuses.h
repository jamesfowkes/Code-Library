/*
 * lib_fuses.h
 *
 *  Created on: 4 Mar 2013
 *      Author: james
 */

#ifndef LIB_FUSES_H_
#define LIB_FUSES_H_

/* Structure to store the fuse bytes */
struct fuse_settings
{
	uint8_t  fuse_low;      /* The low fuse setting */
	uint8_t  fuse_high;     /* The high fuse setting */
	uint8_t  fuse_extended; /* The extended fuse setting */
	uint8_t  lockbits;      /* The lockbits */
};
typedef struct fuse_settings FUSE_SETTINGS;

FUSE_SETTINGS * FUS_GetFuses(void);
bool FUS_IsClockDiv8Enabled(void);

#endif /* LIB_FUSES_H_ */
