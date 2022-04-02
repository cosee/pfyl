//
// Created by Mohamad Ramadan on 10.03.21.
//

#ifndef F7_DEVICE_NANOPB_BUFFER_H
#define F7_DEVICE_NANOPB_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif


int pfylSinkWrite(const void *buf, size_t bufSize);
const void* pfylGetSink();
uint32_t pfylGetContentLength();
uint32_t pfylGetSinkSize();
void pfylClearSink();

#ifdef __cplusplus
}
#endif
#endif //F7_DEVICE_NANOPB_BUFFER_H