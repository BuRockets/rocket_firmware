#include "usart.h"
#include "gpio.h"
#include "lora.h"
//#include "crc.h"
#include    <string.h>
//#include <stdbool.h>
/*************************************/


void Lora_configure(lora_t Lora){
	Lora.LoraPortSpeed = LORA_PORT_SPEED_9600;
	Lora.LoraChannel = LORA_CHANNEL;
	Lora.LoraSelfAddr = LORA_ADDRESS;
	Lora.LoraAirSpeed = LORA_AIR_SPEED_2400;
	Lora.LoraTxPower = LORA_TX_POWER_20dbm;
	Lora.LoraRxIndex = 0;
}
bool Lora_Init(lora_t lorastr){
	bool ret = true;
	uint32_t portspeed;


	Lora_Reset(lorastr);
	HAL_Delay(500);
	Lora_Flush_Buf(lorastr.LoraRxBuff);
	Lora_Flush_Buf(lorastr.LoraTxBuff);
    portspeed = LORA_PORT.Init.BaudRate;
    LORA_PORT.Init.BaudRate = 9600;
    ret = HAL_UART_Init(&LORA_PORT);
    if (ret != HAL_OK)
    {
        return ret;
    }
    lorastr.LoraTxBuff[0]= 0xC0;                                                                                     //first byte
    lorastr.LoraTxBuff[1]= (uint8_t)((lorastr.LoraSelfAddr & 0xFF00)>>8); //second byte - high address
    lorastr.LoraTxBuff[2]= (uint8_t)(lorastr.LoraSelfAddr & 0x00FF);             //3 byte - lowbyte address
    lorastr.LoraTxBuff[3]= (uint8_t) ((0x38&(lorastr.LoraPortSpeed<<3))|(0x07&lorastr.LoraAirSpeed));    // 4 byte - speed
    lorastr.LoraTxBuff[4]= (uint8_t) (0x1F&lorastr.LoraChannel);    // 5 byte - channel
    lorastr.LoraTxBuff[5]= (uint8_t) ((0xC4) | (0x03&lorastr.LoraTxPower));
    while (!HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {}
    Lora_SetMode(lorastr, LORA_MODE_SLEEP);
    HAL_Delay(4);
    ret = HAL_UART_Transmit(&LORA_PORT, lorastr.LoraTxBuff,6,100);
    if (ret != HAL_OK)
    {
        return ret;
    }
    HAL_Delay(50);
    Lora_Flush_Buf(lorastr.LoraTxBuff);
    LORA_PORT.Init.BaudRate = portspeed;
    ret = HAL_UART_Init(&(LORA_PORT));
    return ret;
}
uint8_t MyStsrStsr(lora_t lorastr, uint8_t* bytes, uint8_t sizeofbytes)
{
    for (int i = 0; i<sizeofbytes-1;i++)
      {
          if ((lorastr.LoraRxBuff[i] == bytes[1]) && (lorastr.LoraRxBuff[i - 1] == bytes[0]))
          {
              return i;
          }
      }
    return 0;
}
/******************************************/
bool Lora_RxBuf_Bytes(lora_t lorastr, uint8_t* bytes, uint8_t sizeofbytes)
{
  for (int i = 0; i<sizeofbytes-1;i++)
  {
      if ((lorastr.LoraRxBuff[i] == bytes[1]) && (lorastr.LoraRxBuff[i - 1] == bytes[0]))
      {
          return true;
      }
  }
  return false;
}
/****************************************/
bool Lora_Message_TX(lora_t* lorastr, uint8_t *pData, uint16_t size, uint8_t channel, uint16_t addr, uint8_t ack, uint16_t timeout){
    bool ret = true;
    if(Lora_GetMode(*lorastr) != LORA_MODE_NORMAL) {
    Lora_SetMode(*lorastr, LORA_MODE_NORMAL);
    }
    if((addr>0x1F)&&(addr !=LORA_BC_MESSAGE_ADDR)) {
    ret = false;
        return ret;
    }
    
    Lora_Flush_Buf(lorastr->LoraTxBuff);
    lorastr->LoraTxBuff[0] = (uint8_t) addr>>8;
    lorastr->LoraTxBuff[1] = (uint8_t) (addr & 0x00FF);
    lorastr->LoraTxBuff[2] = channel;
    if (size > LORA_PACKET_SIZE) { 
        ret = false;
        return ret;
    }
        for (int i = 0; i<size; i++) {
            lorastr->LoraTxBuff[i + 3] = pData[i];
        }
        
        strcat((char *)&lorastr->LoraTxBuff[size + 3], (char*) LORA_END_OF_MESSAGE_STRING);
        while (!Lora_TX_ReadyStatus(*lorastr)) {}
        ret = HAL_UART_Transmit(&LORA_PORT, lorastr->LoraTxBuff, size+3+strlen(LORA_END_OF_MESSAGE_STRING), timeout);
        while (!Lora_TX_ReadyStatus(*lorastr)) {}
            HAL_Delay(timeout);
        Lora_Flush_Buf(lorastr->LoraTxBuff);
        return true;
}
/***************************************/
void Lora_Message_RX(lora_t lorastr){
    HAL_UART_Receive_IT(&LORA_PORT, &lorastr.LoraRxByte, 1);
    lorastr.LoraRxIndex = 0;
}
void Lora_Get_Message_RX(lora_t lorastr, uint8_t *pData){
    uint32_t lenght;
    if(Lora_RxBuf_Bytes(lorastr, (uint8_t *) LORA_END_OF_MESSAGE_STRING,LORA_SIZE_BUFFER)){
    uint8_t ret;
    ret = MyStsrStsr(lorastr, (uint8_t *) LORA_END_OF_MESSAGE_STRING,LORA_SIZE_BUFFER);
        lenght = ret;
        for (int i=0; i<lenght; i++) {
            pData[i] = lorastr.LoraRxBuff[i];
        }            
    }
}
/***************************************/
bool Lora_Reset(lora_t lorastr) {
uint16_t status;
Lora_SetMode(lorastr, LORA_MODE_SLEEP);
Lora_Flush_Buf(lorastr.LoraTxBuff);
lorastr.LoraTxBuff[0] = 0xC4;
lorastr.LoraTxBuff[1] = 0xC4;
lorastr.LoraTxBuff[2] = 0xC4;
HAL_Delay(3);
status = HAL_UART_Transmit(&LORA_PORT, lorastr.LoraTxBuff,3,100);
HAL_Delay(100);
Lora_SetMode(lorastr, LORA_MODE_NORMAL);
if(status!= 0x00U) {return false;}
return true;
}
/*******************************************/
bool Lora_TX_ReadyStatus(lora_t lorastr){
    if (HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {return true;}
    return false;
}
/*******************************************/
bool Lora_RxBuf_String(lora_t lorastr, uint8_t* str){
    char *ret;
    ret = strstr((char *) lorastr.LoraRxBuff, (char *) str);
    if (ret != NULL) {
        return true;
    }
    return false;
}
/*******************************************/
uint16_t Lora_RxNbrBytes(lora_t lorastr) {
    return lorastr.LoraRxIndex;
}
/*******************************************/
void Lora_Flush_Buf(uint8_t* Buffer){
    for (uint16_t i=0; i<LORA_SIZE_BUFFER;i++){
        Buffer[i] = 0;
    }
}
/***********************************************/
uint16_t Lora_SetMode(lora_t lorastr, uint16_t Mode) {
    uint16_t ret;
    while(!HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {}
    switch (Mode) {
        case LORA_MODE_NORMAL:
            while (!HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {}
            HAL_GPIO_WritePin(LORA_PIN_M0_PORT, LORA_M0_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LORA_PIN_M1_PORT, LORA_M1_PIN, GPIO_PIN_RESET);
        break;        
        case LORA_MODE_WAKEUP:
            while (!HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {}
            HAL_GPIO_WritePin(LORA_PIN_M0_PORT, LORA_M0_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LORA_PIN_M1_PORT, LORA_M1_PIN, GPIO_PIN_RESET);
        break;
        case LORA_MODE_POWER_SAVING:
            while (!HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {}
            HAL_GPIO_WritePin(LORA_PIN_M0_PORT, LORA_M0_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LORA_PIN_M1_PORT, LORA_M1_PIN, GPIO_PIN_SET);
        break;
        case LORA_MODE_SLEEP:
            while (!HAL_GPIO_ReadPin(LORA_PIN_AUX_PORT, LORA_AUX_PIN)) {}
            HAL_GPIO_WritePin(LORA_PIN_M0_PORT, LORA_M0_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LORA_PIN_M1_PORT, LORA_M1_PIN, GPIO_PIN_SET);
        break;
       
    
}    
   HAL_Delay(10);
    ret = Mode;
    return ret;
}        
                         
/***************************************/
uint16_t Lora_GetMode(lora_t lorastr) {
    uint16_t mode;
    if((!(HAL_GPIO_ReadPin(LORA_PIN_M0_PORT, LORA_M0_PIN)))&&(!(HAL_GPIO_ReadPin(LORA_PIN_M1_PORT, LORA_M1_PIN)))){mode = LORA_MODE_NORMAL;}
        if((HAL_GPIO_ReadPin(LORA_PIN_M0_PORT, LORA_M0_PIN))&&(!(HAL_GPIO_ReadPin(LORA_PIN_M1_PORT, LORA_M1_PIN)))){mode = LORA_MODE_WAKEUP;}
            if((HAL_GPIO_ReadPin(LORA_PIN_M1_PORT, LORA_M1_PIN))&&(!(HAL_GPIO_ReadPin(LORA_PIN_M0_PORT, LORA_M0_PIN)))){mode = LORA_MODE_POWER_SAVING;}
                if((HAL_GPIO_ReadPin(LORA_PIN_M0_PORT,LORA_M0_PIN))&&(HAL_GPIO_ReadPin(LORA_PIN_M1_PORT, LORA_M1_PIN))){mode = LORA_MODE_SLEEP;}
    HAL_Delay(1);
                
    return mode;
}        
bool Lora_GetVersion(lora_t lorastr)
{
  uint16_t status;
  Lora_SetMode(lorastr, LORA_MODE_SLEEP);
  Lora_Flush_Buf(lorastr.LoraTxBuff);
  lorastr.LoraTxBuff[0] = 0xC3;
  lorastr.LoraTxBuff[1] = 0xC3;
  lorastr.LoraTxBuff[2] = 0xC3;
  status = HAL_UART_Transmit(&LORA_PORT, lorastr.LoraTxBuff,3,100);
  HAL_Delay(2);
  Lora_SetMode(lorastr, LORA_MODE_NORMAL);
  if(status!= 0x00U) {return false;}
  return true;
}
bool Lora_GetOptions(lora_t lorastr)
{
  uint16_t status;
  Lora_SetMode(lorastr, LORA_MODE_SLEEP);
  Lora_Flush_Buf(lorastr.LoraTxBuff);
  lorastr.LoraTxBuff[0] = 0xC1;
  lorastr.LoraTxBuff[1] = 0xC1;
  lorastr.LoraTxBuff[2] = 0xC1;
  status = HAL_UART_Transmit(&LORA_PORT, lorastr.LoraTxBuff,3,100);
  HAL_Delay(2);
  if (status == HAL_OK)
      return true;
  else  return false;
}
