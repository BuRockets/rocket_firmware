#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include <stdio.h>
#include "../stm32f4xx_hal.h"
#include "main.h"

#define flashADDR   0x08060000
#define FLASH_USER_START_ADDR   0x08060000   // Начальный адрес последней страницы Flash
#define MAGIC_CHECKSUM_NUMBER   0x10
//#define FLASH_USER_END_ADDR     0x08020000   // Конечный адрес первой страницы после последних трех

//0x0801FC00
//0x0801F800
int CheckFlashData();

HAL_StatusTypeDef WriteToFlash(struct Rocket *rocket);

void ReadFromFlash(struct Rocket *rocket);

#ifdef __cplusplus
}
#endif

#endif
