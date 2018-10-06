################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/library/MadgwickAHRS.c \
../src/library/quaternion.c 

OBJS += \
./src/library/MadgwickAHRS.o \
./src/library/quaternion.o 

C_DEPS += \
./src/library/MadgwickAHRS.d \
./src/library/quaternion.d 


# Each subdirectory must supply rules for building sources it contributes
src/library/%.o: ../src/library/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -march=armv7e-m -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/system/include" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/system/include/cmsis" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/system/include/stm32f4-hal" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/system-usr" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/FreeRTOS/include" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/FreeRTOS/portable" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/FreeRTOS-usr" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/sofa" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/src" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/src/drivers" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/src/library" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/src/modules" -I"/home/developer/CanSat_20I9/board/Unica_20I9_STM32F405/mavlink-messages" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


