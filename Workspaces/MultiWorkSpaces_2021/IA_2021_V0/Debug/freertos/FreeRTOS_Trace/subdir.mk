################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/FreeRTOS_Trace/trcHardwarePort.c \
../freertos/FreeRTOS_Trace/trcKernelPort.c \
../freertos/FreeRTOS_Trace/trcSnapshotRecorder.c 

OBJS += \
./freertos/FreeRTOS_Trace/trcHardwarePort.o \
./freertos/FreeRTOS_Trace/trcKernelPort.o \
./freertos/FreeRTOS_Trace/trcSnapshotRecorder.o 

C_DEPS += \
./freertos/FreeRTOS_Trace/trcHardwarePort.d \
./freertos/FreeRTOS_Trace/trcKernelPort.d \
./freertos/FreeRTOS_Trace/trcSnapshotRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/FreeRTOS_Trace/%.o: ../freertos/FreeRTOS_Trace/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DTYPE_CARTE_IA -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -D__NEWLIB__ -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\freertos" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\freertos\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\freertos\FreeRTOS_Trace" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication\USB" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication\USB\inc" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Communication\RS485" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Pathfinding\Astar" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Strategie" -I"C:\Robot\trunk\Workspaces\MultiWorkSpaces_2021\IA_2021_V0\Strategie\Deplacement" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


