################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/AVR_AVRDx/port.c 

OBJS += \
./FreeRTOS/portable/GCC/AVR_AVRDx/port.o 

C_DEPS += \
./FreeRTOS/portable/GCC/AVR_AVRDx/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/AVR_AVRDx/%.o: ../FreeRTOS/portable/GCC/AVR_AVRDx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/portable" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


