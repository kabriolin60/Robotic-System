################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Communication/0_Communication.c \
../Communication/0_Event_Group.c \
../Communication/1_Trame_Communication.c \
../Communication/2_Echange_Datas.c \
../Communication/2_Echange_Datas_Reception.c 

OBJS += \
./Communication/0_Communication.o \
./Communication/0_Event_Group.o \
./Communication/1_Trame_Communication.o \
./Communication/2_Echange_Datas.o \
./Communication/2_Echange_Datas_Reception.o 

C_DEPS += \
./Communication/0_Communication.d \
./Communication/0_Event_Group.d \
./Communication/1_Trame_Communication.d \
./Communication/2_Echange_Datas.d \
./Communication/2_Echange_Datas_Reception.d 


# Each subdirectory must supply rules for building sources it contributes
Communication/%.o: ../Communication/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DTYPE_CARTE_IA -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -D__NEWLIB__ -I"D:\Developpement_Robot\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\freertos" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\freertos\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\freertos\FreeRTOS_Trace" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Communication\USB" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Communication\USB\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Communication" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Communication\RS485" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Pathfinding\Astar" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Strategie" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Strategie\Deplacement" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


