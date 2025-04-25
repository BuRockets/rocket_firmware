#include <stdio.h>
#include <string.h>
#include "system/rocket.h"
#include "MPU9250.h"

void rocket_init(struct Rocket *rocket, const char *teamId, struct Altitude* altitude, struct Atmosphere_param* atmosphere,struct Accelerate* accelerate, struct Angle* angle, struct Angle_velocity* angle_velocity) {

	strncpy(rocket->teamId, teamId, sizeof(rocket->teamId) - 1);
	rocket->teamId[sizeof(rocket->teamId) - 1] = '\0'; // Обеспечиваем нуль-терминатор

	/*rocket->altitude = 0;

	rocket->max_altitude = 0;*/

	rocket->altitude = altitude;

    /*for(int i = 0;i < 3; i++){
    	rocket->destination_a[i] = 0;
    	rocket->destination_g[i] = 0;
    }

    rocket->acceleration = 0;

    rocket->gyro = 0;*/

	//rocket->accelerate = accelerate;

	//rocket->gyro = gyro;

	rocket->angle = angle;

	rocket->angle_velocity = angle_velocity;

	rocket->accel_mod = accelerate->accel_mod;

    rocket->start_point = 0;
    rocket->apogee_point = 0;
    rocket->activate_point = 0;
    rocket->landing_point = 0;

    //rocket->delta_apogee = 1;
    //rocket->delta_activate = 1;
    //rocket->starting_height = 3;

    rocket->battery_voltage = 0;

    rocket->time = HAL_GetTick();

    rocket->atmosphere = atmosphere;

}

void atmosphere_init(struct Atmosphere_param* atmosphere){
	atmosphere->pressure = 0;
	atmosphere->temperature = 0;
	atmosphere->humidity = 0;

	atmosphere->start_pressure = 0;
	atmosphere->start_temperature = 0;
	atmosphere->start_humidity = 0;
}

void fly_control(struct Rocket* rocket){
	if (rocket->altitude->altitude > rocket->starting_height)
	{
	  rocket->start_point = 1;
	}

	rocket->altitude->max_altitude= fmax( rocket->altitude->max_altitude, rocket->altitude->altitude );

	if ((rocket->altitude->max_altitude - rocket->altitude->altitude) > rocket->delta_apogee && rocket->start_point == 1)
	{
	  rocket->apogee_point = 1;
	}

	if ((rocket->altitude->max_altitude - rocket->altitude->altitude) > rocket->delta_activate && rocket->apogee_point == 1)
	{
	  rocket->activate_point = 1;
	}

	if(rocket->start_point == 1 && rocket->apogee_point == 1 && rocket->activate_point == 1 && rocket->altitude->altitude < rocket->starting_height)//высота считающаяся приземлением
	{
	  rocket->landing_point = 1;
	}
}

void delta_init(struct Rocket* rocket){
	rocket->delta_apogee = 1;
	rocket->delta_activate = 1;
	rocket->starting_height = 2;
}
