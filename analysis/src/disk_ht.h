#ifndef __DISK_HT_H__
#define __DISK_HT_H__

/******************************** DEFINES *************************************/
#define DISK_MODEL_LEN_MAX 1000

/******************************** TYPEDEFS ************************************/
typedef struct _disk_ht_t disk_ht_t;

/**
 * Disk hash table insert method return codes
 */
typedef enum _disk_ht_ret_t {
    DISK_HT_SUCESS = 0, /**< Model added successfully to the HT */
    DISK_HT_EINVAL, /**< Invalid argument */
    DISK_HT_ENOMEM, /**< Out of memory */
    DISK_HT_MEXIST, /**< Model exists in the HT */
} disk_ht_ret_t;

/***************************** INTERFACE FUNCTIONS ****************************/

/**
 * Create new disk hash table
 *
 * @param[in] size Size of the hash table
 * @return New disk_ht_t object
 */
disk_ht_t *disk_ht_new(int size);

/**
 * Destroy disk_ht_t object. You must use this for any disk_ht created via the
 * disk_ht_new method.
 *
 * @param[in]: self_p Pointer to es_client_t object reference, so the destructor
 * can set it to NULL
 */
void disk_ht_destroy(disk_ht_t **self_p);

/**
 * Print disk hash table
 *
 * @param[in] self Reference to disk_ht_t object
 */
void disk_ht_print_table(disk_ht_t *self);

/**
 * Print only populated entries in the hash table
 *
 * @param[in] self Reference to disk_ht_t object
 */
void disk_ht_print_data(disk_ht_t *self);

/**
 * Store disk model into the hash table. If the model is already present in the
 * hash table the model_cnt value is incremented for the corespondent model.
 *
 * @param[in] self Reference to disk_ht_t object
 * @param[in] model Model string
 * @return DISK_HT_SUCESS for successful insertion, DISK_HT_MEXIST if the model
 * is already present, DISK_HT_EINVAL for invalid argument, DISK_HT_ENOMEM memory
 * allocation failiure.
 */
disk_ht_ret_t disk_ht_insert(disk_ht_t *self, const char *model);

#endif /* __DISK_HT_H__ */
