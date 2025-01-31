#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# Dependencies
include(${CMAKE_CURRENT_LIST_DIR}/flags/tools.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/flags/common.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/flags/asan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/flags/ubsan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/flags/tsan.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/flags/optimization.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/flags/warning.cmake)

function(setup_common_flags)
    message(CHECK_START "Setup CXX common flags")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # Select common flags based on the compiler
    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_common_flags(COMPILER_FLAGS)
        custom_msvc_cxx_linker_common_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_common_flags(COMPILER_FLAGS)
        custom_msvc_cxx_linker_common_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_common_flags(COMPILER_FLAGS)
        custom_clang_cxx_linker_common_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_common_flags(COMPILER_FLAGS)
        custom_clang_cxx_linker_common_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_common_flags(COMPILER_FLAGS)
        custom_gcc_cxx_linker_common_flags(LINKER_FLAGS)
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Choose try_compile build configuration
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(is_multi_config)
        set(CMAKE_TRY_COMPILE_CONFIGURATION "Release")
    endif()

    # Check compiler flags
    set(CHECKED_COMPILER_FLAGS)
    foreach(flag ${COMPILER_FLAGS})
        custom_check_cxx_compiler_linker_flag(${flag} res)
        if(res)
            set(CHECKED_COMPILER_FLAGS "${CHECKED_COMPILER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Check linker flags
    set(CHECKED_LINKER_FLAGS)
    foreach(flag ${LINKER_FLAGS})
        custom_check_cxx_linker_flag(${flag} res)
        if(res)
            set(CHECKED_LINKER_FLAGS "${CHECKED_LINKER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Add flags
    custom_add_base_flags(
      COMPILER_FLAGS ${CHECKED_COMPILER_FLAGS}
      LINKER_FLAGS ${CHECKED_LINKER_FLAGS}
    )

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()

function(setup_asan_flags)
    message(CHECK_START "Setup CXX Asan flags")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # Select Asan flags based on the compiler
    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_asan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_asan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_asan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_asan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_asan_flags(COMPILER_FLAGS)
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Choose try_compile build configuration
    set(CMAKE_TRY_COMPILE_CONFIGURATION "Debug")

    # Check compiler flags
    set(CHECKED_COMPILER_FLAGS)
    foreach(flag ${COMPILER_FLAGS})
        custom_check_cxx_compiler_linker_flag(${flag} res)
        if(res)
            set(CHECKED_COMPILER_FLAGS "${CHECKED_COMPILER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Set flags
    custom_build_type_set_base_flags(ASAN
      INIT_FROM DEBUG
      COMPILER_FLAGS ${CHECKED_COMPILER_FLAGS}
    )

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()

function(setup_ubsan_flags)
    message(CHECK_START "Setup CXX UBsan flags")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # Select UBsan flags based on the compiler
    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_ubsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_ubsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_ubsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_ubsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_ubsan_flags(COMPILER_FLAGS)
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Choose try_compile build configuration
    set(CMAKE_TRY_COMPILE_CONFIGURATION "Debug")

    # Check compiler flags
    set(CHECKED_COMPILER_FLAGS)
    foreach(flag ${COMPILER_FLAGS})
        custom_check_cxx_compiler_linker_flag(${flag} res)
        if(res)
            set(CHECKED_COMPILER_FLAGS "${CHECKED_COMPILER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Set flags
    custom_build_type_set_base_flags(UBSAN
      INIT_FROM DEBUG
      COMPILER_FLAGS ${CHECKED_COMPILER_FLAGS}
    )

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()

function(setup_tsan_flags)
    message(CHECK_START "Setup CXX Tsan flags")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # Select Tsan flags based on the compiler
    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_tsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_tsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_tsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_tsan_flags(COMPILER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_tsan_flags(COMPILER_FLAGS)
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Choose try_compile build configuration
    set(CMAKE_TRY_COMPILE_CONFIGURATION "Debug")

    # Check compiler flags
    set(CHECKED_COMPILER_FLAGS)
    foreach(flag ${COMPILER_FLAGS})
        custom_check_cxx_compiler_linker_flag(${flag} res)
        if(res)
            set(CHECKED_COMPILER_FLAGS "${CHECKED_COMPILER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Set flags
    custom_build_type_set_base_flags(TSAN
      INIT_FROM DEBUG
      COMPILER_FLAGS ${CHECKED_COMPILER_FLAGS}
    )

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()

function(setup_sanitize_flags)
    message(CHECK_START "Setup CXX Sanitize flags")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # Select Sanitize flags based on the compiler
    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_asan_flags(ASAN_COMPILER_FLAGS)
        custom_msvc_cxx_compiler_ubsan_flags(UBSAN_COMPILER_FLAGS)
        set(COMPILER_FLAGS "${ASAN_COMPILER_FLAGS};${UBSAN_COMPILER_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_asan_flags(ASAN_COMPILER_FLAGS)
        custom_msvc_cxx_compiler_ubsan_flags(UBSAN_COMPILER_FLAGS)
        set(COMPILER_FLAGS "${ASAN_COMPILER_FLAGS};${UBSAN_COMPILER_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_asan_flags(ASAN_COMPILER_FLAGS)
        custom_clang_cxx_compiler_ubsan_flags(UBSAN_COMPILER_FLAGS)
        set(COMPILER_FLAGS "${ASAN_COMPILER_FLAGS};${UBSAN_COMPILER_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_asan_flags(ASAN_COMPILER_FLAGS)
        custom_clang_cxx_compiler_ubsan_flags(UBSAN_COMPILER_FLAGS)
        set(COMPILER_FLAGS "${ASAN_COMPILER_FLAGS};${UBSAN_COMPILER_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_asan_flags(ASAN_COMPILER_FLAGS)
        custom_gcc_cxx_compiler_ubsan_flags(UBSAN_COMPILER_FLAGS)
        set(COMPILER_FLAGS "${ASAN_COMPILER_FLAGS};${UBSAN_COMPILER_FLAGS}")
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Choose try_compile build configuration
    set(CMAKE_TRY_COMPILE_CONFIGURATION "Debug")

    # Check compiler flags
    set(CHECKED_COMPILER_FLAGS)
    foreach(flag ${COMPILER_FLAGS})
        custom_check_cxx_compiler_linker_flag(${flag} res)
        if(res)
            set(CHECKED_COMPILER_FLAGS "${CHECKED_COMPILER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Set flags
    custom_build_type_set_base_flags(SANITIZE
      INIT_FROM DEBUG
      COMPILER_FLAGS ${CHECKED_COMPILER_FLAGS}
    )

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()

function(setup_optimization_flags)
    message(CHECK_START "Setup CXX optimization flags")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # Select optimization flags based on the compiler
    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_optimization_flags(COMPILER_FLAGS)
        custom_msvc_cxx_linker_optimization_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_optimization_flags(COMPILER_FLAGS)
        custom_msvc_cxx_linker_optimization_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_optimization_flags(COMPILER_FLAGS)
        custom_clang_cxx_linker_optimization_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_optimization_flags(COMPILER_FLAGS)
        custom_clang_cxx_linker_optimization_flags(LINKER_FLAGS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_optimization_flags(COMPILER_FLAGS)
        custom_gcc_cxx_linker_optimization_flags(LINKER_FLAGS)
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    # Choose try_compile build configuration
    set(CMAKE_TRY_COMPILE_CONFIGURATION "Release")

    # Check compiler flags
    set(CHECKED_COMPILER_FLAGS)
    foreach(flag ${COMPILER_FLAGS})
        custom_check_cxx_compiler_linker_flag(${flag} res)
        if(res)
            set(CHECKED_COMPILER_FLAGS "${CHECKED_COMPILER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Check linker flags
    set(CHECKED_LINKER_FLAGS)
    foreach(flag ${LINKER_FLAGS})
        custom_check_cxx_linker_flag(${flag} res)
        if(res)
            set(CHECKED_LINKER_FLAGS "${CHECKED_LINKER_FLAGS} ${flag}")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    # Add flags
    custom_build_type_add_base_flags(RELEASE
      COMPILER_FLAGS ${CHECKED_COMPILER_FLAGS}
      LINKER_FLAGS ${CHECKED_LINKER_FLAGS}
    )

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()

function(target_add_cxx_warning_flags target)
    message(CHECK_START "Adding CXX warning flags to ${target} target")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        message(STATUS "C++ compiler is MSVC-like Clang, using MSVC flags")
        custom_msvc_cxx_compiler_warning_flags(CUSTOM_MSVC_CXX_WARNING_FLAGS)
        set(WARNING_FLAGS ${CUSTOM_MSVC_CXX_WARNING_FLAGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "C++ compiler is Microsoft Visual Studio, using MSVC flags")
        custom_msvc_cxx_compiler_warning_flags(CUSTOM_MSVC_CXX_WARNING_FLAGS)
        set(WARNING_FLAGS ${CUSTOM_MSVC_CXX_WARNING_FLAGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "C++ compiler is LLVM Clang, using clang flags")
        custom_clang_cxx_compiler_warning_flags(CUSTOM_CLANG_CXX_WARNING_FLAGS)
        set(WARNING_FLAGS ${CUSTOM_CLANG_CXX_WARNING_FLAGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "C++ compiler is Apple Clang, using clang flags")
        custom_clang_cxx_compiler_warning_flags(CUSTOM_CLANG_CXX_WARNING_FLAGS)
        set(WARNING_FLAGS ${CUSTOM_CLANG_CXX_WARNING_FLAGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "C++ compiler is GNU Compiler Collection, using gcc flags")
        custom_gcc_cxx_compiler_warning_flags(CUSTOM_GCC_CXX_WARNING_FLAGS)
        set(WARNING_FLAGS ${CUSTOM_GCC_CXX_WARNING_FLAGS})
    else()
        message(STATUS "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    foreach(flag ${WARNING_FLAGS})
        custom_check_cxx_compiler_flag(${flag} res)
        if(res)
            target_compile_options(${target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${flag}>")
            message(STATUS "[✔] ${flag}")
        else()
            message(STATUS "[✘] ${flag}")
        endif()
    endforeach()

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "done")
endfunction()
