################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g474retx.s 

OBJS += \
./Core/Startup/startup_stm32g474retx.o 

S_DEPS += \
./Core/Startup/startup_stm32g474retx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/actions" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/board2" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/bus" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/constants" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/driver" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/driver_init" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/stub" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/test" -I"C:/Users/andre/Desktop/ProgettiCUBE/RoverSpazialeOS/Board2/Core/Inc/wcet_estimation" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g474retx.d ./Core/Startup/startup_stm32g474retx.o

.PHONY: clean-Core-2f-Startup

