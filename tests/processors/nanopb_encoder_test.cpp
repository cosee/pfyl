//
// Created by Mohamad Ramadan on 08.04.22.
//

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"
#include "SinkModel.pb.h"
#include "flow_helper.h"

char* testBuffer = nullptr;
size_t testBufferSize = 0;

extern "C" {
    void pfyl_begin_transfer() {
        mock().actualCall("pfyl_begin_transfer");
    }

    uint32_t pfyl_transfer(const uint8_t *buf, size_t size) {
        mock().actualCall("pfyl_transfer").withParameter("size", size);
        printf("\nsize of simpleLinearBuffer array is %lu", size);
        CHECK(size > 0);
        CHECK(size <= testBufferSize);
        memcpy(testBuffer, buf, size);
        return size;
    }

    void pfyl_end_transfer() {
        mock().actualCall("pfyl_end_transfer");
    }
}

TEST_GROUP(NanoPbEncoderTest) {
    void setup(){
        setReferenceTick(0);
    }
    void teardown() {
        mock().clear();
    }
};

TEST(NanoPbEncoderTest, TestWithSufficientBufferLength) {
    mock().expectOneCall("pfyl_begin_transfer");
    mock().expectOneCall("pfyl_transfer").withParameter("size", 10);
    mock().expectOneCall("pfyl_end_transfer");
    char buf[100] = {0};
    testBuffer = buf;
    testBufferSize = 100;
    pfyl_freertos_trace_entity freertosTrace = {
            .traceType=PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_RDY,
            .tick=123,
            .taskName = "testTask",
            .taskHandle = nullptr,
    };
    push_pfyl_trace(&freertosTrace);
    flush_buffer();
    CHECK(strlen(buf) > 0);
}