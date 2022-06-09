#ifndef RADIO_H_
#define RADIO_H_

#include <includes.h>

typedef enum {
	RF_FLAG_DOSIMETER,
	RF_FLAG_BMP,
	RF_FLAG_DS,
	RF_GLAG_GPS,
	RF_FLAG_INERTIAL,
	RF_FLAG_SEBASTIAN
} rf_flag_t;

/* Begin RF Structures */

typedef struct __attribute__((packed)) { // Дозиметр, отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	uint32_t ticks_now;
	uint32_t ticks_sum;
} rf_dosimeter_package_t;

typedef struct __attribute__((packed)) {
	rf_dosimeter_package_t pack;
	uint16_t crc;
} rf_dosimeter_package_crc_t;



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



typedef struct __attribute__((packed)) { // DS18B20, отправка каждые ~750 мс
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float ds18b20_temperature;
} rf_ds_package_t;

typedef struct __attribute__((packed)) {
	rf_ds_package_t pack;
	uint16_t crc;
} rf_ds_package_crc_t;



typedef struct __attribute__((packed)) { // GPS+status, отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float longtitude;
	float latitude;
	int16_t altitude;
	uint32_t time_s;
	uint32_t time_us;
	uint8_t fix;

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

nrf24_spi_pins_sr_t nrf24_create_sr_descriptor(shift_reg_t *shift_reg, uint8_t pos_CE, uint8_t pos_CS);
nrf24_rf_config_t nrf24_create_rf_descriptor(nrf24_data_rate_t data_rate, nrf24_tx_power_t tx_power, uint8_t rf_channel);
nrf24_protocol_config_t nrf24_create_protocol_descriptor(nrf24_crc_size_t crc_size, nrf24_address_width_t address_width, bool en_dyn_payload_size, bool en_ack_payload, bool en_dyn_ack, uint8_t auto_retransmit_count, uint8_t auto_retransmit_delay);
nrf24_pipe_config_t nrf24_create_pipe_descriptor(bool enable_auto_ack, uint64_t address, int8_t payload_size);
void nrf24_init_stm32(nrf24_lower_api_config_t *nrf24_config_, SPI_HandleTypeDef *bus, nrf24_spi_pins_sr_t *nrf24_sr_setup_, nrf24_rf_config_t *nrf24_rf_setup_, nrf24_protocol_config_t *nrf24_protocol_setup_, nrf24_pipe_config_t *nrf24_pipe_setup_);



//Funcs for packing data for radio
rf_dosimeter_package_crc_t pack_rf_dosimeter(uint32_t ticks_last_sec, uint32_t ticks_sum);
void pack_rf_bmp();
void pack_rf_ds();
void pack_rf_gps();
void pack_rf_inertial();
void pack_rf_sebastian();

//Funcs for sending data by radio
void send_rf_package(void *pack, uint8_t pack_size);


#endif /* RADIO_H_ */

/*
nrf24_rf_config_t nrf24_rf_setup = {
	.data_rate = NRF24_DATARATE_250_KBIT,
	.tx_power = NRF24_TXPOWER_MINUS_18_DBM,
	.rf_channel = 116
};
nrf24_protocol_config_t nrf24_protocol_setup = {
	.crc_size = NRF24_CRCSIZE_DISABLE,
	.address_width = NRF24_ADDRES_WIDTH_5_BYTES,
	.en_dyn_payload_size = true,
	.en_ack_payload = true,
	.en_dyn_ack = true,
	.auto_retransmit_count = 0,
	.auto_retransmit_delay = 0
};
nrf24_pipe_config_t nrf24_pipe_setup = {
	.enable_auto_ack = false,
	.address = 0xacacacacac,
	.payload_size = -1
};
nrf24_spi_pins_sr_t nrf24_shift_reg_setup = {
	.this = &shift_reg_rf,
	.pos_CE = 0,
	.pos_CS = 1
};
*/
