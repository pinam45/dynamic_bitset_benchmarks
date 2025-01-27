#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# MSVC common flags
macro(custom_msvc_cxx_compiler_common_flags var)
    set(${var})
endmacro()
macro(custom_msvc_cxx_linker_common_flags var)
    set(${var})
endmacro()

# CLANG common flags
macro(custom_clang_cxx_compiler_common_flags var)
    set(${var})
endmacro()
macro(custom_clang_cxx_linker_common_flags var)
    set(${var}
      ################################
      # https://clang.llvm.org/docs/Toolchain.html
      # https://clang.llvm.org/docs/ClangCommandLineReference.html
      # going full LLVM
      # Linker
      -fuse-ld=lld # $ sudo apt install lld
      # C++ standard library
      #-lc++ # $ sudo apt install libc++-dev
      # C++ ABI library
      #-lc++abi # $ sudo apt install libc++abi-dev
      # Compiler runtime
      #-rtlib=compiler-rt
      # Unwind library
      #-unwindlib=unwindlib # $ sudo apt install libunwind-dev
    )
endmacro()

# GCC common flags
macro(custom_gcc_cxx_compiler_common_flags var)
    set(${var})
endmacro()
macro(custom_gcc_cxx_linker_common_flags var)
    set(${var})
endmacro()
