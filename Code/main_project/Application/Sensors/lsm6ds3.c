#include <lsm6ds3.h>
#include <includes.h>

lsm_spi_intf_sr lsm_create_descriptor(int shift_reg_pin, SPI_HandleTypeDef *bus, shift_reg_t *shift_reg) {
	lsm_spi_intf_sr lsm_ = {
		.sr_pin = shift_reg_pin,
		.spi = bus,
		.sr = shift_reg
	};
	return lsm_;
}

stmdev_ctx_t lsm_init(lsm_spi_intf_sr *lsm_setup_) {
	stmdev_ctx_t lsm_ = {0};
	lsmset_sr(&lsm_, lsm_setup_);
	return lsm_;
}

lsm_data_t lsm_get_data(stmdev_ctx_t *lsm_) {
	lsm_data_t lsm_data_ = {0};
	lsmread(lsm_, &lsm_data_.temperature, &lsm_data_.acc, &lsm_data_.gyro);
	return lsm_data_;
}
