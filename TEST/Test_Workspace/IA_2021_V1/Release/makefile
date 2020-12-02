################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
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
all: IA_2021_V1.axf

# Tool invocations
IA_2021_V1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\Release" -L"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_chip\libs" -Xlinker -Map="IA_2021_V1.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "IA_2021_V1_Release.ld" -u _printf_float -o "IA_2021_V1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) IA_2021_V1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "IA_2021_V1.axf"; # arm-none-eabi-objcopy -v -O binary "IA_2021_V1.axf" "IA_2021_V1.bin" ; # checksum -p LPC1769 -d "IA_2021_V1.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
