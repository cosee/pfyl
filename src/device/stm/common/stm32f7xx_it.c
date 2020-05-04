//
// Created by mohamad on 30.01.20.
//


#include <stm32f7xx_hal.h>
#include <sink_entity.h>
#include <cmsis_os.h>

void __attribute__((no_instrument_function))  HardFault_Handler(void) {
    __asm__ __volatile__ (
    "nop"
    );
}

void __attribute__((no_instrument_function))  SysTick_Handler(void) {
    HAL_IncTick();
    osSystickHandler();
#ifndef USE_HW_TIMER
    increase_tick();
#endif
}

