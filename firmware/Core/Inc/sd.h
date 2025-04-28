#ifndef __SD_H__
#define __SD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


/* USER CODE BEGIN Includes */
#include "ff.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
extern FATFS FatFs; 	//Fatfs handle
extern FIL fil; 		//File handle
extern FRESULT fres; //Result after operations
extern BYTE readBuf[30];
extern BYTE writeBuf[30];
extern UINT bytesWrote;
/* USER CODE END Private defines */

void SD_init();

void SD_write();

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

