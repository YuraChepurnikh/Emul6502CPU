#ifndef FLAGS_6502_H
#define FLAGS_6502_H

#include <stdint.h>

/* 
*  6502 uses eight flags to describe its state.
*  For example, if an overflow occurs, we set the
*  V flag to logical one. The order of the flags 
*  is fixed by hardware and cannot be changed in
*  any way!
*/

#define FLAGS_6502_LIST      \
    X(C, 0) /* Carry     */  \
    X(Z, 1) /* Zerro     */  \
    X(I, 2) /* Interrupt */  \
    X(D, 3) /* Decimal   */  \
    X(B, 4) /* Break     */  \
    X(U, 5) /* Unused    */  \
    X(V, 6) /* Overflow  */  \
    X(N, 7) /* Negative  */  

typedef enum Flags6502
{
    #define X(name, bit) FLAG_##name = (1 << bit),
        FLAGS_6502_LIST
    #undef X
}
Flags6502;

void flag_name(uint8_t status);

#endif // FLAGS_6502_H