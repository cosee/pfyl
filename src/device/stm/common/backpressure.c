//
// Created by mohamad on 30.01.20.
//


#include <stm32f7xx.h>
#include <flow_helper.h>
#include "stm32f7xx_hal_conf.h"
#include "backpressure.h"

void activateBackpressure() {
    HAL_NVIC_SetPriority(DEFAULT_BACKPRESSURE_IRQ, DEFAULT_BACKPRESSURE_IRQ_LEVEL, 0);
    HAL_NVIC_EnableIRQ(DEFAULT_BACKPRESSURE_IRQ);
}

void __attribute__((no_instrument_function)) EXTI1_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DEFAULT_BACKPRESSURE_IRQ);
    flush_buffer();
}
