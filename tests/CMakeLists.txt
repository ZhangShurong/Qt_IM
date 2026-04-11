# Qt_IM Unit Tests CMake Configuration
# Minimal test build for core logic (no Qt dependency required)

cmake_minimum_required(VERSION 3.10)
project(Qt_IM_Tests VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(run_tests
    test_framework.h
    test_main.cpp
)

enable_testing()
add_test(NAME UnitTests COMMAND run_tests)
