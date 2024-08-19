#ifndef __CALLBACK_H
#define __CALLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include <stdio.h>
#include "../stm32f4xx_hal.h"
#include "usart.h"
#include "main.h"
#include "flash.h"
#include "system/radio.h"
#include "tim.h"


uint16_t Hz_to_ms(uint16_t frequency);

#ifdef __cplusplus
}
#endif

#endif
