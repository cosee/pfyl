//
// Created by mohamad on 10.12.19.
//

#ifndef TEST_PFYL_RAW_DECODER_H
#define TEST_PFYL_RAW_DECODER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "sink_entity.h"
int convertByteArrayToSinkEntity(const void *buf, uint32_t count, const sink_entity* se);

#ifdef __cplusplus
};
#endif
#endif //TEST_PFYL_RAW_DECODER_H
