#include "../initializations.h"
#include "../includes.h"

extern SPI_HandleTypeDef hspi2;
extern struct bme280_dev bmp280;
extern stmdev_ctx_t lis_ctx;
extern stmdev_ctx_t lsm_ctx;


void app_init(void) {


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

	//imu bmp descriptor
	bme_spi_intf_sr bmp_setup = {
		.sr_pin = 2,
		.spi = &hspi2,
		.sr = &shift_reg_imu
	};

	//imu lis descriptor
	lis_spi_intf_sr lis_setup = {
		.sr_pin = 3,
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
	shift_reg_oe(&shift_reg_rf, false);

	//Init imu
	bme_init_default_sr(&bmp280, &bmp_setup);
	lisset_sr(&lis_ctx, &lis_setup);
	lsmset_sr(&lsm_ctx, &lsm_setup);

	//Init GNSS
	static uint8_t gps_cycle_buffer[500];
	LL_DMA_ConfigAddresses(DMA2, DMA2_Stream1, USART6->DR, gps_cycle_buffer, DMA_PERIPH_TO_MEMORY);
	LL_DMA_SetDataLength(DMA2, DMA2_Stream1, sizeof(gps_cycle_buffer)*sizeof(gps_cycle_buffer[0]));
	LL_DMA_EnableStream(DMA2, DMA2_Stream1);
	LL_USART_EnableDMAReq_RX(USART6);
	gps_init();

	//Init rf
	nrf24_spi_init_sr(&nrf24_lowlevel_config, &hspi2, &nrf24_shift_reg_setup);
	nrf24_mode_standby(&nrf24_lowlevel_config);
	nrf24_setup_rf(&nrf24_lowlevel_config, &nrf24_rf_setup);
	nrf24_setup_protocol(&nrf24_lowlevel_config, &nrf24_protocol_setup);
	nrf24_pipe_set_tx_addr(&nrf24_lowlevel_config, 0xacacacacac);
	nrf24_pipe_rx_start(&nrf24_lowlevel_config, 0, &nrf24_pipe_setup);
	nrf24_pipe_rx_start(&nrf24_lowlevel_config, 1, &nrf24_pipe_setup);
	nrf24_mode_standby(&nrf24_lowlevel_config);
	nrf24_fifo_flush_tx(&nrf24_lowlevel_config);

	//Init dosimeter


}
