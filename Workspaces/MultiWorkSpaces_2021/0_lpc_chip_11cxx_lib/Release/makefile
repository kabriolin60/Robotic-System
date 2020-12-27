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
all: lib0_lpc_chip_11cxx_lib.a

# Tool invocations
lib0_lpc_chip_11cxx_lib.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Archiver'
	arm-none-eabi-ar -r  "lib0_lpc_chip_11cxx_lib.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(C_DEPS) lib0_lpc_chip_11cxx_lib.a
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "lib0_lpc_chip_11cxx_lib.a" ; # arm-none-eabi-objdump -h -S "lib0_lpc_chip_11cxx_lib.a" >"lib0_lpc_chip_11cxx_lib.lss"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
