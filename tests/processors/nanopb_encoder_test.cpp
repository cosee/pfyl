//
// Created by Mohamad Ramadan on 08.04.22.
//

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"
#include "SinkModel.pb.h"
#include "flow_helper.h"
#include "pb_decode.h"

uint8_t* testBuffer = nullptr;
size_t testBufferSize = 0;
size_t testBufferWriteLength = 0;
extern "C" {
    void pfyl_begin_transfer() {
        mock().actualCall("pfyl_begin_transfer");
    }

    uint32_t pfyl_transfer(const uint8_t *buf, size_t size) {
        mock().actualCall("pfyl_transfer").withParameter("size", size);
        CHECK(size > 0);
        CHECK(size <= testBufferSize);
        testBufferWriteLength = size;
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
        testBuffer = nullptr;
        testBufferSize = 0;
        testBufferWriteLength = 0;
    }
    void teardown() {
        mock().clear();
    }
};

TEST(NanoPbEncoderTest, TestWithSufficientBufferLength) {
    mock().expectOneCall("pfyl_begin_transfer");
    mock().expectOneCall("pfyl_transfer").withParameter("size", 13);
    mock().expectOneCall("pfyl_end_transfer");
    uint8_t buf[100] = {0};
    {
        testBuffer = buf;
        testBufferSize = 100;
        pfyl_freertos_trace_entity freertosTrace = {
                .traceType=PFYL_FREERTOS_TRACE_ENTITY_TYPE_TASK_CREATE,
                .tick=123,
                .taskName = "testTask",
                .taskHandle = nullptr,
        };
        push_pfyl_trace(&freertosTrace);
        flush_buffer();
    }
    {
        PfylTaskCreated decodedMessage = PfylTaskCreated_init_zero;
        pb_istream_t decodeStream = pb_istream_from_buffer(buf, testBufferWriteLength);
        bool status = pb_decode_delimited(&decodeStream, PfylTaskCreated_fields, &decodedMessage);
        CHECK(status);

        STRCMP_EQUAL("testTask", decodedMessage.taskName);

    }
}


// TODO: add test with size small buffer