################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/driver_init/gyroscope_init.c \
../Core/Src/driver_init/gyroscope_test.c \
../Core/Src/driver_init/sonar_init.c 

OBJS += \
./Core/Src/driver_init/gyroscope_init.o \
./Core/Src/driver_init/gyroscope_test.o \
./Core/Src/driver_init/sonar_init.o 

C_DEPS += \
./Core/Src/driver_init/gyroscope_init.d \
./Core/Src/driver_init/gyroscope_test.d \
./Core/Src/driver_init/sonar_init.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/driver_init/%.o Core/Src/driver_init/%.su Core/Src/driver_init/%.cyclo: ../Core/Src/driver_init/%.c Core/Src/driver_init/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/board2" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-driver_init

clean-Core-2f-Src-2f-driver_init:
	-$(RM) ./Core/Src/driver_init/gyroscope_init.cyclo ./Core/Src/driver_init/gyroscope_init.d ./Core/Src/driver_init/gyroscope_init.o ./Core/Src/driver_init/gyroscope_init.su ./Core/Src/driver_init/gyroscope_test.cyclo ./Core/Src/driver_init/gyroscope_test.d ./Core/Src/driver_init/gyroscope_test.o ./Core/Src/driver_init/gyroscope_test.su ./Core/Src/driver_init/sonar_init.cyclo ./Core/Src/driver_init/sonar_init.d ./Core/Src/driver_init/sonar_init.o ./Core/Src/driver_init/sonar_init.su

.PHONY: clean-Core-2f-Src-2f-driver_init

