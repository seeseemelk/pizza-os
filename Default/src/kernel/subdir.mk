################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/kernel/boot.asm 

C_SRCS += \
../src/kernel/main.c 

OBJS += \
./src/kernel/boot.o \
./src/kernel/main.o 

C_DEPS += \
./src/kernel/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/kernel/%.o: ../src/kernel/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	i686-elf-as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/kernel/%.o: ../src/kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i686-elf-gcc -std=c99 -O2 -g -pedantic -pedantic-errors -Wall -Wextra -Werror -Wconversion -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


