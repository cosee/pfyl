# sources for ansi c
set(SINK_ENTITY_SOURCE_FILE )
include_directories(${PFYL_HOME}/src/common/ansi_c)

# uncomment for cxx support
# keep in mind: c bindings of flow helper functions are required
set(FLOW_HELPER_SOURCE_FILE ${PFYL_HOME}/src/common/ansi_c/flow_helper.c)
#set(SINK_ENTITY_SOURCE_FILE ${PFYL_HOME}/src/common/cxx/sink_entity.cpp)
#include_directories(${PFYL_HOME}/src/common/cxx)

set(COMMON_SOURCE_FILES
        ${PFYL_HOME}/src/common/ansi_c/flow_helper.c)
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
set(ITM_ENCODER_SOURCE_FILES
        ${PFYL_HOME}/src/processors/encoding/itm/itm_encoder.c)

include_directories(${PFYL_HOME}/src/processors/decoding/raw)
include_directories(${PFYL_HOME}/src/processors/encoding/itm)

include_directories(${PFYL_HOME}/src/sinks/raw)

include_directories(${PFYL_HOME}/src/sources/heap)
include_directories(${PFYL_HOME}/src/sources/freertos)
