#ifndef RADIO_H_
#define RADIO_H_

#include <includes.h>

typedef enum {
	RF_FLAG_DOSIMETER,
	RF_FLAG_BMP,
	RF_FLAG_DS,
	RF_FLAG_GPS,
	RF_FLAG_INERTIAL,
	RF_FLAG_SEBASTIAN
} rf_flag_t;

typedef struct {
	nrf24_lower_api_config_t *nrf24_lower_api_config;
	nrf24_fifo_status_t rf_fifo_status_rx;
	nrf24_fifo_status_t rf_fifo_status_tx;
} nrf24_service_t;

/* Begin RF Structures */

typedef struct __attribute__((packed)) { // Дозиметр, отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	uint32_t ticks_per_last_second;
	uint32_t ticks_per_last_minute;
	uint32_t ticks_sum;
} rf_dosimeter_package_t;

typedef struct __attribute__((packed)) {
	rf_dosimeter_package_t pack;
	uint16_t crc;
} rf_dosimeter_package_crc_t;



typedef struct __attribute__((packed)) { // BMP-пакет, отправка так быстро, как можно
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	int16_t bmp_temperature;
	uint32_t bmp_pressure;
} rf_bmp_package_t;

typedef struct __attribute__((packed)) {
	rf_bmp_package_t pack;
	uint16_t crc;
} rf_bmp_package_crc_t;



typedef struct __attribute__((packed)) { // DS18B20+фоторезисторы+статус, отправка каждые ~750 мс
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float ds18b20_temperature;
	float rocket_lux;
	float seed_lux;
	uint8_t status;
} rf_ds_package_t;

typedef struct __attribute__((packed)) {
	rf_ds_package_t pack;
	uint16_t crc;
} rf_ds_package_crc_t;



typedef struct __attribute__((packed)) { // GPS, отправка каждую секунду
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float longtitude;
	float latitude;
	int16_t altitude;
	uint64_t time_sec;
	uint32_t time_microsec;
	uint8_t fix;
} rf_gps_package_t;

typedef struct __attribute__((packed)) {
	rf_gps_package_t pack;
	uint16_t crc;
} rf_gps_package_crc_t;



typedef struct __attribute__((packed)) { // Инерциалка, отправка так быстро, как можно (и ещё быстрее)
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	int16_t lsm_acc [3];
	int16_t lsm_gyro [3];
	int16_t lis_mag [3];
} rf_inertial_package_t;

typedef struct __attribute__((packed)) {
	rf_inertial_package_t pack;
	uint16_t crc;
} rf_inertial_package_crc_t;



typedef struct  __attribute__((packed)) { // Маджвик, отправка каждые ??
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

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
rf_dosimeter_package_crc_t pack_rf_dosimeter(uint32_t ticks_per_last_sec, uint32_t ticks_per_last_minute, uint32_t ticks_sum);
rf_bmp_package_crc_t pack_rf_bmp(int16_t temperature, uint32_t pressure);
rf_ds_package_crc_t pack_rf_ds(float temperature, float rckt_lux, float seed_lux, uint8_t status);
rf_gps_package_crc_t pack_rf_gps(float lon, float lat, int16_t alt, uint64_t time_sec, uint32_t time_microsec, uint8_t fix);
rf_inertial_package_crc_t pack_rf_inertial(int16_t acc [3], int16_t gyro [3], int16_t mag [3]);
rf_sebastian_package_crc_t pack_rf_sebastian(float quaternion [4]);

//Funcs for sending data by radio
void send_rf_package(nrf24_service_t *nrf24_service, void *package, size_t size);


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
