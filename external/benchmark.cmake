#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# Start message
message(CHECK_START "external: configuring benchmark::benchmark")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Get submodule directory absolute path
get_filename_component(BENCHMARK_DIR "${CMAKE_CURRENT_LIST_DIR}/benchmark" ABSOLUTE)

# Submodule check
if(NOT EXISTS "${BENCHMARK_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "benchmark dependency is missing, maybe you didn't pull the git submodules")
endif()

# Set options
set(BUILD_TESTING OFF)
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_EXCEPTIONS ON)

set(BENCHMARK_ENABLE_LTO OFF)
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(is_multi_config)
    if(CMAKE_CONFIGURATION_TYPES)
        set(build_types ${CMAKE_CONFIGURATION_TYPES})
        list(TRANSFORM build_types TOUPPER)
        if("RELEASE" IN_LIST build_types)
            set(BENCHMARK_ENABLE_LTO ON)
        endif()
    endif()
else()
    if(CMAKE_BUILD_TYPE)
        string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
        if(build_type STREQUAL "RELEASE")
            set(BENCHMARK_ENABLE_LTO ON)
        endif()
    endif()
endif()

set(BENCHMARK_USE_LIBCXX OFF)
set(BENCHMARK_ENABLE_WERROR OFF)
set(BENCHMARK_FORCE_WERROR OFF)
set(BENCHMARK_BUILD_32_BITS OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BENCHMARK_ENABLE_DOXYGEN OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_USE_BUNDLED_GTEST OFF)
set(BENCHMARK_ENABLE_LIBPFM OFF)
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF)

# Include subdirectory
add_subdirectory(${BENCHMARK_DIR})
if(NOT TARGET benchmark::benchmark)
    message(FATAL_ERROR "benchmark::benchmark target is missing")
endif()

# Set IDE folder
set_target_properties(benchmark PROPERTIES FOLDER external/benchmark)

# End message
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
