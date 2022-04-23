#include "../app_main.h"

#include "DriverForBME280.h"

extern SPI_HandleTypeDef hspi2;
//bme
//lsm
//rf
//~~ds~~

//imu lsm service struct
stmdev_ctx_t = {0};
//rf shift register descriptor
shift_reg_t shift_reg_rf = {
	.bus = &hspi2,
	.latch_port = GPIOC,
	.latch_pin = GPIO_Pin_4,
	.oe_port = GPIOC,
	.oe_pin = GPIO_Pin_5,
	.value = 0
};
//imu shift register descriptor
shift_reg_t shift_reg_imu = {
	.bus = &hspi2,
	.latch_port = GPIOC,
	.latch_pin = GPIO_Pin_1,
	.oe_port = GPIOC,
	.oe_pin = GPIO_Pin_13,
	.value = 0
};
//imu bme descriptor
bme_spi_intf_sr bm = {
	.sr_pin = 2,
	SPI_HandleTypeDef &hspi2,
	shift_reg_t *sr
};



void app_main(void) {
	void bme_init_default_sr( struct bme280_dev *bme, struct bme_spi_intf_sr* spi_intf);
}
