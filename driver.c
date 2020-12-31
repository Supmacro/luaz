
#include <stdlib.h>
#include <stdio.h>

#include "dblist.h"
#include "option.h"

extern void  opt_driver_name(const char *, char *, char *);

int db_connect(const char *lname)
{
    char name[16] = {0}, s_name[32] = {0};

    opt_driver_name(lname, name, s_name);
    drv_t *driver = dl_find_db_drv(name, s_name);
    if(!driver)
    {
        
    }

    printf("host_ip:%s\n", (char*)(driver->drv.dbval[host_ip].value));
    printf("user_name:%s\n", (char*)(driver->drv.dbval[user_name].value));
    printf("user_passwd:%s\n", (char*)(driver->drv.dbval[user_passwd].value));


    return 1;
}


