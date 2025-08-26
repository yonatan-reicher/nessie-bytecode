#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef enum Opcode : uint8_t {
    /// Push a 4 bit value
    SMALL_PUSH,
    /// Push a 12 bit value
    PUSH,
    /// Add the two values on top and a 4 bit value
    SMALL_ADD,
    /// Add the two values on top and a 12 bit value
    ADD,
} Opcode;


bool opcode_has_arg2(Opcode op) {
    switch (op) {
        case SMALL_PUSH: return false;
        case PUSH: return true;
        case SMALL_ADD: return false;
        case ADD: return true;
    }
}


/// An instruction
typedef struct {
    Opcode op : 4;
    // arg1 is unsigned because there is no point in a 4-bit field
    uint8_t arg1 : 4;
    // arg2 may be read as signed or unsigned
    union {
        uint8_t s;
        int8_t u;
    } arg2;
} Inst;

#define PRInst "04"PRIx16 /* Print a 16-bit int with all 4 digits */

/// Return the concatanation of arg1 and arg2
uint16_t arg12(Inst inst) {
    return (inst.arg1 << 8) + inst.arg2.u;
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
    ASSERT_EQ(sizeof(Inst), 2);
}


Inst read_inst(FILE* input) {
    Inst inst;
    size_t n_read = fread(&inst, 1, 1, input);
    if (n_read != 1) {
        fprintf(stderr, "Error! could not read instruction from input\n");
        exit(1);
    }
    bool read_more = opcode_has_arg2(inst.op);
    if (read_more) {
        n_read = fread(&inst.arg2.u, 1, 1, input);
        if (n_read != 1) {
            fprintf(stderr, "Error! could not read instruction from input\n");
            exit(1);
        }
    }
    return inst;
}

void start_loop() {
    // TODO: Bound handling!
    int* stack = (int*)calloc(1000, sizeof(int));
    size_t i = 0;
    while(1) {
        Inst inst = read_inst(stdin);
        printf("inst %"PRInst"\n", inst);
        printf("opcode %"PRIx8"\n", inst.op);
        printf("arg1 %"PRIu8"\n", inst.arg1);
        printf("arg2 %"PRIu8"\n", inst.arg2.u);
        switch(inst.op) {
        case SMALL_PUSH:
            stack[i++] = inst.arg1;
            break;
        case PUSH:
            stack[i++] = arg12(inst);
            break;
        case SMALL_ADD:
            stack[i - 1] += stack[i] + inst.arg1;
            i--;
            break;
        case ADD:
            stack[i - 1] += stack[i] + arg12(inst);
            i--;
            break;
        }
        fprintf(stderr, "Bad instruction! %"PRInst"\n", inst);
        exit(1);
    }
}

int main() {
    assert_sizes();
    printf("Hello world!\n");
    start_loop();
    return 0;
}
