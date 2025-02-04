# Min CMake version
cmake_minimum_required(VERSION 3.10)
project(BaseModuleProject CXX)

# Set directory vars
set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")
set(SOURCE_TESTS_DIR "source_tests")
set(INCLUDE_TESTS_DIR "include_tests")

message(STATUS "Building for Unix")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Main executable
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
add_library(BaseModuleLib
    ${SOURCES}
)

add_subdirectory(components/Chunk_Types)
add_subdirectory(components/plog)

# Include directories for main code and tests
target_include_directories(BaseModuleLib
    PRIVATE ${INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Chunk_Types/components/Nlohmann_JSON/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Chunk_Types/components/Nlohmann_JSON/include/nlohmann
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Chunk_Types/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/plog/include
)

# Adding threads
target_link_libraries(BaseModuleLib PRIVATE pthread)
target_link_libraries(BaseModuleLib PRIVATE ${THREADS_LIBRARIES})

# Link with required libraries for the main executable
target_link_libraries(BaseModuleLib PRIVATE ChunkTypesLib doctest::doctest)

# Enable testing
enable_testing()

# Add tests to CTest
add_test(NAME BaseModuleTest COMMAND BaseModuleTest)
