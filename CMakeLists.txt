cmake_minimum_required(VERSION 3.10) # For CXX_STANDARD 17 property on Visual Studio

# Project declaration
project(
	dynamic_bitset_benchmarks
	DESCRIPTION "C++ dynamic bitset benchmarks "
	HOMEPAGE_URL "https://github.com/pinam45/dynamic_bitset_benchmarks"
	LANGUAGES CXX
)

# By default build in Release mode
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release, RelWithDebInfo or Debug" FORCE)
endif()

# CMake utilities
get_filename_component(CMUTILS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMutils/cmutils.cmake" ABSOLUTE)
if(EXISTS "${CMUTILS_PATH}")
	include(${CMUTILS_PATH})
else()
	message(FATAL_ERROR "CMutils is missing, maybe you didn't pull the git submodules")
endif()

# Global config
cmutils_disable_in_sources_build()
cmutils_enable_ide_folders()
cmutils_set_classic_output_folders()

# Libraries
set(BUILD_TESTING OFF CACHE INTERNAL "")
include(cmake/benchmark.cmake)
include(cmake/dynamic_bitset.cmake)

# Declare dynamic_bitset_benchmarks
add_executable(dynamic_bitset_benchmarks)

# Add sources
cmutils_target_sources_folders(
	dynamic_bitset_benchmarks PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Add includes
target_include_directories(
	dynamic_bitset_benchmarks PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link dependencies
target_link_libraries(
	dynamic_bitset_benchmarks PRIVATE
	benchmark
	dynamic_bitset
)

# Link filesystem library for clang and gcc < 9.0
cmutils_define_compiler_variables()
if(COMPILER_CLANG OR (COMPILER_GCC AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)))
	target_link_libraries(dynamic_bitset_benchmarks PRIVATE stdc++fs)
endif()

# Configure compile options
cmutils_target_configure_compile_options(dynamic_bitset_benchmarks)

# Enable cmutils default warnings
cmutils_target_enable_warnings(dynamic_bitset_benchmarks)

# Build in C++17
cmutils_target_set_standard(dynamic_bitset_benchmarks CXX 17)

# Set static runtime
#cmutils_target_set_runtime(dynamic_bitset_benchmarks STATIC)

# Set target IDE folder
cmutils_target_set_ide_folder(dynamic_bitset_benchmarks "dynamic_bitset_benchmarks")

# Group sources for IDEs
cmutils_target_source_group(dynamic_bitset_benchmarks "${CMAKE_CURRENT_SOURCE_DIR}")

# Visual studio config
get_target_property(dynamic_bitset_benchmarks_binary_folder dynamic_bitset_benchmarks RUNTIME_OUTPUT_DIRECTORY)
set_target_properties(
	dynamic_bitset_benchmarks PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${dynamic_bitset_benchmarks_binary_folder}"
)
set_property(
	DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
	VS_STARTUP_PROJECT dynamic_bitset_benchmarks
)

# Use ccache
cmutils_target_use_ccache(dynamic_bitset_benchmarks)

# Generate format target
cmutils_target_generate_clang_format(dynamic_bitset_benchmarks)

# Verbose makefile
#set(CMAKE_VERBOSE_MAKEFILE ON)
