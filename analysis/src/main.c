/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <yajl/yajl_parse.h>

#include "disk_ht.h"

/****************************** LOCAL DEFINES *********************************/
#define KEY_LEN_MAX 6
#define READ_BUFFER_LEN 65536

/******************************** TYPEDEFS ************************************/
typedef struct _ctx_t {
    int in_array;
    int in_object;

    char key[KEY_LEN_MAX];
    char model[DISK_MODEL_LEN_MAX];

    uint64_t total_entries;
} ctx_t;

/******************************** LOCAL DATA **********************************/

/******************************* LOCAL FUNCTIONS ******************************/
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
        memcpy(c->model, str, model_len);
        c->model[model_len] = '\0';
    }

    return 1;
}

static int on_map_key(void *_ctx, const unsigned char *key, size_t len) {
    ctx_t *c = (ctx_t *)_ctx;
    size_t n = 0;

    memcpy(c->key, key, len);
    c->key[n] = '\0';

    return 1;
}

static int on_start_map(void *_ctx) {
    ctx_t *c = (ctx_t *)_ctx;

    if (c->in_array) {
        c->in_object = 1;
        memset(c->key, '\0', KEY_LEN_MAX);
        memset(c->model, '\0', DISK_MODEL_LEN_MAX);
        c->total_entries++;
    }

    return 1;
}

static int on_end_map(void *_ctx) {
    ctx_t *c = (ctx_t *)_ctx;
    if (c->in_object) {
        /* Emit the record; adjust to your needs (e.g., write to DB). */
        //printf("Model: %s\n", c->model);

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
    unsigned char buf[READ_BUFFER_LEN];  /* 64KB buffer */

    if (argc < 2) {
        //fprintf(stderr, "Usage: %s data.json\n", argv);
        return 1;
    }

    const char *path = argv[1];
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Set callbacks */
    yajl_callbacks cbs;
    memset(&cbs, 0, sizeof(cbs));

    cbs.yajl_string = on_string;
    cbs.yajl_start_map = on_start_map;
    cbs.yajl_map_key = on_map_key;
    cbs.yajl_end_map = on_end_map;
    cbs.yajl_start_array = on_start_array;
    cbs.yajl_end_array = on_end_array;

    yajl_handle handle = yajl_alloc(&cbs, NULL, (void *)&ctx);
    if (!handle) {
        fprintf(stderr, "yajl_alloc failed\n");
        fclose(fp);
        return 1;
    }

    /* yajl_config(hand, yajl_allow_comments, 1); */
    size_t rd;
    yajl_status stat = yajl_status_ok;

    while ((rd = fread(buf, 1, sizeof(buf), fp)) > 0) {
        stat = yajl_parse(handle, buf, rd);
        if (stat != yajl_status_ok) {
            break;
        }
    }

    if (stat == yajl_status_ok) {
        stat = yajl_complete_parse(handle);
    }

    fprintf(stdout, "Total entries: %ld\n", ctx.total_entries);

    if (stat != yajl_status_ok) {
        unsigned char *err = yajl_get_error(handle, 1, buf, 0);
        fprintf(stderr, "Parse error: %s\n", err);
        yajl_free_error(handle, err);
        yajl_free(handle);
        fclose(fp);
        return 1;
    }

    yajl_free(handle);
    fclose(fp);

    return 0;
}
