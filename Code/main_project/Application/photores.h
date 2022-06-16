#ifndef PHOTORES_H_
#define PHOTORES_H_

#include <includes.h>

typedef enum {
	ANALOG_TARGET_ROCKET_CHECKER,
	ANALOG_TARGER_SEED_CHECKER
} analog_target_t;

typedef struct {
	analog_target_t target;
	ADC_HandleTypeDef* adc;
	float resist;
	uint16_t oversampling;
} photoresistor_t;

photoresistor_t photores_create_descriptor(float resist, ADC_HandleTypeDef *hadc);
void adc1_init(void);

float photores_get_data(photoresistor_t photores_);

#endif /* PHOTORES_H_ */
