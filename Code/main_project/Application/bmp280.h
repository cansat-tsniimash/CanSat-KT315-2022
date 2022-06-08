#ifndef BMP280_H_
#define BMP280_H_

#include <includes.h>

typedef struct {
	double temperature;
	double pressure;
} bmp_data_t;

bme_spi_intf_sr bmp280_create_descriptor(int shift_reg_pin, SPI_HandleTypeDef* bus, shift_reg_t *shift_reg);
struct bme280_dev bmp280_init(bme_spi_intf_sr *bmp_setup);
bmp_data_t bmp280_get_data(struct bme280_dev *bmp_);

#endif /* BMP280_H_ */

//imu bmp descriptor
//bme_spi_intf_sr bmp_setup = {
//	.sr_pin = 2,
//	.spi = &hspi2,
//	.sr = &shift_reg_imu
//};
