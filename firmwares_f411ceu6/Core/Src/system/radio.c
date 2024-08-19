#include "system/radio.h"


void radio_init(struct Radio* radio){
	radio->TRANSMIT_IS_OK = false;
	radio->frequency_data_transmission = 1;
}

void set_data_transmit_frequency(TIM_HandleTypeDef *htim, struct Radio* radio){
	htim->Instance->ARR = 8*powf(10,6) / radio->frequency_data_transmission/ htim->Instance->PSC;
}

void transmit_data(struct Rocket* rocket, struct Radio* radio){
	char buf[100];

	snprintf(buf, sizeof(buf), "%s %f %f %f %f %d %d %d %d;\n", rocket->teamId, rocket->time, rocket->battery_voltage, rocket->altitude->altitude_f, rocket->accelerate->accel_mod, rocket->start_point, rocket->apogee_point, rocket->activate_point, rocket->landing_point);

	//sprintf(buf, "$%f %f %f %f;", rocket->atmosphere.pressure, rocket->accelerate->destination_a[0], rocket->accelerate->destination_a[1], rocket->accelerate->destination_a[2]);
	if(radio->TRANSMIT_IS_OK == true){
		HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
	}
}
