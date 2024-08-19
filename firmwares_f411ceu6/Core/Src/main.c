/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define btn_time 200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct Rocket rocket;
struct Atmosphere_param atmosphere;
struct Altitude altitude;
struct Accelerate accelerate;
struct Gyro gyro;
struct Radio radio;

struct GMedian gmedian_a[3];
struct GMedian gmedian_g[3];

struct GMedian gmedian_alt;

struct Angle angle;

struct Angle_velocity angle_velocity;

struct PID pid;

float Kp = 1;

float Ki = 0;

float Kd = 0;

char rx_buffer[100] = {0,};

uint8_t flag_irq = 0;
uint32_t time_irq = 0;

uint32_t time = 0;
//uint8_t counter = 0;
//float destination_a_f_m[3] = {0,};

bool PID_WORK = 0;

static float set_data[dimension_in] = {0,};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void start_blink_led(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t time){
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(time);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

}

float tick_to_sec(uint32_t tick){
	return tick/(float)1000;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  __HAL_RCC_I2C1_CLK_ENABLE();
    HAL_Delay(100);
    __HAL_RCC_I2C1_FORCE_RESET();
    HAL_Delay(100);
    __HAL_RCC_I2C1_RELEASE_RESET();
    HAL_Delay(100);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)&rx_buffer, 100);

    //HAL_ADCEx_Calibration_Start(&hadc1);

    HAL_ADC_Start(&hadc1);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);

    atmosphere_init(&atmosphere);

    rocket_init(&rocket, "1A", &altitude, &atmosphere, &accelerate, &gyro);

    if (CheckFlashData()) {
  	  ReadFromFlash(&rocket); // Чтение данных из Flash, если они корректны
    }
    else {
  	  delta_init(&rocket);
    }

    angle_init(&angle);

    angle_velocity_init(&angle_velocity);

    PID_init(&pid);

    set_PID_coefficients(&pid, Kp, Ki, Kd);

    altitude_init(&rocket);

    rescue_system_init(TIM1);

    initGMedian(&gmedian_alt);

    for(int i = 0; i < 3; i++){
  	  initGMedian(&(gmedian_a[i]));
  	  initGMedian(&(gmedian_g[i]));
    }

    radio_init(&radio);

    HAL_TIM_Base_Start_IT(&htim3);

    HAL_Delay(500);
    HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    start_blink_led(led_GPIO_Port, led_Pin, 200);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  get_inertia_measurement(&rocket);

	  filtered_inertia_measurement(&rocket,gmedian_a,gmedian_g);

	  get_inertia_measurement_mod(&rocket);

	  angle_calculate(&angle, rocket.accelerate);

	  angle_velocity_calculate(&angle_velocity, &gyro);

	  get_altitude_measurement(&rocket);

	  filtered_altitude_measurement(&rocket, &gmedian_alt);

	  if(flag_irq && (HAL_GetTick() - time_irq) > btn_time)
		  {
		  turn_servo(90);
		  }

	  fly_control(&rocket);

	  if(rocket.activate_point){
		 //turn_servo(90);
	  }
	  rocket.time = tick_to_sec(HAL_GetTick());
	  rocket.battery_voltage = get_mcu_voltage();

	  if(PID_WORK == true){
		  get_PID_out(&pid, &angle, &angle_velocity, set_data);
		  set_pwm(&pid);
		  PID_WORK = false;
	  }

	  if(HAL_GetTick() - time > Hz_to_ms(radio.frequency_data_transmission)){
		  time = HAL_GetTick();

		  transmit_data(&rocket, &radio);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == btn_Pin){
  	  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);  // сразу же отключаем прерывания на этом пине
  	  flag_irq = 1;							//поднимаем флажок
  	  time_irq = HAL_GetTick();				//запоминаем текущее время
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
