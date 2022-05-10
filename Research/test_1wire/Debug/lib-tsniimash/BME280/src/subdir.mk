################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/BME280/src/DriverForBME280.c \
D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/BME280/src/bme280.c 

OBJS += \
./lib-tsniimash/BME280/src/DriverForBME280.o \
./lib-tsniimash/BME280/src/bme280.o 

C_DEPS += \
./lib-tsniimash/BME280/src/DriverForBME280.d \
./lib-tsniimash/BME280/src/bme280.d 


# Each subdirectory must supply rules for building sources it contributes
lib-tsniimash/BME280/src/DriverForBME280.o: D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/BME280/src/DriverForBME280.c lib-tsniimash/BME280/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
lib-tsniimash/BME280/src/bme280.o: D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/BME280/src/bme280.c lib-tsniimash/BME280/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/CanSat/Git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2d-tsniimash-2f-BME280-2f-src

clean-lib-2d-tsniimash-2f-BME280-2f-src:
	-$(RM) ./lib-tsniimash/BME280/src/DriverForBME280.d ./lib-tsniimash/BME280/src/DriverForBME280.o ./lib-tsniimash/BME280/src/bme280.d ./lib-tsniimash/BME280/src/bme280.o

.PHONY: clean-lib-2d-tsniimash-2f-BME280-2f-src

