/******************************** INCLUDE FILES *******************************/
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <yajl/yajl_parse.h>

#include "disk_ht.h"

/****************************** LOCAL DEFINES *********************************/
#define KEY_LEN_MAX 6
#define READ_BUFFER_LEN 65536 /**< Size of a single read*/
#define DISK_HT_LEN 20
#define ARGS_DEFAULT {NULL}

/******************************** TYPEDEFS ************************************/
typedef struct _arguments_t {
    char *input_file;
} arguments_t;

typedef struct _ctx_t {
    int in_array;
    int in_object;

    char key[KEY_LEN_MAX];
    char model[DISK_MODEL_LEN_MAX];

    uint64_t total_entries;
    disk_ht_t *disk_ht;
} ctx_t;

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

/*************************** JSON PARSER FUNCTIONS ****************************/
static void reset_object(ctx_t *c) {
    c->in_object = 0;
    memset(c->key, '\0', KEY_LEN_MAX);
    memset(c->model, '\0', DISK_MODEL_LEN_MAX);
}

static int on_string(void *_ctx, const unsigned char *str, size_t len) {
    ctx_t *c = (ctx_t *)_ctx;

    if (!c->in_object || c->key == NULL)
        return 1;

    if (strcmp(c->key, "model") == 0) {
        const char *p = strchr(str, '\"');
        size_t model_len = (size_t)(p - (const char *)str);
        strncpy(c->model, str, model_len);
        c->model[model_len] = '\0';
    }

    return 1;
}

static int on_map_key(void *_ctx, const unsigned char *key, size_t len) {
    ctx_t *c = (ctx_t *)_ctx;
    strncpy(c->key, key, len);

    return 1;
}

static int on_start_map(void *_ctx) {
    ctx_t *c = (ctx_t *)_ctx;

    if (c->in_array) {
        c->in_object = 1;
        memset(c->key, '\0', KEY_LEN_MAX);
        memset(c->model, '\0', DISK_MODEL_LEN_MAX);
    }

    return 1;
}

static int on_end_map(void *_ctx) {
    ctx_t *c = (ctx_t *)_ctx;
    int ret = 0;

    if (c->in_object) {
        /* Add model record to the hash table if not present in it */
        c->total_entries++;
        disk_ht_insert(c->disk_ht, c->model);

        reset_object(c);
    }
    return 1;
}

static int on_start_array(void *_ctx) {
    ctx_t *c = (ctx_t *)_ctx;

    if (!c->in_array) {
        c->in_array = 1;
    }

    return 1;
}

static int on_end_array(void *_ctx) {
    ctx_t *c = (ctx_t *)_ctx;
    c->in_array = 0;

    return 1;
}

int main(int argc, char **argv) {
    arguments_t args = ARGS_DEFAULT;
    FILE *file = NULL;
    size_t n = 0;
    uint8_t buffer[READ_BUFFER_LEN];  /* 64KB buffer */
    ctx_t ctx;
    yajl_callbacks cbs;
    yajl_status stat = yajl_status_ok;
    int ret = 0;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    file = fopen(args.input_file, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file: %s\n", args.input_file);
        return -1;
    }

    memset(&ctx, 0, sizeof(ctx));
    memset(&cbs, 0, sizeof(cbs));
    /* Initialize context */
    ctx.disk_ht = disk_ht_new(DISK_HT_LEN);

    /* Set callbacks */
    cbs.yajl_string = on_string;
    cbs.yajl_start_map = on_start_map;
    cbs.yajl_map_key = on_map_key;
    cbs.yajl_end_map = on_end_map;
    cbs.yajl_start_array = on_start_array;
    cbs.yajl_end_array = on_end_array;

    yajl_handle handle = yajl_alloc(&cbs, NULL, (void *)&ctx);
    if (handle == NULL) {
        fprintf(stderr, "Error: yajl_alloc failed!\n");
        ret = -1;
        goto exit;
    }

    fprintf(stdout, "Info: Processing: %s ...\n", args.input_file);
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        stat = yajl_parse(handle, buffer, n);
        if (stat != yajl_status_ok) {
            break;
        }
    }

    if (stat == yajl_status_ok) {
        stat = yajl_complete_parse(handle);
    } else {
        unsigned char *err = yajl_get_error(handle, 1, buffer, 0);
        fprintf(stderr, "Error: Parse error: %s\n", err);
        yajl_free_error(handle, err);
        ret = -1;
    }

    yajl_free(handle);
    disk_ht_print_data(ctx.disk_ht);
    fprintf(stdout, "Info: Total entries: %ld\n", ctx.total_entries);

exit:
    disk_ht_destroy(&ctx.disk_ht);
    fclose(file);

    return ret;
}
