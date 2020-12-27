################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/cr_startup_lpc11xx.c \
../example/src/freertos_blinky.c \
../example/src/sysinit.c 

OBJS += \
./example/src/cr_startup_lpc11xx.o \
./example/src/freertos_blinky.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/cr_startup_lpc11xx.d \
./example/src/freertos_blinky.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M0 -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\Peripheriques_2021_V0\example\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\Peripheriques_2021_V0\freertos\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_11cxx_lib\inc" -I"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\0_lpcxpresso_11c24_board_lib\inc" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


