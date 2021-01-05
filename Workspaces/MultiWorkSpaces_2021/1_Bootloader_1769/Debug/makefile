################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include freertos/src/subdir.mk
-include 0_Communication/USB/scr/subdir.mk
-include 0_Communication/RS485/subdir.mk
-include 0_Communication/subdir.mk
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
all: 1_Bootloader_1769.axf

# Tool invocations
1_Bootloader_1769.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\Release" -L"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_chip_175x_6x\libs" -L"C:\Robot\Robotic-System.git\trunk\Workspaces\MultiWorkSpaces_2021\0_lpc_board_nxp_lpcxpresso_1769\Release" -Xlinker -Map="1_Bootloader_1769.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "1_Bootloader_1769_Debug.ld" -o "1_Bootloader_1769.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) 1_Bootloader_1769.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "1_Bootloader_1769.axf"; # arm-none-eabi-objcopy -v -O binary "1_Bootloader_1769.axf" "1_Bootloader_1769.bin" ; # checksum -p LPC1769 -d "1_Bootloader_1769.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
