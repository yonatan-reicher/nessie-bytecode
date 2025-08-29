#include "common.c"


/// The kind of Cli commands we can do.
typedef enum CliCommandKind {
    HELP_CMD,
    RUN_TEXT_CMD,
    RUN_BIN_CMD,
    TEXT_TO_BIN_CMD,
    DEBUG_TEXT_CMD,
    DEBUG_BIN_CMD,
} CliCommandKind;
#define N_CLI_COMMAND_KIND 6


typedef struct CliCommand {
    CliCommandKind kind;
} CliCommand;


/// The result of trying to parse the cli arguments.
typedef struct CliParse {
    enum { SUCCESS_STATUS, ERROR_STATUS } status;
    union {
        CliCommand command;
        ErrorMsg error_msg;
    } data;
} CliParse;



const char* cli_command_name(CliCommandKind c) {
    switch (c) {
    case HELP_CMD: return "help";
    case RUN_TEXT_CMD: return "run-text";
    case RUN_BIN_CMD: return "run-bin";
    case TEXT_TO_BIN_CMD: return "text-to-bin";
    case DEBUG_TEXT_CMD: return "debug-text";
    case DEBUG_BIN_CMD: return "debug-bin";
    }
}


/// When should we display the help menu?
bool cli_is_help(int argc, const char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "help") == 0
        ||  strcmp(argv[i], "--help") == 0
        ||  strcmp(argv[i], "-h") == 0
        ||  strcmp(argv[i], "-?") == 0) {
            return true;
        }
    }
    return false;
}


CliParse init_cli(int argc, const char* argv[]) {
    CliParse ret;
    ret.status = SUCCESS_STATUS;
    if (cli_is_help(argc, argv)) {
        ret.data.command.kind = HELP_CMD;
        return ret;
    }

    if (argc == 1) {
        ret.status = ERROR_STATUS;
        ret.data.error_msg.owned = false;
        ret.data.error_msg.msg = "no arguments, should have at least one to choose a command";
        return ret;
    }

    // What's the command?
    bool found = false;
    CliCommandKind kind = 0;
    for (; kind < N_CLI_COMMAND_KIND; kind++) {
        if (strcmp(cli_command_name(kind), argv[1]) == 0) {
            ret.data.command.kind = kind;
            found = true;
            break;
        }
    }
    if (!found) {
        ret.status = ERROR_STATUS;
        ret.data.error_msg.owned = true;
        asprintf(
            &ret.data.error_msg.msg,
            "'%s' is not a name of a command",
            argv[1]
        );
        return ret;
    }

    if (argc != 2) {

    }
}
