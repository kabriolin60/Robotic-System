################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include freertos/src/subdir.mk
-include example/src/subdir.mk
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
all: Peripheriques_2021_V1.axf

# Tool invocations
Peripheriques_2021_V1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_13xx\Debug" -L"D:\Developpement_Robot\Robotic-System\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1347\Debug" -Xlinker -Map="Peripheriques_2021_V1.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "Peripheriques_2021_V1_Debug.ld" -o "Peripheriques_2021_V1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Peripheriques_2021_V1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Peripheriques_2021_V1.axf"; # arm-none-eabi-objcopy -O binary "Peripheriques_2021_V1.axf" "Peripheriques_2021_V1.bin" ; checksum -p LPC1347 -d "Peripheriques_2021_V1.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
