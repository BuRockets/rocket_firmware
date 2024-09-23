#include "system/radio.h"


void radio_init(struct Radio* radio){
	radio->TRANSMIT_IS_OK = false;
	radio->ALTITUDE_IS_OK = false;
	radio->PITCH_IS_OK = false;
	radio->ROLL_IS_OK = false;
	radio->YAW_IS_OK = false;
	radio->D_PITCH_IS_OK = false;
	radio->D_ROLL_IS_OK = false;
	radio->D_YAW_IS_OK = false;
	radio->TEMPERATURE_IS_OK = false;
	radio->BATTERY_VOLTAGE_IS_OK = false;
	radio->frequency_data_transmission = 1;
}

void set_data_transmit_frequency(TIM_HandleTypeDef *htim, struct Radio* radio){
	htim->Instance->ARR = 8*powf(10,6) / radio->frequency_data_transmission/ htim->Instance->PSC;
}

void transmit_data(struct Rocket* rocket, struct Radio* radio){
	/*char buf[100];

	snprintf(buf, sizeof(buf), "%s %f %f %f %f %d %d %d %d;\n", rocket->teamId, rocket->time, rocket->battery_voltage, rocket->altitude->altitude_f, rocket->angle->pitch, rocket->start_point, rocket->apogee_point, rocket->activate_point, rocket->landing_point);

	if(radio->TRANSMIT_IS_OK == true){
		HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
	}*/
	char time_buf[50];
	char altitude_buf[50];
	char pitch_buf[50];
	char roll_buf[50];
	char yaw_buf[50];
	char d_pitch_buf[50];
	char d_roll_buf[50];
	char d_yaw_buf[50];
	char temperature_buf[50];
	char battery_voltage_buf[50];


	if(radio->TRANSMIT_IS_OK == true){
		snprintf(time_buf, sizeof(time_buf), "time:%f ", rocket->time);
		HAL_UART_Transmit(&huart1, (uint8_t*)time_buf, strlen(time_buf), 100);

		if(radio->ALTITUDE_IS_OK == true){
			snprintf(altitude_buf, sizeof(altitude_buf), "altitude:%f ", rocket->altitude->altitude_f);
			HAL_UART_Transmit(&huart1, (uint8_t*)altitude_buf, strlen(altitude_buf), 100);
		}
		if(radio->PITCH_IS_OK == true){
			snprintf(pitch_buf, sizeof(pitch_buf), "pitch:%f ", rocket->angle->pitch);
			HAL_UART_Transmit(&huart1, (uint8_t*)pitch_buf, strlen(pitch_buf), 100);
		}
		if(radio->ROLL_IS_OK == true){
			snprintf(roll_buf, sizeof(roll_buf), "roll:%f ", rocket->angle->roll);
			HAL_UART_Transmit(&huart1, (uint8_t*)roll_buf, strlen(roll_buf), 100);
		}
		if(radio->YAW_IS_OK == true){
			snprintf(yaw_buf, sizeof(yaw_buf), "yaw:%f ", rocket->angle->yaw);
			HAL_UART_Transmit(&huart1, (uint8_t*)yaw_buf, strlen(yaw_buf), 100);
		}

		if(radio->D_PITCH_IS_OK == true){
			snprintf(d_pitch_buf, sizeof(d_pitch_buf), "d_pitch:%f ", rocket->angle_velocity->d_pitch);
			HAL_UART_Transmit(&huart1, (uint8_t*)d_pitch_buf, strlen(d_pitch_buf), 100);
		}
		if(radio->D_ROLL_IS_OK == true){
			snprintf(d_roll_buf, sizeof(d_roll_buf), "d_roll:%f ", rocket->angle_velocity->d_roll);
			HAL_UART_Transmit(&huart1, (uint8_t*)d_roll_buf, strlen(d_roll_buf), 100);
		}
		if(radio->D_YAW_IS_OK == true){
			snprintf(d_yaw_buf, sizeof(d_yaw_buf), "d_yaw:%f ", rocket->angle_velocity->d_yaw);
			HAL_UART_Transmit(&huart1, (uint8_t*)d_yaw_buf, strlen(d_yaw_buf), 100);
		}

		if(radio->TEMPERATURE_IS_OK == true){
			snprintf(temperature_buf, sizeof(temperature_buf), "temperature:%f ", rocket->atmosphere->temperature);
			HAL_UART_Transmit(&huart1, (uint8_t*)temperature_buf, strlen(temperature_buf), 100);
		}
		if(radio->BATTERY_VOLTAGE_IS_OK == true){
			snprintf(battery_voltage_buf, sizeof(battery_voltage_buf), "battery_voltage:%f ", rocket->battery_voltage);
			HAL_UART_Transmit(&huart1, (uint8_t*)battery_voltage_buf, strlen(battery_voltage_buf), 100);
		}
		HAL_UART_Transmit(&huart1, (uint8_t*)";\n", strlen(";\n"), 100);
	}
}
