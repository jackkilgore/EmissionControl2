# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jkilgore/Applications/allo/allolib/cmake/single_file

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/build/granular/Release

# Utility rule file for granular_run.

# Include the progress variables for this target.
include CMakeFiles/granular_run.dir/progress.make

CMakeFiles/granular_run: /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/bin/granular
	cd /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/bin && ./granular

granular_run: CMakeFiles/granular_run
granular_run: CMakeFiles/granular_run.dir/build.make

.PHONY : granular_run

# Rule to build all files generated by this target.
CMakeFiles/granular_run.dir/build: granular_run

.PHONY : CMakeFiles/granular_run.dir/build

CMakeFiles/granular_run.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/granular_run.dir/cmake_clean.cmake
.PHONY : CMakeFiles/granular_run.dir/clean

CMakeFiles/granular_run.dir/depend:
	cd /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/build/granular/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jkilgore/Applications/allo/allolib/cmake/single_file /Users/jkilgore/Applications/allo/allolib/cmake/single_file /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/build/granular/Release /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/build/granular/Release /Users/jkilgore/Applications/allo/EmissionControlPort/dsp/alloTests/build/granular/Release/CMakeFiles/granular_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/granular_run.dir/depend
