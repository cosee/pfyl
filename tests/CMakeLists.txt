cmake_minimum_required(VERSION 3.13)
project(test-pfyl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/build/FindCppUTest.cmake")
    MESSAGE(FATAL_ERROR "Please run 'conan install' first")
endif()

include(${PFYL_HOME}/build/FindCppUTest.cmake)
include(src/CMakeLists.txt)

list(APPEND CMAKE_MODULE_PATH ${PFYL_HOME}/build)
find_package(CppUTest REQUIRED)

include_directories(${HEADER_FREERTOS_TRACE_DIRECTORY})
add_executable(test_sink_model
        tests/traces/freertos_trace_test.cpp
        tests/common/sink_model_test.cpp
        tests/sinks/linear_buffer_test.cpp
        tests/main_test.cpp
        ${SINK_LINEAR_SOURCE_FILES}
        ${NANOPB_SRCS}
        ${NANOPB_HDRS}
        ${SINK_MODEL_SOURCE_FILE}
        )

target_link_libraries(test_sink_model PRIVATE
        CppUTest::CppUTest
        CppUTest::CppUTestExt)

