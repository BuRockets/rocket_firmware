#ifndef __RADIO_H
#define __RADIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../stm32f4xx_hal.h"
#include "usart.h"
#include <stdbool.h>
#include <math.h>

struct Radio{

	bool TRANSMIT_IS_OK;
	bool ALTITUDE_IS_OK;
	bool PITCH_IS_OK;
	bool ROLL_IS_OK;
	bool YAW_IS_OK;
	bool D_PITCH_IS_OK;
	bool D_ROLL_IS_OK;
	bool D_YAW_IS_OK;
	bool TEMPERATURE_IS_OK;
	bool BATTERY_VOLTAGE_IS_OK;

	uint16_t frequency_data_transmission;
};

void radio_init(struct Radio* radio);

void transmit_data(struct Rocket* rocket, struct Radio* radio);

void set_data_transmit_frequency(TIM_HandleTypeDef *htim, struct Radio* radio);






#ifdef __cplusplus
}
#endif

#endif
