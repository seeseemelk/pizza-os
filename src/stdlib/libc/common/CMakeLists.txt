file(GLOB_RECURSE SOURCES *.asm *.c)
add_library(c STATIC ${SOURCES})
