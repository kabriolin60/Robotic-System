################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/croutine.c \
../FreeRTOS/event_groups.c \
../FreeRTOS/list.c \
../FreeRTOS/queue.c \
../FreeRTOS/stream_buffer.c \
../FreeRTOS/tasks.c \
../FreeRTOS/timers.c 

OBJS += \
./FreeRTOS/croutine.o \
./FreeRTOS/event_groups.o \
./FreeRTOS/list.o \
./FreeRTOS/queue.o \
./FreeRTOS/stream_buffer.o \
./FreeRTOS/tasks.o \
./FreeRTOS/timers.o 

C_DEPS += \
./FreeRTOS/croutine.d \
./FreeRTOS/event_groups.d \
./FreeRTOS/list.d \
./FreeRTOS/queue.d \
./FreeRTOS/stream_buffer.d \
./FreeRTOS/tasks.d \
./FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/%.o: ../FreeRTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable/GCC/POSIX" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable/GCC/POSIX/utils" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/Astar/Astar" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/TCP_UDP" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


