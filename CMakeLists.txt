#
# Copyright (c) 2025 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

# Project declaration
project(
  dynamic_bitset_benchmarks
  VERSION 0.0.1
  DESCRIPTION "C++ dynamic bitset benchmarks "
  HOMEPAGE_URL "https://github.com/pinam45/dynamic_bitset_benchmarks"
  LANGUAGES CXX
)

# Set CMake modules path
list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

# Include modules
include(flags)
include(git_info)
include(version_info)

# By default build in Release mode
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Debug, Release, Asan, UBsan, Tsan, Sanitize" FORCE)
endif()

# setup build types
setup_common_flags()
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(is_multi_config)
    if(CMAKE_CONFIGURATION_TYPES)
        set(build_types ${CMAKE_CONFIGURATION_TYPES})
        list(TRANSFORM build_types TOUPPER)
        if("RELEASE" IN_LIST build_types)
            setup_optimization_flags()
        elseif("ASAN" IN_LIST build_types)
            setup_asan_flags()
        elseif("UBSAN" IN_LIST build_types)
            setup_ubsan_flags()
        elseif("TSAN" IN_LIST build_types)
            setup_tsan_flags()
        elseif("SANITIZE" IN_LIST build_types)
            setup_sanitize_flags()
        endif()
    endif()
else()
    if(CMAKE_BUILD_TYPE)
        string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
        if(build_type STREQUAL "RELEASE")
            setup_optimization_flags()
        elseif(build_type STREQUAL "ASAN")
            setup_asan_flags()
        elseif(build_type STREQUAL "UBSAN")
            setup_ubsan_flags()
        elseif(build_type STREQUAL "TSAN")
            setup_tsan_flags()
        elseif(build_type STREQUAL "SANITIZE")
            setup_sanitize_flags()
        endif()
    endif()
endif()

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Use strict C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Use strict C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for external tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# IDE folders config
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")

# Seems like you need to explicitly enable exceptions with clan-cl
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    message(CHECK_START "Add /EHsc flag to to explicitly enable exceptions with clan-cl")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    custom_add_base_flags(CXX_FLAGS "/EHsc")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endif()

# Dependencies
include(external/benchmark.cmake)
include(external/dynamic_bitset.cmake)
include(external/boost.cmake)
include(external/std_tr2_dynamic_bitset.cmake)

# Libs
add_subdirectory(libs)

# Benchmarks
add_subdirectory(benchmarks)

# Tools
add_subdirectory(tools)

# Verbose makefile
#set(CMAKE_VERBOSE_MAKEFILE ON)
