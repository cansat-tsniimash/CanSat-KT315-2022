#include <timers.h>
#include <includes.h>

static uint32_t ds18b20_last_measurement = 0;
static uint32_t sd_last_sync = 0;

void timer_update_ds18b20(void) {
	ds18b20_last_measurement = HAL_GetTick();
	return;
}

void timer_update_sd(void) {
	sd_last_sync = HAL_GetTick();
	return;
}

bool timecheck_ds18b20(void) {
	if (HAL_GetTick() - ds18b20_last_measurement < 750) {
		return false;
	} else {
		return true;
	}
}

bool timecheck_sd(void) {
	if (HAL_GetTick() - sd_last_sync < 1000) {
		return false;
	} else {
		return true;
	}
}
