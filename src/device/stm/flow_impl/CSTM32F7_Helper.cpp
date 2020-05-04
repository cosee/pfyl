//
// Created by mohamad on 09.02.20.
//


#include <flow_helper.h>
#include "FreeRTOS.h"
#include <task.h>
#include <stm32f7xx_hal_uart.h>
#include "CSTM32F7_Helper.h"


uint8_t CSTM32F7_Helper::push_sink_entity(const sink_entity *entity) {
    return 0;
}

uint32_t CSTM32F7_Helper::flush_buffer() {
    return 0;
}

void CSTM32F7_Helper::pfyl_begin_transfer() {
#if defined(BACKPRESSURE_ENABLED)
    tempUxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
#else
    taskENTER_CRITICAL();
#endif
}

uint32_t CSTM32F7_Helper::pfyl_transfer(uint8_t *buf, size_t size) {
    return HAL_UART_Transmit(uart, buf, size, HAL_UART_TIMEOUT_VALUE);
}

void CSTM32F7_Helper::pfyl_end_transfer() {
#if defined(BACKPRESSURE_ENABLED)
    taskEXIT_CRITICAL_FROM_ISR(tempUxSavedInterruptStatus);
#else
    taskEXIT_CRITICAL();
#endif
}

void CSTM32F7_Helper::setUART(UART_HandleTypeDef * temp_uart) {
    this->uart = temp_uart;
}
