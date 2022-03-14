//
// Created by Mohamad Ramadan on 10.03.21.
//

#ifndef F7_DEVICE_NANOPB_BUFFER_H
#define F7_DEVICE_NANOPB_BUFFER_H



int write_to_buffer(const void *buf, size_t bufSize);
const uint8_t* getNBBuffer();
uint32_t getBufferSize();
void clearBuffer();

#endif //F7_DEVICE_NANOPB_BUFFER_H