#include "ff.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg functions
#include "sd.h"
#include "main.h"

void SD_init(){
	//Open the file system
    fres = f_mount(&FatFs, "", 1); //1=mount now
  	while(fres != FR_OK){
  		fres = f_mount(&FatFs, "", 1); //1=mount now
  		myprintf("f_mount error (%i)\r\n", fres);
  		HAL_Delay(100);
    }

    //Now let's try and write a file "write.txt"
    fres = f_open(&fil, "write.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if(fres == FR_OK) {
  	myprintf("I was able to open 'write.txt' for writing\r\n");
    } else {
  	myprintf("f_open error (%i)\r\n", fres);
    }

    //Copy in a string
    snprintf((char*)readBuf, strlen((char*)readBuf), "Start work with SD card!\r\n");

    fres = f_write(&fil, readBuf, strlen((char*)readBuf), &bytesWrote);
    if(fres != FR_OK) {
    	myprintf("f_write error (%i)\r\n");
    }

    //Be a tidy kiwi - don't forget to close your file!
    f_close(&fil);
}

//FATFS FatFs; 	//Fatfs handle
//FIL fil; 		//File handle
//FRESULT fres; //Result after operations
//BYTE readBuf[30];

void SD_write(){
	// Открываем файл для дописывания
	fres = f_open(&fil, "write.txt", FA_WRITE | FA_OPEN_APPEND);
	if (fres != FR_OK) {
	  myprintf("f_open error (%i)\r\n", fres);
	}

	// Формируем строку
	//snprintf((char*)writeBuf, sizeof(writeBuf), "File number %d is made!\r\n", i);

	// Записываем только актуальные данные (без лишних нулей)

	fres = f_write(&fil, writeBuf, strlen((char *)writeBuf), &bytesWrote);
	if (fres != FR_OK) {
	  myprintf("f_write error (%i)\r\n", fres);
	}

	// Сброс буферов и закрытие
	f_close(&fil);
}
