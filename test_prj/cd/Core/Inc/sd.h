/*
 * sd.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Bunakov Egor
 */

#ifndef INC_SD_H_
#define INC_SD_H_

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define CS_SD_GPIO_PORT CD_CS_GPIO_Port

#define CS_SD_PIN CD_CS_Pin

#define SS_SD_SELECT() HAL_GPIO_WritePin(CS_SD_GPIO_PORT, CS_SD_PIN, GPIO_PIN_RESET)

#define SS_SD_DESELECT() HAL_GPIO_WritePin(CS_SD_GPIO_PORT, CS_SD_PIN, GPIO_PIN_SET)

#define LD_ON HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); //RED

#define LD_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); //RED

//--------------------------------------------------

/* Card type flags (CardType) */

#define CT_MMC 0x01 /* MMC ver 3 */

#define CT_SD1 0x02 /* SD ver 1 */

#define CT_SD2 0x04 /* SD ver 2 */

#define CT_SDC (CT_SD1|CT_SD2) /* SD */

#define CT_BLOCK 0x08 /* Block addressing */

//--------------------------------------------------

typedef struct sd_info {

  volatile uint8_t type;//тип карты

} sd_info_ptr;

//--------------------------------------------------

#endif /* INC_SD_H_ */
