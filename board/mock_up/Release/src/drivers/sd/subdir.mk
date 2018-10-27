################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/sd/crc.c \
../src/drivers/sd/dump.c \
../src/drivers/sd/sd.c 

OBJS += \
./src/drivers/sd/crc.o \
./src/drivers/sd/dump.o \
./src/drivers/sd/sd.o 

C_DEPS += \
./src/drivers/sd/crc.d \
./src/drivers/sd/dump.d \
./src/drivers/sd/sd.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/sd/%.o: ../src/drivers/sd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -march=armv7e-m -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DNDEBUG -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I/usr/include/newlib -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


