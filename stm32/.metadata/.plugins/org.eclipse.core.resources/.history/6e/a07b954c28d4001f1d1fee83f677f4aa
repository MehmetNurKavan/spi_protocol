################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := spi_protocol_kvn_ucdr
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
spi_protocol_kvn_ucdr.elf \

MAP_FILES += \
spi_protocol_kvn_ucdr.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
spi_protocol_kvn_ucdr.list \

OBJCOPY_HEX += \
spi_protocol_kvn_ucdr.hex \

OBJCOPY_BIN += \
spi_protocol_kvn_ucdr.bin \


# All Target
all: main-build

# Main-build Target
main-build: spi_protocol_kvn_ucdr.elf secondary-outputs

# Tool invocations
spi_protocol_kvn_ucdr.elf spi_protocol_kvn_ucdr.map: $(OBJS) $(USER_OBJS) C:\Users\Mehmet\ Nur\ Kavan\Desktop\spi_protocol\stm32\spi_protocol_kvn_ucdr\STM32F103C6TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "spi_protocol_kvn_ucdr.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Mehmet Nur Kavan\Desktop\spi_protocol\stm32\spi_protocol_kvn_ucdr\STM32F103C6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="spi_protocol_kvn_ucdr.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

spi_protocol_kvn_ucdr.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "spi_protocol_kvn_ucdr.list"
	@echo 'Finished building: $@'
	@echo ' '

spi_protocol_kvn_ucdr.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "spi_protocol_kvn_ucdr.hex"
	@echo 'Finished building: $@'
	@echo ' '

spi_protocol_kvn_ucdr.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "spi_protocol_kvn_ucdr.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout spi_protocol_kvn_ucdr.bin spi_protocol_kvn_ucdr.elf spi_protocol_kvn_ucdr.hex spi_protocol_kvn_ucdr.list spi_protocol_kvn_ucdr.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
