//
// Created by Mohamad Ramadan on 19.04.22.
//
#include "flow_helper.h"
#include "pfyl_mutex.h"
#include "freertos_trace.h"

void *pfylMutex = nullptr;


static pfyl_freertos_trace_entity freertosTrace = {
        .traceType=PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY,
        .tick=0,
        .taskName = nullptr,
        .taskHandle = nullptr,
};

void updateFreertosTrace(
        enum PFYL_FREERTOS_TRACE_ENTITY_TYPE type,
        const void *taskHandle,
        const char *taskName) {
    if (pfylMutex == nullptr) {
        pfylMutex = createPfylMutex();
    }

    if (aquireLock(pfylMutex)) {
        return;
    }

    freertosTrace.taskName = taskName;
    freertosTrace.taskHandle = taskHandle;
    freertosTrace.traceType = type;
    freertosTrace.tick = getReferenceTick();
    push_pfyl_trace(&freertosTrace);
    releaseLock(pfylMutex);
}

pfyl_freertos_trace_entity getFreeRTOSPfylTrace() {
    return freertosTrace;
}

void freeFreeRTOSMutex() {
    deleteMutex(pfylMutex);
    pfylMutex = nullptr;
}
