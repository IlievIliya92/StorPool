/******************************** INCLUDE FILES *******************************/
#include <argp.h>

/******************************** LOCAL DEFINES *******************************/
#define ARGS_DEFAULT {NULL}

/******************************** TYPEDEFS ************************************/
typedef struct _arguments_t {
    char *input_file;
} arguments_t;

/******************************** LOCAL DATA **********************************/
static struct argp_option options[] = {
    { "input", 'i', "FILE", 0, "Input binary file path", 0 },
    { 0 }
};

/******************************* LOCAL FUNCTIONS ******************************/
static error_t parse_option( int key, char *arg, struct argp_state *state ) {
    arguments_t *arguments = state->input;

    switch (key) {
        case 'i':
            arguments->input_file = arg;
            break;

        case ARGP_KEY_ARG:
            return ARGP_ERR_UNKNOWN;

        case ARGP_KEY_END:
            if (!arguments->input_file) {
                argp_error(state, "You must specify an input file with -i.");
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
static struct argp argp = {options, parse_option, NULL, NULL, NULL, NULL, NULL};

int main(int argc, char **argv) {
    arguments_t args = ARGS_DEFAULT;
    argp_parse(&argp, argc, argv, 0, 0, &args);

    fprintf(stdout, "Input file: %s\n", args.input_file);

    return 0;
}
