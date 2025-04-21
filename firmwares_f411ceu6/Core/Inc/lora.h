#ifndef __LORA_H__
#define __LORA_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include "main.h"
/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Private defines */


#define    LORA_M0_PORT                         M0_GPIO_Port
#define    LORA_M0_PIN                                M0_Pin

#define    LORA_M1_PORT                         M1_GPIO_Port
#define    LORA_M1_PIN                                M1_Pin

#define    LORA_AUX_PORT                         AUX_GPIO_Port
#define    LORA_AUX_PIN                            AUX_Pin

/* USER CODE END Private defines */

void LoRa_SetMode(uint8_t m0, uint8_t m1);

void LoRa_WaitAUX(void);

void LoRa_Init(void);

void LoRa_SendData(uint8_t *data, uint16_t size);


#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
