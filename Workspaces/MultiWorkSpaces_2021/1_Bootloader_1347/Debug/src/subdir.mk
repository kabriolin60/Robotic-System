################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Bootloader.c \
../src/Bootloader_Data.c \
../src/Flash_iap.c \
../src/Init.c \
../src/TEST.c \
../src/cr_startup_lpc13xx.c \
../src/main.c \
../src/sysinit.c 

OBJS += \
./src/Bootloader.o \
./src/Bootloader_Data.o \
./src/Flash_iap.o \
./src/Init.o \
./src/TEST.o \
./src/cr_startup_lpc13xx.o \
./src/main.o \
./src/sysinit.o 

C_DEPS += \
./src/Bootloader.d \
./src/Bootloader_Data.d \
./src/Flash_iap.d \
./src/Init.d \
./src/TEST.d \
./src/cr_startup_lpc13xx.d \
./src/main.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M3 -D__NEWLIB__ -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_13xx\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1347\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\1_Bootloader_1347\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\1_Bootloader_1347\freertos\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\1_Bootloader_1347\freertos" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\1_Bootloader_1347\Communication" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\1_Bootloader_1347\Communication\RS485" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\MultiFonction_2021_V0\Definitions" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

