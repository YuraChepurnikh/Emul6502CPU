#ifndef CPU_6502_H
#define CPU_6502_H

#include <stdint.h>
#include "bus.h"

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

typedef struct Bus Bus;

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

    // Helper variables for emulation
    uint8_t fetch;
    uint16_t addr_rel, addr_abs;

    // Pointer to Bus
    Bus* bus;

} CPU6502;

// create/destroy
CPU6502* create_cpu(void);
void destroy_cpu(CPU6502 **cpu);

// fetch byte
uint8_t fetch_byte(CPU6502 *cpu, uint8_t addr);

// Addressing modes
uint8_t IMP(CPU6502* cpu);
uint8_t IMM(CPU6502* cpu);
uint8_t ZP0(CPU6502* cpu);
uint8_t ZPX(CPU6502* cpu);
uint8_t ZPY(CPU6502* cpu);

#endif // CPU_6502_H