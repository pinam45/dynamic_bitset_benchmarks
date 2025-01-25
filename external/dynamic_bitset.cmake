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
message(CHECK_START "external: configuring sul::dynamic_bitset")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Get submodule directory absolute path
get_filename_component(DYNAMIC_BITSET_DIR "${CMAKE_CURRENT_LIST_DIR}/dynamic_bitset" ABSOLUTE)

# Submodule check
if(NOT EXISTS "${DYNAMIC_BITSET_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "dynamic_bitset dependency is missing, maybe you didn't pull the git submodules")
endif()

# Set options
set(BUILD_TESTING OFF)
set(DYNAMICBITSET_NO_NAMESPACE OFF)
set(DYNAMICBITSET_USE_LIBPOPCNT ON)
set(DYNAMICBITSET_USE_LIBPOPCNT_SUBMODULE ON)
set(DYNAMICBITSET_USE_STD_BITOPS ON)
set(DYNAMICBITSET_USE_COMPILER_BUILTIN ON)
set(DYNAMICBITSET_BUILD_EXAMPLE OFF)
set(DYNAMICBITSET_BUILD_TESTS OFF)
set(DYNAMICBITSET_BUILD_DOCS OFF)
set(DYNAMICBITSET_FORMAT_TARGET OFF)
set(DYNAMICBITSET_HEADERS_TARGET_IDE ON)
set(DYNAMICBITSET_INSTALL OFF)
set(DYNAMICBITSET_INSTALL_LIBPOPCNT_SUBMODULE OFF)

# Include subdirectory
add_subdirectory(${DYNAMIC_BITSET_DIR})
if(NOT TARGET dynamic_bitset)
    message(FATAL_ERROR "dynamic_bitset target is missing")
endif()

# Set IDE folder
if(TARGET dynamic_bitset_headers_for_ide)
    set_target_properties(dynamic_bitset_headers_for_ide PROPERTIES FOLDER external/dynamic_bitset)
endif()

# End message
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
