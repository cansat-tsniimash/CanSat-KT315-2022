#include "lis3mdl.h"
#include "includes.h"

lis_spi_intf_sr lis_create_descriptor(int shift_reg_pin, SPI_HandleTypeDef *bus, shift_reg_t *shift_reg) {
	lis_spi_intf_sr lis_ = {
		.sr_pin = shift_reg_pin,
		.spi = bus,
		.sr = shift_reg
	};
	return lis_;
}

stmdev_ctx_t lis_init(lis_spi_intf_sr *lis_setup_) {
	stmdev_ctx_t lis_ = {0};
	lisset_sr(&lis_, lis_setup_);
	return lis_;
}

lis_data_t lis_get_data(stmdev_ctx_t *lis_) {
	lis_data_t lis_data_ = {0};
	lisread(lis_, &lis_data_.temperature, &lis_data_.mag);
	return lis_data_;
}
