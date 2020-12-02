################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include FreeRTOS/src/subdir.mk
-include FreeRTOS/FreeRTOS_Trace/subdir.mk
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
all: lib0_Robot_Library.a

# Tool invocations
lib0_Robot_Library.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Archiver'
	arm-none-eabi-ar -r  "lib0_Robot_Library.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(C_DEPS) lib0_Robot_Library.a
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "lib0_Robot_Library.a" ; # arm-none-eabi-objdump -h -S "lib0_Robot_Library.a" >"lib0_Robot_Library.lss"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
