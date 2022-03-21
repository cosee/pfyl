//
// Created by Mohamad Ramadan on 21.03.22.
//

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "SinkModel.pb.h"
#include "pb_encode.h"
#include <cstring>

TEST_GROUP(SinkModelTest) {
};

bool write_test_task_name(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg)
{

    if (!pb_encode_tag_for_field(stream, field))
        return false;
    const char* stringValue = (const char *)arg;
    return pb_encode_string(stream, (uint8_t*)stringValue, strlen(stringValue));
}

TEST(SinkModelTest, TestOneWrite) {
    PfylTaskCreated message = PfylTaskCreated_init_zero;
    uint8_t buffer[128];
    size_t message_length;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    const char *taskName = "test_task1";
    message.taskName.arg = (void*)taskName;
    message.taskName.funcs.encode = &write_test_task_name;

    bool status = pb_encode(&stream, PfylTaskCreated_fields, &message);;
    message_length = stream.bytes_written;
    printf("%lu", message_length);
//
//    SimpleMessage message = SimpleMessage_init_zero;
//
//    /* Create a stream that will write to our buffer. */
//    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
//
//    /* Fill in the lucky number */
//    message.lucky_number = 13;
//
//    /* Now we are ready to encode the message! */
//    status = pb_encode(&stream, SimpleMessage_fields, &message);
//    message_length = stream.bytes_written;
}