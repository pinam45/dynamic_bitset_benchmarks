# Declare lib
add_library(uscp INTERFACE)

# Add includes
target_include_directories(
  uscp INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link dependencies
target_link_libraries(
  uscp INTERFACE
  fix
  # external
  tl::expected
  fmt::fmt
)

# Build in C++20
target_compile_features(uscp INTERFACE cxx_std_20)
