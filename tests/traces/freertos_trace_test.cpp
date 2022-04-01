//
// Created by Mohamad Ramadan on 01.04.22.
//

#include "flow_helper.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"
#include "freertos_trace.h"

extern "C" void push_pfyl_trace(const pfyl_freertos_trace_entity* trace) {
    mock().actualCall("push_pfyl_trace").withPointerParameter("trace", (void *) trace);
}


TEST_GROUP(FreeRTOSTraceTestGroup) {
    void teardown() {
        reference_tick = 0;
        mock().clear();
    }
};
typedef struct {
    char pcTaskName[32];
} FreeRTOSTaskMock;

TEST(FreeRTOSTraceTestGroup, TestTraceRecordsTaskCreation) {
    mock().expectOneCall("push_pfyl_trace").withPointerParameter("trace", &freertosTrace);
    FreeRTOSTaskMock mockTask = {
                             .pcTaskName = {"testTask 1"}
    };
    FreeRTOSTaskMock* pxCurrentTCB = &mockTask;
    traceTASK_CREATE(&mockTask);
    STRCMP_EQUAL(freertosTrace.taskName, mockTask.pcTaskName);
    POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
    CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE);
    CHECK_EQUAL(freertosTrace.tick, 0);
}

TEST(FreeRTOSTraceTestGroup, TestTraceRecordsTaskReady) {
    mock().expectOneCall("push_pfyl_trace").withPointerParameter("trace", &freertosTrace);
    FreeRTOSTaskMock mockTask = {
                             .pcTaskName = {"testTask 1"}
    };
    FreeRTOSTaskMock* pxCurrentTCB = &mockTask;
    traceTASK_SWITCHED_IN();

    POINTERS_EQUAL(freertosTrace.taskName, nullptr);
    POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
    CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY);
    CHECK_EQUAL(freertosTrace.tick, 0);
}

TEST(FreeRTOSTraceTestGroup, TestTraceRecordsTaskReadyAfterCreation) {
    mock().expectNCalls(2, "push_pfyl_trace").withPointerParameter("trace", &freertosTrace);
    FreeRTOSTaskMock mockTask = {
                             .pcTaskName = {"testTask 1"}
    };
    FreeRTOSTaskMock* pxCurrentTCB = &mockTask;
    {
        traceTASK_CREATE(&mockTask);
        STRCMP_EQUAL(freertosTrace.taskName, mockTask.pcTaskName);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE);
        CHECK_EQUAL(freertosTrace.tick, 0);
    }
    reference_tick++;
    {
        traceTASK_SWITCHED_IN();
        POINTERS_EQUAL(freertosTrace.taskName, nullptr);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY);
        CHECK_EQUAL(freertosTrace.tick, 1);
    }
}

TEST(FreeRTOSTraceTestGroup, TestTraceRecordsTaskInactive) {
    mock().expectNCalls(2, "push_pfyl_trace").withPointerParameter("trace", &freertosTrace);
    FreeRTOSTaskMock mockTask = {
                             .pcTaskName = {"testTask 1"}
    };
    FreeRTOSTaskMock* pxCurrentTCB = &mockTask;
    {
        traceTASK_CREATE(&mockTask);
        STRCMP_EQUAL(freertosTrace.taskName, mockTask.pcTaskName);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE);
        CHECK_EQUAL(freertosTrace.tick, 0);
    }
    reference_tick++;
    {
        traceTASK_SWITCHED_OUT();
        POINTERS_EQUAL(freertosTrace.taskName, nullptr);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY);
        CHECK_EQUAL(freertosTrace.tick, 1);
    }
}

TEST(FreeRTOSTraceTestGroup, TestTraceRecordsTaskInactiveAfterActive) {
    mock().expectNCalls(3, "push_pfyl_trace").withPointerParameter("trace", &freertosTrace);
    FreeRTOSTaskMock mockTask = {
                             .pcTaskName = {"testTask 1"}
    };
    FreeRTOSTaskMock* pxCurrentTCB = &mockTask;
    {
        traceTASK_CREATE(&mockTask);
        STRCMP_EQUAL(freertosTrace.taskName, mockTask.pcTaskName);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE);
        CHECK_EQUAL(freertosTrace.tick, 0);
    }
    reference_tick++;
    {
        traceTASK_SWITCHED_IN();
        STRCMP_EQUAL(freertosTrace.taskName, nullptr);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY);
        CHECK_EQUAL(freertosTrace.tick, 1);
    }
    reference_tick++;
    {
        traceTASK_SWITCHED_OUT();
        POINTERS_EQUAL(freertosTrace.taskName, nullptr);
        POINTERS_EQUAL(freertosTrace.taskHandle, pxCurrentTCB);
        CHECK_EQUAL(freertosTrace.traceType, PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_DELAY);
        CHECK_EQUAL(freertosTrace.tick, 2);
    }
}