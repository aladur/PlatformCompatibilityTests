cmake_minimum_required(VERSION 3.18.0)

project(PlatformCompatibilityTests
        VERSION 1.0.0
        DESCRIPTION "Collection of platform independent C++ free functions and classes"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

# Prevent googletest from also being installed
option(INSTALL_GTEST OFF)

# For Windows: Prevent overriding the parent project's compiler/linker set      tings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(CMAKE_DEBUG_POSTFIX d)

if(MSVC)
    add_compile_options(/W4 /wd"4996")
endif()
if(UNIX)
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wswitch-enum
        -Wfloat-conversion
        -Wundef
        -Wconversion
        -Wsign-conversion
        -Wfloat-conversion
    )
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
