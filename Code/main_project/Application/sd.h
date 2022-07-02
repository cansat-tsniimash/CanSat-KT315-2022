#ifndef SD_H_
#define SD_H_

#include <includes.h>
#include <radio.h>

#define SD_REBOOT_DELAY 1800

typedef struct __attribute__((packed)) {
	double ground_pressure;
	float ground_lux_rckt;
	uint8_t status;
	uint8_t reboot_counter;
} sd_defaults_t;

typedef struct __attribute__((packed)) {
	sd_defaults_t pack;
	uint16_t crc;
} sd_defaults_crc_t;

void update_status_in_defaults(FATFS* file_system, FIL* file, const char* path, UINT* bytes_written, sd_defaults_crc_t *data, uint8_t status, uint8_t reboot_count);
uint16_t sd_parse_to_bytes_dosimeter(char *buffer, rf_dosimeter_package_crc_t *data);
uint16_t sd_parse_to_bytes_bmp(char *buffer, rf_bmp_package_crc_t *data);
uint16_t sd_parse_to_bytes_ds(char *buffer, rf_ds_package_crc_t *data);
uint16_t sd_parse_to_bytes_gps(char *buffer, rf_gps_package_crc_t *data, uint64_t time_s);
uint16_t sd_parse_to_bytes_inertial(char *buffer, rf_inertial_package_crc_t *data, float lsm_acc [3], float lsm_gyro [3], float lis_mag [3]);
uint16_t sd_parse_to_bytes_sebastian(char *buffer, rf_sebastian_package_crc_t *data);

void file_system_mount(FATFS* file_system);
void file_open(FATFS* file_system, FIL* file, const char* path, BYTE mode);
void file_read(FATFS* file_system, FIL* file, const char* path, void* buffer, uint16_t bytes_to_read, UINT* bytes_read);
void file_puts(FATFS* file_system, FIL* file, const char* path, const char* str);
void file_write(FATFS* file_system, FIL* file, const char* path, void* buffer, uint16_t buffer_size, UINT* bytes_written);
void file_sync(FATFS* file_system, FIL* file, const char* path);

#endif /* SD_H_ */
