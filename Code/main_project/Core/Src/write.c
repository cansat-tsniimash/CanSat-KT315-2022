/*#include <stm32f4xx_hal.h>
int _write(int file, char *ptr, int len) {
	extern UART_HandleTypeDef huart1;
	HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
	return len;
}
*/
