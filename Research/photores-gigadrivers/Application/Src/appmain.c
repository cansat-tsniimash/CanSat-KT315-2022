#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;

int adc(ADC_HandleTypeDef *hadc1) {
	HAL_ADC_Start(hadc1);
	HAL_ADC_PollForConversion(hadc1, 100);
	int res = HAL_ADC_GetValue(hadc1);
	HAL_ADC_Stop(hadc1);
	return res;
}

int app_main() {
	while (1) {
		char arr[100] = {0};
		snprintf(arr, sizeof(arr), "resistor: %d\n\r", adc(&hadc1));
		HAL_UART_Transmit(&huart1, arr, (sizeof(arr) / sizeof(arr[0])), 100);
	}
}
