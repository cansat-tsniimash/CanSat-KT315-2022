################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/Photorezistor/src/photorezistor.c 

OBJS += \
./lib-tsniimash/Photorezistor/src/photorezistor.o 

C_DEPS += \
./lib-tsniimash/Photorezistor/src/photorezistor.d 


# Each subdirectory must supply rules for building sources it contributes
lib-tsniimash/Photorezistor/src/photorezistor.o: D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/Photorezistor/src/photorezistor.c lib-tsniimash/Photorezistor/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2d-tsniimash-2f-Photorezistor-2f-src

clean-lib-2d-tsniimash-2f-Photorezistor-2f-src:
	-$(RM) ./lib-tsniimash/Photorezistor/src/photorezistor.d ./lib-tsniimash/Photorezistor/src/photorezistor.o

.PHONY: clean-lib-2d-tsniimash-2f-Photorezistor-2f-src

