################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/MicroBlazeV8/port.c \
../FreeRTOS/portable/GCC/MicroBlazeV8/port_exceptions.c 

S_UPPER_SRCS += \
../FreeRTOS/portable/GCC/MicroBlazeV8/portasm.S 

OBJS += \
./FreeRTOS/portable/GCC/MicroBlazeV8/port.o \
./FreeRTOS/portable/GCC/MicroBlazeV8/port_exceptions.o \
./FreeRTOS/portable/GCC/MicroBlazeV8/portasm.o 

C_DEPS += \
./FreeRTOS/portable/GCC/MicroBlazeV8/port.d \
./FreeRTOS/portable/GCC/MicroBlazeV8/port_exceptions.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/MicroBlazeV8/%.o: ../FreeRTOS/portable/GCC/MicroBlazeV8/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/portable" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS/portable/GCC/MicroBlazeV8/%.o: ../FreeRTOS/portable/GCC/MicroBlazeV8/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


