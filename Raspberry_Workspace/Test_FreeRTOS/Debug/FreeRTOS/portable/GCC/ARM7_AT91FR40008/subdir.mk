################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/ARM7_AT91FR40008/port.c \
../FreeRTOS/portable/GCC/ARM7_AT91FR40008/portISR.c 

OBJS += \
./FreeRTOS/portable/GCC/ARM7_AT91FR40008/port.o \
./FreeRTOS/portable/GCC/ARM7_AT91FR40008/portISR.o 

C_DEPS += \
./FreeRTOS/portable/GCC/ARM7_AT91FR40008/port.d \
./FreeRTOS/portable/GCC/ARM7_AT91FR40008/portISR.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/ARM7_AT91FR40008/%.o: ../FreeRTOS/portable/GCC/ARM7_AT91FR40008/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/portable" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


