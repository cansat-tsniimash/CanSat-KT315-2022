################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/nRF24L01_PL/src/nrf24_lower_api_stm32.c \
D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/nRF24L01_PL/src/nrf24_upper_api.c 

OBJS += \
./lib-tsniimash/nRF24L01_PL/src/nrf24_lower_api_stm32.o \
./lib-tsniimash/nRF24L01_PL/src/nrf24_upper_api.o 

C_DEPS += \
./lib-tsniimash/nRF24L01_PL/src/nrf24_lower_api_stm32.d \
./lib-tsniimash/nRF24L01_PL/src/nrf24_upper_api.d 


# Each subdirectory must supply rules for building sources it contributes
lib-tsniimash/nRF24L01_PL/src/nrf24_lower_api_stm32.o: D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/nRF24L01_PL/src/nrf24_lower_api_stm32.c lib-tsniimash/nRF24L01_PL/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
lib-tsniimash/nRF24L01_PL/src/nrf24_upper_api.o: D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/nRF24L01_PL/src/nrf24_upper_api.c lib-tsniimash/nRF24L01_PL/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2d-tsniimash-2f-nRF24L01_PL-2f-src

clean-lib-2d-tsniimash-2f-nRF24L01_PL-2f-src:
	-$(RM) ./lib-tsniimash/nRF24L01_PL/src/nrf24_lower_api_stm32.d ./lib-tsniimash/nRF24L01_PL/src/nrf24_lower_api_stm32.o ./lib-tsniimash/nRF24L01_PL/src/nrf24_upper_api.d ./lib-tsniimash/nRF24L01_PL/src/nrf24_upper_api.o

.PHONY: clean-lib-2d-tsniimash-2f-nRF24L01_PL-2f-src

