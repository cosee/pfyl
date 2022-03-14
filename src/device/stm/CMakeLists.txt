cmake_minimum_required(VERSION 3.17)
set(PFYL_HOME /Users/ramadan/repos/pfyl)

if(NOT DEFINED PFYL_HOME)
    MESSAGE(FATAL_ERROR "Please set the (absolute) pfyl home path")
ENDIF()
SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a" ".dylib")
set(STM32_FAMILY F7)
set(FLATCC_LIB_DIR /Users/ramadan/repos/flatcc)
set(STM32_CHIP STM32F746NG)
set(STM_BOARD STM32746G-Discovery)
set(STM32Cube_DIR /Users/ramadan/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.0)
set(STM32BSP_INCLUDE_DIR /Users/ramadan/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.0/Drivers/BSP/STM32746G-Discovery)
set(STM32BSP_SOURCES /Users/ramadan/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.0/Drivers/BSP/STM32746G-Discovery)
#set(FreeRTOS_INCLUDE_DIRS /home/mohamad/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FreeRTOS)
set(TOOLCHAIN_PREFIX /opt/gcc-arm-none-eabi-9-2019-q4-major)
set(TARGET_TRIPLET arm-none-eabi)
set(uGFX_DIR ${PFYL_HOME}/lib/ugfx)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc_stm32.cmake)

include(${PFYL_HOME}/src/CMakeLists.txt)
project(f7-device)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_LANGUAGE(ASM)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

SET(FREERTOS_HEAP_IMPL 4)
FIND_PACKAGE(CMSIS REQUIRED)
FIND_PACKAGE(STM32HAL COMPONENTS gpio uart tim sdram i2c dma REQUIRED)
FIND_PACKAGE(STM32BSP REQUIRED sdram)
FIND_PACKAGE(FreeRTOS REQUIRED)
FIND_PACKAGE(uGFX REQUIRED gos gos_freertos gdisp driver_STM32LTDC)

INCLUDE_DIRECTORIES(
        flow_impl
        common
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMSIS_INCLUDE_DIRS}
        ${STM32HAL_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/f7
        ${FreeRTOS_INCLUDE_DIRS}
        ${STM32BSP_INCLUDE_DIR}
        ${uGFX_INCLUDE_DIRS}
        ${FLATCC_LIB_DIR}/include
)


file(GLOB NANOPB_SRC_FILES
        "${NANOPB_SRC_ROOT_FOLDER}/*.c"
        )
SET(PROJECT_SOURCES
        #flow_impl/CSTM32F7_Helper.cpp # uncomment for cxx support, also see: src/CMakeLists.txt
        f7/main.c
        common/backpressure.c
        common/stm32f7xx_it.c
        ${FLOW_HELPER_SOURCE_FILE}
        ${SOURCE_FUNC_INSTR_SOURCE_FILES}
        ${TRACE_ENTITY_PROTOBUF_SOURCE_FILES}
        ${NANOPB_SRC_FILES}
        ${NANOPB_ENCODER_SOURCE_FILES}
        ${SINK_NANOPB_SOURCE_FILES}
        )

add_link_options("-Wl,--wrap,malloc")
add_link_options("-Wl,--wrap,free")
add_compile_options("-Wa,-mimplicit-it=thumb")
add_compile_options("-finstrument-functions")

list(JOIN NANOPB_SRC_FILES , NANO_PB_JOINED_SRCS)
add_compile_options("-finstrument-functions-exclude-file-list=src/common,lib/ugfx,src/sinks,src/sources,src/processors,src/device/stm/f7/freertos_trace.h,${STM32Cube_DIR}/Drivers/STM32F7xx_HAL_Driver,${STM32Cube_DIR}/Drivers/CMSIS,${STM32Cube_DIR}/Middlewares/Third_Party/FreeRTOS,${NANO_PB_JOINED_SRCS}")
add_compile_options("-finstrument-functions-exclude-function-list=__get_MSP")

message(${FLATCC_LIB_DIR}/lib)
#link_directories(/usr/local/Cellar/flatcc/0.6.0/lib/)
file(GLOB flatcc_src
        "/Users/ramadan/repos/flatcc/src/runtime/*.c"
        )
#find_library(FLATCC_LIB NAMES flatccrt HINTS /Users/ramadan/repos/flatcc/lib)

#add_definitions("-DITM_ENABLED -DITM_ENCODER_ENABLED")
add_definitions("-DUART1_ENABLED -DBACKPRESSURE_ENABLED -DRAW_ENCODER_ENABLED")

add_definitions("-DPFYL_USE_FREERTOS -DPFYL_PLATFORM_ARM -DPFYL_USE_STATIC_ALLOC")
SET(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES} ${CMSIS_SOURCES} ${STM32HAL_SOURCES} ${flatcc_src})
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${SOURCE_HEAP_SOURCE_FILES} ${STM32BSP_SOURCES} ${FreeRTOS_SOURCES} ${uGFX_SOURCES}
        ${STM32Cube_DIR}/Drivers/BSP/${STM_BOARD}/stm32746g_discovery.c ${STM32Cube_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c)

#target_link_libraries(${CMAKE_PROJECT_NAME} flatccrt)
#find_library(
#        FLATCC_LIBRARY
#        NAME flatccrt
#        PATHS ${FLATCC_LIB_DIR}/lib NO_DEFAULT_PATH
#)

#target_link_libraries(${CMAKE_PROJECT_NAME} m)
target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -specs=nano.specs -specs=nosys.specs)
STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME})
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME})