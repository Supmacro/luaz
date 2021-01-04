

local ffi = require("ffi")

sql = {}

ffi.cdef [[

    typedef struct{
        int  (*drv_connect)(void *);
        void (*drv_disconnect)(void *);
        void (*drv_error)(void *);
        void (*drv_register)(void *);
    }op_t;

    typedef struct{
        void    *value;
        int      length;
    }db_value_t;

    typedef struct{
        char    *lname;
        void    *dbconn;

        op_t         ops;
        db_value_t  *dbval;

    }db_driver_t;

    db_driver_t *lz_db_connect(const char *);
    int lz_db_destroy(const char *);
]]


local db_driver = ffi.typeof('db_driver_t *')

function sql.driver(driver_name)
    local db = ffi.C.lz_db_connect(driver_name)
    return ffi.gc(db, ffi.C.lz_db_destroy)
end


local dbI = {}

function dbI.driver_name(self)
    return ffi.string(self.lname)
end

--driver metamethod
local dbmeta = {
    __index = dbI,
    __gc = ffi.C.lz_db_destroy,
    __tostring = function() return '<db_driver>' end
}

ffi.metatype("db_driver_t", dbmeta)

local db = sql.driver("xgci.drv1")

local name = db:driver_name()
print(name)

db = nil


