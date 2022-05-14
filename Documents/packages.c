typedef struct __attribute__((packed)) {
	uint8_t flag;
	uint8_t BME_temperature;

	uint16_t LSM_acc_x;
	uint16_t LSM_acc_y;
	uint16_t LSM_acc_z;
	uint16_t LSM_gyro_x;
	uint16_t LSM_gyro_y;
	uint16_t LSM_gyro_z;

	uint16_t num;

	uint32_t time_from_start;
	//uint32_t time_real;
	uint32_t BME_pressure;
} rf_package1_t;

typedef struct __attribute__((packed)) {
	rf_package1_t pack;
	uint16_t crc;
} rf_package_crc_t;






	























typedef struct __attribute__((packed)) { // Отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	uint32_t tick_now;
	uint32_t tick_sum;
} rf_dose_package_t;

typedef struct __attribute__((packed)) {
	rf_slow_package_t pack;
	uint16_t crc;
} rf_dose_package_crc_t;



typedef struct __attribute__((packed)) { // Отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	int16_t BME_temperature;
	uint32_t BME_pressure;
} rf_bme_package_t;

typedef struct __attribute__((packed)) {
	rf_fast_package_t pack;
	uint16_t crc;
} rf_bme_package_crc_t;



typedef struct __attribute__((packed)) { // Отправка 
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

	uint8_t state;
} rf_gps_package_t;

typedef struct __attribute__((packed)) {
	rf_fast_package_t pack;
	uint16_t crc;
} rf_gps_package_crc_t;



typedef struct __attribute__((packed)) { // Отправка каждые n, надо тестить (~10 мс)
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