#include <bmp280.h>
#include <includes.h>

#include <math.h>

bme_spi_intf_sr bmp280_create_descriptor(int shift_reg_pin, SPI_HandleTypeDef *bus, shift_reg_t *shift_reg) {
	bme_spi_intf_sr bmp_ = {
		.sr_pin = shift_reg_pin,
		.spi = bus,
		.sr = shift_reg
	};
	return bmp_;
}

struct bme280_dev bmp280_init(bme_spi_intf_sr *bmp_setup_) {
	struct bme280_dev bmp280_ = {0};
	bme_init_default_sr(&bmp280_, bmp_setup_);
	return bmp280_;
}

bmp_data_t bmp280_get_data(struct bme280_dev *bmp280_) {
	struct bme280_data bmp_data_drv = {0};
	bmp_data_t bmp_data_ = {0};
	bmp_data_drv = bme_read_data(bmp280_);
	bmp_data_.temperature = bmp_data_drv.temperature;
	bmp_data_.pressure = bmp_data_drv.pressure;
	return bmp_data_;
}

float calculate_height(float pressure, float ground_pressure) {
	float height = 0.0;
	height = 44330 * (1 - pow(pressure / ground_pressure, 1.0 / 5.255));
	return height;
}
