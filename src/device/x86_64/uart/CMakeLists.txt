cmake_minimum_required(VERSION 3.15)
project(uart)
set(TARGET_NAME uart)

set(PFYL_HOME /home/mohamad/repos/pfyl)

include(${PFYL_HOME}/src/CMakeLists.txt)
include(${PFYL_HOME}/lib/CMakeLists.txt)


include_directories(common)
add_executable(${TARGET_NAME} ../common/influx_client.cpp uart_client.cpp ${RAW_DECODER_SOURCE_FILES})
target_link_directories(${TARGET_NAME} PUBLIC ${INFLUX_LIB_PATH})
target_link_libraries(${TARGET_NAME} PUBLIC ${INFLUX_LIB})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)