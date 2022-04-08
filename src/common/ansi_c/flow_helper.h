//
// Created by mohamad on 22.12.19.
//

#ifndef TEST_PFYL_CONTROLLER_HELPER_H
#define TEST_PFYL_CONTROLLER_HELPER_H

#include "sink_entity.h"
#include "freertos_trace.h"

#ifdef __cplusplus
extern "C" {
#endif

void push_pfyl_trace(const pfyl_freertos_trace_entity*);
/**
 * adds sink_entity to buffer and transmits it
 * must delete sink_entity
 * @return
 */
uint8_t push_sink_entity( const pb_msgdesc_t *fields, const void *src_struct);

/**
 *
 * @return
 */

uint32_t flush_buffer();

void pfyl_begin_transfer();

uint32_t pfyl_transfer(const uint8_t *buf, size_t size);
void pfyl_end_transfer();

#ifdef __cplusplus
};
#endif

#endif //TEST_PFYL_CONTROLLER_HELPER_H
