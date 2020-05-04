//
// Created by mohamad on 08.01.20.
//

#ifndef F7_DEVICE_FREERTOS_TRACE_H
#define F7_DEVICE_FREERTOS_TRACE_H

#include "sink_entity.h"
#include <string.h>
static sink_entity freertos_se = {
        .type=PFYL_ENTITY_TYPE_TASK_RDY,
        .tick=0,
        .data = 0,
        .data2=0,
        .data3=0
};


#define traceTASK_SWITCHED_IN() { \
		updateSinkEntity(&freertos_se, PFYL_ENTITY_TYPE_TASK_RDY, pxCurrentTCB, 0, 0); \
		push_sink_entity(&freertos_se); \
}

#define traceTASK_CREATE(xTask) { \
    union task_name { \
        uint64_t part; \
        uint8_t part_c[8]; \
    };\
    union task_name task_name_1 = {0}; \
    union task_name task_name_2 = {0}; \
    memcpy(task_name_1.part_c, xTask->pcTaskName, 8); \
    memcpy(task_name_2.part_c, &xTask->pcTaskName[8], 8); \
    updateSinkEntity(&freertos_se, PFYL_ENTITY_TYPE_TASK_CREATE, xTask, task_name_1.part, task_name_2.part); \
    push_sink_entity(&freertos_se); \
}

#define traceTASK_SWITCHED_OUT() { \
    updateSinkEntity(&freertos_se, PFYL_ENTITY_TYPE_TASK_DELAY, pxCurrentTCB, 0, 0); \
    push_sink_entity(&freertos_se); \
}
#endif //F7_DEVICE_FREERTOS_TRACE_H
