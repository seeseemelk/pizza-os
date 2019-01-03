file(GLOB_RECURSE SOURCES *)
include_directories(${LIBC_INC})
add_library(cxx STATIC ${SOURCES})
target_link_libraries(cxx kcxx)
