#ifndef PFYL_HEAP_SOURCE_H
#define PFYL_HEAP_SOURCE_H

#ifdef __cplusplus
extern "C" {
#endif
extern void * __real_malloc(size_t size);
void * __wrap_malloc(size_t size);

void* pfyl_malloc(size_t size, void*);
extern void __real_free(void* ptr);
void __wrap_free(void* ptr);

void pfyl_free(void* ptr, void*);

#ifdef __cplusplus
};
#endif
#endif //PFYL_HEAP_SOURCE_H
