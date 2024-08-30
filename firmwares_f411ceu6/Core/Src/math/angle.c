#include "math/angle.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

void angle_init(struct Angle* angle){
	angle->pitch = 0;
	angle->roll = 0;
	angle->yaw = 0;
}

void angle_velocity_init(struct Angle_velocity* angle_velocity){
	angle_velocity->d_pitch = 0;
	angle_velocity->d_roll = 0;
	angle_velocity->d_yaw = 0;
	for(int i = 0; i < dimension_in; i++){
		angle_velocity->d_angle[i] = 0;
	}
}

void angle_calculate(struct Angle* angle, struct Accelerate* accelerate){

	float Bx = accelerate->destination_a_f[0];
	float By = accelerate->destination_a_f[1];
	float Bz = accelerate->destination_a_f[2];

	angle->pitch = atan2(-Bx, sqrt(By * By + Bz * Bz)) / M_PI * 180;
	angle->roll = atan2(By, Bz) / M_PI * 180;
	angle->yaw = 0;//надо реализовать

	angle->angle[0] = atan2(-Bx, sqrt(By * By + Bz * Bz)) / M_PI * 180;
	angle->angle[1] = atan2(By, Bz) / M_PI * 180;
	angle->angle[2] = 0;//надо реализовать
}


void angle_velocity_calculate(struct Angle_velocity* angle_velocity, struct Gyro* gyro){

	angle_velocity->d_pitch = gyro->destination_g_f[0];
	angle_velocity->d_roll = gyro->destination_g_f[1];
	angle_velocity->d_yaw = gyro->destination_g_f[2];

	for(int i = 0; i < 3; i++){
		angle_velocity->d_angle[i] = gyro->destination_g_f[i];
	}
}
