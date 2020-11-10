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
all: TestFreeRTOSV8_USB.axf

# Tool invocations
TestFreeRTOSV8_USB.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Fixe\Desktop\Eurobot 214 SVN\trunk\Informatique\Bootloader\TestFreeRTOSV8_USB\USB" -Xlinker -Map="TestFreeRTOSV8_USB.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "TestFreeRTOSV8_USB_Multiple_configurations.ld" -o "TestFreeRTOSV8_USB.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) TestFreeRTOSV8_USB.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TestFreeRTOSV8_USB.axf";  arm-none-eabi-objcopy -O binary "TestFreeRTOSV8_USB.axf" "TestFreeRTOSV8_USB.bin" ; checksum -p LPC1769 -d "TestFreeRTOSV8_USB.bin"; arm-none-eabi-objcopy -I binary "TestFreeRTOSV8_USB.bin" -O ihex "TestFreeRTOSV8_USB.hex";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
