//
// Created by mohamad on 30.01.20.
//

#ifndef F7_DEVICE_ITM_BOOTSTRAP_H
#define F7_DEVICE_ITM_BOOTSTRAP_H

#include <stm32f7xx_hal.h>

#ifndef TRC_CFG_ITM_PORT
#define TRC_CFG_ITM_PORT 1
#endif


static inline void initITM() {
    /* Aktiviere TRACE */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    /* Port 0 und 3 aktivieren */
    ITM->TER |= 9;
}

#endif //F7_DEVICE_ITM_BOOTSTRAP_H
