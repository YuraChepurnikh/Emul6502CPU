#include "bus.h"
#include "types6502.h"

Bus* create_bus(void)
{
    Bus *bus = (Bus*) malloc(sizeof(Bus));

    if (!bus)
    {
        fprintf(stderr, "[ERROR] Failed to allocate memory for Bus structure\n");
        return NULL;
    }

    bus->RAM = (uint8_t*) malloc(RAM_SIZE_BYTES * sizeof(uint8_t));

    if (!bus->RAM)
    {
        fprintf(stderr, "[ERROR] Failed to allocate 64KB RAM\n");
        free(bus);
        return NULL;
    }

    memset(bus->RAM, 0, RAM_SIZE_BYTES);

    bus->cpu = create_cpu();

    if (!bus->cpu)
    {
        fprintf(stderr, "[ERROR] Failed to create CPU\n");
        free(bus->RAM);
        free(bus);
        return NULL;
    }

    bus->cpu->bus = bus;

    return bus;
}

void destroy_bus(Bus **bus)
{
    if (!bus || !(*bus))
    {
        return;
    }

    free((*bus)->RAM);
    free(*bus);
    *bus = NULL;
}

Byte mem_read(Bus* bus, Word addr)
{
    return bus->RAM[addr];
}