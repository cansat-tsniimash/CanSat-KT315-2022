#include "../initializations.h"
#include "../includes.h"

extern SPI_HandleTypeDef hspi2;
extern struct bme280_dev bmp280;
extern stmdev_ctx_t lis_ctx;
extern stmdev_ctx_t lsm_ctx;
extern ds18b20_t ds18b20;

void app_init() {

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


	//Init DS18B20


	//Init GNSS
	//static uint8_t gps_cycle_buffer[500];
	//LL_DMA_ConfigAddresses(DMA2, DMA2_Stream1, USART6->DR, gps_cycle_buffer, DMA_PERIPH_TO_MEMORY);
	//LL_DMA_SetDataLength(DMA2, DMA2_Stream1, sizeof(gps_cycle_buffer)*sizeof(gps_cycle_buffer[0]));
	//LL_DMA_EnableStream(DMA2, DMA2_Stream1);
	//LL_USART_EnableDMAReq_RX(USART6);
	//gps_init();

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

}
