################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Level_0/0_Communication/0_Communication.c \
../Level_0/0_Communication/0_Event_Group.c 

OBJS += \
./Level_0/0_Communication/0_Communication.o \
./Level_0/0_Communication/0_Event_Group.o 

C_DEPS += \
./Level_0/0_Communication/0_Communication.d \
./Level_0/0_Communication/0_Event_Group.d 


# Each subdirectory must supply rules for building sources it contributes
Level_0/0_Communication/%.o: ../Level_0/0_Communication/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DTYPE_CARTE_IA -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCR_INTEGER_PRINTF -D__LPC17XX__ -D__REDLIB__ -I"D:\Developpement_Robot\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\inc\usbd" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\freertos" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\freertos\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\freertos\FreeRTOS_Trace" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Level_0\0_Communication" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Level_0\0_Communication\USB" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Level_0\0_Communication\USB\inc" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Level_0\0_Communication\RS485" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Level_1\1_Communication" -I"D:\Developpement_Robot\MultiWorkSpaces_2021\IA_2021_V0\Level_2\2_Communication" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


