cmake_minimum_required (VERSION 3.0)

project (slotty)

option (BUILD_TESTS "Build all tests" ON)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

add_definitions (-std=c++14)
add_library (slotty INTERFACE)
target_include_directories(slotty INTERFACE
	include/
	include/slotty/slotty.h
)

include (cmake/clang-format.cmake)

if (BUILD_TESTS)
	add_subdirectory (third-party/googletest/googletest)
	add_subdirectory (tests)
endif ()