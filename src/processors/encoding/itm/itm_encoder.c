//
// Created by mohamad on 30.01.20.
//
#ifdef ITM_ENCODER_ENABLED
#include <flow_helper.h>
#include <raw_sink.h>
#include "itm_bootstrap.h"

static inline void transfer_over_itm(uint8_t* buf) {
    for(int i = 0; i<SINK_ENTITY_SIZE; i++) {
        while (ITM->PORT[TRC_CFG_ITM_PORT].u32 == 0);     // Block until room in ITM FIFO
        ITM->PORT[TRC_CFG_ITM_PORT].u8 = buf[i];           // Write the data

    }
}
uint8_t push_sink_entity(const sink_entity *se) {
#ifdef ITM_PREFLIGHT_CHECK
    if   ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)  &&      // Trace enabled
          (ITM->TCR & ITM_TCR_ITMENA_Msk)                  &&      // ITM enabled
          (ITM->TER & (1UL << TRC_CFG_ITM_PORT)))                  // ITM port enabled
    {
        return 0;
    }
#endif
#ifndef ITM_USE_BUFFER
    transfer_over_itm((uint8_t*) se);
#else
    return write_to_buffer(se);
#endif
    return 1;
}

uint32_t flush_buffer() {
#ifdef ITM_USE_BUFFER
    uint32_t bytes_written = 0;
    const uint8_t *buf = getBuffer();
    uint32_t buf_size = getBufferSize();

    pfyl_begin_transfer();
    while (bytes_written < buf_size) {
        transfer_over_itm(buf);
        bytes_written += SINK_ENTITY_SIZE;
    }
    clearBuffer();
    pfyl_end_transfer();
#endif
    return 0;
}
#endif