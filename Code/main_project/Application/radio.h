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

typedef struct __attribute__((packed)) {
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



typedef struct __attribute__((packed)) {
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	double bmp_temperature;
	double bmp_pressure;
	uint8_t status;
} rf_bmp_package_t;

typedef struct __attribute__((packed)) {
	rf_bmp_package_t pack;
	uint16_t crc;
} rf_bmp_package_crc_t;



typedef struct __attribute__((packed)) {
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float ds18b20_temperature;
	float rocket_lux;
	float seed_lux;
} rf_ds_package_t;

typedef struct __attribute__((packed)) {
	rf_ds_package_t pack;
	uint16_t crc;
} rf_ds_package_crc_t;



typedef struct __attribute__((packed)) {
	uint8_t flag;
	uint16_t num;
	uint32_t time_from_start;

	float longtitude;
	float latitude;
	float altitude;
	uint32_t time_sec;
	uint32_t time_microsec;
	uint8_t fix;
} rf_gps_package_t;

typedef struct __attribute__((packed)) {
	rf_gps_package_t pack;
	uint16_t crc;
} rf_gps_package_crc_t;



typedef struct __attribute__((packed)) {
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



typedef struct  __attribute__((packed)) {
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


uint16_t Crc16(uint8_t *buf, uint16_t len);

nrf24_spi_pins_sr_t nrf24_create_sr_descriptor(shift_reg_t *shift_reg, uint8_t pos_CE, uint8_t pos_CS);
nrf24_rf_config_t nrf24_create_rf_descriptor(nrf24_data_rate_t data_rate, nrf24_tx_power_t tx_power, uint8_t rf_channel);
nrf24_protocol_config_t nrf24_create_protocol_descriptor(nrf24_crc_size_t crc_size, nrf24_address_width_t address_width, bool en_dyn_payload_size, bool en_ack_payload, bool en_dyn_ack, uint8_t auto_retransmit_count, uint8_t auto_retransmit_delay);
nrf24_pipe_config_t nrf24_create_pipe_descriptor(bool enable_auto_ack, uint64_t address, int8_t payload_size);
void nrf24_init_stm32(nrf24_lower_api_config_t *nrf24_config_, SPI_HandleTypeDef *bus, nrf24_spi_pins_sr_t *nrf24_sr_setup_, nrf24_rf_config_t *nrf24_rf_setup_, nrf24_protocol_config_t *nrf24_protocol_setup_, nrf24_pipe_config_t *nrf24_pipe_setup_);


rf_dosimeter_package_crc_t pack_rf_dosimeter(uint32_t ticks_per_last_sec, uint32_t ticks_per_last_minute, uint32_t ticks_sum);
rf_bmp_package_crc_t pack_rf_bmp(double temperature, double pressure, uint8_t status);
rf_ds_package_crc_t pack_rf_ds(float temperature, float rckt_lux, float seed_lux);
rf_gps_package_crc_t pack_rf_gps(float lon, float lat, float alt, uint32_t time_sec, uint32_t time_microsec, uint8_t fix);
rf_inertial_package_crc_t pack_rf_inertial(int16_t acc [3], int16_t gyro [3], int16_t mag [3]);
rf_sebastian_package_crc_t pack_rf_sebastian(float quaternion [4]);


void send_rf_package(nrf24_service_t *nrf24_service, void *package, size_t size);


#endif /* RADIO_H_ */
