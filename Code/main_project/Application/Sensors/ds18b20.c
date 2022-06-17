#include <ds18b20.h>
#include <includes.h>

ds18b20_t ds_create_descriptor(GPIO_TypeDef *onewire_port, uint16_t onewire_pin) {
	ds18b20_t ds18_ = {
			.onewire_port = onewire_port,
			.onewire_pin = onewire_pin
	};
	return ds18_;
}

void ds_init(ds18b20_t *ds18b20_) {
	onewire_init(ds18b20_);
	ds18b20_set_config(ds18b20_, 100, -100, DS18B20_RESOLUTION_12_BIT);
}

float ds_get_data(ds18b20_t *ds18b20_) {
	uint16_t raw_temp = 0;
	ds18b20_read_raw_temperature(ds18b20_, &raw_temp, 0);
	ds18b20_start_conversion(ds18b20_);
	float temp = raw_temp / 16.0;
	return temp;
}
