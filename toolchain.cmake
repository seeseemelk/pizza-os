set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "i386")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER "i386-elf-gcc")
set(CMAKE_CXX_COMPILER "i386-elf-g++")

# Set flags and linker script
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffreestanding -std=gnu17 -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ffreestanding -std=gnu++17 -nostdlib -fno-exceptions -fno-rtti")
