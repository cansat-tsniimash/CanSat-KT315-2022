#include "../sensors.h"
#include "../includes.h"


extern struct bme280_dev bmp280;
extern stmdev_ctx_t lis_ctx;
extern stmdev_ctx_t lsm_ctx;
extern ds18b20_t ds18b20;

uint16_t ds18b20_read(ds18b20_t *ds18b20) {
	ds18b20_start_conversion(ds18b20);
	HAL_Delay(750);
	uint16_t res;
	ds18b20_read_raw_temperature(ds18b20, &res, 0);
	return res;
}

sensors_t app_work() {


	/* Begin data structures */

	struct bme280_data bmp_data_ = {0};
	bmp_data_t bmp_data = {0};

	lis_data_t lis_data = {0};

	lsm_data_t lsm_data = {0};

	uint16_t ds_data = 0;

	/* Begin data structures */

	//BMP
	bmp_data_ = bme_read_data(&bmp280);
	bmp_data.temperature = bmp_data_.temperature;
	bmp_data.pressure = bmp_data_.pressure;

	//LIS
	lisread(&lis_ctx, &lis_data.temperature, &lis_data.mag);

	//LSM
	lsmread(&lsm_ctx, &lsm_data.temperature, &lsm_data.acc, &lsm_data.gyro);

	//DS18B20
	ds_data = ds18b20_read(&ds18b20);

	sensors_t result = {
		.bmp_data = bmp_data,
		.lis_data = lis_data,
		.lsm_data = lsm_data,
		.ds_temperature = ds_data
	};

	return result;
}
