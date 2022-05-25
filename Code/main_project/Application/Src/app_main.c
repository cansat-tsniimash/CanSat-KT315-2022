<<<<<<< HEAD
#include "app_main.h"
#include "includes.h"

#include "shift_regs.h"
#include "bmp280.h"
#include "lis3mdl.h"
#include "lsm6ds3.h"
=======
#include "../app_main.h"
#include "../includes.h"
#include "../initializations.h"
#include "../sensors.h"



#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx_hal.h>

#include "BME280/DriverForBME280.h"
#include "LSM6DS3/DLSM.h"
#include "ATGM336H/nmea_gps.h"
>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4


<<<<<<< HEAD
=======
/* Begin data structures */



>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4
/*struct minmea_sentence_gga {
    struct minmea_time time;
    struct minmea_float latitude;
    struct minmea_float longitude;
    int fix_quality;
    int satellites_tracked;
    struct minmea_float hdop;
    struct minmea_float altitude; char altitude_units;
    struct minmea_float height; char height_units;
    int dgps_age;
};*/
/* End data structures */


//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//
//}
<<<<<<< HEAD
=======




/* Begin service structures */


//imu bmp service structure
struct bme280_dev bmp280 = {0};

//imu lis service structure
stmdev_ctx_t lis_ctx = {0};

//imu lsm service structure
stmdev_ctx_t lsm_ctx = {0};


/* End service structures */
>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4


void app_main (void) {

<<<<<<< HEAD

	/* Begin Init */

	//SHIF_REG
	shift_reg_t shift_reg_imu = shift_reg_create_descriptor(shift_reg_imu_conf);
	shift_reg_init_stm32(&shift_reg_imu);

	shift_reg_t shift_reg_rf = shift_reg_create_descriptor(shift_reg_rf_conf);
	shift_reg_init_stm32(&shift_reg_rf);

	//BMP280
	bme_spi_intf_sr bmp_setup = bmp280_create_descriptor(bmp280_conf);
	struct bme280_dev bmp280 = bmp280_init(&bmp_setup);

	//LIS3MDL
	lis_spi_intf_sr lis_setup = lis_create_descriptor(lis_conf);
	stmdev_ctx_t lis = lis_init(&lis_setup);
=======
	//Init
	//app_init();
>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4

	//LSM6DS3
	lsm_spi_intf_sr lsm_setup = lsm_create_descriptor(lsm_conf);
	stmdev_ctx_t lsm = lsm_init(&lsm_setup);







	/* Begin rf package structs*/

	//rf_package_crc_t rf_package_crc = {0};
	//uint16_t package_num = 0;

	/* End rf package structs*/


	/* Begin data structures */

<<<<<<< HEAD
	bmp_data_t bmp_data = {0};

	lis_data_t lis_data = {0};

	lsm_data_t lsm_data = {0};
=======
	//struct bme280_data bmp_data = {0};
	//lsm_data_t lsm_data = {0};
>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4

	uint16_t ds_temperature = 0;

	/* End data structures */


	// Eternal loop
	while(true) {

		// Work~~ OwO
<<<<<<< HEAD

		printf("t_bme: %f, t_ds: %f, mag_x: %f, mag_y: %f, mag_z: %f\n\n", bmp_data.temperature, ds_temperature, lis_data.mag[0], lis_data.mag[1], lis_data.mag[2]);
=======
		//app_work();

		/* Begin GetData */

			//bmp_data = bme_read_data(&bme280);
			//lsmread(&lsm_ctx, &lsm_data.temperature, &lsm_data.acc, &lsm_data.gyro);

		/* End GetData */
>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4


		/* Begin working with GNSS */

		//Working with Raw Data buffer
		//size_t tail = 0;
		//size_t head = sizeof(gps_cycle_buffer) - LL_DMA_GetDataLength(DMA2, DMA2_Stream1);
		//while (tail != head) {
		//	gps_push_byte(gps_cycle_buffer[tail]);
		//	tail++;
		//	if (tail >= sizeof(gps_cycle_buffer)) {
		//		tail = 0;
		//	}
		//}
		//Parsing Raw Data
		//gps_work();
		//
		//gps_get_gga(cookie, gga_);

		/* End working with GNSS */

<<<<<<< HEAD
=======




>>>>>>> 8a7e5a19c2d7f0e0e4f4e7611d544e2697a211c4
	}
}
