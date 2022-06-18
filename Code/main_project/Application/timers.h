#ifndef TIMERS_H_
#define TIMERS_H_

#include <includes.h>

void timer_update_ds18b20(void);
void timer_update_sd(void);
bool timecheck_ds18b20(void);
bool timecheck_sd(void);
bool timecheck_dosimeter(void);

#endif /* TIMERS_H_ */
