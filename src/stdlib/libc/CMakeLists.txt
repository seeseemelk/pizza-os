file(GLOB_RECURSE COMMON_SOURCES common/*.c)
add_library(c STATIC ${COMMON_SOURCES})

file(GLOB_RECURSE KERNEL_SOURCES kernel/*.c)
add_library(kc STATIC ${KERNEL_SOURCES})
target_link_libraries(kc c)
