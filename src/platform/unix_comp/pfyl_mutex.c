//
// Created by Mohamad Ramadan on 19.04.22.
//
#include <stdbool.h>
#include "pfyl_mutex.h"
#include "sink_entity.h"
#include <pthread.h>
#include <stdlib.h>

void * createPfylMutex() {
    pthread_mutex_t* lock = malloc(sizeof(pthread_mutex_t));;
    if (pthread_mutex_init(lock, NULL) != 0)
    {
        return nullptr;
    }
    return lock;
}

bool aquireLock(void *mutexHandle) {
    return pthread_mutex_lock(mutexHandle);
}

bool releaseLock(void *mutexHandle) {
    int i = pthread_mutex_unlock(mutexHandle);
    return i;
}

bool deleteMutex(void* mutexHandle) {
    return pthread_mutex_destroy(mutexHandle);
}