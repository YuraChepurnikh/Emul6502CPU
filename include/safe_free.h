#ifndef SAFE_FREE_H
#define SAFE_FREE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define EN_SAFE_FREE_LOG

#ifdef EN_SAFE_FREE_LOG
    #define SAFE_FREE_LOG(ptr) \
        printf("[safe_free] Release address: %p (%s,%d)\n", (void*)(*(ptr)), __FILE__, __LINE__)
#else 
    #define SAFE_FREE_LOG(ptr) /* Empty definition */
#endif

void safer_free(void** ptr, const char* file, int line);

#define safe_free(ptr) safer_free((void**)&(ptr), __FILE__, __LINE__)

#endif // SAFE_FREE_H