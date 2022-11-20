
# Min CMake version
cmake_minimum_required(VERSION 3.20)

idf_component_register(SRCS "BaseModule.cpp" 
							"RouterModule.cpp"
						INCLUDE_DIRS "./include"
						REQUIRES Chunk_Types
						)

