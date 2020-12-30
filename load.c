
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load.h"
#include "io.h"

static void luaz_set_paths(lua_State *L)
{
    /*
     * Push the value in the global variable 'package' on 
     * the stack and return the type of the value
     * */
    lua_getglobal(L, "package");

    /* Returns the index of the top element of the stack. 
     * Because the index is numbered from 1, 
     * the result is equal to the number of elements 
     * on the stack; 
     * in particular, 0 means the stack is empty */
    int top = lua_gettop(L);

    /* This macro is equivalent to lua_pushlstring, 
     * except that it can only be used when s is a literal. 
     * It will automatically give the length of the string */
    lua_pushliteral(L, "./?.lua;");
    lua_pushliteral(L, "./?/init.lua;");

    const char *home = getenv("HOME");
    if(home)
    {
        lua_pushstring(L, home);
        lua_pushstring(L, "/.luarocks/share/lua/5.1/?.lua;");
        lua_pushstring(L, home);
        lua_pushstring(L, "/.luarocks/share/lua/?.lua;");
    }

    lua_pushliteral(L, "/usr/local/share/lua/5.1/?.lua;");
    lua_pushliteral(L, "/usr/share/lua/5.1/?.lua;");

    /* Connect the n values at the top of the stack, 
     * then pop these values from the stack, 
     * and put the result on the top of the stack */
    lua_concat(L, lua_gettop(L) - top);

    const char *lua_path = getenv("LUA_PATH");
    if(lua_path)
    {
        const char *def = lua_tostring(L, -1); 
        lua_path = luaL_gsub(L, lua_path, ";;", ";\1;");
        luaL_gsub(L, lua_path, "\1", def);
        lua_remove(L, -2);
        lua_remove(L, -2);
    }

    /* Do an operation equivalent to package[path] = v, 
     * And v is the value at the top of the stack,
     * the v value will be popped from the stack*/
    lua_setfield(L, top, "path");

    lua_pushliteral(L, "./?.so;");
    if(home)
    {
        lua_pushstring(L, home);
        lua_pushliteral(L, "/.luarocks/lib/lua/5.1/?.so;");
        lua_pushstring(L, home);
        lua_pushliteral(L, "/.luarocks/lib/lua/?.so;");
    }

    lua_pushliteral(L, "/usr/local/lib/lua/5.1/?.so;");
    lua_pushliteral(L, "/usr/lib/lua/5.1/?.so;");

    lua_concat(L, lua_gettop(L) - top);
    const char *cpath = getenv("LUA_CPATH");
    if(cpath)
    {
        const char *def = lua_tostring(L, -1);
        cpath = luaL_gsub(L, cpath, ";;", ";\1;");
        luaL_gsub(L, cpath, "\1", def);
        lua_remove(L, -2);
        lua_remove(L, -2);
    }

    lua_setfield(L, top, "cpath");
    lua_pop(L, 1); /* package.path, package.cpath */

}


static void luaz_load_number(lua_State *L, const char *name, lua_Number n)
{
    lua_pushstring(L, name);
    lua_pushnumber(L, n);
    lua_settable(L, -3);
}


static void luaz_load_string(lua_State *L, const char *name, const char *s)
{
    lua_pushstring(L, name);
    lua_pushstring(L, s);
    lua_settable(L, -3);
}


int luaz_close_state(lua_State *state)
{
    if(state)
        lua_close(state);

    return 0;
}


lua_State *luaz_new_state(void)
{
    lua_State *L = luaL_newstate();
    if(!L)
        return NULL;

    luaL_openlibs(L);
    luaz_set_paths(L);

    /* Create an empty table and push it onto the stack. 
     * It is equivalent to lua_createtable(L, 0, 0) */
    //lua_newtable(L);
    
    int rc;

    if((rc = luaL_loadfile(L, "configure")) != 0)
    {
        lua_getglobal(L, "require");
        lua_pushstring(L, "configure");
        
        if(lua_pcall(L, 1, 1, 0))
        {
            const char * err = lua_tostring(L, -1);
            if(err)
            {
                PRT_ERROR(err);
                PRT_TAIL_CHR;
                exit(-1);
            }
    
            exit(-1);
        }

    }else if(lua_pcall(L, 0, 0, 0))
    {
        const char *err = lua_tostring(L, -1);
        if(err)
        {
            PRT_ERROR(err);
            PRT_TAIL_CHR;
            exit(-1);
        }

        exit(-1);
    }

    do{
        int top = lua_gettop(L);
        if(top == 1)
            break;
        
        int pos = top * (-1);
        lua_remove(L, pos);
    
    }while(1);

    return L; 
}


const char *luaz_parse_driver(lua_State *L, const char *drv)
{
    static int j = 2, top = 1;
    int    op = lua_gettop(L);
        
    while(j-- > 0)
    {
        if(!lua_istable(L, -1))
        {
            PRT_ERROR("Lua configuration file format error");
            PRT_TAIL_CHR;
            exit(-1);
        }

        if(j){
            lua_getfield(L, -1, drv);
            top = lua_gettop(L);
        }else{
            lua_pushnil(L);
        }
    }

    if(lua_next(L, top))
    {
       op = lua_gettop(L);
        lua_pushvalue(L, -2);
        return lua_tostring(L, -1); 
    }

    int dcap = lua_gettop(L) - 1;
    lua_pop(L, dcap);

    j = 2;

    return NULL;
}


void luaz_pop_elements(lua_State *L, int count)
{
    lua_pop(L, count);
}


const char *luaz_get_element(lua_State *L,  const char *name)
{
    int top = lua_gettop(L);

    if(!lua_istable(L, -1))
    {
        PRT_ERROR("The top element of the stack is not a table object");
        PRT_TAIL_CHR;
        exit(-1);
    }       

    const char *p = NULL;
   
    lua_getfield(L, -1, name);
    if(lua_isnil(L, -1))
    {
        lua_pop(L, 1);
        return p;
    }
    
    if(lua_isstring(L, -1))
        p = lua_tostring(L, -1);

    lua_pop(L, 1);
    return p;
}


void luaz_load_internal_script(lua_State *L)
{
    const char *luaz[] = {
                            "./internal/luaz.lua",
    };
   

    int j, sz = sizeof(luaz)/sizeof(const char *);
    for(j = 0; j < sz; j++)
    {
        if(luaL_loadfile(L, luaz[j]))
        {
            PRT_ERROR("luaL_loadfile() failed to load file");
            PRT_TAIL_CHR;
            exit(-1);
        }

        lua_call(L, 0, 0);
    }

}


void luaz_safe_free(void *p)
{
    if(p){
        free(p);
    }    
}
