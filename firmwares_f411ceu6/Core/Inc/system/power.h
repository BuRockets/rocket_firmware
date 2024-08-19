#ifndef __POWER_H
#define __POWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../stm32f4xx_hal.h"
#include "adc.h"

#define ADC_REFERENCE_VOLTAGE                                           1.2
#define ADC_MAX                                                         0xFFF

extern uint16_t adcData;

float get_mcu_voltage();


#ifdef __cplusplus
}
#endif

#endif
