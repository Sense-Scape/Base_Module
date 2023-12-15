# Min CMake version
cmake_minimum_required(VERSION 3.18)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")

idf_component_register(
  SRCS
    ${SOURCE_DIR}/BaseModule.cpp
    ${SOURCE_DIR}/RouterModule.cpp
  INCLUDE_DIRS
    ${INCLUDE_DIR}
  REQUIRES
    Chunk_Types
)
