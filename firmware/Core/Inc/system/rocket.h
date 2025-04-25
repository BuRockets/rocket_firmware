#ifndef __ROCKET_H
#define __ROCKET_H

#ifdef __cplusplus
extern "C" {
#endif



#include "../stm32f4xx_hal.h"
#include "main.h"
//#include "inertia.h"
#include <stdbool.h>
#include "math/angle.h"

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

// не могу объявить структуры Accelerate и Gyro в файле inertia.h, возникает ошибка сборки
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


struct Atmosphere_param{

	float pressure;								//давление за бортом
	float temperature;							//температура за бортом
	float humidity;								//влажность за бортом

	float start_pressure;						//давление за бортом во время инициализации
	float start_temperature;					//температура за бортом во время инициализации
	float start_humidity;						//влажность за бортом во время инициализации
};

struct Altitude{

	float altitude;
	float altitude_f;
	float max_altitude;
};

struct Rocket{

	char teamId[10];							//название команды

	struct Altitude* altitude;

	/*
	float altitude;								//высота полета

	float max_altitude;							//максимальная высота полета


	float destination_a[3];						//вектор ускорения
	float destination_a_f[3];					//отфильтрованый вектор ускорения
	float acceleration;							//модуль вектора ускорения

	float destination_g[3];						//вектор скорости
	float destination_g_f[3];					//вектор скорости с фильтром
	float gyro;									//амплитуда вектора скорости
	*/
	//struct Inertia inertia;

	bool start_point;							//становится true если altitude > starting_height
	bool apogee_point;							//становится true если (max_alt - altitude) > delta_apogee
	bool activate_point;						//уведомляет о необходимости открытия парашюта
	bool landing_point;							//становится true при приземлении

	uint8_t delta_apogee;						//сколько нужно отпуститься метров от максимального значения max_altitude чтобы был зафиксирован апогей
	uint8_t delta_activate;					//какая должна быть разница между максимальной высотой и действительной высотой чтобы раскрылся парашют
	uint8_t starting_height;					//при atitude == starting height start_point становится true, фиксация взлета ракеты

	float battery_voltage;						//напряжение батареи

	float time;									//время

	float accel_mod;							//модуль ускорения, копируется из структуры accelerate

	struct Atmosphere_param* atmosphere;			//параметры атмосферы снаружи ракеты

	struct Angle* angle;

	struct Angle_velocity* angle_velocity;

	//struct Accelerate* accelerate;

	//struct Gyro* gyro;

};




void rocket_init(struct Rocket *rocket, const char *teamId, struct Altitude* altitude, struct Atmosphere_param* atmosphere, struct Accelerate* accelerate, struct Angle* angle, struct Angle_velocity* angle_velocity);

void atmosphere_init(struct Atmosphere_param *atmosphere);

void fly_control(struct Rocket* rocket);

void delta_init(struct Rocket* rocket);


#ifdef __cplusplus
}
#endif

#endif
