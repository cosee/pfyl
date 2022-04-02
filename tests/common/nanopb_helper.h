//
// Created by Mohamad Ramadan on 02.04.22.
//

#ifndef TEST_PFYL_NANOPB_HELPER_H
#define TEST_PFYL_NANOPB_HELPER_H

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
#endif //TEST_PFYL_NANOPB_HELPER_H
