#include "system/power.h"

uint16_t adcData = 0;


float get_mcu_voltage(){
	//HAL_ADC_PollForConversion(&hadc1, 100); // ожидаем окончания преобразования
	adcData = HAL_ADC_GetValue(&hadc1); // читаем полученное значение в переменную adc
	return ADC_MAX * ADC_REFERENCE_VOLTAGE / adcData;
}
