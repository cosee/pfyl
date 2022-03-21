cmake_minimum_required(VERSION 3.13)
project(test-pfyl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/build/FindCppUTest.cmake")
    MESSAGE(FATAL_ERROR "Please run 'conan install' first")
endif()
include(${PFYL_HOME}/build/FindCppUTest.cmake)
include(${PFYL_HOME}/lib/CMakeLists.txt)
include(src/CMakeLists.txt)

list(APPEND CMAKE_MODULE_PATH ${PFYL_HOME}/build)
find_package(CppUTest REQUIRED)

add_executable(test_sink_model
        tests/common/sink_model_test.cpp
        tests/MainTest.cpp
        ${NANOPB_SRCS}
        ${NANOPB_HDRS}
        ${SINK_MODEL_SOURCE_FILE}
        )

target_link_libraries(test_sink_model PRIVATE
        CppUTest::CppUTest
        CppUTest::CppUTestExt)

