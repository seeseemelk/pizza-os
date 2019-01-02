set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu11 -nostdlib -static-libgcc -static-libstdc++ -lgcc")
file(GLOB_RECURSE SOURCES *.c *.cpp *.asm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBCXX_INC} ${LIBC_INC})
add_executable(pizzaos.elf $<TARGET_OBJECTS:crti> ${SOURCES})
target_link_libraries(pizzaos.elf PUBLIC c PUBLIC cxx)

execute_process(COMMAND ${CMAKE_C_COMPILER} "--print-file-name=crtbegin.o" OUTPUT_VARIABLE CRTBEGIN_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_C_COMPILER} "--print-file-name=crtend.o" OUTPUT_VARIABLE CRTEND_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)
target_link_libraries(pizzaos.elf PUBLIC ${CRTBEGIN_OBJ} PUBLIC ${CRTEND_OBJ} $<TARGET_OBJECTS:crtn>)

add_custom_command(TARGET pizzaos.elf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pizzaos.elf> ${ISODIR}/boot
)
