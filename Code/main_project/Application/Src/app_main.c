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


/* Begin data structures */



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




/* Begin service structures */


//imu bmp service structure
struct bme280_dev bmp280 = {0};

//imu lis service structure
stmdev_ctx_t lis_ctx = {0};

//imu lsm service structure
stmdev_ctx_t lsm_ctx = {0};


/* End service structures */


void app_main (void) {

	//Init
	//app_init();




	/* Begin rf package structs*/

	//rf_package_crc_t rf_package_crc = {0};
	//uint16_t package_num = 0;

	/* End rf package structs*/


	/* Begin data structures */

	//struct bme280_data bmp_data = {0};
	//lsm_data_t lsm_data = {0};

	/* End data structures */


	// Eternal loop
	while(true) {

		// Work~~ OwO
		//app_work();

		/* Begin GetData */

			//bmp_data = bme_read_data(&bme280);
			//lsmread(&lsm_ctx, &lsm_data.temperature, &lsm_data.acc, &lsm_data.gyro);

		/* End GetData */


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





	}
}
