
#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include "dlist.h"
#include "io.h"

extern void xugusql_register_plg(db_value_t **);
extern void odbc_register_plg(db_value_t **);


static drv_list_t  dlist;


drv_t *dl_find_db_drv(const char *name)
{
    drv_list_t *pl, *phead = &dlist;

    for(pl = phead->p_next; pl != phead; pl = pl->p_next)
    {
        drv_t *db = (drv_t *)(void *)((char *)pl - offsetof(drv_t, list));
        if(!strcmp(db->drv.drv_name, name)){
            return db;
        }
    }

    return null;
}


void dl_init()
{
    drv_list_t *pl = &dlist;
    pl->p_next = pl->p_prev = pl;
}


drv_t *dl_new_db_drv(const char *name)
{
    drv_t *db = calloc(1, sizeof(drv_t));
    if(db)
    {
        if(!strcmp(name , "xugusql")){
            xugusql_register_plg(&(db->drv.dbval)); 

        }else if(!strcmp(name, "odbc")){
            odbc_register_plg(&(db->drv.dbval)); 
        }else {
            PRT_ERROR("Nnknown driver");
            PRT_TAIL_CHR;
            exit(-1);
        }

        db->drv.drv_name = strdup(name);
        if(db->drv.drv_name){
            return db;
        }
    }

    PRT_ERROR("Not enough free memory");
    PRT_TAIL_CHR;
    exit(-1);
}


drv_t *dl_add_db_drv(drv_t *db)
{
    drv_t *rp = dl_find_db_drv(db->drv.drv_name);
    if(rp){
        return null;
    }

    DSR_LIST_ADD_TAIL(&db->list, &dlist);   
    return db;
}


drv_t *dl_delete_db_drv(const char *name)
{
    drv_t *rp = dl_find_db_drv(name);
    if(!rp){
        return null;
    }

    DSR_LIST_DELETE(&rp->list);
    return rp;
}



