# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/seeseemelk/Eclipse/pizza_os

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/seeseemelk/Eclipse/pizza_os/build

# Include any dependencies generated for this target.
include CMakeFiles/kernel.bin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kernel.bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kernel.bin.dir/flags.make

CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj: CMakeFiles/kernel.bin.dir/flags.make
CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj: ../src/kernel/boot.asm
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/seeseemelk/Eclipse/pizza_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM_NASM object CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj"
	/usr/bin/nasm $(ASM_NASM_INCLUDES) $(ASM_NASM_FLAGS) -f elf -o CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj /home/seeseemelk/Eclipse/pizza_os/src/kernel/boot.asm

CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.requires:

.PHONY : CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.requires

CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.provides: CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.requires
	$(MAKE) -f CMakeFiles/kernel.bin.dir/build.make CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.provides.build
.PHONY : CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.provides

CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.provides.build: CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj


CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj: CMakeFiles/kernel.bin.dir/flags.make
CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj: ../src/kernel/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/seeseemelk/Eclipse/pizza_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj"
	/home/seeseemelk/opt/cross/bin/i686-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj   -c /home/seeseemelk/Eclipse/pizza_os/src/kernel/main.c

CMakeFiles/kernel.bin.dir/src/kernel/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kernel.bin.dir/src/kernel/main.c.i"
	/home/seeseemelk/opt/cross/bin/i686-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/seeseemelk/Eclipse/pizza_os/src/kernel/main.c > CMakeFiles/kernel.bin.dir/src/kernel/main.c.i

CMakeFiles/kernel.bin.dir/src/kernel/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kernel.bin.dir/src/kernel/main.c.s"
	/home/seeseemelk/opt/cross/bin/i686-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/seeseemelk/Eclipse/pizza_os/src/kernel/main.c -o CMakeFiles/kernel.bin.dir/src/kernel/main.c.s

CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.requires:

.PHONY : CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.requires

CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.provides: CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.requires
	$(MAKE) -f CMakeFiles/kernel.bin.dir/build.make CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.provides.build
.PHONY : CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.provides

CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.provides.build: CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj


# Object files for target kernel.bin
kernel_bin_OBJECTS = \
"CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj" \
"CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj"

# External object files for target kernel.bin
kernel_bin_EXTERNAL_OBJECTS =

kernel.bin: CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj
kernel.bin: CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj
kernel.bin: CMakeFiles/kernel.bin.dir/build.make
kernel.bin: CMakeFiles/kernel.bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/seeseemelk/Eclipse/pizza_os/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable kernel.bin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kernel.bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kernel.bin.dir/build: kernel.bin

.PHONY : CMakeFiles/kernel.bin.dir/build

CMakeFiles/kernel.bin.dir/requires: CMakeFiles/kernel.bin.dir/src/kernel/boot.asm.obj.requires
CMakeFiles/kernel.bin.dir/requires: CMakeFiles/kernel.bin.dir/src/kernel/main.c.obj.requires

.PHONY : CMakeFiles/kernel.bin.dir/requires

CMakeFiles/kernel.bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kernel.bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kernel.bin.dir/clean

CMakeFiles/kernel.bin.dir/depend:
	cd /home/seeseemelk/Eclipse/pizza_os/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/seeseemelk/Eclipse/pizza_os /home/seeseemelk/Eclipse/pizza_os /home/seeseemelk/Eclipse/pizza_os/build /home/seeseemelk/Eclipse/pizza_os/build /home/seeseemelk/Eclipse/pizza_os/build/CMakeFiles/kernel.bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kernel.bin.dir/depend

