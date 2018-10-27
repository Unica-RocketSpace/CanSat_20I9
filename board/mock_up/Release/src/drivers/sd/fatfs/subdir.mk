################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/sd/fatfs/diskio.c \
../src/drivers/sd/fatfs/ff.c \
../src/drivers/sd/fatfs/ffsystem.c \
../src/drivers/sd/fatfs/ffunicode.c 

OBJS += \
./src/drivers/sd/fatfs/diskio.o \
./src/drivers/sd/fatfs/ff.o \
./src/drivers/sd/fatfs/ffsystem.o \
./src/drivers/sd/fatfs/ffunicode.o 

C_DEPS += \
./src/drivers/sd/fatfs/diskio.d \
./src/drivers/sd/fatfs/ff.d \
./src/drivers/sd/fatfs/ffsystem.d \
./src/drivers/sd/fatfs/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/sd/fatfs/%.o: ../src/drivers/sd/fatfs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -march=armv7e-m -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DNDEBUG -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I/usr/include/newlib -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


