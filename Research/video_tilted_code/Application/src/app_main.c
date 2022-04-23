#include "app_main.h"
#include "DriverForBME280.h"
#include "DLSM.h"
#include ""

extern SPI_HandleTypeDef hspi2;
//bme
//lsm
//rf
//~~ds~~

void app_main(void) {
	/* Begin Init */
	//imu bme service struct
	struct bme280_dev bme280 = {0};
	//imu lsm service struct
	stmdev_ctx_t ctx = {0};
	//imu shift register descriptor
	shift_reg_t shift_reg_imu = {
		.bus = &hspi2,
		.latch_port = GPIOC,
		.latch_pin = GPIO_Pin_1,
		.oe_port = GPIOC,
		.oe_pin = GPIO_Pin_13,
		.value = 0
	};
	//rf shift register descriptor
	shift_reg_t shift_reg_rf = {
		.bus = &hspi2,
		.latch_port = GPIOC,
		.latch_pin = GPIO_Pin_4,
		.oe_port = GPIOC,
		.oe_pin = GPIO_Pin_5,
		.value = 0
	};
	//imu bme descriptor
	bme_spi_intf_sr bme_setup = {
		.sr_pin = 2,
		.spi = &hspi2,
		.sr = &shift_reg_imu
	};
	//imu lsm descriptor
	lsm_spi_intf_sr lsm_setup = {
		.sr_pin = 4,
		.spi = &hspi2,
		.sr = shift_reg_imu
	};
	//Init
	bme_init_default_sr(&bme280, &bme_setup);
	lsmset_sr(&ctx, &lsm_setup);

}
