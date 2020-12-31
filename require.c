
#include <stdlib.h>
#include <string.h>

#include "load.h"
#include "dblist.h"
#include "option.h"


int main(int argc, char *argv[])
{
    KV dsn[] = {
        OPT("xgci", host_ip  , "IP address of the server host" , "host_ip", "127.0.0.1", STRING),
        OPT("xgci", host_port, "Port number of the server host", "host_port", "5138", INT),
        OPT("xgci", dbname   , "The database name of the server login", "dbname", "SYSTEM", STRING),
        OPT("xgci", user_name, "User name for server login", "user_name", "SYSDBA", STRING),
        OPT("xgci", user_passwd, "User password for server login", "user_passwd", "SYSDBA", STRING),
        OPT("xgci", charset  , "The character set of the connection server", "charset", "UTF8", INT),
        OPT("odbc", host_dsn , "Data source information of the server host", "host_dsn", "A", STRING),
        OPT("odbc", user_name, "User name for server login", "user_name", "A", STRING),
        OPT("odbc", user_passwd, "User password for server login", "user_passwd", "A", STRING),
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
            }

            opt_login_argv(opt, driver->drv.dbval + opt.item);
            luaz_pop_elements(L, 1);
        
        }while(1);

    }

    int top = lua_gettop(L);
    luaz_load_internal_script(L);

    luaz_close_state(L);


}




