# Declare dynamic_bitset_benchmarks_uscp
add_executable(dynamic_bitset_benchmarks_uscp)

# Get sources
file(
  GLOB_RECURSE sources
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Add sources
target_sources(dynamic_bitset_benchmarks_uscp PRIVATE ${sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

# Add includes
target_include_directories(
  dynamic_bitset_benchmarks_uscp PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link dependencies
target_link_libraries(
  dynamic_bitset_benchmarks_uscp PRIVATE
  uscp
  git_info
  version_info
  # external
  benchmark
  sul::dynamic_bitset
)
if(HAS_BOOST)
    target_link_libraries(
      dynamic_bitset_benchmarks_uscp PRIVATE
      Boost::headers
    )
    target_compile_definitions(
      dynamic_bitset_benchmarks_uscp PRIVATE
      HAS_BOOST
    )
endif()
if(HAS_STD_TR2_DYNAMIC_BITSET)
    target_compile_definitions(
      dynamic_bitset_benchmarks_uscp PRIVATE
      HAS_STD_TR2_DYNAMIC_BITSET
    )
endif()

# Link filesystem library for clang and gcc < 9.0
if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
  OR (COMPILER_GCC AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)))
    target_link_libraries(dynamic_bitset_benchmarks_uscp PRIVATE stdc++fs)
endif()

# Build in C++20
target_compile_features(dynamic_bitset_benchmarks_uscp PRIVATE cxx_std_20)

# Visual studio config
get_target_property(dynamic_bitset_benchmarks_uscp_binary_folder dynamic_bitset_benchmarks_uscp RUNTIME_OUTPUT_DIRECTORY)
set_target_properties(
  dynamic_bitset_benchmarks_uscp PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${dynamic_bitset_benchmarks_uscp_binary_folder}"
)
