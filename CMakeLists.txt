# Initial setup
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Setup toolchain
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "i686")

set(CMAKE_C_COMPILER "i686-elf-gcc")
set(CMAKE_CXX_COMPILER "i686-elf-gcc")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set flags and linker script
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -ffreestanding -std=gnu99 -nostdlib -lgcc")

# Build project
project("Pizza OS" C)
enable_language(ASM_NASM)
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu99 -nostdlib -lgcc")

file(GLOB C_SRCS src/**/*.c)
file(GLOB ASM_SRCS src/**/*.asm)

add_executable(kernel.bin ${ASM_SRCS} ${C_SRCS})
