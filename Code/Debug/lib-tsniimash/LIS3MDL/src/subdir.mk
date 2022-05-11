################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/KT315-git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/LIS3MDL/src/DLIS3.c \
D:/KT315-git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/LIS3MDL/src/lis3mdl_reg.c 

OBJS += \
./lib-tsniimash/LIS3MDL/src/DLIS3.o \
./lib-tsniimash/LIS3MDL/src/lis3mdl_reg.o 

C_DEPS += \
./lib-tsniimash/LIS3MDL/src/DLIS3.d \
./lib-tsniimash/LIS3MDL/src/lis3mdl_reg.d 


# Each subdirectory must supply rules for building sources it contributes
lib-tsniimash/LIS3MDL/src/DLIS3.o: D:/KT315-git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/LIS3MDL/src/DLIS3.c lib-tsniimash/LIS3MDL/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/KT315-git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -I../Application/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
lib-tsniimash/LIS3MDL/src/lis3mdl_reg.o: D:/KT315-git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4/LIS3MDL/src/lis3mdl_reg.c lib-tsniimash/LIS3MDL/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/KT315-git/CanSat-KT315-2022/Common/lib-tsniimash/stm32f4" -I../Application/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2d-tsniimash-2f-LIS3MDL-2f-src

clean-lib-2d-tsniimash-2f-LIS3MDL-2f-src:
	-$(RM) ./lib-tsniimash/LIS3MDL/src/DLIS3.d ./lib-tsniimash/LIS3MDL/src/DLIS3.o ./lib-tsniimash/LIS3MDL/src/lis3mdl_reg.d ./lib-tsniimash/LIS3MDL/src/lis3mdl_reg.o

.PHONY: clean-lib-2d-tsniimash-2f-LIS3MDL-2f-src

