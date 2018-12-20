file(GLOB_RECURSE SOURCES *.c *.asm)
add_library(kc STATIC ${SOURCES})
target_link_libraries(kc PUBLIC c)
