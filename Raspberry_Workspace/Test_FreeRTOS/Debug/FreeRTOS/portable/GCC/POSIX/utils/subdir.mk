################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/POSIX/utils/wait_for_event.c 

OBJS += \
./FreeRTOS/portable/GCC/POSIX/utils/wait_for_event.o 

C_DEPS += \
./FreeRTOS/portable/GCC/POSIX/utils/wait_for_event.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/POSIX/utils/%.o: ../FreeRTOS/portable/GCC/POSIX/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable/GCC/POSIX" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable/GCC/POSIX/utils" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/Astar/Astar" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/TCP_UDP" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


