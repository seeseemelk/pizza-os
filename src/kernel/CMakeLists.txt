set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu11 -nostdlib -static-libgcc -static-libstdc++ -lgcc")
file(GLOB_RECURSE SOURCES *.c *.cpp *.asm)

# Find the paths for crtbegin.o and crtend.o
execute_process(COMMAND ${CMAKE_C_COMPILER} "--print-file-name=crtbegin.o" OUTPUT_VARIABLE CRTBEGIN_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_C_COMPILER} "--print-file-name=crtend.o" OUTPUT_VARIABLE CRTEND_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBKCXX_INC} ${LIBKC_INC})

add_executable(pizzaos.elf $<TARGET_OBJECTS:crti> ${SOURCES} $<TARGET_OBJECTS:crtn>)
target_link_libraries(pizzaos.elf ${CRTBEGIN_OBJ} ${CRTEND_OBJ} kc kcxx)
add_custom_command(TARGET pizzaos.elf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pizzaos.elf> ${ISODIR}/boot
)

# Test executable
add_executable(pizzaos.test.elf EXCLUDE_FROM_ALL $<TARGET_OBJECTS:crti> ${SOURCES} $<TARGET_OBJECTS:crtn>)
target_compile_definitions(pizzaos.test.elf PUBLIC TEST=1)
target_link_libraries(pizzaos.test.elf ${CRTBEGIN_OBJ} ${CRTEND_OBJ} kc kcxx)
add_custom_command(TARGET pizzaos.test.elf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pizzaos.test.elf> ${ISODIR_TEST}/boot
)
