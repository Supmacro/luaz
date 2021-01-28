
#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include "list.h"
#include "pipe.h"
#include "option.h"


extern int sql_connect(void *);
extern int sql_prepare(void *voip, char *sql);
extern int sql_bindparam(void *voip, db_param_t *param, int no);
extern int sql_execute(void *);
extern int sql_freehandle(void *voip);

extern int sql_disconnect(void *);
extern void sql_error(void *);
extern void sql_register_plg(void *);


extern int odbc_connect(void *);
extern int odbc_prepare(void *voip, char *sql);
extern int odbc_bindparam(void *voip, db_param_t *param, int no);
extern int odbc_execute(void *);
extern int odbc_freehandle(void *voip);

extern int odbc_disconnect(void *);
extern void odbc_error(void *);
extern void odbc_register_plg(void *);


static drv_list_t  list;

drv_t *dl_find_db_drv(const char *name, const char *sname)
{
    drv_list_t *pl, *phead = &list;

    for(pl = phead->p_next; pl != phead; pl = pl->p_next)
    {
        drv_t *db = (drv_t *)(void *)((char *)pl - offsetof(drv_t, list));
        if(strstr(db->drv.lname, name) && 
                        !strcmp(db->drv.lname + strlen(name) + 1, sname)){
            return db;
        }
    }

    return null;
}


void dl_init()
{
    drv_list_t *pl = &list;
    pl->p_next = pl->p_prev = pl;
}


drv_t *dl_new_db_drv(const char *name, const char *sname)
{
    drv_t *db = calloc(1, sizeof(drv_t));
    if(db)
    {
        struct drvop op[] = {
                    { "odbc",
                      odbceof,
                      {
                          odbc_connect,
                          odbc_prepare,
                          odbc_bindparam,
                          odbc_execute,
                          odbc_freehandle,
                          odbc_disconnect,
                          odbc_error,
                          odbc_register_plg
                      }},
                    
                    { "xgci",
                      xgcieof,
                     {
                         sql_connect,
                         sql_prepare,
                         sql_bindparam,
                         sql_execute,
                         sql_freehandle,
                         sql_disconnect,
                         sql_error,
                         sql_register_plg
                     }}
            };  

        int j, sz = sizeof(op)/sizeof(struct drvop);

        for(j = 0; j < sz; j++)
        {
            if(strcmp(name, op[j].name)){
                continue;
            }
                
            db->drv.ops = op[j].ops;
            db->drv.vcap = op[j].attr_cap;
            break;
        }

        if(!db->drv.ops.drv_connect)
        {
            PRT_ERROR("Unknown driver");
            PRT_TAIL_CHR;
            exit(-1);
        }

        db->drv.lname = (char *)calloc(1, strlen(name) + strlen(sname) + 1);
        if(db->drv.lname){
            sprintf(db->drv.lname, "%s.%s", name, sname);
            return db;
        }
    }

    PRT_ERROR("Not enough free memory");
    PRT_TAIL_CHR;
    exit(-1);
}


drv_t *dl_add_db_drv(drv_t *db)
{
    char *p = strchr(db->drv.lname, '.');
    *p = '\0';

    drv_t *rp = dl_find_db_drv(db->drv.lname, p+1);
    if(rp){
        *p = '.';
        return null;
    }

    *p = '.';
    DSR_LIST_ADD_TAIL(&db->list, &list);   
    return db;
}



