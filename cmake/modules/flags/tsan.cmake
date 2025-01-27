#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# MSVC Tsan flags
macro(custom_msvc_cxx_compiler_tsan_flags var)
    # Not supported yet: https://learn.microsoft.com/en-us/cpp/sanitizers
    set(${var})
endmacro()

# CLANG Tsan flags
macro(custom_clang_cxx_compiler_tsan_flags var)
    set(${var}
      ################################################################
      # https://clang.llvm.org/docs/ThreadSanitizer.html
      ################################
      # Enable ThreadSanitizer
      -fsanitize=thread
      ################################
      # To get nicer stack traces in error messages
      -fno-omit-frame-pointer
    )
endmacro()

# GCC Tsan flags
macro(custom_gcc_cxx_compiler_tsan_flags var)
    set(${var}
      ################################################################
      # https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
      ################################
      # Enable ThreadSanitizer
      -fsanitize=thread
      ################################
      # To get nicer stack traces in error messages
      -fno-omit-frame-pointer
    )
endmacro()
