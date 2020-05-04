//
// Created by mohamad on 17.01.20.
//
#include "flow_helper.h"
#include <cmsis_gcc.h>

static sink_entity func_se = {
        .type=PFYL_ENTITY_TYPE_FUNC_ENTER,
        .tick=0,
        .data = 0,
        .data2=0,
        .data3=0
};

#ifdef __cplusplus
extern "C" {
#endif
void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void *current_func, void *callsite) {
    updateSinkEntity(&func_se, PFYL_ENTITY_TYPE_FUNC_ENTER, (uint64_t)current_func, (uint64_t)callsite, (uint64_t) __get_PSP());
    push_sink_entity(&func_se);
}

void __attribute__((no_instrument_function)) __cyg_profile_func_exit(void *current_func, void *callsite) {
    updateSinkEntity(&func_se, PFYL_ENTITY_TYPE_FUNC_EXIT, (uint64_t)current_func, (uint64_t)callsite, (uint64_t)__get_PSP());
    push_sink_entity(&func_se);
}

#ifdef __cplusplus
};
#endif