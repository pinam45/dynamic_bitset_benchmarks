message(STATUS "Configuring benchmark")

get_filename_component(BENCHMARK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/benchmark ABSOLUTE)

# Submodule check
if(NOT EXISTS ${BENCHMARK_DIR}/CMakeLists.txt)
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

# Set IDE folder
set_target_properties(benchmark PROPERTIES FOLDER extlibs/benchmark)

message(STATUS "Configuring benchmark - Done")
