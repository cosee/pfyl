//
// Created by mohamad on 10.12.19.
//

#ifndef TEST_PFYL_SINK_ENTITY_H
#define TEST_PFYL_SINK_ENTITY_H
#define nullptr ((void*)0)
#include <stdint.h>
#include <stddef.h>
#include "SinkModel.pb.h"

#ifdef __cplusplus
extern "C" {
#endif
enum PFYL_ENTITY_TYPE {
    PFYL_ENTITY_TYPE_MALLOC = 0,
    PFYL_ENTITY_TYPE_TASK_CREATE = 1,
    PFYL_ENTITY_TYPE_TASK_RDY = 2,
    PFYL_ENTITY_TYPE_TASK_DELAY = 3,
    PFYL_ENTITY_TYPE_FUNC_ENTER = 4,
    PFYL_ENTITY_TYPE_FUNC_EXIT = 5,
    PFYL_ENTITY_TYPE_FREE = 6,
    PFYL_ENTITY_TYPE_FAULT_HANDLER = 7
};

uint64_t getReferenceTick();

void increaseTick();

void setReferenceTick(uint64_t);


#ifndef SINK_ENTITY_SIZE
#define SINK_ENTITY_SIZE sizeof(sink_entity)
#endif

#ifdef __cplusplus
}
#endif

#endif //TEST_PFYL_SINK_ENTITY_H
