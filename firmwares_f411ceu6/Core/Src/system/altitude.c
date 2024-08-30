#include "system/altitude.h"

uint16_t size;
uint8_t Data[256];


BMP280_HandleTypedef bmp280;

void altitude_init(struct Rocket* rocket){

	if (rocket->altitude == NULL) {
		// Обработка ошибки: указатель не инициализирован
		return;
	}

	rocket->altitude->altitude = 0;

	rocket->altitude->altitude_f = 0;

	rocket->altitude->max_altitude = 0;

	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0;
	bmp280.i2c = &hi2c1;

	while (!bmp280_init(&bmp280, &bmp280.params)) {
		size = sprintf((char *)Data, "BMP280 initialization failed\n");
		HAL_UART_Transmit(&huart1, Data, size, 1000);
		HAL_Delay(2000);
	}
	bool bme280p = bmp280.id == BME280_CHIP_ID;
	size = sprintf((char *)Data, "BMP280: found %s\n", bme280p ? "BME280" : "BMP280");
	HAL_UART_Transmit(&huart1, Data, size, 1000);
	for(int i = 0; i < 100; i++){
		bmp280_read_float(&bmp280, &(rocket->atmosphere->start_temperature), &(rocket->atmosphere->start_pressure), &(rocket->atmosphere->start_humidity));
	}

}



void get_altitude_measurement(struct Rocket* rocket){
	  bmp280_read_float(&bmp280, &(rocket->atmosphere->temperature), &(rocket->atmosphere->pressure), &(rocket->atmosphere->humidity));
	  rocket->altitude->altitude = pascal_to_metrs(rocket->atmosphere->pressure,rocket->atmosphere->start_pressure);
	  //rocket->altitude->altitude_f = average_filter(rocket->altitude->altitude, rocket->altitude->altitude_f, k_alt);
}

void filtered_altitude_measurement(struct Rocket* rocket, struct GMedian* gmedian_alt){
	gmedian_alt->destination_f_m = filtered(gmedian_alt, rocket->altitude->altitude);
	rocket->altitude->altitude_f = average_filter(gmedian_alt->destination_f_m,rocket->altitude->altitude_f, k_alt);

	//rocket->altitude->altitude_f = average_filter(rocket->altitude->altitude, rocket->altitude->altitude_f, k_alt);

}
/*
bmp280_init_default_params(&bmp280.params);
bmp280.addr = BMP280_I2C_ADDRESS_0;
bmp280.i2c = &hi2c1;

while (!bmp280_init(&bmp280, &bmp280.params)) {
	size = sprintf((char *)Data, "BMP280 initialization failed\n");
	HAL_UART_Transmit(&huart1, Data, size, 1000);
	HAL_Delay(2000);
}
bool bme280p = bmp280.id == BME280_CHIP_ID;
size = sprintf((char *)Data, "BMP280: found %s\n", bme280p ? "BME280" : "BMP280");
HAL_UART_Transmit(&huart1, Data, size, 1000);
*/
