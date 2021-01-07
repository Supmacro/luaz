

local ffi = require("ffi")

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

    typedef struct{
        void    *pdb;
        short    Isdown;

    }db_conn_t;

    typedef struct{
        void    *pdb;
        char    *sql;

        short   Ispre;
        short   Isclose;

    }db_stmt_t;

    typedef struct{
        void   *value;
        int     len;
        int     len_max;

        int     iot;
        int     rcode;
        short   type;
    
    }db_param_t;

    db_driver_t *db_loaded(const char *);
    void db_destroy(db_driver_t *pd);
    
    db_conn_t *db_connect(db_driver_t *);
    void db_disconnect(db_conn_t *);
    void db_conn_destroy(db_conn_t *);

    db_stmt_t *db_prepare(db_conn_t *, const char *);
    void db_bind_parameter(db_stmt_t *, db_param_t *, int);
    void db_execute(db_stmt_t *);
    void db_stmt_close(db_stmt_t *);
    void db_stmt_destroy(db_stmt_t *);

]]


local db_driver = ffi.typeof('db_driver_t *')
local DBI = {}

function sql.driver(driver_name)
    local db = ffi.C.db_loaded(driver_name)
    return ffi.gc(db, ffi.C.db_destroy)
end


function DBI.connect(self)
    local conn = ffi.C.db_connect(self)
    return ffi.gc(conn, ffi.C.db_conn_destroy)
end


function DBI.driver_name(self)
    return ffi.string(self.lname)
end


--driver metamethod
local dbmeta = {
    __index = DBI,
    __gc = ffi.C.db_destroy,
    __tostring = function() return '<db_driver>' end
}
ffi.metatype("db_driver_t", dbmeta)


local db_conn = ffi.typeof('db_conn_t *')
local CI = {}

function CI.prepare(self, sql)
    local stmt = ffi.C.db_prepare(self, sql)
    return ffi.gc(stmt, ffi.C.db_stmt_destroy) 
end


function CI.close(self)
    ffi.C.db_disconnect(self)
end


local cmeta = {
    __index = CI,
    __gc = ffi.C.db_conn_destroy,
    __tostring = function() return '<db_conn>' end
}
ffi.metatype("db_conn_t", cmeta)


local db_stmt = ffi.typeof('db_stmt_t *')
local db_param = ffi.typeof('db_param_t[?]')
local SI = {}

function SI.bindParam(self, ...)

    local pcap = select(#, ...)
    if(pcap < 1) then
        error("The number of parameter bindings is at least 1")
    end

    local binds = ffi.new(db_param, pcap)

    for i, param in pairs(...)
    do
        binds[i].value = param.value
        binds[i].len = param.len
        binds[i].len_max = param.len_max
        binds[i].type = param.type
        binds[i].iot = param.iotype
    end

    ffi.C.db_bind_parameter(self, binds, pcap)
end


function SI.execute(self)
    ffi.C.db_execute(self) 
end


function SI.close(self)
    ffi.C.db_stmt_close(self);
end


local smeta = {
    __index = SI,
    __tostring = function() return '<db_stmt>' end
}
ffi.metatype("db_stmt_t", smeta)


