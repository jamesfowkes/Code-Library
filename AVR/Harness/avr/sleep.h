#ifndef _SLEEP_H_
#define _SLEEP_H_

#define SLEEP_MODE_IDLE         0
#define SLEEP_MODE_PWR_DOWN     1
#define SLEEP_MODE_PWR_SAVE     2
#define SLEEP_MODE_ADC          3
#define SLEEP_MODE_STANDBY      4
#define SLEEP_MODE_EXT_STANDBY  5

#define set_sleep_mode(x) {(void)x;}
#define sleep_enable() {}
#define sleep_mode() {}
#define sleep_disable() {}
#endif
