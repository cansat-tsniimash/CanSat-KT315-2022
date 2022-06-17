#include <photores.h>
#include <includes.h>

#include <errno.h>
#include <math.h>

static int channel_config_for_target(analog_target_t target, ADC_ChannelConfTypeDef * config)
{
	int error = 0;
	switch(target) {
	case ANALOG_TARGET_ROCKET_CHECKER:
		config->Channel = ADC_CHANNEL_0;
		config->Rank = 1;
		config->SamplingTime = ADC_SAMPLETIME_480CYCLES;
		break;

	case ANALOG_TARGER_SEED_CHECKER:
		config->Channel = ADC_CHANNEL_0;
		config->Rank = 1;
		config->SamplingTime = ADC_SAMPLETIME_480CYCLES;
		break;

	default:
		error = -ENOSYS;
		break;
	}

	return error;
}


photoresistor_t photores_create_descriptor(analog_target_t target, ADC_HandleTypeDef *hadc, float resist, uint16_t oversampling) {
	photoresistor_t photoresistor_ = {
		.target = target,
		.adc = hadc,
		.resist = resist,
		.oversampling = oversampling
	};
	return photoresistor_;
}

void adc_init(ADC_HandleTypeDef *hadc) {
	__HAL_ADC_ENABLE(hadc);
	return;
}


static void analog_get_raw(analog_target_t target, ADC_HandleTypeDef *hadc, uint16_t oversampling, uint16_t *value) {
	ADC_ChannelConfTypeDef target_config;
	channel_config_for_target(target, &target_config);


	ADC_ChannelConfTypeDef vtref_config;
	vtref_config.Channel = ADC_CHANNEL_VREFINT;
	vtref_config.Offset = 0;
	vtref_config.Rank = 1;
	vtref_config.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	uint32_t raw_sum = 0;
	uint32_t vrefint_sum = 0;
	for (uint16_t i = 0; i < oversampling; i++)
	{
		// Замер целевого напряжения
		HAL_ADC_ConfigChannel(hadc, &target_config);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
		raw_sum += HAL_ADC_GetValue(hadc);

		// Замер вдда
		HAL_ADC_ConfigChannel(hadc, &vtref_config);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
		vrefint_sum += HAL_ADC_GetValue(hadc);
	}

	uint32_t retval = raw_sum / oversampling;
	uint32_t vrefint = vrefint_sum / oversampling;

	// Корректируем с учетом внешнего напряжения
	retval = retval * (*VREFINT_CAL_ADDR) / vrefint ;

	*value = (uint16_t)retval;
	return;
}
/*
 * error = hal_status_to_errno(hal_error);
 * if (0 != error) return error;
 */

float photores_get_data(photoresistor_t photoresistor_) {
	uint16_t raw_voltage = 0;
	analog_get_raw(photoresistor_.target, photoresistor_.adc, photoresistor_.oversampling, &raw_voltage);
	float voltage = raw_voltage * 3.3 / 4095;
	float resistance = voltage * (photoresistor_.resist) / (3.3 - voltage);
	float lux = exp((3.823 - log(resistance / 1000)) / 0.816) * 10.764;
	return lux;
}
