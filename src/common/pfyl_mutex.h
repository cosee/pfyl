//
// Created by Mohamad Ramadan on 19.04.22.
//

#ifndef TEST_PFYL_PFYL_MUTEX_H
#define TEST_PFYL_PFYL_MUTEX_H

void* createPfylMutex();
bool aquireLock(void* mutexHandle);
bool releaseLock(void* mutexHandle);
bool deleteMutex(void* mutexHandle);

#endif //TEST_PFYL_PFYL_MUTEX_H
