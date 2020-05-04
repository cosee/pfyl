//
// this file is only required for unit tests
// Created by mohamad on 16.01.20.
//
#include "flow_helper.h"

/**
 * adds sink_entity to buffer and transmits it
 * must delete sink_entity
 * @return
 */
__attribute__((weak)) uint8_t push_sink_entity(const sink_entity *se) {
    //must be overridden in user code
    return 0;
}

/**
 *
 * @return
 */
__attribute__((weak)) uint8_t pop_sink_entity(sink_entity *se) {
    //must be overridden in user code
    return 0;
}

__attribute__((weak)) uint32_t flush_buffer() {
    return 0;
}

__attribute__((weak)) uint64_t get_tick() {
    return 0;
}

__attribute__((weak)) void pfyl_begin_transfer() {
}
__attribute__((weak)) uint32_t pfyl_transfer(const uint8_t *buf, size_t size) {
    return 0;
}
__attribute__((weak)) void pfyl_end_transfer() {
}
