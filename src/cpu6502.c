#include <stdlib.h>
#include <string.h>

#include "cpu6502.h"
#include "safe_free.h"
#include "types6502.h"

CPU6502* create_cpu(void)
{   
    CPU6502* cpu6502 = (CPU6502*) malloc(sizeof(CPU6502));

    if (!cpu6502)
    {
        fprintf(stderr, "[ERROR] Failed to allocate memory for CPU structure\n");
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

    return cpu6502;
}

void destroy_cpu(CPU6502** cpu)
{
    if (!cpu || !(*cpu))
    {
        return;
    }

    free(*cpu);
    *cpu = NULL;
}

Byte fetch_byte(CPU6502 *cpu, Byte addr)
{
    return mem_read(cpu->bus, addr);
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

Byte IMP(CPU6502* cpu) 
{
    // Backup even if AСС is not used in the instruction
    cpu->fetch = cpu->ACC;
    return 0;
}

/* 
 * IMM (Immediate mode) - This is an addressing mode in which the operand
 * is specified directly in the byte following the instruction and is interpreted
 * as a literal value rather than an address.
 * 
 * For example
 * LDA #$10 
 * 
 * LDA - Load value to accumulator.
 * #$10 - Load value 10 (The # symbol indicates that the value
 * should be used directly, not as an address).
 * 
 * This is a two-byte command 
 * The first byte is the Opcode (LDA)
 * The second byte is the Immediate value (#$10)
 * 
 * The first byte had already been read before IMM, so
 * 
 * 1) We need to save the value 
 * 2) Increase the PC so that it points to the next instruction, otherwise the CPU 
 *    will think that our next opcode is the Immediate value
 */

Byte IMM(CPU6502* cpu)
{
    // Record the value
    cpu->addr_abs = cpu->PC; 

    // point to the next instruction
    cpu->PC++; 

    // no extra cycles needed
    return 0; 
}

/*
* Processors use the concept of pages to increase efficiency. 
* For example, the 6502 has a 16-bit address bus, and we can divide 16 bits
* into 2 bytes, where the first byte is the page number and the second byte 
* is the offset within that page. Since the 6502 has a 16-bit address bus, 
* it can address up to 64Kb of information, which can be represented as 
* 256 pages of 256 bytes of memory.
* 
* ZP0 (Zero Page Addressing) - This is an addressing mode where
* addressing occurs on the zero page, i.e., the address we are interested 
* in is somewhere on the zero page and therefore the high byte is zero.
* For this reason, programmers often place the working part of the memory
* on the zero page, as this allows them to access these bytes using instructions
* that require fewer bytes. An instruction consists of bytes, each of which
* requires processor cycles to read, and we use this trick, this clever moment, 
* to immediately read the low byte from the zero page, i.e., no
* additional cycle is required to read the high byte, 
* and this saves us processor time, where each cycle costs memory and time. 
* 
* Essentially, if we write LDA $42, thanks to ZP0, the compiler will understand 
* that we are dealing with an address in the zero page (0x0042), but we can also
* write the full address LDA $0042, and thus flush everything down the toilet, 
* as the compiler will waste a clock cycle reading the high byte, which could 
* have been avoided.
*/

Byte ZP0(CPU6502 *cpu)
{
    cpu->addr_abs = fetch_byte(cpu, cpu->PC);

    cpu->PC++;

    // Just in case, we reset the High byte to zero
    cpu->addr_abs &= 0x00FF;

    // No extra cycles needed
    return 0;
}

/* 
 * ZPX (Zero Page Addressing With X Register Offset) - This is an addressing mode 
 * within the zero page where the X register is used as an offset. It is convenient
 * for iterating through memory areas and can be compared to arrays in the C language, 
 * where the array has a base address, and the index offsets into this array. 
*/

Byte ZPX(CPU6502* cpu)
{
    cpu->addr_abs = fetch_byte(cpu, cpu->PC) + cpu->X;

    cpu->PC++;

    cpu->addr_abs &= 0x00FF;

    return 0;
}

/* 
 * ZPY (Zero Page Addressing With Y Register Offset) - Essentially the same as ZPX, 
 * except that the Y register is used for the offset instead of the X register.
*/

Byte ZPY(CPU6502* cpu)
{
    cpu->addr_abs = fetch_byte(cpu, cpu->PC) + cpu->Y;

    cpu->PC++;

    cpu->addr_abs &= 0x00FF;

    return 0;
}

/* 
 * ABS (Absolute Addressing) - This is an addressing mode 
 * where we provide the address in its full form, 
 * i.e. including the high and low bytes.   
 */

Byte ABS(CPU6502* cpu)
{
    /*
     * Since the 6502 is a little-endian machine, 
     * we first read the lower bytes and then the higher bytes.
     */

    Byte low_byte = fetch_byte(cpu, cpu->PC);
    cpu->PC++;

    Byte high_byte = fetch_byte(cpu, cpu->PC);
    cpu->PC++;

    cpu->addr_abs = (high_byte << 8) | low_byte;

    return 0;
}

/* 
 * ABS (Absolute Addressing With X Register Offset) - This is an addressing mode
 * where we provide the address in full form with a register offset X, i.e. 
 * including the most significant and least significant bytes.   
 */

Byte ABX(CPU6502* cpu)
{    
    Byte low_byte = fetch_byte(cpu, cpu->PC);
    cpu->PC++;

    Byte high_byte = fetch_byte(cpu, cpu->PC);
    cpu->PC++;

    cpu->addr_abs = (high_byte << 8) | low_byte;

    cpu->addr_abs += cpu->X;

    /*
     * If, after increasing the base address by the X register offset,
     * the address has moved to another page, we will need an additional processor cycle.
     * We determine this by the high byte; if it has changed,
     * then an overflow has occurred (the carry bit from the low byte has carried into the high byte).
     */

    if ((cpu->addr_abs & 0xFF00) != (high_byte << 8))
    {
        return 1; // We need one extra cycle
    }

    else
    {
        return 0;
    }
}