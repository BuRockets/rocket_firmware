#include "system/inertia.h"

//float destination_a[3] = { 0, };
//float destination_g[3] = { 0, };
//int16_t destination_m[3] = { 0, };
//float acceleration = 0;



float pow2(float a){
	return a*a;
}

void accelerate_init(struct Accelerate* accelerate){
	if (accelerate == NULL) {
	        // Обработка ошибки
	        return;
	}
	for(int i = 0; i < 3; i++){
		accelerate->destination_a[i] = 0;
		accelerate->destination_a_f[i] = 0;
	}
	accelerate->accel_mod = 0;
}

void gyro_init(struct Gyro* gyro){
	for(int i = 0; i < 3; i++){
		gyro->destination_g[i] = 0;
		gyro->destination_g_f[i] = 0;
	}
	gyro->gyro_mod = 0;
}

float acceleration_modulus(float* destination_a){
	return sqrt(pow2(destination_a[0]) + pow2(destination_a[1]) + pow2(destination_a[2]));

}

void inertia_init(struct Accelerate* accelerate, struct Gyro* gyro){
	//accelerate_init(accelerate);
	//gyro_init(gyro);
	MPU_init();
}

void get_inertia_measurement(struct Rocket* rocket){
	MPU_get_accel(rocket->accelerate->destination_a);
	MPU_get_gyro(rocket->gyro->destination_g);
	//rocket->accelerate->destination_a_f[0] = average_filter(rocket->accelerate->destination_a[0],rocket->accelerate->destination_a_f[0], k_accel);
	/*for(int i = 0; i < 3; i++){
		rocket->accelerate->destination_a_f[i] = average_filter(rocket->accelerate->destination_a[i],rocket->accelerate->destination_a_f[i], k_accel);
		rocket->gyro->destination_g_f[i] = average_filter(rocket->gyro->destination_g[i],rocket->gyro->destination_g_f[i], k_gyro);

	}*///если раскоментировать, то только for
	//destination_a_f_m[0] = filtered(&gmedian, rocket->accelerate->destination_a_f[0]);
	//rocket->accelerate->destination_a[i]
}

void filtered_inertia_measurement(struct Rocket* rocket, struct GMedian* gmedian_a, struct GMedian* gmedian_g){

	for(int i = 0; i < 3; i++){
			gmedian_a[i].destination_f_m = filtered(&(gmedian_a[i]), rocket->accelerate->destination_a[i]);
			rocket->accelerate->destination_a_f[i] = average_filter(gmedian_a[i].destination_f_m,rocket->accelerate->destination_a_f[i], k_accel);

			gmedian_g[i].destination_f_m = filtered(&(gmedian_g[i]), rocket->gyro->destination_g[i]);
			rocket->gyro->destination_g_f[i] = average_filter(gmedian_g[i].destination_f_m,rocket->gyro->destination_g_f[i], k_gyro);
		}
}

void get_inertia_measurement_mod(struct Rocket* rocket){
	rocket->accelerate->accel_mod = acceleration_modulus(rocket->accelerate->destination_a_f);
	rocket->gyro->gyro_mod = acceleration_modulus(rocket->gyro->destination_g_f);
}
