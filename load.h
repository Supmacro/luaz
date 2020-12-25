
#include "lualib.h"
#include "lua.h"
#include "lauxlib.h"


static void luaz_set_paths(lua_State *L);

static void luaz_load_number(lua_State *L, const char *name, lua_Number n);

static void luaz_load_string(lua_State *L, const char *name, const char *s);

int luaz_close_state(lua_State *state);

lua_State *luaz_new_state(void);

const char *luaz_get_element(lua_State *L, const char *name);

const char *luaz_parse_driver(lua_State *L, const char *drv);

void luaz_load_internal_script(lua_State *L);
