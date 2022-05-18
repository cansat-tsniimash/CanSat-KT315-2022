#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>

/* Begin RF Structures */

typedef struct __attribute__((packed)) { // Дозиметр, отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	uint32_t tick_now;
	uint32_t tick_sum;
} rf_dose_package_t;

typedef struct __attribute__((packed)) {
	rf_dose_package_t pack;
	uint16_t crc;
} rf_dose_package_crc_t;



typedef struct __attribute__((packed)) { // BMP-пакет, отправка каждую секунду (к кому-то ещё просто не влез)
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	int16_t BMP_temperature;
	uint32_t BMP_pressure;
} rf_bmp_package_t;

typedef struct __attribute__((packed)) {
	rf_bmp_package_t pack;
	uint16_t crc;
} rf_bmp_package_crc_t;



typedef struct __attribute__((packed)) { // GPS+DS18+status, отправка
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float longtitude;
	float latitude;
	int16_t altitude;
	uint32_t time_s;
	uint32_t time_us;
	uint8_t fix;

	uint16_t ds18b20_temp;

	uint8_t status;
} rf_gps_package_t;

typedef struct __attribute__((packed)) {
	rf_gps_package_t pack;
	uint16_t crc;
} rf_gps_package_crc_t;



typedef struct __attribute__((packed)) { // Инерциалка+фоторез, отправка каждые n, надо тестить (~10 мс)
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	int16_t LSM_acc_x;
	int16_t LSM_acc_y;
	int16_t LSM_acc_z;
	int16_t LSM_gyro_x;
	int16_t LSM_gyro_y;
	int16_t LSM_gyro_z;
	int16_t LIS_mag_x;
	int16_t LIS_mag_y;
	int16_t LIS_mag_z;
	float lux;
} rf_inertial_package_t;

typedef struct __attribute__((packed)) {
	rf_inertial_package_t pack;
	uint16_t crc;
} rf_inertial_package_crc_t;



typedef struct  __attribute__((packed)) { // Маджвик, отправка каждые ??
	float quaternion [4];
} rf_sebastian_package_t;

typedef struct __attribute__((packed)) {
	rf_sebastian_package_t pack;
	uint16_t crc;
} rf_sebastian_package_crc_t;

/* End RF Structures */


//Funcs for packing data for radio
void pack_rf_dose();
void pack_rf_bmp();
void pack_rf_gps();
void pack_rf_inertial();
void pack_rf_sebastian();

//Funcs for sending data by radio
void send_rf_dose();
void send_rf_bmp();
void send_rf_gps();
void send_rf_inertial();
void send_rf_sebastian();


#endif /* RADIO_H_ */
