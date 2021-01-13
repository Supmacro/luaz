

local ffi = require("ffi")

ffi.cdef [[

    typedef struct{
        void   *value;
        int    *len;
        int     len_max;

        int     iot;
        int     rcode;
        short   type;
    
    }db_param_t;

    typedef struct{
        int  (*drv_connect)(void *);
        int  (*drv_prepare)(void *, char *);
        int  (*drv_bindparam)(void *, db_param_t *, int);
        int  (*drv_execute)(void *);
        int  (*drv_freestmt)(void *);

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
        int      dbsize;

        short    vcap;
        op_t     ops;

        db_value_t  *dbval;

    }db_driver_t;

    typedef struct{
        void    *pdb;
        short    Isdown;

        void    *connect;
        int      concap;

    }db_conn_t;

    typedef struct{
        void    *pdb;
        char    *sql;

        void    *connect;
        int      concap;

        short   Ispre;
        short   Isclose;

    }db_stmt_t;

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
local driver = {}

function sql.driver(driver_name)
    local db = ffi.C.db_loaded(driver_name)
    return ffi.gc(db, ffi.C.db_destroy)
end

function driver.connect(self)
    local conn = ffi.C.db_connect(self)
    return ffi.gc(conn, ffi.C.db_conn_destroy)
end

function driver.name(self)
    return ffi.string(self.lname)
end

--driver metamethod
local dbmeta = {
    __index = driver,
    __gc = ffi.C.db_destroy,
    __tostring = function() return '<db_driver>' end
}

ffi.metatype("db_driver_t", dbmeta)


--driver connect
local db_conn = ffi.typeof('db_conn_t *')
local connect = {}

function connect.prepare(self, query)
    local stmt = ffi.C.db_prepare(self, query)
    return ffi.gc(stmt, ffi.C.db_stmt_destroy) 
end

function connect.close(self)
    ffi.C.db_disconnect(self)
end

local cmeta = {
    __index = connect,
    __gc = ffi.C.db_conn_destroy,
    __tostring = function() return '<db_conn>' end
}

ffi.metatype("db_conn_t", cmeta)


--driver statement
local db_stmt = ffi.typeof('db_stmt_t *')
local stmt = {}


local pmeta = {}

function pmeta.set(self, value, ...)

    local t = sql.type
   
    if value == nil then
        self.data_len[0] = 0
        return 
    end

    if self.type == t.CHAR 
    then
        local len = #value
        self.data_len[0] = len == 0 and 1 or len 

        if len ~= 0 then
            ffi.copy(self.value, value, self.data_len[0])
        end
    elseif self.type == t.TINYINT or
           self.type == t.SMALLINT or
           self.type == t.INTEGER or
           self.type == t.BIGINT 
    then
        self.data_len[0] = 8
        self.value[0] = value
    elseif self.type == t.FLOAT or
           self.type == t.DOUBLE 
    then
        self.data_len[0] = 8
        self.value[0] = value
    elseif self.type == t.BOOLEAN 
    then
        self.data_len[0] = 1
        self.value[0] = value
    else
        local cap = select("#", ...)
        assert(cap ~= 1, "Missing parameter identifying data length")

        for i, v in pairs{...} do
            if type(v) ~= "integer" then
                error("The data type of the parameter is wrong")
            end

            self.data_len[0] = v > self.max_len and self.max_len or v
            ffi.copy(self.value, value, self.data_len[0]) 
        end
    end
    
end

pmeta.__index = pmeta
pmeta.__tostring = function() return '<param>' end

function stmt.bind_create(self, btype, max_len, ...)

    local t, ptype = sql.type, sql.type.PARAM_INPUT
    local param = setmetatable({}, pmeta)

    assert(select("#", ...) <= 1, "Too many parameters, " ..
              "'bind_create()' can receive 3 parameters at most")
   
    if select("#", ...) == 1 then
        ptype = select(1, ...)
    end

    if not btype or not max_len or not ptype then
        error("bind_create() received an illegal value of 'nil'")
    end

    if sql.map[btype] == t.CHAR 
    then
        param.max_len = max_len
        param.type = t.CHAR
        param.value = ffi.new("char[?]", max_len)
        param.pio = ptype 
    elseif sql.map[btype] == t.TINYINT or
           sql.map[btype] == t.SMALLINT or
           sql.map[btype] == t.INTEGER or
           sql.map[btype] == t.BIGINT 
    then
        param.max_len = 8
        param.type = btype
        param.value = ffi.new("int64_t[1]")
        param.pio = ptype
    elseif sql.map[btype] == t.FLOAT or
           sql.map[btype] == t.DOUBLE 
    then
        param.max_len = 8
        param.type = btype
        param.value = ffi.new("double[1]")
        param.pio = ptype
    elseif sql.map[btype] == t.BOOLEAN 
    then
        param.max_len = 1
        param.type = t.BOOLEAN
        param.value = ffi.new("char[1]")
        param.pio = ptype
    else
        param.max_len = max_len
        param.type = btype
        param.value = ffi.new("char[?]", max_len)
        param.pio = ptype
    end

    param.data_len = ffi.new("int[1]")
    return param

end

function stmt.bind_param(self, ...)

    local pcap = select("#", ...)
    assert(pcap >= 1, "The number of parameter bindings is at least 1")
    
    local params = ffi.new(ffi.typeof('db_param_t[?]'), pcap)
    for i, param in pairs({...}) 
    do
        params[i-1].len_max = param.max_len
        params[i-1].len = param.data_len
        params[i-1].type = param.type
        params[i-1].value = param.value
        params[i-1].iot = param.pio         
    end

    return ffi.C.db_bind_parameter(self, params, pcap)
end

function stmt.execute(self)
    ffi.C.db_execute(self) 
end

function stmt.close(self)
    ffi.C.db_stmt_close(self)
end

local smeta = {
    __index = stmt,
    __tostring = function() return '<db_stmt>' end
}

ffi.metatype("db_stmt_t", smeta)



