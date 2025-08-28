#include "common.c"


#define WORD_SIZE 100
#define STR_0(X) #X
#define STR(X) STR_0(X) // Call a helper to make sure X is expanded already.


static bool is_letter(char c) {
    return 'a' <= c && c <= 'z';
}


static void str_uppercase(char* str) {
    for (; *str != 0; str++) {
        if (is_letter(*str)) {
            *str += 'A' - 'a';
        }
    }
}


static success read_int(FILE* input, int* i) {
    int n_read = scanf("%d", i);
    return n_read == 1;
}


static success read_word(
    FILE* input,
    char word[WORD_SIZE+1 /* 1 for zero-terminator */]
) {
    int n_read = scanf("%" STR(WORD_SIZE) "s", word);
    return n_read == 1;
}


static ErrorMsg parse_opcode(FILE* input, Opcode* inst) {
    char word[WORD_SIZE + 1] = {0};
    if (!read_word(input, word)) return (ErrorMsg){ false, "an instruction should start with a name" };
    str_uppercase(word);
    // Look for the opcode
    for (size_t i = 0; i < N_OPCODES; i++) {
        if (strcmp(word, OPCODE_NAMES[i]) == 0) {
            *inst = OPCODE_ALL[i];
            return (ErrorMsg) { 0 };
        }
    }
    // Return an error
    char* msg;
    asprintf(&msg, "'%s' is not an opcode name!", word);
    return (ErrorMsg) { true, msg };
}


/// Read an instruction in text format from an input stream.
ErrorMsg parse_inst(FILE* input, Inst* inst) {
    Opcode opcode = 0;
    int arg1 = 0;
    ErrorMsg em = parse_opcode(input, &opcode);
    if (em.msg) return em;
    inst->op = opcode;
    if (!read_int(input, &arg1)) return (ErrorMsg) { false, "should have a 4-bit unsigned integer after the opcode name" };
    if (arg1 < 0 || arg1 > 15) return (ErrorMsg) { false, "arg1 should be a 4-bit unsigned integer" };
    inst->arg1 = arg1;

    if (opcode_n_bytes(opcode) > 1) {
        int arg = 0;
        if (!read_int(input, &arg)) {
            char* msg;
            asprintf(&msg,
                "opcode '%s' expects an extra argument",
                OPCODE_NAMES[inst->op]
            );
            return (ErrorMsg) { true, msg };
        }
        inst->arg2.s64 = arg;
    }

    /* This is code for reading as multiple 8-bit arguments
    // Now read extra arguments
    for (uint8_t i = 1; i < opcode_n_bytes(opcode); i++) {
        int arg = 0;
        if (!read_int(input, &arg)) {
            char* msg;
            asprintf(&msg,
                "opcode '%s' expects %d arguments, failed to read the %dth argument",
                OPCODE_NAMES[inst->op],
                opcode_n_bytes(opcode),
                i + 1
            );
            return (ErrorMsg) { true, msg };
        }
        // TODO: Check that arg fits in an int8_t
        *(&inst.arg2.u8 + i - 1) = arg;
    }
    */
    return (ErrorMsg) { 0 };
}
