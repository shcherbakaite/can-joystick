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
		0,       // channel1_se
		0,       // channel2_se
		0,       // divider_calibration_se
		{0,0,0}, // adcResultsDMA
	};
	return joystick;
}

int joystick_read(struct joystick_t* joystick) {
	HAL_ADC_Start_DMA(joystick->hadc, (uint32_t*)joystick->adcResultsDMA, JOYSTICK_ADC_CHANNEL_N);
	while(adc1_conversion_complete_flag == 0) {} // wait for conversion
	adc1_conversion_complete_flag = 0;           // reset flag
	HAL_ADC_Stop_DMA(joystick->hadc);

	// copy data from DMA buffer
	joystick->channel1_se = joystick->adcResultsDMA[0];
	joystick->channel2_se = joystick->adcResultsDMA[1];
	joystick->divider_calibration_se = joystick->adcResultsDMA[2];
	//joystick->channels_dm = joystick->adcResultsDMA[2];

	return 0;
}

uint16_t adc_value_to_millivolts_16bit(uint16_t adc_value, double scale) {
	//uint adc_full_scale = pow(2,16) - 1;
	double vin_f = scale*VREF*((double)adc_value)/ADC_FULL_SCALE_16BIT;
	uint16_t vin = (uint32_t)round((vin_f*1000.0));
	return vin;
}

uint16_t joystick_get_sensor1_SE(struct joystick_t* joystick) {
	double divider_Av = joystick_get_divider_Av(joystick);
	return adc_value_to_millivolts_16bit(joystick->channel1_se, 1.0/divider_Av);
}

uint16_t joystick_get_sensor2_SE(struct joystick_t* joystick) {
	double divider_Av = joystick_get_divider_Av(joystick);
	return adc_value_to_millivolts_16bit(joystick->channel2_se, 1.0/divider_Av);
}

int16_t joystick_get_throttle1(struct joystick_t* joystick) {
	int16_t sensor1 = joystick_get_sensor1_SE(joystick);
	return (uint16_t)(1000*(sensor1 - 500)/(4500 - 500));
}

int16_t joystick_get_throttle2(struct joystick_t* joystick) {
	int16_t sensor2 = joystick_get_sensor2_SE(joystick);
	return 1000*((-sensor2 + 5000) - 500)/(4500 - 500);
}

double joystick_get_divider_Av(struct joystick_t* joystick) {
	return (double)joystick->divider_calibration_se/(double)ADC_FULL_SCALE_16BIT;
}

//uint32_t joystick_get_sensors_DM(struct joystick_t* joystick) {
//	return joystick->channels_dm;
//}
