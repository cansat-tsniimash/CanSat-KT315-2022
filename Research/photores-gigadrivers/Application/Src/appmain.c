#include "main.h"
#include <stdio.h>

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
// ADC_CHANNEL_1 or ADC_CHANNEL_2
int adc(ADC_HandleTypeDef *hadc1, uint32_t channel) {
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	HAL_ADC_Start(hadc1);
	HAL_ADC_PollForConversion(hadc1, 100);
	int res = HAL_ADC_GetValue(hadc1);
	HAL_ADC_Stop(hadc1);
	return res;
}

void changeChannelTo2() {

}

int app_main() {
	int i;
	for (i = 0; i < 200; i++) {
		char arr[100] = {0};
		snprintf(arr, sizeof(arr), "resistor: %d\n\r", adc(&hadc1, ADC_CHANNEL_1));
		HAL_UART_Transmit(&huart1, (uint8_t*)arr, (sizeof(arr) / sizeof(arr[0])), 100);
	}
	changeChannelTo2();
	HAL_UART_Transmit(&huart1, (uint8_t*)"Channel changed\n\r", 17, 100);
	for (i = 0; i < 200; i++) {
		char arr[100] = {0};
		snprintf(arr, sizeof(arr), "resistor: %d\n\r", adc(&hadc1, ADC_CHANNEL_2));
		HAL_UART_Transmit(&huart1, (uint8_t*)arr, (sizeof(arr) / sizeof(arr[0])), 100);
	}
	HAL_UART_Transmit(&huart1, (uint8_t*)"Finished\n\r", 10, 100);
	return 0;
}
