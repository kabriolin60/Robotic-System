################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Communication/USB/src/cdc_desc.c \
../Communication/USB/src/cdc_usb_main.c \
../Communication/USB/src/cdc_vcom.c 

OBJS += \
./Communication/USB/src/cdc_desc.o \
./Communication/USB/src/cdc_usb_main.o \
./Communication/USB/src/cdc_vcom.o 

C_DEPS += \
./Communication/USB/src/cdc_desc.d \
./Communication/USB/src/cdc_usb_main.d \
./Communication/USB/src/cdc_vcom.d 


# Each subdirectory must supply rules for building sources it contributes
Communication/USB/src/%.o: ../Communication/USB/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DNDEBUG -DTYPE_CARTE_IA -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -D__NEWLIB__ -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Level_0\Drivers\0_ADC" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\freertos" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\freertos\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\freertos\FreeRTOS_Trace" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication\USB" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication\USB\inc" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication\RS485" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Pathfinding\Astar" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Strategie" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Strategie\Strategie_Level0" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Strategie\Strategie_Level0\Deplacement" -I"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Strategie\Strategie_Level0\Actionneurs" -Og -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


