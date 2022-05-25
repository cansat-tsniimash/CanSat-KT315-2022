#ifndef LIS3MDL_H_
#define LIS3MDL_H_

#include "includes.h"

typedef struct {
	float temperature;
	float mag[3];
} lis_data_t;

lis_spi_intf_sr lis_create_descriptor(int shift_reg_pin, SPI_HandleTypeDef *bus, shift_reg_t *shift_reg);
stmdev_ctx_t lis_init(lis_spi_intf_sr *lis_setup_);
lis_data_t lis_get_data(stmdev_ctx_t *lis_);

#endif /* LIS3MDL_H_ */

//imu lis descriptor
//lis_spi_intf_sr lis_setup = {
//	.sr_pin = 3,
//	.spi = &hspi2,
//	.sr = &shift_reg_imu
//};
