//
// Created by Mohamad Ramadan on 10.03.21.
//


#include <flow_helper.h>
#include <pb_encode.h>
#include "sink_entity.h"
#include "nanopb_buffer.h"
#include "TraceEntity.pb.h"

uint8_t push_sink_entity(const sink_entity *se) {
    uint8_t buf[200] = {0};
    pfyl_TraceEntity traceEntity = {};
    traceEntity.type = se->type;
    traceEntity.tick = se->tick;
    traceEntity.data = se->data;
    traceEntity.has_data = se->data != 0;
    traceEntity.data2 = se->data2;
    traceEntity.has_data2 = se->data2 != 0;
    traceEntity.data3 = se->data3;
    traceEntity.has_data3 = se->data3 != 0;

    size_t messageSize;
    pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
    pb_encode_delimited(&stream, pfyl_TraceEntity_fields, &traceEntity);
    pb_get_encoded_size(&messageSize, pfyl_TraceEntity_fields, &traceEntity);
    return write_to_buffer(buf, messageSize);
}

void alert_sink_entity(const sink_entity *se) {
    uint8_t buf[200] = {0};
    pfyl_TraceEntity traceEntity = {};
    traceEntity.type = se->type;
    traceEntity.tick = se->tick;
    traceEntity.data = se->data;
    traceEntity.has_data = se->data != 0;
    traceEntity.data2 = se->data2;
    traceEntity.has_data2 = se->data2 != 0;
    traceEntity.data3 = se->data3;
    traceEntity.has_data3 = se->data3 != 0;

    size_t messageSize;
    pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
    pb_encode_delimited(&stream, pfyl_TraceEntity_fields, &traceEntity);
    pb_get_encoded_size(&messageSize, pfyl_TraceEntity_fields, &traceEntity);
    pfyl_transfer(buf, messageSize + 1);
}

uint32_t flush_buffer() {
    uint32_t bytes_written = 0;
    const uint8_t *buf = getNBBuffer();
    uint32_t buf_size = getBufferSize();
    pfyl_begin_transfer();
    pfyl_transfer(buf, buf_size);
    clearBuffer();
    pfyl_end_transfer();
    return bytes_written;
}