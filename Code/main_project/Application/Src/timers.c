#include <timers.h>
#include <includes.h>

static uint32_t ds18b20_last_measurement = 0;
static uint32_t sd_last_sync = 0;
static uint32_t nrf_last_status_change = 0;
static bool nrf_status = NRF_ON;

void timer_update_ds18b20(void) {
	ds18b20_last_measurement = HAL_GetTick();
	return;
}

void timer_update_sd(void) {
	sd_last_sync = HAL_GetTick();
	return;
}

static void timer_update_nrf(void) {
	nrf_last_status_change = HAL_GetTick();
	return;
}

static void status_update_nrf(void) {
	if (nrf_status) {
		if (HAL_GetTick() - nrf_last_status_change > NRF_DELAY_ON) {
			nrf_status = NRF_OFF;
			timer_update_nrf();
		}
	} else {
		if (HAL_GetTick() - nrf_last_status_change > NRF_DELAY_OFF) {
			nrf_status = NRF_ON;
			timer_update_nrf();
		}
	}
}

bool timecheck_ds18b20(void) {
	if (HAL_GetTick() - ds18b20_last_measurement < DS18_DELAY) {
		return false;
	} else {
		return true;
	}
}

bool timecheck_sd(void) {
	if (HAL_GetTick() - sd_last_sync < SD_DELAY) {
		return false;
	} else {
		return true;
	}
}

bool timecheck_nrf(void) {
	status_update_nrf();
	return nrf_status;
}

bool timecheck_dosimeter(void) {
	if (dosimeter_1sec_past()) {
		return true;
	} else {
		return false;
	}
}
