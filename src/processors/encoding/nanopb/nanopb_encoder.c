//
// Created by Mohamad Ramadan on 10.03.21.
//


#include <pb_encode.h>
#include "flow_helper.h"
#include "pfyl_sink.h"
#include "SinkModel.pb.h"

#define MAX_PROTO_MESSAGE_SIZE 44

// TODO: check out if we can pass with zero copy, static seems error prone
void push_pfyl_trace(const pfyl_freertos_trace_entity *trace) {
    switch (trace->traceType) {
        case PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE: {
            PfylTaskCreated createdTask = PfylTaskCreated_init_zero;
            createdTask.tick = trace->tick;
            strcpy(createdTask.taskName, trace->taskName);
            push_sink_entity(PfylTaskCreated_fields, &createdTask);
            break;
        }
        case PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY:
        case PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY: {
            static PfylTaskState taskState = PfylTaskState_init_zero;
            taskState.tick = trace->tick;
            strcpy(taskState.taskName, trace->taskName);
            push_sink_entity(PfylTaskState_fields, &taskState);
            break;
        }
        default:
            break;
    }
}

uint8_t push_sink_entity(const pb_msgdesc_t *fields, const void *src_struct) {
    uint8_t buf[MAX_PROTO_MESSAGE_SIZE] = {0};
    size_t messageSize;
    pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
    pb_encode_delimited(&stream, fields, src_struct);
    messageSize = stream.bytes_written;
    return pfylSinkWrite(buf, messageSize);
}

uint32_t flush_buffer() {
    uint32_t bytes_written = 0;
    const uint8_t *buf = pfylGetSink();
    uint32_t buf_size = pfylGetContentLength();
    pfyl_begin_transfer();
    pfyl_transfer(buf, buf_size);
    pfyl_end_transfer();
    pfylClearSink();
    return bytes_written;
}