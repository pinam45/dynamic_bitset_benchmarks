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
message(CHECK_START "external: configuring tl::expected")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Get submodule directory absolute path
get_filename_component(EXPECTED_DIR "${CMAKE_CURRENT_LIST_DIR}/expected" ABSOLUTE)

# Submodule check
if(NOT EXISTS "${EXPECTED_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "expected dependency is missing, maybe you didn't pull the git submodules")
endif()

# Set options
set(BUILD_TESTING OFF)
set(EXPECTED_BUILD_PACKAGE OFF)
set(EXPECTED_BUILD_TESTS OFF)
set(EXPECTED_BUILD_PACKAGE_DEB OFF)

# Include subdirectory
add_subdirectory(${EXPECTED_DIR})
if(NOT TARGET tl::expected)
    message(FATAL_ERROR "tl::expected target is missing")
endif()

# Set IDE folder
set_target_properties(expected PROPERTIES FOLDER external/expected)

# End message
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
