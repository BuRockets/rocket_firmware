#ifndef __ANGLE_H
#define __ANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "stm32f4xx_hal.h"

#include "system/inertia.h"
#include "math/pid.h"


struct Accelerate;
struct Gyro;

/*
struct Angle {
	float pitch;
	float roll;
	float yaw;
	float angle[3];
};

struct Angle_velocity{
	float d_pitch;
	float d_roll;
	float d_yaw;
	float d_angle[3];
};
*/
void angle_init(struct Angle* angle);

void angle_velocity_init(struct Angle_velocity* angle_velocity);

void angle_calculate(struct Angle* angle, struct Accelerate* accelerate);

void angle_velocity_calculate(struct Angle_velocity* angle_velocity, struct Gyro* gyro);

#ifdef __cplusplus
}
#endif

#endif
