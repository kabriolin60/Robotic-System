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
-include Strategie/Strategie_Level0/Deplacement/subdir.mk
-include Strategie/Strategie_Level0/Actionneurs/subdir.mk
-include Strategie/Strategie_Level0/subdir.mk
-include Strategie/subdir.mk
-include Pathfinding/Astar/subdir.mk
-include Level_0/Drivers/0_ADC/subdir.mk
-include Communication/USB/src/subdir.mk
-include Communication/RS485/subdir.mk
-include Communication/subdir.mk
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
all: IA_2021_V0.axf

# Tool invocations
IA_2021_V0.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\Release" -L"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\libs" -L"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\Release" -Xlinker -Map="IA_2021_V0.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "IA_2021_V0_Release.ld" -u _printf_float -o "IA_2021_V0.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) IA_2021_V0.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "IA_2021_V0.axf" ; arm-none-eabi-objcopy -O ihex "IA_2021_V0.axf" "IA_2021_V0.hex" ; #checksum -p LPC1769 -d "IA_2021_V0.bin"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
