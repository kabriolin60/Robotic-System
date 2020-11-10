################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB/cdc_desc.c \
../USB/cdc_main.c \
../USB/cdc_vcom.c \
../USB/iocon_17xx_40xx.c 

OBJS += \
./USB/cdc_desc.o \
./USB/cdc_main.o \
./USB/cdc_vcom.o \
./USB/iocon_17xx_40xx.o 

C_DEPS += \
./USB/cdc_desc.d \
./USB/cdc_main.d \
./USB/cdc_vcom.d \
./USB/iocon_17xx_40xx.d 


# Each subdirectory must supply rules for building sources it contributes
USB/%.o: ../USB/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\src" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\Flash" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\Communication" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB\usbd" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\FreeRTOS_include" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\FreeRTOS_portable" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


