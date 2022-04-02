# sources for ansi c
set(SINK_ENTITY_SOURCE_FILE ${CMAKE_CURRENT_LIST_DIR}/common/ansi_c/sink_entity.c)
set(SINK_MODEL_SOURCE_FILE ${CMAKE_CURRENT_LIST_DIR}/common/SinkModel.pb.c)
include_directories(${CMAKE_CURRENT_LIST_DIR}/common)
include_directories(${CMAKE_CURRENT_LIST_DIR}/common/ansi_c)
include_directories(${CMAKE_CURRENT_LIST_DIR}/processors/encoding/common)


set(FLOW_HELPER_SOURCE_FILE ${CMAKE_CURRENT_LIST_DIR}/common/ansi_c/flow_helper.c)

set(COMMON_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/common/ansi_c/flow_helper.c)
set(TRACE_ENTITY_PROTOBUF_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/processors/common/SinkModel.pb.c)
set(SOURCE_HEAP_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/sources/heap/heap_source.c)
set(SINK_RAW_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/sinks/raw/raw_sink.c)
set(SOURCE_FUNC_INSTR_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/sources/instrfunc/instrfunc.c)

set(RAW_DECODER_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/processors/decoding/raw/raw_decoder.c)
set(RAW_ENCODER_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/processors/encoding/raw/raw_encoder.c)

set(NANOPB_ENCODER_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/processors/encoding/nanopb/nanopb_encoder.c)
set(SINK_LINEAR_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/sinks/nanopb/linear_buffer.c)

set(ITM_ENCODER_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/processors/encoding/itm/itm_encoder.c)

# TODO: refactor so include does not happen immediately
include_directories(${CMAKE_CURRENT_LIST_DIR}/sinks)
include_directories(${CMAKE_CURRENT_LIST_DIR}/sinks/nanopb)

include_directories(${CMAKE_CURRENT_LIST_DIR}/processors/common)

include_directories(${CMAKE_CURRENT_LIST_DIR}/sources/heap)

set(HEADER_FREERTOS_TRACE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/sources/freertos)
