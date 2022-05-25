#ifndef LSM6DS3_H_
#define LSM6DS3_H_

#include "includes.h"

typedef struct {
	float temperature;
	float acc[3];
	float gyro[3];
} lsm_data_t;

lsm_spi_intf_sr lsm_create_descriptor(int shift_reg_pin, SPI_HandleTypeDef *bus, shift_reg_t *shift_reg);
stmdev_ctx_t lsm_init(lsm_spi_intf_sr *lsm_setup_);
lsm_data_t lsm_get_data(stmdev_ctx_t *lsm_);

#endif /* LSM6DS3_H_ */

//imu lsm descriptor
//lsm_spi_intf_sr lsm_setup = {
//	.sr_pin = 4,
//	.spi = &hspi2,
//	.sr = &shift_reg_imu
//};
