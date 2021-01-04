
#include <stdlib.h>
#include <stdio.h>

#include "dblist.h"
#include "option.h"

#include "io.h"

extern void  opt_driver_name(const char *, char *, char *);

db_driver_t *lz_db_connect(const char *lname)
{
    char name[16] = {0}, s_name[32] = {0};

    opt_driver_name(lname, name, s_name);
    drv_t *pd = dl_find_db_drv(name, s_name);
    if(!pd)
    {
        PRT_ERROR("The given driver name does not exist"); 
        PRT_TAIL_CHR;
        exit(-1);
    }

    if(pd->drv.ops.drv_connect(pd->drv.dbconn))
    {
        pd->drv.ops.drv_error(pd->drv.dbconn);
    }

    return &pd->drv;
}


int lz_db_destroy(const char *lname)
{
    char name[16] = {0}, s_name[32] = {0};

    opt_driver_name(lname, name, s_name);
    drv_t *pd = dl_find_db_drv(name, s_name);
    if(!pd)
    {
        PRT_ERROR("The given driver name does not exist"); 
        PRT_TAIL_CHR;
        exit(-1);
    }

    if(pd->drv.dbconn){
        free(pd->drv.dbconn); 
        pd->drv.dbconn = null;
    }

    if(pd->drv.lname){
        free(pd->drv.lname);
        pd->drv.lname = null;
    }

    free(pd);
    DSR_LIST_DELETE(&pd->list);

    return 0;
}


