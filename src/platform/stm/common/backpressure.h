//
// Created by mohamad on 30.01.20.
//

#ifndef F7_DEVICE_BACKPRESSURE_H
#define F7_DEVICE_BACKPRESSURE_H

#ifndef DEFAULT_BACKPRESSURE_IRQ
#define DEFAULT_BACKPRESSURE_IRQ EXTI1_IRQn
#endif
#ifndef DEFAULT_BACKPRESSURE_IRQ_LEVEL
#define DEFAULT_BACKPRESSURE_IRQ_LEVEL 3
#endif

void activateBackpressure();
#endif //F7_DEVICE_BACKPRESSURE_H