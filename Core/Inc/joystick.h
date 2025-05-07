/*
 * joystick.h
 *
 *  Created on: Apr 24, 2025
 *      Author: vlad.shcherbakov
 */


/*
 *
 *
 *
 *
 *
 *
 *
 */

#ifndef SRC_JOYSTICK_H_
#define SRC_JOYSTICK_H_

#define R2 330.0
#define R1 220.0
#define PRESCALING_FACTOR (R2/(R1+R2)) // R2/(R1+ R2), inverse of scaling down factor of input signal by a resistor divider
#define VREF 3.3

// Maximum ADC value at 16-bit precision
#define ADC_FULL_SCALE_16BIT_DOUBLE (pow(2.0,16.0) - 1.0)
#define ADC_FULL_SCALE_16BIT 0xFFFF

struct joystick_t;

#define JOYSTICK_ADC_CHANNEL_N 3

struct joystick_t {
	ADC_HandleTypeDef* hadc;

	// Joystick channel 1
	// Channel 1 -> Resistor Divider 1 -> ADC Channel 1
	uint32_t channel1_se; // single-ended measurement of channel 1

	// Joystick channel 2
	// Channel 2 -> Resistor Divider 2 -> ADC Channel 2
	uint32_t channel2_se; // single-ended measurement of channel 2

	// Divider gain estimation
	// VREF -> Resistor Divider 3 -> ADC Channel 3
	uint32_t divider_calibration_se;

	// 5V level measurement
	// VREF -> Resistor Divider 4 -> ADC Channel 4

	//uint32_t channels_dm; // differential measurement between channels 1 and 2
	// DMA buffer
	uint16_t adcResultsDMA [JOYSTICK_ADC_CHANNEL_N];
	int adcChannelCount;
};

struct joystick_t joystick_new(ADC_HandleTypeDef* hadc);

int joystick_read(struct joystick_t* joystick);
uint16_t joystick_get_sensor1_SE(struct joystick_t* joystick);
uint16_t joystick_get_sensor2_SE(struct joystick_t* joystick);
int16_t joystick_get_throttle1(struct joystick_t* joystick);
int16_t joystick_get_throttle2(struct joystick_t* joystick);
double joystick_get_divider_Av(struct joystick_t* joystick);
uint16_t adc_value_to_millivolts_16bit(uint16_t adc_value, double scale);
//uint32_t joystick_get_sensors_DM(struct joystick_t* joystick);

struct joystick_t joystick_deinit(struct joystick_t*);

#endif /* SRC_JOYSTICK_H_ */
