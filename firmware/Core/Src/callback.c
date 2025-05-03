#include "callback.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


uint8_t stringToUint8(const char *str) {
	char *endptr;
	long value;

	// Преобразуем строку в long
	value = strtol(str, &endptr, 10); // 10 - основание (десятичная система)

	// Проверяем, было ли преобразование успешным
	if (endptr == str) {
		// Строка не содержит цифр
		//fprintf(stderr, "Ошибка: строка не содержит чисел.\n");
		return 0; // или любое другое значение по умолчанию
	}

	// Проверяем, находится ли значение в диапазоне uint8_t
	if (value < 0 || value > 255) {
		//fprintf(stderr, "Ошибка: значение вне диапазона uint8_t.\n");
		return 0; // или любое другое значение по умолчанию
	}

	return (uint8_t)value; // Преобразуем long в uint8_t
}

uint16_t Hz_to_ms(uint16_t frequency){
	uint16_t ms = 1000/frequency;
	return ms;
}

void receive_data(char* rx_buffer, struct Rocket* rocket, struct Radio* radio){
	char buf[50];
	char *token = strtok(rx_buffer, ":");
	if (token != NULL) {
		char *var_name = token;
		token = strtok(NULL, ":");
		if (token != NULL) {
			float value = atof(token); // Преобразуем строку в float

			// Присваиваем значение переменной
			if (strcmp(var_name, "delta_apogee") == 0) {
				rocket->delta_apogee = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"delta_apogee = %u\n", rocket->delta_apogee);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write delta_apogee in flash",strlen("Error write delta_apogee in flash") , 200);
				}
			} else if (strcmp(var_name, "delta_activate") == 0) {
				rocket->delta_activate = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"delta_activate = %u\n", rocket->delta_activate);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write delta_activate in flash",strlen("Error write delta_activate in flash") , 200);
				}
			} else if (strcmp(var_name, "starting_height") == 0) {
				rocket->starting_height = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"starting_height = %u\n", rocket->starting_height);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write starting_height in flash",strlen("Error write starting_height in flash") , 200);
				}
			} else if (strcmp(var_name, "Kp") == 0) {
				radio->Kp = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"Kp = %f\n", radio->Kp);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write Kp in flash",strlen("Error write Kp in flash") , 200);
				}
			} else if (strcmp(var_name, "Ki") == 0) {
				radio->Ki = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"Ki = %f\n", radio->Ki);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write Ki in flash",strlen("Error write Ki in flash") , 200);
				}
			} else if (strcmp(var_name, "Kd") == 0) {
				radio->Kd = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"Kd = %f\n", radio->Kd);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write Kd in flash",strlen("Error Kd write in flash") , 200);
				}
			} else if (strcmp(var_name, "zero_out1") == 0) {
				radio->pid_zeros->out1 = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"zero_out1 = %ld\n", radio->pid_zeros->out1);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write zero_out1 in flash",strlen("Error write zero_out1 in flash") , 200);
				}
			} else if (strcmp(var_name, "zero_out2") == 0) {
				radio->pid_zeros->out2 = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"zero_out2 = %ld\n", radio->pid_zeros->out2);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write zero_out2 in flash",strlen("Error write zero_out2 in flash") , 200);
				}
			} else if (strcmp(var_name, "zero_out3") == 0) {
				radio->pid_zeros->out3 = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"zero_out3 = %ld\n", radio->pid_zeros->out3);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write zero_out3 in flash",strlen("Error write zero_out3 in flash") , 200);
				}
			} else if (strcmp(var_name, "zero_out4") == 0) {
				radio->pid_zeros->out4 = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"zero_out4 = %ld\n", radio->pid_zeros->out4);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write zero_out4 in flash",strlen("Error write zero_out4 in flash") , 200);
				}

			} else if (strcmp(var_name, "pwm_mode") == 0) {
				rocket->PWM_out_mode = value;
				if(WriteToFlash(rocket, radio) == HAL_OK){
					snprintf(buf,sizeof(buf),"pwm_mode = %lu\n", rocket->PWM_out_mode);
					HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
				} else {
					HAL_UART_Transmit(&huart1, (uint8_t*)"Error write pwm_mode in flash",strlen("Error write pwm_mode in flash") , 200);
				}
			} else if (strcmp(var_name, "reset_points") == 0) {
				reset_points(rocket);
				HAL_UART_Transmit(&huart1, (uint8_t*)"Reset all points",strlen("Reset all points") , 200);

			} else if (strcmp(var_name, "TRANSMIT_IS_OK") == 0) {
				radio->TRANSMIT_IS_OK = value;
			}
			else if (strcmp(var_name, "frequency_data_transmission") == 0) {
				radio->frequency_data_transmission = value;
				//set_data_transmit_frequency(&htim2, radio); //потребуется при включении TIM2 для отправки телеметрии
			} else if (strcmp(var_name, "ALTITUDE_IS_OK") == 0) {
				radio->ALTITUDE_IS_OK = value;
			} else if (strcmp(var_name, "PITCH_IS_OK") == 0) {
				radio->PITCH_IS_OK = value;
			} else if (strcmp(var_name, "ROLL_IS_OK") == 0) {
				radio->ROLL_IS_OK = value;
			} else if (strcmp(var_name, "YAW_IS_OK") == 0) {
				radio->YAW_IS_OK = value;
			} else if (strcmp(var_name, "D_PITCH_IS_OK") == 0) {
				radio->D_PITCH_IS_OK = value;
			} else if (strcmp(var_name, "D_ROLL_IS_OK") == 0) {
				radio->D_ROLL_IS_OK = value;
			} else if (strcmp(var_name, "D_YAW_IS_OK") == 0) {
				radio->D_YAW_IS_OK = value;
			} else if (strcmp(var_name, "TEMPERATURE_IS_OK") == 0) {
				radio->TEMPERATURE_IS_OK = value;
			} else if (strcmp(var_name, "BATTERY_VOLTAGE_IS_OK") == 0) {
				radio->BATTERY_VOLTAGE_IS_OK = value;
			} else if (strcmp(var_name, "CONTROL_IS_OK") == 0) {
				radio->CNTRL_IS_OK = value;
			} else if (strcmp(var_name, "POINTS_IS_OK") == 0) {
				radio->POINTS_IS_OK = value;
			} else if (strcmp(var_name, "TEST_RESCUE_IS_OK") == 0) {
				radio->TEST_RESCUE_IS_OK = value;
			}

		} else {
			char *command = strtok(rx_buffer, ";");
			if (strcmp(command, "get_delta_apogee") == 0){
				snprintf(buf,sizeof(buf),"delta_apogee = %u\n", rocket->delta_apogee);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			else if (strcmp(command, "get_delta_activate") == 0){
				snprintf(buf,sizeof(buf),"delta_activate = %u\n", rocket->delta_activate);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			else if (strcmp(command, "get_starting_height") == 0){
				snprintf(buf,sizeof(buf),"starting_height = %u\n", rocket->starting_height);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			/*else if (strcmp(command, "get_Kp") == 0){
				char buf[30];
				snprintf(buf,sizeof(buf),"Kp = %f\n", radio->Kp);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			else if (strcmp(command, "get_Ki") == 0){
				char buf[30];
				snprintf(buf,sizeof(buf),"Ki = %f\n", radio->Ki);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			else if (strcmp(command, "get_Kd") == 0){
				char buf[30];
				snprintf(buf,sizeof(buf),"Kd = %f\n", radio->Kd);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}*/
			else if (strcmp(command, "get_pid_K") == 0){
				snprintf(buf,sizeof(buf),"Kp = %f : Ki = %f : Kd = %f\n", radio->Kp, radio->Ki, radio->Kd);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			else if (strcmp(command, "get_pwm_zeros") == 0){
				snprintf(buf,sizeof(buf),"zero1 = %ld : zero2 = %ld : zero3 = %ld : zero4 = %ld\n", radio->pid_zeros->out1,radio->pid_zeros->out2,radio->pid_zeros->out3,radio->pid_zeros->out4);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
			else if (strcmp(command, "get_pwm_mode") == 0){
				snprintf(buf,sizeof(buf),"pwm_mode = %lu\n", rocket->PWM_out_mode);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf,strlen(buf) , 200);
			}
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){

	receive_data(rx_buffer, &rocket, &radio);
	// Запускаем прием следующего байта
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)&rx_buffer, 100);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM4){
		PID_WORK = true;

		//get_PID_out(&pid, &angle, &angle_velocity, set_data);
	}

}
