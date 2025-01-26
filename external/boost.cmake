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
message(CHECK_START "external: configuring Boost")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

find_package(Boost CONFIG)
if(Boost_FOUND)
    message(STATUS "Boost ${Boost_VERSION} found")
    message(STATUS "include: ${Boost_INCLUDE_DIRS}")
    message(STATUS "library: ${Boost_LIBRARY_DIRS}")
    set(HAS_BOOST ON)
else()
    message(WARNING "Boost not found, disabled from benchmarks")
    set(HAS_BOOST OFF)
endif()

# End message
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
