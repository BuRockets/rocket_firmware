/*
 * sd.c
 *
 *  Created on: Mar 6, 2025
 *      Author: Bunakov Egor
 */

#include "sd.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;

extern UART_HandleTypeDef huart3;

sd_info_ptr sdinfo;

void SD_PowerOn(void)
{
	HAL_Delay(20);
}

uint8_t sd_ini(void)
{
	return 0;
}

static void Error (void)

{
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}


uint8_t SPIx_WriteRead(uint8_t Byte)

{

  uint8_t receivedbyte = 0;

  if(HAL_SPI_TransmitReceive(&hspi2,(uint8_t*) &Byte,(uint8_t*) &receivedbyte,1,0x1000)!=HAL_OK)

  {

    Error();

  }

  return receivedbyte;

}

void SPI_SendByte(uint8_t bt)

{

  SPIx_WriteRead(bt);

}

uint8_t SPI_ReceiveByte(void)

{

  uint8_t bt = SPIx_WriteRead(0xFF);

  return bt;

}

void SPI_Release(void)

{

  SPIx_WriteRead(0xFF);

}
