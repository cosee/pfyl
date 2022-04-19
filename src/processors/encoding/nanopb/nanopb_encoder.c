//
// Created by Mohamad Ramadan on 10.03.21.
//


#include <pb_encode.h>
#include <printf.h>
#include "sink_entity.h"
#include "flow_helper.h"
#include "pfyl_sink.h"
#include "SinkModel.pb.h"

#define MAX_PROTO_MESSAGE_SIZE 44
void push_pfyl_trace(const pfyl_freertos_trace_entity* trace) {
    const pb_msgdesc_t *fieldDescription = nullptr;
    switch(trace->traceType) {
        case PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE:
            fieldDescription = PfylTaskCreated_fields;
        case PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY:
        case PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY:
        default:
            fieldDescription = PfylTaskState_fields;
    }
    push_sink_entity(fieldDescription, trace);
}

uint8_t push_sink_entity( const pb_msgdesc_t *fields, const void *src_struct) {
    char buf[MAX_PROTO_MESSAGE_SIZE] = {0};
    size_t messageSize;
    pb_ostream_t stream = pb_ostream_from_buffer((uint8_t *)buf, sizeof(buf));
    pb_encode_delimited(&stream, fields, src_struct);
    pb_get_encoded_size(&messageSize, fields, src_struct);
    return pfylSinkWrite(buf, messageSize);
}

uint32_t flush_buffer() {
    uint32_t bytes_written = 0;
    const uint8_t *buf = pfylGetSink();
    uint32_t buf_size = pfylGetContentLength();
    pfyl_begin_transfer();
    pfyl_transfer(buf, buf_size);
    pfylClearSink();
    pfyl_end_transfer();
    return bytes_written;
}