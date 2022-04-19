//
// Created by Mohamad Ramadan on 21.03.22.
//

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "SinkModel.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

TEST_GROUP(SinkModelTest) {
};



TEST(SinkModelTest, TestSimpleProtobufEncode) {
    PfylTaskCreated message = PfylTaskCreated_init_zero;
    uint8_t buffer[128];
    size_t message_length;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    strcpy(message.taskName, "test_task1");

    bool status = pb_encode(&stream, PfylTaskCreated_fields, &message);;
    message_length = stream.bytes_written;
    LONGS_EQUAL(12, message_length);
    CHECK(status);
}

TEST(SinkModelTest, TestDecodeEncodedTaskTrace) {
    uint8_t buffer[128];
    size_t message_length;
    const char *taskName = "test_task1";
    {
        PfylTaskCreated message = PfylTaskCreated_init_zero;
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        strcpy(message.taskName, taskName);

        bool status = pb_encode(&stream, PfylTaskCreated_fields, &message);;
        message_length = stream.bytes_written;
        CHECK(status);
        LONGS_EQUAL(12, message_length);
    }
    {
        PfylTaskCreated decodedMessage = PfylTaskCreated_init_zero;
        uint8_t taskNameBuffer[24] = {0};
        pb_istream_t decodeStream = pb_istream_from_buffer(buffer, message_length);
        bool status = pb_decode(&decodeStream, PfylTaskCreated_fields, &decodedMessage);
        CHECK(status);
        STRCMP_EQUAL(taskName, decodedMessage.taskName);
    }
}