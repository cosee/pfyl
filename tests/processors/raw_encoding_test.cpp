//
// Created by mohamad on 16.01.20.
//


#include <cstring>
#include <raw_decoder.h>
#include <raw_sink.h>
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"
#include "flow_helper.h"

#define TEST_SINK_ENTITY_SIZE sizeof(sink_entity)
uint8_t sink_entity_buffer[TEST_SINK_ENTITY_SIZE] = {0};

/* mock functions */
extern "C" void pfyl_begin_transfer() {
    mock().actualCall("pfyl_begin_transfer");
}

extern "C" void pfyl_end_transfer() {
    mock().actualCall("pfyl_end_transfer");
}

extern "C" uint32_t pfyl_transfer(const uint8_t *buf, size_t size) {
    mock().actualCall("pfyl_transfer");
    if (size > TEST_SINK_ENTITY_SIZE) {
        FAIL("pfyl_transfer write size is bigger than buffer");
    }
    memcpy(sink_entity_buffer, buf, TEST_SINK_ENTITY_SIZE);
    return 0;
}

TEST_GROUP(RawEncodeTestGroup) {
    void teardown() {
        mock().clear();
    }

    void setup() {
        memset(sink_entity_buffer, 0, TEST_SINK_ENTITY_SIZE);
        clearBuffer();
    }
};


TEST(RawEncodeTestGroup, TestWriteBufferCallOrder) {
    const sink_entity test_se = {
            .type = 1,
            .tick = 0x78UL,
            .data = UINT64_MAX,
            .data2 = UINT64_MAX,
            .data3 = 0x8CUL
    };
    mock().expectOneCall("pfyl_begin_transfer");
    mock().expectOneCall("pfyl_end_transfer");
    mock().expectOneCall("pfyl_transfer");
    clearBuffer();
    push_sink_entity(&test_se);
    flush_buffer();
    mock().checkExpectations();
}

TEST(RawEncodeTestGroup, TestWriteBufferContentWritten) {
    const sink_entity test_se = {
            .type = 1,
            .tick = 0x78UL,
            .data = UINT64_MAX,
            .data2 = UINT64_MAX,
            .data3 = 0x8CUL
    };
    mock().ignoreOtherCalls();
    const uint8_t raw_entity[TEST_SINK_ENTITY_SIZE] = {0};
    convertByteArrayToSinkEntity(raw_entity, sizeof(raw_entity), &test_se);
    push_sink_entity(&test_se);
    flush_buffer();
    for (int i = 0; i < sizeof(raw_entity); i++) {
        CHECK_TRUE(raw_entity[i] == sink_entity_buffer[i]);
    }

}