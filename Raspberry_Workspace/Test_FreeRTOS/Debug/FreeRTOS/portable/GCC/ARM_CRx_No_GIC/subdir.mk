################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/ARM_CRx_No_GIC/port.c 

S_UPPER_SRCS += \
../FreeRTOS/portable/GCC/ARM_CRx_No_GIC/portASM.S 

OBJS += \
./FreeRTOS/portable/GCC/ARM_CRx_No_GIC/port.o \
./FreeRTOS/portable/GCC/ARM_CRx_No_GIC/portASM.o 

C_DEPS += \
./FreeRTOS/portable/GCC/ARM_CRx_No_GIC/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/ARM_CRx_No_GIC/%.o: ../FreeRTOS/portable/GCC/ARM_CRx_No_GIC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/eclipse-workspace/Test_FreeRTOS/FreeRTOS/portable" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS/portable/GCC/ARM_CRx_No_GIC/%.o: ../FreeRTOS/portable/GCC/ARM_CRx_No_GIC/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


