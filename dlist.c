
#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include "dlist.h"
#include "io.h"

extern int xugusql_connect(void *);
extern void xugusql_disconnect(void *);
extern void xugusql_error(void *);
extern void xugusql_register_plg(void *);

extern int odbc_connect(void *);
extern void odbc_disconnect(void *);
extern void odbc_error(void *);
extern void odbc_register_plg(void *);


static drv_list_t  dlist;

drv_t *dl_find_db_drv(const char *name, const char *sname)
{
    drv_list_t *pl, *phead = &dlist;

    for(pl = phead->p_next; pl != phead; pl = pl->p_next)
    {
        drv_t *db = (drv_t *)(void *)((char *)pl - offsetof(drv_t, list));
        if(strstr(db->drv.lname, name) && strstr(db->drv.lname, sname)){
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


drv_t *dl_new_db_drv(const char *name, const char *sname)
{
    drv_t *db = calloc(1, sizeof(drv_t));
    if(db)
    {
        if(!strcmp(name, "odbc"))
        {
            db->drv.ops.drv_connect = odbc_connect;
            db->drv.ops.drv_disconnect = odbc_disconnect;
            db->drv.ops.drv_error = odbc_error;
            db->drv.ops.drv_register = odbc_register_plg;
        
        }else if(!strcmp(name, "xugusql"))
        {
            db->drv.ops.drv_connect = xugusql_connect;
            db->drv.ops.drv_disconnect = xugusql_disconnect;
            db->drv.ops.drv_error = xugusql_error;
            db->drv.ops.drv_register = xugusql_register_plg;

        }else {
            PRT_ERROR("Nnknown driver");
            PRT_TAIL_CHR;
            exit(-1);
        }

        db->drv.lname = (char *)calloc(1, strlen(name) + strlen(sname) + 1);
        if(db->drv.lname){
            sprintf(db->drv.lname, "%s-%s", name, sname);
            return db;
        }
    }

    PRT_ERROR("Not enough free memory");
    PRT_TAIL_CHR;
    exit(-1);
}


drv_t *dl_add_db_drv(drv_t *db)
{
    char *p = strchr(db->drv.lname, '-');
    *p = '\0';

    drv_t *rp = dl_find_db_drv(db->drv.lname, p+1);
    if(rp){
        *p = '-';
        return null;
    }

    *p = '-';
    DSR_LIST_ADD_TAIL(&db->list, &dlist);   
    return db;
}

/*
drv_t *dl_delete_db_drv(const char *name)
{
    drv_t *rp = dl_find_db_drv(name);
    if(!rp){
        return null;
    }

    DSR_LIST_DELETE(&rp->list);
    return rp;
}
*/


