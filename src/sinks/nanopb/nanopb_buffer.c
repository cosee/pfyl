//
// Created by Mohamad Ramadan on 10.03.21.
//

#include <stddef.h>
#include <string.h>

#ifdef PFYL_USE_FREERTOS
#include <stm32f7xx_hal_cortex.h>
#include <backpressure.h>

#endif
#include "nanopb_buffer.h"

#undef SINK_BUFFER_SIZE
#define SINK_BUFFER_SIZE 2000
static uint8_t sink_buffer[SINK_BUFFER_SIZE] = {0};
static uint32_t write_index = 0;
static uint8_t lock = 0;

uint8_t aquire_lock() {
    register uint8_t lock_value_register;
    register uint8_t lock_value;
#ifdef PFYL_PLATFORM_ARM
//    do {
//        __asm__ __volatile__(
//        "MOV %[lock_value],#1;"
//        "LDREXB %[lock_value_register],[%2];"
//        "CMP %[lock_value_register], #0;"
//        "STREXBEQ %[lock_value_register], %[lock_value], [%2];"
//        "CMPEQ %[lock_value_register], #0;"
//        "DMBEQ;"
//        : [lock_value] "=&r"(lock_value), [lock_value_register] "=&r"(lock_value_register)
//        :"r"(&lock)
//        );
//    } while(lock_value_register != 0);
#endif
    return lock_value;
}

void release_lock() {
    register uint8_t lock_value;
#ifdef PFYL_PLATFORM_ARM
    __asm__ __volatile__(
    "MOV %[lock_value],#0;"
    "DMB;"
    "STR %[lock_value], [%1];"
    : [lock_value] "=&r"(lock_value)
    :"r"(&lock)
    );
#endif
}

int write_to_buffer(const void *buf, size_t bufSize) {
    if (buf == 0) {
        return -1;
    }

    if (write_index + bufSize >= SINK_BUFFER_SIZE) {
#ifdef BACKPRESSURE_ENABLED
        HAL_NVIC_SetPendingIRQ(DEFAULT_BACKPRESSURE_IRQ);
#else
        return 1;
#endif
    }
    aquire_lock();
    memcpy(sink_buffer + write_index, buf, bufSize + 1);
    write_index += bufSize + 1;
    release_lock();
    return 0;
}

const uint8_t *getNBBuffer() {
    aquire_lock();
    return (const uint8_t *) &sink_buffer;
}

uint32_t getBufferSize() {
    return write_index;
}

void clearBuffer() {
    memset(sink_buffer, 0, SINK_BUFFER_SIZE);
    write_index = 0;
    release_lock();
}