################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS_src/croutine.c \
../FreeRTOS_src/event_groups.c \
../FreeRTOS_src/list.c \
../FreeRTOS_src/queue.c \
../FreeRTOS_src/tasks.c \
../FreeRTOS_src/timers.c 

OBJS += \
./FreeRTOS_src/croutine.o \
./FreeRTOS_src/event_groups.o \
./FreeRTOS_src/list.o \
./FreeRTOS_src/queue.o \
./FreeRTOS_src/tasks.o \
./FreeRTOS_src/timers.o 

C_DEPS += \
./FreeRTOS_src/croutine.d \
./FreeRTOS_src/event_groups.d \
./FreeRTOS_src/list.d \
./FreeRTOS_src/queue.d \
./FreeRTOS_src/tasks.d \
./FreeRTOS_src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_src/%.o: ../FreeRTOS_src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\src" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\Flash" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\Communication" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB\usbd" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\FreeRTOS_include" -I"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\FreeRTOS_portable" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


