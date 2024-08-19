#include "math/filter.h"

#define NUM_READ  25

float k_accel = 0.09;															//коэфицент фильтра скользящего среднего для данных с акселерометра

float k_gyro = 0.09;															//коэфицент фильтра скользящего среднего для данных с гироскопа

float k_alt = 0.09;																//коэфицент фильтра скользящего среднего для данных с барометра

float average_filter(float newVal, float filVal, float k){
	filVal += (newVal - filVal)*k;
	return filVal;
}


void initGMedian(struct GMedian* gmedian) {
    gmedian->count = 0;
    gmedian->destination_f_m = 0;
}

TYPE filtered(struct GMedian* gmedian, TYPE newVal) {
    gmedian->buffer[gmedian->count] = newVal;

    if ((gmedian->count < MAX_SIZE - 1) && (gmedian->buffer[gmedian->count] > gmedian->buffer[gmedian->count + 1])) {
        for (int i = gmedian->count; i < MAX_SIZE - 1; i++) {
            if (gmedian->buffer[i] > gmedian->buffer[i + 1]) {
                TYPE buff = gmedian->buffer[i];
                gmedian->buffer[i] = gmedian->buffer[i + 1];
                gmedian->buffer[i + 1] = buff;
            }
        }
    } else {
        if ((gmedian->count > 0) && (gmedian->buffer[gmedian->count - 1] > gmedian->buffer[gmedian->count])) {
            for (int i = gmedian->count; i > 0; i--) {
                if (gmedian->buffer[i] < gmedian->buffer[i - 1]) {
                    TYPE buff = gmedian->buffer[i];
                    gmedian->buffer[i] = gmedian->buffer[i - 1];
                    gmedian->buffer[i - 1] = buff;
                }
            }
        }
    }

    if (++gmedian->count >= MAX_SIZE) {
        gmedian->count = 0;
    }

    return gmedian->buffer[MAX_SIZE / 2];
}
