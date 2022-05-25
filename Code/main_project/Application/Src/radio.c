#include "../radio.h"
#include "../includes.h"

static uint16_t package_num = 0;

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



/*void send_rf_package (nrf24_lower_api_config_t *nrf24_lowlevel_config) {
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
}*/


void pack_rf_dose() {

}

void pack_rf_bmp() {

}

void pack_rf_gps() {

}

void pack_rf_inertial() {

}

void pack_rf_sebastian() {

}


void send_rf_dose() {

}

void send_rf_bmp() {

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
