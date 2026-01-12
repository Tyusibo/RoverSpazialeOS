################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/driver/HCSR04.c \
../Core/Src/driver/motor_control.c \
../Core/Src/driver/mpu6050.c \
../Core/Src/driver/pad_receiver.c 

OBJS += \
./Core/Src/driver/HCSR04.o \
./Core/Src/driver/motor_control.o \
./Core/Src/driver/mpu6050.o \
./Core/Src/driver/pad_receiver.o 

C_DEPS += \
./Core/Src/driver/HCSR04.d \
./Core/Src/driver/motor_control.d \
./Core/Src/driver/mpu6050.d \
./Core/Src/driver/pad_receiver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/driver/%.o Core/Src/driver/%.su Core/Src/driver/%.cyclo: ../Core/Src/driver/%.c Core/Src/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/board2" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/test" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/wcet_estimation" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-driver

clean-Core-2f-Src-2f-driver:
	-$(RM) ./Core/Src/driver/HCSR04.cyclo ./Core/Src/driver/HCSR04.d ./Core/Src/driver/HCSR04.o ./Core/Src/driver/HCSR04.su ./Core/Src/driver/motor_control.cyclo ./Core/Src/driver/motor_control.d ./Core/Src/driver/motor_control.o ./Core/Src/driver/motor_control.su ./Core/Src/driver/mpu6050.cyclo ./Core/Src/driver/mpu6050.d ./Core/Src/driver/mpu6050.o ./Core/Src/driver/mpu6050.su ./Core/Src/driver/pad_receiver.cyclo ./Core/Src/driver/pad_receiver.d ./Core/Src/driver/pad_receiver.o ./Core/Src/driver/pad_receiver.su

.PHONY: clean-Core-2f-Src-2f-driver

