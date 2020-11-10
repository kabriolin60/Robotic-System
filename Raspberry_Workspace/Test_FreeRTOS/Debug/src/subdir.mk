################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Test_FreeRTOS.c \
../src/tiny_gpio.c 

OBJS += \
./src/Test_FreeRTOS.o \
./src/tiny_gpio.o 

C_DEPS += \
./src/Test_FreeRTOS.d \
./src/tiny_gpio.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/include" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable/GCC/POSIX" -I"/home/pi/git/Robotic-System/Raspberry_Workspace/Test_FreeRTOS/FreeRTOS/portable/GCC/POSIX/utils" -I/home/pi/WiringPi/wiringPi -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


