################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/system/altitude.c \
../Core/Src/system/inertia.c \
../Core/Src/system/power.c \
../Core/Src/system/radio.c \
../Core/Src/system/rescue.c \
../Core/Src/system/rocket.c 

OBJS += \
./Core/Src/system/altitude.o \
./Core/Src/system/inertia.o \
./Core/Src/system/power.o \
./Core/Src/system/radio.o \
./Core/Src/system/rescue.o \
./Core/Src/system/rocket.o 

C_DEPS += \
./Core/Src/system/altitude.d \
./Core/Src/system/inertia.d \
./Core/Src/system/power.d \
./Core/Src/system/radio.d \
./Core/Src/system/rescue.d \
./Core/Src/system/rocket.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/system/%.o Core/Src/system/%.su Core/Src/system/%.cyclo: ../Core/Src/system/%.c Core/Src/system/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-system

clean-Core-2f-Src-2f-system:
	-$(RM) ./Core/Src/system/altitude.cyclo ./Core/Src/system/altitude.d ./Core/Src/system/altitude.o ./Core/Src/system/altitude.su ./Core/Src/system/inertia.cyclo ./Core/Src/system/inertia.d ./Core/Src/system/inertia.o ./Core/Src/system/inertia.su ./Core/Src/system/power.cyclo ./Core/Src/system/power.d ./Core/Src/system/power.o ./Core/Src/system/power.su ./Core/Src/system/radio.cyclo ./Core/Src/system/radio.d ./Core/Src/system/radio.o ./Core/Src/system/radio.su ./Core/Src/system/rescue.cyclo ./Core/Src/system/rescue.d ./Core/Src/system/rescue.o ./Core/Src/system/rescue.su ./Core/Src/system/rocket.cyclo ./Core/Src/system/rocket.d ./Core/Src/system/rocket.o ./Core/Src/system/rocket.su

.PHONY: clean-Core-2f-Src-2f-system

