# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chiragrana/projects/CSC443/CSC443-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chiragrana/projects/CSC443/CSC443-project

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/homebrew/Cellar/cmake/3.24.2/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/homebrew/Cellar/cmake/3.24.2/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/chiragrana/projects/CSC443/CSC443-project/CMakeFiles /Users/chiragrana/projects/CSC443/CSC443-project//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/chiragrana/projects/CSC443/CSC443-project/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named kvstore

# Build rule for target.
kvstore: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 kvstore
.PHONY : kvstore

# fast build rule for target.
kvstore/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/build
.PHONY : kvstore/fast

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/main.cpp.s
.PHONY : main.cpp.s

src/AVL.o: src/AVL.cpp.o
.PHONY : src/AVL.o

# target to build an object file
src/AVL.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/AVL.cpp.o
.PHONY : src/AVL.cpp.o

src/AVL.i: src/AVL.cpp.i
.PHONY : src/AVL.i

# target to preprocess a source file
src/AVL.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/AVL.cpp.i
.PHONY : src/AVL.cpp.i

src/AVL.s: src/AVL.cpp.s
.PHONY : src/AVL.s

# target to generate assembly for a file
src/AVL.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/AVL.cpp.s
.PHONY : src/AVL.cpp.s

src/Database.o: src/Database.cpp.o
.PHONY : src/Database.o

# target to build an object file
src/Database.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Database.cpp.o
.PHONY : src/Database.cpp.o

src/Database.i: src/Database.cpp.i
.PHONY : src/Database.i

# target to preprocess a source file
src/Database.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Database.cpp.i
.PHONY : src/Database.cpp.i

src/Database.s: src/Database.cpp.s
.PHONY : src/Database.s

# target to generate assembly for a file
src/Database.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Database.cpp.s
.PHONY : src/Database.cpp.s

src/Node.o: src/Node.cpp.o
.PHONY : src/Node.o

# target to build an object file
src/Node.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Node.cpp.o
.PHONY : src/Node.cpp.o

src/Node.i: src/Node.cpp.i
.PHONY : src/Node.i

# target to preprocess a source file
src/Node.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Node.cpp.i
.PHONY : src/Node.cpp.i

src/Node.s: src/Node.cpp.s
.PHONY : src/Node.s

# target to generate assembly for a file
src/Node.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Node.cpp.s
.PHONY : src/Node.cpp.s

src/SST.o: src/SST.cpp.o
.PHONY : src/SST.o

# target to build an object file
src/SST.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/SST.cpp.o
.PHONY : src/SST.cpp.o

src/SST.i: src/SST.cpp.i
.PHONY : src/SST.i

# target to preprocess a source file
src/SST.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/SST.cpp.i
.PHONY : src/SST.cpp.i

src/SST.s: src/SST.cpp.s
.PHONY : src/SST.s

# target to generate assembly for a file
src/SST.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/SST.cpp.s
.PHONY : src/SST.cpp.s

src/Storage.o: src/Storage.cpp.o
.PHONY : src/Storage.o

# target to build an object file
src/Storage.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Storage.cpp.o
.PHONY : src/Storage.cpp.o

src/Storage.i: src/Storage.cpp.i
.PHONY : src/Storage.i

# target to preprocess a source file
src/Storage.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Storage.cpp.i
.PHONY : src/Storage.cpp.i

src/Storage.s: src/Storage.cpp.s
.PHONY : src/Storage.s

# target to generate assembly for a file
src/Storage.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/Storage.cpp.s
.PHONY : src/Storage.cpp.s

src/utils.o: src/utils.cpp.o
.PHONY : src/utils.o

# target to build an object file
src/utils.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/utils.cpp.o
.PHONY : src/utils.cpp.o

src/utils.i: src/utils.cpp.i
.PHONY : src/utils.i

# target to preprocess a source file
src/utils.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/utils.cpp.i
.PHONY : src/utils.cpp.i

src/utils.s: src/utils.cpp.s
.PHONY : src/utils.s

# target to generate assembly for a file
src/utils.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/kvstore.dir/build.make CMakeFiles/kvstore.dir/src/utils.cpp.s
.PHONY : src/utils.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... kvstore"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... src/AVL.o"
	@echo "... src/AVL.i"
	@echo "... src/AVL.s"
	@echo "... src/Database.o"
	@echo "... src/Database.i"
	@echo "... src/Database.s"
	@echo "... src/Node.o"
	@echo "... src/Node.i"
	@echo "... src/Node.s"
	@echo "... src/SST.o"
	@echo "... src/SST.i"
	@echo "... src/SST.s"
	@echo "... src/Storage.o"
	@echo "... src/Storage.i"
	@echo "... src/Storage.s"
	@echo "... src/utils.o"
	@echo "... src/utils.i"
	@echo "... src/utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

