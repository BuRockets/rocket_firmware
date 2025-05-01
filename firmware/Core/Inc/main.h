/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "system/inertia.h"
#include "system/altitude.h"
#include "system/radio.h"
#include "system/rescue.h"
#include "system/rocket.h"
#include "system/power.h"
#include "math/filter.h"
#include "math/angle.h"
#include "math/pid.h"
#include "callback.h"
#include "flash.h"
#include <math.h>
#include <stdio.h>
#include "spi.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern I2C_HandleTypeDef hi2c2;
#define hi2c hi2c2
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t flag_irq;
extern struct Rocket rocket;

extern struct GMedian gmedian_a[3];
extern struct GMedian gmedian_g[3];

extern struct Radio radio;

extern struct Angle angle;

extern struct Angle_velocity angle_velocity;

extern struct PID pid;

extern char rx_buffer[100];

extern bool PID_WORK;

void myprintf(const char *fmt, ...);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_CS_Pin GPIO_PIN_13
#define SD_CS_GPIO_Port GPIOB
#define btn_Pin GPIO_PIN_14
#define btn_GPIO_Port GPIOB
#define led_Pin GPIO_PIN_12
#define led_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE hspi1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
