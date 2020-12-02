################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Common_Init.c 

OBJS += \
./src/Common_Init.o 

C_DEPS += \
./src/Common_Init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__REDLIB__ -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -I"D:\Developpement_Robot\Robotic-System\trunk\TEST\Test_Workspace2\lpc_board_nxp_lpcxpresso_1769\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\TEST\Test_Workspace2\lpc_chip_175x_6x\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\TEST\Test_Workspace2\0_Robot_Library\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\TEST\Test_Workspace2\0_Robot_Library\FreeRTOS" -I"D:\Developpement_Robot\Robotic-System\trunk\TEST\Test_Workspace2\0_Robot_Library\FreeRTOS\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\TEST\Test_Workspace2\0_Robot_Library\FreeRTOS\FreeRTOS_Trace" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


