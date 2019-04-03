file(GLOB_RECURSE SOURCES *.c *.asm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBKC_INC})
add_library(c STATIC ${SOURCES})
target_link_libraries(c PUBLIC kc)