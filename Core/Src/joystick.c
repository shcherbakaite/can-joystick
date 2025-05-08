/*
 * joystick.c
 *
 *  Created on: Apr 24, 2025
 *      Author: vlad.shcherbakov
 */

#include "main.h"
#include "joystick.h"

// DMA conversion complete flag from main.c
extern uint32_t adc1_conversion_complete_flag;

struct joystick_t joystick_new(ADC_HandleTypeDef* hadc) {
	struct joystick_t joystick = {
		hadc,
		0,         // channel1_se, mA
		0,         // channel2_se, mA
		0,         // divider_calibration_se, mA
		0,         // vcc_level_se, mA
		{0,0,0,0}, // adcResultsDMA
	};
	return joystick;
}

int joystick_read(struct joystick_t* joystick) {
	HAL_ADC_Start_DMA(joystick->hadc, (uint32_t*)joystick->adcResultsDMA, JOYSTICK_DMA_CHANNEL_COUNT);
	while(adc1_conversion_complete_flag == 0) {} // wait for conversion
	adc1_conversion_complete_flag = 0;           // reset flag
	HAL_ADC_Stop_DMA(joystick->hadc);

	// copy data from DMA buffer
	joystick->channel1_se = joystick->adcResultsDMA[0];
	joystick->channel2_se = joystick->adcResultsDMA[1];
	joystick->divider_calibration_se = joystick->adcResultsDMA[2];
	joystick->vcc_level_se = joystick->adcResultsDMA[3];
	//joystick->channels_dm = joystick->adcResultsDMA[2];

	return 0;
}

uint16_t adc_value_to_mV_16bit(uint16_t adc_value, double scale) {
	double vin_f = scale*VREF*((double)adc_value)/ADC_FULL_SCALE_16BIT;
	uint16_t vin = (uint32_t)round((vin_f*1000.0));
	return vin;
}

uint16_t joystick_get_sensor1_mV(struct joystick_t* joystick) {
	double divider_Av = joystick_get_divider_Av(joystick);
	return adc_value_to_mV_16bit(joystick->channel1_se, 1.0/divider_Av);
}

uint16_t joystick_get_sensor2_mV(struct joystick_t* joystick) {
	double divider_Av = joystick_get_divider_Av(joystick);
	return adc_value_to_mV_16bit(joystick->channel2_se, 1.0/divider_Av);
}

uint16_t joystick_get_vcc_mV(struct joystick_t* joystick) {
	double divider_Av = joystick_get_divider_Av(joystick);
	return adc_value_to_mV_16bit(joystick->vcc_level_se, 1.0/divider_Av);
}

int16_t joystick_get_throttle1_permille(struct joystick_t* joystick) {
	int16_t sensor1 = joystick_get_sensor1_mV(joystick);
	uint16_t upper = 0;
	uint16_t lower = 0;
	joystick_get_limits_mV(joystick, &upper, &lower);
	uint16_t vcc = joystick_get_vcc_mV(joystick);
	return (uint16_t)(1000*(sensor1 - lower)/(upper - lower));
}

int16_t joystick_get_throttle2_permille(struct joystick_t* joystick) {
	int16_t sensor2 = joystick_get_sensor2_mV(joystick);
	uint16_t upper = 0;
	uint16_t lower = 0;
	uint16_t vcc = joystick_get_vcc_mV(joystick);
	joystick_get_limits_mV(joystick, &upper, &lower);
	return 1000*((-sensor2 + vcc) - lower)/(upper - lower);
}

void joystick_get_limits_mV(struct joystick_t* joystick, uint16_t* upper, uint16_t* lower) {
	uint16_t vcc = joystick_get_vcc_mV(joystick);
	*upper = (uint16_t)round((double)vcc*0.9);
	*lower = (uint16_t)round((double)vcc*0.1);
}

double joystick_get_divider_Av(struct joystick_t* joystick) {
	return (double)joystick->divider_calibration_se/(double)ADC_FULL_SCALE_16BIT;
}

//uint32_t joystick_get_sensors_DM(struct joystick_t* joystick) {
//	return joystick->channels_dm;
//}
