# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /opt/clion-2018.3.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2018.3.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/seba/TUHH/projects/intairnet/arq/dev

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/dev.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dev.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dev.dir/flags.make

CMakeFiles/dev.dir/library.cpp.o: CMakeFiles/dev.dir/flags.make
CMakeFiles/dev.dir/library.cpp.o: ../library.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dev.dir/library.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dev.dir/library.cpp.o -c /home/seba/TUHH/projects/intairnet/arq/dev/library.cpp

CMakeFiles/dev.dir/library.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dev.dir/library.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/seba/TUHH/projects/intairnet/arq/dev/library.cpp > CMakeFiles/dev.dir/library.cpp.i

CMakeFiles/dev.dir/library.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dev.dir/library.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/seba/TUHH/projects/intairnet/arq/dev/library.cpp -o CMakeFiles/dev.dir/library.cpp.s

# Object files for target dev
dev_OBJECTS = \
"CMakeFiles/dev.dir/library.cpp.o"

# External object files for target dev
dev_EXTERNAL_OBJECTS =

libdev.so: CMakeFiles/dev.dir/library.cpp.o
libdev.so: CMakeFiles/dev.dir/build.make
libdev.so: CMakeFiles/dev.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libdev.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dev.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dev.dir/build: libdev.so

.PHONY : CMakeFiles/dev.dir/build

CMakeFiles/dev.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dev.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dev.dir/clean

CMakeFiles/dev.dir/depend:
	cd /home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/seba/TUHH/projects/intairnet/arq/dev /home/seba/TUHH/projects/intairnet/arq/dev /home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug /home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug /home/seba/TUHH/projects/intairnet/arq/dev/cmake-build-debug/CMakeFiles/dev.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dev.dir/depend

