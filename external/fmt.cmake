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
message(CHECK_START "external: configuring fmt::fmt")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Get submodule directory absolute path
get_filename_component(FMT_DIR "${CMAKE_CURRENT_LIST_DIR}/fmt" ABSOLUTE)

# Submodule check
if(NOT EXISTS "${FMT_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "expected dependency is missing, maybe you didn't pull the git submodules")
endif()

# Set options
set(BUILD_TESTING OFF)
set(FMT_PEDANTIC OFF)
set(FMT_WERROR OFF)
set(FMT_DOC OFF)
set(FMT_INSTALL OFF)
set(FMT_TEST OFF)
set(FMT_FUZZ OFF)
set(FMT_CUDA_TEST OFF)
set(FMT_OS OFF)
set(FMT_MODULE OFF)
set(FMT_SYSTEM_HEADERS OFF)
set(FMT_UNICODE OFF)

# Include subdirectory
add_subdirectory(${FMT_DIR})
if(NOT TARGET fmt::fmt)
    message(FATAL_ERROR "fmt::fmt target is missing")
endif()

# Set IDE folder
set_target_properties(expected PROPERTIES FOLDER external/expected)

# End message
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
