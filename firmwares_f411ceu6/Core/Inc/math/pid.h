#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "stm32f4xx_hal.h"
#include "angle.h"
#include "tim.h"

#include <stdint.h>
#include <stdio.h>

#define dimension_in  2

#define dimension_out 4

#define MAX_ANGLE     90

#define MIN_ANGLE     -90

struct Angle;

struct Angle_velocity;

struct PID {
	float kp[dimension_out][dimension_in];
	float ki[dimension_out][dimension_in];
	float kd[dimension_out][dimension_in];
	float data[dimension_in];
	float prev_data[dimension_in];
	float error[dimension_in];
	float prev_error[dimension_in];
	float w_error[dimension_in];
	float w_prev_error[dimension_in];
	int16_t out[dimension_out];

};

void PID_init(struct PID* pid);

void set_PID_coefficients(struct PID* pid,float Kp, float Ki, float Kd);

void get_PID_out(struct PID* pid, struct Angle* angle, struct Angle_velocity* angle_velocity, float set_data[dimension_in]);

void set_pwm(struct PID* pid);

#ifdef __cplusplus
}
#endif

#endif
