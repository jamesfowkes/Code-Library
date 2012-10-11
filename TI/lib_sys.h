/*
 * lib_sys.h
 *
 *  Created on: 1 Aug 2012
 *      Author: james
 */

#ifndef LIB_SYS_H_
#define LIB_SYS_H_

/*
 * The system and user non-maskable interrupts have these sources
 */
enum lib_sys_interruptsources_enum
{
	SYSSRC_RST,
	SYSSRC_OSCFAULT,
	SYSSRC_FRAMERROR,
	SYSSRC_SEGMENTVIOLATION,
	SYSSRC_VACANTACCESS,
	SYSSRC_JTAGMAILIN,
	SYSSRC_JTAGMAILOUT,
	SYSSRC_MAX
};
typedef enum lib_sys_interruptsources_enum LIB_SYS_INTERRUPTSOURCES_ENUM;

/*
 * An external handler for a source shall have this prototype
 */
typedef void (*NMIHANDLER)(void);

/*
 * Public Function Prototypes
 */
bool SYS_Init(void);
bool SYS_AttachHandler(LIB_SYS_INTERRUPTSOURCES_ENUM eSource, NMIHANDLER pHandler);

#endif /* LIB_SYS_H_ */
