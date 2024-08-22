# rocket_firmware
# Основные сведения #
- Основным микроконтроллером проекта является stm32f103c8t6
- Все прошивки написаны с использованием библиотеки HAL

# Команды посылаемые по uart для общения с ракетой #

## Установка значений ##
- ```delta_apogee:1;``` -- установить delta_apogee == 1
- ```delta_activate:2;``` -- установить delta_activate == 2
- ```starting_height:3;``` -- установить starting_height == 3
- ```TRANSMIT_IS_OK:1;``` -- разрешить присылать телеметрию
- ```frequency_data_transmission:5;``` -- установить частоту посылки данных равную 5 Гц

## Считывание значений ##

- ```get_delta_apogee;``` -- получить текущее значение  delta_apogee
- ```get_delta_activate;``` -- получить текущее значение  delta_activate
- ```get_starting_height;``` -- получить текущее значение  starting_height