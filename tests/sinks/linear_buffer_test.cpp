//
// Created by Mohamad Ramadan on 01.04.22.
//

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "pfyl_sink.h"
#include "SinkModel.pb.h"

TEST_GROUP(LinearBufferTestGroup) {
    void teardown() {
        pfylClearSink();
    }
};

TEST(LinearBufferTestGroup, TestSinkWriteWithText) {
    const char testData[] = {"test text for test simple write"};
    pfylSinkWrite(testData, strlen(testData));

    STRCMP_EQUAL(static_cast<const char *>(pfylGetSink()), testData);
    CHECK_EQUAL(pfylGetContentLength(), strlen(testData) + 1);
    CHECK_EQUAL(pfylGetSinkSize(), 2000);
    pfylClearSink();
    CHECK_EQUAL(pfylGetContentLength(), 0);
    STRCMP_EQUAL(static_cast<const char *>(pfylGetSink()), "");
}

TEST(LinearBufferTestGroup, TestSinkWriteWithoutData) {
    pfylSinkWrite(nullptr, 0);

    CHECK_EQUAL(pfylGetContentLength(), 0);
}

TEST(LinearBufferTestGroup, TestSinkWriteWithIncorrectSize) {
    pfylSinkWrite(nullptr, SIZE_MAX);

    CHECK_EQUAL(pfylGetContentLength(), 0);
}

TEST(LinearBufferTestGroup, TestSinkWriteDoesNotCopyReference) {
    const char testData[] = {"test text for test simple write"};
    pfylSinkWrite(testData, strlen(testData));
    CHECK_FALSE(pfylGetSink() == testData);
}
