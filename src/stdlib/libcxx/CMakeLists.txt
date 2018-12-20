file(GLOB_RECURSE COMMON_SOURCES common/*.c common/*.cpp)
add_library(cxx STATIC ${COMMON_SOURCES})

file(GLOB_RECURSE KERNEL_SOURCES kernel/*.c kernel/*.cpp)
add_library(kcxx STATIC ${KERNEL_SOURCES})
target_link_libraries(kcxx cxx)
