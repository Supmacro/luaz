
#include "rows.h"

#define DSR_LIST_DELETE(item)       \
        do{                         \
            (item)->p_next->p_prev = (item)->p_prev;  \
            (item)->p_prev->p_next = (item)->p_next;  \
        } while(0)

#define DSR_LIST_INIT( head )         \
        do{                           \
            (head)->p_next = (head);  \
            (head)->p_prev = (head);  \
        } while(0)

#define DSR_LIST_DOUBLE_INIT(list_1, list_2) \
        do{                                  \
            (list_1)->p_next = (list_1);     \
            (list_1)->p_prev = (list_1);     \
            (list_2)->p_next = (list_2);     \
            (list_2)->p_prev = (list_2);     \
        } while(0)

#define null NULL
#define DSR_LIST_ADD_TAIL(item, head)    \
        do{                              \
            (item)->p_prev = (head)->p_prev;  \
            (item)->p_next = (head);          \
            (head)->p_prev = (item);          \
            (item)->p_prev->p_next = (item);  \
        }while(0)

#define DRV_REGISTERED(K, d) do{       \
            K##_register_drv(&(d));    \
        }while(0)

typedef struct drv_list_t {
    struct drv_list_t *p_next;
    struct drv_list_t *p_prev;
}drv_list_t;


typedef struct{
    void (*drv_connect)();
    void (*drv_disconnect)();
    void (*drv_error)();
    void (*drv_register)(db_value_t **);
}op_t;


typedef struct {
    const char *drv_name;

    db_value_t *dbval;
    op_t  ops;
    
}db_driver_t;


typedef struct {
    db_driver_t   drv;
    drv_list_t    list;

} drv_t;

drv_t *dl_find_db_drv(const char *name);
drv_t *dl_new_db_drv(const char *name);
drv_t *dl_add_db_drv(drv_t *db);
drv_t *dl_delete_db_drv(const char *name);
void dl_init();



