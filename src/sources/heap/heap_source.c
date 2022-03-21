#include <stdio.h>
#include <stdlib.h>
#include "heap_source.h"
#include "flow_helper.h"

static sink_entity heap_se = {
        .type=PFYL_ENTITY_TYPE_MALLOC,
        .tick=0,
        .data = 0,
        .data2=0,
        .data3=0
};

void *__wrap_malloc(size_t bytes) {
    return pfyl_malloc(bytes, __builtin_return_address(0));
}

void *pfyl_malloc(size_t size, void *return_address) {
    void *memory_address = malloc(size);
    updateSinkEntity(&heap_se, PFYL_ENTITY_TYPE_MALLOC, (uint64_t) memory_address, (uint64_t) size,
                     (uint64_t) return_address);
    push_sink_entity(&heap_se);
    return memory_address;
}


void __wrap_free(void *ptr) {
    return pfyl_free(ptr, __builtin_return_address(0));
}

void pfyl_free(void *ptr, void *return_address) {
    free(ptr);
    updateSinkEntity(&heap_se, PFYL_ENTITY_TYPE_FREE, (uint64_t) ptr, 0, (uint64_t) return_address);
    push_sink_entity(&heap_se);
}
