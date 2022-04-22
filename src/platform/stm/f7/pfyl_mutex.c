//
// Created by Mohamad Ramadan on 22.04.22.
//


#include <stdbool.h>
#include "pfyl_mutex.h"
#include "sink_entity.h"
#include <FreeRTOS.h>
#include <semphr.h>

#ifndef configSUPPORT_DYNAMIC_ALLOCATION
    #error this mutex implementation relies on dynamic allocation
#endif

void * createPfylMutex() {
    SemaphoreHandle_t xSemaphore = xSemaphoreCreateBinary();
    return xSemaphore;
}

bool aquireLock(void *mutexHandle) {
    if( mutexHandle != NULL )
    {
        if( xSemaphoreTake( mutexHandle, ( TickType_t ) 10 ) == pdTRUE )
        {
           return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool releaseLock(void *mutexHandle) {
    if( mutexHandle != NULL ) {
        return xSemaphoreGive(mutexHandle);
    }
    return false;
}

bool deleteMutex(void* mutexHandle) {
    vSemaphoreDelete(mutexHandle);
    return true;
}