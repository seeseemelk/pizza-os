file(GLOB_RECURSE SOURCES *.cpp *.asm)
include_directories(${LIBCXX_INC})
add_library(cxx STATIC ${SOURCES})
