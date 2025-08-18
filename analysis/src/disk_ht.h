#ifndef __DISK_HT_H__
#define __DISK_HT_H__

/******************************** DEFINES *************************************/
#define DISK_MODEL_LEN_MAX 1000

/******************************** TYPEDEFS ************************************/
typedef struct _disk_ht_t disk_ht_t;

typedef enum _disk_ht_ret_t {
    DISK_HT_SUCESS = 0, /* Model added successfully to the HT */
    DISK_HT_EINVAL, /* Invalid argument */
    DISK_HT_ENOMEM, /* Out of memory */
    DISK_HT_MEXIST, /* Model exists in the HT */
} disk_ht_ret_t;

/***************************** INTERFACE FUNCTIONS ****************************/
disk_ht_t *disk_ht_new(int size);
void disk_ht_destroy(disk_ht_t **self_p);
void disk_ht_print_table(disk_ht_t *self);
void disk_ht_print_data(disk_ht_t *self);
disk_ht_ret_t disk_ht_insert(disk_ht_t *self, const char *model);

#endif /* __DISK_HT_H__ */
