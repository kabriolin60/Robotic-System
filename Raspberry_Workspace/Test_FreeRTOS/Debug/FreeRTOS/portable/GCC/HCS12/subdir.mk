################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/HCS12/port.c 

OBJS += \
./FreeRTOS/portable/GCC/HCS12/port.o 

C_DEPS += \
./FreeRTOS/portable/GCC/HCS12/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/HCS12/%.o: ../FreeRTOS/portable/GCC/HCS12/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/portable" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


