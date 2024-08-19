#ifndef __INERTIA_H
#define __INERTIA_H

#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include "../stm32f4xx_hal.h"
#include "main.h"
//#include "rocket.h"
#include "MPU9250.h"
#include "math/filter.h"
//extern float destination_a[3];
//extern float destination_g[3];
//extern int16_t destination_m[3];
//extern float acceleration;

/*
struct Accelerate{
	float destination_a[3];
	float destination_a_f[3];
	float accel_mod;
};

struct Gyro{
	float destination_g[3];
	float destination_g_f[3];
	float gyro_mod;
};
*/

struct Accelerate;

struct Gyro;

struct Rocket;

void accelerate_init(struct Accelerate* accelerate);

void gyro_init(struct Gyro* gyro);

float pow2(float a);

float acceleration_modulus(float destination_a[3]);

void inertia_init(struct Accelerate* accelerate, struct Gyro* gyro);

void get_inertia_measurement(struct Rocket* rocket);

void get_inertia_measurement_mod(struct Rocket* rocket);

void filtered_inertia_measurement(struct Rocket* rocket, struct GMedian* gmedian_a, struct GMedian* gmedian_g);


#ifdef __cplusplus
}
#endif

#endif
