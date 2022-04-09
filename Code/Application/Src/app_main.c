#include "main.h"
#include "stm32f4xx_hal.h"
int app_main (void) {
	wchar_t[14] data = "Hello, Sergey\n";
	HAL_UART_Transmit(&huart6, data, data.size());
	return 0;
}
