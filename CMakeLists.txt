cmake_minimum_required(VERSION 3.13)
project(pfyl C)
set(CMAKE_C_STANDARD 90)
add_compile_options("-fpic")
include(${PROJECT_SOURCE_DIR}/lib/CMakeLists.txt)


if(NOT EXISTS "${PROJECT_SOURCE_DIR}/src/common/SinkModel.pb.h" OR NOT EXISTS "${PROJECT_SOURCE_DIR}/src/common/SinkModel.pb.c")
    MESSAGE(FATAL_ERROR "Please generate proto files first with
    'python ../../lib/nanopb/generator/nanopb_generator.py SinkModel.proto' in src/common")
endif()

set(PFYL_HOME ${PROJECT_SOURCE_DIR})

include(tests/CMakeLists.txt)
