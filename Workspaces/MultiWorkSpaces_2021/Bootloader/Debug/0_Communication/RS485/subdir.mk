################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../0_Communication/RS485/0_RS485.c 

OBJS += \
./0_Communication/RS485/0_RS485.o 

C_DEPS += \
./0_Communication/RS485/0_RS485.d 


# Each subdirectory must supply rules for building sources it contributes
0_Communication/RS485/%.o: ../0_Communication/RS485/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -D__NEWLIB__ -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\Bootloader\freertos" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\Bootloader\freertos\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\Bootloader\0_Communication\USB\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\Bootloader\0_Communication" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\Bootloader\0_Communication\RS485" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\Bootloader\src" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


