# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /mnt/v/r3dapple.github.io/encryption/mhcl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/v/r3dapple.github.io/encryption/mhcl/build

# Include any dependencies generated for this target.
include test/CMakeFiles/biginteger_mult_test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/biginteger_mult_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/biginteger_mult_test.dir/flags.make

test/CMakeFiles/biginteger_mult_test.dir/test.cpp.o: test/CMakeFiles/biginteger_mult_test.dir/flags.make
test/CMakeFiles/biginteger_mult_test.dir/test.cpp.o: ../test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/v/r3dapple.github.io/encryption/mhcl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/biginteger_mult_test.dir/test.cpp.o"
	cd /mnt/v/r3dapple.github.io/encryption/mhcl/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/biginteger_mult_test.dir/test.cpp.o -c /mnt/v/r3dapple.github.io/encryption/mhcl/test/test.cpp

test/CMakeFiles/biginteger_mult_test.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/biginteger_mult_test.dir/test.cpp.i"
	cd /mnt/v/r3dapple.github.io/encryption/mhcl/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/v/r3dapple.github.io/encryption/mhcl/test/test.cpp > CMakeFiles/biginteger_mult_test.dir/test.cpp.i

test/CMakeFiles/biginteger_mult_test.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/biginteger_mult_test.dir/test.cpp.s"
	cd /mnt/v/r3dapple.github.io/encryption/mhcl/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/v/r3dapple.github.io/encryption/mhcl/test/test.cpp -o CMakeFiles/biginteger_mult_test.dir/test.cpp.s

# Object files for target biginteger_mult_test
biginteger_mult_test_OBJECTS = \
"CMakeFiles/biginteger_mult_test.dir/test.cpp.o"

# External object files for target biginteger_mult_test
biginteger_mult_test_EXTERNAL_OBJECTS =

test/biginteger_mult_test: test/CMakeFiles/biginteger_mult_test.dir/test.cpp.o
test/biginteger_mult_test: test/CMakeFiles/biginteger_mult_test.dir/build.make
test/biginteger_mult_test: src/libbiginteger.a
test/biginteger_mult_test: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
test/biginteger_mult_test: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
test/biginteger_mult_test: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.so.1.71.0
test/biginteger_mult_test: test/CMakeFiles/biginteger_mult_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/v/r3dapple.github.io/encryption/mhcl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable biginteger_mult_test"
	cd /mnt/v/r3dapple.github.io/encryption/mhcl/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/biginteger_mult_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/biginteger_mult_test.dir/build: test/biginteger_mult_test

.PHONY : test/CMakeFiles/biginteger_mult_test.dir/build

test/CMakeFiles/biginteger_mult_test.dir/clean:
	cd /mnt/v/r3dapple.github.io/encryption/mhcl/build/test && $(CMAKE_COMMAND) -P CMakeFiles/biginteger_mult_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/biginteger_mult_test.dir/clean

test/CMakeFiles/biginteger_mult_test.dir/depend:
	cd /mnt/v/r3dapple.github.io/encryption/mhcl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/v/r3dapple.github.io/encryption/mhcl /mnt/v/r3dapple.github.io/encryption/mhcl/test /mnt/v/r3dapple.github.io/encryption/mhcl/build /mnt/v/r3dapple.github.io/encryption/mhcl/build/test /mnt/v/r3dapple.github.io/encryption/mhcl/build/test/CMakeFiles/biginteger_mult_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/biginteger_mult_test.dir/depend

