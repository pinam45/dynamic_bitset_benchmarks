message(STATUS "Configuring benchmark")

get_filename_component(BENCHMARK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/benchmark ABSOLUTE)

# Submodule check
cmutils_directory_is_empty(is_empty "${BENCHMARK_DIR}")
if(is_empty)
	message(FATAL_ERROR "benchmark dependency is missing, maybe you didn't pull the git submodules")
endif()

# Include benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
set(BENCHMARK_ENABLE_LTO ON CACHE INTERNAL "")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE INTERNAL "")
if (MSVC AND (CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
	set(BENCHMARK_ENABLE_LTO OFF CACHE INTERNAL "")
endif()
add_subdirectory(${BENCHMARK_DIR})
if(NOT TARGET benchmark)
	message(FATAL_ERROR "benchmark target is missing")
endif()

# Set benchmark targets IDE folder
cmutils_target_set_ide_folder(benchmark "extlibs/benchmark")
if(TARGET benchmark_main)
	cmutils_target_set_ide_folder(benchmark_main "extlibs/benchmark")
endif()

message(STATUS "Configuring benchmark - Done")
