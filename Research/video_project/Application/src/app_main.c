#include "app_main.h"

#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx_hal.h>

#include "BME280/DriverForBME280.h"
#include "LSM6DS3/DLSM.h"
#include "nRF24L01_PL/nrf24_upper_api.h"
#include "nRF24L01_PL/nrf24_lower_api.h"
#include "nRF24L01_PL/nrf24_lower_api_stm32.h"
#include "nRF24L01_PL/nrf24_defs.h"

extern SPI_HandleTypeDef hspi2;
//bme
//lsm
//rf
//~~ds~~

void app_main(void) {
	/* Begin Init */

	//imu bme service struct
	struct bme280_dev bme280 = {0};

	//imu lsm service struct
	stmdev_ctx_t ctx = {0};

	//imu shift register descriptor
	shift_reg_t shift_reg_imu = {
		.bus = &hspi2,
		.latch_port = GPIOC,
		.latch_pin = GPIO_PIN_1,
		.oe_port = GPIOC,
		.oe_pin = GPIO_PIN_13,
		.value = 0
	};

	//rf shift register descriptor
	shift_reg_t shift_reg_rf = {
		.bus = &hspi2,
		.latch_port = GPIOC,
		.latch_pin = GPIO_PIN_4,
		.oe_port = GPIOC,
		.oe_pin = GPIO_PIN_5,
		.value = 0
	};

	//imu bme descriptor
	bme_spi_intf_sr bme_setup = {
		.sr_pin = 2,
		.spi = &hspi2,
		.sr = &shift_reg_imu
	};

	//imu lsm descriptor
	lsm_spi_intf_sr lsm_setup = {
		.sr_pin = 4,
		.spi = &hspi2,
		.sr = &shift_reg_imu
	};

	//rf nrf24 descriptors
	/*nrf24_rf_config_t nrf24_rf_setup = {
		.data_rate = NRF24_DATARATE_250_KBIT,
		.tx_power = NRF24_TXPOWER_MINUS_0_DBM,
		.rf_channel = 50
	};
	nrf24_protocol_config_t nrf24_protocol_setup = {
		.crc_size = NRF24_CRCSIZE_DISABLE,
		.address_width = NRF24_ADDRES_WIDTH_5_BYTES,
		.en_dyn_payload_size = false,
		.en_ack_payload = false,
		.en_dyn_ack = false,
		.auto_retransmit_count = 0,
		.auto_retransmit_delay = 0
	};
	nrf24_spi_pins_sr_t nrf24_shift_reg_setup = {
		.this = &shift_reg_rf,
		.pos_CE = 0,
		.pos_CS = 1
	};
	nrf24_lower_api_config_t nrf24_lowlevel_setup = {0};*/

	//Init shift_reg_imu
	shift_reg_init(&shift_reg_imu);
	shift_reg_oe(&shift_reg_imu, true);
	shift_reg_write_8(&shift_reg_imu, 0xFF);
	shift_reg_oe(&shift_reg_imu, false);
	//Init shift_reg_rf
	shift_reg_init(&shift_reg_rf);
	shift_reg_oe(&shift_reg_rf, true);
	shift_reg_write_8(&shift_reg_rf, 0xFF);
	shift_reg_oe(&shift_reg_rf, false);
	//Init imu
	bme_init_default_sr(&bme280, &bme_setup);
	lsmset_sr(&ctx, &lsm_setup);
	//Init rf
	/*nrf24_spi_init_sr(&nrf24_lowlevel_setup, &hspi2, &nrf24_shift_reg_setup);
	//nrf24_setup_rf(&nrf24_lowlevel_setup.intf_ptr, &nrf24_rf_setup);
	nrf24_setup_protocol(&nrf24_lowlevel_setup.intf_ptr, &nrf24_protocol_setup);*/

	/* End Init */

	/* Begin rf package structure */
	/*typedef struct {
		uint8_t flag;
		uint8_t BMP_temperature;

		int16_t LSM_acc_x;
		int16_t LSM_acc_y;
		int16_t LSM_acc_z;
		int16_t LSM_gyro_x;
		int16_t LSM_gyro_y;
		int16_t LSM_gyro_z;

		uint16_t num;
		uint16_t crc;

		uint32_t time_from_start;
		uint32_t time_real;
		uint32_t BMP_pressure;
	} rf_packet_t;
	rf_packet_t rf_packet = {0};*/
	/* End rf package structure */

	/* Begin data structures */
	struct bme280_data bmp_data = {0};

	typedef struct lsm_data_t {
		float temperature;
		float acc[3];
		float gyro[3];
	} lsm_data_t;
	lsm_data_t lsm_data = {0};
	/* End data structures */

	while (true) {
		bmp_data = bme_read_data(&bme280);
		lsmread(&ctx, &lsm_data.temperature, &lsm_data.acc, &lsm_data.gyro);
		/*rf_packet.flag = 0x93;
		rf_packet.BMP_temperature = (uint8_t)bmp_data.temperature;
		rf_packet.LSM_acc_x = (uint16_t)lsm_data.acc[0];
		rf_packet.LSM_acc_y = (uint16_t)lsm_data.acc[1];
		rf_packet.LSM_acc_z = (uint16_t)lsm_data.acc[2];
		rf_packet.LSM_gyro_x = (uint16_t)lsm_data.gyro[0];
		rf_packet.LSM_gyro_y = (uint16_t)lsm_data.gyro[1];
		rf_packet.LSM_gyro_z = (uint16_t)lsm_data.gyro[2];
		rf_packet.num += 1;
		rf_packet.crc = 0;
		//rf_packet.time_from_start = ;
		//rf_packet.time_real = ;
		rf_packet.BMP_pressure = (uint32_t)bmp_data.pressure;
		rf_packet.crc = 0;*/
		printf("temperature: %f, pressure: %f, acc_x: %f, acc_y: %f, acc_z: %f\n", bmp_data.temperature, bmp_data.pressure, lsm_data.acc[0], lsm_data.acc[1], lsm_data.acc[2]);
		HAL_Delay(150);
	}
}
