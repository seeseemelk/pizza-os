set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu11 -nostdlib -static-libgcc")
file(GLOB_RECURSE SOURCES *.c *.cpp *.asm)
add_executable(pizzaos.elf ${SOURCES})
target_link_libraries(pizzaos.elf c cxx)

add_custom_command(TARGET pizzaos.elf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pizzaos.elf> ${ISODIR}/boot
)
