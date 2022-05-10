################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/DWT_Delay/src/dwt_delay.c 

OBJS += \
./lib-tsniimash/DWT_Delay/src/dwt_delay.o 

C_DEPS += \
./lib-tsniimash/DWT_Delay/src/dwt_delay.d 


# Each subdirectory must supply rules for building sources it contributes
lib-tsniimash/DWT_Delay/src/dwt_delay.o: D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/DWT_Delay/src/dwt_delay.c lib-tsniimash/DWT_Delay/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2d-tsniimash-2f-DWT_Delay-2f-src

clean-lib-2d-tsniimash-2f-DWT_Delay-2f-src:
	-$(RM) ./lib-tsniimash/DWT_Delay/src/dwt_delay.d ./lib-tsniimash/DWT_Delay/src/dwt_delay.o

.PHONY: clean-lib-2d-tsniimash-2f-DWT_Delay-2f-src

