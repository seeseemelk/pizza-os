# Initial setup
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# The location at which the ISO file will be placed.
set(PIZZAOS_ISO ${CMAKE_SOURCE_DIR}/pizzaos.iso)
# The directory containing the files that should be on the ISO.
set(ISODIR ${CMAKE_SOURCE_DIR}/isodir)

# Setup toolchain
set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Build project
project("PizzaOS" CXX C)
enable_language(ASM_NASM)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# Set flags and linker script
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffreestanding -std=gnu11 -nostdlib")
set(CMAKE_CPP_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffreestanding -std=gnu11 -nostdlib")

add_subdirectory(src/stdlib)
add_subdirectory(src/kernel)


#add_custom_target(pizzaos.iso ALL
add_custom_command(OUTPUT ${PIZZAOS_ISO}
	COMMAND grub-mkrescue -o pizzaos.iso isodir
	DEPENDS ${ISODIR}/boot/grub/grub.cfg
	COMMENT "Building ISO image"
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(build_iso ALL
	DEPENDS ${PIZZAOS_ISO}
)
add_dependencies(build_iso pizzaos.elf)
