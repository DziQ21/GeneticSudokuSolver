cmake_minimum_required(VERSION 3.0.0)
project(SudokuSolver VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(-Wall -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Find all .cpp files in the src directory
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Add the executable
add_executable(SudokuSolver ${SOURCES})

# Enable testing
include(CTest)
enable_testing()

# Packaging settings
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
