#include <stdbool.h>
#include <inttypes.h>

typedef enum Opcode {
    EXIT,
    SMALL_PUSH,
    PUSH,
    SMALL_ADD,
    ADD,
    PRINT,
} Opcode;
#define N_OPCODES 6
uint8_t opcode_n_bytes(Opcode op) {
    switch (op) {
        case EXIT: return 1;
        case SMALL_PUSH: return 1;
        case PUSH: return 2;
        case SMALL_ADD: return 1;
        case ADD: return 2;
        case PRINT: return 1;
    }
}
const Opcode OPCODE_ALL[] = {
    EXIT,
    SMALL_PUSH,
    PUSH,
    SMALL_ADD,
    ADD,
    PRINT,
};
const char* OPCODE_NAMES[] = {
    "EXIT",
    "SMALL_PUSH",
    "PUSH",
    "SMALL_ADD",
    "ADD",
    "PRINT",
};
