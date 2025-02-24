#
# Copyright (c) 2025 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

# About OR-Library
# OR-Library is a collection of test data sets for a variety of OR problems
# https://people.brunel.ac.uk/~mastjjb/jeb/orlib/scpinfo.html

# Default values
set(DEFAULT_OR_LIBRARY_FILES
  scpcyc06.txt
  scpcyc07.txt
  scpcyc08.txt
  scpcyc09.txt
  scpcyc10.txt
  scpcyc11.txt
)
set(DEFAULT_OR_LIBRARY_BASE_URL "https://people.brunel.ac.uk/~mastjjb/jeb/orlib/files")
set(DEFAULT_OR_LIBRARY_DEST_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Options
set(OR_LIBRARY_FILES "${DEFAULT_OR_LIBRARY_FILES}" CACHE STRING "OR-Library files to get")
set(OR_LIBRARY_BASE_URL "${DEFAULT_OR_LIBRARY_BASE_URL}" CACHE STRING "OR-Library base URL for downloading files")
set(OR_LIBRARY_DEST_DIR "${DEFAULT_OR_LIBRARY_DEST_DIR}" CACHE STRING "OR-Library destination directory")

# Check destination directory
get_filename_component(dest_dir "${OR_LIBRARY_DEST_DIR}" ABSOLUTE)
if(NOT EXISTS ${dest_dir} OR NOT IS_DIRECTORY ${dest_dir})
    message(FATAL_ERROR "${dest_dir}: does not exist or is not a directory")
endif()

# Download files
foreach(file ${OR_LIBRARY_FILES})
    set(dest_file "${dest_dir}/${file}")
    if(EXISTS ${dest_file})
        message(STATUS "${file}: already exists, skipping download.")
    else()
        file(DOWNLOAD "${OR_LIBRARY_BASE_URL}/${file}" "${dest_file}" SHOW_PROGRESS)
        if(EXISTS "${dest_file}")
            message(STATUS "${file}: downloaded successfully")
        else()
            message(WARNING "${file}: downloaded failed")
        endif()
    endif()
endforeach()
