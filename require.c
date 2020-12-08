
#include "loading.h"
#include "connect.h"

int main(int argc, char *argv[])
{
    KV dsn[] = {
                    {host_ip, "host_ip", "127.0.0.1"},
                    {host_port, "host_port", "5138"},
                    {dbname, "dbname", "SYSTEM"},
                    {user_name, "user_name", "SYSDBA"},
                    {user_passwd, "user_passwd", "SYSDBA"},
    };

    connect myconn;

    int j;
    lua_State *L = luaz_new_state();

    for(j = 0; j < eof; j++)
    {
        const char *p = luaz_get_element(L, dsn[j].name); 
        if(j == dsn[j].key){
            myconn.login[j] = !p ? dsn[j].value : (char *)p;
        }

    }

    luaz_close_state(L);
}




