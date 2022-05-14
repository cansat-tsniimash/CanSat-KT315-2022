#include "../sensors.h"
#include "../includes.h"


uint16_t package_num = 0;

void send_rf_package (nrf24_lower_api_config_t *nrf24_lowlevel_config) {
	nrf24_fifo_status(nrf24_lowlevel_config, rf_fifo_status_rx, rf_fifo_status_tx);
	if (rf_fifo_status_tx != NRF24_FIFO_FULL) {
		rf_package_crc = pack(bmp_data, lsm_data, package_num);
		package_num++;
		nrf24_fifo_write(nrf24_lowlevel_config, (uint8_t*) rf_package_crc, sizeof(rf_package_crc), false);
		nrf24_mode_tx(nrf24_lowlevel_config);
		HAL_Delay(3);
		nrf24_mode_standby(nrf24_lowlevel_config);
	} else {
		nrf24_fifo_flush_tx(nrf24_lowlevel_config);
		HAL_Delay(100);
	}
	nrf24_irq_clear(nrf24_lowlevel_config, NRF24_IRQ_RX_DR | NRF24_IRQ_TX_DR | NRF24_IRQ_MAX_RT);
}

unsigned short Crc16(unsigned char *buf, unsigned short len) {
	unsigned short crc = 0xFFFF;
	unsigned char i;
	while (len--) {
		crc ^= *buf++ << 8;
		for (i = 0; i < 8; i++)
			crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
	}
	return crc;
}


typedef struct __attribute__((packed)) { // Отправка каждую секунду
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

void send_rf_dose () {

}









typedef struct __attribute__((packed)) { // Отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	int16_t BME_temperature;
	uint32_t BME_pressure;
} rf_bme_package_t;

typedef struct __attribute__((packed)) {
	rf_bme_package_t pack;
	uint16_t crc;
} rf_bme_package_crc_t;

void send_rf_bme () {

}










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
	rf_gps_package_t pack;
	uint16_t crc;
} rf_gps_package_crc_t;

void send_rf_gps () {

}











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

void send_rf_inertial () {

}










typedef struct  __attribute__((packed)) { // Отправка каждые ??
	float quaternion [4];
} rf_sebastian_package_t;

typedef struct __attribute__((packed)) {
	rf_sebastian_package_t pack;
	uint16_t crc;
} rf_sebastian_package_crc_t;

void send_rf_sebastian () {

}




/* Begin radio data transmit

	nrf24_fifo_status(&nrf24_lowlevel_config, &rf_fifo_status_rx, &rf_fifo_status_tx);
	if (rf_fifo_status_tx != NRF24_FIFO_FULL) {
		rf_package_crc = pack(&bmp_data, &lsm_data, package_num);
		package_num++;
		nrf24_fifo_write(&nrf24_lowlevel_config, (uint8_t*) &rf_package_crc, sizeof(rf_package_crc), false);
		nrf24_mode_tx(&nrf24_lowlevel_config);
		HAL_Delay(3);
		nrf24_mode_standby(&nrf24_lowlevel_config);
	} else {
		nrf24_fifo_flush_tx(&nrf24_lowlevel_config);
		HAL_Delay(100);
	}
	nrf24_irq_clear(&nrf24_lowlevel_config, NRF24_IRQ_RX_DR | NRF24_IRQ_TX_DR | NRF24_IRQ_MAX_RT);

End radio data transmit */




