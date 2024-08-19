#ifndef __RESCUE_H
#define __RESCUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"

uint16_t degrees_to_pulse(int16_t degrees);

void turn_servo(int16_t degrees);

void rescue_system_init(TIM_TypeDef* TIM);








#ifdef __cplusplus
}
#endif

#endif
