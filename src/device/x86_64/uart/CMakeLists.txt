cmake_minimum_required(VERSION 3.15)
project(uart)
set(TARGET_NAME uart)

set(PFYL_HOME /Users/ramadan/repos/pfyl)

include(${PFYL_HOME}/src/CMakeLists.txt)
include(${PFYL_HOME}/lib/CMakeLists.txt)

include_directories(${PFYL_HOME}/src/processors/encoding/raw)
include_directories(${PFYL_HOME}/src/processors/common)
include_directories(common)
include_directories(common/backend)

add_executable(${TARGET_NAME}
        ../common/influx_client.cpp
        uart_client.cpp
        ${RAW_DECODER_SOURCE_FILES}
        ../common/backend/api_client.cpp
        ${flatcc_src}
        ${PFYL_HOME}/src/processors/common/TraceEntity.pb.c
        )
target_link_directories(${TARGET_NAME} PUBLIC ${INFLUX_LIB_PATH})
target_link_libraries(${TARGET_NAME} PUBLIC ${INFLUX_LIB})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)