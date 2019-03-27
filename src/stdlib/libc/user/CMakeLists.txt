file(GLOB_RECURSE SOURCES *.c *.asm)
include_directories(${LIBKC_INC})
add_library(c STATIC ${SOURCES})
target_link_libraries(c PUBLIC kc)