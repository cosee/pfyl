cmake_minimum_required(VERSION 3.14)
project(test-pfyl)

include(src/CMakeLists.txt)

include_directories(${CPPUTEST_PROJECT_DIR}/include)
include_directories(${CPPUTEST_PROJECT_DIR}/include/CppUTest)


link_directories(${CPPUTEST_LIB_DIR})

link_libraries(CppUTest CppUTestExt)

add_executable(test_encoder
        ${RAW_DECODER_SOURCE_FILES}
        ${RAW_ENCODER_SOURCE_FILES}
        ${SINK_RAW_SOURCE_FILES}
        tests/processors/raw_encoding_test.cpp
        tests/MainTest.cpp
        )

add_definitions("-DRAW_ENCODER_ENABLED")
add_executable(test-pfyl
        ${SINK_RAW_SOURCE_FILES}
        ${SOURCE_HEAP_SOURCE_FILES}
        ${RAW_DECODER_SOURCE_FILES}
        ${RAW_ENCODER_SOURCE_FILES}
        tests/MainTest.cpp
        tests/sinks/raw_sink_test.cpp
        tests/processors/raw_decoding_test.cpp
        tests/processors/raw_encoding_test.cpp)
        
target_link_options(test-pfyl PRIVATE "-Wl,--wrap,malloc")
target_link_options(test-pfyl PRIVATE "-Wl,--wrap,free")

