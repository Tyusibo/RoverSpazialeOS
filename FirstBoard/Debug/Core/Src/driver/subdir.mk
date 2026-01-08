################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/driver/a4wd3_led.c \
../Core/Src/driver/encoder.c \
../Core/Src/driver/motor_control.c 

OBJS += \
./Core/Src/driver/a4wd3_led.o \
./Core/Src/driver/encoder.o \
./Core/Src/driver/motor_control.o 

C_DEPS += \
./Core/Src/driver/a4wd3_led.d \
./Core/Src/driver/encoder.d \
./Core/Src/driver/motor_control.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/driver/%.o Core/Src/driver/%.su Core/Src/driver/%.cyclo: ../Core/Src/driver/%.c Core/Src/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/board1" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/FirstBoard/Core/Inc/test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-driver

clean-Core-2f-Src-2f-driver:
	-$(RM) ./Core/Src/driver/a4wd3_led.cyclo ./Core/Src/driver/a4wd3_led.d ./Core/Src/driver/a4wd3_led.o ./Core/Src/driver/a4wd3_led.su ./Core/Src/driver/encoder.cyclo ./Core/Src/driver/encoder.d ./Core/Src/driver/encoder.o ./Core/Src/driver/encoder.su ./Core/Src/driver/motor_control.cyclo ./Core/Src/driver/motor_control.d ./Core/Src/driver/motor_control.o ./Core/Src/driver/motor_control.su

.PHONY: clean-Core-2f-Src-2f-driver

