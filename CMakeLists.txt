# Initial setup
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

# Build project
project("PizzaOS" CXX C)
enable_language(ASM_NASM)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

#################################
### USER CONFIGURABLE SECTION ###
#################################
# The location at which the ISO file will be placed.
set(PIZZAOS_ISO ${CMAKE_SOURCE_DIR}/pizzaos.iso)

# The directory containing the files that should be on the ISO.
set(ISODIR ${CMAKE_SOURCE_DIR}/isodir)
set(INITRD_TAR ${ISODIR}/boot/initrd.tar)

########################################
### END OF USER CONFIGURABLE SECTION ###
########################################

## BUILD SECTION
# Include directories
set(LIBKC_INC ${PROJECT_SOURCE_DIR}/src/stdlib/libc/kernel)
set(LIBC_INC ${PROJECT_SOURCE_DIR}/src/stdlib/libc/user)
set(LIBKCXX_INC ${PROJECT_SOURCE_DIR}/src/stdlib/libcxx/kernel)
set(LIBCXX_INC ${PROJECT_SOURCE_DIR}/src/stdlib/libcxx/user)

add_subdirectory(src/stdlib)
add_subdirectory(src/kernel)
add_subdirectory(src/apps)

add_custom_command(OUTPUT ${PIZZAOS_ISO}
	COMMAND grub-mkrescue -o pizzaos.iso isodir
	DEPENDS ${ISODIR}/boot/grub/grub.cfg
	DEPENDS ${ISODIR}/boot/pizzaos.elf
	DEPENDS ${INITRD_TAR}
	COMMENT "Building ISO image"
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(build_iso ALL
	DEPENDS ${PIZZAOS_ISO}
)
add_dependencies(build_iso pizzaos.elf)
add_dependencies(build_iso build_initrd)
