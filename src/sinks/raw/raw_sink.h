//
// Created by mohamad on 09.12.19.
//
/**
 * the raw sink is intended to be used for sending and receiving meta data from an processor
 * CAUTION: the struct is packed so it is optimized for the target platform + the same endian
 */
#ifndef TEST_PFYL_RAW_SINK_H
#define TEST_PFYL_RAW_SINK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "sink_entity.h"
int write_to_buffer(const void *buf);
const uint8_t* getBuffer();
uint32_t getBufferSize();
void clearBuffer();
#ifdef __cplusplus
};
#endif

#endif //TEST_PFYL_RAW_SINK_H
