#ifndef __ALTITUDE_H
#define __ALTITUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include <stdio.h>
#include "../stm32f4xx_hal.h"
//#include "stm32f1xx_hal_uart.h"
#include "rocket.h"
#include "bmp280.h"
#include "i2c.h"
#include "usart.h"





extern BMP280_HandleTypedef bmp280;

struct GMedian;

void altitude_init(struct Rocket* rocket);

void get_altitude_measurement(struct Rocket* rocket);

void filtered_altitude_measurement(struct Rocket* rocket, struct GMedian* gmedian_alt);

#ifdef __cplusplus
}
#endif

#endif
