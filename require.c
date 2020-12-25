
#include <stdlib.h>
#include <string.h>

#include "load.h"
#include "dlist.h"
#include "rows.h"


int main(int argc, char *argv[])
{
    KV dsn[] = {

            OPT("xugusql", host_ip, "host_ip", "127.0.0.1", STRING),
            OPT("xugusql", host_port, "host_port", "5138", INT),
            OPT("xugusql", dbname, "dbname", "SYSTEM", STRING),
            OPT("xugusql", user_name, "user_name", "SYSDBA", STRING),
            OPT("xugusql", user_passwd, "user_passwd", "SYSDBA", STRING),
            OPT("xugusql", charset, "charset", "UTF8", INT),
            OPT("odbc", odbcdsn, "dsn", "SQL", STRING),
            OPT("odbc", odbcuid, "user_name", "qxj", STRING),
            OPT("odbc", odbcpwd, "user_passwd", "Aa120110", STRING),
    };

    dl_init();
    lua_State *L = luaz_new_state();
    
    int j, sz = sizeof(dsn)/sizeof(KV);
    
    for(j = 0; j < sz; j++)
    {
        do{
            const char *ud = luaz_parse_driver(L, dsn[j].drv_name);
            if(!ud){
                break;
            }

            drv_t *driver = dl_find_db_drv(dsn[j].drv_name, ud);
            if(!driver){
                driver = dl_new_db_drv(dsn[j].drv_name, ud);
                dl_add_db_drv(driver);

                driver->drv.ops.drv_register((void *)(&driver->drv));
            }

            luaz_pop_elements(L, 1);
            KV opt = dsn[j];

            const char *p = luaz_get_element(L, opt.name);
            if(p){
                opt.value = (char *)p; 
            }else{
            
                luaz_pop_elements(L, 1);
            }

            row_read_db_option(opt, driver->drv.dbval + opt.item);
            luaz_pop_elements(L, 1);
        
        }while(1);

    }

    int top = lua_gettop(L);
    luaz_load_internal_script(L);

    luaz_close_state(L);


}




