# Declare dynamic_bitset_benchmarks_base
add_executable(dynamic_bitset_benchmarks_base)

# Get sources
file(
  GLOB_RECURSE sources
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Add sources
target_sources(dynamic_bitset_benchmarks_base PRIVATE ${sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

# Add includes
target_include_directories(
  dynamic_bitset_benchmarks_base PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link dependencies
target_link_libraries(
  dynamic_bitset_benchmarks_base PRIVATE
  uscp
  git_info
  version_info
  # external
  benchmark
  sul::dynamic_bitset
)
if(HAS_BOOST)
    target_link_libraries(
      dynamic_bitset_benchmarks_base PRIVATE
      Boost::headers
    )
    target_compile_definitions(
      dynamic_bitset_benchmarks_base PRIVATE
      HAS_BOOST
    )
endif()
if(HAS_STD_TR2_DYNAMIC_BITSET)
    target_compile_definitions(
      dynamic_bitset_benchmarks_base PRIVATE
      HAS_STD_TR2_DYNAMIC_BITSET
    )
endif()

# Link filesystem library for clang and gcc < 9.0
if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND (NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
  OR (COMPILER_GCC AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)))
    target_link_libraries(dynamic_bitset_benchmarks_base PRIVATE stdc++fs)
endif()

# Build in C++20
target_compile_features(dynamic_bitset_benchmarks_base PRIVATE cxx_std_20)

# Visual studio config
get_target_property(dynamic_bitset_benchmarks_base_binary_folder dynamic_bitset_benchmarks_base RUNTIME_OUTPUT_DIRECTORY)
set_target_properties(
  dynamic_bitset_benchmarks_base PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${dynamic_bitset_benchmarks_base_binary_folder}"
)
set_property(
  DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
  VS_STARTUP_PROJECT dynamic_bitset_benchmarks_base
)
