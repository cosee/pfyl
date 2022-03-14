# sources for ansi c
set(SINK_ENTITY_SOURCE_FILE )
include_directories(${PFYL_HOME}/src/common/ansi_c)
include_directories(${PFYL_HOME}/src/processors/encoding/common)

# Nanopb Settings
set(NANO_PB_ROOT_FOLDER "${PFYL_HOME}/lib/nanopb")
list(APPEND CMAKE_MODULE_PATH ${NANO_PB_ROOT_FOLDER}/extra)
set(NANOPB_SRC_ROOT_FOLDER ${NANO_PB_ROOT_FOLDER})
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

# uncomment for cxx support
# keep in mind: c bindings of flow helper functions are required
set(FLOW_HELPER_SOURCE_FILE ${PFYL_HOME}/src/common/ansi_c/flow_helper.c)
#set(SINK_ENTITY_SOURCE_FILE ${PFYL_HOME}/src/common/cxx/sink_entity.cpp)
#include_directories(${PFYL_HOME}/src/common/cxx)

set(COMMON_SOURCE_FILES
        ${PFYL_HOME}/src/common/ansi_c/flow_helper.c)
set(TRACE_ENTITY_PROTOBUF_SOURCE_FILES
        ${PFYL_HOME}/src/processors/common/TraceEntity.pb.c)
set(SOURCE_HEAP_SOURCE_FILES
        ${PFYL_HOME}/src/sources/heap/heap_source.c)
set(SINK_RAW_SOURCE_FILES
        ${PFYL_HOME}/src/sinks/raw/raw_sink.c)
set(SOURCE_FUNC_INSTR_SOURCE_FILES
        ${PFYL_HOME}/src/sources/instrfunc/instrfunc.c)

set(RAW_DECODER_SOURCE_FILES
        ${PFYL_HOME}/src/processors/decoding/raw/raw_decoder.c)
set(RAW_ENCODER_SOURCE_FILES
        ${PFYL_HOME}/src/processors/encoding/raw/raw_encoder.c)

set(NANOPB_ENCODER_SOURCE_FILES
        ${PFYL_HOME}/src/processors/encoding/nanopb/nanopb_encoder.c)
set(SINK_NANOPB_SOURCE_FILES
        ${PFYL_HOME}/src/sinks/nanopb/nanopb_buffer.c)

set(ITM_ENCODER_SOURCE_FILES
        ${PFYL_HOME}/src/processors/encoding/itm/itm_encoder.c)

# TODO: refactor it so include does not happen immediately
include_directories(${PFYL_HOME}/src/sinks/nanopb)

include_directories(${PFYL_HOME}/src/processors/common)

#include_directories(${PFYL_HOME}/src/processors/decoding/raw)
#include_directories(${PFYL_HOME}/src/processors/encoding/itm)

#include_directories(${PFYL_HOME}/src/sinks/raw)

include_directories(${PFYL_HOME}/src/sources/heap)
include_directories(${PFYL_HOME}/src/sources/freertos)
