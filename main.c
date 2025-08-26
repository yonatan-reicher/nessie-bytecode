#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.c"

/// An instruction. The instruction is not quite represented like this in the
/// encoding, because of alignment. The difference is that, when encoded, the
/// instruction does not have padding between arg1 and arg2.
typedef struct {
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


void assert_sizes() {
    ASSERT_EQ(sizeof(char), 1);
    ASSERT_EQ(sizeof(int8_t), 1);
    ASSERT_EQ(sizeof(Opcode), 1);
    ASSERT_EQ(sizeof(Inst), 16);
}


Inst read_inst(FILE* input) {
    Inst inst;
    size_t n_read = fread(&inst, 1, 1, input);
    if (n_read != 1) {
        fprintf(stderr, "Error! could not read instruction from input\n");
        exit(1);
    }
    uint8_t n_bytes = opcode_n_bytes(inst.op);
    if (n_bytes > 1) {
        n_read = fread(&inst.arg2, n_bytes - 1, 1, input);
        if (n_read == 0) {
            fprintf(stderr, "Error! could not read instruction from input\n");
            exit(1);
        }
    }
    return inst;
}

void print_inst(FILE* f, Inst inst) {
    // fprintf(f, "inst: %x %x\n", *(uint32_t*)&inst, *(uint32_t*)&inst.arg2);
    fprintf(f, "opcode: %s\n", OPCODE_NAMES[inst.op]);
    fprintf(f, "arg1: %"PRIu8"\n", inst.arg1);
    uint8_t bytes = opcode_n_bytes(inst.op);
    // Clean the argument, so we don't print extra shit bits
    uint64_t arg2 = 0;
    memcpy(&arg2, &inst.arg2, bytes - 1);
    if (bytes > 1) {
    fprintf(f, "arg2: %"PRIu64"\n", arg2);
    }
}

void start_loop() {
    // TODO: Bound handling!
    int* stack = (int*)calloc(1000, sizeof(int));
    size_t i = 0;
    while(1) {
        Inst inst = read_inst(stdin);
        print_inst(stdout, inst);
        switch(inst.op) {
        case EXIT:
            exit(0);
            continue;
        case SMALL_PUSH:
            stack[i++] = inst.arg1;
            continue;
        case PUSH:
            stack[i++] = arg12(inst);
            continue;
        case SMALL_ADD:
            stack[i - 2] += stack[i - 1] + inst.arg1;
            i--;
            continue;
        case ADD:
            stack[i - 2] += stack[i - 1] + arg12(inst);
            i--;
            continue;
        case PRINT:
            printf("%d\n", stack[--i]);
            continue;
        }
        fprintf(stderr, "Bad instruction!");
        exit(1);
    }
}

// Some things that could be added to this project:
// 1. Text input
// 2. Disassembling
// 3. Debugging
// 4. More instructions!
// 5. Tests
// 6. Some example programs
// 7. Benchmarks
// 8. Translating to c/LLVM

int main() {
    assert_sizes();
    printf("Hello world!\n");
    start_loop();
    return 0;
}
