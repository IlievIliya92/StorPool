#ifndef __DISK_HT_H__
#define __DISK_HT_H__

/******************************** DEFINES *************************************/
#define DISK_MODEL_LEN_MAX 1000

/******************************** TYPEDEFS ************************************/
typedef struct _disk_ht_t disk_ht_t;

/***************************** INTERFACE FUNCTIONS ****************************/
disk_ht_t *disk_ht_new(int size);
void disk_ht_destroy(disk_ht_t **self_p);
void disk_ht_print_table(disk_ht_t *self);
void disk_ht_print_data(disk_ht_t *self);
int disk_ht_insert(disk_ht_t *self, const char *model);
int disk_ht_check(disk_ht_t *self, const char *model);

#endif /* __DISK_HT_H__ */
