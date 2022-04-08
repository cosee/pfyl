//
// Created by Mohamad Ramadan on 14.03.22.
//
#include "sink_entity.h"

static uint64_t referenceTick;

uint64_t getReferenceTick() {
    return referenceTick;
}

void increaseTick() {
    referenceTick++;
}

void setReferenceTick(uint64_t newReferenceTick) {
    referenceTick = newReferenceTick;
}
