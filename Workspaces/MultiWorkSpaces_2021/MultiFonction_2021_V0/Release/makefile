################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include freertos/src/subdir.mk
-include freertos/FreeRTOS_Trace/subdir.mk
-include Level_0/0_Drivers/0_Servos/subdir.mk
-include Level_0/0_Drivers/0_Motors/subdir.mk
-include Level_0/0_Drivers/0_Codeurs/subdir.mk
-include Level_0/0_Drivers/0_AX12/subdir.mk
-include Level_0/0_Drivers/0_ADC/subdir.mk
-include Communication/USB/src/subdir.mk
-include Communication/RS485/subdir.mk
-include Communication/subdir.mk
-include Asservissement/2_Splines/subdir.mk
-include Asservissement/1_PID/subdir.mk
-include Asservissement/1_Odometrie/subdir.mk
-include Asservissement/subdir.mk
-include Actionneurs_Capteurs/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MultiFonction_2021_V0.axf

# Tool invocations
MultiFonction_2021_V0.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\Release" -L"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\libs" -L"D:\Developpement_Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\Release" -Xlinker -Map="MultiFonction_2021_V0.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "MultiFonction_2021_V0_Release.ld" -u _printf_float -o "MultiFonction_2021_V0.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) MultiFonction_2021_V0.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MultiFonction_2021_V0.axf" ; arm-none-eabi-objcopy -O ihex  "MultiFonction_2021_V0.axf"  "MultiFonction_2021_V0.hex" ; # checksum -p LPC1769 -d "MultiFonction_2021_V0.bin"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
