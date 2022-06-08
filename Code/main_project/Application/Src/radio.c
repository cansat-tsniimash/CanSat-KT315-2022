#include <radio.h>
#include <includes.h>

static uint16_t package_num_dosimeter = 0;
static uint16_t package_num_bmp = 0;
static uint16_t package_num_ds = 0;
static uint16_t package_num_gps = 0;
static uint16_t package_num_inertial = 0;
static uint16_t package_num_sebastian = 0;

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



nrf24_spi_pins_sr_t nrf24_create_sr_descriptor(shift_reg_t *shift_reg, uint8_t pos_CE, uint8_t pos_CS) {
	nrf24_spi_pins_sr_t nrf_sr_ = {
		.this = shift_reg,
		.pos_CE = pos_CE,
		.pos_CS = pos_CS
	};
	return nrf_sr_;
}

nrf24_rf_config_t nrf24_create_rf_descriptor(nrf24_data_rate_t data_rate, nrf24_tx_power_t tx_power, uint8_t rf_channel) {
	nrf24_rf_config_t nrf_rf_ = {
		.data_rate = data_rate,
		.tx_power = tx_power,
		.rf_channel = rf_channel
	};
	return nrf_rf_;
}

nrf24_protocol_config_t nrf24_create_protocol_descriptor(nrf24_crc_size_t crc_size, nrf24_address_width_t address_width, bool en_dyn_payload_size, bool en_ack_payload, bool en_dyn_ack, uint8_t auto_retransmit_count, uint8_t auto_retransmit_delay) {
	nrf24_protocol_config_t nrf_protocol_ = {
		.crc_size = crc_size,
		.address_width = address_width,
		.en_dyn_payload_size = en_dyn_payload_size,
		.en_ack_payload = en_ack_payload,
		.en_dyn_ack = en_dyn_ack,
		.auto_retransmit_count = auto_retransmit_count,
		.auto_retransmit_delay = auto_retransmit_delay
	};
	return nrf_protocol_;
}

nrf24_pipe_config_t nrf24_create_pipe_descriptor(bool enable_auto_ack, uint64_t address, int8_t payload_size) {
	nrf24_pipe_config_t nrf_pipe_ = {
		.enable_auto_ack = enable_auto_ack,
		.address = address,
		.payload_size = payload_size
	};
	return nrf_pipe_;
}

void nrf24_init_stm32(nrf24_lower_api_config_t *nrf24_config_, SPI_HandleTypeDef *bus, nrf24_spi_pins_sr_t *nrf24_sr_setup_, nrf24_rf_config_t *nrf24_rf_setup_, nrf24_protocol_config_t *nrf24_protocol_setup_, nrf24_pipe_config_t *nrf24_pipe_setup_) {
	nrf24_spi_init_sr(nrf24_config_, bus, nrf24_sr_setup_);
	nrf24_mode_standby(nrf24_config_);
	nrf24_setup_rf(nrf24_config_, nrf24_rf_setup_);
	nrf24_setup_protocol(nrf24_config_, nrf24_protocol_setup_);
	nrf24_pipe_set_tx_addr(nrf24_config_, 0xacacacacac);
	nrf24_pipe_rx_start(nrf24_config_, 0, nrf24_pipe_setup_);
	nrf24_pipe_rx_start(nrf24_config_, 1, nrf24_pipe_setup_);
	nrf24_mode_standby(nrf24_config_);
	nrf24_fifo_flush_tx(nrf24_config_);
}



rf_dosimeter_package_crc_t pack_rf_dosimeter(uint32_t ticks_last_sec, uint32_t ticks_sum) {
	rf_dosimeter_package_t rf_package_ = {
		.flag = RF_FLAG_DOSIMETER,
		.num = package_num_dosimeter,
		.time_from_start = HAL_GetTick(),
		.ticks_now = ticks_last_sec,
		.ticks_sum = ticks_sum
	};
	rf_dosimeter_package_crc_t rf_package_crc_ = {
		.pack = rf_package_,
		.crc = Crc16((unsigned char*)&rf_package_, sizeof(rf_package_))
	};
}

void pack_rf_bmp() {

}

void pack_rf_ds() {

}

void pack_rf_gps() {

}

void pack_rf_inertial() {

}

void pack_rf_sebastian() {

}



/*void send_rf_dosimeter(rf_dosimeter_package_crc_t pack, nrf24_lower_api_config_t* nrf24_lowlevel_config) {
	nrf24_fifo_status(nrf24_lowlevel_config, &rf_fifo_status_rx, &rf_fifo_status_tx);
	if (rf_fifo_status_tx != NRF24_FIFO_FULL) {
		rf_package_crc = pack(&bmp_data, &lsm_data, package_num);
		package_num++;
		nrf24_fifo_write(nrf24_lowlevel_config, (uint8_t*) &rf_package_crc, sizeof(rf_package_crc), false);
		nrf24_mode_tx(nrf24_lowlevel_config);
		HAL_Delay(3);
		nrf24_mode_standby(nrf24_lowlevel_config);
	} else {
		nrf24_fifo_flush_tx(nrf24_lowlevel_config);
		HAL_Delay(100);
	}
	nrf24_irq_clear(nrf24_lowlevel_config, NRF24_IRQ_RX_DR | NRF24_IRQ_TX_DR | NRF24_IRQ_MAX_RT);
}*/

void send_rf_bmp() {

}

void send_rf_ds() {

}

void send_rf_gps() {

}

void send_rf_inertial() {

}

void send_rf_sebastian() {

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
