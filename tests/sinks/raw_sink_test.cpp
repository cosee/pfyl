//
// Created by mohamad on 09.12.19.
//

#include <cstring>
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "raw_sink.h"

TEST_GROUP(RawSinkTestGroup) {
    void setup() {
        clearBuffer();
    }
};

#define TEST_SINK_ENTITY_SIZE sizeof(sink_entity)

TEST(RawSinkTestGroup, TestOneWrite) {
    sink_entity test_entity = {
            .type = 1,
            .tick = 120,
            .data = LONG_MAX,
            .data2 = 140
    };
    uint8_t buf2[TEST_SINK_ENTITY_SIZE] = {0};
    memcpy(buf2, &test_entity, TEST_SINK_ENTITY_SIZE);
    write_to_buffer(&test_entity);
    const uint8_t *buf = getBuffer();
    for (int i = 0; i < getBufferSize(); i++) {
        //printf("%x, ", buf[i]);
        CHECK_TRUE(buf[i] == buf2[i]);
    }
}


TEST(RawSinkTestGroup, TestMultipleSimpleWrite) {
    sink_entity test_entity = {
            .type = 1,
            .tick = 120,
            .data = LONG_MAX,
            .data2 = 140
    };
    uint8_t buf2[TEST_SINK_ENTITY_SIZE] = {0};
    memcpy(buf2, &test_entity, TEST_SINK_ENTITY_SIZE);
    write_to_buffer(&test_entity);
    write_to_buffer(&test_entity);
    write_to_buffer(&test_entity);
    const uint8_t *buf = getBuffer();

    for (int i = 0; i < getBufferSize(); i++) {
        uint8_t buffer_item = buf[i];
        size_t ref_item_index = i % TEST_SINK_ENTITY_SIZE;
        uint8_t ref_item = buf2[ref_item_index];
        CHECK_TRUE(buffer_item == ref_item);
    }
}

TEST(RawSinkTestGroup, TestEmptyEntity) {
    write_to_buffer(NULL);
    CHECK_TRUE(getBufferSize() == 0);
}