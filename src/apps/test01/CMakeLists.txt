#project(TEST01)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

file(GLOB_RECURSE SOURCES *.c *.cpp *.asm)
add_executable(${ProjectId} $<TARGET_OBJECTS:crti> ${SOURCES} $<TARGET_OBJECTS:crtn>)
target_link_libraries(${ProjectId} c cxx)
