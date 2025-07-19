#include <stdio.h>
#include <stdbool.h>

#include "flags6502.h"
#include "types6502.h"

void flag_name(Byte status)
{
    bool none = true;
    
    #define X(name, bit) \
        if (status & (1 << bit)) \
        {                        \
            printf(#name " ");   \
            none = false;        \
        }     
        FLAGS_6502_LIST          
    #undef X

    if (none)
    {
        printf("No flags set");

    }
    printf("\n");
}