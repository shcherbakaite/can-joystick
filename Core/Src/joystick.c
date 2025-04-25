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
		0,       // channel2_dm
		{0,0},   // adcResultsDMA
		2        // adcChannelCount
	};
	return joystick;
}

int joystick_read(struct joystick_t* joystick) {
	HAL_ADC_Start_DMA(joystick->hadc, (uint32_t*)joystick->adcResultsDMA, joystick->adcChannelCount);
	while(adc1_conversion_complete_flag == 0) {} // wait for conversion
	adc1_conversion_complete_flag = 0;           // reset flag
	HAL_ADC_Stop_DMA(joystick->hadc);

	// copy data from DMA buffer
	joystick->channel1_se = joystick->adcResultsDMA[0];
	joystick->channel2_se = joystick->adcResultsDMA[1];
	//joystick->channels_dm = joystick->adcResultsDMA[2];

	return 0;
}

uint32_t joystick_get_sensor1_SE(struct joystick_t* joystick) {
	return joystick->channel1_se;
}

uint32_t joystick_get_sensor2_SE(struct joystick_t* joystick) {
	return joystick->channel2_se;
}

//uint32_t joystick_get_sensors_DM(struct joystick_t* joystick) {
//	return joystick->channels_dm;
//}
