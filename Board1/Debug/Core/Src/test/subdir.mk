################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/test/lights_test.c \
../Core/Src/test/motors_test.c \
../Core/Src/test/print.c 

OBJS += \
./Core/Src/test/lights_test.o \
./Core/Src/test/motors_test.o \
./Core/Src/test/print.o 

C_DEPS += \
./Core/Src/test/lights_test.d \
./Core/Src/test/motors_test.d \
./Core/Src/test/print.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/test/%.o Core/Src/test/%.su Core/Src/test/%.cyclo: ../Core/Src/test/%.c Core/Src/test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/board1" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/test" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/wcet_estimation" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-test

clean-Core-2f-Src-2f-test:
	-$(RM) ./Core/Src/test/lights_test.cyclo ./Core/Src/test/lights_test.d ./Core/Src/test/lights_test.o ./Core/Src/test/lights_test.su ./Core/Src/test/motors_test.cyclo ./Core/Src/test/motors_test.d ./Core/Src/test/motors_test.o ./Core/Src/test/motors_test.su ./Core/Src/test/print.cyclo ./Core/Src/test/print.d ./Core/Src/test/print.o ./Core/Src/test/print.su

.PHONY: clean-Core-2f-Src-2f-test

