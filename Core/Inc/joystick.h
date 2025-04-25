/*
 * joystick.h
 *
 *  Created on: Apr 24, 2025
 *      Author: vlad.shcherbakov
 */

#ifndef SRC_JOYSTICK_H_
#define SRC_JOYSTICK_H_

struct joystick_t;

struct joystick_t {
	ADC_HandleTypeDef* hadc;
	uint32_t channel1_se; // single-ended measurement of channel 1
	uint32_t channel2_se; // single-ended measurement of channel 2
	uint32_t channels_dm; // differential measurement between channels 1 and 2
	// DMA buffer
	uint16_t adcResultsDMA [2];
	int adcChannelCount;
};

struct joystick_t joystick_new(ADC_HandleTypeDef* hadc);

int joystick_read(struct joystick_t* joystick);
uint32_t joystick_get_sensor1_SE(struct joystick_t* joystick);
uint32_t joystick_get_sensor2_SE(struct joystick_t* joystick);
//uint32_t joystick_get_sensors_DM(struct joystick_t* joystick);

struct joystick_t joystick_deinit(struct joystick_t*);

#endif /* SRC_JOYSTICK_H_ */
