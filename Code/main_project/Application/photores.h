#ifndef PHOTORES_H_
#define PHOTORES_H_

#include <includes.h>

photorezistor_t photores_create_descriptor(float resist, ADC_HandleTypeDef *hadc);
float photores_get_data(photorezistor_t photores_);

#endif /* PHOTORES_H_ */
