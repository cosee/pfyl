//
// Created by mohamad on 16.01.20.
//

#ifdef RAW_ENCODER_ENABLED
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <flow_helper.h>
#include "sink_entity.h"
#include "raw_sink.h"

uint8_t push_sink_entity(const sink_entity *se) {
#ifdef UART_DIRECT_SEND
    pfyl_transfer((uint8_t*) se, SINK_ENTITY_SIZE);
#else
    return write_to_buffer(se);
#endif
}

uint32_t flush_buffer() {
#ifndef UART_DIRECT_SEND
    uint32_t bytes_written = 0;
    const uint8_t *buf = getBuffer();
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