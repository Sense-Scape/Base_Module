# CMake instructions to make the static lib

# Min CMake version
cmake_minimum_required(VERSION 3.24)

# Project name and langues (C, C++, Assembly)
project(SC_Base_Module_Types C CXX ASM)

# Adding Chunk Types to modules
find_library(SC_Chunk_Types
			PUBLIC
			$ENV{SC_LIB_DIR}/Lib_Chunk_Types/CPP_Chunk_Types/build
			)
include_directories(SC_Chunk_Types
				INTERFACE
				$ENV{SC_LIB_DIR}/Lib_Chunk_Types/CPP_Chunk_Types
				)

# What to add to library
add_library(SC_Base_Module_Types 
			STATIC
			CircularBuffer.h
			ModuleTypes.h
			BaseModule.h
			BaseModule.cpp
			RouterModule.h
			RouterModule.cpp
			)

# Adding sources to library
target_sources(SC_Base_Module_Types
				PUBLIC 
				CircularBuffer.h
				ModuleTypes.h
				BaseModule.h
				BaseModule.cpp
				RouterModule.h
				RouterModule.cpp
				)