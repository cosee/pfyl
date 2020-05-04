//
// Created by mohamad on 10.12.19.
//


#include <cstring>
#include <raw_sink.h>
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "raw_decoder.h"

TEST_GROUP(RawDecodeTestGroup) {
    void setup()
    {
        clearBuffer();
    }
};

#define TEST_SINK_ENTITY_SIZE sizeof(sink_entity)

TEST(RawDecodeTestGroup, TestOneWrite) {
    const uint8_t raw_entity[] = {0x01, 0x78, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8c, 0x0, 0x0, 0x0, 0x0,
                                  0x0, 0x0, 0x0};
    sink_entity se = {0};
    convertByteArrayToSinkEntity(raw_entity, sizeof(raw_entity), &se);

    uint8_t buf2[TEST_SINK_ENTITY_SIZE] = {0};
    memcpy(buf2, &se, TEST_SINK_ENTITY_SIZE);

    for (int i = 0; i < sizeof(raw_entity); i++) {
        //printf("%x", buf2[i]);
        CHECK_TRUE(raw_entity[i] == buf2[i]);
    }
}

TEST(RawDecodeTestGroup, TestConversionForIncompatibleSize) {
    const uint8_t paddingByte = 0;
    const uint8_t raw_entity[] = {paddingByte, 0x2, 0x78, 0x0, 0x8c, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0x7f};
    sink_entity se = {0};
    convertByteArrayToSinkEntity(raw_entity, sizeof(raw_entity), &se);

    uint8_t buf2[TEST_SINK_ENTITY_SIZE] = {0};
    memcpy(buf2, &se, TEST_SINK_ENTITY_SIZE);

    for (int i = 0; i < sizeof(raw_entity); i++) {
        CHECK_TRUE(buf2[i] == 0);
    }
}
