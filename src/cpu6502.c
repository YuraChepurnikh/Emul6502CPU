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

    cpu6502->X = cpu6502->Y = cpu6502->ACC = 0;

    // 0x34 - set U, B, ans I
    cpu6502->status = 0x34;

    memset(cpu6502->RAM, 0, RAM_SIZE_BYTES);

    return cpu6502;
}

void destroy_cpu(CPU6502** cpu)
{
    if (!cpu || !(*cpu))
    {
        return;
    }

    free((*cpu)->RAM);
    free(*cpu);

    *cpu = NULL;
}

/* 
 * IMP (Implied Addressing) - This is an addressing mode 
 * where the instruction does not require an explicit operand,
 * since the operand is either not needed or is fixed within the command,
 * for example, in the accumulator (ACC) or in registers (X, Y). 
 * It is used for consistency with the architecture, 
 * acting as an empty placeholder when 
 * no special addressing mode is required, such as (ABX, ZPX, etc.).
 */

uint8_t IMP(CPU6502* cpu) 
{
    // Backup even if AСС is not used in the instruction
    cpu->fetch = cpu->ACC;
    return 0;
}
