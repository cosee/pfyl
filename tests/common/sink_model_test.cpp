//
// Created by Mohamad Ramadan on 21.03.22.
//

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "SinkModel.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include <cstring>

TEST_GROUP(SinkModelTest) {
};

typedef struct {
    uint8_t* buffer;
    size_t length;
} VarString;


bool read_test_task_name(pb_istream_t *stream, const pb_field_iter_t *field, void **arg)
{
    VarString * value = (VarString *)*arg;
    if (stream->bytes_left > (value->length) - 1)
        return false;

    if (!pb_read(stream, value->buffer, stream->bytes_left))
        return false;
    return true;
}
bool write_test_task_name(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg)
{
    if (!pb_encode_tag_for_field(stream, field))
        return false;
    const char* stringValue = (const char *)*arg;
    return pb_encode_string(stream, (uint8_t*)stringValue, strlen(stringValue));
}

TEST(SinkModelTest, TestSimpleProtobufEncode) {
    PfylTaskCreated message = PfylTaskCreated_init_zero;
    uint8_t buffer[128];
    size_t message_length;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    const char *taskName = "test_task1";
    message.taskName.arg = (void*)taskName;
    message.taskName.funcs.encode = &write_test_task_name;

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
        message.taskName.arg = (void *) taskName;
        message.taskName.funcs.encode = &write_test_task_name;

        bool status = pb_encode(&stream, PfylTaskCreated_fields, &message);;
        message_length = stream.bytes_written;
        CHECK(status);
        LONGS_EQUAL(12, message_length);
    }
    {
        PfylTaskCreated decodedMessage = PfylTaskCreated_init_zero;
        uint8_t taskNameBuffer[24] = {0};
        VarString varTaskName = {taskNameBuffer, 24};
        decodedMessage.taskName.arg = &varTaskName;
        decodedMessage.taskName.funcs.decode = read_test_task_name;
        pb_istream_t decodeStream = pb_istream_from_buffer(buffer, message_length);
        bool status = pb_decode(&decodeStream, PfylTaskCreated_fields, &decodedMessage);
        CHECK(status);
        STRCMP_EQUAL(taskName, (char*)taskNameBuffer);
    }
}