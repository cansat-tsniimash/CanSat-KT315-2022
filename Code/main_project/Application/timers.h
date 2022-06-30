#ifndef TIMERS_H_
#define TIMERS_H_

#include <includes.h>

#define DS18_DELAY 750
#define SD_DELAY 1000
//#define NRF_DELAY_ON 5000
//#define NRF_ON true
//#define NRF_DELAY_OFF 1000
//#define NRF_OFF false

void timer_update_ds18b20(void);
void timer_update_sebastian(void);
uint16_t sebastian_get_delta(void);
void timer_update_sd_sync(void);
void timer_update_sd_reboot(void);

bool timecheck_ds18b20(void);
bool timecheck_sd(void);
//bool timecheck_nrf(void);
bool timecheck_dosimeter(void);

#endif /* TIMERS_H_ */
