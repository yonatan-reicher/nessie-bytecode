#ifndef COMMON_C
#define COMMON_C

// This flag enables GNU extensions like 'asprintf'.
#define _GNU_SOURCE

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcode.c"

/// An instruction. The instruction is not quite represented like this in the
/// encoding, because of alignment. The difference is that, when encoded, the
/// instruction does not have padding between arg1 and arg2.
typedef struct Inst {
    Opcode op : 4;
    // arg1 is unsigned because there is no point in a 4-bit field
    uint8_t arg1 : 4;
    // arg2 may be read as signed or unsigned
    union {
        uint8_t s8;
        int8_t u8;
        uint16_t s16;
        int16_t u16;
        uint32_t s32;
        int32_t u32;
        uint64_t s64 : 60;
        int64_t u64 : 60;
    } arg2;
} Inst;

/// Return the concatanation of arg1 and arg2 , when arg2 is 8 bits.
uint16_t arg12(Inst inst) {
    return (inst.arg1 << 8) + inst.arg2.u8;
}

#define ASSERT_EQ(L, R) assert_eq((L), (R), #L, #R)
void assert_eq(
    intmax_t left, intmax_t right,
    const char* left_str, const char* right_str
) {
    if (left != right) {
        fprintf(
            stderr,
            "Assertion failure: "
            "%s != %s "
            "(%"PRIdMAX" != %"PRIdMAX")\n",
            left_str, right_str,
            left, right
        );
        exit(1);
    }
}

typedef bool success;

typedef struct ErrorMsg {
    bool owned; /* If this is set true, the string should be free'd */
    char* msg;
} ErrorMsg;

#endif
