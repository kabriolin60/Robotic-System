################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/STR75x/port.c \
../FreeRTOS/portable/GCC/STR75x/portISR.c 

OBJS += \
./FreeRTOS/portable/GCC/STR75x/port.o \
./FreeRTOS/portable/GCC/STR75x/portISR.o 

C_DEPS += \
./FreeRTOS/portable/GCC/STR75x/port.d \
./FreeRTOS/portable/GCC/STR75x/portISR.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/STR75x/%.o: ../FreeRTOS/portable/GCC/STR75x/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/portable" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


