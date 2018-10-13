################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cortexm/_initialize_hardware.c \
../system/src/cortexm/_reset_hardware.c \
../system/src/cortexm/exception_handlers.c 

OBJS += \
./system/src/cortexm/_initialize_hardware.o \
./system/src/cortexm/_reset_hardware.o \
./system/src/cortexm/exception_handlers.o 

C_DEPS += \
./system/src/cortexm/_initialize_hardware.d \
./system/src/cortexm/_reset_hardware.d \
./system/src/cortexm/exception_handlers.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cortexm/%.o: ../system/src/cortexm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -march=armv7e-m -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I/usr/include/newlib -I"/home/developer/CanSat_20I9/board/mock_up/system/include" -I"/home/developer/CanSat_20I9/board/mock_up/system/include/cmsis" -I"/home/developer/CanSat_20I9/board/mock_up/system/include/stm32f4-hal" -I"/home/developer/CanSat_20I9/board/mock_up/system-usr" -I"/home/developer/CanSat_20I9/board/mock_up/FreeRTOS/include" -I"/home/developer/CanSat_20I9/board/mock_up/FreeRTOS/portable" -I"/home/developer/CanSat_20I9/board/mock_up/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/developer/CanSat_20I9/board/mock_up/FreeRTOS-usr" -I"/home/developer/CanSat_20I9/board/mock_up/sofa" -I"/home/developer/CanSat_20I9/board/mock_up/src" -I"/home/developer/CanSat_20I9/board/mock_up/src/drivers" -I"/home/developer/CanSat_20I9/board/mock_up/src/library" -I"/home/developer/CanSat_20I9/board/mock_up/src/modules" -I"/home/developer/CanSat_20I9/board/mock_up/mavlink-messages" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


