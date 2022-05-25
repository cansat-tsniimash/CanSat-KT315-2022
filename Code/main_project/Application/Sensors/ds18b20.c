#include "ds18b20.h"
#include "includes.h"

ds18b20_t ds18_create_descriptor(GPIO_TypeDef *onewire_port, uint16_t onewire_pin) {
	ds18b20_t ds18_ = {
			.onewire_port = onewire_port,
			.onewire_pin = onewire_pin
	};
	return ds18_;
}

void ds18_init(ds18b20_t *ds18_setup_) {
	onewire_init(ds18_setup_);
	ds18b20_set_config(ds18_setup_, 100, -100, DS18B20_RESOLUTION_12_BIT);
}
