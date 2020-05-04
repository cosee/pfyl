//
// Created by mohamad on 10.12.19.
//

#include "raw_decoder.h"
#include <string.h>

int convertByteArrayToSinkEntity(const void *buf, uint32_t count, const sink_entity* se) {
    if(count == 0 || buf == 0) {
        return -1;
    }
    if(sizeof(sink_entity) != count) {
        return 1;
    }
    memcpy(se, buf, count);
    return 0;
}