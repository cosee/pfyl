//
// Created by mohamad on 30.01.20.
//


#include <stm32f7xx_hal.h>
#include <sink_entity.h>
#include <cmsis_os.h>
#include <flow_helper.h>

static sink_entity fault_se = {
        .type=PFYL_ENTITY_TYPE_FAULT_HANDLER,
        .tick=0,
        .data = 0,
        .data2=0,
        .data3=0
};


void __attribute__((no_instrument_function))  HardFault_Handler(void) {
    __asm__ __volatile__ (
    "TST lr, #4\n"
    "ITE EQ \n"
    "MRSEQ r0, MSP\n"
    "MRSNE r0, PSP\n"
    "b fault_handler"
    );
}

void fault_handler(const uint32_t stack[]) {
    fault_se.tick = reference_tick;
    fault_se.data = stack[5];
    alert_sink_entity(&fault_se);
}

void __attribute__((no_instrument_function))  SysTick_Handler(void) {
    HAL_IncTick();
    osSystickHandler();
#ifndef USE_HW_TIMER
    increase_tick();
#endif
}

