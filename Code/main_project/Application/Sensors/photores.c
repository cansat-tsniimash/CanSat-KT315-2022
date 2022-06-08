#include <photores.h>
#include <includes.h>

photorezistor_t photores_create_descriptor(float resist, ADC_HandleTypeDef *hadc) {
	photorezistor_t photores_ = {
		.resist = resist,
		.hadc = hadc
	};
	return photores_;
}

float photores_get_data(photorezistor_t photores_) {
	return photorezistor_get_lux(photores_);
}
