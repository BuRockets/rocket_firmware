#ifndef __FILTER_H
#define __FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

extern float k_accel;

extern float k_gyro;

extern float k_alt;

#include <math.h>
#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdio.h>

#define MAX_SIZE 10 // Задайте максимальный размер массива
#define TYPE float    // Задайте нужный тип данных

struct GMedian{
    TYPE buffer[MAX_SIZE];
    uint8_t count;

    TYPE destination_f_m;
};

void initGMedian(struct GMedian* gmedian);

TYPE filtered(struct GMedian* gmedian, TYPE newVal);

float average_filter(float newVal, float filVal, float k);


#ifdef __cplusplus
}
#endif

#endif
