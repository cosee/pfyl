//
// Created by mohamad on 08.01.20.
//

#ifndef F7_DEVICE_FREERTOS_TRACE_H
#define F7_DEVICE_FREERTOS_TRACE_H
#include "flow_helper.h"
#ifdef __cplusplus
extern "C" {
#endif
void updateFreertosTrace(enum PFYL_FREERTOS_TRACE_ENTITY_TYPE type, const void *taskHandle, const char *taskName);

pfyl_freertos_trace_entity getFreeRTOSPfylTrace();

void freeFreeRTOSMutex();
#define traceTASK_SWITCHED_IN() { \
    updateFreertosTrace(PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY, pxCurrentTCB, nullptr); \
}

#define traceTASK_CREATE(xTask) { \
    updateFreertosTrace(PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE, xTask, (xTask)->pcTaskName); \
}

#define traceTASK_SWITCHED_OUT() { \
    updateFreertosTrace(PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY, pxCurrentTCB, nullptr); \
}
#ifdef __cplusplus
};
#endif
#endif //F7_DEVICE_FREERTOS_TRACE_H
