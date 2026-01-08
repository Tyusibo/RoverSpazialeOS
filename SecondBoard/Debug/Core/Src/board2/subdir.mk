################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/board2/Board2.c 

OBJS += \
./Core/Src/board2/Board2.o 

C_DEPS += \
./Core/Src/board2/Board2.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/board2/%.o Core/Src/board2/%.su Core/Src/board2/%.cyclo: ../Core/Src/board2/%.c Core/Src/board2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/board2" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/SecondBoard/Core/Inc/test" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-board2

clean-Core-2f-Src-2f-board2:
	-$(RM) ./Core/Src/board2/Board2.cyclo ./Core/Src/board2/Board2.d ./Core/Src/board2/Board2.o ./Core/Src/board2/Board2.su

.PHONY: clean-Core-2f-Src-2f-board2

