#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# MSVC Asan flags
macro(custom_msvc_cxx_compiler_asan_flags var)
    set(${var}
      ################################################################
      # https://learn.microsoft.com/en-us/cpp/build/reference/fsanitize
      ################################
      # Enable AddressSanitizer
      /fsanitize=address
    )
endmacro()

# CLANG Asan flags
macro(custom_clang_cxx_compiler_asan_flags var)
    set(${var}
      ################################################################
      # https://clang.llvm.org/docs/AddressSanitizer.html
      ################################
      # Enable AddressSanitizer
      -fsanitize=address
      ################################
      # Stack Use After Return (UAR)
      -fsanitize-address-use-after-return=always
      ################################
      # Use-after-scope
      -fsanitize-address-use-after-scope
      ################################
      # To get nicer stack traces in error messages
      -fno-omit-frame-pointer
      ################################
      # To get perfect stack traces you may need to disable inlining (just use -O1) and tail call elimination
      -fno-optimize-sibling-calls
    )
    if(MINGW)
        list(FILTER ${var} EXCLUDE REGEX "-fsanitize-address-use-after-return=.*")
        list(REMOVE_ITEM ${var} "-fsanitize-address-use-after-scope")
    endif()
endmacro()

# GCC Asan flags
macro(custom_gcc_cxx_compiler_asan_flags var)
    set(${var}
      ################################################################
      # https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
      ################################
      # Enable AddressSanitizer, a fast memory error detector.
      # Memory access instructions are instrumented to detect out-of-bounds and use-after-free bugs.
      # The option enables -fsanitize-address-use-after-scope.
      # See https://github.com/google/sanitizers/wiki/AddressSanitizer for more details.
      # The run-time behavior can be influenced using the ASAN_OPTIONS environment variable.
      # When set to help=1, the available options are shown at startup of the instrumented program.
      # See https://github.com/google/sanitizers/wiki/AddressSanitizerFlags#run-time-flags for a list of supported options.
      # The option cannot be combined with -fsanitize=thread or -fsanitize=hwaddress.
      -fsanitize=address
      ################################
      # Instrument comparison operation (<, <=, >, >=) with pointer operands.
      # The option must be combined with either -fsanitize=kernel-address or -fsanitize=address The option cannot be combined with -fsanitize=thread.
      # Note: By default the check is disabled at run time.
      # To enable it, add detect_invalid_pointer_pairs=2 to the environment variable ASAN_OPTIONS.
      # Using detect_invalid_pointer_pairs=1 detects invalid operation only when both pointers are non-null.
      -fsanitize=pointer-compare
      ################################
      # Instrument subtraction with pointer operands.
      # The option must be combined with either -fsanitize=kernel-address or -fsanitize=address The option cannot be combined with -fsanitize=thread.
      # Note: By default the check is disabled at run time.
      # To enable it, add detect_invalid_pointer_pairs=2 to the environment variable ASAN_OPTIONS.
      # Using detect_invalid_pointer_pairs=1 detects invalid operation only when both pointers are non-null.
      -fsanitize=pointer-subtract
      ################################
      # Prevents optimizing sibling and tail recursive calls; this option is implicit for -O0, -O1, or -Og
      -fno-optimize-sibling-calls
      ################################
      # Disables Identical Code Folding for functions
      -fno-ipa-icf
      ################################
      # Note that -fno-omit-frame-pointer doesn't guarantee the frame pointer is used in all functions.
      # Several targets always omit the frame pointer in leaf functions.
      -fno-omit-frame-pointer
    )
endmacro()
