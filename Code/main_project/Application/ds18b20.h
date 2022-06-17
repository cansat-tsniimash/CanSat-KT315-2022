#ifndef DS18B20_H_
#define DS18B20_H_

#include <includes.h>

ds18b20_t ds_create_descriptor(GPIO_TypeDef *onewire_port, uint16_t onewire_pin);
void ds_init(ds18b20_t *ds18_setup_);
float ds_get_data(ds18b20_t *ds18b20_);

#endif /* DS18B20_H_ */

//ds18b20 descriptor
//ds18b20_t ds18b20 = {
//	.onewire_port = GPIOA,
//	.onewire_pin = GPIO_PIN_1
//};
