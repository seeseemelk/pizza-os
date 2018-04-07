# Initial setup
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Setup toolchain
set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Build project
project("Pizza OS" C)
enable_language(ASM_NASM)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# Add doxygen support
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

# Set flags and linker script
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -ffreestanding -std=gnu99 -nostdlib")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu99 -nostdlib -static-libgcc")

include_directories("${CMAKE_SOURCE_DIR}/src/kernel" "${CMAKE_SOURCE_DIR}/src/libc")

file(GLOB_RECURSE C_SRCS src/*.c)
file(GLOB_RECURSE ASM_SRCS src/*.asm)

add_executable(kernel.elf ${ASM_SRCS} ${C_SRCS})
target_link_libraries(kernel.elf gcc)
