#ifndef __MPU9250_H
#define __MPU9250_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"


void Error_Handler(void);

void MPU_init(void);

void MAGN_init(float * destination);

void MPU_get_magn(int16_t * destination);

void MPU_get_accel(float * destination);

void MPU_get_gyro(float * destination);


#ifdef __cplusplus
}
#endif

#endif /* __MPU9250_H */
