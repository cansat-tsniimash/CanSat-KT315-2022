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

/* Begin rf package structure */

typedef struct __attribute__((packed)) {
	uint8_t flag;
	uint8_t BMP_temperature;

	uint16_t LSM_acc_x;
	uint16_t LSM_acc_y;
	uint16_t LSM_acc_z;
	uint16_t LSM_gyro_x;
	uint16_t LSM_gyro_y;
	uint16_t LSM_gyro_z;

	uint16_t num;

	uint32_t time_from_start;
	//uint32_t time_real;
	uint32_t BMP_pressure;
} rf_package_t;

typedef struct __attribute__((packed)) {
	rf_package_t pack;
	uint16_t crc;
} rf_package_crc_t;

/* End rf package structure  */


/* Begin data structures */

typedef struct {
		float temperature;
		float acc[3];
		float gyro[3];
} lsm_data_t;

/* End data structures */


/*static void dump_registers(void *intf_ptr)
{
	const size_t regs_count = sizeof(reg_params)/sizeof(reg_params[0]);
	for (size_t i = 0 ; i < regs_count; i++)
	{
		uint8_t reg_addr = reg_params[i].addr;
		uint8_t reg_size = reg_params[i].size;

		uint8_t reg_data[5] = { 0 };
		nrf24_read_register(intf_ptr, reg_addr, reg_data, reg_size);

		print_register(reg_addr, reg_data);
	}
}*/


unsigned short Crc16(unsigned char *buf, unsigned short len) //crc func
{
	unsigned short crc = 0xFFFF;
	unsigned char i;
	while (len--) {
		crc ^= *buf++ << 8;
		for (i = 0; i < 8; i++)
			crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
	}
	return crc;
}

rf_package_crc_t pack(struct bme280_data *bmp_data, lsm_data_t *lsm_data, uint16_t package_num) {
	rf_package_t rf_package = {
		.flag = 0x93,
		.BMP_temperature = (uint8_t)(bmp_data->temperature * 10),
		.LSM_acc_x = (int16_t)(lsm_data->acc[0] * 1000),
		.LSM_acc_y = (int16_t)(lsm_data->acc[1] * 1000),
		.LSM_acc_z = (int16_t)(lsm_data->acc[2] * 1000),
		.LSM_gyro_x = (int16_t)(lsm_data->gyro[0] * 1000),
		.LSM_gyro_y = (int16_t)(lsm_data->gyro[1] * 1000),
		.LSM_gyro_z = (int16_t)(lsm_data->gyro[2] * 1000),
		.num = package_num,
		.time_from_start = HAL_GetTick(),
		//.time_real = ,
		.BMP_pressure = (uint32_t)bmp_data->pressure
	};
	rf_package_crc_t rf_package_crc = {
		.pack = rf_package,
		.crc = Crc16((unsigned char*)&rf_package, sizeof(rf_package))
	};
	return rf_package_crc;
}

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
	nrf24_rf_config_t nrf24_rf_setup = {
		.data_rate = NRF24_DATARATE_250_KBIT,
		.tx_power = NRF24_TXPOWER_MINUS_0_DBM,
		.rf_channel = 116
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
	nrf24_pipe_config_t nrf24_pipe_setup = {
		.enable_auto_ack = true,
		.address = 0xacacacacac,
		.payload_size = -1
	};
	nrf24_spi_pins_sr_t nrf24_shift_reg_setup = {
		.this = &shift_reg_rf,
		.pos_CE = 0,
		.pos_CS = 1
	};
	nrf24_fifo_status_t rf_fifo_status_rx;
	nrf24_fifo_status_t rf_fifo_status_tx;
	nrf24_lower_api_config_t nrf24_lowlevel_config = {0};

	//Init shift_reg_imu
	shift_reg_init(&shift_reg_imu);
	shift_reg_oe(&shift_reg_imu, true);
	shift_reg_write_8(&shift_reg_imu, 0xFF);
	shift_reg_oe(&shift_reg_imu, false);

	//Init shift_reg_rf
	shift_reg_init(&shift_reg_rf);
	shift_reg_oe(&shift_reg_rf, true);
	shift_reg_write_8(&shift_reg_rf, 0xFF);
	//shift_reg_oe(&shift_reg_rf, false);

	//Init imu
	bme_init_default_sr(&bme280, &bme_setup);
	lsmset_sr(&ctx, &lsm_setup);

	//Init rf
	nrf24_spi_init_sr(&nrf24_lowlevel_config, &hspi2, &nrf24_shift_reg_setup);
	nrf24_mode_standby(&nrf24_lowlevel_config);
	nrf24_setup_rf(&nrf24_lowlevel_config, &nrf24_rf_setup);
	nrf24_setup_protocol(&nrf24_lowlevel_config, &nrf24_protocol_setup);
	nrf24_pipe_set_tx_addr(&nrf24_lowlevel_config, 0xacacacacac);
	nrf24_pipe_rx_start(&nrf24_lowlevel_config, 0, &nrf24_pipe_setup);
	nrf24_pipe_rx_start(&nrf24_lowlevel_config, 1, &nrf24_pipe_setup);
	nrf24_mode_standby(&nrf24_lowlevel_config);

	/* End Init */

	/* Begin rf package structs*/

	rf_package_crc_t rf_package_crc = {0};
	uint16_t package_num = 0;

	/* End rf package structs*/


	/* Begin data structures */

	struct bme280_data bmp_data = {0};
	lsm_data_t lsm_data = {0};

	/* End data structures */


	while (true) {

		/* Begin GetData */

		bmp_data = bme_read_data(&bme280);
		lsmread(&ctx, &lsm_data.temperature, &lsm_data.acc, &lsm_data.gyro);

		/* End GetData */



		//UART data transmit
		//printf("temperature: %f, pressure: %f, acc_x: %f, acc_y: %f, acc_z: %f\n", bmp_data.temperature, bmp_data.pressure, lsm_data.acc[0], lsm_data.acc[1], lsm_data.acc[2]);


		/* Begin radio data transmit */

		nrf24_fifo_status(&nrf24_lowlevel_config, &rf_fifo_status_rx, &rf_fifo_status_tx);
		if (rf_fifo_status_tx != NRF24_FIFO_FULL) {
			rf_package_crc = pack(&bmp_data, &lsm_data, package_num);
			package_num++;
			nrf24_fifo_write(&nrf24_lowlevel_config, (uint8_t*) &rf_package_crc, sizeof(rf_package_crc), false);
			nrf24_mode_tx(&nrf24_lowlevel_config);
			HAL_Delay(50);
			nrf24_mode_standby(&nrf24_lowlevel_config);
		} else {
			HAL_Delay(100);
			nrf24_fifo_flush_tx(&nrf24_lowlevel_config);
		}
		nrf24_irq_clear(&nrf24_lowlevel_config, NRF24_IRQ_RX_DR | NRF24_IRQ_TX_DR | NRF24_IRQ_MAX_RT);

		/* End radio data transmit */

	}
}
