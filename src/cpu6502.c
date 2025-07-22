#include <stdlib.h>
#include <string.h>

#include "cpu6502.h"
#include "safe_free.h"

CPU6502* create_cpu()
{   
    CPU6502* cpu6502 = (CPU6502*) malloc(sizeof(CPU6502));

    if (!cpu6502)
    {
        fprintf(stderr, "[ERROR] Failed to allocate memory for CPU structure\n");
        return NULL;
    }

    cpu6502->RAM = (uint8_t*) malloc(RAM_SIZE_BYTES * sizeof(uint8_t));

    if (!cpu6502->RAM)
    {
        fprintf(stderr, "[ERROR] Failed to allocate 64KB RAM\n");
        free(cpu6502);
        return NULL;
    }

    cpu6502->PC = 0x0000;
    
    /* 
     * Since the stack grows from large addresses to
     * small ones, at reset we set SP according to the 6502 specification to
     * 0x01FD (we do not set it to 0x01FF, since addresses
     * 0x01FD - 0x01FF are reserved for service data).
    */

    cpu6502->SP = 0xFD;

    cpu6502->X = cpu6502->Y = cpu6502->A = 0;

    // 0x34 - set U, B, ans I
    cpu6502->status = 0x34;

    memset(cpu6502->RAM, 0, RAM_SIZE_BYTES);

    return cpu6502;
}

void destroy_cpu(CPU6502** cpu)
{
    safe_free((*cpu)->RAM);
    safe_free(cpu);
}