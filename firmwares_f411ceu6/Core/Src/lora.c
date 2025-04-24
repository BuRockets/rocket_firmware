#include "usart.h"
#include "gpio.h"
#include "lora.h"
#include <string.h>
#include <stdbool.h>
#include "main.h"


void LoRa_SetMode(uint8_t m0, uint8_t m1) {
    HAL_GPIO_WritePin(LORA_M0_PORT, LORA_M0_PIN, m0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LORA_M1_PORT, LORA_M1_PIN, m1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LoRa_WaitAUX(void) {
    while (HAL_GPIO_ReadPin(LORA_AUX_PORT, LORA_AUX_PIN) == GPIO_PIN_RESET) {
        HAL_Delay(10);  // Ждем, пока AUX станет HIGH (модуль готов)
    }
}

void LoRa_Init(void) {
    LoRa_SetMode(0, 0);  // Normal Mode
    LoRa_WaitAUX();      // Ждем готовности модуля
}

void LoRa_SendData(uint8_t* data, uint16_t size) {
    LoRa_WaitAUX();  // Убедимся, что модуль не занят
    
    // Отправляем данные через UART4
    HAL_UART_Transmit(&huart1, data, size, 100);
    
    // Можно добавить проверку AUX после отправки
    LoRa_WaitAUX();
}

uint8_t LoRa_is_Ready() {
    return HAL_GPIO_ReadPin(AUX_GPIO_Port, AUX_Pin) == GPIO_PIN_SET;
}

void Process_LoRa_Data(void) {
    if (lora_rx_ready && LoRa_is_Ready()) {
        receive_data(rx_buffer, &rocket, &radio);
        lora_rx_ready = 0;  // Сбрасываем флаг
    }
        // Перезапускаем приём (если модуль готов)
        if (LoRa_is_Ready()) {
            HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)&rx_buffer, 100);
        }

}
