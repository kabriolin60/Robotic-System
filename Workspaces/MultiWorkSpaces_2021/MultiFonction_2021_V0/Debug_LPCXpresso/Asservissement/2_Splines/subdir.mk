################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Asservissement/2_Splines/2_Spline.c 

OBJS += \
./Asservissement/2_Splines/2_Spline.o 

C_DEPS += \
./Asservissement/2_Splines/2_Spline.d 


# Each subdirectory must supply rules for building sources it contributes
Asservissement/2_Splines/%.o: ../Asservissement/2_Splines/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__CODE_RED -DDEBUG -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -DTYPE_CARTE_MULTIFCT -D__NEWLIB__ -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos\FreeRTOS_Trace" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB\src" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\RS485" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\1_PID" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\1_Odometrie" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\2_Splines" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Actionneurs_Capteurs" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Codeurs" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Motors" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Servos" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_AX12" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_ADC" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


