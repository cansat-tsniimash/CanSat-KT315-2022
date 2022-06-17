#ifndef PHOTORES_H_
#define PHOTORES_H_

#include <includes.h>

typedef enum {
	ANALOG_TARGET_ROCKET_CHECKER,
	ANALOG_TARGER_SEED_CHECKER
} analog_target_t;

typedef struct {
	analog_target_t target;
	ADC_HandleTypeDef *adc;
	float resist;
	uint16_t oversampling;
} photoresistor_t;

photoresistor_t photores_create_descriptor(analog_target_t target, ADC_HandleTypeDef *hadc, float resist, uint16_t oversampling);
void adc_init(ADC_HandleTypeDef *hadc);

float photores_get_data(photoresistor_t photoresistor_);

#endif /* PHOTORES_H_ */
