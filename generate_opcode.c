#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/// A case in the opcode enum
typedef struct Case {
    const char* const name;
    const uint8_t n_bytes;
} Case;

/// zero-terminated array of enum cases.
Case CASES[] = {
    { "EXIT",       1   },
    { "SMALL_PUSH", 1   },
    { "PUSH",       2   },
    { "SMALL_ADD",  1   },
    { "ADD",        2   },
    { "PRINT",      1   },
    { 0 },
};

/// The number of opcodes
const uint32_t N_OPCODES = sizeof(CASES) / sizeof(Case) - 1 /* for the terminating 0 */ ;

/// Macro for iterating all cases
#define FOR_CASE(VAR, DO) \
    for (int i = 0; i < N_OPCODES; i++) { \
        Case VAR = CASES[i]; \
        DO; \
    }

#define TAB "    " /* 4 spaces! */

int main() {
    printf(
        "#include <stdbool.h>\n"
        "#include <inttypes.h>\n"
        "\n"
    );

    // Output the typedef
    printf("typedef enum Opcode {\n");
    FOR_CASE(c, printf(TAB "%s,\n", c.name));
    printf("} Opcode;\n");
    
    printf("#define N_OPCODES %u\n", (uint32_t)N_OPCODES);

    printf(
        "uint8_t opcode_n_bytes(Opcode op) {\n"
        TAB "switch (op) {\n"
    );
    FOR_CASE(c,
        printf(TAB TAB "case %s: return %d;\n", c.name, (int)c.n_bytes);
    )
    printf(
        TAB "}\n"
        "}\n"
    );

    printf("const Opcode OPCODE_ALL[] = {\n");
    FOR_CASE(c, printf(TAB "%s,\n", c.name));
    printf("};\n");

    printf("const char* OPCODE_NAMES[] = {\n");
    FOR_CASE(c, printf(TAB "\"%s\",\n", c.name));
    printf("};\n");
    return 0;
}
