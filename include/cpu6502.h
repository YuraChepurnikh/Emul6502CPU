#ifndef CPU_6502_H
#define CPU_6502_H

#include <stdint.h>

#define RAM_SIZE_KB 64
#define RAM_SIZE_BYTES (RAM_SIZE_KB * 1024)

// The 6502 is a simple 8-bit processor

/* 
 * The 6502 has an 8-bit data bus, which means that the 6502
 * can read or write one byte at a time.
 */

/* 
 * The address bus is 16-bit, so in order for the 6502 to 
 * read a 16-bit address, for example, it must 
 * perform two read operations, one 
 * byte at a time 
 */

typedef struct CPU6502
{
    /* 
     * Program Counter (PC) A 16-bit register that indicates the address of 
     * the next instruction to be executed by the processor.
     * Since the 6502 uses 16-bit addressing, it can 
     * address up to 64KB of memory, which is why the PC register 
     * has a bit width of 16 bits. 
     */

    uint16_t PC; 

    /* 
     * Stack Pointer (SP) - This is an 8-bit register exclusively for working with the stack, 
     * since the stack occupies addresses 0x0100 - 0x01FF. In order to obtain the necessary 
     * address, the 6502 adds the SP address to the base address 0x0100. 
     */

    uint8_t SP;

    // Registers for general arithmetic operations Flags6502
    uint8_t X, Y, ACC;

    // 6502 flags
    uint8_t status;

    // 64Kb RAM
    uint8_t* RAM;

    // Helper variables for emulation
    uint8_t fetch;
    uint16_t addr_rel, addr_abs;

} CPU6502;

// create/destroy
CPU6502* create_cpu();
void destroy_cpu(CPU6502 **cpu);

// Addressing modes
uint8_t IMP(CPU6502* cpu);


#endif // CPU_6502_H