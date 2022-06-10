#include <timers.h>
#include <includes.h>

static uint32_t ds18b20_last_measurement = 0;
static uint32_t sd_last_sync = 0;

void timer_update_ds18b20() {
	ds18b20_last_measurement = HAL_GetTick();
	return;
}

void timer_update_sd() {
	sd_last_sync = HAL_GetTick();
	return;
}

bool timecheck_ds18b20() {
	if (HAL_GetTick - ds18b20_last_measurement < 750) {
		return false;
	} else {
		return true;
	}
}

bool timecheck_sd() {
	if (HAL_GetTick - sd_last_sync < 1000) {
		return false;
	} else {
		return true;
	}
}
