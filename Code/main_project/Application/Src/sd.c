#include <sd.h>
#include <includes.h>

#include <radio.h>

uint16_t sd_parse_to_text_dosimeter(char *buffer, rf_dosimeter_package_crc_t data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(buffer, 300, "%u;%u;%lu;%lu;%lu;%lu;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.ticks_per_last_second, data.pack.ticks_per_last_minute, data.pack.ticks_sum, data.crc);
	return num_written;
}

uint16_t sd_parse_to_text_bmp(char *buffer, rf_bmp_package_crc_t data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(buffer, 300, "%u;%u;%lu;%d;%lu;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.bmp_temperature, data.pack.bmp_pressure, data.crc);
	return num_written;
}

uint16_t sd_parse_to_text_ds(char *buffer, rf_ds_package_crc_t data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(buffer, 300, "%u;%u;%lu;%f;%f;%f;%u;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.ds18b20_temperature, data.pack.rocket_lux, data.pack.seed_lux, data.pack.status, data.crc);
	return num_written;
}

uint16_t sd_parse_to_text_gps(char *buffer, rf_gps_package_crc_t data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(buffer, 300, "%u;%u;%lu;%f;%f;%d;%lu;%lu;%u;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.longtitude, data.pack.latitude, data.pack.altitude, data.pack.time_sec, data.pack.time_microsec, data.pack.fix, data.crc);
	return num_written;
}

uint16_t sd_parse_to_text_inertial(char *buffer, rf_inertial_package_crc_t data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(buffer, 300, "%u;%u;%lu;%d;%d;%d;%d;%d;%d;%d;%d;%d;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.lsm_acc[0], data.pack.lsm_acc[1], data.pack.lsm_acc[2], data.pack.lsm_gyro[0], data.pack.lsm_gyro[1], data.pack.lsm_gyro[2], data.pack.lis_mag[0], data.pack.lis_mag[1], data.pack.lis_mag[2], data.crc);
	return num_written;
}

uint16_t sd_parse_to_text_sebastian(char *buffer, rf_sebastian_package_crc_t data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(buffer, 300, "%u;%u;%lu;%f;%f;%f;%f;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.quaternion[0], data.pack.quaternion[1], data.pack.quaternion[2], data.pack.quaternion[3], data.crc);
	return num_written;
}
