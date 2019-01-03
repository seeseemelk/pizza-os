file(GLOB_RECURSE SOURCES *.asm *.c)
add_library(kc STATIC ${SOURCES})
