//
// Created by mohamad on 16.01.20.
//

#ifdef RAW_ENCODER_ENABLED
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <flow_helper.h>
#include <pb_encode.h>
#include "sink_entity.h"
#include "raw_sink.h"
#include "TraceEntity.pb.h"

uint8_t buf[200] = {0};
uint8_t push_sink_entity(const sink_entity *se) {
#ifdef UART_DIRECT_SEND
    pfyl_transfer((uint8_t*) se, SINK_ENTITY_SIZE);
#else
    pfyl_TraceEntity traceEntity = {};
    traceEntity.type = se->type;
    traceEntity.tick = se->tick;
    traceEntity.data = se->data;
    traceEntity.data2 = se->data2;
    traceEntity.data3 = se->data3;

    pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
    pb_encode(&stream, pfyl_TraceEntity_fields, &traceEntity);
    return write_to_buffer(buf);
#endif
}

uint32_t flush_buffer() {
#ifndef UART_DIRECT_SEND

    uint32_t bytes_written = 0;
    size_t size;
    const uint8_t *buf;
    uint32_t buf_size = getBufferSize();

    pfyl_begin_transfer();
    while (bytes_written < buf_size) {
        const uint16_t se_size = sizeof(sink_entity);
        pfyl_transfer(buf+bytes_written, se_size);
        bytes_written += se_size;
    }
    clearBuffer();
    pfyl_end_transfer();
    return bytes_written;
#else
    return 0;
#endif
}
#endif