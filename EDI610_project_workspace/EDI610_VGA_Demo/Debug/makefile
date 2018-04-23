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
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
EDI610_VGA_Demo.elf.size \


# All Target
all: EDI610_VGA_Demo.elf secondary-outputs

# Tool invocations
EDI610_VGA_Demo.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze gcc linker'
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../EDI610_VGA_Demo_bsp/microblaze_0/lib -mlittle-endian -mcpu=v9.6 -mxl-soft-mul -Wl,--no-relax -o "EDI610_VGA_Demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

EDI610_VGA_Demo.elf.size: EDI610_VGA_Demo.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size EDI610_VGA_Demo.elf  |tee "EDI610_VGA_Demo.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) EDI610_VGA_Demo.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
