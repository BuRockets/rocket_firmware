#ifndef __SD_H__
#define __SD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void SD_init(FATFS FatFs, FIL fil, FRESULT fres, BYTE* readBuf);

void SD_write(FIL fil, FRESULT fres, char* writeBuf);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

