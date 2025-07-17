#include "safe_free.h"

void safer_free(void** ptr, const char* file, int line)
{
    (void) file;
    (void) line;

    assert(ptr && *ptr && "NULL pointer passed");

    if (ptr && *ptr)
    {
        SAFE_FREE_LOG(ptr);
        free(*ptr);
        *ptr = NULL;
    }
}