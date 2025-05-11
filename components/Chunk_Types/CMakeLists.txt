cmake_minimum_required(VERSION 3.10)
project(ChunkTypesProject CXX)

# Set directory vars
set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")
set(SOURCE_TESTS_DIR "source_tests")
set(INCLUDE_TESTS_DIR "include_tests")

message(STATUS "Generating files to build Chunks for UNIX")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
message(STATUS "Chunk Types Setting C++ standard to ${CMAKE_CXX_STANDARD}")

# Main executable
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
add_library(ChunkTypesLib
    ${SOURCES}
)

# Test executable
add_executable(ChunkTypesTests
    ${SOURCE_TESTS_DIR}/ChunkTypesTest.cpp 
)

# Include directories for main code and tests
target_include_directories(ChunkTypesLib
    PRIVATE ${INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Nlohmann_JSON/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Nlohmann_JSON/include/nlohmann
)

target_include_directories(ChunkTypesTests
    PRIVATE ${INCLUDE_DIR}
    PRIVATE ${INCLUDE_TESTS_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/doctest/doctest
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Nlohmann_JSON/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/components/Nlohmann_JSON/include/nlohmann
)

# Add components (even if no CMakeLists.txt)
add_subdirectory(components/doctest)

# Link with required libraries for the main executable
target_link_libraries(ChunkTypesLib PRIVATE doctest::doctest)

# Link with required libraries for the test executable
target_link_libraries(ChunkTypesTests PRIVATE ChunkTypesLib doctest::doctest)

# Enable testing
enable_testing()

# Add tests to CTest
add_test(NAME ChunkTypesTests COMMAND ChunkTypesTests)

