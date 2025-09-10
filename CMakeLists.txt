cmake_minimum_required(VERSION 3.16.0)
project(SBF VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(DEPS_DIR "${CMAKE_SOURCE_DIR}/deps")

if (MSVC)
	add_compile_options(/utf-8)
endif()

file(GLOB LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(SBF SHARED "${LIB_SOURCES}")

target_include_directories(SBF 
	PUBLIC include 
	PRIVATE ${CMAKE_SOURCE_DIR}/src/internal
)

target_compile_definitions(SBF PRIVATE SBF_EXPORTS)

