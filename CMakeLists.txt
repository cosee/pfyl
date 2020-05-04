cmake_minimum_required(VERSION 3.14)
project(pfyl C)
set(CMAKE_C_STANDARD 90)
add_compile_options("-fpic")

if(NOT DEFINED CPPUTEST_PROJECT_DIR)
    MESSAGE(FATAL_ERROR "Please set the absolute path to the cpputest header")
ENDIF()

if(NOT DEFINED CPPUTEST_LIB_DIR)
    MESSAGE(FATAL_ERROR "Please set the absolute path to the cpputest library")
ENDIF()

set(PFYL_HOME ${PROJECT_SOURCE_DIR})

include(tests/CMakeLists.txt)
