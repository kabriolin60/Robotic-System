################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Communication/Communication_NG.c \
../Communication/Echange_Data.c \
../Communication/FIFO.c \
../Communication/Uart1.c 

OBJS += \
./Communication/Communication_NG.o \
./Communication/Echange_Data.o \
./Communication/FIFO.o \
./Communication/Uart1.o 

C_DEPS += \
./Communication/Communication_NG.d \
./Communication/Echange_Data.d \
./Communication/FIFO.d \
./Communication/Uart1.d 


# Each subdirectory must supply rules for building sources it contributes
Communication/%.o: ../Communication/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\src" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\Flash" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\Communication" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB\usbd" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\FreeRTOS_include" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\FreeRTOS_portable" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


