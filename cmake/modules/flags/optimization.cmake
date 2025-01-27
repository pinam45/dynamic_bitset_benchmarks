#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# MSVC optimization flags
macro(custom_msvc_cxx_compiler_optimization_flags var)
    set(${var}
      ################################
      # https://learn.microsoft.com/en-us/cpp/build/reference/o-options-optimize-code
      /O2
      ################################
      # https://learn.microsoft.com/en-us/cpp/build/reference/q-options-low-level-operations
      /QIntel-jcc-erratum
      /Qpar
    )
endmacro()
macro(custom_msvc_cxx_linker_optimization_flags var)
    set(${var})
endmacro()

# CLANG optimization flags
macro(custom_clang_cxx_compiler_optimization_flags var)
    set(${var}
      ################################
      # https://clang.llvm.org/docs/CommandGuide/clang.html
      -O3
      -march=native
      -mtune=native
      ################################
      # https://clang.llvm.org/docs/UsersManual.html
      -fstrict-vtable-pointers # Turned off by default, because it is still experimental
      -fwhole-program-vtables # Requires -flto
      -fsplit-lto-unit
      -fforce-emit-vtables
      ################################
      # https://clang.llvm.org/docs/CommandGuide/clang.html
      -flto=full
      ################################
      # https://llvm.org/docs/FatLTO.html
      -ffat-lto-objects
    )
endmacro()
macro(custom_clang_cxx_linker_optimization_flags var)
    set(${var})
endmacro()

# GCC optimization flags
macro(custom_gcc_cxx_compiler_optimization_flags var)
    set(${var}
      ################################
      # https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html
      -O3
      -march=native
      -mtune=native
      ################################
      # https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
      -fmodulo-sched
      -fmodulo-sched-allow-regmoves
      -fgcse-sm
      -fgcse-las
      -fgcse-after-reload
      -fdevirtualize-speculatively
      -fdevirtualize-at-ltrans
      -floop-nest-optimize
      -floop-parallelize-all
      -fsplit-loops
      -funswitch-loops
      -floop-unroll-and-jam
      -flto=auto
      -fuse-linker-plugin
      -ffat-lto-objects
    )
endmacro()
macro(custom_gcc_cxx_linker_optimization_flags var)
    set(${var})
endmacro()
