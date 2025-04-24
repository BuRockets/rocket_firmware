################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/math/angle.c \
../Core/Src/math/filter.c \
../Core/Src/math/pid.c 

OBJS += \
./Core/Src/math/angle.o \
./Core/Src/math/filter.o \
./Core/Src/math/pid.o 

C_DEPS += \
./Core/Src/math/angle.d \
./Core/Src/math/filter.d \
./Core/Src/math/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/math/%.o Core/Src/math/%.su Core/Src/math/%.cyclo: ../Core/Src/math/%.c Core/Src/math/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-math

clean-Core-2f-Src-2f-math:
	-$(RM) ./Core/Src/math/angle.cyclo ./Core/Src/math/angle.d ./Core/Src/math/angle.o ./Core/Src/math/angle.su ./Core/Src/math/filter.cyclo ./Core/Src/math/filter.d ./Core/Src/math/filter.o ./Core/Src/math/filter.su ./Core/Src/math/pid.cyclo ./Core/Src/math/pid.d ./Core/Src/math/pid.o ./Core/Src/math/pid.su

.PHONY: clean-Core-2f-Src-2f-math

