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

//nrf24_fifo_status_t rf_fifo_status_rx;
//nrf24_fifo_status_t rf_fifo_status_tx;
nrf24_lower_api_config_t nrf24_lowlevel_config = {0};

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