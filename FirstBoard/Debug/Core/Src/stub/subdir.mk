################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/stub/crc_functions.c \
../Core/Src/stub/uart_functions.c 

OBJS += \
./Core/Src/stub/crc_functions.o \
./Core/Src/stub/uart_functions.o 

C_DEPS += \
./Core/Src/stub/crc_functions.d \
./Core/Src/stub/uart_functions.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/stub/%.o Core/Src/stub/%.su Core/Src/stub/%.cyclo: ../Core/Src/stub/%.c Core/Src/stub/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/board1" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-stub

clean-Core-2f-Src-2f-stub:
	-$(RM) ./Core/Src/stub/crc_functions.cyclo ./Core/Src/stub/crc_functions.d ./Core/Src/stub/crc_functions.o ./Core/Src/stub/crc_functions.su ./Core/Src/stub/uart_functions.cyclo ./Core/Src/stub/uart_functions.d ./Core/Src/stub/uart_functions.o ./Core/Src/stub/uart_functions.su

.PHONY: clean-Core-2f-Src-2f-stub

