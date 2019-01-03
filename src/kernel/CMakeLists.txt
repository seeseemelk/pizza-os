set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -ffreestanding -std=gnu11 -nostdlib -static-libgcc -static-libstdc++ -lgcc")
file(GLOB_RECURSE SOURCES *.c *.cpp *.asm)

# Find the paths for crtbegin.o and crtend.o
execute_process(COMMAND ${CMAKE_C_COMPILER} "--print-file-name=crtbegin.o" OUTPUT_VARIABLE CRTBEGIN_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_C_COMPILER} "--print-file-name=crtend.o" OUTPUT_VARIABLE CRTEND_OBJ OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBCXX_INC} ${LIBC_INC})
#add_executable(pizzaos.elf $<TARGET_OBJECTS:crti> ${CRTBEGIN_OBJ} ${SOURCES} ${CRTEND_OBJ} $<TARGET_OBJECTS:crtn>)
add_executable(pizzaos.elf $<TARGET_OBJECTS:crti> ${SOURCES} $<TARGET_OBJECTS:crtn>)
#add_executable(pizzaos.elf ${SOURCES})
#target_link_libraries(pizzaos.elf PUBLIC c PUBLIC cxx)
#target_link_libraries(pizzaos.elf crti ${CRTBEGIN_OBJ} c cxx ${CRTEND_OBJ} crtn)
#target_link_libraries(pizzaos.elf crti crtn)
#add_dependencies(pizzaos.elf crti crtn)

#target_link_libraries(pizzaos.elf PUBLIC ${CRTBEGIN_OBJ} PUBLIC ${CRTEND_OBJ} $<TARGET_OBJECTS:crtn>)
target_link_libraries(pizzaos.elf ${CRTBEGIN_OBJ} ${CRTEND_OBJ})

add_custom_command(TARGET pizzaos.elf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pizzaos.elf> ${ISODIR}/boot
)
