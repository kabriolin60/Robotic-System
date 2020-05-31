################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Level_0/0_Drivers/0_Codeurs/0_Codeurs.c 

OBJS += \
./Level_0/0_Drivers/0_Codeurs/0_Codeurs.o 

C_DEPS += \
./Level_0/0_Drivers/0_Codeurs/0_Codeurs.d 


# Each subdirectory must supply rules for building sources it contributes
Level_0/0_Drivers/0_Codeurs/%.o: ../Level_0/0_Drivers/0_Codeurs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__CODE_RED -DDEBUG -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -DTYPE_CARTE_MULTIFCT -D__NEWLIB__ -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos\FreeRTOS_Trace" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB\src" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\RS485" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\1_PID" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\1_Odometrie" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\2_Splines" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Actionneurs_Capteurs" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Codeurs" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Motors" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Servos" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_AX12" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_ADC" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


