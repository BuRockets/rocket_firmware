#ifndef __LORA_H__
#define __LORA_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include "main.h"
/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define    LORA_PORT                                            huart1
#define    LORA_TX_COMPL                                    0x00000000U
#define    LORA_PIN_M0_PORT                         GPIOD
#define    LORA_PIN_M1_PORT                         GPIOD
#define    LORA_PIN_AUX_PORT                         GPIOC
#define    LORA_M0_PIN                                Lora_M0_Pin
#define    LORA_M1_PIN                                Lora_M1_Pin
#define    LORA_AUX_PIN                            Lora_AUX_Pin
#define    LORA_PORT_SPEED_1200                    0
#define    LORA_PORT_SPEED_2400                    1
#define    LORA_PORT_SPEED_4800                    2
#define    LORA_PORT_SPEED_9600                    3
#define    LORA_PORT_SPEED_19200                    4
#define    LORA_PORT_SPEED_38400                    5
#define    LORA_PORT_SPEED_57800                    6
#define    LORA_PORT_SPEED_115200                    7
#define    LORA_AIR_SPEED_0_3                        0
#define    LORA_AIR_SPEED_1200                        1
#define    LORA_AIR_SPEED_2400                        2
#define    LORA_AIR_SPEED_4800                        3
#define    LORA_AIR_SPEED_9600                        4
#define    LORA_AIR_SPEED_19200                    5
#define    LORA_TX_POWER_20dbm                        0
#define    LORA_TX_POWER_17dbm                        1
#define    LORA_TX_POWER_14dbm                        2
#define    LORA_TX_POWER_10dbm                        3
#define    LORA_CHANNEL                                    0
#define    LORA_ADDRESS                                    23
#define    LORA_BC_MESSAGE_ADDR                    0xFFFF
#define    LORA_END_OF_MESSAGE_STRING        "\r\n"
#define    LORA_MODE_NORMAL                            0
#define    LORA_MODE_WAKEUP                            1
#define    LORA_MODE_POWER_SAVING                2
#define    LORA_MODE_SLEEP                                3
#define    LORA_MODE_NOT_IDENT                        4
#define    LORA_PACKET_SIZE                            50
#define    LORA_SIZE_BUFFER                            56
/* USER CODE END Private defines */
typedef struct {
//UART_HandleTypeDef LoraPort;
	uint32_t LoraPortSpeed;
	//GPIO_TypeDef * LoraPinPort;
	uint16_t LoraM0Pin;
	uint16_t LoraM1Pin;
	uint16_t LoraAuxPin;
	uint16_t LoraChannel;
	uint16_t LoraSelfAddr;
	uint16_t LoraAirSpeed;
	uint16_t LoraTxPower;
	uint8_t LoraTxBuff[LORA_SIZE_BUFFER];
	uint8_t LoraRxBuff[LORA_SIZE_BUFFER];
	uint8_t LoraRxByte;
	volatile uint16_t LoraRxIndex;
} lora_t;
/* USER CODE BEGIN PV */
/* USER CODE END PV */
/* USER CODE BEGIN Prototypes */
void Lora_configure(lora_t Lora);
bool Lora_Init(lora_t lorastr);
bool Lora_TX_ReadyStatus(lora_t lorastr);
uint16_t Lora_SetMode(lora_t lorastr, uint16_t Mode);
uint16_t Lora_GetMode(lora_t lorastr);
bool Lora_Message_TX(lora_t* lorastr, uint8_t *pData, uint16_t size, uint8_t channel, uint16_t addr, uint8_t ack, uint16_t timeout);
void Lora_Message_RX(lora_t lorastr);
void Lora_Get_Message_RX(lora_t lorastr, uint8_t *pData);
uint16_t Lora_RxNbrBytes(lora_t lorastr);
void Lora_Flush_Buf(uint8_t* Buffer);
bool Lora_Reset(lora_t lorastr);
bool Lora_RxBuf_String(lora_t lorastr, uint8_t* str);
bool Lora_RxBuf_Bytes(lora_t lorastr, uint8_t* bytes, uint8_t sizeofbytes);
uint8_t MyStsrStsr(lora_t lorastr, uint8_t* bytes, uint8_t sizeofbytes);
bool Lora_GetOptions(lora_t lorastr);

#ifdef __cplusplus
}
#endif

#endif /* __LORA_H__ */
