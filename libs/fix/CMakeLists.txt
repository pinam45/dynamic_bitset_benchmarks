# Declare lib
add_library(fix INTERFACE)

# Add includes
target_include_directories(
  fix INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Build in C++20
target_compile_features(fix INTERFACE cxx_std_20)
