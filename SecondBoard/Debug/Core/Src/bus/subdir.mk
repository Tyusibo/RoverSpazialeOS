################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bus/deserialize.c \
../Core/Src/bus/serialize.c 

OBJS += \
./Core/Src/bus/deserialize.o \
./Core/Src/bus/serialize.o 

C_DEPS += \
./Core/Src/bus/deserialize.d \
./Core/Src/bus/serialize.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bus/%.o Core/Src/bus/%.su Core/Src/bus/%.cyclo: ../Core/Src/bus/%.c Core/Src/bus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/board2" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-bus

clean-Core-2f-Src-2f-bus:
	-$(RM) ./Core/Src/bus/deserialize.cyclo ./Core/Src/bus/deserialize.d ./Core/Src/bus/deserialize.o ./Core/Src/bus/deserialize.su ./Core/Src/bus/serialize.cyclo ./Core/Src/bus/serialize.d ./Core/Src/bus/serialize.o ./Core/Src/bus/serialize.su

.PHONY: clean-Core-2f-Src-2f-bus

