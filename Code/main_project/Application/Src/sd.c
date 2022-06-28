#include <sd.h>
#include <inttypes.h>
#include <includes.h>

#include <radio.h>

uint16_t sd_parse_to_bytes_dosimeter(char *buffer, rf_dosimeter_package_crc_t *data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(
			buffer, 300,
			"%"PRIu8";%"PRIu16";%"PRIu32";%"PRIu32";%"PRIu32";%"PRIu32";%"PRIu16"\n",
			data->pack.flag, data->pack.num, data->pack.time_from_start,
			data->pack.ticks_per_last_second, data->pack.ticks_per_last_minute, data->pack.ticks_sum,
			data->crc);
	return num_written;
}

uint16_t sd_parse_to_bytes_bmp(char *buffer, rf_bmp_package_crc_t *data, double temperature, double pressure) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(
			buffer, 300,
			"%"PRIu8";%"PRIu16";%"PRIu32";%f;%f;%"PRIu16"\n",
			data->pack.flag, data->pack.num, data->pack.time_from_start,
			temperature, pressure,
			data->crc);
	return num_written;
}

uint16_t sd_parse_to_bytes_ds(char *buffer, rf_ds_package_crc_t *data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(
			buffer, 300,
			"%"PRIu8";%"PRIu16";%"PRIu32";%f;%f;%f;%"PRIu8";%"PRIu16"\n",
			data->pack.flag, data->pack.num, data->pack.time_from_start,
			data->pack.ds18b20_temperature,
			data->pack.rocket_lux, data->pack.seed_lux,
			data->pack.status,
			data->crc);
	return num_written;
}

uint16_t sd_parse_to_bytes_gps(char *buffer, rf_gps_package_crc_t *data) {
	memset(buffer, 0, 300);
	uint32_t time_sec_high = data->pack.time_sec >> (4 * 8);
	uint32_t time_sec_low = data->pack.time_sec & 0xFFFFFFFF;
	uint16_t num_written = snprintf(
			buffer, 300,
			"%"PRIu8";%"PRIu16";%"PRIu32";%f;%f;%f;%"PRIu32";%"PRIu32";%"PRIu32";%"PRIu8";%"PRIu16"\n",
			data->pack.flag, data->pack.num, data->pack.time_from_start,
			data->pack.longtitude, data->pack.latitude, data->pack.altitude,
			time_sec_high, time_sec_low, data->pack.time_microsec,
			data->pack.fix,
			data->crc);
	return num_written;
}

uint16_t sd_parse_to_bytes_inertial(char *buffer, rf_inertial_package_crc_t *data, float lsm_acc [3], float lsm_gyro [3], float lis_mag [3]) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(
			buffer, 300,
			"%"PRIu8";%"PRIu16";%"PRIu32";%f;%f;%f;%f;%f;%f;%f;%f;%f;%"PRIu16"\n",
			data->pack.flag, data->pack.num, data->pack.time_from_start,
			lsm_acc[0], lsm_acc[1], lsm_acc[2],
			lsm_gyro[0], lsm_gyro[1], lsm_gyro[2],
			lis_mag[0], lis_mag[1], lis_mag[2],
			data->crc);
	return num_written;
}

uint16_t sd_parse_to_bytes_sebastian(char *buffer, rf_sebastian_package_crc_t *data) {
	memset(buffer, 0, 300);
	uint16_t num_written = snprintf(
			buffer, 300,
			"%"PRIu8";%"PRIu16";%"PRIu32";%f;%f;%f;%f;%"PRIu16"\n",
			data->pack.flag, data->pack.num, data->pack.time_from_start,
			data->pack.quaternion[0], data->pack.quaternion[1], data->pack.quaternion[2], data->pack.quaternion[3],
			data->crc);
	return num_written;
}

void file_system_mount(FATFS* file_system) {
	FRESULT fres = 0;
	fres = f_mount(file_system, "", 1);
	if (FR_OK != fres) {
		NVIC_SystemReset();
	}
	return;
}

void file_open(FATFS* file_system, FIL* file, const char* path) {
	FRESULT fres = 0;
	fres = f_open(file, path, FA_WRITE | FA_CREATE_NEW | FA_OPEN_APPEND);
	if (FR_OK != fres) {
		f_mount(NULL, "", 1);
		file_system_mount(file_system);
		fres = f_open(file, path, FA_WRITE | FA_CREATE_NEW | FA_OPEN_APPEND);
		if (FR_OK != fres) {
			NVIC_SystemReset();
		}
	}
	return;
}

void file_puts(FATFS* file_system, FIL* file, const char* path, const char* str) {
	FRESULT fres = 0;
	fres = f_puts(str, file);
	if (FR_OK != fres) {
		f_close(file);
		file_open(file_system, file, path);
		fres = f_puts(str, file);
		if (FR_OK != fres) {
			f_mount(NULL, "", 1);
			file_system_mount(file_system);
			file_open(file_system, file, path);
			fres = f_puts(str, file);
			if (FR_OK != fres) {
				NVIC_SystemReset();
			} else {
				f_puts("remounted, file error fixed\n", file);
			}
		} else {
			f_puts("reopened, file error fixed\n", file);
		}
	}
	return;
}

void file_write(FATFS* file_system, FIL* file, const char* path, char* buffer, uint16_t buffer_size, UINT* bytes_written) {
	FRESULT fres = 0;
	fres = f_write(file, buffer, buffer_size, bytes_written);
	if (FR_OK != fres) {
		f_close(file);
		file_open(file_system, file, path);
		fres = f_write(file, buffer, buffer_size, bytes_written);
		if (FR_OK != fres) {
			f_mount(NULL, "", 1);
			file_system_mount(file_system);
			file_open(file_system, file, path);
			fres = f_write(file, buffer, buffer_size, bytes_written);
			if (FR_OK != fres) {
				NVIC_SystemReset();
			} else {
				file_puts(file_system, file, path, "remounted, file error fixed\n");
			}
		} else {
			file_puts(file_system, file, path, "reopened, file error fixed\n");
		}
	}
	return;
}


