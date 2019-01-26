#project(TEST01)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

file(GLOB_RECURSE FILES *.c *.cpp *.asm)
add_executable(${PROJECT_NAME} ${FILES})
