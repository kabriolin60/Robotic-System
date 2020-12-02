################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/IA_2021_V1.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/sysinit.c \
../src/test.c 

OBJS += \
./src/IA_2021_V1.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/sysinit.o \
./src/test.o 

C_DEPS += \
./src/IA_2021_V1.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/sysinit.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -DNO_BOARD_LIB -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_board\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_chip\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_chip\inc\usbd" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS\FreeRTOS_Trace" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\IA_2021_V1\inc" -Og -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


