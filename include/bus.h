#ifndef BUS_H
#define BUS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu6502.h"

#define RAM_SIZE_KB 64
#define RAM_SIZE_BYTES (RAM_SIZE_KB * 1024)

typedef struct CPU6502 CPU6502;

typedef struct Bus
{
    // 64Kb RAM
    uint8_t* RAM;

    // CPU
    CPU6502* cpu;

} Bus;

// create/destroy
Bus* create_bus(void);
void destroy_bus(Bus **bus);

uint8_t mem_read(Bus* bus, uint16_t addr);

#endif // BUS_H