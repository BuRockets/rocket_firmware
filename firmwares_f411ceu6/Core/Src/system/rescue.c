#include "system/rescue.h"

uint16_t degrees_to_pulse(int16_t degrees){
  return 11.1*degrees + 1500;
}

void turn_servo(int16_t degrees){
	__HAL_GPIO_EXTI_CLEAR_IT(btn_Pin);  // очищаем бит EXTI_PR
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn); // очищаем бит NVIC_ICPRx

	TIM1->CCR1 = degrees_to_pulse(degrees);
	HAL_Delay(500);
	TIM1->CCR1 = degrees_to_pulse(0);

	//HAL_UART_Transmit(&huart1, (uint8_t*)"servo is turn\n", sizeof("servo is turn\n"), 200);

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);   // включаем внешнее прерывание

	flag_irq = 0;
}

void rescue_system_init(TIM_TypeDef* TIM){
	TIM->CCR2 = degrees_to_pulse(0);
}

