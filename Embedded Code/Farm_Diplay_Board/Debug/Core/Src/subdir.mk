################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Bloutoth_Program.c \
../Core/Src/CLCD_program.c \
../Core/Src/DS1308_Program.c \
../Core/Src/EEPROM_Program.c \
../Core/Src/SWCHES_Program.c \
../Core/Src/Servec_Program.c \
../Core/Src/main.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f0xx.c 

C_DEPS += \
./Core/Src/Bloutoth_Program.d \
./Core/Src/CLCD_program.d \
./Core/Src/DS1308_Program.d \
./Core/Src/EEPROM_Program.d \
./Core/Src/SWCHES_Program.d \
./Core/Src/Servec_Program.d \
./Core/Src/main.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f0xx.d 

OBJS += \
./Core/Src/Bloutoth_Program.o \
./Core/Src/CLCD_program.o \
./Core/Src/DS1308_Program.o \
./Core/Src/EEPROM_Program.o \
./Core/Src/SWCHES_Program.o \
./Core/Src/Servec_Program.o \
./Core/Src/main.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f0xx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing -Wall -Werror -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Bloutoth_Program.cyclo ./Core/Src/Bloutoth_Program.d ./Core/Src/Bloutoth_Program.o ./Core/Src/Bloutoth_Program.su ./Core/Src/CLCD_program.cyclo ./Core/Src/CLCD_program.d ./Core/Src/CLCD_program.o ./Core/Src/CLCD_program.su ./Core/Src/DS1308_Program.cyclo ./Core/Src/DS1308_Program.d ./Core/Src/DS1308_Program.o ./Core/Src/DS1308_Program.su ./Core/Src/EEPROM_Program.cyclo ./Core/Src/EEPROM_Program.d ./Core/Src/EEPROM_Program.o ./Core/Src/EEPROM_Program.su ./Core/Src/SWCHES_Program.cyclo ./Core/Src/SWCHES_Program.d ./Core/Src/SWCHES_Program.o ./Core/Src/SWCHES_Program.su ./Core/Src/Servec_Program.cyclo ./Core/Src/Servec_Program.d ./Core/Src/Servec_Program.o ./Core/Src/Servec_Program.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f0xx_hal_msp.cyclo ./Core/Src/stm32f0xx_hal_msp.d ./Core/Src/stm32f0xx_hal_msp.o ./Core/Src/stm32f0xx_hal_msp.su ./Core/Src/stm32f0xx_it.cyclo ./Core/Src/stm32f0xx_it.d ./Core/Src/stm32f0xx_it.o ./Core/Src/stm32f0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f0xx.cyclo ./Core/Src/system_stm32f0xx.d ./Core/Src/system_stm32f0xx.o ./Core/Src/system_stm32f0xx.su

.PHONY: clean-Core-2f-Src

