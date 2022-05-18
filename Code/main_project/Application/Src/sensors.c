#include "../sensors.h"
#include "../includes.h"


extern struct bme280_dev bmp280;
extern stmdev_ctx_t lis_ctx;
extern stmdev_ctx_t lsm_ctx;


void app_work() {


	/* Begin data structures */

	struct bme280_data bmp_data_ = {0};
	bmp_data_t bmp_data = {0};

	lis_data_t lis_data = {0};

	lsm_data_t lsm_data = {0};

	/* Begin data structures */

	//BMP
	bmp_data_ = bme_read_data(&bmp280);
	bmp_data.temperature = bmp_data_.temperature;
	bmp_data.pressure = bmp_data_.pressure;

	//LIS
	lisread(&lis_ctx, &lis_data.temperature, &lis_data.mag);

	//LSM
	lsmread(&lsm_ctx, &lsm_data.temperature, &lsm_data.acc, &lsm_data.gyro);
}
