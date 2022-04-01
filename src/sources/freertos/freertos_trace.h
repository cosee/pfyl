//
// Created by mohamad on 08.01.20.
//

#ifndef F7_DEVICE_FREERTOS_TRACE_H
#define F7_DEVICE_FREERTOS_TRACE_H
enum PFYL_FREERTOS_TRACE_ENTITY_TYPE {
    PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE = 1,
    PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY = 2,
    PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY = 3,
};
typedef struct {
    uint8_t traceType;
    uint64_t tick;
    const char* taskName;
    const void* taskHandle;
} pfyl_freertos_trace_entity;

static pfyl_freertos_trace_entity freertosTrace = {
        .traceType=PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY,
        .tick=0,
        .taskName = nullptr,
        .taskHandle = nullptr,
};

void inline updateFreertosTrace(pfyl_freertos_trace_entity* trace, PFYL_FREERTOS_TRACE_ENTITY_TYPE type,
                         const void* taskHandle = nullptr, const char* taskName = nullptr) {
    trace->taskName = taskName;
    trace->taskHandle = taskHandle;
    trace->traceType = type;
    trace->tick = reference_tick;
}


#define traceTASK_SWITCHED_IN() { \
		updateFreertosTrace(&freertosTrace, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY, pxCurrentTCB); \
		push_pfyl_trace(&freertosTrace); \
}

#define traceTASK_CREATE(xTask) { \
    updateFreertosTrace(&freertosTrace, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE, xTask, (xTask)->pcTaskName); \
    push_pfyl_trace(&freertosTrace); \
}

#define traceTASK_SWITCHED_OUT() { \
    updateFreertosTrace(&freertosTrace, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY, pxCurrentTCB); \
    push_pfyl_trace(&freertosTrace); \
}
#endif //F7_DEVICE_FREERTOS_TRACE_H
