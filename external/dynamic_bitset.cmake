message(STATUS "Configuring dynamic_bitset")

get_filename_component(DYNAMIC_BITSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/dynamic_bitset ABSOLUTE)

# Submodule check
if(NOT EXISTS ${DYNAMIC_BITSET_DIR}/CMakeLists.txt)
	message(FATAL_ERROR "dynamic_bitset dependency is missing, maybe you didn't pull the git submodules")
endif()

# Include dynamic_bitset
#set(DYNAMICBITSET_USE_LIBPOPCNT ON CACHE INTERNAL "")
#set(DYNAMICBITSET_USE_LIBPOPCNT_SUBMODULE ON CACHE INTERNAL "")
#set(DYNAMICBITSET_BUILD_TESTS OFF CACHE INTERNAL "")
#set(DYNAMICBITSET_BUILD_EXAMPLE OFF CACHE INTERNAL "")
#set(DYNAMICBITSET_FORMAT_TARGET OFF CACHE INTERNAL "")
set(DYNAMICBITSET_HEADERS_TARGET_IDE ON CACHE INTERNAL "")
add_subdirectory(${DYNAMIC_BITSET_DIR})
if(NOT TARGET dynamic_bitset)
	message(FATAL_ERROR "dynamic_bitset target is missing")
endif()

# Set IDE folder
if(TARGET dynamic_bitset_headers_for_ide)
	set_target_properties(dynamic_bitset_headers_for_ide PROPERTIES FOLDER extlibs/dynamic_bitset)
endif()

message(STATUS "Configuring dynamic_bitset - Done")
