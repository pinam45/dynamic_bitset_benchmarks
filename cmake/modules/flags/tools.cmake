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
enable_language(C)
enable_language(CXX)
include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckLinkerFlag)

function(custom_add_base_flags)
    # Process arguments
    cmake_parse_arguments(arg_custom_add_base_flags
      ""
      ""
      "COMPILER_FLAGS;C_FLAGS;CXX_FLAGS;LINKER_FLAGS;EXE_LINKER_FLAGS;SHARED_LINKER_FLAGS"
      ${ARGN}
    )
    if(arg_custom_add_base_flags_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments: ${arg_custom_add_base_flags_UNPARSED_ARGUMENTS}")
    endif()

    # Initialize flags
    set(NEW_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(NEW_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(NEW_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set(NEW_CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

    # Add compiler flags
    foreach(flag ${arg_custom_add_base_flags_COMPILER_FLAGS})
        set(NEW_CMAKE_C_FLAGS "${NEW_CMAKE_C_FLAGS} ${flag}")
        set(NEW_CMAKE_CXX_FLAGS "${NEW_CMAKE_CXX_FLAGS} ${flag}")
    endforeach()

    # Add C specific flags
    foreach(flag ${arg_custom_add_base_flags_C_FLAGS})
        set(NEW_CMAKE_C_FLAGS "${NEW_CMAKE_C_FLAGS} ${flag}")
    endforeach()

    # Add C++ specific flags
    foreach(flag ${arg_custom_add_base_flags_CXX_FLAGS})
        set(NEW_CMAKE_CXX_FLAGS "${NEW_CMAKE_CXX_FLAGS} ${flag}")
    endforeach()

    # Add linker flags
    foreach(flag ${arg_custom_add_base_flags_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS "${NEW_CMAKE_EXE_LINKER_FLAGS} ${flag}")
        set(NEW_CMAKE_SHARED_LINKER_FLAGS "${NEW_CMAKE_SHARED_LINKER_FLAGS} ${flag}")
    endforeach()

    # Add executable linker flags
    foreach(flag ${arg_custom_add_base_flags_EXE_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS "${NEW_CMAKE_EXE_LINKER_FLAGS} ${flag}")
    endforeach()

    # Add shared libraries linker flags
    foreach(flag ${arg_custom_add_base_flags_SHARED_LINKER_FLAGS})
        set(NEW_CMAKE_SHARED_LINKER_FLAGS "${NEW_CMAKE_SHARED_LINKER_FLAGS} ${flag}")
    endforeach()

    # Set the new flags in the cache
    set(CMAKE_C_FLAGS
      "${NEW_CMAKE_C_FLAGS}"
      CACHE STRING "Flags used by the C compiler"
      FORCE
    )
    set(CMAKE_CXX_FLAGS
      "${NEW_CMAKE_CXX_FLAGS}"
      CACHE STRING "Flags used by the CXX compiler"
      FORCE
    )
    set(CMAKE_EXE_LINKER_FLAGS
      "${NEW_CMAKE_EXE_LINKER_FLAGS}"
      CACHE STRING "Linker flags to be used to create executables"
      FORCE
    )
    set(CMAKE_SHARED_LINKER_FLAGS
      "${NEW_CMAKE_SHARED_LINKER_FLAGS}"
      CACHE STRING "Linker lags to be used to create shared libraries"
      FORCE
    )

    # Mark the variables as advanced so they don't clutter the CMake GUI
    mark_as_advanced(
      CMAKE_C_FLAGS
      CMAKE_CXX_FLAGS
      CMAKE_EXE_LINKER_FLAGS
      CMAKE_SHARED_LINKER_FLAGS
    )

    # Print the new flags
    foreach(flag_type C_FLAGS CXX_FLAGS EXE_LINKER_FLAGS SHARED_LINKER_FLAGS)
        string(REPLACE ";" " " flags_spaced "${CMAKE_${flag_type}}")
        if(flags_spaced)
            message(STATUS "[custom] ${flag_type}: set to \"${flags_spaced}\"")
        endif()
    endforeach()
endfunction()

function(custom_build_type_set_base_flags build_type)
    # Process arguments
    cmake_parse_arguments(arg_custom_build_type_set_base_flags
      ""
      "INIT_FROM"
      "COMPILER_FLAGS;C_FLAGS;CXX_FLAGS;LINKER_FLAGS;EXE_LINKER_FLAGS;SHARED_LINKER_FLAGS"
      ${ARGN}
    )
    if(arg_custom_build_type_set_base_flags_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments: ${arg_custom_build_type_set_base_flags_UNPARSED_ARGUMENTS}")
    endif()
    string(TOUPPER ${build_type} build_type)

    # Initialize flags from another build type if specified
    if(arg_custom_build_type_set_base_flags_INIT_FROM)
        string(TOUPPER "${arg_custom_build_type_set_base_flags_INIT_FROM}" init_from_build_type)
        set(NEW_CMAKE_C_FLAGS_${build_type} "${CMAKE_C_FLAGS_${init_from_build_type}}")
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${CMAKE_CXX_FLAGS_${init_from_build_type}}")
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${CMAKE_EXE_LINKER_FLAGS_${init_from_build_type}}")
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${CMAKE_SHARED_LINKER_FLAGS_${init_from_build_type}}")
    else()
        set(NEW_CMAKE_C_FLAGS_${build_type})
        set(NEW_CMAKE_CXX_FLAGS_${build_type})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type})
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type})
    endif()

    # Add compiler flags
    foreach(flag ${arg_custom_build_type_set_base_flags_COMPILER_FLAGS})
        set(NEW_CMAKE_C_FLAGS_${build_type} "${NEW_CMAKE_C_FLAGS_${build_type}} ${flag}")
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${NEW_CMAKE_CXX_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add C specific flags
    foreach(flag ${arg_custom_build_type_set_base_flags_C_FLAGS})
        set(NEW_CMAKE_C_FLAGS_${build_type} "${NEW_CMAKE_C_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add C++ specific flags
    foreach(flag ${arg_custom_build_type_set_base_flags_CXX_FLAGS})
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${NEW_CMAKE_CXX_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add linker flags
    foreach(flag ${arg_custom_build_type_set_base_flags_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}} ${flag}")
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add executable linker flags
    foreach(flag ${arg_custom_build_type_set_base_flags_EXE_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add shared libraries linker flags
    foreach(flag ${arg_custom_build_type_set_base_flags_SHARED_LINKER_FLAGS})
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Set the new flags in the cache
    set(CMAKE_C_FLAGS_${build_type}
      "${NEW_CMAKE_C_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the C compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_CXX_FLAGS_${build_type}
      "${NEW_CMAKE_CXX_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the CXX compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_EXE_LINKER_FLAGS_${build_type}
      "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker flags to be used to create executables for ${build_type} build type."
      FORCE
    )
    set(CMAKE_SHARED_LINKER_FLAGS_${build_type}
      "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker lags to be used to create shared libraries for ${build_type} build type."
      FORCE
    )

    # Mark the variables as advanced so they don't clutter the CMake GUI
    mark_as_advanced(
      CMAKE_C_FLAGS_${build_type}
      CMAKE_CXX_FLAGS_${build_type}
      CMAKE_EXE_LINKER_FLAGS_${build_type}
      CMAKE_SHARED_LINKER_FLAGS_${build_type}
    )

    # Print the new flags
     foreach(flag_type C_FLAGS CXX_FLAGS EXE_LINKER_FLAGS SHARED_LINKER_FLAGS)
         string(REPLACE ";" " " flags_spaced "${CMAKE_${flag_type}_${build_type}}")
         if(flags_spaced)
             message(STATUS "[custom] ${build_type} ${flag_type}: set to \"${flags_spaced}\"")
         endif()
     endforeach()
endfunction()

function(custom_build_type_add_base_flags build_type)
    # Process arguments
    cmake_parse_arguments(arg_custom_build_type_add_base_flags
      ""
      ""
      "COMPILER_FLAGS;C_FLAGS;CXX_FLAGS;LINKER_FLAGS;EXE_LINKER_FLAGS;SHARED_LINKER_FLAGS"
      ${ARGN}
    )
    if(arg_custom_build_type_add_base_flags_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments: ${arg_custom_build_type_add_base_flags_UNPARSED_ARGUMENTS}")
    endif()
    string(TOUPPER ${build_type} build_type)

    # Initialize flags
    set(NEW_CMAKE_C_FLAGS_${build_type} "${CMAKE_C_FLAGS_${build_type}}")
    set(NEW_CMAKE_CXX_FLAGS_${build_type} "${CMAKE_CXX_FLAGS_${build_type}}")
    set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${CMAKE_EXE_LINKER_FLAGS_${build_type}}")
    set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${CMAKE_SHARED_LINKER_FLAGS_${build_type}}")

    # Add compiler flags
    foreach(flag ${arg_custom_build_type_add_base_flags_COMPILER_FLAGS})
        set(NEW_CMAKE_C_FLAGS_${build_type} "${NEW_CMAKE_C_FLAGS_${build_type}} ${flag}")
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${NEW_CMAKE_CXX_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add C specific flags
    foreach(flag ${arg_custom_build_type_add_base_flags_C_FLAGS})
        set(NEW_CMAKE_C_FLAGS_${build_type} "${NEW_CMAKE_C_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add C++ specific flags
    foreach(flag ${arg_custom_build_type_add_base_flags_CXX_FLAGS})
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${NEW_CMAKE_CXX_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add linker flags
    foreach(flag ${arg_custom_build_type_add_base_flags_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}} ${flag}")
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add executable linker flags
    foreach(flag ${arg_custom_build_type_add_base_flags_EXE_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Add shared libraries linker flags
    foreach(flag ${arg_custom_build_type_add_base_flags_SHARED_LINKER_FLAGS})
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    # Set the new flags in the cache
    set(CMAKE_C_FLAGS_${build_type}
      "${NEW_CMAKE_C_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the C compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_CXX_FLAGS_${build_type}
      "${NEW_CMAKE_CXX_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the CXX compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_EXE_LINKER_FLAGS_${build_type}
      "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker flags to be used to create executables for ${build_type} build type."
      FORCE
    )
    set(CMAKE_SHARED_LINKER_FLAGS_${build_type}
      "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker lags to be used to create shared libraries for ${build_type} build type."
      FORCE
    )

    # Mark the variables as advanced so they don't clutter the CMake GUI
    mark_as_advanced(
      CMAKE_C_FLAGS_${build_type}
      CMAKE_CXX_FLAGS_${build_type}
      CMAKE_EXE_LINKER_FLAGS_${build_type}
      CMAKE_SHARED_LINKER_FLAGS_${build_type}
    )

    # Print the new flags
    foreach(flag_type C_FLAGS CXX_FLAGS EXE_LINKER_FLAGS SHARED_LINKER_FLAGS)
        string(REPLACE ";" " " flags_spaced "${CMAKE_${flag_type}_${build_type}}")
        if(flags_spaced)
            message(STATUS "[custom] ${build_type} ${flag_type}: set to \"${flags_spaced}\"")
        endif()
    endforeach()
endfunction()

function(custom_check_cxx_compiler_flag flag var)
    # Get check variable name from flag
    set(check ${flag})
    string(REPLACE "-" "_" check ${check})
    string(REPLACE "=" "_" check ${check})
    string(REPLACE " " "_" check ${check})
    string(REPLACE "," "_" check ${check})
    string(REPLACE "/" "_" check ${check})
    string(REPLACE ":" "_" check ${check})
    string(REPLACE ";" "_" check ${check})
    string(REPLACE "!" "_" check ${check})
    string(REPLACE "+" "_" check ${check})

    # clang only rejects unknown warning flags if -Werror=unknown-warning-option is also specified.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        check_cxx_compiler_flag(-Werror=unknown-warning-option _Werror_unknown_warning_option)
        if(_Werror_unknown_warning_option)
            list(APPEND CMAKE_REQUIRED_FLAGS "-Werror=unknown-warning-option")
        endif()
    endif()

    # Do the check
    check_cxx_compiler_flag(${flag} ${check})

    # Set result
    set(${var} ${${check}} PARENT_SCOPE)
endfunction()

function(custom_check_cxx_linker_flag flag var)
    # Get check variable name from flag
    set(check ${flag})
    string(REPLACE "-" "_" check ${check})
    string(REPLACE "=" "_" check ${check})
    string(REPLACE " " "_" check ${check})
    string(REPLACE "," "_" check ${check})
    string(REPLACE "/" "_" check ${check})
    string(REPLACE ":" "_" check ${check})
    string(REPLACE ";" "_" check ${check})
    string(REPLACE "!" "_" check ${check})
    string(REPLACE "+" "_" check ${check})

    # clang only rejects unknown warning flags if -Werror=unknown-warning-option is also specified.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        check_cxx_compiler_flag(-Werror=unknown-warning-option _Werror_unknown_warning_option)
        if(_Werror_unknown_warning_option)
            list(APPEND CMAKE_REQUIRED_FLAGS "-Werror=unknown-warning-option")
            list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "-Werror=unknown-warning-option")
        endif()
    endif()

    # Do the check
    check_linker_flag(CXX ${flag} ${check})

    # Set result
    set(${var} ${${check}} PARENT_SCOPE)
endfunction()

function(custom_check_cxx_compiler_linker_flag flag var)
    # Get check variable name from flag
    set(check ${flag})
    string(REPLACE "-" "_" check ${check})
    string(REPLACE "=" "_" check ${check})
    string(REPLACE " " "_" check ${check})
    string(REPLACE "," "_" check ${check})
    string(REPLACE "/" "_" check ${check})
    string(REPLACE ":" "_" check ${check})
    string(REPLACE ";" "_" check ${check})
    string(REPLACE "!" "_" check ${check})

    # clang only rejects unknown warning flags if -Werror=unknown-warning-option is also specified.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        check_cxx_compiler_flag(-Werror=unknown-warning-option _Werror_unknown_warning_option)
        if(_Werror_unknown_warning_option)
            list(APPEND CMAKE_REQUIRED_FLAGS "-Werror=unknown-warning-option")
            list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "-Werror=unknown-warning-option")
        endif()
    endif()

    # Handle special cases for flags that require additional flags to be set
    if("${flag}" STREQUAL "-fwhole-program-vtables")
        list(APPEND CMAKE_REQUIRED_FLAGS "-flto")
        list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "-flto")
    endif()
    if("${flag}" STREQUAL "-fsanitize=pointer-compare")
        list(APPEND CMAKE_REQUIRED_FLAGS "-fsanitize=address")
        list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
    endif()
    if("${flag}" STREQUAL "-fsanitize=pointer-subtract")
        list(APPEND CMAKE_REQUIRED_FLAGS "-fsanitize=address")
        list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
    endif()

    # Set flag for linker
    list(APPEND CMAKE_REQUIRED_FLAGS "${flag}")
    list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "${flag}")

    # Do the check
    check_cxx_compiler_flag(${flag} ${check})

    # Set result
    set(${var} ${${check}} PARENT_SCOPE)
endfunction()
