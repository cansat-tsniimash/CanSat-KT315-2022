#include <main.h>
#include <includes.h>

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	Dosimeter_Callback(pin, Dosimeter_Pin);
	return;
}
