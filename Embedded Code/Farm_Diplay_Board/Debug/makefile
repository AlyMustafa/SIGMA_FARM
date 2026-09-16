################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := Farm_Diplay_Board
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Farm_Diplay_Board.elf \

MAP_FILES += \
Farm_Diplay_Board.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Farm_Diplay_Board.list \

OBJCOPY_HEX += \
Farm_Diplay_Board.hex \

OBJCOPY_BIN += \
Farm_Diplay_Board.bin \

OBJCOPY_VERILOG += \
Farm_Diplay_Board.verilog \

OBJCOPY_SREC += \
Farm_Diplay_Board.srec \

OBJCOPY_SYMBOLSREC += \
Farm_Diplay_Board.symbolsrec \


# All Target
all: main-build

# Main-build Target
main-build: Farm_Diplay_Board.elf secondary-outputs

# Tool invocations
Farm_Diplay_Board.elf Farm_Diplay_Board.map: $(OBJS) $(USER_OBJS) C:\Users\Admin\Desktop\Farm_Diplay_Board_ref\STM32F030R8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Farm_Diplay_Board.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\Admin\Desktop\Farm_Diplay_Board_ref\STM32F030R8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Farm_Diplay_Board.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Farm_Diplay_Board.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Farm_Diplay_Board.list"
	@echo 'Finished building: $@'
	@echo ' '

Farm_Diplay_Board.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Farm_Diplay_Board.hex"
	@echo 'Finished building: $@'
	@echo ' '

Farm_Diplay_Board.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Farm_Diplay_Board.bin"
	@echo 'Finished building: $@'
	@echo ' '

Farm_Diplay_Board.verilog: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O verilog $(EXECUTABLES) "Farm_Diplay_Board.verilog"
	@echo 'Finished building: $@'
	@echo ' '

Farm_Diplay_Board.srec: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O srec $(EXECUTABLES) "Farm_Diplay_Board.srec"
	@echo 'Finished building: $@'
	@echo ' '

Farm_Diplay_Board.symbolsrec: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O symbolsrec $(EXECUTABLES) "Farm_Diplay_Board.symbolsrec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Farm_Diplay_Board.bin Farm_Diplay_Board.elf Farm_Diplay_Board.hex Farm_Diplay_Board.list Farm_Diplay_Board.map Farm_Diplay_Board.srec Farm_Diplay_Board.symbolsrec Farm_Diplay_Board.verilog default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN) $(OBJCOPY_VERILOG) $(OBJCOPY_SREC) $(OBJCOPY_SYMBOLSREC)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
