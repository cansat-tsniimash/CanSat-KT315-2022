#include <sd.h>
#include <includes.h>

#include <radio.h>

uint16_t parse_to_text_dose(char *buffer, rf_dosimeter_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(&(buf[0]), 500, "%u;%u;%lu;%lu;%lu;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.ticks_now, data.pack.ticks_sum, data.crc);
	memcpy(buffer, &buf, num_written);
	return num_written;
}

uint16_t parse_to_text_bmp(char *buffer, rf_bmp_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(&(buf[0]), 500, "%u;%u;%lu;%d;%lu;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.bmp_temperature, data.pack.bmp_pressure, data.crc);
	memcpy(buffer, &buf, num_written);
	return num_written;
}

uint16_t parse_to_text_ds(char *buffer, rf_ds_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(&(buf[0]), 500, "%u;%u;%lu;%f;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.ds18b20_temperature, data.crc);
	memcpy(buffer, &buf, num_written);
	return num_written;
}

uint16_t parse_to_text_gps(char *buffer, rf_gps_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(&(buf[0]), 500, "%u;%u;%lu;%f;%f;%d;%lu;%lu;%u;%u;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.longtitude, data.pack.latitude, data.pack.altitude, data.pack.time_s, data.pack.time_us, data.pack.fix, data.pack.status, data.crc);
	memcpy(buffer, &buf, num_written);
	return num_written;
}

uint16_t parse_to_text_inertial(char *buffer, rf_inertial_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(&(buf[0]), 500, "%u;%u;%lu;%d;%d;%d;%d;%d;%d;%d;%d;%d;%f;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.lsm_acc[0], data.pack.lsm_acc[1], data.pack.lsm_acc[2], data.pack.lsm_gyro[0], data.pack.lsm_gyro[1], data.pack.lsm_gyro[2], data.pack.lis_mag[0], data.pack.lis_mag[1], data.pack.lis_mag[2], data.pack.lux, data.crc);
	memcpy(buffer, &buf, num_written);
	return num_written;
}

uint16_t parse_to_text_sebastian(char *buffer, rf_sebastian_package_crc_t data) {
	char buf [500];
	uint16_t num_written = snprintf(&(buf[0]), 500, "%u;%u;%lu;%f;%f;%f;%f;%u\n", data.pack.flag, data.pack.num, data.pack.time_from_start, data.pack.quaternion[0], data.pack.quaternion[1], data.pack.quaternion[2], data.pack.quaternion[3], data.crc);
	memcpy(buffer, &buf, num_written);
	return num_written;
}
