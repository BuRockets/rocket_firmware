#include "flash.h"


// Функция проверки наличия корректных данных во Flash
int CheckFlashData() {
    uint32_t address = FLASH_USER_START_ADDR;

    // Проверяем контрольную сумму или признак валидности данных
    uint32_t checksum = *(uint32_t*)address; // Предположим, что первый байт - это контрольная сумма
    uint32_t calculated_checksum = MAGIC_CHECKSUM_NUMBER; // Ваша логика для расчета контрольной суммы

    // Пример: просто суммируем все поля структуры
    /*for (int i = 0; i < sizeof(Rocket); i++) {
        calculated_checksum += *((uint8_t*)&rocket + i);
    }*/
    //calculated_checksum += rocket.delta_apogee;
    //calculated_checksum += rocket.delta_activate;
    //calculated_checksum += rocket.starting_height;

    return (checksum == calculated_checksum); // Возвращаем true, если данные корректны
}

// Функция записи структуры в Flash
HAL_StatusTypeDef WriteToFlash(struct Rocket *rocket, struct Radio* radio) {

    HAL_FLASH_Unlock(); // Разблокировка Flash

    uint32_t address = FLASH_USER_START_ADDR;

    // Удаление страниц перед записью
    FLASH_EraseInitTypeDef FlashErase;                     // структура для функции стирания страницы
	uint32_t pageError = 0;                                // переменная для записи информации об ошибках в процессе стирания

	__disable_irq();                                       // запрещаем прерывания
	//HAL_FLASH_Unlock();
	FlashErase.TypeErase = FLASH_TYPEERASE_SECTORS;          // стирать постранично
	FlashErase.NbSectors = 1;         // считаем количество страниц, чтобы наш массив поместился
	FlashErase.Sector = FLASH_SECTOR_7;
	FlashErase.VoltageRange = VOLTAGE_RANGE_3;
	if (HAL_FLASHEx_Erase(&FlashErase, &pageError) != HAL_OK)   // вызов функции стирания
	{
		HAL_FLASH_Lock();                                  // если не смог стереть, то закрыть память и вернуть ошибку
		return HAL_ERROR;
	}

    // Запись данных в Flash


    uint32_t checksum = MAGIC_CHECKSUM_NUMBER; // Для контрольной суммы
    //checksum += rocket->delta_apogee;
    //checksum += rocket->delta_activate;
    //checksum += rocket->starting_height;


    // Запись контрольной суммы в начало области Flash
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, checksum) != HAL_OK) {
        HAL_FLASH_Lock();
        return HAL_ERROR; // Ошибка записи
    }
    address += sizeof(uint32_t);

    // Запись структуры в Flash*((uint32_t*)&value)
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, rocket->delta_apogee) != HAL_OK) {
        HAL_FLASH_Lock();
        return HAL_ERROR; // Ошибка записи
    }
    address += sizeof(uint32_t);

    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, rocket->delta_activate) != HAL_OK) {
        HAL_FLASH_Lock();
        return HAL_ERROR; // Ошибка записи
    }
    address += sizeof(uint32_t);

    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, rocket->starting_height) != HAL_OK) {
            HAL_FLASH_Lock();
            return HAL_ERROR; // Ошибка записи
    }
    address += sizeof(uint32_t);

    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, radio->pid_zeros->out1) != HAL_OK) {
            HAL_FLASH_Lock();
            return HAL_ERROR; // Ошибка записи
    }
    address += sizeof(uint32_t);

    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, radio->pid_zeros->out2) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
	address += sizeof(uint32_t);

	if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, radio->pid_zeros->out3) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
	address += sizeof(uint32_t);

	if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, radio->pid_zeros->out4) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
	address += sizeof(uint32_t);

	if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, rocket->PWM_out_mode) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
	address += sizeof(uint32_t);

	uint32_t float_as_uint_Kp;
    memcpy(&float_as_uint_Kp, &(radio->Kp), 4);  // Копируем побайтово
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, float_as_uint_Kp) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
    address += sizeof(float);

	uint32_t float_as_uint_Ki;
    memcpy(&float_as_uint_Ki, &(radio->Ki), 4);  // Копируем побайтово
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, float_as_uint_Ki) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
    address += sizeof(float);

	uint32_t float_as_uint_Kd;
    memcpy(&float_as_uint_Kd, &(radio->Kd), 4);  // Копируем побайтово
    if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, float_as_uint_Kd) != HAL_OK) {
			HAL_FLASH_Lock();
			return HAL_ERROR; // Ошибка записи
	}
    //address += sizeof(float);

    HAL_FLASH_Lock(); // Блокировка Flash
    __enable_irq();                                        // включаем прерывания обратно
    return HAL_OK; // Успешно
}

// Функция чтения структуры из Flash
void ReadFromFlash(struct Rocket *rocket, struct Radio* radio, struct PID_zeros* pid_zeros) {
    uint32_t address = FLASH_USER_START_ADDR + 4; // Пропускаем контрольную сумму

    if (address % 4 != 0) {
            // Обработка ошибки: адрес не выровнен
            return;
        }

    rocket->delta_apogee = *(__IO uint32_t*)address; // Чтение первого поля
    address += 4;

    rocket->delta_activate = *(__IO uint32_t*)address; // Чтение второго поля
    address += 4;

    rocket->starting_height = *(__IO uint32_t*)address; // Чтение второго поля
    address += 4;

    pid_zeros->out1= *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

    pid_zeros->out2= *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

    pid_zeros->out3= *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

    pid_zeros->out4= *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

	radio->pid_zeros = pid_zeros;

	rocket->PWM_out_mode= *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

    uint32_t uint_to_float_Kp = *(__IO uint32_t*)address; // Чтение второго поля;
    memcpy(&(radio->Kp),&uint_to_float_Kp,4);
    //radio->Kp	 = *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

	uint32_t uint_to_float_Ki = *(__IO uint32_t*)address; // Чтение второго поля;
	memcpy(&(radio->Ki),&uint_to_float_Ki,4);
	//radio->Ki = *(__IO uint32_t*)address; // Чтение второго поля
	address += 4;

	uint32_t uint_to_float_Kd = *(__IO uint32_t*)address; // Чтение второго поля;
	memcpy(&(radio->Kd),&uint_to_float_Kd,4);
	//radio->Kd = *(__IO uint32_t*)address; // Чтение второго поля
	//address += 4;

    //return HAL_OK; // Успешно
}
//HAL_StatusTypeDef
