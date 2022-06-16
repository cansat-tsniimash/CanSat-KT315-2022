#include <photores.h>
#include <includes.h>

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


photoresistor_t photores_create_descriptor(analog_target_t target, float resist, ADC_HandleTypeDef *hadc) {
	photoresistor_t photoresistor_ = {
		.target = target,
		.resist = resist,
		.adc = hadc
	};
	return photoresistor_;
}

void adc1_init(void) {
	__HAL_ADC_ENABLE(&hadc1);
	return;
}


static int analog_get_raw(analog_target_t target, uint16_t oversampling, uint16_t * value) {
	int error = 0;

	ADC_ChannelConfTypeDef target_config;
	error = channel_config_for_target(target, &target_config);
	if (0 != error)
		return error;

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
		HAL_StatusTypeDef hal_error = HAL_ADC_ConfigChannel(_ADC_HANDLE, &target_config);
		error = hal_status_to_errno(hal_error);
		if (0 != error)
			return error;

		hal_error = HAL_ADC_Start(&hadc1);
		error = hal_status_to_errno(hal_error);
		if (0 != error)
			return error;

		hal_error = HAL_ADC_PollForConversion(_ADC_HANDLE, _ADC_HAL_TIMEOUT);
		error = hal_status_to_errno(hal_error);
		if (0 != error)
			return error;

		raw_sum += HAL_ADC_GetValue(&hadc1);


		// Замер вдда
		hal_error = HAL_ADC_ConfigChannel(_ADC_HANDLE, &vtref_config);
		error = hal_status_to_errno(hal_error);
		if (0 != error)
			return error;

		hal_error = HAL_ADC_Start(&hadc1);
		error = hal_status_to_errno(hal_error);
		if (0 != error)
			return error;

		hal_error = HAL_ADC_PollForConversion(_ADC_HANDLE, _ADC_HAL_TIMEOUT);
		error = hal_status_to_errno(hal_error);
		if (0 != error)
			return error;

		vrefint_sum += HAL_ADC_GetValue(&hadc1);
	}

	uint32_t retval = raw_sum / oversampling;
	uint32_t vrefint = vrefint_sum / oversampling;

	// Корректируем с учетом внешнего напряжения
	retval = retval * (*VREFINT_CAL_ADDR) / vrefint ;

	*value = (uint16_t)retval;
	return 0;
}

float photores_get_data(photoresistor_t photoresistor_) {
	float voltage = 0;
	analog_get_raw(photoresistor_.target, photoresistor_.oversampling, &voltage);
	voltage = voltage * 3.3 / 4095;
	float resistance = voltage * (photoresistor_.resist) / (3.3 - voltage);
	float lux = exp((3.823 - log(resistance / 1000)) / 0.816) * 10.764;
	return lux;
}
