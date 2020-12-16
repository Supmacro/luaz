
#include <stdlib.h>
#include <string.h>

#include "load.h"
#include "dlist.h"

int main(int argc, char *argv[])
{
    KV dsn[] = {

            OPT("xugusql", host_ip, "host_ip", "127.0.0.1", STRING),
            OPT("xugusql", host_port, "host_port", "5138", INT),
            OPT("xugusql", dbname, "dbname", "SYSTEM", STRING),
            OPT("xugusql", user_name, "user_name", "SYSDBA", STRING),
            OPT("xugusql", user_passwd, "user_passwd", "SYSDBA", STRING),
            OPT("xugusql", charset, "charset", "UTF8", INT),
            OPT("odbc", odbcdsn, "odbcdsn", "SQL", STRING),
            OPT("odbc", odbcuid, "odbcuid", "qxj", STRING),
            OPT("odbc", odbcpwd, "odbcpwd", "Aa120110", STRING),
    };

    dl_init();
    int j, sz = sizeof(dsn)/sizeof(KV);
    
    for(j = 0; j < sz; j++)
    {
        drv_t *driver = dl_find_db_drv(dsn[j].drv_name);
        if(!driver)
        {
            driver = dl_new_db_drv(dsn[j].drv_name);
            dl_add_db_drv(driver);
        }

        db_value_t *dval = driver->drv.dbval;
        row_read_db_option(dsn[j], dval + dsn[j].item);
    }
    
    xugusql_connect();
    odbc_connect();

    lua_State *L = luaz_new_state();

    /*
    for(j = 0; j < eof; j++)
    {
        const char *p = luaz_get_element(L, dsn[j].name); 
        if(j == dsn[j].key){
            myconn.login[j] = !p ? dsn[j].value : (char *)p;
            printf("%s:%s\n", dsn[j].name, myconn.login[j]);
        }

    }
*/
    luaz_close_state(L);


}




