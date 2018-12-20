file(GLOB_RECURSE SOURCES *.cpp)
include_directories(${LIBC_INC})
add_library(kcxx STATIC ${SOURCES})
target_link_libraries(kcxx cxx)
