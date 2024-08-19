#include "math/pid.h"


void PID_init(struct PID* pid){
	for(int i = 0; i < dimension_in; i++){
		for(int j = 0; j < dimension_out; j++){
			pid->kp[i][j] = 0;
			pid->ki[i][j] = 0;
			pid->kd[i][j] = 0;
		}
	}
	for(int j = 0; j < dimension_out; j++){
		pid->prev_data[j] = 0;
		pid->error[j] = 0;
		pid->out[j] = 0;
	}
}
/*
 * функция set_PID_coefficients написана для регуятора не корректирующего вращение ракеты
 * Также считается, что ракета симметрична
 * и коэффиценты по одной горизонтальной оси полность совпадают с коэффицентами по другой горизонтальной оси
 */
void set_PID_coefficients(struct PID* pid,float Kp, float Ki, float Kd){

	for(int i = 0; i < dimension_out; i++){
			for(int j = 0; j < dimension_in; j++){
				if((i + j) % 2 != 0){
					pid->kp[i][j] = 0;
					pid->ki[i][j] = 0;
					pid->kd[i][j] = 0;
				} else {
					if(i == 0 || i == 1){
						pid->kp[i][j] = Kp;
						pid->ki[i][j] = Ki;
						pid->kd[i][j] = Kd;
					} else {
						pid->kp[i][j] = -Kp;
						pid->ki[i][j] = -Ki;
						pid->kd[i][j] = -Kd;
					}
				}
			}
		}
}


void get_PID_out(struct PID* pid, struct Angle* angle, struct Angle_velocity* angle_velocity, float set_data[dimension_in]){

	int16_t protect_out[dimension_out] = {0,};

	for(int i = 0;i < dimension_in; i++){
		pid->error[i] = set_data[i] - angle->angle[i];
	}

	for(int i = 0; i < dimension_out; i++){
		for(int j = 0; j < dimension_in; j++){
			protect_out[i] += pid->kp[i][j]*pid->error[j];
		}

		if(protect_out[i] > 90){
			pid->out[i] = 90;
		}
		else if (protect_out[i] < -90){
			pid->out[i] = -90;
		}
		else {
			pid->out[i] = protect_out[i];
		}
	}
}

void set_pwm(struct PID* pid){
	TIM2->CCR1 = degrees_to_pulse(pid->out[0]);
	TIM2->CCR2 = degrees_to_pulse(pid->out[1]);
	TIM2->CCR3 = degrees_to_pulse(pid->out[2]);
	TIM2->CCR4 = degrees_to_pulse(pid->out[3]);
}
