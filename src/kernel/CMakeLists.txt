set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu11 -nostdlib -static-libgcc -static-libstdc++ -lgcc")
file(GLOB_RECURSE SOURCES *.c *.cpp *.asm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBCXX_INC} ${LIBC_INC})
add_executable(pizzaos.elf ${SOURCES})
target_link_libraries(pizzaos.elf PUBLIC c PUBLIC cxx)

add_custom_command(TARGET pizzaos.elf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pizzaos.elf> ${ISODIR}/boot
)
