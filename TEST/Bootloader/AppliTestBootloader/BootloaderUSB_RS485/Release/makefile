################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include USB/subdir.mk
-include FreeRTOS_src/subdir.mk
-include FreeRTOS_portable/MemMang/subdir.mk
-include FreeRTOS_portable/subdir.mk
-include Flash/subdir.mk
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
all: BootloaderUSB_RS485.axf

# Tool invocations
BootloaderUSB_RS485.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\nicolas\Desktop\Eurobot 2015 SVN\trunk\Informatique\Bootloader\CMSISv2p00_LPC17xx\inc" -L"C:\Users\nicolas\Desktop\Eurobot 2015 SVN\trunk\Informatique\Bootloader\BootloaderUSB_RS485\USB" -Xlinker -Map="BootloaderUSB_RS485.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "BootloaderUSB_RS485.ld" -o "BootloaderUSB_RS485.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) BootloaderUSB_RS485.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "BootloaderUSB_RS485.axf";  arm-none-eabi-objcopy -O binary "BootloaderUSB_RS485.axf" "BootloaderUSB_RS485.bin" ; checksum -p LPC1769 -d "BootloaderUSB_RS485.bin"; arm-none-eabi-objcopy -I binary "BootloaderUSB_RS485.bin" -O ihex "BootloaderUSB_RS485.hex";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
