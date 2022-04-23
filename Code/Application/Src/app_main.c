#include "main.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart6;

/* begin structures */
struct _gps_data_t {
	float gps_lat;
	float gps_lon;
	float gps_alt;
};
typedef struct _gps_data_t gps_data;
/* end structures*/

/* begin interrupts */

/* end interrupts */

int app_main (void) {

	/* begin initializations */
	gps_init();
	/* end initializations */
	HAL_DMA_DeInit(hdma)
	HAL_UART_AbortCpltCallback(huart)
	while(1) {}
	return 0;
}
