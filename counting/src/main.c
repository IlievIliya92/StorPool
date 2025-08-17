/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdint.h>
#include <stdio.h>

/******************************** LOCAL DEFINES *******************************/
#define ARGS_DEFAULT {NULL}
#define UNIQUE_NUMBERS_MAX 4096 /* (2^12) */

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

    uint64_t i = 0;
    uint8_t buffer[3] = {0};
    uint16_t value1 = 0;
    uint16_t value2 = 0;
    size_t n = 0;
    uint64_t numbers[UNIQUE_NUMBERS_MAX] = {0}; /* numbers frequency table */
    /* Output stats */
    uint64_t unique_numbers_cnt = 0;
    uint64_t seen_only_once_cnt = 0;

    FILE *file = NULL;
    argp_parse(&argp, argc, argv, 0, 0, &args);

    file = fopen(args.input_file, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file: %s\n", args.input_file);
        return 1;
    }

    while ((n = fread(buffer, 1, 3, file)) > 0) {
        if (n < 2) {
            /**
             * Files with less than 12 bits to form a valid number are not processed.
             */
            break;
        }

        value1 = (buffer[0] << 4) | (buffer[1] >> 4);
        numbers[value1]++;
        if (n == 3) {
            value2 = ((buffer[1] & 0x0F) << 8) | buffer[2];
            numbers[value2]++;
        }
        /**
         * Leftover 4 bits which are not enough to form a valid 12 bit number
         * are discarted. This case occurs when the file length in bits
         * is not multiple of 12.
         */
    }

    /* Evaluate */
    for (i = 0; i < UNIQUE_NUMBERS_MAX; i++) {
        if (0 != numbers[i]) {
            unique_numbers_cnt++;
            if (1 == numbers[i]) {
                seen_only_once_cnt++;
            }
        }
    }
    fprintf(stdout, "%ld unique numbers\n", unique_numbers_cnt);
    fprintf(stdout, "%ld numbers seen only once\n", seen_only_once_cnt);

    fclose(file);

    return 0;
}
