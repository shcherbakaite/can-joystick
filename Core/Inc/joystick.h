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

#define VREF 3.3

// Maximum ADC value at 16-bit precision
#define ADC_FULL_SCALE_16BIT_DOUBLE (pow(2.0,16.0) - 1.0)
#define ADC_FULL_SCALE_16BIT 0xFFFF

struct joystick_t;

#define JOYSTICK_DMA_CHANNEL_COUNT 4

struct joystick_t {
	ADC_HandleTypeDef* hadc;

	// Joystick channel 1, raw ADC 0 to VREF
	uint16_t channel1_se;

	// Joystick channel 2, raw ADC 0 to VREF
	uint16_t channel2_se;

	// Divider gain estimation, raw ADC 0 to VREF
	uint32_t divider_calibration_se;

	// 5V level measurement, raw ADC 0 to VREF
	uint32_t vcc_level_se;

	//uint32_t channels_dm; // differential measurement between channels 1 and 2

	// DMA buffer
	uint16_t adcResultsDMA [JOYSTICK_DMA_CHANNEL_COUNT];
	int adcChannelCount;
};

struct joystick_t joystick_new(ADC_HandleTypeDef* hadc);

uint16_t adc_value_to_mV_16bit(uint16_t adc_value, double scale);

int joystick_read(struct joystick_t* joystick);
uint16_t joystick_get_sensor1_mV(struct joystick_t* joystick);
uint16_t joystick_get_sensor2_mV(struct joystick_t* joystick);
uint16_t joystick_get_vcc_mV(struct joystick_t* joystick);
int16_t joystick_get_throttle1_permille(struct joystick_t* joystick);
int16_t joystick_get_throttle2_permille(struct joystick_t* joystick);
void joystick_get_limits_mV(struct joystick_t* joystick, uint16_t* upper, uint16_t* lower);
double joystick_get_divider_Av(struct joystick_t* joystick);

//uint32_t joystick_get_sensors_DM(struct joystick_t* joystick);

struct joystick_t joystick_deinit(struct joystick_t*);

#endif /* SRC_JOYSTICK_H_ */
