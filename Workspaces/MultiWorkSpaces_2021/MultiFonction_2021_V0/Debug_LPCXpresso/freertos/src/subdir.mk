################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/src/FreeRTOSCommonHooks.c \
../freertos/src/croutine.c \
../freertos/src/event_groups.c \
../freertos/src/heap_5.c \
../freertos/src/list.c \
../freertos/src/port.c \
../freertos/src/queue.c \
../freertos/src/stream_buffer.c \
../freertos/src/tasks.c \
../freertos/src/timers.c 

OBJS += \
./freertos/src/FreeRTOSCommonHooks.o \
./freertos/src/croutine.o \
./freertos/src/event_groups.o \
./freertos/src/heap_5.o \
./freertos/src/list.o \
./freertos/src/port.o \
./freertos/src/queue.o \
./freertos/src/stream_buffer.o \
./freertos/src/tasks.o \
./freertos/src/timers.o 

C_DEPS += \
./freertos/src/FreeRTOSCommonHooks.d \
./freertos/src/croutine.d \
./freertos/src/event_groups.d \
./freertos/src/heap_5.d \
./freertos/src/list.d \
./freertos/src/port.d \
./freertos/src/queue.d \
./freertos/src/stream_buffer.d \
./freertos/src/tasks.d \
./freertos/src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/src/%.o: ../freertos/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__CODE_RED -DDEBUG -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -DTYPE_CARTE_MULTIFCT -D__NEWLIB__ -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\freertos\FreeRTOS_Trace" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB\inc" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\USB\src" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Communication\RS485" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\1_PID" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\1_Odometrie" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Asservissement\2_Splines" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Actionneurs_Capteurs" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Codeurs" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Motors" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_Servos" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_AX12" -I"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Level_0\0_Drivers\0_ADC" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


