cmake_minimum_required(VERSION 3.0.0)
project(SudokuSolver VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(-Wall -Werror -fopenmp)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
    find_package(OpenMP REQUIRED)
    set(OpenMP_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    set(OpenMP_CXX_LIB_NAMES ${OpenMP_CXX_LIB_NAMES})
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Find all .cpp files in the src directory
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Add the executable
add_executable(SudokuSolver ${SOURCES})

# Link OpenMP libraries if using GNU, Clang, or AppleClang
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_link_libraries(SudokuSolver PRIVATE gomp)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(SudokuSolver PRIVATE OpenMP::OpenMP_CXX)
endif()

# Enable testing
include(CTest)
enable_testing()

# Packaging settings
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)