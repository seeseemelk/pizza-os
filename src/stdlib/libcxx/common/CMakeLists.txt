file(GLOB_RECURSE SOURCES *.cpp *.asm)
add_library(cxx STATIC ${SOURCES})
