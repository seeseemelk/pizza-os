cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("Pizza OS" C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

enable_language(ASM_NASM)
file(GLOB C_SRCS src/*.c src/*.asm)
add_executable(kernel ${C_SRCS})

set_target_flags(kernel PROPERTIES LINK_FLAGS "")
