#include "common.c"
#include "parse.c"


void assert_sizes() {
    ASSERT_EQ(sizeof(char), 1);
    ASSERT_EQ(sizeof(int8_t), 1);
    // An instruction struct is 16 bytes, because an instruction is encoded as
    // anywhere between 1 up to 9 bytes, and in C, we want some padding when
    // using 9 bytes.
    ASSERT_EQ(sizeof(Inst), 16);
}


Inst read_inst(FILE* input) {
    Inst inst;
    ErrorMsg err = parse_inst(input, &inst);
    if (err.msg) {
        fprintf(stderr, "%s\n", err.msg);
        // We don't care about freeing because the program is done
        exit(1);
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
// 1. Better text input (parse arguments according to the instruction)
// 2. Disassembling
// 3. Debugging
// 4. More instructions!
// 5. Tests
// 6. Some example programs
// 7. Benchmarks
// 8. Translating to c/LLVM
// 9. Cli

int main() {
    assert_sizes();
    start_loop();
    return 0;
}
