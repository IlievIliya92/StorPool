/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "disk_ht.h"

/******************************** TYPEDEFS ************************************/
typedef struct _disk_data_t {
    char model[DISK_MODEL_LEN_MAX];
    int model_cnt;

    // struct _disk_data_t *next; /* external chaining for ht collisions */
} disk_data_t;

struct _disk_ht_t {
    disk_data_t **disk_data;
    int table_len;
};

/******************************* LOCAL FUNCTIONS ******************************/
static unsigned int hash(const char *model, int table_len) {
    unsigned int hash_value = 0;
    int length = 0;
    int i = 0;

    length = strnlen(model, DISK_MODEL_LEN_MAX);
    for (i = 0; i < length; i++)
    {
        hash_value += model[i];
        hash_value = hash_value % table_len;
    }

    return hash_value;
}

/***************************** INTERFACE FUNCTIONS ****************************/
disk_ht_t *disk_ht_new(int size)
{
    int i = 0;
    disk_ht_t *self = NULL;

    self = (disk_ht_t *)malloc(sizeof(disk_ht_t));
    if (self == NULL)
    {
        return NULL;
    }
    self->table_len = size;
    self->disk_data = (disk_data_t **)malloc(sizeof(disk_data_t) * self->table_len);
    if (self->disk_data == NULL)
    {
        free(self);
        return NULL;
    }

    /* Initialize the table */
    for (i = 0; i < self->table_len; i++) {
        self->disk_data[i] = NULL;
    }

    return self;
}

void disk_ht_destroy(disk_ht_t **self_p)
{
    if (*self_p) {
        disk_ht_t *self = *self_p;
        /* Free class properties */
        int i = 0;
        for (i = 0; i < self->table_len; i++) {
            free(self->disk_data[i]);
        }
        free(self->disk_data);
        /* Free object itself */
        free (self);
        *self_p = NULL;
    }
}

void disk_ht_print(disk_ht_t *self) {
    int i = 0;

    for (i = 0; i < self->table_len; i++) {
        if (self->disk_data[i] != NULL) {
            fprintf(stdout, "%i\t %s: %d\n",i , self->disk_data[i]->model,
                self->disk_data[i]->model_cnt);
        } else {
            fprintf(stdout, "%i\t ---\n", i);
        }
    }
}

int disk_ht_insert(disk_ht_t *self, const char *model) {
    if (model == NULL) {
        // print error
        return -1;
    }
    int idx = 0;

    disk_data_t *disk_new = (disk_data_t *) malloc(sizeof(disk_data_t));
    strncpy(disk_new->model, model, DISK_MODEL_LEN_MAX);
    disk_new->model_cnt = 1;

    idx = hash(model, self->table_len);
    if (self->disk_data[idx] != NULL) {
        return -1;
    }

    self->disk_data[idx] = disk_new;

    return 0;
}

int disk_ht_check(disk_ht_t *self, const char *model) {
    if (model == NULL) {
        // print error
        return -1;
    }
    int idx = 0;

    idx = hash(model, self->table_len);
    if (self->disk_data[idx] != NULL && strncmp(self->disk_data[idx]->model,
        model, DISK_MODEL_LEN_MAX) == 0) {
        /**
         * If we are checking for a model that is already present in our hash table
         * increment the model_cnt
         */
        self->disk_data[idx]->model_cnt++;
        return 0;
    }

    return -1;
}

/*
#define TABLE_LEN 10
#define DISK_NAMES 7
int main(int argc, char const *argv[])
{
    const char *disk_names[DISK_NAMES] = {
        "RDV2", "RDV2", "SSDF1", "123456789", "HGST2048T", "HGST2048T", "HGST2048T"
    };

    int ret = 0;
    int i = 0;

    disk_ht_t *disk_ht = disk_ht_new(TABLE_LEN);

    for (i = 0; i < DISK_NAMES; i++) {
        ret = disk_ht_check(disk_ht, disk_names[i]);
        if (ret)
            disk_ht_insert(disk_ht, disk_names[i]);
    }

    disk_ht_print(disk_ht);

    disk_ht_destroy(&disk_ht);

    return 0;
}
*/
