################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/wcet_estimation.c/DWT.c 

OBJS += \
./Core/Src/wcet_estimation.c/DWT.o 

C_DEPS += \
./Core/Src/wcet_estimation.c/DWT.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/wcet_estimation.c/%.o Core/Src/wcet_estimation.c/%.su Core/Src/wcet_estimation.c/%.cyclo: ../Core/Src/wcet_estimation.c/%.c Core/Src/wcet_estimation.c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/board1" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/test" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board1/Core/Inc/wcet_estimation" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-wcet_estimation-2e-c

clean-Core-2f-Src-2f-wcet_estimation-2e-c:
	-$(RM) ./Core/Src/wcet_estimation.c/DWT.cyclo ./Core/Src/wcet_estimation.c/DWT.d ./Core/Src/wcet_estimation.c/DWT.o ./Core/Src/wcet_estimation.c/DWT.su

.PHONY: clean-Core-2f-Src-2f-wcet_estimation-2e-c

