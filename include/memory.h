#pragma once

#include "commons.h"
#include <stdlib.h>
#define GROW_ARRAY(type, pointer, oldcount, newcount) \
    (type*)reallocate(pointer, sizeof(type)*(oldcount), sizeof(type)*(newcount))
#define FREE_ARRAY(type, pointer, oldcount) \
    (type*)reallocate(pointer, sizeof(type)*(oldcount), sizeof(type)*(0))
inline void *reallocate(void *pointer, size_t oldsize, size_t newsize){
    (void) oldsize;
    if(newsize == 0){
        free(pointer);
        return NULL;
    }
    void* result = realloc(pointer, newsize);
    return result;
}