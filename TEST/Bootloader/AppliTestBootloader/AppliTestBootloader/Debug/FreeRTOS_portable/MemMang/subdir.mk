################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS_portable/MemMang/heap_4.c 

OBJS += \
./FreeRTOS_portable/MemMang/heap_4.o 

C_DEPS += \
./FreeRTOS_portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_portable/MemMang/%.o: ../FreeRTOS_portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I"C:\Users\Fixe\Desktop\Eurobot SVN\trunk\Informatique\Bootloader\AppliTestBootloader\src" -I"C:\Users\Fixe\Desktop\Eurobot SVN\trunk\Informatique\Bootloader\AppliTestBootloader\FreeRTOS_include" -I"C:\Users\Fixe\Desktop\Eurobot SVN\trunk\Informatique\Bootloader\AppliTestBootloader\FreeRTOS_portable" -I"C:\Users\Fixe\Desktop\Eurobot SVN\trunk\Informatique\Bootloader\CMSISv2p00_LPC17xx\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


